#pragma once

#include "g_gsys.h"
#include "g_file.h"
#include "g_data_buffer.h"

#include "g_alg.h"

namespace gsys
{
	class mem_file : public gfile
	{
	private:
		typedef raw_data_buffer<gsys_byte,0,256> buffer_type;

		buffer_type m_Buffer;
		size_type m_stCurrentPos;

	public: // init
		mem_file():m_stCurrentPos(0){}
		void init(size_type size=0)
		{
			m_Buffer.reset(size);
			m_stCurrentPos = 0;
		}
		gsys_byte* buffer()
		{
			return m_Buffer.get();
		}

	public:   // file interface
		bool valid()const
		{
			return true;
		}
		size_type read(void* pBuffer, size_type iSize, size_type iCount = 1)
		{
			size_type sz = iSize*iCount;

			if(m_stCurrentPos + sz > m_Buffer.size())
			{
				size_type dif = m_stCurrentPos + sz -  m_Buffer.size();
				iCount -= (dif / iSize);
				sz = iSize*iCount;
			}
			if(sz)
			{
				gsys_byte* pt = &m_Buffer[m_stCurrentPos];
				memcpy(pBuffer,pt,sz);
				m_stCurrentPos += sz;
			}
			return iCount;
		}
		size_type write(void const* pBuffer, size_type iSize, size_type iCount = 1)
		{
			size_type sz = iSize*iCount;

			if(m_stCurrentPos + sz > m_Buffer.size())
				m_Buffer.resize(m_stCurrentPos + sz);


			gsys_byte* pt = &m_Buffer[m_stCurrentPos];
			memcpy(pt,pBuffer,sz);

			m_stCurrentPos += sz;

			return iCount;
		}
		size_type seek(long lOffset, FileSeek iOrigin)
		{
			switch(iOrigin)
			{
			case SeekCur:
				m_stCurrentPos += lOffset;
				break;
			case SeekSet:
				m_stCurrentPos = lOffset;
				break;
			case SeekEnd:
				m_stCurrentPos = m_Buffer.size()-lOffset;
				break;
			}
			bound(m_stCurrentPos,0ul,m_Buffer.size());
			return m_stCurrentPos;
		}
		size_type size()
		{
			return m_Buffer.size();
		}
		size_type pos()
		{
			return m_stCurrentPos;
		}
		void flush()
		{
		}
		void close()
		{
		}
		bool eof()const
		{
			return m_stCurrentPos == m_Buffer.size();
		}
	};


	class memptr_file : public gfile
	{
	public: // init
		typedef gsys_byte value_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
	private:
		gsys_byte* m_Buffer;
		size_type m_buffer_size;
		size_type m_stCurrentPos;

	public: // init
		memptr_file(gsys_byte* buf,size_type size):
			m_Buffer(buf),m_buffer_size(size),m_stCurrentPos(0){}

		memptr_file(gsys_byte const* buf,size_type size):
			m_Buffer(const_cast<gsys_byte*>(buf)),m_buffer_size(size),m_stCurrentPos(0){}

		gsys_byte* buffer()
		{
			return m_Buffer;
		}

	public:   // file interface
		bool valid()const
		{
			return true;
		}
		size_type read(void* pBuffer, size_type iSize, size_type iCount = 1)
		{
			size_type sz = iSize*iCount;

			if(m_stCurrentPos + sz > m_buffer_size)
			{
				size_type dif = m_stCurrentPos + sz -  m_buffer_size;
				iCount -= (dif / iSize);
				sz = iSize*iCount;
			}
			if(sz)
			{
				gsys_byte* pt = &m_Buffer[m_stCurrentPos];
				memcpy(pBuffer,pt,sz);
				m_stCurrentPos += sz;
			}
			return iCount;
		}
		size_type write(void const* pBuffer, size_type iSize, size_type iCount = 1)
		{
			size_type sz = iSize*iCount;

			if(m_stCurrentPos + sz > m_buffer_size) // End of file
			{
				m_stCurrentPos = m_buffer_size;
				return 0;
			}

			gsys_byte* pt = &m_Buffer[m_stCurrentPos];
			memcpy(pt,pBuffer,sz);

			m_stCurrentPos += sz;

			return iCount;
		}
		size_type seek(long lOffset, FileSeek iOrigin)
		{
			switch(iOrigin)
			{
			case SeekCur:
				m_stCurrentPos += lOffset;
				break;
			case SeekSet:
				m_stCurrentPos = lOffset;
				break;
			case SeekEnd:
				m_stCurrentPos =m_buffer_size-lOffset;
				break;
			}
			bound(m_stCurrentPos,0ul,m_buffer_size);
			return m_stCurrentPos;
		}
		size_type size()
		{
			return m_buffer_size;
		}
		size_type pos()
		{
			return m_stCurrentPos;
		}
		void flush()
		{
		}
		void close()
		{
		}
		bool eof()const
		{
			return m_stCurrentPos == m_buffer_size;
		}
		pointer position(){return m_Buffer+m_stCurrentPos;}
	};

}