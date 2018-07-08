/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * commit.hpp
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

#ifndef _COMMIT_HPP_
#define _COMMIT_HPP_

#include <git2pp.hpp>

#include <wx/dataview.h>
#include <wx/variant.h>

#include <map>
#include <set>
#include <vector>

class Commit;
class CommitListDataModel;


template <class Type>
class Vector : public std::vector<Type>
{
public:
	typedef std::vector<Type> Parent;
	typedef Vector<Type> Self;
        
        typedef typename Parent::iterator iterator;
        typedef typename Parent::const_iterator const_iterator;
	
	Vector(){}
	Vector(const Parent& vec):Parent(vec){}

	void ensure(size_t idx)
	{
		if(idx>=Parent::size())
		{
			Parent::resize(idx+1);
		}
	}

	void ensure(size_t idx, const Type& val)
	{
		if(idx>=Parent::size())
		{
			Parent::resize(idx+1, val);
		}
	}

	Type& get(size_t idx)
	{
		ensure(idx);
		return Parent::at(idx);
	}

	void set(size_t idx, const Type& value)
	{
		ensure(idx);
		Parent::at(idx) = value;
	}
	
};



class GraphicLine: public wxObject, public Vector<uint8_t>
{
public:
	wxDECLARE_CLASS(GraphicLine);
		
	GraphicLine();
	GraphicLine(const GraphicLine& line);

	enum {
		NOTHING = 0,
		COMMIT  = 1,
		CHILD   = 1 << 1,
		PARENT  = 1 << 2,
		PASS    = CHILD + PARENT,
		MERGE   = 1 << 3,
		FORK    = 1 << 4
	};

	void clear(size_t idx);
	void clear();

	void add(size_t idx, uint8_t flags);
	void rem(size_t idx, uint8_t flags);

	size_t firstEmpty()const;
	int commit()const;
	
	void shrink();

};
DECLARE_VARIANT_OBJECT(GraphicLine)





class Commit: public git2::Commit 
{
public:
	Commit(const git2::Commit& commit);
	Commit(const Commit& commit);

	void setChild(const git2::OId& child);
	void clearChildren();
	
	std::vector<git2::OId> children;
	GraphicLine     	   line;
        
        std::vector<std::string> refs;

};




class CommitListDataModel : public wxDataViewVirtualListModel
{
public:
	CommitListDataModel();
	CommitListDataModel(git2::Repository* repo);
	virtual ~CommitListDataModel();

	void setRepository(git2::Repository* repo);
	
	enum CommitListDataModelColumns {
		GRAPH,
		SHA1,
		DATE,
		AUTHOR,
		COMMITTER,
		MESSAGE,
		COLUMN_COUNT
	};

	void clear();
	void fillFromRevWalker(git2::RevWalk& walker, git2::Repository* repo = NULL);	

	Commit getCommit(size_t idx)const{return _commitList[idx];}
	
	virtual unsigned int GetColumnCount()const;
	virtual wxString GetColumnType(unsigned int col)const;

	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col);
	virtual bool GetAttrByRow( unsigned int row, unsigned int col, wxDataViewItemAttr &attr )const;

        Commit& operator[](git2::OId oid);
        
protected:
	void updateChildren();
	void updateGraphicLines();
	Commit& addCommit(const Commit& commit);
	

	git2::Repository* _repo;
	
	std::vector<Commit> _commitList;
	std::map<git2::OId, size_t> _commitIndexMap;

	size_t _headIndex;

};


class GraphicalLineRenderer : public wxDataViewCustomRenderer
{
public:
	GraphicalLineRenderer();

    virtual bool Render( wxRect rect, wxDC *dc, int state );
    virtual wxSize GetSize() const;
    virtual bool SetValue( const wxVariant &value );
	virtual bool GetValue( wxVariant &value )const;
	
protected:
	GraphicLine _line;
};


#endif // _COMMIT_HPP_

