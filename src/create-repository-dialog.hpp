/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * create-bar-repository-dialog.hpp
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

#ifndef _CREATE_REPOSITORY_DIALOG_HPP_
#define _CREATE_REPOSITORY_DIALOG_HPP_

#include <wx/checkbox.h>
#include <wx/filepicker.h>


class CreateRepositoryDialog: public wxDialog 
{
public:
	CreateRepositoryDialog();

	wxString getPath()const;
	bool     isBare()const;
	
protected:
	void CommonInit();
	
private:
	wxDirPickerCtrl *_path;
	wxCheckBox*      _bare;
};

#endif // _CREATE_REPOSITORY_DIALOG_HPP_

