#pragma once
#ifndef G_TEXT_ALG_H
#define G_TEXT_ALG_H

#include "gsys.h"
#include "boost/lexical_cast.hpp"
#include <string>
#include <vector>
#include "g_utility.h"

#ifndef CP_UTF8
#define CP_UTF8 65001
#endif

namespace gsys
{
	namespace detail
	{
		wchar_t const* StrToWide(char const* src, int Codepage = 0 ); 
		char const* WideToStr(wchar_t const* src, int Codepage = 0 );
	}

	inline wchar_t const* str_to_wide(char const* src, int Codepage=0){return detail::StrToWide(src,Codepage);}
	inline char const* wide_to_str(wchar_t const* src, int Codepage=0){return detail::WideToStr(src,Codepage);}

	inline wchar_t const* utf8_to_wide(char const* src){return detail::StrToWide(src,CP_UTF8);}
	inline char const* wide_to_utf8(wchar_t const* src){return detail::WideToStr(src,CP_UTF8);}


	template< typename T,typename F>
	T lex_cast(F const& v)
	{
		return boost::lexical_cast<T>(v);
	}

	template< typename T, typename F>
	bool lex_convert(T & r,F const& v)
	{
		try
		{
			r = lex_cast<T>(v);
			return true;
		}
		catch(boost::bad_lexical_cast)
		{
			return false;
		}
	}

	template<>bool inline lex_cast<bool,std::string>(std::string const& v)
	{
		if(_stricmp(v.c_str(),"true")==0)return true;
		if(_stricmp(v.c_str(),"yes")==0)return true;
		if(_stricmp(v.c_str(),"on")==0)return true;
		if(_stricmp(v.c_str(),"false")==0)return false;
		if(_stricmp(v.c_str(),"no")==0)return false;
		if(_stricmp(v.c_str(),"off")==0)return false;
		return boost::lexical_cast<bool>(v);
	}

	template<typename T>
	bool str_to(char const* str,T& d)
	{
		try
		{
			d = ::gsys::lex_cast<T>(str);
		}
		catch(::boost::bad_lexical_cast)
		{
			return false;
		}
		return true;
	}

	template<typename T>
	bool str_to(std::string const* str,T& d)
	{
		try
		{
			d = ::gsys::lex_cast<T>(str);
		}
		catch(::boost::bad_lexical_cast)
		{
			return false;
		}
		return true;
	}

	template<typename T>
	std::string to_str(T const& d)
	{
		return  ::gsys::lex_cast<std::string>(d);
	}

	template<>	inline std::string to_str(char const* const& str){return std::string(str);}
	template<>	inline std::string to_str(std::string const& str){return str;}
	template<>	inline std::string to_str(wchar_t const* const& str){return detail::WideToStr(str);}
	template<>	inline std::string to_str(wchar_t * const& str){return detail::WideToStr(str);}
	template<>	inline std::string to_str(std::wstring const& str){return detail::WideToStr(str.c_str());}

	template<typename T> std::wstring to_wstr(T const& d)
	{
		return  ::gsys::lex_cast<std::wstring>(d);
	}

	template<>	inline std::wstring to_wstr(wchar_t const* const& str){return std::wstring(str);}
	template<>	inline std::wstring to_wstr(std::wstring const& str){return str;}

	template<>	inline std::wstring to_wstr(char const* const & str){return detail::StrToWide(str);}
	template<>	inline std::wstring to_wstr(char * const & str){return detail::StrToWide(str);}
	template<>	inline std::wstring to_wstr(std::string const& str){return detail::StrToWide(str.c_str());}

	inline char decode_cescaped_char(char ch)
	{
		switch(ch)
		{
		case 'r': return '\r';
		case 'n': return '\n';
		case 't': return '\t';
		case 's': return ' ';
		default: return ch;
		}
	}

	inline void append_cescaped_char(char ch,std::string & str)
	{
		switch(ch)
		{
		case '\r': str += "\\r";break;
		case '\n': str += "\\n";break;
		case '\"': str += "\\\"";break;
		case '\\': str += "\\\\";break;
		default: str += ch;break;
		}
	}

	inline void cexcape(std::string & str)
	{
		size_t n=str.length();
		size_t p = str.find_first_of("\\\"\r\n");
		if(p == std::string::npos)return;
		string unq = str.substr(0,p);
		for(size_t i=p;i<n;i++)
		{
			append_cescaped_char(str[i],unq);
		}
		str = unq;
	}

	inline void uncexcape(std::string & str)
	{
		size_t n=str.length();
		size_t p = str.find_first_of("\\");
		if(p == std::string::npos)return;
		string unq = str.substr(0,p);
		for(size_t i=p;i<n;i++)
		{
			if(str[i] == '\\')
			{
				++i;
				unq += decode_cescaped_char(str[i]);
			}
			else unq += str[i];
		}
		str = unq;
	}

	inline void quote(std::string & str)
	{
		cexcape(str);
		str = '"'+str+'"';
	}

	inline void unquote(std::string & str)
	{
		if(str.empty())return;
		char quote_char;
		if(str[0] == '\"')quote_char = '"';
		else if(str[0] == '\'')quote_char = '\'';
		else if(str[0] == '`')quote_char = '`';
		else return;

		str.erase(0,1); // remove first quote
		size_t len = str.length();
		if(str[len-1] == quote_char)str.erase(len-1); // remove last quote
		uncexcape(str);
	}


	inline std::ostream& operator<<(std::ostream& os,std::wstring const& v)
	{
		os << wide_to_str(v.c_str());	
		return os;
	}

	inline std::istream& operator>>(std::istream& is,std::wstring & v)
	{
		std::string str;
		is >> str;
		v = str_to_wide(str.c_str());
		return is;
	}

	inline std::wostream& operator<<(std::wostream& os,std::string const& v)
	{
		os << str_to_wide(v.c_str());	
		return os;
	}

	inline std::wistream& operator>>(std::wistream& is,std::string & v)
	{
		std::wstring str;
		is >> str;
		v = wide_to_str(str.c_str());
		return is;
	}

	inline std::wostream& operator<<(std::wostream& os,char const* const& v)
	{
		os << str_to_wide(v);	
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os,wchar_t const* const& v)
	{
		os << wide_to_str(v);	
		return os;
	}
}

#endif