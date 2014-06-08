#pragma once
#ifndef G_BINSTREAM_H
#define G_BINSTREAM_H

#include "g_gsys.h"
#include "g_data_buffer.h"

#include "g_types.h"
#include "g_text.h"

namespace gsys
{
	class biniostream_buffer
	{
	public:
		typedef gsys_ulong size_type;
		typedef gsys_byte value_type;
		typedef value_type const* const_pointer;
		typedef value_type * pointer;
	public:
		virtual void put(value_type const* data, size_type sz){}

		virtual void get(value_type * data, size_type sz){}
		virtual value_type const* get_ref(size_type sz){return 0;}
	};
	////////////////////////////////////////////////////////////////////
	class binistream
	{
		biniostream_buffer* m_pBuffer;
	public:
		typedef gsys_ulong size_type;

		binistream(biniostream_buffer* pBuffer):
		  m_pBuffer(pBuffer){}

		void const* get_ref(size_type sz)
		{GSYS_CFN;GSYS_ASSERTOBJ(m_pBuffer);
			return m_pBuffer->get_ref(sz);
		}

		template<class T>
		T const* get_tref(size_type sz)
		{GSYS_CFN;GSYS_ASSERTOBJ(m_pBuffer);
			return reinterpret_cast<T const*>(m_pBuffer->get_ref(sz*sizeof(T)));
		}

		template<class T>
		binistream & get_string(gsys_dword &size, T const* &dta)
		{
			get(size);
			dta = get_tref<T>(size);
			return *this;
		}
		// container functions

		template<class T>
		binistream & get_rawdata(T& dta)
		{GSYS_CFN;GSYS_ASSERTOBJ(m_pBuffer);
			GSYS_ASSERTREF(dta);
			m_pBuffer->get((biniostream_buffer::pointer)&dta,sizeof(dta));
			return *this;
		}

		template<class T>
		binistream & get_rawdata(T* dta,size_type sz)
		{GSYS_CFN;GSYS_ASSERTOBJ(m_pBuffer);
			GSYS_ASSERTREF(dta);
			m_pBuffer->get((biniostream_buffer::pointer)dta,sizeof(*dta)*sz);
			return *this;
		}

		template<class T>
		binistream & get(T& dta)
		{
			*this >> dta;
			return *this;
		}
	};

	//---------------------------------------------------------------------------
	// std::basic_string
	template<class _Elem,class _Traits,class _Ax>
	inline binistream & operator >> (binistream &is,std::basic_string<_Elem,_Traits,_Ax> & str)
	{
		gsys_dword sz;
		_Elem const* dta;
		is.get_string(sz,dta);
		str.assign(dta,sz);
        return is;
	}
	//---------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////
	class binostream
	{
		biniostream_buffer* m_pBuffer;
	public:
		typedef gsys_ulong size_type;
		typedef gsys_byte value_type;

		binostream(biniostream_buffer* pBuffer):
		  m_pBuffer(pBuffer){}

		template<class T>
		binostream & put_rawdata(T const* dta,size_type sz)
		{GSYS_CFN;GSYS_ASSERTOBJ(m_pBuffer);
			GSYS_ASSERTPT(dta,sz);
			m_pBuffer->put((biniostream_buffer::const_pointer)dta,sizeof(*dta)*sz);
			return *this;
		}

		template<class T>
		binostream & put_rawdata(T const& dta)
		{GSYS_CFN;GSYS_ASSERTOBJ(m_pBuffer);
			GSYS_ASSERTREF(dta);
			m_pBuffer->put((biniostream_buffer::const_pointer)&dta,sizeof(dta));
			return *this;
		}

		template<class T>
		binostream & put(T const& dta)
		{
			*this << dta;
			return *this;
		}

		template<class T>
		binostream & put_string(T const* dta,gsys_dword sz)
		{
			put_rawdata(sz);
			put_rawdata(dta,sz);
			return *this;
		}	

		template<class T>
		binostream & put_string(T const* dta)
		{
			return put_string(dta,basic_char_classify<T>::length(dta));
		}
		// container functions

		template<class T>
		binostream & put_container(T const& cnt)
		{
			put_rawdata((gsys_size_t)cnt.size());
			_const_foreach(typename T,cnt,it) put(*it);
			return *this;
		}	

		template<class T>
		binostream & put_container_raw(T const& cnt)
		{
			put_rawdata((gsys_size_t)cnt.size());
			_const_foreach(typename T,cnt,it)put_rawdata(*it);
			return *this;
		}

		template<class T>
		binostream & put_container_ptr(T const& cnt)
		{
			put_rawdata((gsys_size_t)cnt.size());
			_const_foreach(typename T,cnt,it)put(**it);
			return *this;
		}

	};

	//---------------------------------------------------------------------------
	// std::basic_string
	template<class _Elem,class _Traits,class _Ax>
	inline binostream & operator << (binostream &os,std::basic_string<_Elem,_Traits,_Ax> const& str)
	{
		os.put_string(str.data(),str.length());
		return os;
	}
	//---------------------------------------------------------------------------

	// memory binstream buffer used for collecting data
	class mem_binostream_buffer : public biniostream_buffer
	{
		raw_data_buffer<value_type> m_Buffer;
		size_type m_szPos;
	protected:
		void put(value_type const* data,size_type sz)
		{
			size_type rsz = (m_szPos + sz);
			if(rsz > m_Buffer.size())m_Buffer.resize(rsz);
			memcpy(&m_Buffer[m_szPos],data,sz);
			m_szPos += sz;
		}
	public:
		mem_binostream_buffer():m_szPos(0){}
		virtual ~mem_binostream_buffer(){}
		void reset(){m_szPos=0;}
		value_type const* data()const{return m_Buffer.get();}
		value_type* data(){return m_Buffer.get();}
		size_type size()const{return m_szPos;}
	};

	class mem_binostream : public binostream
	{
		mem_binostream_buffer buffer;
	public:
		mem_binostream():binostream(&buffer){};
		value_type const* data()const{return buffer.data();}
		gsys_size_t size()const{return buffer.size();}
	};

	// memory binstream buffer used for retrewing data
	class mem_binistream_buffer : public biniostream_buffer
	{
		value_type const* m_pDta;
		value_type const* m_pPos;
		value_type const* m_pEnd;
	protected:

	public:
		mem_binistream_buffer(value_type const* dta,size_type size):
			m_pDta(dta),m_pPos(dta),m_pEnd(dta+size){}

		mem_binistream_buffer(void const* dta,size_type size):
			m_pDta((value_type const*)dta),m_pPos((value_type*)dta),m_pEnd(((value_type const*)dta)+size){}

		void get(value_type * data, size_type sz)
		{
			value_type const* pt = m_pPos+sz;
			if(pt > m_pEnd)
			{
				// error 
				return;
			}
			memcpy(data,m_pPos,sz);
			m_pPos = pt;
		}
		value_type const* get_ref(size_type sz)
		{
			value_type const* pt = m_pPos;
			if(m_pPos+sz > m_pEnd)
			{
				// error 
				return 0;
			}
			m_pPos += sz;
			return pt;
		}
	};
	class mem_binistream : public binistream
	{
		mem_binistream_buffer buffer;
	public:
		typedef gsys_byte value_type;

		mem_binistream(value_type const* dta,size_type size):
		  buffer(dta,size),binistream(&buffer){}

		  mem_binistream(void const* dta,size_type size):
		  buffer(dta,size),binistream(&buffer){}
	};

	// defines for simplifying integration to gsys

#define GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(_T)\
	inline gsys::binistream & operator >> (gsys::binistream &is,_T& v){return is.get_rawdata((_T&)v);}\
	inline gsys::binostream & operator << (gsys::binostream &os,_T v){return os.put_rawdata((_T)v);}

#define GSYSDETAIL_BINSTREAM_RAWDATA(_T)\
	inline gsys::binistream & operator >> (gsys::binistream &is,_T& v){return is.get_rawdata(v);}\
	inline gsys::binostream & operator << (gsys::binostream &os,_T const& v){return os.put_rawdata(v);}

#define GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(_T)\
	friend inline gsys::binistream & operator >> (gsys::binistream &is,_T& v){return is.get_rawdata(v);}\
	friend inline  gsys::binostream & operator << (gsys::binostream &os,_T const& v){return os.put_rawdata(v);}


	// floating point
	GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_float)
	GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_double)

	// integers
	GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_long)
	GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_int)
	GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_char)
	GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_short)
	// unsigned integers

	GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_unsigned_long)
	GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_unsigned_int)
	GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_unsigned_char)
	GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_unsigned_short)
	// boolean
	GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_bool)

	// integers
    #ifdef GSYS_DETAIL_USE_64BIT_TYPES
	    GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_long_long)
	    GSYSDETAIL_BINSTREAM_LITTLE_RAWDATA(gsys_unsigned_long_long)
    #endif

	class binostream_iterator : public std::iterator<std::output_iterator_tag, void, void,	void, void>
	{	
	public:
		typedef binostream ostream_type;
		typedef ostream_type::value_type value_type;
		typedef value_type char_type;

		binostream_iterator(ostream_type& _Ostr):_Myostr(&_Ostr){}	

		binostream_iterator& operator=(const value_type& _Val)
		{	
			*_Myostr << _Val;
			return (*this);
		}

		binostream_iterator& operator*()
		{	// pretend to return designated value
			return (*this);
		}

		binostream_iterator& operator++()
		{	// pretend to preincrement
			return (*this);
		}

		binostream_iterator operator++(int)
		{	// pretend to postincrement
			return (*this);
		}

	protected:
		ostream_type *_Myostr;	// pointer to output stream
	};

};


#endif

