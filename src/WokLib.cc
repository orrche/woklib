/***************************************************************************
 *  Copyright (C) 2003-2007  Kent Gustavsson <nedo80@gmail.com>
 ****************************************************************************/
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#include "include/WokLib.h"
using namespace Woklib;

#include "include/WokLibSignal.h"
#include "include/WoklibPlugin.h"
#ifdef __WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif
#include <iostream>
#include <time.h>
#include <stdlib.h>

WokLib::WokLib () :
WLSignalInstance(&wls_main)
{
	EXP_SIGHOOK("Woklib Socket In Add", &WokLib::AddListener, 1000);
	EXP_SIGHOOK("Woklib Plugin Add", &WokLib::AddPlugin, 1000);
	EXP_SIGHOOK("Woklib Plugin Remove", &WokLib::RemovePlugin, 1000);
	EXP_SIGHOOK("Woklib Plugin Get", &WokLib::GetLoadedPlugins, 1000);
	EXP_SIGHOOK("Woklib Timmer Add", &WokLib::AddTimer, 1000);
}

WokLib::~WokLib ()
{
	std::map<std::string , WoklibPlugin *>::iterator piter; //plugins;
	for( piter = plugins.begin() ; piter != plugins.end(); piter = plugins.begin())
	{
		UnLoadPlugin(piter->first);
	}

	EXP_SIGUNHOOK("Woklib Socket In Add", &WokLib::AddListener, 1000);
	EXP_SIGUNHOOK("Woklib Plugin Add", &WokLib::AddPlugin, 1000);
	EXP_SIGUNHOOK("Woklib Plugin Remove", &WokLib::RemovePlugin, 1000);
	EXP_SIGUNHOOK("Woklib Plugin Get", &WokLib::GetLoadedPlugins, 1000);
	EXP_SIGUNHOOK("Woklib Timmer Add", &WokLib::AddTimer, 1000);
}


int
WokLib::GetSocket ()
{
	return 0;
}


int
WokLib::RemovePlugin( WokXMLTag *tag )
{
	UnLoadPlugin(tag->GetAttr("filename"));
}

int
WokLib::AddPlugin( WokXMLTag *tag )
{
	if( LoadPlugIn(tag->GetAttr("filename")))
		tag->AddAttr("outcome", "good");
	else
		tag->AddAttr("outcome", "bad");
	return 1;
}

int
WokLib::LoadPlugIn (std::string filename)
{
	WoklibPlugin *plugin;
	WoklibPlugin *(*mkr) (WLSignal *wls);

#ifdef __WIN32
    HINSTANCE hLib=LoadLibrary(filename.c_str());
    if (!hLib)
	{
		woklib_error(wls, "Cannot load library");
		return 0;
	}

	mkr = (WoklibPlugin *(*)(WLSignal *wls)) GetProcAddress((HMODULE)hLib, "maker");

    if ( mkr == NULL )
    {
        std::cout << "Unable to load the function" << std::endl;
        return 0;
    }

	plugin = mkr (wls);

	pluginhandle[filename] = hLib;
	plugins[filename] = plugin;

#else
	void *PlugInLib = dlopen (filename.c_str (), RTLD_LOCAL|RTLD_LAZY);
	if (!PlugInLib)
	{
//		woklib_error(wls, "Cannot load library: " + std::string(dlerror()));
		std::cerr << "Cannot load library: " << dlerror() << std::endl;
		return 0;
	}

	mkr = (WoklibPlugin *(*)(WLSignal *wls)) dlsym (PlugInLib, "maker");

	plugin = mkr (wls);

	pluginhandle[filename] = PlugInLib;
	plugins[filename] = plugin;
#endif
	return 1;
}

int
WokLib::UnLoadPlugin(std::string filename)
{
	if ( plugins.find(filename) == plugins.end() )
		return false;

#ifdef __WIN32

	((WoklibPlugin *(*)(WoklibPlugin *)) GetProcAddress((HMODULE)pluginhandle[filename], "maker"))(plugins[filename]);
	plugins.erase(filename);

	FreeLibrary((HMODULE)pluginhandle[filename]);
	pluginhandle.erase(filename);

#else
	((WoklibPlugin *(*)(WoklibPlugin *)) dlsym (pluginhandle[filename], "destroyer"))(plugins[filename]);
	plugins.erase(filename);

	dlclose(pluginhandle[filename]);
	pluginhandle.erase(filename);

#endif
	return true;
}

int
WokLib::GetLoadedPlugins(WokXMLTag *tag)
{
	std::map<std::string , WoklibPlugin *>::iterator iter;

	for( iter = plugins.begin(); iter != plugins.end() ; iter++)
	{
		WokXMLTag& plugin_tag = tag->AddTag("plugin");
		plugin_tag.AddTag("filename").AddText(iter->first);
		plugin_tag.AddTag("info").AddText(iter->second->GetInfo());
		plugin_tag.AddTag("version").AddText(iter->second->GetVersion());
	}

	return 1;
}

int
WokLib::AddListener( WokXMLTag *xml )
{
	static int id = 0;
	id++;

	char buf[20];
	sprintf(buf, "Socket Active %d", id);
	sockets[atoi(xml->GetAttr("socket").c_str())] = buf;
	xml->AddAttr("signal", buf);

	return 1;
}

int
WokLib::AddTimer( WokXMLTag *xml )
{
	Timer *t = new Timer(wls, xml);
	timer_instance.push_back(t);
	int ti;
	ti = static_cast <int> (t->GetDelay() + time(0));
	while ( timer_timing.find(ti) != timer_timing.end() )
		ti++;
	timer_timing[ti] = t;
	return 1;
}

int
WokLib::main ()
{
#ifdef __WIN32
#else
	std::map < int, std::string>::iterator iter;
	struct timeval tv;
	int max_socket;
	int ret;
	int sleep_time;
	fd_set inp;

	
	for (;;)
	{
		if ( !timer_instance.empty())
			sleep_time = timer_timing.begin()->first - time(0);
		else
			sleep_time = 100;

		if ( sockets.empty() )
		{

			sleep(sleep_time);
		}
		else
		{
			tv.tv_sec = sleep_time;
			tv.tv_usec = 0;
			max_socket = 0;
			
			FD_ZERO (&inp);

			for (iter = sockets.begin (); iter != sockets.end (); iter++)
			{
				if (max_socket < iter->first)
					max_socket = iter->first;
				FD_SET (iter->first, &inp);
			}

			max_socket++;

			ret = select (max_socket, &inp, NULL, NULL, &tv);

			if ((ret == -1) && (errno == EINTR))
			{
				continue;
			}

			if ((ret == -1))
			{
				perror ("Not a connected socket");
				return -1;
			}

			for (iter = sockets.begin (); iter != sockets.end (); iter++)
			{
				if (FD_ISSET (iter->first, &inp))
				{
					WokXMLTag sigtag(NULL, "read");
					wls->SendSignal(iter->second, sigtag);

					if (!sigtag.GetAttr("error").empty())
					{
						sockets.erase ( iter );
						break;
						//iter = sockets.begin();
					}
				}
			}
		}

		while ( !timer_instance.empty() && timer_timing.begin()->first <= time(0) )
		{
			Timer *t = timer_timing.begin()->second;
			timer_timing.erase(timer_timing.begin());

			WokXMLTag tag(NULL, "time");
			if ( wls->SendSignal(t->GetSignal(), tag) == 0 || !tag.GetAttr("stop").empty())
			{
				timer_instance.remove(t);
				delete t;
			}
			else
			{
				int ti;
				ti = static_cast <int> (t->GetDelay() + time(0));
				while ( timer_timing.find(ti) != timer_timing.end() )
					ti++;
				timer_timing[ti] = t;
			}
		}
	}

#endif
}


namespace Woklib
{
/* XML'isise string remove <, >, & , ", 'and replace with &lt;, &gt;, &amp;, &quot;, &apos;   *
 * Thanks to waqas in jabber@conference.jabber.org for helping me correct this and complete it */
std::string
XMLisize(const std::string &str)
{
	std::string retstr;

	for( int i = 0 ; i < str.size() ; i++)
	{
		if( str[i] == '<' )
			retstr += "&lt;";
		else if( str[i] == '>' )
			retstr += "&gt;";
		else if( str[i] == '&' )
			retstr += "&amp;";
		else if( str[i] == '"' )
			retstr += "&quot;";
		else if( str[i] == '\'' )
			retstr += "&apos;";
		else
			retstr += str[i];
	}

	return retstr;
}

std::string
DeXMLisize(const std::string &str)
{
	std::string retstr = str;

	for( int i = 0 ; i < retstr.size() ; i++ )
	{
		if ( retstr[i] == '&' )
		{
			if (retstr.substr(i,4) == "&lt;" )
				retstr.replace(i,4, "<");
			else if (retstr.substr(i,4) == "&gt;" )
				retstr.replace(i,4, ">");
			else if (retstr.substr(i,5) == "&amp;" )
				retstr.replace(i,5, "&");
			else if (retstr.substr(i,6) == "&quot;" )
				retstr.replace(i,6, "\"");
			else if (retstr.substr(i,6) == "&apos;" )
				retstr.replace(i,6, "\'");

		}
	}
	return retstr;
}

}
