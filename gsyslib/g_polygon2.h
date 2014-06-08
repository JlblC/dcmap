#pragma once
#ifndef G_POLYGON2_H
#define G_POLYGON2_H

#include "g_gsys.h"
#include "g_line2.h"
#include "g_rect.h"

#include <list>

namespace gsys
{
	class polygon2
	{
		typedef list<vector2> VertexList;
	public:
		typedef VertexList::iterator iterator; 
	private:
		VertexList m_VertexList;
		iterator m_itCurrent;

		iterator _cw()
		{
			iterator it = m_itCurrent;
			it++;
			if(it==m_VertexList.end())
				it = m_VertexList.begin();
			return it;
		}
		iterator _ccw()
		{
			if(m_itCurrent == m_VertexList.begin())
				return --m_VertexList.end();
			else return --iterator(m_itCurrent);
		}	
	
	public:
		polygon2()
		{
			m_itCurrent = m_VertexList.begin();
		}
		polygon2(polygon2& poly)
		{
			m_VertexList = poly.m_VertexList;
			m_itCurrent = m_VertexList.begin();
		}

		polygon2& operator = (polygon2& poly)
		{
			m_VertexList = poly.m_VertexList;
			m_itCurrent = m_VertexList.begin();
			return *this;
		}

		vector2& v(){return *m_itCurrent;}
		vector2& cw(){return *_cw();}
		vector2& ccw(){return *_ccw();}

		edge2 edge(){return edge2(*m_itCurrent,*_cw());}

		void advance_cw(){m_itCurrent = _cw();}
		void advance_ccw(){m_itCurrent = _ccw();}

		void advance_cw(int n){for(int i=0;i<n;i++)advance_cw();}
		void advance_ccw(int n){for(int i=0;i<n;i++)advance_ccw();}
		
		void reset_pos(){m_itCurrent = m_VertexList.begin();}

		gsys_size_t size()const{return (gsys_size_t)m_VertexList.size();}

		vector2& add(vector2 const& vt){return insert_after(vt);}

		vector2& insert_after(vector2 const& vt)
		{
			if(m_itCurrent==m_VertexList.end())
			{
				m_VertexList.push_back(vt);
				m_itCurrent = m_VertexList.begin();
			}
			else
			{
				m_itCurrent++;
				m_itCurrent = m_VertexList.insert(m_itCurrent,vt);
			}
			return *m_itCurrent;
		}

		vector2& insert_before(vector2 const& vt)
		{
			m_itCurrent = m_VertexList.insert(m_itCurrent,vt);
			return *m_itCurrent;
		}
		
		template<template<typename T> class C> 
		vector2& insert(C<vector2> const& vec)
		{
			m_itCurrent = m_VertexList.insert(m_itCurrent,vec.begin(),vec.end());
			return *m_itCurrent;
		}

		void remove()
		{
			m_itCurrent = m_VertexList.erase(m_itCurrent);
			if(m_itCurrent==m_VertexList.end())m_itCurrent = m_VertexList.begin();
		}

		void clear()
		{
			m_VertexList.clear();
			m_itCurrent = m_VertexList.begin();
		}

		void build_circumscribed_rect(frect & rc);

	};

	bool clip_polygon2(polygon2&s,polygon2 &p,polygon2& result);
	bool clip_polygon2_to_edge2(polygon2& s,edge2 const& e,polygon2& r);
	bool point_inside_convex_polygon(polygon2 & poly,vector2 const& pt);

}

#ifdef GSYSDETAIL_FORCE_INLINE
#	include "src/inc/g_polygon2.hpp"
#endif 

#endif // G_LINE3_H
