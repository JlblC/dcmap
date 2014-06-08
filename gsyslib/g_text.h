#ifndef G_TEXT_H
#define G_TEXT_H
#pragma once

#include "g_gsys.h"

#include <istream>
#include <ostream>
#include <string.h>
#include "boost/lexical_cast.hpp"

namespace gsys
{
	namespace detail
	{
		template <class Elem> 
		struct basic_char_classify_base : public std::char_traits<Elem>
		{
			typedef Elem char_type; 
			typedef char_traits<char_type> std_traits; 
			static bool eof(char_type ch)
			{
				return ch == char_traits<char_type>::eof();
			}
			static char_type eof()
			{
				return char_traits<char_type>::eof();
			}
		};

		template <class Elem> struct basic_char_classify_impl : public basic_char_classify_base<Elem>{};
		template<> struct basic_char_classify_impl<char> : public basic_char_classify_base<char>
		{
			static char_type const* empty_string(){return "";}

			static bool unicode_marker(char_type ch){return false;};

			static char_type etscape(){return '\\';};
			static char_type single_quote(){return '\'';};
			static char_type double_quote(){return '\"';};
			static char_type minus(){return '-';};
			static char_type plus(){return '+';};
			static char_type dot(){return '.';};
			static char_type exponent_e(){return 'e';};
			static char_type exponent_E(){return 'E';};

			static int compare(char_type const* a,char_type const* b){return strcmp(a,b);}
			static int compare_i(char_type const* a,char_type const* b){return stricmp(a,b);}

			static bool path_divider(char_type ch){return ch == '\\' || ch == '/';}

			static bool ucmt(char_type ch){return ch == '#';}

			static bool alpha(char_type ch)
			{
				if(ch >= 'a' && ch <= 'z')return true;
				if(ch >= 'A' && ch <= 'Z')return true;
				if(ch == '_') return true;
				return false;
			}
		
			static bool numeric(char_type ch)
			{
				if(ch >= '0' && ch <= '9')return true;
				return false;
			}	
			static bool numeric_hex(char_type ch)
			{
				if(ch >= '0' && ch <= '9')return true;
				if(ch >= 'a' && ch <= 'f')return true;
				if(ch >= 'A' && ch <= 'F')return true;
				return false;
			}
			static bool numeric_ex(char_type ch)
			{
				if(ch >= '0' && ch <= '9')return true;
				if(ch == '.' || ch == '+' || ch == '-')return true;
				return false;
			}		
			static bool whitespace(char_type ch)
			{
				switch(ch)
				{
				case ' ':
				case '\t':
					return true;
				default:
					return false;
				}
			}
			static bool newline(char_type ch)
			{
				switch(ch)
				{
				case '\r':
				case '\n':
					return true;
				default:
					return false;
				}
			}

			static bool whitespace_newline(char_type ch)
			{
				switch(ch)
				{
				case ' ':
				case '\t':
				case '\r':
				case '\n':
					return true;
				default:
					return false;
				}
			}
			static char_type quote_pair(char_type ch)
			{
				switch(ch)
				{
				case '"':return '"';
				case '\'':return '\'';
				case '`':return '`';
				case '/':return '/';
				case '|':return '|';
				case '(':return ')';
				case '[':return ']';
				case '{':return '}';
				case '<':return '>';
				default: return 0;
				}
			}

			static bool open_braces(char_type ch)
			{
				switch(ch)
				{
				case '(':
				case '{':
				case '[':
				case '<':
					return true;
				}
				return false;
			}

			static bool closing_braces(char_type ch)
			{
				switch(ch)
				{
				case ')':
				case '}':
				case ']':
				case '>':
					return true;
				}
				return false;
			}

			static char_type decode_cescaped_char(char_type ch)
			{
				switch(ch)
				{
				case 'a': return '\a';
				case 'b': return '\b';
				case 'f': return '\f';
				case 'n': return '\n';
				case 'r': return '\r';
				case 't': return '\t';
				case 'v': return '\v';
				case 's': return ' ';
				default: return ch;
				}
			}
			static void append_cescaped_char(char_type ch,std::basic_string<char_type> & str)
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
			static bool extended_letters(char_type ch)
			{
				if(ch > 0x7F)return true;
				return false;
			}
		};

		template<> struct basic_char_classify_impl<wchar_t> : public basic_char_classify_base<wchar_t>
		{
			static char_type const* empty_string(){return L"";}

			static char_type etscape(){return L'\\';};
			static char_type single_quote(){return L'\'';};
			static char_type double_quote(){return L'\"';};
			static char_type minus(){return L'-';};
			static char_type plus(){return L'+';};
			static char_type dot(){return L'.';};
			static char_type exponent_e(){return L'e';};
			static char_type exponent_E(){return L'E';};

			static int compare(char_type const* a,char_type const* b){return wcscmp(a,b);}
			static int compare_i(char_type const* a,char_type const* b){return _wcsicmp(a,b);}

			static bool unicode_marker(char_type ch){return ch == L'\xfeff';};

			static bool ucmt(char_type ch){return ch == L'#';}

			static bool path_divider(char_type ch){return ch == L'\\' || ch == L'/';}

			static bool alpha(char_type ch)
			{
				if(ch >= L'a' && ch <= L'z')return true;
				if(ch >= L'A' && ch <= L'Z')return true;
				if(ch == L'_') return true;
				return false;
			}

			static bool extended_letters(char_type ch)
			{
				if(ch > 0x7F)return true;
				return false;
			}

			static bool numeric(char_type ch)
			{
				if(ch >= L'0' && ch <= L'9')return true;
				return false;
			}	
			static bool numeric_hex(char_type ch)
			{
				if(ch >= L'0' && ch <= L'9')return true;
				if(ch >= L'a' && ch <= L'f')return true;
				if(ch >= L'A' && ch <= L'F')return true;
				return false;
			}
			static bool numeric_ex(char_type ch)
			{
				if(ch >= L'0' && ch <= L'9')return true;
				if(ch == L'.' || ch == L'+' || ch == L'-')return true;
				return false;
			}
			static bool whitespace(char_type ch)
			{
				switch(ch)
				{
				case L' ':
				case L'\t':
					return true;
				default:
					return false;
				}
			}
			static bool newline(char_type ch)
			{
				switch(ch)
				{
				case L'\r':
				case L'\n':
					return true;
				default:
					return false;
				}
			}

			static bool whitespace_newline(char_type ch)
			{
				switch(ch)
				{
				case L' ':
				case L'\t':
				case L'\r':
				case L'\n':
					return true;
				default:
					return false;
				}
			}

			static bool closing_braces(char_type ch)
			{
				switch(ch)
				{
				case L')':
				case L'}':
				case L']':
				//case L'':
				case L'>':
					return true;
				}
				return false;
			}

			static bool open_braces(char_type ch)
			{
				switch(ch)
				{
				case L'(':
				case L'{':
				case L'[':
				//case L'':
				case L'<':
					return true;
				}
				return false;
			}

			static char_type quote_pair(char_type ch)
			{
				switch(ch)
				{
				case L'"':return L'"';
				case L'`':return L'`';
				case L'\'':return L'\'';
				case L'/':return L'/';
				case L'|':return L'|';
				case L'(':return L')';
				case L'[':return L']';
				case L'{':return L'}';
				case L'<':return L'>';
				//case L'':return L'';
				default: return 0;
				}
			}

			static char_type decode_cescaped_char(char_type ch)
			{
				switch(ch)
				{
				case L'a': return L'\a';
				case L'b': return L'\b';
				case L'f': return L'\f';
				case L'n': return L'\n';
				case L'r': return L'\r';
				case L't': return L'\t';
				case L'v': return L'\v';
				case L's': return L' ';
				default: return ch;
				}
			}
			static void append_cescaped_char(char_type ch,std::basic_string<char_type> & str)
			{
				switch(ch)
				{
				case L'\r': str += L"\\r";break;
				case L'\n': str += L"\\n";break;
				case L'\"': str += L"\\\"";break;
				case L'\\': str += L"\\\\";break;
				default: str += ch;break;
				}
			}
		};

	}

	template <class Elem> struct basic_char_classify : public detail::basic_char_classify_impl<Elem>
	{
		typedef Elem char_type;
		static bool alpha_ext(char_type ch)
		{
			return detail::basic_char_classify_impl<Elem>::alpha(ch) || 
				   detail::basic_char_classify_impl<Elem>::extended_letters(ch);
		}
	};

	typedef basic_char_classify<wchar_t> wchar_classify;
	typedef basic_char_classify<char> char_classify;

	namespace detail
	{
		// read keyword from stream 
		template <class Elem, class Tr = gsys::basic_char_classify<Elem> >
		class base_istream_reader
		{
		public:
			typedef std::basic_string<Elem,typename Tr::std_traits> string_type;
			typedef std::basic_istream<Elem,typename Tr::std_traits> istream_type;
			typedef Elem char_type;
		protected:
			typedef Tr chars;
		};
	}

	template <class Elem> inline bool tchar_is_alpha(Elem ch){return basic_char_classify<Elem>::alpha(ch);}
	template <class Elem> inline bool tchar_is_numb(Elem ch){return basic_char_classify<Elem>::numeric(ch);}
	template <class Elem> inline bool tchar_is_wsp(Elem ch){return basic_char_classify<Elem>::whitespace(ch);}
	template <class Elem> inline bool tchar_is_nl(Elem ch){return basic_char_classify<Elem>::newline(ch);}
	template <class Elem> inline bool tchar_is_wsp_nl(Elem ch){return basic_char_classify<Elem>::whitespace_newline(ch);}
	template <class Elem> inline Elem tchar_quote_pair(Elem ch){return basic_char_classify<Elem>::quote_pair(ch);}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	inline bool char_is_alpha(char ch){return char_classify::alpha(ch);}
	inline bool char_is_numb(char ch){return char_classify::numeric(ch);}
	inline bool char_is_wsp(char ch){return char_classify::whitespace(ch);}
	inline bool char_is_nl(char ch){return char_classify::newline(ch);}
	inline bool char_is_wsp_nl(char ch){return char_classify::whitespace_newline(ch);}


	template<typename T> 
	inline bool char_is_eof(T ch)
	{
		return ch == char_traits<T>::eof();
	}

#define GSYS_DETAIL_TSTREAM_TYPEDEFS \
	typedef std::basic_string<Elem,typename Tr::std_traits> string_type; \
	typedef std::basic_istream<Elem,typename Tr::std_traits> istream_type; \
	typedef Elem char_type; \
	typedef Tr chars; 


	template <class val,class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_numeric : public detail::base_istream_reader<Elem,Tr>
	{
		val* pv;
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
	public:
		basic_istream_numeric(val& v):pv(&v){ }
		
		
		friend istream_type& operator >> (istream_type& is,basic_istream_numeric<val,Elem,Tr> in)
		{
			char_type buf[32];
			char_type* pb=buf;
			char_type ch = is.peek();
			bool exp=false;
			bool sign=true;
			bool dot = true;
			while(pb < buf+32)
			{
				ch = is.peek();
				if(sign)
				{
					if(ch == chars::minus() || ch == chars::plus())
					{
						*(pb++) = ch;
						is.ignore();
						continue;
					}
				}
				sign = false;

				if(chars::numeric(ch))*(pb++) = ch;
				else if(ch == chars::dot() && dot){*(pb++) = ch;dot=false;}
				else if((ch == chars::exponent_e() || ch == chars::exponent_E()) && !exp){*(pb++) = chars::exponent_e();exp=true;sign=true;dot=false;}
				else break;
				is.ignore();
			}
			*pb=0;
			std::basic_istringstream<Elem,Tr> ibuf(buf);
			ibuf >> *(in.pv);
			return is;
		}
	};
	template <class val> basic_istream_numeric<val,wchar_t> WISN(val& v){return basic_istream_numeric<val,wchar_t>(v);}
	template <class val> basic_istream_numeric<val,char> ISN(val& v){return basic_istream_numeric<val,char>(v);}


	template <class val,class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_numeric_hex : public detail::base_istream_reader<Elem,Tr>
	{
		val* pv;
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
	public:
		basic_istream_numeric_hex(val& v):pv(&v){}
		friend istream_type& operator >> (istream_type& is,basic_istream_numeric_hex<val,Elem,Tr> in)
		{
			char_type buf[16];
			//buf[0]=(char_type)'0';
			//buf[1]=(char_type)'x';

			char_type* pb=buf;
			char_type ch;
			while(pb < buf+15)
			{
				ch = is.peek();
				if(chars::numeric_hex(ch))*(pb++) = ch;
				else break;
				is.ignore();
			}
			*pb=0;
			
			std::basic_istringstream<Elem,Tr> ibuf(buf);
			ibuf >> hex >> *(in.pv);
			return is;
		}
	};
	template <class val> basic_istream_numeric_hex<val,wchar_t> WISN_HEX(val& v){return basic_istream_numeric_hex<val,wchar_t>(v);}
	template <class val> basic_istream_numeric_hex<val,char> ISN_HEX(val& v){return basic_istream_numeric_hex<val,char>(v);}

	template <class val>
	class istream_numeric
	{
		val* pv;
	public:
		istream_numeric(val& v):pv(&v){}
		friend std::istream& operator>>(std::istream& is,istream_numeric<val> in)
		{
			char buf[32];
			char* pb=buf;
			char ch = is.peek();
			bool exp=false;
			bool sign=true;
			bool dot = true;
			while(pb < buf+32)
			{
				ch = is.peek();
				if(sign)
				{
					if(ch == '-' || ch == '+')
					{
						*(pb++) = ch;
						is.ignore();
						continue;
					}
				}
				sign = false;

				if(char_is_numb(ch))*(pb++) = ch;
				else if(ch == '.' && dot){*(pb++) = ch;dot=false;}
				else if((ch == 'e' || ch == 'E') && !exp){*(pb++) = 'e';exp=true;sign=true;dot=false;}
				else break;

				is.ignore();
			}
			*pb=0;
			std::istringstream ibuf(buf);
			ibuf >> *(in.pv);
			return is;
		}
	};
	//template<class val> istream_numeric<val> ISN(val& v){return istream_numeric<val>(v);}


	// uses for scip white spaces on input stream
	// input:	456			45465
	// code: is >> wsp_scip >> a >> wsp_scip >> b;
	template <class Elem, class Tr>
	std::basic_istream<Elem,Tr>& wsp_scip(std::basic_istream<Elem,Tr>& is)
	{
		Elem ps;
		if(is.fail())return is;
		while(true)
		{
			ps = is.peek();
			if(!tchar_is_wsp(ps)) break;
			is.ignore();
		}
		return is;	
	}

	// uses for scip white spaces and newlines  on input stream
	template <class Elem,class Tr> inline
	std::basic_istream<Elem,Tr>& wsp_nl_scip(std::basic_istream<Elem,Tr>& is)
	{
		Elem ps;
		if(is.fail())return is;
		while(!is.eof())
		{
			ps = is.peek();
			if(!tchar_is_wsp_nl(ps)) break;
			is.ignore();
		}
		return is;	
	}

	// uses for scip all chars till end of line
	template <class Elem, class Tr>
	std::basic_istream<Elem,Tr>& istream_scip_line(std::basic_istream<Elem,Tr>& is)
	{
		typedef basic_char_classify<Elem> cc;
		bool nl = false;
		Elem ps;
		while(!is.fail())
		{
			ps = is.peek();
			if(nl){if(!cc::newline(ps))break;}
			else nl = cc::newline(ps);
			is.ignore();
		}
		return is;	
	}

	template <class Elem,class Tr> inline
		std::basic_istream<Elem,Tr>& wsp_nl_ucmt_scip(std::basic_istream<Elem,Tr>& is)
	{
		typedef basic_char_classify<Elem> cc;
		Elem ps;
		if(is.fail())return is;
		while(!is.fail())
		{
			ps = is.peek();
			if(cc::ucmt(ps))
			{
				is >> istream_scip_line;
			}
			else
			{
				if(!cc::whitespace_newline(ps)) break;
				is.ignore();
			}
		}
		return is;	
	}


	// uses for scip newlines  on input stream
	template <class Elem, class Tr>
	inline std::basic_istream<Elem,Tr>& nl_scip(std::basic_istream<Elem,Tr>& is)
	{
		Elem ps;
		if(is.fail())return is;
		while(!is.eof())
		{
			ps = is.peek();
			if(!tchar_is_nl(ps)) break;
			is.ignore();
		}
		return is;	
	}

	template <class Elem, class Tr>
	inline std::basic_istream<Elem,Tr>& istream_scip_till_nl(std::basic_istream<Elem,Tr>& is)
	{
		bool nl = false;
		if(is.fail())return is;
		while(!is.eof())
		{
			if(tchar_is_nl((Elem)is.peek()))break;
			is.ignore();
		}
		return is;	
	}

	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_line:public detail::base_istream_reader<Elem,Tr>
	{
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
		string_type* m_pStr;
	public:
		basic_istream_line(string_type & str) : m_pStr(&str){}
		friend inline istream_type& operator>>(istream_type& is,basic_istream_line<Elem,Tr> & isk)
		{
			char_type ps;
			if(is.fail())return is;
			isk.m_pStr->clear();
			while(true)
			{
				ps = is.peek();
				if(chars::newline(ps))break;
				is.ignore();
				if(is.eof())break;
				(*isk.m_pStr) += ps;
			}
			is >> wsp_scip; // scip tailing spaces
			return is;	
		}
	};
	typedef basic_istream_line<char> istream_line;
	typedef basic_istream_line<wchar_t> wistream_line;

	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_perl_quoted :public detail::base_istream_reader<Elem,Tr>
	{
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
	public:
		basic_istream_perl_quoted(string_type & str) : m_pStr(&str){}
		friend inline istream_type& operator>>(istream_type& is,basic_istream_perl_quoted<Elem,Tr> & isk)
		{
			const char_type etsc = chars::etscape();
			char_type qs=0;
			char_type ps;
			if(is.fail())return is;
			isk.m_pStr->clear();

			qs = is.peek();
			qs = tchar_quote_pair(qs);
			if(!qs){is.setstate(std::ios::failbit);return is;}
			is.ignore();
			while(!is.fail())
			{
				ps = is.peek();
				if(chars::newline(ps)) // newlines ignored
				{
					is.ignore();
					continue;
				}
				//if(char_is_eof(ps))break;
				if(ps == etsc)
				{
					(*isk.m_pStr) += is.get();
					(*isk.m_pStr) += is.get();
				}
				else if(ps == qs)
				{
					is.ignore();
					break;
				}
				else
				{
					(*isk.m_pStr) += is.get();
				}
			}
			return is;	
		}
	private:
		string_type* m_pStr;
	};
	typedef basic_istream_perl_quoted<char> istream_perl_quoted;
	typedef basic_istream_perl_quoted<wchar_t> wistream_perl_quoted;

	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_quoted :public detail::base_istream_reader<Elem,Tr>
	{
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
		string_type* m_pStr;
        bool ml;
	public:
		basic_istream_quoted(string_type & str,bool multyline=false) : m_pStr(&str),ml(multyline){}
		friend inline istream_type& operator>>(istream_type& is,basic_istream_quoted<Elem,Tr> & isk)
		{
			const char_type etsc = chars::etscape();
			char_type qs=0;
			char_type ps;
			if(is.fail())return is;
			isk.m_pStr->clear();

			char_type qi = is.peek();

			qs = chars::quote_pair(qi);
			if(!qs){is.setstate(std::ios::failbit);return is;}
			is.ignore();

			unsigned qcount=1;
			
			while(!is.eof())
			{
				ps = is.peek();
				if(chars::newline(ps) && !isk.ml){is.setstate(std::ios::failbit);return is;}
				if(ps == etsc)
				{
					is.ignore();
					(*isk.m_pStr) += chars::decode_cescaped_char(is.get());
					continue;
				}
				else if(ps == qs)
				{
					qcount--;
					if(!qcount)
					{
						is.ignore();
						break;
					}
				}				
				else if(ps == qi)
				{
					qcount++;
				}
				(*isk.m_pStr) += is.get();
			}
			//is >> wsp_scip; // scip tailing spaces
			return is;	
		}
	};
	typedef basic_istream_quoted<char> istream_quoted;
	typedef basic_istream_quoted<wchar_t> wistream_quoted;

	// read line till enl or one of specified characters
	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_line_till_first_of :public detail::base_istream_reader<Elem,Tr>
	{
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
		string_type* m_pStr;
		char_type const* m_szNums;
	public:
		basic_istream_line_till_first_of(string_type & str,char_type const* chrz)
						: m_pStr(&str),m_szNums(chrz){}

		friend inline istream_type& operator>>(istream_type& is,basic_istream_line_till_first_of<Elem,Tr> const& isk)
		{
			int cc = (int)Tr::length(isk.m_szNums);
			char_type ps;
			if(is.fail())return is;
			isk.m_pStr->clear();
			int nwsp=0;

			while(true)
			{
				ps = is.peek();
				bool br=false;
				if(char_is_nl(ps))break;
				
				for(int i=0;i<cc;i++)
				{
					if(ps == isk.m_szNums[i]){br=true;break;}
				}
				if(br)break;

				if(Tr::whitespace(ps))nwsp++;
				else nwsp=0;

				is.get();
				if(is.eof()||is.fail())break;
				(*isk.m_pStr) += ps;
			}
			isk.m_pStr->erase(isk.m_pStr->length()-nwsp);
			return is;	
		}
	};

	typedef basic_istream_line_till_first_of<char> istream_line_till_first_of;
	typedef basic_istream_line_till_first_of<wchar_t> wistream_line_till_first_of;


	// read keyword from stream 
	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_keyword : public detail::base_istream_reader<Elem,Tr>
	{
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
	public:
		basic_istream_keyword(string_type & str) : m_pStr(&str){}
		friend inline istream_type& operator>>(istream_type& is,basic_istream_keyword<Elem,Tr> & isk)
		{
			char_type ps;
			if(is.fail())return is;
			isk.m_pStr->clear();
			ps = is.peek();
			if(!chars::alpha(ps)){/*is.setstate( std::ios::failbit );*/return is;}
			(*isk.m_pStr) += ps;
			is.ignore();
			while(!is.fail())
			{
				ps = is.peek();
				if(!(chars::alpha(ps) || chars::numeric(ps)))break;
				(*isk.m_pStr) += ps;
				is.ignore();
			}
			//is >> wsp_scip; // skip tailing spaces
			return is;	
		}
	private:
		string_type* m_pStr;
	};
	typedef basic_istream_keyword<char>    istream_keyword;
	typedef basic_istream_keyword<wchar_t> wistream_keyword;


	// read keyword from stream and allow russian letters
	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_keyword_ext : public detail::base_istream_reader<Elem,Tr>
	{
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
	public:
		basic_istream_keyword_ext(string_type & str) : m_pStr(&str){}
		friend inline istream_type& operator>>(istream_type& is,basic_istream_keyword_ext<Elem,Tr> & isk)
		{
			char_type ps;
			if(is.fail())return is;
			isk.m_pStr->clear();
			ps = is.peek();
			if(!(chars::alpha_ext(ps))){/*is.setstate( std::ios::failbit );*/return is;}
			(*isk.m_pStr) += ps;
			is.ignore();
			while(!is.fail())
			{
				ps = is.peek();
				if(!(chars::alpha_ext(ps) || chars::numeric(ps)))break;
				(*isk.m_pStr) += ps;
				is.ignore();
			}
			//is >> wsp_scip; // skip tailing spaces
			return is;	
		}
	private:
		string_type* m_pStr;
	};
	typedef basic_istream_keyword_ext<char>    istream_keyword_ext;
	typedef basic_istream_keyword_ext<wchar_t> wistream_keyword_ext;


	// read number or keyword from stream 
	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_numb_keyword: public detail::base_istream_reader<Elem,Tr>
	{
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
		string_type* m_pStr;
	public:
		basic_istream_numb_keyword(string_type & str) : m_pStr(&str){}
		friend inline istream_type& operator>>(istream_type& is,basic_istream_numb_keyword<Elem,Tr> & isk)
		{
			char_type ps;
			if(is.fail())return is;
			isk.m_pStr->clear();
			ps = is.peek();
			bool numeric=false;
			if(chars::numeric_ex(ps))numeric = true;
			else if(!(chars::alpha(ps))){is.setstate( std::ios::failbit );return is;}
			(*isk.m_pStr) += ps;
			is.ignore();
			while(!is.fail())
			{
				ps = is.peek();
				if(numeric)
				{
					if(!(chars::numeric(ps)|| ps == chars::dot()))break;
				}
				else
				{
					if(!chars::alpha(ps) && !chars::numeric(ps))break;
				}
				(*isk.m_pStr) += ps;
				is.ignore();
			}
			//is >> wsp_scip; // skip tailing spaces
			return is;	
		}
	};
	typedef basic_istream_numb_keyword<char>    istream_numb_keyword;
	typedef basic_istream_numb_keyword<wchar_t> wistream_numb_keyword;

	// read number or keyword from stream 
	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_numb_integer: public detail::base_istream_reader<Elem,Tr>
	{
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
		string_type* m_pStr;
	public:
		basic_istream_numb_integer(string_type & str) : m_pStr(&str){}
		friend inline istream_type& operator>>(istream_type& is,basic_istream_numb_integer<Elem,Tr> & isk)
		{
			char_type ps;
			if(is.fail())return is;
			isk.m_pStr->clear();
			while(!is.fail())
			{
				ps = is.peek();
				if(!(chars::numeric(ps)))break;
				(*isk.m_pStr) += ps;
				is.ignore();
			}
			return is;	
		}
	};
	typedef basic_istream_numb_integer<char>    istream_numb_integer;
	typedef basic_istream_numb_integer<wchar_t> wistream_numb_integer;

	// read keyword from stream 
	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_str_numeric : public detail::base_istream_reader<Elem,Tr>
	{
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
		string_type* m_pStr;
	public:
		basic_istream_str_numeric(string_type & str) : m_pStr(&str){}
		friend inline istream_type& operator>>(istream_type& is,basic_istream_str_numeric<Elem,Tr> & isk)
		{
			char_type ch = is.peek();
			bool exp=false;
			bool sign=true;
			bool dot = true;
			while(!is.fail())
			{
				ch = is.peek();
				if(sign)
				{
					sign = false;
					if(ch == chars::minus() || ch == chars::plus())
					{
						(*isk.m_pStr) += ch;
						is.ignore();
						continue;
					}
				}

				if(chars::numeric(ch))(*isk.m_pStr) += ch;
				else if(ch == chars::dot() && dot){(*isk.m_pStr) += ch;;dot=false;}
				else if((ch == chars::exponent_e() || ch == chars::exponent_E()) && !exp)
				{(*isk.m_pStr) += chars::exponent_e();exp=true;sign=true;dot=false;}
				else break;

				is.ignore();
			}
			return is;	
		}
	};
	typedef basic_istream_str_numeric<char>    istream_str_numeric;
	typedef basic_istream_str_numeric<wchar_t> wistream_str_numeric;


	// counts whitespaces in input stream
	// input:
	//			int& cnt - where to place count
	//			int tbl - how mush spaces in one tab (default 1)
	//                    we assume regular grid tabulations 
	class istream_wsp_count
	{
		int* m_pCnt;
		int  m_iTbl;
	public:
		istream_wsp_count(int & cnt,int tbl=1) : m_pCnt(&cnt),m_iTbl(tbl){}
		friend inline std::istream& operator>>(std::istream& is,istream_wsp_count & isk)
		{
			char ps;
			if(is.fail())return is;

			*isk.m_pCnt = 0;
			while(1)
			{
				ps = is.peek();
				if(!char_is_wsp(ps))break;
				if(ps == '\t') 
					(*isk.m_pCnt)+= isk.m_iTbl-(*isk.m_pCnt)%isk.m_iTbl;
				else 
					(*isk.m_pCnt)+= 1;
				is.ignore();
			}
			return is;	
		}
	};


	// class wsp_delim uses to cheked scip delimeters between tokens and optional
	// whitespaces for example:
	// int a,b
	// cin >> wsp_delim('{') >> a >> wsp_delim(',') >> b >> wsp_delim('}');
	// will handle input like this "{ 123, 456 }"

	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_wsp_delim : public detail::base_istream_reader<Elem,Tr>
	{
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
		char_type m_tl;
	public:
		basic_wsp_delim(char_type tl) : m_tl(tl){}

		friend istream_type& operator >> (istream_type& is,basic_wsp_delim<Elem,Tr> & sp)
		{
			if(is.fail())return is;
			char_type ch[2];
			while(true)
			{
				is.get(ch,2);
				if(ch[0] == sp.m_tl) break;
				if(!chars::whitespace(ch[0])) // erros
				{
					is.setstate( std::ios::failbit );
					return is;	
				}
			}
			is >> wsp_scip; // scip tailing spaces
			return is;	
		}
	};
	typedef basic_wsp_delim<char> wsp_delim;
	typedef basic_wsp_delim<wchar_t> wsp_delimw;

	template <char D>inline ::std::istream& t_wsp_delim(::std::istream& is){wsp_delim d(D);return is >> d;}
	template <wchar_t D>inline ::std::wistream& t_wsp_delimw(::std::wistream& is){wsp_delimw d(D);return is >> d;}


	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_wsp_delims : public detail::base_istream_reader<Elem,Tr>
	{
		GSYS_DETAIL_TSTREAM_TYPEDEFS;
		char_type const* m_tl;
	public:
		basic_wsp_delims(char_type const* tl) : m_tl(tl){}

		friend istream_type& operator >> (istream_type& is,basic_wsp_delims<Elem,Tr> & sp)
		{
			if(is.fail())return is;
			char_type ch[2];
			while(true)
			{
				is.get(ch,2);
				for(char_type const* pt = sp.m_tl;pt!=0;pt++)
				{
					if(ch[0] == *pt) 
					{
						is >> wsp_scip; // scip tailing spaces
						return is;	
					}
				}
				if(!chars::whitespace(ch[0])) // erros
				{
					is.setstate( std::ios::failbit );
					return is;	
				}
			}
			is >> wsp_scip; // scip tailing spaces
			return is;	
		}
	};
	typedef basic_wsp_delims<char> wsp_delims;
	typedef basic_wsp_delims<wchar_t> wsp_delimsw;

	// class stream_array uses to stream array of objects whits have operator [ ] and size
	// it can be static array's vectord & etc
	// example
	// float buf[20];
	// is >> stream_array<float*>(buf,20);
	// os << stream_array<float*>(buf,20);

	template<class T>
	class stream_array
	{
		typedef gsys_uint size_type;
		typedef T value_type;
		T* m_pt;
		size_type m_sz;
	public:
		stream_array(T const* pt, size_type sz) : m_pt(const_cast<T*>(pt)),m_sz(sz) {}
		friend inline std::istream& operator>>(std::istream& is,stream_array & ar)
		{
            wsp_delim D_BO('{');wsp_delim D_BC('}');wsp_delim D_D(',');
			is >> D_BO;
			for(size_type i=0;i<ar.m_sz-1;i++)
				is >> ar.m_pt[i] >> D_D;
			is >> ar.m_pt[ar.m_sz-2] >> D_BC;
			return is;
		}
		friend inline std::ostream& operator<<(std::ostream& os,stream_array const& ar)
		{
			os << '{';
			for(size_type i=0;i<ar.m_sz-1;i++)
				os << ar.m_pt[i] << ',';
			os << ar.m_pt[ar.m_sz-2] << '}';
			return os;
		}
	};

	template<typename T>
	std::istream& stream_container(std::istream& is, T& ptc)
	{
		typename T::value_type val;
		char ps;
		ptc.clear();
		is >> wsp_delim('{');
		ps = is.peek();
		if(ps != '}')
		{
			is >> val;
			ptc.push_back(val);
			while(true)
			{
				is >> ws;
				ps = is.peek();
				if(ps == '}')// stop adding
					break;

				is >> wsp_delim(',');
				is >> val; // else get new value
				ptc.push_back(val);
			}
		}
		is.ignore(); // ignore last }
		return is;
	}

	template<typename T>
	std::ostream& stream_container(std::ostream& os,T const& ptc)
	{
		os << '{';
		typename T::const_iterator it = ptc.begin();
		typename T::const_iterator itend = ptc.end();

		if(it == itend)return os << '}';

		while(true)
		{
			os << *it;
			++it;
			if(it == itend)	return os << '}';
			else os << ',';
		}
		return os;
	}

#define GSYSDETAIL_STREAM_CONTAINER(x) \
	template<class T>\
	std::ostream& operator<<(std::ostream& os,x<T> const& v)\
	{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);\
		return stream_container(os,v);\
	}\
	template<class T>\
	std::istream& operator>>(std::istream& is,x<T> & v)\
	{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);\
		return stream_container(is,v);\
	}

#define GSYSDETAIL_ISTREAM_BASIC_OBJ(_T,_OP)\
friend inline std::istream& operator>>(std::istream& is,_T & v)\
{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);\
	_OP;\
	return is;\
}

#define GSYSDETAIL_OSTREAM_BASIC_OBJ(_TYPE,_OP)\
friend inline std::ostream& operator<<(std::ostream& os,_TYPE const& v)\
{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);\
	_OP;\
	return os;\
}

#define GSYSDETAIL_OSTREAM_STD_BASIC_OBJ(_TYPE,_OP)\
	friend inline std::ostream& operator<<(std::ostream& os,_TYPE const& v)\
	{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);\
	const char DO='{';const char D=',';const char DC='}';\
	os << _OP;\
	return os;\
	}

#define GSYSDETAIL_ISTREAM_STD_BASIC_OBJ(_TYPE,_OP)\
friend inline std::istream& operator>>(std::istream& is,_TYPE & v)\
{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);\
	gsys::wsp_delim DO('{');gsys::wsp_delim DC('}');gsys::wsp_delim D(',');\
	is >> _OP;\
	return is;\
}

//	template<typename T>
//	std::string inline to_str(T i){return boost::lexical_cast<std::string>(i);}

	template<typename T>
	std::string inline to_string(T i){return boost::lexical_cast<std::string>(i);}

}

#endif