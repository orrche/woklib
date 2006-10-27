/***************************************************************************
 *  Copyright (C) 2003-2005  Kent Gustavsson <oden@gmx.net>
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


#include "include/WLSignal.h"
#include "include/WokXMLTag.h"
#include <iostream>

using std::cerr;
using std::endl;
using namespace Woklib;

WLSignal::WLSignal ()
{
	i = 1;
	level = 0;
}


WLSignal::~WLSignal ()
{
}

WLSignalContainer *
WLSignal::SignalHook(const std::string &signal, WLSignalContainer *wlsc)
{
	return(hooks[signal].AddInstance(wlsc));
}
void 
WLSignal::SignalHook (const char *signal, WLSignalInstance * wlsi, int (WLSignalInstance::*member)(WLSignalData *wlsd), int priority)
{
	SignalHook(signal, new WLSignalContainer(wlsi, member, priority));
}
	
void
WLSignal::SignalUnHook(const char *signal, WLSignalInstance * wlsi, int (WLSignalInstance::*member)(WLSignalData *wlsd), int priority )
{
	if(!hooks[signal].RemoveInstance(wlsi,member,priority))
		hooks.erase(signal);
}

int
WLSignal::SendSignal (const std::string &signal, WLSignalData &wlsd)
{
	return ( SendSignal(signal, &wlsd) );	
}

int
WLSignal::SendSignal (const std::string &signal, WLSignalData * wlsd)
{
	if(hooks.find(signal) == hooks.end() )
	{
#ifdef DEBUG
		if(signal != "Display Debug")
		{
			WokXMLTag sigtag(NULL, "message");
			sigtag.AddTag("body").AddText(signal + " Not hooked");
			SendSignal("Display Debug", sigtag);
		}
#endif
		if(signal != "Display Signal" && signal != "Display Debug")
		{
			WokXMLTag sigtag(NULL, "signal");
			sigtag.AddAttr("name", signal);
			
			if( dynamic_cast<WokXMLTag *> ( wlsd ) )
				sigtag.AddTag(static_cast<WokXMLTag *> (wlsd));
			else
			{
				WokXMLTag oldtag(NULL, "message");
				oldtag.AddTag("body").AddText(signal + " is using non xml structure data");
				SendSignal("Display Error", oldtag);
			}
			
			SendSignal("Display Signal", sigtag);
		}
		return 0;
	}
	
	int run;
	if( (run = hooks[signal].Execute(wlsd)) < 0 )
	{	
		hooks.erase(signal);
		run = -run;
	}
		
#ifdef DEBUG
	if(signal != "Display Signal" && signal != "Display Debug")
	{
		WokXMLTag sigtag(NULL, "signal");
		sigtag.AddAttr("name", signal);
		
		if( dynamic_cast<WokXMLTag *> ( wlsd ) )
			sigtag.AddTag(static_cast<WokXMLTag *> (wlsd));
		else
		{
			WokXMLTag oldtag(NULL, "message");
			oldtag.AddTag("body").AddText(signal + " is using non xml structure data");
			SendSignal("Display Error", oldtag);
		}
		
		SendSignal("Display Signal", sigtag);
	}
#endif // DEBUG

	return run;
}
