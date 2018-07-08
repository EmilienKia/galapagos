/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * history-panel.cpp
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

#include "history-panel.hpp"
#include "history-detail-panel.hpp"

#include "app.hpp"
#include "commit.hpp"

BEGIN_EVENT_TABLE(HistoryPanel, wxPanel)
	EVT_DATAVIEW_SELECTION_CHANGED(wxID_ANY, HistoryPanel::onHistoryItemSelected)
END_EVENT_TABLE()

HistoryPanel::HistoryPanel(wxWindow* parent,  wxWindowID id):
wxPanel(parent, id)
{
	CommonInit();
}

void HistoryPanel::CommonInit()
{
	_splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER|wxSP_THIN_SASH|wxSP_LIVE_UPDATE);
	_history = new wxDataViewCtrl(_splitter, wxID_ANY);
	wxNotebook* nb = new wxNotebook(_splitter, wxID_ANY);
	_splitter->SplitHorizontally(_history, nb);

	wxSizer* gsz = new wxBoxSizer(wxVERTICAL);
	gsz->Add(_splitter, 1, wxEXPAND);
	SetSizer(gsz);

    _history->AppendColumn(new wxDataViewColumn( "Graph", new GraphicalLineRenderer, CommitListDataModel::GRAPH, wxDVC_DEFAULT_WIDTH, wxALIGN_LEFT ));
	_history->AppendTextColumn("Message", CommitListDataModel::MESSAGE, wxDATAVIEW_CELL_INERT, 480);
	_history->AppendTextColumn("SHA-1", CommitListDataModel::SHA1);
	_history->AppendTextColumn("Date", CommitListDataModel::DATE);
	_history->AppendTextColumn("Author", CommitListDataModel::AUTHOR);
	_history->AppendTextColumn("Committer", CommitListDataModel::COMMITTER);
	_history->AssociateModel(wxGetApp().getHistoryDataModel());

	nb->AddPage(_details = new HistoryDetailPanel(nb, wxID_ANY), "Details");
}

void HistoryPanel::onHistoryItemSelected(wxDataViewEvent& event)
{
	if(_history->HasSelection())
	{
		unsigned int row = ((wxDataViewListModel*)_history->GetModel())->GetRow(_history->GetSelection());
		_details->SetSelection(row);
	}
}

