/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * history-panel.hpp
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

#ifndef _HISTORY_PANEL_HPP_
#define _HISTORY_PANEL_HPP_

#include <wx/dataview.h>
#include <wx/splitter.h>

class HistoryDetailPanel;

class HistoryPanel: public wxPanel 
{
	DECLARE_EVENT_TABLE ()		
public:
	HistoryPanel(wxWindow* parent,  wxWindowID id);

protected:

private:
	void CommonInit();

	wxSplitterWindow* _splitter;

	wxDataViewCtrl* _history;
	HistoryDetailPanel* _details;

	void onHistoryItemSelected(wxDataViewEvent& event);	
};

#endif // _HISTORY_PANEL_HPP_

