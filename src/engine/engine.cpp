/*
Copyright (C) 2007 FROUIN Jean-Michel (jmfrouin@gmail.com)
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
$Date$
$Rev$
$Author$
------------------------------------------------------

*/

#include <interface/maininterface.h>
#include "engine.h"

int CEngine::loadPlugins(const std::string& _path)
{
	int l_ret=0;
	//Plugins manager
	m_pfm = CPluginManager::Instance();
    l_ret = m_pfm->loadPlugins(_path);
	return l_ret;
}

bool CEngine::loadInterface()
{
	bool l_ret = false;

	CMainInterface* l_Main = 0;
	l_Main = new CMainInterface(NULL, wxID_ANY, wxT(NAME), wxDefaultPosition, wxSize(365, 290));
	if(l_Main)
    {
		l_Main->Show(true);
		l_ret = true;
	}

	return l_ret;
}
