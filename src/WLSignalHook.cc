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


#include "include/WLSignalHook.h"
#include <string.h>
#include <iostream>
#include <algorithm>
using namespace Woklib;

WLSignalHook::WLSignalHook()
{
	exec = 0;
}

WLSignalHook::~WLSignalHook()
{
	std::map < int, std::list<WLSignalContainer *> >::iterator iter;
	std::list<WLSignalContainer *>::iterator list_iter;
	
	for( iter = Hooks.begin (); iter != Hooks.end (); iter++ )
		for(list_iter = iter->second.begin(); list_iter != iter->second.end(); list_iter++)
			if(*list_iter)
				delete (*list_iter);
}

int
WLSignalHook::Execute(WLSignalData *wlsd)
{
	if(exec == 0)
		deletion = false;
	int run = 0;
	exec++;
	bool brake = false;
	std::map < int, std::list<WLSignalContainer *> >::iterator iter;
	std::list<WLSignalContainer *>::iterator list_iter;
	for( iter = Hooks.begin (); iter != Hooks.end (); iter++ )
	{
		
		for(list_iter = iter->second.begin(); list_iter != iter->second.end(); list_iter++)
		{
			if( *list_iter != NULL )
			{
				run++;
				if(!((**list_iter).run(wlsd)))
				{
					brake = true;
					break;
				}
			}	
		}
		
		if ( brake )
			break;
	}
	exec--;
	if(deletion && !exec)
	{
		for( iter = Hooks.begin() ; iter != Hooks.end() ;)
		{
			while( (list_iter = find(iter->second.begin(), iter->second.end(), (WLSignalContainer*)NULL)) != iter->second.end() )
				iter->second.erase(list_iter);
			if( iter->second.size() == 0)
			{
				Hooks.erase(iter);
				iter = Hooks.begin();
				continue;
			}
			iter++;		
		}
	}
	
	if(exec == 0 && Hooks.size() == 0)
		return -run; // Schedule for deletion..
	return (run);
}

WLSignalContainer *
WLSignalHook::AddInstance(WLSignalContainer *wlsc)
{
	Hooks[wlsc->GetPriority()].push_back(wlsc);
	return wlsc;
}

WLSignalContainer *
WLSignalHook::AddInstance(WLSignalInstance *wlsi, int (WLSignalInstance::*member)(WLSignalData *wlsd), int priority)
{
	return(AddInstance(new WLSignalContainer(wlsi, member, priority)));
}

int
WLSignalHook::RemoveInstance(WLSignalInstance *wlsi, int (WLSignalInstance::*member)(WLSignalData *wlsd), int priority)
{	
	if ( Hooks.find(priority) == Hooks.end())
		return(Hooks.size());

	WLSignalContainer comp(wlsi, member, priority);
	
	std::list<WLSignalContainer *>::iterator iter;
	
	for(iter =  Hooks[priority].begin();iter != Hooks[priority].end(); iter++)
	{
		if(*iter && (**iter) == comp)
		{
			/* 
			   Be carefull here the list can already be in use because 
			   executing signal that is removing it self :)
			*/
			delete *iter;
			if( exec )
			{
				*iter = NULL;
				deletion = true;
			}
			else
			{
				Hooks[priority].erase(iter);
				if( !Hooks[priority].size())
					Hooks.erase(priority);
			}
			
			break;
		}
	}
	return (Hooks.size());
}
