/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * app.cpp
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

#include <wx/strconv.h>
#include <wx/convauto.h>

#include "app.hpp"

#include "create-repository-dialog.hpp"
#include "repository-properties-dialog.hpp"

IMPLEMENT_APP(GalapagosApp)

GalapagosApp::GalapagosApp():
_repo()
{
}

bool GalapagosApp::OnInit()
{
	SetAppName("galapagos");
	SetAppDisplayName("Galapagos");

        git_libgit2_init();
	
	_config = new wxConfig(GetAppName());
	_fileHistory.Load(*_config);
	_fileHistoryMenu = new wxMenu;
	_fileHistory.UseMenu(_fileHistoryMenu);
	_fileHistory.AddFilesToMenu();

	_historyDataModel = new CommitListDataModel;
	//_historyDataModel->IncRef();
	
	_frame = new Frame();
	_frame->Show(TRUE);
	SetTopWindow(_frame);

	return TRUE;
}

bool GalapagosApp::createRepository(const wxString& localPath, bool bare)
{
	if(localPath.IsEmpty())
		return false;

	try
	{
		_repo = git2::Repository::init(localPath.ToStdString(), bare);
	}
	catch(git2::Exception& ex)
	{
		std::cerr << ex.err() << " - " << ex.message() << std::endl;
		return false;
	}

	if(_repo.ok())
	{
		_fileHistory.AddFileToHistory(localPath);
		_fileHistory.Save(*_config);
		_config->Flush();
		
		refresh();
		return true;
	}
	else
	{
		return false;
	}
}

bool GalapagosApp::loadRepository(const wxString& localPath)
{
	if(localPath.IsEmpty())
		return false;

	try
	{
		_repo = git2::Repository::discoverAndOpen(localPath.ToStdString());
	}
	catch(git2::Exception& ex)
	{
		std::cerr << ex.err() << " - " << ex.message() << std::endl;
		return false;
	}

	if(_repo.ok())
	{
		_fileHistory.AddFileToHistory(localPath);
		_fileHistory.Save(*_config);
		_config->Flush();
		
		refresh();
		return true;
	}
	else
	{
		return false;
	}
}

void GalapagosApp::refresh()
{
	if(!_repo.ok())
		_frame->SetTitle( GetAppDisplayName() );
	else if(_repo.isBare())
		_frame->SetTitle( GetAppDisplayName() + " - " +  str2wx(_repo.path()) );
	else
		_frame->SetTitle( GetAppDisplayName() + " - " +  str2wx(_repo.workdir()) );
	updateRepositoryModel();
}

void GalapagosApp::updateRepositoryModel()
{
    if(_repo.ok())
    {
        CommitListDataModel& model = *_historyDataModel;
        
        try
        {
            git2::RevWalk walker = _repo.createRevWalk();
            walker.reset();
            walker.setSorting(GIT_SORT_TIME/*|GIT_SORT_TOPOLOGICAL*/);
            walker.pushHead();
            _repo.foreachReferenceName([&](const std::string& refname)->bool{walker.pushRef(refname); return false;});
            model.fillFromRevWalker(walker, &_repo);
        }
        catch(git2::Exception& ex){
            std::cerr << ex.message() << std::endl;
        }
        
        try
        {
            _repo.foreachReference([&model](git2::Reference ref)->bool{
                model[ref.target()].refs.push_back(ref.name());
                return false;
            });
        }
        catch(git2::Exception& ex){
            std::cerr << ex.message() << std::endl;
        }                
    }
}

void GalapagosApp::queryCloneRepository()
{
	wxMessageBox("Clone");
}

Commit* GalapagosApp::getSelectedCommit()
{
    
}

BEGIN_EVENT_TABLE(GalapagosApp, wxApp)
	EVT_MENU(wxID_OPEN, GalapagosApp::onOpen)
	EVT_MENU(wxID_REFRESH, GalapagosApp::onRefresh)
	EVT_MENU(wxID_NEW, GalapagosApp::onCreateRepository)
	EVT_MENU(ID_CLONE, GalapagosApp::onCloneRepository)

	EVT_MENU(ID_SWITCH, GalapagosApp::onSwitch)

	EVT_COMMAND_RANGE(wxID_FILE1, wxID_FILE9, wxEVT_COMMAND_MENU_SELECTED, GalapagosApp::onOpenRecent)
END_EVENT_TABLE()


void GalapagosApp::onOpen(wxCommandEvent& event)
{
	const wxString& dir = wxDirSelector("Choose a folder");
	loadRepository(dir);
}

void GalapagosApp::onOpenRecent(wxCommandEvent& event)
{
	wxString dir = _fileHistory.GetHistoryFile(event.GetId() - _fileHistory.GetBaseId());
	loadRepository(dir);
}

void GalapagosApp::onCreateRepository(wxCommandEvent& event)
{
	CreateRepositoryDialog dialog;
	if(dialog.ShowModal()==wxID_OK)
	{
		createRepository(dialog.getPath(), dialog.isBare());
	}
}

void GalapagosApp::onCloneRepository(wxCommandEvent& event)
{
	queryCloneRepository();
}

void GalapagosApp::onRefresh(wxCommandEvent& event)
{
	refresh();
}

void GalapagosApp::onSwitch(wxCommandEvent& event)
{
    RepositoryPropertiesDialog dlg(_frame, _repo);
    dlg.ShowModal();
}