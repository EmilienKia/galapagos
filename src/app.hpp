/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * app.hpp
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

#ifndef _APP_HPP_
#define _APP_HPP_

#include <git2pp.hpp>

#include <wx/config.h>
#include <wx/dataview.h>
#include <wx/filehistory.h>

#include "commit.hpp"
#include "frame.hpp"


inline wxString str2wx(const std::string& str)
{
    return !str.empty() ? wxString(str.data(), wxConvUTF8) : wxString();
}

inline std::string wx2str(const wxString str)
{
    return std::string(str.utf8_str());
}

enum {

    ID_CLONE = wxID_HIGHEST + 1,
    ID_SWITCH,
    
    ID_REMOTE_PANEL_LIST,
    ID_REMOTE_PANEL_NAME,
    ID_REMOTE_PANEL_URL,
    ID_REMOTE_PANEL_PUSH_URL,
    ID_REMOTE_PANEL_BUTTON_ADD,
    ID_REMOTE_PANEL_BUTTON_DELETE,
    ID_REMOTE_PANEL_BUTTON_EDIT,
    ID_REMOTE_PANEL_BUTTON_SAVE,
    ID_REMOTE_PANEL_BUTTON_DISCARD,
};

typedef std::map<git2::OId, std::vector<std::string>> ReferenceMap;

class GalapagosApp : public wxApp
{
    DECLARE_EVENT_TABLE();
	
public:
	GalapagosApp();
	
	virtual bool OnInit();

	git2::Repository& getRepository() {return _repo;}

	bool createRepository(const wxString& localPath, bool bare);
	bool loadRepository(const wxString& localPath);
	void refresh();

	CommitListDataModel* getHistoryDataModel(){return _historyDataModel;}

	void updateRepositoryModel();

	void queryCloneRepository();

	wxFileHistory* fileHistory(){return &_fileHistory;}
	wxConfig* config(){return _config;}
	wxMenu* fileHistoryMenu(){return _fileHistoryMenu;}
	
protected:
	wxConfig	 *_config; 
	wxFileHistory _fileHistory;
	wxMenu       *_fileHistoryMenu;
	
	Frame* _frame;

	git2::Repository _repo;

	CommitListDataModel* _historyDataModel;
        
        ReferenceMap _refs;

        Commit* getSelectedCommit();
        
private:
	
	void onOpen(wxCommandEvent& event);
	void onOpenRecent(wxCommandEvent& event);
	void onCreateRepository(wxCommandEvent& event);
	void onCloneRepository(wxCommandEvent& event);
	void onRefresh(wxCommandEvent& event);
        
        void onSwitch(wxCommandEvent& event);
};

DECLARE_APP(GalapagosApp);


#endif // _APP_HPP_

