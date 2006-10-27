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


#include "include/WLSignalContainer.h"
#include <iostream>

using namespace Woklib;


WLSignalContainer::WLSignalContainer(WLSignalInstance *wlsi,  int (WLSignalInstance::*member)(WLSignalData *wlsd), int priority, std::string signal):
member(member),
wlsi(wlsi),
priority(priority),
signal(signal)
{
}


WLSignalContainer::~WLSignalContainer()
{
}

int
WLSignalContainer::run(WLSignalData * wlsd)
{
	return ( (wlsi->*member)(wlsd) );
}

int
WLSignalContainer::GetPriority()
{
	return(priority);
}

const bool 
WLSignalContainer::operator== (const WLSignalContainer &p)
{
	return (member == p.member && wlsi == p.wlsi && priority == p.priority);	
}


std::string &
WLSignalContainer::GetSig()
{
	return signal;
}
