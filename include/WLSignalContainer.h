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



#ifndef _WLSIGNALCONTAINER_H_
#define _WLSIGNALCONTAINER_H_

namespace Woklib
{
class WLSignalContainer;
}

#include "WLSignalInstance.h"
#include "WLSignalData.h"

namespace Woklib
{


class WLSignalContainer
{
	public:
		WLSignalContainer (WLSignalInstance * wlsi,
			   int (WLSignalInstance::*member) (WLSignalData *
							     wlsd),
			   int priority, std::string signal = "");
		~WLSignalContainer ();

		int run (WLSignalData * wlsd);
		int GetPriority ();
		int (WLSignalInstance::*member) (WLSignalData * wlsd);
		std::string & GetSig();
		const bool operator== (const WLSignalContainer &p);
    protected:
		  
		WLSignalInstance *wlsi;
		int priority;
		std::string signal;
};

}

#endif //_WLSIGNALCONTAINER_H_
