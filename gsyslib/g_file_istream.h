#pragma once
#ifndef G_FILE_ISTREAM_H
#define G_FILE_ISTREAM_H

#include "g_gsys.h"
#include "g_text.h"
#include "g_file.h"

#include <string>
#include <streambuf>

namespace gsys
{
	template <class Elem, class Tr = char_traits<Elem> >
		class basic_gfilestreambuf : public basic_streambuf<Elem, Tr>
	{	
		gfile* m_pFile;
		Elem m_CurrenetElem;
		bool m_bReaded;
	public:
		typedef Tr traits_type;
		typedef typename basic_streambuf<Elem, Tr>::int_type int_type;
		typedef typename basic_streambuf<Elem, Tr>::char_type char_type;
	public:
	
		basic_gfilestreambuf()
		{
			m_pFile = 0;
			m_bReaded = false;
		}

		explicit basic_gfilestreambuf(gfile* file)
		{
			m_pFile = file;
			m_bReaded = false;
		}
		bool init(gfile* file)
		{
			m_pFile = file;
			return bool(*file);
		}

	protected:
		streamsize xsgetn(typename basic_streambuf<Elem, Tr>::char_type* s, streamsize n)
		{
			if(!m_pFile) return 0;
			return (streamsize)m_pFile->tread(s,n);
		}

		typename basic_streambuf<Elem, Tr>::int_type underflow( )
		{
			if(m_bReaded)
				return traits_type::to_int_type(m_CurrenetElem);
			if(!m_pFile)return traits_type::eof();
			int i = m_pFile->tread(&m_CurrenetElem,1);
			if(i == 0)return traits_type::eof();
            m_bReaded = true;
			return traits_type::to_int_type(m_CurrenetElem);
		}

		typename basic_streambuf<Elem, Tr>::int_type uflow( )
		{
			int_type tm = underflow( );
			m_bReaded = false;
			return tm;
		}

		typename basic_streambuf<Elem, Tr>::int_type overflow(typename basic_streambuf<Elem, Tr>::int_type _Meta)
		{
			if(_Meta == traits_type::eof())return traits_type::not_eof(_Meta);
			if(!m_pFile)return traits_type::eof();
			char_type ch =   traits_type::to_char_type(_Meta);
			m_pFile->twrite(&ch);
			return  traits_type::not_eof(_Meta);
		}

		int sync()
		{
			if(!m_pFile)return -1;
			m_pFile->flush();
			return 0;
		}
	};

	typedef basic_gfilestreambuf<char> gfilestreambuf;

	template <class Elem, class Tr = char_traits<Elem> >
		class basic_igfilestream : public basic_istream<Elem, Tr>
	{
		basic_gfilestreambuf<Elem,Tr> m_buf;
	public:
		basic_igfilestream():basic_istream<Elem, Tr>(&m_buf){}
		explicit basic_igfilestream(gfile* pFile):
				m_buf(pFile),
				basic_istream<Elem, Tr>(&m_buf){}
		bool init(gfile* pFile){return m_buf.init(pFile);}
	};

	typedef basic_igfilestream<char> igfilestream;

	template <class Elem, class Tr = char_traits<Elem> >
	class basic_ogfilestream : public basic_ostream<Elem, Tr>
	{
		basic_gfilestreambuf<Elem,Tr> m_buf;
	public:
		basic_ogfilestream():basic_istream<Elem, Tr>(&m_buf){}
		explicit basic_ogfilestream(gfile* pFile):
			    m_buf(pFile),
				basic_ostream<Elem, Tr>(&m_buf){}
		bool init(gfile* pFile){return m_buf.init(pFile);}
	};

    typedef basic_ogfilestream<char> ogfilestream;
};

#endif