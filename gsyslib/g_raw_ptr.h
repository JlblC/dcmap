#pragma once
#include "g_gsys.h"

namespace gsys
{
	template<class T>
	class raw_ptr
	{
	public:
		typedef T value_type;

		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
	protected:
		pointer m_ptr;
	public:
		raw_ptr():m_ptr(gsys_null){}
		raw_ptr(pointer pun):m_ptr(pun){}
		raw_ptr(raw_ptr<T> const& ptr):m_ptr(ptr.m_ptr){}

		raw_ptr<T>& operator = (raw_ptr<T> const& ptr)
		{
			m_ptr = ptr.m_ptr;
			return *this;
		}

		void reset(pointer pun){m_ptr = pun;}	  

		// data access
		pointer get(){return m_ptr;}
		const_pointer get()const{return m_ptr;}
		pointer operator ->(){return m_ptr;}
		const_pointer operator ->()const{return m_ptr;}
		reference operator *(){return *m_ptr;}
		const_reference operator *()const{return *m_ptr;}
	};
}