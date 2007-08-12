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



#include "../include/WokXMLText.h"
#include "../include/WokLib.h"
using namespace Woklib;

WokXMLText::WokXMLText(WokXMLObject* parant, const std::string& text) 
: WokXMLObject(parant),
text(text)
{
	
}

WokXMLText::WokXMLText(const WokXMLText &xml_text) 
: WokXMLObject(xml_text.parant)
{
	text = xml_text.text;
}

WokXMLText::~WokXMLText()
{

}

const std::string
WokXMLText::GetStr()
{
	return XMLisize(text);
}

int
WokXMLText::GetType()
{
	return 2;
}

const std::string&
WokXMLText::GetText()
{
	return text;
}

void
WokXMLText::Print(std::ostream &output)
{
	output << *this;
}

bool WokXMLText::operator == ( WokXMLText &wxt)
{
	return(wxt.text == text);
}

namespace Woklib
{
std::ostream &
operator<<( std::ostream &output, const WokXMLText &s)
{
	output << XMLisize(s.text);	
	return output;
}
}
