#pragma once
#ifndef G_FILE_BINSTREAM_H
#define G_FILE_BINSTREAM_H

#include "g_binstream.h"
#include "g_file.h"

namespace gsys
{
	class gfile_biniostream_buffer : public biniostream_buffer
	{
		raw_data_buffer<value_type,0,512> m_Buffer;
		gfile* m_pFile;
		friend class gfile_binistream;
		friend class gfile_binostream;

		gfile_biniostream_buffer():m_pFile(0){}
	public:
		gfile_biniostream_buffer(gfile* pFile):m_pFile(pFile){GSYS_ASSERTOBJ(pFile);}
		virtual void put(value_type const* data, size_type sz)
		{
			GSYS_ASSERTOBJ(m_pFile);
			m_pFile->write(data,sz);
		}
		virtual void get(value_type * data, size_type sz)
		{
			GSYS_ASSERTOBJ(m_pFile);
			m_pFile->read(data,sz);
		}
		virtual value_type const* get_ref(size_type sz)
		{
			GSYS_ASSERTOBJ(m_pFile);
			m_Buffer.reset(sz);
			m_pFile->read(m_Buffer.get(),sz);
			return m_Buffer.get();
		}
	};

	class gfile_binistream : public binistream
	{
		gfile_biniostream_buffer buffer;
	public:
		gfile_binistream():binistream(&buffer){}
		gfile_binistream(gfile* pFile):buffer(pFile),binistream(&buffer){}
		void setup(gfile* pFile){buffer.m_pFile = pFile;}
	};

	class gfile_binostream : public binostream
	{
		gfile_biniostream_buffer buffer;
	public:
		gfile_binostream():binostream(&buffer){}
		gfile_binostream(gfile* pFile):buffer(pFile),binostream(&buffer){}
		void setup(gfile* pFile){buffer.m_pFile = pFile;}
	};
};
#endif