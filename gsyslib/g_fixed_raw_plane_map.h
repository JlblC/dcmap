#pragma once
#ifndef G_FIXED_RAW_PLANE_MAP_H
#define G_FIXED_RAW_PLANE_MAP_H

#include "g_gsys.h"
#include "g_assert.h"
#include "detail/g_iterators.h"

namespace gsys
{
	// very simple variable size array implementation takes constant memory, no memory allocation at all
	// and used ONLY for trivial constructor, simple types !!!
	template<typename Key,typename Val,int MaxSize>
	class fixed_raw_plane_map
	{
		/*	BOOST_STATIC_ASSERT(sizeof(Val) <= 8);
		BOOST_STATIC_ASSERT(sizeof(Key) <= 8);
		BOOST_STATIC_ASSERT(::boost::has_trivial_constructor<Val>::value &&
		::boost::has_trivial_copy<Val>::value&&
		::boost::has_trivial_destructor<Val>::value);

		BOOST_STATIC_ASSERT(::boost::has_trivial_constructor<Key>::value &&
		::boost::has_trivial_copy<Key>::value&&
		::boost::has_trivial_destructor<Key>::value);*/

		typedef fixed_raw_plane_map<Key,Val,MaxSize> this_type;
	public:
		typedef std::pair<Key,Val> value_type;
		typedef gsys_size_t size_type;
		typedef gsys_pt_diff difference_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
	private:
		pointer m_pEnd;
		value_type m_Data[MaxSize+1];
	public:
		typedef detail::ptr_iterator<value_type> iterator;
		typedef detail::const_ptr_iterator<value_type> const_iterator;

	public:
		fixed_raw_plane_map():m_pEnd(m_Data){}
		fixed_raw_plane_map(this_type const& v):m_pEnd(m_Data+v.size())
		{
			memcpy(m_Data,v.m_Data,size());
		}

		iterator begin(){return iterator(m_Data);}
		iterator end(){return iterator(m_pEnd);}
		const_iterator begin()const{return const_iterator(m_Data);}
		const_iterator end()const{return const_iterator(m_pEnd);}

		reference front(){GSYS_ASSERT(m_Data != m_pEnd);return *(m_Data);}
		reference back(){GSYS_ASSERT(m_Data != m_pEnd);return *(m_pEnd-1);}
		const_reference front()const{GSYS_ASSERT(m_Data != m_pEnd);return *(m_Data);}
		const_reference back()const{GSYS_ASSERT(m_Data != m_pEnd);return *(m_pEnd-1);}

		void push_back(value_type val)
		{
			GSYS_ASSERT(size() < MaxSize);
			*m_pEnd = val;
			m_pEnd++;
		}

		void push_back(Key key,Val val)
		{
			GSYS_ASSERT(size() < MaxSize);
			m_pEnd->first = key;
			m_pEnd->second = val;
			m_pEnd++;
		}

		void pop_back()
		{
			GSYS_ASSERT(m_Data != m_pEnd);
			m_Size--;
		}	
		reference at(size_type pos){return *(m_Data+pos);}
		const_reference at(size_type pos)const{return *(m_Data+pos);}

		reference operator [](size_type pos){return *(m_Data+pos);}
		const_reference operator [](size_type pos)const{return *(m_Data+pos);}

		void clear()
		{
			m_pEnd=m_pData;
		}	

		const_iterator find(Key key)const
		{
			m_pEnd->first = key;
			const_pointer ptr = m_Data;
			while(ptr->first != key && ptr != m_pEnd)++ptr;
			return const_iterator(ptr);
		}
		iterator find(Key key)
		{
			m_pEnd->first = key;
			pointer ptr = m_Data;
			while(ptr->first != key)++ptr;
			return iterator(ptr);
		}	
		size_type size()const{return m_pEnd-m_Data;}

		bool erase(Key key) // TODO: Test routine:  bool erase(Key key)
		{
			// locate key 
			m_pEnd->first = key;
			pointer ptr = m_Data;
			while(ptr->first != key)++ptr;
			if(ptr == m_pEnd)return false;
			// shift values back
			memmove(ptr,ptr+1,(m_pEnd-(ptr+1))*sizeof(value_type));
			m_pEnd--;
			return true;
		}
	};
}

#endif //G_FIXED_RAW_PLANE_MAP_H
