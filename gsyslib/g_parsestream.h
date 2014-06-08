#pragma once
#include <ios> 
namespace gsys
{
	template <class Elem, class Tr = char_traits<Elem> > class basic_iparsestream;

	template <class Elem, class Tr = char_traits<Elem> >
	class basic_iparsestreambuf : public basic_streambuf<Elem, Tr>
	{
		friend class basic_iparsestream<Elem,Tr>;
		char_type const* _begin;
		char_type const* _end;
		char_type const* position;

		basic_iparsestreambuf(){setup(0);}

		explicit basic_iparsestreambuf(char_type const* src){setup(src);}
		basic_iparsestreambuf(char_type const* src,char_type const* end){setup(src,end);}
		void setup(char_type const* src,char_type const* end=0)
		{
			_begin=src;
			position=src;
			if(end)_end = end;
			else if(_begin)	_end = _begin+Tr::length(_begin);
			else _end=0;
		}

	protected:
		typename int_type underflow( )
		{
			if(position >= _end)return traits_type::eof();
			return (*position)?traits_type::to_int_type(*position):traits_type::eof();
		}
		typename int_type uflow( )
		{
			if(position >= _end)return traits_type::eof();
			return (*position)?traits_type::to_int_type(*(position++)):traits_type::eof();
		}
		streamsize xsgetn(typename char_type* s, streamsize n)
		{
			Elem const* stop = position+n;
			if(stop > _end)stop = _end;
			Elem const* i = position;
			for(;i<stop;i++,s++)
			{
				if(!*i)break;
				*s = *i;
			}
			n = (streamsize)(i-position);
			position = stop;
			return  n;
		}
		pos_type seekoff(off_type _Off,ios_base::seekdir _Way,ios_base::openmode _Which)
		{
			char_type const* pos;

			if(_Way == ios_base::beg)pos = _begin + _Off;
			if(_Way == ios_base::cur)pos = position + _Off;
			if(_Way == ios_base::end)pos = _end + _Off;

			if(pos < _begin ||  pos > _end)return pos_type(off_type(-1));
			position = pos;
			return  (streamsize)(pos-_begin);			
		}
		pos_type seekpos(pos_type _Sp,ios_base::openmode _Which)
		{
			char_type const* pos = _begin + _Sp;
			if(pos > _end)return pos_type(off_type(-1));
			position = pos;
			return _Sp;			
		}
	};

	template <class Elem, class Tr>
	class basic_iparsestream : public basic_istream<Elem, Tr>
	{
	public:
		typedef basic_string<Elem,Tr> string_type;
		typedef gsys_ulong size_type;
	protected:
		basic_iparsestreambuf<Elem,Tr> m_buf;
		typedef gsys::basic_char_classify<Elem> chars;
		string_type m_filename;
	public:

		basic_iparsestream():basic_istream<Elem, Tr>(&m_buf){}
		explicit basic_iparsestream(Elem const* pData,Elem const* szFn=0):
		m_buf(pData),m_filename(szFn?szFn:(Elem const*)L""),basic_istream<Elem, Tr>(&m_buf){}
		explicit basic_iparsestream(string_type const& pData,Elem const* szFn=0):
		m_buf(pData.c_str()),m_filename(szFn?szFn:(Elem const*)L""),basic_istream<Elem, Tr>(&m_buf){}

		basic_iparsestream(Elem const* pData,size_type nDataSize,Elem const* szFn=0):
		m_buf(pData,pData+nDataSize),m_filename(szFn?szFn:(Elem const*)L""),basic_istream<Elem, Tr>(&m_buf){}

		void setup(std::basic_string<Elem,Tr> const& pData,Elem const* szFn)
		{m_buf.setup(pData.c_str());init(&m_buf);m_filename = (szFn?szFn:(Elem const*)L"");}
		void setup(Elem const* pData,Elem const* szFn)
		{m_buf.setup(pData);init(&m_buf);m_filename = (szFn?szFn:(Elem const*)L"");}

		void setup(Elem const* pData,size_type nDataSize,Elem const* szFn)
		{m_buf.setup(pData,pData+nDataSize);init(&m_buf);m_filename = (szFn?szFn:(Elem const*)L"");}

		void setup(Elem const* pBegin,Elem const* pEnd,Elem const* szFn)
		{m_buf.setup(pBegin,pEnd);init(&m_buf);m_filename = (szFn?szFn:(Elem const*)L"");}

		string_type const& filename(){return m_filename;};

		bool get_pos_info(int* pline, int* pcol=0,string_type* pdata=0)
		{
			int cpos = tellg();
			int line=1,col=1;
			char_type const* pLineStart=m_buf._begin;
			char_type const* pos =  m_buf._begin;

			while(pos < m_buf._end)
			{
				if(pos >= m_buf.position)
				{
					if(pline)*pline = line;
					if(pcol)*pcol  = col;
					if(pdata)
					{
						pos = pLineStart;
						pdata->clear();
						while(pos < m_buf._end)
						{
							if(chars::newline(*pos))break;
							*pdata += *pos;       
							pos++;
						}
					}
					return true;
				}
				if(chars::newline(*pos))
				{
					pos++;
					if(chars::newline(*pos) && pos[0] != pos[-1])pos++;
					line++;
					col=1;
					pLineStart = pos;
				}
				else
				{
					col++;
					pos++;
				}
			}
			return false;
		}
	};

	typedef basic_iparsestream<char> iparsestream;
	typedef basic_iparsestream<wchar_t> wiparsestream;

}