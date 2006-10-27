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


#ifndef _WOKXMLTEXT_H_
#define _WOKXMLTEXT_H_


#include "WokXMLObject.h"
#include <string>

namespace Woklib
{


class WokXMLText : public WokXMLObject
{
	friend std::ostream &operator << ( std::ostream &output, const WokXMLText &s);
	
	public:
		WokXMLText(WokXMLObject* parant, const std::string& text);
		WokXMLText(const WokXMLText &xml_text);
		 virtual ~WokXMLText();
		
		const std::string GetStr();
		const std::string& GetText();
		int GetType();
		void Print(std::ostream &output);
		bool operator == ( WokXMLText &wxt);

	protected:
		std::string text;
};

}

#endif	//_WOKXMLTEXT_H_
