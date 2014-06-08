#pragma once

#include "g_gsys.h"
#include "g_staticstring.h"
#include "g_binstream.h"
#include "g_ref_string.h"
#include <set>
#include "boost/shared_ptr.hpp"

namespace gsys
{
	namespace detail
	{
		class string_collector
		{
			class entry
			{
			public:
				boost::shared_ptr<ref_string> ptr;

				entry(){}
				entry(ref_string const& str)
				{
					ptr.reset(new ref_string(str));
				}
				const char* get()const{return ptr->get();}

				bool operator < (entry const& ent)const
				{
					return *ptr < *(ent.ptr);
				}
			};

			set<entry> m_setStrings;
		public:
			char const* insert(char const* ptr)
			{
				set<entry>::iterator it = 
					m_setStrings.find(ref_string(ptr));
				if(it == m_setStrings.end()) // add string
				{
					pair<set<entry>::iterator, bool> r = 
						m_setStrings.insert(ref_string().make_copy(ptr));
					if(!r.second) return (const char*)gsys_null;
					set<entry>::iterator sit = r.first;
					return r.first->get();
				}
				else
					return it->get();
			}
			char const* insert(static_string const& str)
			{
				return insert_static(str.get());
			}
			char const* insert_static(char const* ptr)
			{
				set<entry>::iterator it = 
					m_setStrings.find(ref_string(ptr));
				if(it == m_setStrings.end()) // add string
				{
					pair<set<entry>::iterator, bool> r = 
						m_setStrings.insert(ref_string(ptr));
					if(!r.second) return (const char*)gsys_null;
					return r.first->get();
				}
				else
					return it->get();
			}		
		};
		extern string_collector g_string_collector;
	};

	class collected_string
	{
		const char* ptr;
	public:
		// constructors
		collected_string():ptr(0){}
		~collected_string(){}

		collected_string(collected_string const& cstr):ptr(cstr.ptr){}
		collected_string& operator = (collected_string const& str)
			{ptr = str.ptr;return *this;}

		// conversions from

		collected_string(char const* ch):
			ptr(detail::g_string_collector.insert(ch)){}
		collected_string(std::string const& str):
			ptr(detail::g_string_collector.insert(str.c_str())){}
		collected_string(static_string const& str):
			ptr(detail::g_string_collector.insert(str)){}
		
		collected_string& operator = (char const* ch)
			{ptr =detail::g_string_collector.insert(ch);return *this;}
		collected_string& operator = (std::string const& str)
			{ptr =detail::g_string_collector.insert(str.c_str());return *this;}
		collected_string& operator = (static_string const& str)
			{ptr =detail::g_string_collector.insert(str);return *this;}

		// conversions to
		operator std::string (){return std::string(ptr);}
		operator bool(){return ptr != 0;}

		// operations
		bool operator < (collected_string const& ref)const
			{return ptr < ref.ptr;}
		bool operator > (collected_string const& ref)const
			{return ptr > ref.ptr;}
		bool operator == (collected_string const& ref)const
			{return ptr == ref.ptr;}

		char const* get(){return ptr;} 
		char const* c_str(){return ptr;} 

		friend inline std::ostream& operator<<(std::ostream& os,collected_string const& v)
		{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
			if(v.ptr) os << v.ptr;	
			return os;
		}

		friend inline std::istream& operator>>(std::istream& is,collected_string & v)
		{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
			std::string s;
			is >> s;
			v = s;
			return is;
		}

		friend inline binostream& operator<<(binostream& os,collected_string const& v)
		{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
			if(v.ptr) os << string(v.ptr);	
			return os;
		}

		friend inline binistream& operator>>(binistream& is,collected_string & v)
		{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
			std::string s;
			is >> s;
			v = s;
			return is;
		}
	};


	typedef collected_string clstring;
}





