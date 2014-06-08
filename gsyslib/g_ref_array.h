#pragma once
#ifndef g_ref_array_h__
#define g_ref_array_h__

#include "g_utility.h"

namespace gsys
{
	template<class T>
	class ref_array
	{
	public:
		typedef T value_type;
		typedef gsys_unsigned size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
	public:
		reference operator[](size_type n){GSYS_ASSERT(_begin && _begin+n < _end);return _begin+n;} 
		const_reference operator[](size_type n)const{GSYS_ASSERT(_begin && _begin+n < _end);return _begin+n;} 
		size_type size()const{return _end-_begin;}
		bool empty()const{return _end==_begin;}
		typedef pointer iterator;
		typedef const_pointer const_iterator;
		iterator begin(){return _begin;}
		iterator end(){return _end;}
		const_iterator begin()const{return _begin;}
		const_iterator end()const{return _end;}
		reference front(){return *_begin;}
		reference back(){return *_end;}
		const_reference front()const{return *_begin;}
		const_reference back()const{return *_end;}
		pointer get(){return _begin;}
		const_pointer get()const{return _begin;}
	public:
		ref_array(pointer begin,pointer end):_begin(begin),_end(end){GSYS_ASSERTARRAY(_begin,size());}
		ref_array(pointer begin,size_type sz):_begin(begin),_end(begin+sz){GSYS_ASSERTARRAY(_begin,size());}
		ref_array():_begin(0),_end(0){}
	public:
		void init(pointer begin,pointer end)
		{
			_begin = begin;_end = end;
			GSYS_ASSERTARRAY(_begin,size());
		}
		void init(pointer begin,size_type sz)
		{
			_begin = begin;_end = begin+sz;
			GSYS_ASSERTARRAY(_begin,size());
		}

		void clear()
		{
			_begin = 0;_end = 0;
		}

	private:
		pointer _begin;
		pointer _end;
	};
}

#endif // g_ref_array_h__