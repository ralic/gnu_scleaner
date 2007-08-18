/*
Copyright (C) 2007 FROUIN Jean-Michel (jmfrouin@gmail.com)
*/
/*
------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------
Project : scleaner
------------------------------------------------------
$Date: 2007-08-13 21:07:17 +0200 (lun, 13 aoû 2007) $
$Rev: 13 $
$Author: snoogie_sf $
------------------------------------------------------

*/

#include <iostream>
#include <config.h>
#include <plugins/plugin_manager.h>
#include <leak/memory_manager.h>
#include <tools/smart_pointer.h>
#include <interface/app.h>

CMemoryManager g_mm;

#include <leak/leak_detector.h>

int main()
{
	std::cout << "sclean " << VER << '\n';

    bool l_ret = false;
    TSmartPtr<CPluginManager> l_pfm;
	l_pfm = CPluginManager::Instance();
    l_ret = l_pfm->loadPlugins("plugs");
    l_pfm->SpaceUsed();
	
	std::cout << "Launch" << '\n';

	//delete l_pfm; //Not need anymore : thanks smart pointers.
	return l_ret;
}
/* vi:set ts=4: */