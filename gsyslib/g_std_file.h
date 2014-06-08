/*
	������ �������������:

	#include <g_std_file.h>
	struct A
	{
		int m_iA;
	};
	int main()
	{
		gsys::std_file File("file.bin","wb"); // ������� � ��������� �������� ����
		if(File) // ��������� �������� �� ����
		{
			int i=20;
			File << int(10) << float(1.0f) << i; // ����� ����������
			A a[3] = {{10},{20},{30};
			A b;
			File << a[0] << a[1] << a[2] << b; // ����� ���������
			File.twrite(a,3); // ����������� ������
			File.twrite(&b); 
			File.write(a,sizeof(A),3); // ������� ������ � ����
			File.write(&b,sizeof(A)); 
		}
		// ���� ����� ������ �������������
	}
	// ������ ����������

*/

#pragma once
#include "g_gsys.h"
#include "g_file.h"

#include <stdlib.h>
#include <stdarg.h>

namespace gsys 
{
	class std_file : public gfile
	{
		typedef std::basic_string<char_type> string_type;
	private:
		FILE* m_pFileHandle;
	public:
		bool valid()const{return m_pFileHandle != 0;}

		std_file():
			m_pFileHandle(0)
		{}
		~std_file()
		{
			close();
		}
		explicit std_file(char_type const* szFileName,FileMode mode):
			m_pFileHandle(0)
		{open(szFileName,mode);}
		explicit std_file(char_type const* szFileName,char_type const* mode):
			m_pFileHandle(0)
		{open(szFileName,mode);}

		explicit std_file(string_type const& szFileName,FileMode mode):
			m_pFileHandle(0)
		{open(szFileName.c_str(),mode);}
		explicit std_file(string_type const& szFileName,char_type const* mode):
			m_pFileHandle(0)
		{open(szFileName.c_str(),mode);}

		bool open(string_type const& szFileName,FileMode mode)
			{return open(szFileName.c_str(),mode);}

		bool open(const char_type* szFileName,const char_type* szMode)
		{
			GSYS_ASSERT(szFileName);
			//GSYS_ASSERT(strlen(szFileName));
			close();
#ifdef GSYS_GFILE_UNICODE
			m_pFileHandle = _wfopen(szFileName,szMode);
#else
			m_pFileHandle = fopen(szFileName,szMode);
#endif
			return valid();
		}

		bool open(const char_type* szFileName,FileMode mode)
		{
			char_type const* szModeString;
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
			default:
				GSYS_ASSERT(false);
				szModeString = 0;
				break;
			}
			return open(szFileName,szModeString);
		}
		void close()
		{
			if(m_pFileHandle)fclose(m_pFileHandle);
			m_pFileHandle = 0;
		}
		size_type read(void* pBuffer, size_type iSize, size_type iCount = 1)
		{
			GSYS_ASSERT(m_pFileHandle);
			int r = (int)fread(pBuffer,iSize,iCount,m_pFileHandle);
			return r;
		}
		size_type write(void const* pBuffer, size_type iSize, size_type iCount = 1)
		{
			GSYS_ASSERT(m_pFileHandle);
			return (size_type)fwrite(pBuffer,iSize,iCount,m_pFileHandle);
		}
		size_type seek(long lOffset, FileSeek iOrigin)
		{
			GSYS_ASSERT(m_pFileHandle);

			int order;
			switch (iOrigin)
			{
			case SeekCur:
				order = SEEK_CUR;
				break;
			case SeekEnd:
				order = SEEK_END;
				break;
			case SeekSet:
				order = SEEK_SET;
				break;
			default: 
				return npos;
			}

			fseek(m_pFileHandle,lOffset,order);
			long pos = ftell(m_pFileHandle);
			return pos>=0?pos:npos;
		}
		size_type size()
		{
			GSYS_ASSERT(m_pFileHandle);
			gsys_ulong cpos = ftell(m_pFileHandle);
			fseek(m_pFileHandle,0,SEEK_END);
			gsys_ulong size = ftell(m_pFileHandle);
			fseek(m_pFileHandle,cpos,SEEK_SET);
			return size;
		}
		size_type pos()
		{
			GSYS_ASSERT(m_pFileHandle);
			return ftell(m_pFileHandle);
		}
		void flush()
		{
			GSYS_ASSERT(m_pFileHandle);
			fflush(m_pFileHandle);
		}
		bool eof()const
		{
			GSYS_ASSERT(m_pFileHandle);
			return feof(m_pFileHandle) != 0;
		}
		FILE* handle(){return m_pFileHandle;}

	};
}; //namespace gsys 


