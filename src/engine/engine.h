/*
Copyright (C) 2007 FROUIN Jean-Michel
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

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <string>
#include <tools/smart_pointer.h>
#include <tools/singleton.h>
#include <plugins/plugin_manager.h>
#include <plugins/root_plugin.h>
#include "iprogress.h"

#define FSTAB "/etc/fstab"
#define ROUND(x) ((x-(int)x>0)?(int)x+1:(int)x) //Mainly wrote for formatSize

/*!
 *@brief Manage all operations
 */
class CEngine: public CSmartCpt, public TSingleton<CEngine>
{
	public:
		/*!
		 *@brief ctor
		 */
		CEngine();

		/*!
		 *@brief dtor
		 */
		~CEngine(){}

	public:
		/*!
		 *@brief Call load plugins in PluginManager.
		 *@param path The folder location.
		 *@return Number of plugins founded.
		 */
		int loadPlugins (const std::string& path);

		/*!
		 *@brief Load GFX interface build on wxWidgets
		 *@return true is everything is OK, false otherwise.
		 */
		bool loadInterface();

		/*!
		 *@brief Detect if launch as root
		 */
		static bool isRoot();

		/*!
		 *@brief Detect kernel version
		 */
		static bool getKernelVersion(std::string& _version);

		/*!
		 *@brief callOutputPlugins with a list of files
		 *@param _list A files list.
		 *@param _name Name of plugin.
		 *@param _path Path to save (if applicable) the output file.
		 *@param _callback For progress bar.
		 *@return true on success, false otherwise.
		 *@todo Implement errors code.
		 */
		bool callOutputPlugin(std::list<std::string>& _list, std::string& _name, const std::string& _path, IProgressbar* _callback);

		/*!
		 *@brief Replace wxDir usage.
		 *@param _path Where retrieve file list.
		 *@param _asRoot Scan in root mode.
		 *@param _rootPlugin Root plugin.
		 *@param _recursive If true, enter subfolders.
		 *@return true on success, false otherwise.
		 */
		bool scanDirectory(const std::string& _path, bool _asRoot = false, IRootPlugin* _rootPlugin = 0, bool _recursive = true);

		/*!
		 *@brief Callback : man ftw.
		 *@param _fpath Directory.
		 *@param _stat Where retrieve file list.
		 *@param _tflag Flags.
		 *@param _ftwbuf the TFW buffer.
		 *@return true on success, false otherwise.
		 */
		static int FTW_callback(const char* _fpath, const struct stat* _stat, int _tflag, struct FTW* _ftwbuf);

		/*!
		 *@brief Retrieve username.
		 *@param _username The std::string to fill.
		 *@return true on success, false otherwise.
		 */
		static bool getUsername(std::string& _username);

		/*!
		 *@brief Better size display.
		 *@param _size Size to format.
		 *@param _str String to fill.
		 */
		static void formatSize(double _size, std::string& _str);

		/*!
		 *@brief Get free space on system (Using mount table : by default /etc/fstab).
		 *@param _path To get space from a path.
		 *@param _used Used space correctly formatted.
		 *@param _free Available space correctly formatted.
		 *@param _total Total space correctly formatted.
		 *@return free space size in bytes.
		 */
		static double getFreeSpace(const std::string& _path, std::string& _used, std::string& _free, std::string& _total);

		/*!
		*@brief Launch a scan and pass each file to input plugin, to allow them to build file list.
		 *@param _callback For progress bar.
		*/
		bool scanDisk(IProgressbar* _callback);
		
		/*!
		*@brief Get a timestamp.
		*@param _str The str to fill.
		*/
		static void getTimestamp(std::string& _str);

		//Accessors
		CPluginManager*	getPluginManager()
		{
			return m_pfm;
		}

		bool asRoot()
		{
			return m_asRoot;
		}

		IRootPlugin* rootPlugin()
		{
			return m_rootPlugin;
		}

		IProgressbar* getCallback()
		{
			return m_callback;
		}

	private:
		TSmartPtr<CPluginManager>   m_pfm;
		//Due to this fuck*** callback
		IRootPlugin*				m_rootPlugin;
		bool						m_asRoot;
		IProgressbar*				m_callback;
};
#endif							 //_ENGINE_H_
