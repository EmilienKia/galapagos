/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * history-detail-panel.hpp
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

#ifndef _HISTORY_DETAIL_PANEL_HPP_
#define _HISTORY_DETAIL_PANEL_HPP_

class HistoryDetailPanel: public wxPanel 
{
public:
	HistoryDetailPanel(wxWindow* parent, wxWindowID id);

	void SetSelection(size_t pos);
	
protected:

private:
	wxStaticText *sha1, *date, *author, *committer, *subject, *message;

	wxListBox *parents, *children;
	
	void CommonInit();
};

#endif // _HISTORY_DETAIL_PANEL_HPP_

