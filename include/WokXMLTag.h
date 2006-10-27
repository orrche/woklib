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



#ifndef _WOKXMLTAG_H_
#define _WOKXMLTAG_H_


#include <list>
#include <map>
#include <string>
#include <iostream>

namespace Woklib
{
class WokXMLTag;
}

#include "WokXMLObject.h"
#include "WokXMLText.h"
#include "XMLReader.h"

namespace Woklib
{

std::ostream &operator << ( std::ostream &output, WokXMLTag &s);
std::istream &operator >> ( std::istream &input, WokXMLTag &s);


class WokXMLTag : public WokXMLObject
{
	friend std::ostream &operator << ( std::ostream &output, WokXMLTag &s);
	friend std::istream &operator >> ( std::istream &input, WokXMLTag &s);
	
	public:
		WokXMLTag(WokXMLTag* parant, const std::string& name);
		WokXMLTag(const std::string& name);

		WokXMLTag(WokXMLTag& tag);
		 virtual ~WokXMLTag();
	
		void AddText(const std::string& name);
		WokXMLTag& AddTag(const std::string& name);
		void AddText(WokXMLText *);
		WokXMLTag& AddTag(WokXMLTag *);
		void RemoveTag(WokXMLTag *);
		void AddAttr(std::string name, std::string value);
		void RemoveAttr(std::string name);
		void AddObject(WokXMLObject *obj);

		const std::string& GetBody();
		const std::string& GetName();
		const std::string GetChildrenStr();
		const std::string GetStr();
		WokXMLTag &GetFirstTag(const std::string& name);
		std::list <WokXMLTag *>& GetTags();
		std::list <WokXMLObject *>& GetItemList();
		std::list <WokXMLTag *>& GetTagList(const std::string& name);
		const std::string& GetAttr(const std::string& name);
	
		bool In(WokXMLTag &tag);
		void RemoveChildrenTags();
		void RemoveBody();
		int GetType();
		void Print(std::ostream &output);

		bool operator == ( WokXMLTag &event);

	protected:
		XMLReader *xr;
		
		std::string body;
		std::string name;
		std::string text;
		std::list <WokXMLTag *> tags;
		std::list <WokXMLObject *> object_list;
		std::map <std::string, std::string> arg_list;
		std::map <std::string, std::list<WokXMLTag *> > object_reference;
};

}

#endif	//_WOKXMLTAG_H_
