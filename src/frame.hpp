/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * frame.hpp
 * Copyright (C) 2018 Emilien Kia <Emilien.Kia+dev@gmail.com>
 *
 * galapagos is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * galapagos is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _FRAME_HPP_
#define _FRAME_HPP_

#include <wx/frame.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/auibook.h>
#include <wx/log.h>
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/stc/stc.h>

#include "history-panel.hpp"

class Frame: public wxFrame 
{
	DECLARE_EVENT_TABLE()
public:
	Frame();
	~Frame();

protected:
	void init();

	wxAuiManager _manager;
	wxRibbonBar*  _ribbonBar;
	wxAuiNotebook* _notebook;

	HistoryPanel* _historyPanel;
        wxTextCtrl* _logTextCtrl;
	
private:
	void initRibbon();
	
	void onRibbonButtonClicked(wxEvent/*wxRibbonButtonBarEvent*/& event);

	void onRecentFileMenuDropDown(wxRibbonButtonBarEvent& event);

};

#endif // _FRAME_HPP_

