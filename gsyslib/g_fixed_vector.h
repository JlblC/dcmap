#pragma once
#ifndef G_FIXED_VECTOR_H
#define G_FIXED_VECTOR_H

#include "g_gsys.h"
#include "g_assert.h"
#include "detail/g_iterators.h"

namespace gsys
{
	// very simple variable size array implementation takes constant memory, no memory allocation at all
	// and used ONLY for trivial constructor, simple types !!!
	template<typename Type,int MaxSize>
	class fixed_vector
	{
/*		BOOST_STATIC_ASSERT(sizeof(Type) <= 8);
		BOOST_STATIC_ASSERT(::boost::has_trivial_constructor<Type>::value &&
						    ::boost::has_trivial_copy<Type>::value&&
			                ::boost::has_trivial_destructor<Type>::value);
*/
		typedef fixed_vector<Type,MaxSize> this_type;
	public:
		typedef Type value_type;
		typedef gsys_size_t size_type;
		typedef gsys_pt_diff difference_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
	private:
		pointer m_pEnd;
		value_type m_Data[MaxSize];
	public:
		typedef detail::ptr_iterator<Type> iterator;
		typedef detail::const_ptr_iterator<Type> const_iterator;

	public:
		fixed_vector():m_pEnd(m_Data){}
		fixed_vector(this_type const& v):m_pEnd(m_Data+v.size())
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
		void pop_back()
		{
			GSYS_ASSERT(m_Data != m_pEnd);
			m_pEnd--;
		}	
		reference at(size_type pos){return *(m_Data+pos);}
		const_reference at(size_type pos)const{return *(m_Data+pos);}

		reference operator [](size_type pos){return *(m_Data+pos);}
		const_reference operator [](size_type pos)const{return *(m_Data+pos);}

		void clear()
		{
			m_pEnd=m_Data;
		}	

		void erase(size_type pos)
		{
			GSYS_ASSERT(!empty());
			memmove(m_Data+pos,m_Data+pos+1,(m_pEnd-(m_Data+pos+1))*sizeof(value_type));
			m_pEnd--;
		}	
		size_type size()const{return m_pEnd-m_Data;}
		size_type capacity()const{return MaxSize;}
		bool empty()const{return m_pEnd-m_Data == 0;}
	};
}

#endif //G_FIXED_VECTOR_H
