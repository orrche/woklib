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


#ifndef _WOKXMLOBJECT_H_
#define _WOKXMLOBJECT_H_


#include <iostream>
#include "WLSignal.h"

namespace Woklib
{


class WokXMLObject : public WLSignalData
{
	friend std::ostream &operator << ( std::ostream &output, WokXMLObject &s);
	public:
		WokXMLObject(WokXMLObject* parant);
		WokXMLObject(const WokXMLObject &obj);
		virtual ~WokXMLObject();
		virtual int GetType();
		WokXMLObject* GetParant();
		
		virtual const std::string GetStr() {return "";};
	
		virtual void Print(std::ostream &output);
		virtual bool operator == ( WokXMLObject &obj);

	protected:
		WokXMLObject* parant;
};

}

#endif	//_WOKXMLOBJECT_H_
