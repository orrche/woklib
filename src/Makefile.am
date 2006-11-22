#
#  Copyright (C) 2003-2006  Kent Gustavsson <nedo80@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

INCLUDES = -I.. -I../include/

lib_LTLIBRARIES = libwoklib.la

includedir = $(prefix)/include/Woklib

include_HEADERS = \
  Timer.h

libwoklib_la_SOURCES = \
	WokLibSignal.cc \
	main.cc \
	WokXMLObject.cc \
	WLSignal.cc \
	WokXMLTag.cc \
	WLSignalContainer.cc \
	WokXMLText.cc \
	WLSignalData.cc \
	WoklibPlugin.cc \
	WLSignalHook.cc \
	WLSignalInstance.cc \
	WokLib.cc \
	Messages.cc \
	XMLReader.cc \
	Timer.cc \
	Timer.h \
	WokXMLData.cc \
	WokXMLData.h 

libwoklib_la_LDFLAGS = \
	-g 

libwoklib_la_LIBADD = -lssl -lexpat
