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

#include "include/WLSignalInstance.h"
#include <iostream>
using namespace Woklib;

WLSignalInstance::WLSignalInstance(WLSignal *wls)
{
	this->wls = wls;
}
	
WLSignalInstance::~WLSignalInstance()
{
	while(!signals.empty())
	{
		std::string signame = (*signals.begin())->GetSig();
		wls->SignalUnHook( signame.c_str() , this, reinterpret_cast < int (WLSignalInstance::*)(WLSignalData * wlsd) > ((*signals.begin())->member), (*signals.begin())->GetPriority());
			// Gets deleted in WLSignalHook.cc
		signals.pop_front();
		
	}
}

void
WLSignalInstance::SignalHook(std::string name, int (WLSignalInstance::*member)(WLSignalData *wlsd), int prio)
{
	WLSignalContainer *wlsc = new WLSignalContainer(this, member, prio, name);
	wls->SignalHook (name, wlsc);
	signals.push_back(wlsc);
	
}

void
WLSignalInstance::SignalUnHook(std::string name, int (WLSignalInstance::*member)(WLSignalData *wlsd), int prio)
{
	WLSignalContainer compto(this, member, prio);
	std::list < WLSignalContainer *>::iterator sigiter;
	for(sigiter = signals.begin(); sigiter != signals.end(); sigiter++)
	{
		if( **sigiter == compto )
		{
			// Gets deleted in WLSignalHook.cc
			signals.erase(sigiter);
			break;
		}
	}
	wls->SignalUnHook(name.c_str(), this, member, prio);
}
