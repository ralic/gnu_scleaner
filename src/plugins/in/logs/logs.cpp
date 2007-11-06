/**
 * This file is part of scleaner project.

 * Copyright (C) 2007 FROUIN Jean-Michel

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include <plugins/inplugin_initializer.h>
#include "logs.h"
#include <leak/leak_detector.h>
#include <engine/engine.h>

Plugins::CPluginInitializerIn<ClogsPlugin> g_logs;

ClogsPlugin::ClogsPlugin()
{
	setName("logs");
}


ClogsPlugin::~ClogsPlugin()
{
}


void ClogsPlugin::getDirectory(std::string& _path)
{
	_path = "/var/log/";
}


void ClogsPlugin::processFile(const std::string& _filename)
{
	m_fl.push_back(_filename);
}


std::string ClogsPlugin::Description()
{
	return "Find old logs";
}
/* vi:set ts=4: */
