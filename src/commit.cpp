/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * commit.cpp
 * Copyright (C) 2018 Emilien Kia <emilien.kia+dev@gmail.com>
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

#include "commit.hpp"

#include <algorithm>


//
// GraphicLine
//

wxIMPLEMENT_CLASS(GraphicLine, wxObject);
IMPLEMENT_VARIANT_OBJECT(GraphicLine);


GraphicLine::GraphicLine()
{
}

GraphicLine::GraphicLine(const GraphicLine& line):
Vector<uint8_t>((Vector<uint8_t>)line)
{
}

void GraphicLine::clear()
{
	Vector<uint8_t>::clear();
}

void GraphicLine::clear(size_t idx)
{
	if(idx<size())
	{
		at(idx) = NOTHING;
	}
}

size_t GraphicLine::firstEmpty()const
{
	for(size_t n=0; n<size(); ++n)
	{
		if(at(n)==NOTHING)
		{
			return n;
		}
	}
	return size();
}

int GraphicLine::commit()const
{
	for(size_t n=0; n<size(); ++n)
	{
		if((at(n)&COMMIT)!=0)
		{
			return (int)n;
		}
	}
	return -1;
}

void GraphicLine::add(size_t idx, uint8_t flags)
{
	ensure(idx);
	at(idx) |= flags;
}

void GraphicLine::rem(size_t idx, uint8_t flags)
{
	ensure(idx);
	at(idx) &= ~flags;	
}

void GraphicLine::shrink()
{
	while(!empty())
	{
		if(back()==NOTHING)
			pop_back();
		else
			break;
	}
}

//
// Commit
//

Commit::Commit(const git2::Commit& commit):
git2::Commit(commit)
{
}

Commit::Commit(const Commit& commit):
git2::Commit((git2::Commit)commit),
children(commit.children),
line(commit.line)
{
}

void Commit::setChild(const git2::OId& child)
{
	children.push_back(child);
}

void Commit::clearChildren()
{
	children.clear();
}





//
// CommitListDataModel
//

CommitListDataModel::CommitListDataModel():
_repo(NULL)
{
}


CommitListDataModel::CommitListDataModel(git2::Repository* repo):
_repo(repo)
{
}


CommitListDataModel::~CommitListDataModel()
{
}

void CommitListDataModel::setRepository(git2::Repository* repo)
{
	_repo = repo;
	clear();
}


void CommitListDataModel::clear()
{
	_commitIndexMap.clear();
	_commitList.clear();
	Reset(_commitList.size());
}

void CommitListDataModel::fillFromRevWalker(git2::RevWalk& walker, git2::Repository* repo)
{
	if(repo!=NULL)
	{
		_repo = repo;
	}

	clear();
	
	git2::OId oid;
	while(walker.next(oid))
	{
		try {
			git2::Object obj = _repo->lookup(oid);
			if(GIT_OBJ_COMMIT==obj.getType()){
				git2::Commit commit = obj.toCommit();
				addCommit(Commit(commit));
			}
		} catch(git2::Exception& ex) {
//			std::cerr << ex.err() << " - " << ex.message() << std::endl;				
		}
	}
	updateChildren();
	updateGraphicLines();

	_headIndex = _commitIndexMap[_repo->head().target()];
	
	Reset(_commitList.size());
}

Commit& CommitListDataModel::addCommit(const Commit& commit)
{
	_commitIndexMap[commit.oid()] = _commitList.size();
	_commitList.push_back(commit);
        return _commitList.back();
}

void CommitListDataModel::updateChildren()
{
	for(Commit& commit : _commitList)
	{
		commit.clearChildren();
	}

	for(const Commit& commit : _commitList)
	{
//std::cout << " + " << commit.oid().format() << " - " << commit.parentCount() << std::endl;
		for(size_t p=0; p<commit.parentCount(); ++p)
//		for(int p=commit.parentCount()-1; p>=0; --p)
		{			
			git2::OId poid = commit.parentId(p);
			auto parent = _commitIndexMap.find(poid);
			if(parent!=_commitIndexMap.end())
			{
				size_t pidx = parent->second;
				_commitList[pidx].setChild(commit.oid());
			}
		}
	}
}

template<class Type, class Iterator>
int findFirstOfOrFirstEmpty(Iterator first, Iterator last, const Type& value, const Type& empty)
{
	Iterator foundValue = std::find(first, last, value);
	if(foundValue!=last)
	{
		// Find value
		return foundValue - first;
	}

	Iterator foundEmpty = std::find(first, last, empty);
	if(foundEmpty!=last)
	{
		// Find empty
		return foundEmpty - first;
	}

	// Not found
	return last - first;
}


void CommitListDataModel::updateGraphicLines()
{
	// List of next expected commit for each column
	Vector<int> line;

	for(int row = 0; row < _commitList.size(); ++row)
	{
		// Get and reset line of current commit
		Commit& commit = _commitList.at(row);
		commit.line.clear();

		// Find a column for current commit (first used value or first empty)
		int currCommitColumn = findFirstOfOrFirstEmpty(line.begin(), line.end(), row, -1);
		line.ensure(currCommitColumn, -1);
		
		// Set commit symbol
		commit.line.set(currCommitColumn, GraphicLine::COMMIT);

		// Iterate each column
		for(int col=0; col<line.size(); ++col)
		{			
			int colval = line.get(col);
			
			if(colval == row)
			{
				if(col == currCommitColumn)
					commit.line.add(col, GraphicLine::CHILD);
				else
					commit.line.add(col, GraphicLine::FORK);
				
				line.set(col, -1);
			}
			else if(colval != -1)
			{
				commit.line.add(col, GraphicLine::PASS);
			}
		}

		// Iterate on each parent of current commit.
		for(size_t p=0; p<commit.parentCount(); ++p)
		{
			git2::OId poid = commit.parentId(p);
			auto parent = _commitIndexMap.find(poid);
			if(parent!=_commitIndexMap.end())
			{
				size_t pidx = parent->second;
				
				// Find a column for child commit (first used value or first empty)
				int parentCommitColumn = findFirstOfOrFirstEmpty(line.begin(), line.end(), (int)pidx, -1);
				line.set(parentCommitColumn, pidx);

				if(parentCommitColumn == currCommitColumn)
					commit.line.add(parentCommitColumn, GraphicLine::PARENT);
				else
					commit.line.add(parentCommitColumn, GraphicLine::MERGE);
			}
		}
	}
	
	
/*	for(int row = _commitList.size()-1; row>=0; --row)
	{
		// Get and reset line of current commit
		Commit& commit = _commitList.at(row);
		commit._line.clear();

		// Find a column for current commit (first used value or first empty)
		int currCommitColumn = findFirstOfOrFirstEmpty(line.begin(), line.end(), row, -1);
		line.ensure(currCommitColumn, -1);

		// Set commit symbol
		commit._line.set(currCommitColumn, GraphicLine::COMMIT);
		
		// Iterate each column
		for(int col=0; col<line.size(); ++col)
		{			
			int colval = line.get(col);
			
			if(colval == row)
			{
				if(col == currCommitColumn)
					commit._line.add(col, GraphicLine::PARENT);
				else
					commit._line.add(col, GraphicLine::MERGE);
				
				line.set(col, -1);
			}
			else if(colval != -1)
			{
				commit._line.add(col, GraphicLine::PASS);
			}
		}

		// Iterate on each child of current commit.
		for(git2::OId child: commit._children)
		{
			auto iter = _commitIndexMap.find(child);
			if(iter!=_commitIndexMap.end())
			{
				size_t child = iter->second;
				
				// Find a column for child commit (first used value or first empty)
				int childCommitColumn = findFirstOfOrFirstEmpty(line.begin(), line.end(), (int)child, -1);
				line.set(childCommitColumn, child);

				if(childCommitColumn == currCommitColumn)
					commit._line.add(childCommitColumn, GraphicLine::CHILD);
				else
					commit._line.add(childCommitColumn, GraphicLine::FORK);
			}
		}
	}
*/
}

unsigned int CommitListDataModel::GetColumnCount()const
{
	return CommitListDataModelColumns::COLUMN_COUNT;
}

wxString CommitListDataModel::GetColumnType(unsigned int col)const
{
	static GraphicLine line;
	static wxVariant var(&line);
	
	switch(col)
	{
		case CommitListDataModelColumns::GRAPH:
			return var.GetType();
		case CommitListDataModelColumns::SHA1:
		case CommitListDataModelColumns::DATE:
		case CommitListDataModelColumns::AUTHOR:
		case CommitListDataModelColumns::COMMITTER:
		case CommitListDataModelColumns::MESSAGE:
			return "string";
		default:
			return "";
	}
}

void CommitListDataModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const
{
	if(row<_commitList.size())
	{
		const Commit& commit = _commitList[row];
		switch(col)
		{
			case GRAPH:
				variant << commit.line;
				break;				
			case SHA1:
				variant = wxString(commit.oid().format().data(),wxConvUTF8);
				break;
			case DATE:
				variant = wxDateTime(commit.time()).Format();
				break;
			case AUTHOR:
				variant = wxString(commit.author().name().data(),wxConvUTF8);
				break;
			case COMMITTER:
				variant = wxString(commit.committer().name().data(),wxConvUTF8);
				break;
			case MESSAGE:
                        {
                                std::string str;
                                for(const std::string& refname : commit.refs)
                                {
                                    try
                                    {
                                        git2::Reference ref = _repo->lookupReference(refname);
                                        str += "[" + ref.shorthand() + "] ";
                                    }
                                    catch(...){}
                                }
                                str += commit.shortMessage();
				variant = wxString(str.data(),wxConvUTF8);
				break;
                        }
			default:
				break;
		}

	}
}

bool CommitListDataModel::GetAttrByRow( unsigned int row, unsigned int col, wxDataViewItemAttr &attr )const
{
	if(row==_headIndex)
	{
		attr.SetBold(true);
		return true;
	}
	else
	{
		return false;
	}
}

bool CommitListDataModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col)
{
	return false;
}

Commit& CommitListDataModel::operator[](git2::OId oid)
{
    return _commitList[_commitIndexMap[oid]];
}

//
// GraphicalLineRenderer
//

GraphicalLineRenderer::GraphicalLineRenderer():
wxDataViewCustomRenderer("GraphicLine")
{
}

bool GraphicalLineRenderer::Render( wxRect rect, wxDC *dc, int state )
{
	static const int demiwidth = 8;
	
	int top = rect.GetTop() - 2,
	    middle = (rect.GetTop() + rect.GetBottom() ) / 2,
		bottom = rect.GetBottom() + 2;
	
    dc->SetBrush( *wxBLACK_BRUSH );
    dc->SetPen( *wxBLACK_PEN );

	int commitcol = _line.commit();
	int commitleft   = rect.GetLeft() + commitcol*2*demiwidth,
		commitcenter = rect.GetLeft() + (commitcol*2+1)*demiwidth,
		commitright  =  rect.GetLeft() + (commitcol+1)*2*demiwidth;

	for(int col=0; col<_line.size(); ++col)
	{
		uint8_t state = _line.get(col);

		int left   = rect.GetLeft() + col*2*demiwidth,
			center = rect.GetLeft() + (col*2+1)*demiwidth,
			right  =  rect.GetLeft() + (col+1)*2*demiwidth;
		
		if(state & GraphicLine::CHILD)
		{
			dc->DrawLine(center, top, center, middle);
		}
		if(state & GraphicLine::PARENT)
		{
			dc->DrawLine(center, middle, center, bottom);
		}
		if(state & GraphicLine::FORK)
		{
			dc->DrawLine(center, top, commitcenter, middle);
		}
		if(state & GraphicLine::MERGE)
		{
			dc->DrawLine(commitcenter, middle, center, bottom);
		}
		if(state & GraphicLine::COMMIT)
		{
			dc->DrawCircle(center, middle, 4);
		}
	}
    return true;
}

wxSize GraphicalLineRenderer::GetSize() const
{
    return wxSize(11*_line.size(),21);
}

bool GraphicalLineRenderer::SetValue( const wxVariant &value )
{
	if(value.GetType()=="GraphicLine")
	{
		_line << value;
		_line.shrink();
		return true;
	}
	else
	{
		return false;
	}
}

bool GraphicalLineRenderer::GetValue( wxVariant &WXUNUSED(value) ) const
{
	return true;
}

