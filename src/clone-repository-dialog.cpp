/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * clone-bar-repository-dialog.cpp
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


#include "clone-repository-dialog.hpp"

#include <wx/checkbox.h>
#include <wx/filepicker.h>


CloneRepositoryDialog::CloneRepositoryDialog():
wxDialog(NULL, wxID_ANY, wxGetApp().GetAppDisplayName() + " - " + "Clone repository", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER)
{
	CommonInit();
}

void CloneRepositoryDialog::CommonInit()
{
	wxSizer* gsz = new wxBoxSizer(wxVERTICAL);
	gsz->AddStretchSpacer(1);
	gsz->Add(new wxStaticText(this, wxID_ANY, "Clone a new repository:"), 0, wxEXPAND|wxALL, 4);
	gsz->AddSpacer(4);
	gsz->Add(_path = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, wxDirSelectorPromptStr, wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL/*|wxDIRP_DIR_MUST_EXIST*/), 0, wxEXPAND|wxALL, 4);
	gsz->Add(_bare = new wxCheckBox(this, wxID_ANY, "Bare repository"), 0, wxEXPAND|wxALL, 4);
	gsz->AddStretchSpacer(1);
	gsz->Add(CreateStdDialogButtonSizer(wxOK|wxCANCEL), 0, wxEXPAND|wxALL, 4);
	SetSizer(gsz);
}

wxString CloneRepositoryDialog::getPath()const
{
	return _path->GetPath();
}

bool CloneRepositoryDialog::isBare()const
{
	return _bare->GetValue();
}
