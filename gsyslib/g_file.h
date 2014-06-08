#pragma once
#ifndef G_FILE_H
#define G_FILE_H

#include "g_gsys.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#if defined(GSYS_DETAIL_UNICODE_OS) && !defined(GSYS_DISABLE_UNICODE_GFILE)  
#	define GSYS_GFILE_UNICODE
#	define GSYS_DETAIL_GFILE_TEXT(x) L##x
#else
#	define GSYS_DETAIL_GFILE_TEXT(x) x
#endif

namespace gsys 
{
#ifdef GSYS_GFILE_UNICODE  
	typedef wchar_t gfile_char_t;
#else
	typedef char    gfile_char_t;
#endif

	/////////////////////////////////////////////////////////////////////////////////////// 
	class gfile : noncopyable
	{
	public:
		typedef gsys_ulong size_type;
		typedef gfile_char_t char_type;
		enum FileMode
		{
			ModeRead,
			ModeWrite,
			ModeReadWrite,
			ModeAppend,
			ModeCreate
		};
		enum FileSeek
		{
			SeekSet = 0,
			SeekCur = 1,
			SeekEnd = 2
		};

	public:  // constructors  
		gfile(){}
		virtual ~gfile(){}
	public:   // file interface
		virtual bool valid()const=0;
		virtual bool eof()const=0;

		static const size_type npos = 0xffffffff;

		virtual size_type read(void* pBuffer, size_type iSize, size_type iCount = 1)=0;
		virtual size_type write(void const* pBuffer, size_type iSize, size_type iCount = 1)=0;
		virtual size_type seek(long lOffset, FileSeek iOrigin)=0;
		virtual size_type size()=0;
		virtual size_type pos()=0;
		virtual void flush()=0;
		virtual void close()=0;
		////////////////////////////////////////////////
	public: // helpers
		operator bool(){return valid();}

		void set_pos(size_type pos)
		{
			seek(pos,SeekSet);
		}

		void rewind()
		{
			seek(0,SeekSet);
		}

		size_type seek(long lOffset, long iOrigin){return seek(lOffset,(FileSeek)iOrigin);}
		size_type seek(gsys_int64 lOffset, long iOrigin){return seek(lOffset,(FileSeek)iOrigin);}
		size_type seek(gsys_int64 lOffset, FileSeek iOrigin){return seek((long)lOffset,(FileSeek)iOrigin);}
		size_type seek(ulong lOffset, long iOrigin){return seek(lOffset,(FileSeek)iOrigin);}
		size_type seek(ulong lOffset, FileSeek iOrigin){return seek((long)lOffset,(FileSeek)iOrigin);}
		size_type seek(int lOffset, long iOrigin){return seek(lOffset,(FileSeek)iOrigin);}
		size_type seek(int lOffset, FileSeek iOrigin){return seek((long)lOffset,(FileSeek)iOrigin);}
		size_type seek(uint lOffset, long iOrigin){return seek(lOffset,(FileSeek)iOrigin);}
		size_type seek(uint lOffset, FileSeek iOrigin){return seek((long)lOffset,(FileSeek)iOrigin);}

		size_type skip(long num_bytes)
		{
			return seek(num_bytes,SeekCur);
		}


		template<class T> 
		int tread(T* pBuffer, size_type iCount = 1)
		{
			return read(pBuffer,sizeof(T),iCount);
		}

		template<class T> 
		int twrite(T const* pBuffer, size_type iCount = 1)
		{
			return write(pBuffer,sizeof(T),iCount);
		}

		template<class T> 
		gfile& operator << (T const& obj)
		{
			twrite(&obj);
			return *this;
		}

		int GSYS_CDECL printf(char* str, ... )
		{
			va_list argptr;
			char	text[1024];
			va_start (argptr,str);
			vsprintf (text, str, argptr);
			va_end (argptr);
			return twrite(&text[0],(int)strlen(text));
		}

		template<class T> 
		gfile& operator >> (T & obj)
		{
			tread(&obj);
			return *this;
		}

		template<class SizeT>
		bool load_sized_buffer(gsys_byte* buffer,size_type buffer_size,SizeT &cbSize)
		{
			if(!tread(&cbSize))return false;
			if(cbSize<sizeof(SizeT))return false;

			size_type read_size = cbSize-sizeof(SizeT);

			if(buffer_size > read_size)
			{
				if(!read(buffer,read_size))return false;
				memset(buffer+read_size,0,buffer_size-read_size);
			}
			else if(read_size > buffer_size)
			{
				if(!read(buffer,buffer_size))return false;
				skip(read_size-buffer_size);
			}
			else 
			{
				if(!read(buffer,buffer_size))return false;
			}
			return true;
		}

		template<class Struct,class SizeT>
		bool load_sized_struct(Struct& st)
		{
			BOOST_STATIC_ASSERT(sizeof(Struct)>sizeof(SizeT));

			SizeT& cbSize = *reinterpret_cast<SizeT*>(&st);
			gsys_byte* buffer = reinterpret_cast<gsys_byte*>(&st)+sizeof(SizeT);
			size_type buffer_size = sizeof(Struct)-sizeof(SizeT);

			return load_sized_buffer(buffer,buffer_size,cbSize);
		}
	};

	template<> 
	inline gfile& gfile::operator << <std::string>(std::string const& obj)
	{
		twrite(obj.data(),(size_type)obj.size());
		return *this;
	}

	template<> 
	inline gfile& gfile::operator << <std::wstring>(std::wstring const& obj)
	{
		twrite(obj.data(),(size_type)obj.size());
		return *this;
	}

	template<> 
	inline gfile& gfile::operator << <char*>(char* const& obj)
	{
		if(obj)twrite(obj,(size_type)strlen(obj));
		return *this;
	}

	template<> 
	inline gfile& gfile::operator << <wchar_t*>(wchar_t* const& obj)
	{
		if(obj)twrite(obj,(size_type)wcslen(obj));
		return *this;
	}


	inline size_t fwrite(const void *buffer,size_t size,size_t count,gfile *stream)
	{return stream->write(buffer,(gfile::size_type)size,(gfile::size_type)count);}
	inline size_t fread(void *buffer,size_t size,size_t count,gfile *stream)
	{return stream->read(buffer,(gfile::size_type)size,(gfile::size_type)count);}
	inline int fseek(gfile *stream,long offset,int origin)
	{return stream->seek(offset,origin);}
	inline int _fseeki64(gfile *stream, gsys_int64 offset,int origin)
	{return stream->seek(offset,origin);}
	inline int fflush(gfile *stream){stream->flush();return 0;}

#if defined(_MSC_VER)
	inline int fgetpos(gfile *stream,fpos_t *pos)
	{*pos = stream->pos();return 0;}
#endif

	#ifndef GSYS_DETAIL_BDS2006
	inline int feof(gfile* fstream){return fstream->eof();}
	#endif
}; //namespace gsys

#endif
