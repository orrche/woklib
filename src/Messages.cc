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

#include "include/Messages.h"
namespace Woklib
{
void
woklib_message(WLSignal *wls, std::string message)
{
	WokXMLTag msgtag(NULL, "message");
	msgtag.AddTag("body").AddText(message);
	wls->SendSignal("Display Message", &msgtag);
}

void
woklib_error(WLSignal *wls, std::string message)
{
	WokXMLTag msgtag(NULL, "message");
	msgtag.AddTag("body").AddText(message);
	wls->SendSignal("Display Error", &msgtag);
}

void
woklib_debug(WLSignal *wls, std::string message)
{
	WokXMLTag msgtag(NULL, "message");
	msgtag.AddTag("body").AddText(message);
	wls->SendSignal("Display Debug", &msgtag);
}
}
