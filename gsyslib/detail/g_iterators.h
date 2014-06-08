#pragma once
#ifndef DETAIL_G_ITERATORS_H
#define DETAIL_G_ITERATORS_H

#include "g_gsys.h"
#include "g_assert.h"

namespace gsys
{
	namespace detail
	{
		template<typename Type>
		class ptr_iterator
		{
			typedef ptr_iterator<Type> this_type;
		public:
			typedef Type value_type;
			typedef gsys_size_t size_type;
			typedef gsys_pt_diff difference_type;
			GSYSDETAIL_TYPEDEF_VALUE_POINTERS
		private:
			pointer m_ptr;
		public:
			explicit ptr_iterator(pointer ptr):m_ptr(ptr){}
		public:
			ptr_iterator():m_ptr(0){}
			ptr_iterator(this_type const& it):m_ptr(it.m_ptr){}
			this_type& operator = (this_type const& it){m_ptr = it.m_ptr;return *this;}

			pointer operator ->(){return m_ptr;}
			reference operator *(){return *m_ptr;}

			this_type& operator ++ ()
			{
				++m_ptr;
				return *this;
			}

			this_type& operator ++ (int)
			{
				this_type it=*this;
				++*this;
				return it;
			}

			bool operator == (this_type const& it){return m_ptr == it.m_ptr;}
			bool operator != (this_type const& it){return m_ptr != it.m_ptr;}
		};

		template<typename Type>
		class const_ptr_iterator
		{
			typedef const_ptr_iterator<Type> this_type;
		public:
			typedef Type value_type;
			typedef gsys_size_t size_type;
			typedef gsys_pt_diff difference_type;
			GSYSDETAIL_TYPEDEF_VALUE_POINTERS
		private:
			const_pointer m_ptr;
		public:
			explicit const_ptr_iterator(const_pointer ptr):m_ptr(ptr){}
		public:
			const_ptr_iterator():m_ptr(0){}
			const_ptr_iterator(this_type const& it):m_ptr(it.m_ptr){}
			this_type& operator = (this_type const& it){m_ptr = it.m_ptr;return *this;}

			const_pointer operator ->(){return m_ptr;}
			const_reference operator *(){return *m_ptr;}

			this_type& operator ++ ()
			{
				++m_ptr;
				return *this;
			}

			this_type& operator ++ (int)
			{
				this_type it=*this;
				++*this;
				return it;
			}

			bool operator == (this_type const& it){return m_ptr == it.m_ptr;}
			bool operator != (this_type const& it){return m_ptr != it.m_ptr;}
		};
	}
}

#endif