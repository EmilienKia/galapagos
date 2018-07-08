/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * history-detail-panel.cpp
 * Copyright (C) 2018 Emilien Kia <Emilien.kia+dev@gmail.com>
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

#include "history-detail-panel.hpp"
#include "app.hpp"

HistoryDetailPanel::HistoryDetailPanel(wxWindow* parent, wxWindowID id):
wxPanel(parent, id)
{
	CommonInit();
}

void HistoryDetailPanel::CommonInit()
{
	wxSizer* gsz = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer* fgsz = new wxFlexGridSizer(7, 2, 4, 4);
	fgsz->AddGrowableCol(1);
	fgsz->AddGrowableRow(5);
	fgsz->AddGrowableRow(6);
	fgsz->Add(new wxStaticText(this, wxID_ANY, "SHA-1: "), 0, wxALIGN_RIGHT);
	fgsz->Add(sha1 = new wxStaticText(this, wxID_ANY, ""), 1, wxEXPAND|wxALIGN_LEFT);
	fgsz->Add(new wxStaticText(this, wxID_ANY, "Date: "), 0, wxALIGN_RIGHT);
	fgsz->Add(date = new wxStaticText(this, wxID_ANY, ""), 1, wxEXPAND|wxALIGN_LEFT);
	fgsz->Add(new wxStaticText(this, wxID_ANY, "Author: "), 0, wxALIGN_RIGHT);
	fgsz->Add(author = new wxStaticText(this, wxID_ANY, ""), 1, wxEXPAND|wxALIGN_LEFT);
	fgsz->Add(new wxStaticText(this, wxID_ANY, "Committer: "), 0, wxALIGN_RIGHT);
	fgsz->Add(committer = new wxStaticText(this, wxID_ANY, ""), 1, wxEXPAND|wxALIGN_LEFT);
	fgsz->Add(new wxStaticText(this, wxID_ANY, "Subject: "), 0, wxALIGN_RIGHT);
	fgsz->Add(subject = new wxStaticText(this, wxID_ANY, ""), 1, wxEXPAND|wxALIGN_LEFT);
	fgsz->Add(new wxStaticText(this, wxID_ANY, "Parents: "), 0, wxALIGN_RIGHT);
	fgsz->Add(parents = new wxListBox(this, wxID_ANY), 1, wxEXPAND|wxALIGN_LEFT);
	fgsz->Add(new wxStaticText(this, wxID_ANY, "Children: "), 0, wxALIGN_RIGHT);
	fgsz->Add(children = new wxListBox(this, wxID_ANY), 1, wxEXPAND|wxALIGN_LEFT);

	gsz->Add(fgsz, 0, wxEXPAND);

	gsz->Add(message = new wxStaticText(this, wxID_ANY, ""), 0, wxEXPAND|wxALIGN_LEFT);

	gsz->AddSpacer(1);
	
	SetSizer(gsz);
}

void HistoryDetailPanel::SetSelection(size_t pos)
{
	const Commit& commit = wxGetApp().getHistoryDataModel()->getCommit(pos);

	sha1->SetLabel(wxString(commit.oid().format().data(),wxConvUTF8));
	date->SetLabel(wxDateTime(commit.time()).Format());
	author->SetLabel(wxString(commit.author().name().data(),wxConvUTF8));
	committer->SetLabel(wxString(commit.committer().name().data(),wxConvUTF8));
	subject->SetLabel(wxString(commit.shortMessage().data(),wxConvUTF8));
	
	parents->Clear();
	for(size_t n=0; n<commit.parentCount(); ++n)
	{
		parents->Append(commit.parentId(n).format());
	}

	children->Clear();
	for(const git2::OId& oid : commit.children)
	{
		children->Append(oid.format());
	}

	message->SetLabel(wxString(commit.message().data(),wxConvUTF8));	
}
	                  