/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * remotes-panel.cpp
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
#include <wx/statline.h>
#include <wx-3.0/wx/gtk/textctrl.h>

#include "app.hpp"

#include "remotes-panel.hpp"

RemotesPanel::RemotesPanel(git2::Repository& repo, wxWindow* parent, wxWindowID id):
wxPanel(parent, id),
_repo(repo)
{
    CommonInit();
}

void RemotesPanel::CommonInit()
{
    wxSizer* gsz = new wxBoxSizer(wxHORIZONTAL);
    
    wxSizer* lsz = new wxBoxSizer(wxVERTICAL);
    _remotes = new wxListBox(this, ID_REMOTE_PANEL_LIST, wxDefaultPosition, wxDefaultSize, 0, nullptr, wxLB_SINGLE|wxLB_NEEDED_SB);
    lsz->Add(_remotes, 1, wxEXPAND);    
    gsz->Add(lsz, 1, wxEXPAND|wxALL, 4);

    wxSizer* dsz = new wxBoxSizer(wxVERTICAL);
    dsz->Add(_name = new wxTextCtrl(this, ID_REMOTE_PANEL_NAME, wxEmptyString), 0, wxEXPAND|wxALL, 4);
    dsz->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxEXPAND|wxALL, 4);
    dsz->Add(new wxStaticText(this, wxID_ANY, "URL"), 0, wxEXPAND|wxALL, 4);
    dsz->Add(_url = new wxTextCtrl(this, ID_REMOTE_PANEL_URL, wxEmptyString), 0, wxEXPAND|wxALL, 4);
    dsz->Add(new wxStaticText(this, wxID_ANY, "Push URL"), 0, wxEXPAND|wxALL, 4);
    dsz->Add(_pushurl = new wxTextCtrl(this, ID_REMOTE_PANEL_PUSH_URL, wxEmptyString), 0, wxEXPAND|wxALL, 4);
    dsz->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxEXPAND|wxALL, 4);    
    wxSizer* bsz = new wxBoxSizer(wxHORIZONTAL);
    bsz->Add(_add = new wxButton(this, ID_REMOTE_PANEL_BUTTON_ADD,    "Add"), 1, wxEXPAND|wxALL, 4);
    bsz->Add(_delete = new wxButton(this, ID_REMOTE_PANEL_BUTTON_DELETE, "Delete"), 1, wxEXPAND|wxALL, 4);
    bsz->Add(_edit = new wxButton(this, ID_REMOTE_PANEL_BUTTON_EDIT,   "Edit"), 1, wxEXPAND|wxALL, 4);
    bsz->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), 0, wxEXPAND|wxALL, 4);    
    bsz->Add(_save = new wxButton(this, ID_REMOTE_PANEL_BUTTON_SAVE,   "Save"), 1, wxEXPAND|wxALL, 4);
    bsz->Add(_discard = new wxButton(this, ID_REMOTE_PANEL_BUTTON_DISCARD,   "Discard"), 1, wxEXPAND|wxALL, 4);
    dsz->Add(bsz, 0, wxEXPAND);
    
    gsz->Add(dsz, 3, wxEXPAND|wxALL, 4);

    SetSizer(gsz);
    
    updateRemoteList();
}

void RemotesPanel::updateRemoteList()
{
    _remotes->Clear();
    
    _add->Enable();
    _delete->Disable();
    _edit->Disable();
    _save->Disable();
    _discard->Disable();
    
    if(_repo.ok())
    {
        for(std::string rname : _repo.listRemote())
        {
            _remotes->Append(str2wx(rname));
        }
    }
}


BEGIN_EVENT_TABLE(RemotesPanel, wxPanel)
    EVT_LISTBOX(ID_REMOTE_PANEL_LIST, RemotesPanel::onListSelected)

    EVT_BUTTON(ID_REMOTE_PANEL_BUTTON_ADD, RemotesPanel::onAdd)
    EVT_BUTTON(ID_REMOTE_PANEL_BUTTON_DELETE, RemotesPanel::onDelete)
    EVT_BUTTON(ID_REMOTE_PANEL_BUTTON_EDIT, RemotesPanel::onEdit)
    EVT_BUTTON(ID_REMOTE_PANEL_BUTTON_SAVE, RemotesPanel::onSave)
    EVT_BUTTON(ID_REMOTE_PANEL_BUTTON_DISCARD, RemotesPanel::onDiscard)
END_EVENT_TABLE()

void RemotesPanel::updateWithRemote(const wxString& rem)
{
    std::string name{rem.utf8_str()};
    git2::Remote* remote = name.empty() ? nullptr : _repo.getRemote(name);
    if(remote)
    {
        _name->SetValue(rem);
        _url->SetValue(str2wx(remote->url()));
        _pushurl->SetValue(str2wx(remote->pushUrl()));        
    }
    else
    {
        _name->Clear();
        _url->Clear();
        _pushurl->Clear();
    }

    _add->Enable();
    _delete->Enable((bool)remote);
    _edit->Enable((bool)remote);
    _save->Disable();
    _discard->Disable();
    
    _name->Disable();
    _url->Disable();
    _pushurl->Disable();
}

void RemotesPanel::onListSelected(wxCommandEvent& event)
{
    updateWithRemote(event.GetString());
    _remotes->Enable();
}

void RemotesPanel::onAdd(wxCommandEvent& event)
{
    _remotes->DeselectAll();
    _remotes->Disable();
    
    _name->SetValue("<<newremote>>"); // TODO add a smarter remote name (origin, upstream ...)
    _name->Enable();
    _url->Clear();
    _url->Enable();
    _pushurl->Clear();
    _pushurl->Enable();
    
    _add->Disable();
    _delete->Disable();
    _edit->Disable();
    
    _save->Enable();
    _discard->Enable();
    
    _name->SelectAll();
    _name->SetFocus();
}

void RemotesPanel::onDelete(wxCommandEvent& event)
{
    wxString sel = _remotes->GetStringSelection();
    if(!sel.IsEmpty())
    {
        if(wxMessageBox(
                "Do you really want to remove the '" + sel + "' remote ?\nThis action cannot be canceled.",
                "Galapagos",
                wxOK|wxCANCEL|wxCANCEL_DEFAULT,
                this
                ) == wxOK)
        {
            _repo.deleteRemote(wx2str(sel));
            // TODO catch exception.
        }
    }

}

void RemotesPanel::onEdit(wxCommandEvent& event)
{
    if(_remotes->GetSelection()!=wxNOT_FOUND)
    {
        _name->Enable();
        _url->Enable();
        _pushurl->Enable();
        
        _add->Disable();
        _delete->Disable();
        _edit->Disable();
        _save->Enable();
        _discard->Enable();
        _remotes->Disable();
        
        _url->SetFocus();
    }
    else
    {
        _name->Disable();
        _url->Disable();
        _pushurl->Disable();
        
        _add->Enable();
        _delete->Disable();
        _edit->Disable();
        _save->Disable();
        _discard->Disable();
        _remotes->Enable();
        
        _remotes->SetFocus();
    }
}

void RemotesPanel::onSave(wxCommandEvent& event)
{
    // TODO
    wxMessageBox("Not implemented yet !");

    wxString old = _remotes->GetStringSelection();
    bool isNew = old.empty();
    std::string newname = wx2str(_name->GetValue());
    
    if(old.empty()) // New remote, create it
    {
//        _repo.createRemote(newname, )
    }
    
    
    wxMessageBox("TODO");
}

void RemotesPanel::onDiscard(wxCommandEvent& event)
{
    updateWithRemote(_remotes->GetStringSelection());
    _remotes->Enable();
    _remotes->SetFocus();
}