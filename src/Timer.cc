/***************************************************************************
 *  Copyright (C) 2006  Kent Gustavsson <nedo80@gmail.com>
 ****************************************************************************/
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


//
// Class: Timer
//
// Created by: Kent Gustavsson <nedo80@gmail.com>
// Created on: Tue Sep 19 20:46:05 2006
//

#include "Timer.h"
using namespace Woklib;

Timer::Timer(WLSignal *wls, WokXMLTag *tag) : WLSignalInstance(wls)
{
	delay = atoi(tag->GetAttr("time").c_str())/1000.;
	signal = tag->GetAttr("signal");
}


Timer::~Timer()
{

}

double
Timer::GetDelay()
{
	return delay;
}

const std::string &
Timer::GetSignal()
{
	return signal;
}

