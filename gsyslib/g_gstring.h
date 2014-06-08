#pragma once
#ifndef G_GSTRING_H
#define G_GSTRING_H
#include "g_gsys.h"
#include "g_alg.h"
#include <string>
#include "g_binstream.h"
#include "g_text_alg.h"
#include "g_text.h"

/*
*	Simple class for string analog to STL
*   Currently provide only minimal functionality
*/

namespace gsys
{
/*
	namespace detail
	{
		template<class CharT> int t_stricmp(CharT const* s1,CharT const* s2){BOOST_STATIC_ASSERT(0);}
		template<>	inline int t_stricmp<char>(char const* s1,char const* s2){return stricmp(s1,s2);}
		template<>	inline int t_stricmp<wchar_t>(wchar_t const* s1,wchar_t const* s2){return _wcsicmp(s1,s2);}

		template<class CharT> size_t t_strlen(CharT const* s1){BOOST_STATIC_ASSERT(0);}
		template<>	inline size_t t_strlen<char>(char const* s1){return strlen(s1);}
		template<>	inline size_t t_strlen<wchar_t>(wchar_t const* s1){return wcslen(s1);}

		template<class CharT> CharT const* t_nullstr(){BOOST_STATIC_ASSERT(0);}
		template<>	inline char const* t_nullstr<char>(){return "";}
		template<>	inline wchar_t const* t_nullstr<wchar_t>(){return L"";}
	}
*/

	template<typename CharT,typename _CharCL = basic_char_classify<CharT> >
	class basic_gstring
	{
		typedef basic_gstring<CharT> this_type ;
		typedef std::basic_string<CharT,typename _CharCL::std_traits> std_string_type;	
		typedef std::basic_istream<CharT,typename _CharCL::std_traits> std_istream_type;	
		typedef std::basic_ostream<CharT,typename _CharCL::std_traits> std_ostream_type;	
	public:
		typedef CharT value_type;
		typedef gsys_size_t size_type;
		typedef gsys_pt_diff difference_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
		GSYSDETAIL_IMPLEMENT_ARRAY_CONTAINER;
	private:
		struct header
		{
			size_type ref_count;
		};
	private:
		pointer m_pBegin;

	private:
		void _new_buffer(size_type sz)
		{
			gsys_byte* pBuffer = (gsys_byte*)galloc(sizeof(value_type)*(sz)+sizeof(header));
			header& head = *((header*)pBuffer);
			value_type* pBegin = (value_type*)(pBuffer+sizeof(header));
			head.ref_count = 1;
			clear();
			m_pBegin = pBegin;
		}
	public:	
		this_type& assign(this_type const& ref)
		{
			if(!ref.empty())
			{
				header& head = *((header*)((gsys_byte*)ref.m_pBegin - sizeof(header)));
				head.ref_count++;
				clear();
				m_pBegin = ref.m_pBegin;
			}
			else clear();
			return *this;
		}

		this_type& assign(const_pointer pt,size_type sz)
		{
			if(pt && pt[0])
			{
				gsys_byte* pBuffer = (gsys_byte*)galloc(sizeof(value_type)*(sz+1)+sizeof(header));
				header& head = *((header*)pBuffer);
				value_type* pBegin = (value_type*)(pBuffer+sizeof(header));
				memcpy(pBegin,pt,sz*sizeof(value_type));
				pBegin[sz]=0;
				head.ref_count = 1;
				clear();
				m_pBegin = pBegin;
			}
			else clear();
			return *this;
		}

		this_type& assign(const_pointer pt)
		{
			return assign(pt,pt?_CharCL::length(pt):0);
		}
		void clear()
		{
			if(m_pBegin)
			{
				header& head = *((header*)((gsys_byte*)m_pBegin - sizeof(header)));
				head.ref_count--;
				if(!head.ref_count)gfree(&head);
				m_pBegin=0;
			}
		}
	public: // iterators
		basic_gstring():m_pBegin(0){};
		~basic_gstring(){clear();}

		this_type& operator = (this_type const& s){return assign(s);}
		basic_gstring(this_type const& s):m_pBegin(gsys_null){assign(s);}

		this_type& operator = (std_string_type const& s){return assign(s.data());}
		basic_gstring(std_string_type const& s):m_pBegin(gsys_null){assign(s.data());}

		this_type& operator = (const_pointer s){return assign(s);}
		basic_gstring(const_pointer s):m_pBegin(gsys_null){assign(s);}  

		basic_gstring(const_pointer s,size_type sz):m_pBegin(gsys_null){assign(s,sz);}  
		basic_gstring(const_pointer s,const_pointer e):m_pBegin(gsys_null){assign(s,(size_type)(e-s));}  

	public: // operations
		void swap(this_type& str)
		{
			pointer t(m_pBegin);
			m_pBegin=str.m_pBegin;
			str.m_pBegin=t;
		}
	public: // operators
		bool operator == (const_pointer v)const{return _CharCL::compare_i(c_str(),v)==0;}
		bool operator != (const_pointer v)const{return _CharCL::compare_i(c_str(),v)!=0;}

		bool operator == (this_type const& v)const{return _CharCL::compare_i(c_str(),v.c_str())==0;}
		bool operator != (this_type const& v)const{return _CharCL::compare_i(c_str(),v.c_str())!=0;}
		bool operator < (this_type const& v)const{return _CharCL::compare_i(c_str(),v.c_str())<0;}
		bool operator > (this_type const& v)const{return _CharCL::compare_i(c_str(),v.c_str())>0;}
		bool operator <= (this_type const& v)const{return _CharCL::compare_i(c_str(),v.c_str())<=0;}
		bool operator >= (this_type const& v)const{return _CharCL::compare_i(c_str(),v.c_str())>=0;}

	public: 
		const_pointer c_str()const{return m_pBegin?m_pBegin:_CharCL::empty_string();}
		const_pointer data()const{return m_pBegin;}
		operator std_string_type ()const{return std_string_type(c_str());}
		bool empty()const{if(!m_pBegin)return true; return m_pBegin[0]==0;}
		size_t length()const{if(!m_pBegin)return 0;return _CharCL::length(m_pBegin);}
		size_t size()const{return length();}

		// prepare buffer for putting new string into
		pointer update_buffer(size_type new_sz)
		{
			if(m_pBegin)
			{
				header* head = ((header*)((gsys_byte*)m_pBegin - sizeof(header)));
				if(head->ref_count>1)
				{
					_new_buffer(new_sz);
					return m_pBegin;
				}
				else
				{
					head = (header*)grealloc(head,sizeof(value_type)*(new_sz+1)+sizeof(header));
					head->ref_count=1;
					m_pBegin = (value_type*)(((gsys_byte*)head)+sizeof(header));
					return m_pBegin;
				}
			}
			else
			{
				_new_buffer(new_sz);
				return m_pBegin;
			}
		}

		value_type operator[](size_type pos)const
		{
			if(!m_pBegin)return 0;
			GSYS_ASSERT(pos <= length());
			return m_pBegin[pos];
		}

		reference operator[](size_type pos)
		{
			GSYS_ASSERT(m_pBegin && pos <= length());
			return m_pBegin[pos];
		}

		friend inline std_ostream_type& operator<<(std_ostream_type& os,this_type const& v)
		{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
			os << v.c_str();	
			return os;
		}

		friend inline std_istream_type& operator>>(std_istream_type& is,this_type & v)
		{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
			static raw_data_buffer<value_type,512,512> buf;
			buf.reset();
			while(is.peek() != _CharCL::eof())
			{
				buf.push_back(is.get());
			}
			v.assign(buf.get(),buf.size());
			return is;
		}

 		friend inline binistream& operator>>(binistream& is,this_type & v)
		{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
			gsys_dword sz;
			value_type const* dta;
			is.get_string(sz,dta);
			v.assign(dta,sz);
			return is;
		}

		friend inline binostream& operator<<(binostream& os,this_type const& v)
		{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
			os.put_string(v.c_str(),(gsys_dword)v.size());
			return os;
		}
    };


	template<typename CharT,typename _CharCL >
	inline void swap(basic_gstring<CharT,_CharCL>& a,basic_gstring<CharT,_CharCL>& b){a.swap(b);}

	typedef basic_gstring<char> gstring;
	typedef basic_gstring<wchar_t> wgstring;


	inline std::ostream& operator<<(std::ostream& os,wgstring const& v)
	{
		os << wide_to_str(v.c_str());	
		return os;
	}

	inline std::istream& operator>>(std::istream& is,wgstring & v)
	{
		gstring str;
		is >> str;
		v = str_to_wide(str.c_str());
		return is;
	}

	inline std::wostream& operator<<(std::wostream& os,gstring const& v)
	{
		os << str_to_wide(v.c_str());	
		return os;
	}

	inline std::wistream& operator>>(std::wistream& is,gstring & v)
	{
		wgstring str;
		is >> str;
		v = wide_to_str(str.c_str());
		return is;
	}

	template<>bool inline lex_cast<bool,gsys::gstring>(gsys::gstring const& v)
	{
		if(_stricmp(v.c_str(),"true")==0)return true;
		if(_stricmp(v.c_str(),"yes")==0)return true;
		if(_stricmp(v.c_str(),"on")==0)return true;
		if(_stricmp(v.c_str(),"false")==0)return false;
		if(_stricmp(v.c_str(),"no")==0)return false;
		if(_stricmp(v.c_str(),"off")==0)return false;
		return boost::lexical_cast<bool>(v);
	}


    GSYS_SET_TYPENAME(gstring,"gS");
	GSYS_SET_TYPENAME(wgstring,"wgS");
}

#endif



