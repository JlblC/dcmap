#pragma once
#ifndef G_STATIC_ARRAY2D_H
#define G_STATIC_ARRAY2D_H

#include "g_gsys.h"
#include "g_rect.h"
#include "g_point.h"
#include "g_assert.h"

namespace gsys
{
	template<typename T, gsys_size_t _P>
	class array2d_static_iterator
	{
		T* pValue;
	public:
		typedef T value_type;
		typedef gsys_size_t size_type;
		typedef gsys_size_t difference_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
	public:
	};

	template<typename T, gsys_size_t _X,gsys_size_t _Y>
	class static_array2d
	{
	public:
		typedef static_array2d<T,_X,_Y> this_type ;
		typedef T value_type;
		typedef gsys_size_t size_type;
		typedef gsys_size_t difference_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
	private:
		value_type m_values[_X][_Y];
	public:
		size_type size()const{return _X*_Y;}
		size_type raw_size()const{return _X*_Y*sizeof(value_type);}
		size_type width()const{return _X;}
		size_type height()const{return _Y;}
	public:
		typedef array2d_static_iterator<T,sizeof(T)*_X> iterator;
		typedef const array2d_static_iterator<T,sizeof(T)*_X> const_iterator;
	public:
		pointer get(){return &m_values[0][0];}
		const_pointer get()const{return &m_values[0][0];}

		pointer row(size_type y){GSYS_ASSERT(y < _Y);return &m_values[y][0];}
		const_pointer row(size_type y)const{GSYS_ASSERT(y < _Y);return &m_values[y][0];}

		pointer operator[](size_type ps){return m_values[ps];}
		const_pointer operator[](size_type ps)const{return m_values[ps];}

		reference operator()(size_type at){return *(((pointer)m_values)+at);}
		const_reference operator()(size_type at)const{return *(((const_pointer)m_values)+at);}

		reference operator()(size_type px,size_type py){return *((pointer)m_values+(py*_X+px));}
		const_reference operator()(size_type px,size_type py)const{return *((const_pointer)m_values+(py*_X+px));}

		template<class T> reference operator()(tPoint<T> const& pt){return (*this)(pt.x,pt.y);}
		template<class T> const_reference operator()(tPoint<T> const& pt)const{return (*this)(pt.x,pt.y);}

		friend bool operator<(this_type const& v1, this_type const& v2)
		{
			for(int i=0; i<(_X*_Y); i++)
			{
				if(v1(i) == v2(i))continue;

				if( v1(i) < v2(i) )return true;
				else return false;
			}
			return false;
		}
	};
}

#endif