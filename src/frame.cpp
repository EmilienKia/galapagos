/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * frame.cpp
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <wx/wx.h>

#include <wx/artprov.h>
#include <wx/dirdlg.h> 
#include <wx/ribbon/buttonbar.h>

#include "app.hpp"
#include "commit.hpp"
#include "frame.hpp"

#define wxLOG_COMPONENT "TOTO"

#define wxArtIcon(artid, sz) (wxArtProvider::GetBitmap(artid, wxART_OTHER, wxSize(sz,sz)))
#define RibbonIcon(artid) wxArtIcon(artid, 32)

Frame::Frame():
wxFrame(NULL, wxID_ANY, wxGetApp().GetAppDisplayName(), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
{
	init();
}

Frame::~Frame()
{
	_manager.UnInit();
}

void Frame::init()
{
	_manager.SetManagedWindow(this);

	// Ribbon bar init
	{
		_ribbonBar = new wxRibbonBar(this, wxID_ANY, wxDefaultPosition, wxSize(800, 600), wxRIBBON_BAR_FLOW_HORIZONTAL
								| wxRIBBON_BAR_SHOW_PAGE_LABELS
								| wxRIBBON_BAR_SHOW_PANEL_EXT_BUTTONS
								//| wxRIBBON_BAR_SHOW_TOGGLE_BUTTON
								//| wxRIBBON_BAR_SHOW_HELP_BUTTON
								);

		initRibbon();
		_ribbonBar->Realise();

		_manager.AddPane(_ribbonBar, wxAuiPaneInfo().Top().Show(true));
	}

	// Notebook
	{
		_notebook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS );
		_historyPanel = new HistoryPanel(_notebook, wxID_ANY);
		_logTextCtrl = new wxTextCtrl(_notebook, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL);
			
		_notebook->AddPage(_historyPanel, "History");
		_notebook->AddPage(_logTextCtrl, "Logs");
		_manager.AddPane(_notebook, wxAuiPaneInfo().CenterPane().Name("Content").Show(true));
	}

/*	wxPanel* panel = new wxPanel(this, wxID_ANY);

	wxSizer* sz = new wxBoxSizer(wxVERTICAL);
	sz->Add(_ribbonBar, 0, wxEXPAND);
	sz->Add(panel, 1, wxEXPAND);
	SetSizer(sz);

*/
	
        wxLog::SetActiveTarget(new wxLogTextCtrl(_logTextCtrl));
        
//	_manager.AddPane(_notebook, wxAuiPaneInfo().CenterPane().Name("Notebook").Show(true));
	_manager.Update();



	_manager.Update();
    wxLogMessage("Hello");
}

void Frame::initRibbon()
{
	{
		wxRibbonPage* page = new wxRibbonPage(_ribbonBar, wxID_ANY, "Repository");
		{
			wxRibbonPanel* panel = new wxRibbonPanel(page, wxID_ANY, "Repository"); 
			wxRibbonButtonBar* bar = new wxRibbonButtonBar(panel, wxID_ANY);
			bar->AddHybridButton(wxID_OPEN, "Open", RibbonIcon(wxART_FILE_OPEN));
			bar->AddButton(wxID_NEW, "New", RibbonIcon(wxART_NEW));
//			bar->AddButton(ID_CLONE, "Clone", RibbonIcon(wxART_NEW));
			bar->AddButton(wxID_REFRESH, "Refresh", RibbonIcon("gtk-refresh"));
		}
		{
			wxRibbonPanel* panel = new wxRibbonPanel(page, wxID_ANY, "Working copy"); 
			wxRibbonButtonBar* bar = new wxRibbonButtonBar(panel, wxID_ANY);
                        
			bar->AddButton(ID_SWITCH, "Switch", RibbonIcon(wxART_NEW));
			bar->AddButton(wxID_INDEX, "...", RibbonIcon(wxART_NEW));
		}
	}

}


BEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_CUSTOM(wxEVT_COMMAND_RIBBONBUTTON_CLICKED, wxID_ANY, Frame::onRibbonButtonClicked)

	EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(wxID_OPEN, Frame::onRecentFileMenuDropDown)
END_EVENT_TABLE()


void Frame::onRibbonButtonClicked(wxEvent/*wxRibbonButtonBarEvent*/& event)
{
    wxRibbonButtonBarEvent *ribbonevent = dynamic_cast<wxRibbonButtonBarEvent*>(&event);

    // Process an equivalent wxEVT_COMMAND_MENU_SELECTED event for simulating menu events.
    // Used to unify RibbonButton click and accelerator entries.

    wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, event.GetId());
    if(ribbonevent)
        evt.SetInt(ribbonevent->GetInt());
    ProcessEvent(evt);

    event.Skip();
}

void Frame::onRecentFileMenuDropDown(wxRibbonButtonBarEvent& event)
{
	event.PopupMenu(wxGetApp().fileHistoryMenu());
}


