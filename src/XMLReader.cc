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

#include "../include/XMLReader.h"
using namespace Woklib;

XMLReader::XMLReader()
{
	depth=0;
	finished = false;
	current_xml_tag = NULL;
	
	p = XML_ParserCreate (NULL);
	
	XML_SetUnknownEncodingHandler(p, XMLReader::enchandler, this);
	XML_SetUserData (p, this);
	XML_SetElementHandler (p, XMLReader::vxt_start, XMLReader::vxt_end);
	XML_SetCharacterDataHandler (p, XMLReader::vxt_contence);
}

XMLReader::~XMLReader()
{
	XML_ParserFree(p);

	if(current_xml_tag)
	{
		while ( depth-- )
		{
			current_xml_tag = static_cast<WokXMLTag *>(current_xml_tag->GetParant());
			std::cout << current_xml_tag << std::endl;
			std::cout << depth << std::endl;
		}
		delete current_xml_tag;
	}
}

WokXMLTag *
XMLReader::GetTag()
{
	if(finished == false )
		return NULL;
	return current_xml_tag;
}

void
XMLReader::Add(std::string data)
{
	if (!XML_Parse (p, data.c_str(), data.size(), 0))
	{
		std::string msg;
		msg = "Parse error at line " ;
		msg += XML_GetCurrentLineNumber (p);
		msg += ":\n";
		msg +=  XML_ErrorString (XML_GetErrorCode (p));
		msg += '\n';
		std::cout << "Error: " << msg << std::endl;
	}
}

namespace Woklib
{
std::istream &
operator>>( std::istream &input, XMLReader &c)
{
	char buffer[200];
	int len = 0;
	
	while(!c.finished && input)
	{
		if(input.read(buffer,20))
			len = 20;
		else
			len = input.gcount();
		
		c.Add(std::string(buffer,len));
	}
	return input;
}
}

/// This sucks but works sometimes
int
XMLReader::enchandler(void *c, const XML_Char *name, XML_Encoding *info)
{
	for ( int i = 0 ; i < 256 ; i++ )
	{
		info->map[i] = i;
	}

	info->convert = NULL;
	info->data = c;
	info->release = NULL;
	
	return 1;
}

void
XMLReader::vxt_start(void *data, const XML_Char *el, const XML_Char **attr)
{
	XMLReader *c;
	c = static_cast <XMLReader *>(data);

	if(c->depth == 0)
		c->current_xml_tag = new WokXMLTag(NULL, el);
	else if (c->depth > 0)
		c->current_xml_tag = &c->current_xml_tag->AddTag(el);
	for (int i = 0; attr[i]; i += 2)
		c->current_xml_tag->AddAttr(attr[i], attr[i+1]);
		
	c->depth++;
}

void
XMLReader::vxt_contence(void *data, const XML_Char *string, int len)
{
	XMLReader *c;
	c = static_cast <XMLReader *>(data);

	std::string str = std::string(string, 0 , len );
	
	c->current_xml_tag->AddText(str);
}

void
XMLReader::vxt_end(void *data, const XML_Char *el)
{
	XMLReader *c;
	c = static_cast <XMLReader *>(data);

	c->depth--;
	if (c->depth == 0)
	{
		c->finished = true;
	}
	else if ( c->depth > 0 )
		c->current_xml_tag = static_cast<WokXMLTag *>(c->current_xml_tag->GetParant());
}
