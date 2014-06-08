#pragma once
#ifndef G_STATIC_STRING_H
#define G_STATIC_STRING_H

#include "g_gsys.h"
#include <ostream>

namespace gsys
{
	class static_string
	{
		char const* m_ptr;
	public:
		typedef char value_type;
		typedef uint size_type;
		typedef value_type const* const_pointer;
	public:
		// constructions
		static_string():m_ptr(""){}
		static_string(const_pointer str):m_ptr(str){}
		//assigment
		static_string& operator = (static_string const& str)
		{
			m_ptr = str.m_ptr;
			return *this;
		}

		//data access
		operator const_pointer()const{return m_ptr;}
		operator std::string()const{return std::string(m_ptr);}
		const_pointer get()const{return m_ptr;}

		size_type lenght()const{return (size_type)strlen(m_ptr);}

		value_type operator[](size_type n)const{return m_ptr[n];}

        bool operator > (static_string const& str)const
			{return strcmp(m_ptr,str.m_ptr) > 0;}
        bool operator < (static_string const& str)const
			{return strcmp(m_ptr,str.m_ptr) < 0;}
        bool operator == (static_string const& str)const
			{return strcmp(m_ptr,str.m_ptr) == 0;}
        bool operator != (static_string const& str)const
			{return strcmp(m_ptr,str.m_ptr) != 0;}

		friend inline std::ostream& operator<<(std::ostream& os,static_string const& str)
		{
			return os << str.m_ptr;
		}
	};
}

#endif