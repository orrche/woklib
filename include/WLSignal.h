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


#ifndef __WL_SIGNAL
#define __WL_SIGNAL

namespace Woklib
{
class WLSignal;
}

#include "WLSignalData.h"
#include "WLSignalHook.h"
#include <string>
#include <map>

namespace Woklib
{


#define DEBUG 1
#define EXP_SIGHOOK(signame, function, prio) (SignalHook (signame, reinterpret_cast < int (WLSignalInstance::*)(WLSignalData * wlsd) > (function), prio))
#define EXP_SIGUNHOOK(signame, function, prio) (SignalUnHook (signame, reinterpret_cast < int (WLSignalInstance::*)(WLSignalData * wlsd) > (function), prio))

class WLSignal
{
      public:
	WLSignal ();
	~WLSignal ();

	WLSignalContainer *SignalHook(const std::string &signal, WLSignalContainer *wlsc);
	void SignalHook (const char *signal, WLSignalInstance * wlsi, int (WLSignalInstance::*member)(WLSignalData *wlsd), int priority);
	void SignalUnHook(const char *signal, WLSignalInstance * wlsi, int (WLSignalInstance::*member)(WLSignalData *wlsd), int priority );
	  
	int SendSignal (const std::string &signal, WLSignalData & wlsd);
	int SendSignal (const std::string &signal, WLSignalData * wlsd);
	
	int i;
      protected:
	int level;
	std::map <std::string , WLSignalHook> hooks;
};

}
#endif // __WL_SIGNAL
