#pragma once
#ifndef G_POOLED_STRING_H
#define G_POOLED_STRING_H
#include "g_gsys.h"
#include "g_gstring.h"
#include <set>

/*
*	Simple class for string analog to STL
*   Currently provide only minimal functonality
*/

#define GSYS_IMPLEMENT_STRING_POOL_MANAGER \
	gsys::string_pool g_StrPool;gsys::string_pool_interface* gsys::g_pStringPool = &g_StrPool;\
	gsys::wstring_pool g_WStrPool;gsys::wstring_pool_interface* gsys::g_pWideStringPool = &g_WStrPool;


namespace gsys
{
	template<typename CharT,typename _CharCL=basic_char_classify<CharT> >
	class basic_string_pool_interface
	{
	public:
		virtual CharT const* GSYS_API pool(CharT const* str)=0;
		virtual CharT const* GSYS_API pool(CharT const* str,size_t sz)=0;
	};

	template<typename CharT,typename _CharCL=basic_char_classify<CharT> >
	class basic_string_pool : public basic_string_pool_interface<CharT,_CharCL>
	{
		struct pool_data
		{
			size_t size;
			CharT * ptr;
			pool_data(CharT * p,size_t s):size(s),ptr(p){}
		};

		struct comparator
		{
			bool operator()(pool_data const& a,pool_data const& b)const
			{
				if(a.size == b.size)	return  _CharCL::compare_i(a.ptr,b.ptr) < 0;
				return a.size < b.size;
			}
		};

		typedef std::set<pool_data,comparator> SetStrings;
		SetStrings m_setStrings;
	public:
		basic_string_pool(){}
		~basic_string_pool(){free_pooled_data();}

		virtual CharT const* GSYS_API pool(CharT const* str)
		{
			return basic_string_pool::pool(str,_CharCL::length(str));
		}

		virtual CharT const* GSYS_API pool(CharT const* str,size_t sz)
		{
			pool_data fdta(const_cast<CharT*>(str),sz);
			SetStrings::iterator it = m_setStrings.find(fdta);
			if(it != m_setStrings.end()) return it->ptr;
			pool_data dta((CharT*)galloc((sz+1)*sizeof(CharT)),sz);
			memcpy(dta.ptr,str,sz*sizeof(CharT));
			dta.ptr[sz]=0;
			m_setStrings.insert(dta);
			return dta.ptr;
		}

		void free_pooled_data()
		{
			_foreach(SetStrings,m_setStrings,it) gfree(it->ptr);
			m_setStrings.clear();	
		}
	};

	template<typename CharT,typename _CharCL>
	basic_string_pool_interface<CharT,_CharCL>* get_basic_string_pool()
	{
		return 0;
	}

	template<typename PoolT>
	inline void set_string_pool(PoolT* pPool)
	{
		if(!g_pStringPool)g_pStringPool = pPool;
	}

	template<typename CharT,typename _CharCL=basic_char_classify<CharT> >
	class basic_pooled_string
	{
	public:
		typedef basic_pooled_string<CharT,_CharCL> this_type;
		typedef CharT value_type;
		typedef CharT char_type;
		typedef gsys_size_t size_type;
		typedef gsys_pt_diff difference_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
	private:
		typedef basic_string<CharT,typename _CharCL::std_traits> std_string_type;
		typedef basic_gstring<CharT,_CharCL> gstring_type;

		typedef std::basic_istream<CharT,typename _CharCL::std_traits> istream_type;
		typedef std::basic_ostream<CharT,typename _CharCL::std_traits> ostream_type;

	private:
		const_pointer m_pBegin;
	private:
		const_pointer pool(const_pointer str)
		{
			return get_basic_string_pool<CharT,_CharCL>()->pool(str);
		}
		const_pointer pool(const_pointer str,size_type sz)
		{
			return get_basic_string_pool<CharT,_CharCL>()->pool(str,sz);
		}
		this_type& _init(const_pointer pt)
		{
			assign(pt);
			return *this;
		}
	public: // iterators
		basic_pooled_string():m_pBegin(0){};
		~basic_pooled_string(){}

		this_type& operator = (this_type const& s){m_pBegin = s.m_pBegin;return *this;}
		basic_pooled_string(this_type const& s):m_pBegin(s.m_pBegin){}

		this_type& operator = (std_string_type const& s){return _init(s.data());}
		basic_pooled_string(std_string_type const& s):m_pBegin(gsys_null){operator=(s);}

		this_type& operator = (gstring_type const& s){return _init(s.data());}
		basic_pooled_string(gstring_type const& s):m_pBegin(gsys_null){operator=(s);}

		this_type& operator = (const_pointer s){return _init(s);}
		basic_pooled_string(const_pointer s):m_pBegin(gsys_null){operator=(s);}

		void assign_pooled(char const* dta){m_pBegin = dta;}

		void assign(const_pointer pt,size_type sz)
		{
			if(sz && pt && pt[0])m_pBegin = pool(pt,sz);
			else m_pBegin = 0;
		}
		void assign(const_pointer pt)
		{
			if(pt && pt[0])m_pBegin = pool(pt);
			else m_pBegin = 0;
		}
		void clear()
		{
			m_pBegin = 0;
		}
	public: // iterators

		bool operator == (this_type const& v)const{return m_pBegin == v.m_pBegin;}
		bool operator != (this_type const& v)const{return m_pBegin != v.m_pBegin;}
		bool operator < (this_type const& v)const{return m_pBegin < v.m_pBegin;}
		bool operator > (this_type const& v)const{return m_pBegin > v.m_pBegin;}
		bool operator <= (this_type const& v)const{return m_pBegin <= v.m_pBegin;}
		bool operator >= (this_type const& v)const{return m_pBegin >= v.m_pBegin;}

	public: // iterators
		CharT const* c_str()const{return m_pBegin?m_pBegin:_CharCL::empty_string();}
		CharT const* data()const{return m_pBegin;}
		operator std_string_type ()const{return std_string_type(c_str());}
		operator gstring_type ()const{return gstring_type(m_pBegin);}

		bool empty()const{return m_pBegin == 0;}

		friend inline ostream_type& operator<<(ostream_type& os,this_type const& v)
		{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
			os << v.c_str();	
			return os;
		}

		friend inline istream_type& operator>>(istream_type& is,this_type & v)
		{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
			std_string_type s;
			is >> s;
			v = s;
			return is;
		}

		friend inline binostream& operator<<(binostream& os,this_type const& v)
		{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
			return os.put_string(v.c_str());
		}

		friend inline binistream& operator>>(binistream& is,this_type & v)
		{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
			gsys_dword sz;
			value_type const* dta;
			is.get_string(sz,dta);
			v.assign(dta,sz);
			return is;
		}
	};
	typedef basic_pooled_string<char> pooled_string;
	typedef basic_pooled_string<wchar_t> pooled_wstring;

	GSYS_SET_TYPENAME(pooled_string,"pS");
	GSYS_SET_TYPENAME(pooled_wstring,"pwS");



	extern basic_string_pool_interface<char>* g_pStringPool; 
	extern basic_string_pool_interface<wchar_t>* g_pWideStringPool; 

	typedef basic_string_pool_interface<char> string_pool_interface;
	typedef basic_string_pool_interface<wchar_t> wstring_pool_interface;

	typedef basic_string_pool<char> string_pool;
	typedef basic_string_pool<wchar_t> wstring_pool;

	extern string_pool_interface* g_pStringPool; 
	extern wstring_pool_interface* g_pWideStringPool; 

	template<>
	inline string_pool_interface* get_basic_string_pool<char,char_classify>()
	{
		if(!g_pStringPool)g_pStringPool = new string_pool();
		return g_pStringPool;
	}

	template<>
	inline wstring_pool_interface* get_basic_string_pool<wchar_t,wchar_classify>()
	{
		if(!g_pWideStringPool)g_pWideStringPool = new wstring_pool();
		return g_pWideStringPool;
	}

	template<>
	inline void set_string_pool<string_pool>(string_pool* pPool)
	{
		if(!g_pStringPool)g_pStringPool = pPool;
	}

	template<>
	inline void set_string_pool<wstring_pool>(wstring_pool* pPool)
	{
		if(!g_pWideStringPool)g_pWideStringPool = pPool;
	}

	inline string_pool_interface* get_string_pool(){return get_basic_string_pool<char,char_classify>();}
	inline wstring_pool_interface* get_wstring_pool(){return get_basic_string_pool<wchar_t,wchar_classify>();}
}

#endif



