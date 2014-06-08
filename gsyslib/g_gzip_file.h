#ifndef __INCLUDED_g_gzip_file_h__
#define __INCLUDED_g_gzip_file_h__
#pragma once

#include "g_gsys.h"
#include "g_file.h"
#include "g_text.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define GSYS_ZLIB_STATIC_LINK
#include "g_zlib.h"

namespace gsys
{
	class gzip_file : public gfile
	{
	private:
		gzFile m_pFileHandle;
	public: // construction

		typedef std::basic_string<char_type> string_type;

	public: // open closr
		~gzip_file()
		{
			close();
		}
		gzip_file():m_pFileHandle(0){}

		gzip_file(const string_type& szFileName,FileMode mode,int cmpr=9):
		m_pFileHandle(0)
		{open(szFileName.c_str(),mode,cmpr);}

		gzip_file(const string_type& szFileName,const string_type& mode):
		m_pFileHandle(0)
		{open(szFileName.c_str(),mode.c_str());}

		gzip_file(const char_type* szFileName,FileMode mode,int cmpr=9):
			m_pFileHandle(0)
			{open(szFileName,mode,cmpr);}

		gzip_file(const char_type* szFileName,const char_type* mode):
			m_pFileHandle(0)
			{open(szFileName,mode);}

		bool open(const char_type* szFileName,const char_type* szMode)
		{
			close();
#ifdef GSYS_GFILE_UNICODE
			/* decode file string */
			int mode=0;
			const char_type* md = szMode;
			while(*md)
			{
				if(*md == 'a')mode |= _O_APPEND|_O_WRONLY;
				else if(*md == 'w')mode |= _O_WRONLY|_O_CREAT;
				else if(*md == 'b')mode |= _O_BINARY;
				else if(*md == 't')mode |= _O_TEXT;
				else if(*md == '+'){mode &= ~_O_WRONLY; mode |= _O_RDWR|_O_CREAT;}
				++md;
			}
			int file = _wopen(szFileName,mode,_S_IREAD | _S_IWRITE);
			m_pFileHandle = gzdopen(file,to_str(szMode).c_str());
#else
			m_pFileHandle = gzopen(szFileName,szMode);
#endif
			return valid();
		}

		bool open(const char_type* szFileName,FileMode mode,int cmpr=9)
		{
			string_type szModeString;
			switch(mode)
			{
			case ModeRead:
				szModeString = GSYS_DETAIL_GFILE_TEXT("rb");
				break;
			case ModeWrite:
			case ModeCreate:
				szModeString = GSYS_DETAIL_GFILE_TEXT("wb");
				break;
			case ModeAppend:
				szModeString = GSYS_DETAIL_GFILE_TEXT("ab");
				break;
			}

#ifdef GSYS_GFILE_UNICODE
			szModeString += to_wstr(cmpr);
#else
			szModeString += to_str(cmpr);
#endif

			return open(szFileName,szModeString.c_str());
		}

		void close()
		{
			if(m_pFileHandle)gzclose(m_pFileHandle);
			m_pFileHandle = 0;
		}
	public: // gfile interface
		bool valid()const{return m_pFileHandle != 0;}
		size_type read(void* pBuffer, size_type iSize, size_type iCount = 1)
		{
			GSYS_ASSERT(m_pFileHandle);
			if(iSize == 0 || iCount == 0)return 0;
			size_type sz = (size_type)gzread(m_pFileHandle,pBuffer,iSize*iCount);
			return sz/iSize;
		}
		size_type write(void const* pBuffer, size_type iSize, size_type iCount = 1)
		{
			GSYS_ASSERT(m_pFileHandle);
			if(iSize == 0 || iCount == 0)return 0;
			size_type sz = (size_type)gzwrite(m_pFileHandle,
									(const voidp)pBuffer,iSize*iCount);
			return sz/iSize;
		}
		size_type seek(long lOffset, FileSeek iOrigin)
		{
			GSYS_ASSERT(m_pFileHandle);
			gzseek(m_pFileHandle,lOffset,(int)iOrigin);
			return gztell(m_pFileHandle);
		}
		size_type size()
		{
			return 0;
		}
		size_type pos()
		{
			GSYS_ASSERT(m_pFileHandle);
			return gztell(m_pFileHandle);
		}
		bool eof()const
		{
			GSYS_ASSERT(m_pFileHandle);
			return gzeof(m_pFileHandle) != 0;
		}

		void flush() // do not flush 
		{
			gzflush(m_pFileHandle,Z_SYNC_FLUSH);
		}	
	};
};

#endif // __INCLUDED_g_gzip_file_h__
