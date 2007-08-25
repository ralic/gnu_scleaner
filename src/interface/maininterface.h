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

#ifndef _MAININTERFACE_H_
#define _MAININTERFACE_H_

#include <config.h>
#include <map>
#include <list>
#include <string>
#include <wx/frame.h>
#include <wx/toolbar.h>
#include <wx/splitter.h>
//#include <wx/treectrl.h>
#include "checktreectrl.h"
#include <wx/statusbr.h>
#include "tray.h"

class IPlugin;

/*!
 * CMainInterface class declaration
 */
class CMainInterface: public wxFrame
{    
    DECLARE_CLASS( CMainInterface )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CMainInterface();
    CMainInterface(wxWindow* parent, wxWindowID id = SYMBOL_MAININTERFACE_IDNAME, const wxString& caption = SYMBOL_MAININTERFACE_TITLE, const wxPoint& pos = SYMBOL_MAININTERFACE_POSITION, const wxSize& size = SYMBOL_MAININTERFACE_SIZE, long style = SYMBOL_MAININTERFACE_STYLE);

    bool Create(wxWindow* parent, wxWindowID id = SYMBOL_MAININTERFACE_IDNAME, const wxString& caption = SYMBOL_MAININTERFACE_TITLE, const wxPoint& pos = SYMBOL_MAININTERFACE_POSITION, const wxSize& size = SYMBOL_MAININTERFACE_SIZE, long style = SYMBOL_MAININTERFACE_STYLE );

    ~CMainInterface();

    void Init();
    void CreateControls();
    wxBitmap GetBitmapResource( const wxString& name );
    wxIcon GetIconResource( const wxString& name );
    static bool ShowToolTips();

protected:
    CTrayIcon*	m_Icon;
#if defined(__WXCOCOA__)
    CTrayIcon*	m_DockIcon;
#endif
	std::map<std::string, IPlugin*>* 	m_PluginsList;
	wxCheckTreeCtrl* m_TreeList;
};

#endif
    // _MAININTERFACE_H_