/**
 * This file is part of scleaner project.

 * Copyright (C) 2004-2005 Andrew Reading.
 * Copyright (C) 2007 Sylvain Beucler.
 * Visit scleaner website : http://www.scleaner.org
 * Copyright (C) 2007, 2008 FROUIN Jean-Michel

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

/*! @page page3 Plugins documentations.
 * - @subpage tbz
 */

/*! @page tbz tbz: output plugin.
 * @section desc Description
 * This plugin allow user to create a <b>tarball</b> with all selected files from input plugins. Then it will compress it using <b>bzip2</b>.
 * <br>
 * @section param Parameters
 * @section pb Knows bugs
 * @section todo Todo
 */

#ifndef __TBZ_H__
#define __TBZ_H__

#include <config.h>
#include <plugins/out/out_plugin.h>

class CtbzPlugin : public Plugins::IOutPlugin
{
  public:
    CtbzPlugin();
    ~CtbzPlugin();

    /*!
     *@brief From IPlugin.
     */
    EType Type() { return eOutput; }

    /*!
     *@brief From IOutPlugin
     */
    void ProcessFileList(std::list<std::string>& fl, const std::string& path, IProgressbar* callback);

  private:
    /*!
     *@brief Compress a file using bz algo.
     *@param input Input file name.
     *@param output Output file name.
     *@param callback For the progress bar.
     *@return true on success, false otherwise.
     */
    bool Compress(const std::string& input, const std::string& output, IProgressbar* callback);

    /*!
     *@brief From IPlugin
     */
    std::string Description();
};
#endif                           //__TBZ_H__
/* vi:set ts=4: */
