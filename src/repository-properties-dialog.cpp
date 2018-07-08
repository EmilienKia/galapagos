/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * repository-properties-dialog.cpp
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

#include "app.hpp"
#include "repository-properties-dialog.hpp"

#include "remotes-panel.hpp"


RepositoryPropertiesDialog::RepositoryPropertiesDialog(wxFrame* frame, git2::Repository& repo):
wxDialog(frame, wxID_ANY, wxGetApp().GetAppDisplayName() + " - " + "Repository properties", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER),
_repo(repo)
{
    CommonInit();
}

void RepositoryPropertiesDialog::CommonInit()
{
    wxSizer* gsz = new wxBoxSizer(wxVERTICAL);
    _book = new wxNotebook(this, wxID_ANY);
    _book->AddPage(new RemotesPanel(_repo, _book, wxID_ANY), "Remotes");

    gsz->Add(_book, 1, wxEXPAND|wxALL, 4);
    gsz->Add(CreateStdDialogButtonSizer(wxOK), 0, wxEXPAND|wxALL, 4);
    SetSizer(gsz);
    
}

