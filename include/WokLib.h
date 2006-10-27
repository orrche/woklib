/***************************************************************************
 *  Copyright (C) 2003-2004  Kent Gustavsson <oden@gmx.net>
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


#ifndef __WOKLIB_H
#define __WOKLIB_H

namespace Woklib
{
class WokLib;
}

#include "Timer.h"
#include "WLSignal.h"
#include "WokXMLTag.h"
#include "WoklibPlugin.h"
#include "Messages.h"
	
#include <string>
#include <list>
#include <map>

namespace Woklib 
{


class WokLib : public WLSignalInstance
{
	public:
	WokLib();
	~WokLib();

	int main();

	int GetSocket();
	WLSignal wls_main;
	int ReadData(WokXMLTag *xml);
	int AddListener( WokXMLTag *xml );
	int AddPlugin(WokXMLTag *tag );
	int RemovePlugin( WokXMLTag *tag );
	int GetLoadedPlugins(WokXMLTag *tag);
	int AddTimer( WokXMLTag *tag );

	int LoadPlugIn(std::string filename);
	int UnLoadPlugin(std::string filename);
	
	private:
	std::map<int, std::string> sockets;
	std::map<std::string , WoklibPlugin *> plugins;
	std::map<std::string , void *> pluginhandle;
	
	std::map<int, Woklib::Timer *> timer_timing;
	std::list<Timer *> timer_instance;
};

extern std::string XMLisize(const std::string &str);
extern std::string DeXMLisize(const std::string &str);
}

#endif // _WOKLIB_H
