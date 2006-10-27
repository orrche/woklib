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

#ifndef __WL_SIGNAL_DATA
#define __WL_SIGNAL_DATA

#include <string>
#include <map>

namespace Woklib 
{
class WLSignalData
{
	public:
	WLSignalData();
	virtual ~WLSignalData();
	
	void WLDSetString( const std::string &name, const std::string &data);
	void WLDSetInt( const std::string &name, const int &data);
	
	const std::string & WLDGetString( const std::string &name );
	const int & WLDGetInt( const std::string &name );
	
	private:
	std::map <std::string, std::string> WLDStringStorage;
	std::map <std::string, int > WLDIntStorage;

	std::string WLDEmptyStr;
	int WLDEmptyInt;
};

}
#endif // __WL_SIGNAL_DATA
