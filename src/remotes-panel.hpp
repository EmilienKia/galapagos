/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * remotes-panel.hpp
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

#ifndef _REMOTES_PANEL_HPP_
#define _REMOTES_PANEL_HPP_


#include <wx/listbox.h>

class RemotesPanel: public wxPanel 
{
public:
    RemotesPanel(git2::Repository& repo, wxWindow* parent, wxWindowID id);
	
protected:
    DECLARE_EVENT_TABLE();

    void updateRemoteList();

    void onListSelected(wxCommandEvent& event);
    
    void onAdd(wxCommandEvent& event);
    void onDelete(wxCommandEvent& event);
    void onEdit(wxCommandEvent& event);
    void onSave(wxCommandEvent& event);
    void onDiscard(wxCommandEvent& event);

    void updateWithRemote(const wxString& remote);
    
private:
    git2::Repository& _repo;

    wxListBox* _remotes;
    
    wxTextCtrl* _name;
    wxTextCtrl* _url;
    wxTextCtrl* _pushurl;

    wxButton* _add;
    wxButton* _delete;
    wxButton* _edit;
    wxButton* _save;
    wxButton* _discard;
    
    void CommonInit();
    

};

#endif // _REMOTES_PANEL_HPP_

