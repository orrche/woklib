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


#include "include/WLSignalData.h"

using std::string;
using std::map;
using namespace Woklib;

WLSignalData::WLSignalData()
{
	WLDEmptyInt = 0;
	WLDEmptyStr = "";
}

WLSignalData::~WLSignalData()
{
	
}

void
WLSignalData::WLDSetString( const string &name, const string &data)
{
	WLDStringStorage[name] = data;	
}

void
WLSignalData::WLDSetInt( const string &name, const int &data)
{
	WLDIntStorage[name] = data;	
}

const string &
WLSignalData::WLDGetString( const string &name )
{
	map <string, string>::iterator str_iter;
	
	if((str_iter = WLDStringStorage.find(name)) != WLDStringStorage.end()   )
		return str_iter->second;		

	return WLDEmptyStr;
}

const int &
WLSignalData::WLDGetInt( const string &name )
{
	map <string, int>::iterator str_iter;
	
	if((str_iter = WLDIntStorage.find(name)) != WLDIntStorage.end()   )
		return str_iter->second;		

	return WLDEmptyInt;
}
