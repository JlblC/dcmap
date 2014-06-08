#pragma once
#include <ios> 
#include <streambuf>

//class basic_iparsestream;

template <class Elem, class Tr = std::char_traits<Elem> > class basic_iparsestream;


template <class Elem, class Tr = std::char_traits<Elem> >
class basic_iparsestreambuf : public std::basic_streambuf<Elem, Tr>
{
	typedef typename std::basic_streambuf<Elem, Tr> Inherited;
	typedef typename Inherited::char_type char_type;
	typedef typename Inherited::int_type int_type;
	typedef typename Inherited::pos_type pos_type;
	typedef typename Inherited::off_type off_type;
	typedef typename Inherited::traits_type traits_type;
	typedef int streamsize;

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
		_end = end?end:src;
	}

protected:
	int_type underflow( )
	{
		return (*position)?traits_type::to_int_type(*position):traits_type::eof();
	}
	int_type uflow( )
	{
		return (*position)?traits_type::to_int_type(*(position++)):traits_type::eof();
	}
	streamsize xsgetn(char_type* s, streamsize n)
	{
		Elem const* stop = position+n;
		for(Elem const* i = position;i<stop;i++,s++)
		{
			if(!*i)return i-position;
			*s = *i;
		}
		return n;
	}
	pos_type seekoff(off_type _Off,std::ios_base::seekdir _Way,std::ios_base::openmode _Which)
	{
		if(_end == _begin)_end = _begin+Tr::length(_begin);

		char_type const* pos;

		if(_Way == std::ios_base::beg)pos = _begin + _Off;
		if(_Way == std::ios_base::cur)pos = position + _Off;
		if(_Way == std::ios_base::end)pos = _end + _Off;

		if(pos < _begin ||  pos > _end)return pos_type(off_type(-1));
		position = pos;
		return pos-_begin;			
	}
	pos_type seekpos(pos_type _Sp,std::ios_base::openmode _Which)
	{
		if(_end == _begin)_end = _begin+Tr::length(_begin);
		char_type const* pos = _begin + _Sp;
		if(pos > _end)return pos_type(off_type(-1));
		position = pos;
		return _Sp;			
	}
};

template <class Elem, class Tr >
class basic_iparsestream : public std::basic_istream<Elem, Tr>
{
	typedef std::basic_istream<Elem, Tr> Inherited;
	typedef typename Inherited::char_type char_type;

public:
    typedef std::basic_string<Elem,Tr> string_type;
protected:
    basic_iparsestreambuf<Elem,Tr> m_buf;
    typedef gsys::basic_char_classify<Elem> chars;
    string_type m_filename;
public:

	basic_iparsestream():std::basic_istream<Elem, Tr>(&m_buf){}

    	explicit basic_iparsestream(Elem const* pData,Elem const* szFn=0):
        	m_buf(pData),m_filename(szFn?szFn:(Elem const*)L""),std::basic_istream<Elem, Tr>(&m_buf){}

    	explicit basic_iparsestream(string_type const& pData,Elem const* szFn=0):
        	m_buf(pData.c_str()),m_filename(szFn?szFn:(Elem const*)L""),std::basic_istream<Elem, Tr>(&m_buf){}

	void setup(std::basic_string<Elem,Tr> const& pData,Elem const* szFn=0)
    {m_buf.setup(pData.c_str());init(&m_buf);m_filename = (szFn?szFn:(Elem const*)L"");}
	void setup(Elem const* pData,Elem const* szFn=0)
    {m_buf.setup(pData);init(&m_buf);m_filename = (szFn?szFn:(Elem const*)L"");}

    string_type const& filename(){return m_filename;};

    bool get_pos_info(int* pline, int* pcol,string_type* pdata)
    {
        int cpos = Inherited::tellg();
        int line=1,col=1;
        char_type const* pLineStart=m_buf._begin;
        char_type const* pos =  m_buf._begin;

        while(pos < m_buf._end)
        {
            if(pos >= m_buf.position)
            {
                if(pline)*pline = line;
                if(pline)*pcol  = col;
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
