/***************************************************************************
 *  Copyright (C) 2003-2005  Kent Gustavsson <nedo80@gmail.com>
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

#ifndef __XML_READER_H
#define __XML_READER_H

namespace Woklib
{
class XMLReader;
}

#include <errno.h>
#include "WokLib.h"

namespace Woklib
{

#include <expat.h>

class XMLReader
{
	friend std::istream &operator >> ( std::istream &input, XMLReader &c);
	
	public:
		XMLReader();
		~XMLReader();
		
		WokXMLTag* GetTag();
		void Add(std::string data);
		
		static void vxt_start(void *data, const XML_Char *el, const XML_Char **attr);
		static void vxt_contence(void *data, const XML_Char *string, int len);
		static void vxt_end(void *data, const XML_Char *el);
		static int enchandler(void *c, const XML_Char *name, XML_Encoding *info);
	protected:
		XML_Parser p;
		WokXMLTag *current_xml_tag;
		int depth;
		bool finished;

};

}

#endif // __XML_READER_H
