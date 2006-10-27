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


#ifndef __WL_SIGNAL_HOOK
#define __WL_SIGNAL_HOOK

namespace Woklib 
{
class WLSignalHook;
}

#include "WLSignalInstance.h"
#include "WLSignalContainer.h"
#include <map>
#include <list>

namespace Woklib 
{

class WLSignalHook
{
	public:
	WLSignalHook();
	~WLSignalHook();
	int Execute(WLSignalData *wlsd);
	WLSignalContainer * AddInstance(WLSignalContainer *wlsc);
	WLSignalContainer * AddInstance(WLSignalInstance *wlsi, int (WLSignalInstance::*member)(WLSignalData *wlsd), int priority);
	int RemoveInstance(WLSignalInstance *wlsi, int (WLSignalInstance::*member)(WLSignalData *wlsd), int priority);
	private:
	std::map <int , std::list<WLSignalContainer *> > Hooks;
	int exec;
	bool deletion;
};

}

#endif // __WL_SIGNAL_HOOK
