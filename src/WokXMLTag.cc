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

#include <sstream>

#include "../include/WokXMLTag.h"
#include "../include/WokXMLText.h"
using namespace Woklib;
const char* base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

WokXMLTag::WokXMLTag(WokXMLTag* parant, const std::string& name)
: WokXMLObject(parant),
name(name)
{
	xr = NULL;
}

WokXMLTag::WokXMLTag(WokXMLTag* parant, const std::string& name, const std::string& ns)
: WokXMLObject(parant),
name(name)
{
	arg_list["xmlns"] = ns;
	xr = NULL;
}

WokXMLTag::WokXMLTag(const std::string& name, const std::string& ns)
: WokXMLObject(NULL),
name(name)
{
	arg_list["xmlns"] = ns;
	xr = NULL;
}

WokXMLTag::WokXMLTag(const std::string& name)
: WokXMLObject(NULL),
name(name)
{
	xr = NULL;
}

WokXMLTag::WokXMLTag(WokXMLTag& tag)
: WokXMLObject(tag.parant)
{
	name = tag.name;
	arg_list = tag.arg_list;

	std::list <WokXMLObject *>::iterator iter;

	for( iter = tag.object_list.begin() ; iter != tag.object_list.end(); iter++)
		AddObject(*iter);

	xr = NULL;
}


WokXMLTag::~WokXMLTag()
{
	std::list <WokXMLObject *>::iterator iter;

	for(iter = object_list.begin(); iter != object_list.end(); iter++)
		delete *iter;


	if( xr )
		delete xr;
}

void
WokXMLTag::AddObject(WokXMLObject *obj)
{
	switch(obj->GetType())
	{
		case 0:
			break;
		case 1:
			AddTag(reinterpret_cast <WokXMLTag *>(obj));
			break;
		case 2:
			AddText(reinterpret_cast <WokXMLText *>(obj));
			break;
	}
}

void
WokXMLTag::AddData(std::string buf)
{
	std::string ret("");
	int n = 0;

	for ( int i = static_cast<int>((2.99 + buf.size())/3) ; i ; i-- )
	{
		int data=0;

		if( buf.size() - n*3 == 1 )
			data = buf[n*3]*256*256;
		else if ( buf.size() - n*3 == 2 )
			data = buf[n*3+1]*256 + buf[n*3]*256*256;
		else
			data = buf[n*3+2] + buf[n*3+1]*256 + buf[n*3]*256*256;

		if( n * 3 > buf.size() )
		{
			ret += "=";
			ret += "=";
		}
		else
		{
			ret += base64char[(data/64/64/64)%64];
			ret += base64char[(data/64/64)%64];
		}
		if( n * 3 + 2 > buf.size() )
			ret += "=";
		else
		{
			ret += base64char[(data/64)%64];
		}
		if( n * 3 + 3 > buf.size() )
			ret += "=";
		else
		{
			ret += base64char[data%64];
		}

		n++;
	}
	AddText(ret);
}

void
WokXMLTag::AddText(const std::string &text)
{
	WokXMLText* tmp = new WokXMLText(this, text);
	object_list.push_back(tmp);
	body += text;
}

void
WokXMLTag::AddText(WokXMLText *t)
{
	WokXMLText *tmp = new WokXMLText(*t);
	object_list.push_back(tmp);
	body += t->GetText();
}

WokXMLTag&
WokXMLTag::AddTag(const std::string &tagname, const std::string &ns)
{
	WokXMLTag *tmp = new WokXMLTag(this, tagname, ns);
	
	object_list.push_back(tmp);
	object_reference[tagname].push_back(tmp);
	namespace_list[ns].push_back(tmp);
	namespace_reference[tagname][ns].push_back(tmp);
	tags.push_back(tmp);

	return *tmp;
}

WokXMLTag&
WokXMLTag::AddTag(const std::string &tagname)
{
	WokXMLTag *tmp = new WokXMLTag(this, tagname);

	object_list.push_back(tmp);
	object_reference[tagname].push_back(tmp);
	namespace_list[""].push_back(tmp);
	namespace_reference[tagname][""].push_back(tmp);
	tags.push_back(tmp);

	return *tmp;
}

WokXMLTag&
WokXMLTag::AddTag(WokXMLTag *tag)
{
	WokXMLTag *t = new WokXMLTag(*tag);
	t->parant = this;
	object_list.push_back(t);
	object_reference[t->GetName()].push_back(t);
	namespace_list[t->GetAttr("xmlns")].push_back(t);
	namespace_reference[t->GetName()][t->GetAttr("xmlns")].push_back(t);
	tags.push_back(t);

	return *t;
}

void
WokXMLTag::RemoveTag(WokXMLTag *tag)
{
	std::list <WokXMLTag *>::iterator tagiter;
	tagiter = find(tags.begin(), tags.end(), tag); // Search the list.
	if (tagiter != tags.end())
	{
		tags.erase(tagiter);
	}

	std::list <WokXMLObject *>::iterator objiter;
	objiter = find(object_list.begin(), object_list.end(), static_cast<WokXMLObject*>(tag)); // Search the list.
	if (objiter != object_list.end())
	{
		object_list.erase(objiter);
	}

	tagiter = find(object_reference[tag->GetName()].begin(), object_reference[tag->GetName()].end(), tag); // Search the list.
	if (tagiter != object_reference[tag->GetName()].end())
	{
		object_reference[tag->GetName()].erase(tagiter);
	}

	tagiter = find(namespace_list[tag->GetAttr("xmlns")].begin(), namespace_list[tag->GetAttr("xmlns")].end(), tag);
	if ( tagiter != namespace_list[tag->GetAttr("xmlns")].end() )
		namespace_list[tag->GetAttr("xmlns")].erase(tagiter);
	
	tagiter = find(namespace_reference[tag->GetAttr("name")][tag->GetAttr("xmlns")].begin(), namespace_reference[tag->GetAttr("name")][tag->GetAttr("xmlns")].end(), tag);
	if ( tagiter != namespace_reference[tag->GetAttr("name")][tag->GetAttr("xmlns")].end() )
		namespace_reference[tag->GetAttr("name")][tag->GetAttr("xmlns")].erase(tagiter);

	tag->parant = NULL;
}

void
WokXMLTag::ChangeXMLNSFor(WokXMLTag *child, std::string name)
{
	std::string old = child->GetAttr("xmlns");
	std::list <WokXMLTag *>::iterator tagiter;
	
	std::cout << "So we crash here... this is a really odd place for a crash tho I think " << std::endl;
	std::cout << old << " <- " << name << std::endl;
	tagiter = find(namespace_list[old].begin(), namespace_list[old].end(), child);
	std::cout << "Its soo clean that there shouldn't be any errors here.." << std::endl;
	
	if ( tagiter != namespace_list[child->GetAttr("xmlns")].end() )
		namespace_list[child->GetAttr("xmlns")].erase(tagiter);
	
	tagiter = find(namespace_reference[child->GetAttr("name")][child->GetAttr("xmlns")].begin(), namespace_reference[child->GetAttr("name")][child->GetAttr("xmlns")].end(), child);
	if ( tagiter != namespace_reference[child->GetAttr("name")][child->GetAttr("xmlns")].end() )
		namespace_reference[child->GetAttr("name")][child->GetAttr("xmlns")].erase(tagiter);

	
	namespace_list[name].push_back(child);
	namespace_reference[child->GetName()][old].push_back(child);
	
}

void
WokXMLTag::AddAttr(std::string name, std::string value)
{
	if ( name == "xmlns" )
	{
		if ( parant )	
			static_cast <WokXMLTag *>(parant)->ChangeXMLNSFor(this, value);		
	}
	arg_list[name] = value;
}

void
WokXMLTag::RemoveAttr(std::string name)
{
	if( name == "xmlns" )
	{
		static_cast <WokXMLTag *>(parant)->ChangeXMLNSFor(this, "");		
	}
	arg_list.erase(name);
}

const std::string&
WokXMLTag::GetName()
{
	return name;
}

std::list <WokXMLObject *> &
WokXMLTag::GetItemList()
{
	return object_list;
}

std::list<WokXMLTag *>&
WokXMLTag::GetTagList(const std::string& name)
{
	return object_reference[name];
}

std::list<WokXMLTag *>&
WokXMLTag::GetTags()
{
	return tags;
}

void
WokXMLTag::Add(std::string data)
{
	if(!xr)
		xr = new XMLReader();

	xr->Add(data);

	if(!xr->GetTag())
		return;

	AddTag(xr->GetTag());

	delete xr;
	xr = NULL;

	return;
}

char *
WokXMLTag::GetBodyAsBase64(char *buffer, int size)
{
	std::string data = GetBody();
	std::stringstream file;
	int origsize = size;

	unsigned int c = 0;
	int pos = 0;
	unsigned int tmp;
	bool ended = false;
	for ( int i = 0 ; i < data.size() ; i++)
	{
		if ( data[i] <= 'z' && data[i] >= 'a' )
		{
			tmp = data[i] - 'a' + 26;
		}
		else if ( data[i] <= 'Z' && data[i] >= 'A' )
		{
			tmp = data[i] - 'A';
		}
		else if ( data[i] >= '0' && data[i] <= '9' )
		{
			tmp = data[i] - '0' + 52;
		}
		else if ( data[i] == '+' )
		{
			tmp = 62;
		}
		else if ( data[i] == '/' )
		{
			tmp = 63;
		}
		else if ( data[i] == '=' )
		{
			ended = true;
			tmp = 0;
		}
		else
		{
//			woklib_debug(wls, "base64: bad data");
			continue;
		}

		switch (pos)
		{
			case 0:
				tmp *= 64 * 64 * 64;
				break;
			case 1:
				tmp *= 64 * 64;
				break;
			case 2:
				tmp *= 64;
				break;
		}

		c += tmp;
		if ( pos == 3 || ended)
		{
			if ( ended )
			{
				pos--;
			}

			int spos = 2;
			for(;pos;pos--)
			{
				size--;
				if ( !size )
				{
					ended = true;
					break;
				}
				file << static_cast <unsigned char> ((c>>(spos--*8)) & 0xFF);
			}

			c = 0;
			pos = 0;
		}
		else
			pos++;
		if (ended)
			break;
	}

	memcpy(buffer, file.str().c_str(), origsize);
	return buffer;
}

const std::string&
WokXMLTag::GetBody()
{
	return body;
}

const std::string
WokXMLTag::GetChildrenStr()
{
	std::string ret;

	std::map < std::string, std::string >::iterator arg_iter;
    std::list <WokXMLObject *>::iterator object_iter;

	ret = "";
	for(object_iter = object_list.begin(); object_iter != object_list.end(); object_iter++)
	{
		ret += (*object_iter)->GetStr();
	}
	return ret;
}

void
WokXMLTag::RemoveBody()
{
	std::list <WokXMLObject *>::iterator iter;


	for( iter = object_list.begin(); iter != object_list.end();)
	{
		if( (*iter)->GetType() == 2)
		{
			delete *iter;
			object_list.erase(iter);
			iter = object_list.begin();

			continue;
		}
		iter++;
	}
	body = "";
}

void
WokXMLTag::RemoveChildrenTags()
{
	std::list <WokXMLTag *>::iterator iter;

	for(iter = tags.begin(); iter != tags.end(); iter++)
	{
		object_list.remove(*iter);
		delete *iter;
	}


	tags.clear();
	object_reference.clear();
	namespace_list.clear();
	namespace_reference.clear();
}

const std::string
WokXMLTag::GetStr()
{
	std::string ret;

	std::map < std::string, std::string >::iterator arg_iter;
  std::list <WokXMLObject *>::iterator object_iter;

	ret = '<' + name;
	for(arg_iter = arg_list.begin();arg_iter != arg_list.end();arg_iter++)
	{
		ret += ' ' + arg_iter->first + "='" + XMLisize(arg_iter->second) + '\'';
	}
	ret += ">";
	for(object_iter = object_list.begin(); object_iter != object_list.end(); object_iter++)
	{
		ret += (*object_iter)->GetStr();
	}
	ret += "</" + name + ">";
	return ret;
}

const std::string&
WokXMLTag::GetNS()
{
	if ( GetAttr("xmlns").empty() )
	{
		if ( parant )
			return static_cast<WokXMLTag*>(parant)->GetNS();
	}
	else
		return GetAttr("xmlns");
	
	return empty_str;
}

const std::string&
WokXMLTag::GetAttr(const std::string& name)
{
	if ( arg_list.find(name) != arg_list.end() )
		return arg_list[name];
	return empty_str;
}

WokXMLTag &
WokXMLTag::GetFirstTag(const std::string& name, const std::string& ns)
{
	if ( namespace_reference.find(name) != namespace_reference.end() )
	{
		if ( namespace_reference[name].find(ns) != namespace_reference[name].end() )
		{
			return **namespace_reference[name][ns].begin();
		}
		else
			return AddTag(name,ns);
	}
	else
		return AddTag(name,ns);
	
}

WokXMLTag &
WokXMLTag::GetFirstTag(const std::string& name)
{
	if( object_reference.find(name) == object_reference.end() )
		return AddTag(name);
	if( object_reference[name].begin() == object_reference[name].end() )
		return AddTag(name);
	return **object_reference[name].begin();
}

int
WokXMLTag::GetType()
{
	return 1;
}

bool
WokXMLTag::In(WokXMLTag &tag)
{
	std::list <WokXMLTag *>::iterator tag_iter;
	std::map <std::string, std::string>::iterator arg_iter;

	if( name != tag.name )
		return false;

	for( arg_iter = arg_list.begin() ; arg_iter != arg_list.end() ; arg_iter++)
	{
		if(tag.arg_list.find(arg_iter->first) != tag.arg_list.end())
		{
			if( tag.GetAttr(arg_iter->first) == arg_iter->second )
				continue;
			else
				return false;
		}
		else
			return false;
	}


	for( tag_iter = tags.begin() ; tag_iter != tags.end() ; tag_iter++)
	{
		std::list <WokXMLTag *>::iterator innertag_iter;

		if( (tag.object_reference.find((*tag_iter)->GetName())) != tag.object_reference.end() )
		{
			std::list <WokXMLTag *> *list;
			list = &tag.GetTagList((*tag_iter)->GetName());

			for( innertag_iter = list->begin(); innertag_iter != list->end() ; innertag_iter++)
			{
				if( (*tag_iter)->In(**innertag_iter) )
					break;
			}
			if( innertag_iter == list->end() )
				return false;
		}
		else
			return false;
	}
	return true;
}

void
WokXMLTag::Print(std::ostream &output)
{
	output << *this;
}

WokXMLTag &
WokXMLTag::operator =(WokXMLTag &tag)
{
	std::list <WokXMLObject *>::iterator iter;

	for(iter = object_list.begin(); iter != object_list.end(); iter++)
		delete *iter;

	object_list.clear();
	tags.clear();
	arg_list.clear();
	object_reference.clear();

	if( xr )
		delete xr;

	name = tag.name;
	arg_list = tag.arg_list;

	for( iter = tag.object_list.begin() ; iter != tag.object_list.end(); iter++)
		AddObject(*iter);

	xr = NULL;

}

bool WokXMLTag::operator == ( WokXMLTag &tag)
{
	if ( GetStr() == tag.GetStr() )
		return true;
	return false;

	std::list <WokXMLObject *>::iterator object_iter;
	std::list <WokXMLObject *>::iterator tag_object_iter;

	if ( name != tag.name )
	{
		return false;
	}
	if ( arg_list != tag.arg_list )
	{
		return false;
	}
	if(object_list.size() != tag.object_list.size())
	{
		//return false;
	}

	tag_object_iter = tag.object_list.begin();
	for(object_iter = object_list.begin(); object_iter != object_list.end(); object_iter++)
	{
		if(tag_object_iter == tag.object_list.end())
			return false;

		if((*object_iter)->GetType() == (*tag_object_iter)->GetType())
		{
			switch((*object_iter)->GetType())
			{
				case 0:
					break;
				case 1:
					/* 2 rows because pre gcc 3.4.3 it wasn't possibule to have those on 1 line */
					WokXMLTag *tag1;
					tag1 = reinterpret_cast <WokXMLTag *>(*object_iter);
					WokXMLTag *tag2;
					tag2 = reinterpret_cast <WokXMLTag *>(*tag_object_iter);

					if( !(*tag1 == *tag2) )
						return false;
					break;
				case 2:
					WokXMLText *text1;
					text1 = reinterpret_cast <WokXMLText *>(*object_iter);
					WokXMLText *text2;
					text2 = reinterpret_cast <WokXMLText *>(*tag_object_iter);

					if( !(*text1 == *text2) )
					{
						std::cout << "Not the same: '" << *text1 << "' != '" << *text2 << "'" << std::endl;
						return false;
					}
					break;
			}
		}
		else
			return false;

		tag_object_iter++;
	}
	if(tag_object_iter != tag.object_list.end())
		return false;

	return true;
}

namespace Woklib
{

std::istream &
operator>>( std::istream &input, WokXMLTag &s)
{
	if(!s.xr)
		s.xr = new XMLReader();

	input >> *s.xr;

	if(!s.xr->GetTag())
		return input;

	s.AddTag(s.xr->GetTag());
	/*
	delete s.xr;
	s.xr = NULL;
	*/
	return input;
}

std::ostream &
operator<<( std::ostream &output, WokXMLTag &s)
{
	output << s.GetStr();
	return output;
}

}
