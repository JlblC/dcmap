#pragma once
#include "g_gsys.h"

#include "g_file.h"
#include "g_data_buffer.h"

namespace gsys 
{
	class file_pipe
	{
	protected:
		typedef gsys_byte buffer_data_type;

		typedef raw_data_buffer<buffer_data_type,0,8> buffer_type;
		buffer_type m_buf;
		gfile::size_type m_BuffedRead;
		gfile::size_type m_BuffedWrite;
		gfile::size_type m_BufferLimit;
	public:
		typedef gsys_ulong size_type;
	public: // data
		gfile* in;
		gfile* out;

      //  bool m_bEnd;
		
	public:
		file_pipe()
		{
			free();
		}

	protected:

		gfile::size_type do_in(gfile::size_type from,gfile::size_type to)
		{
			if(from == to)return 0;
			return in->read(m_buf.get_at(from),sizeof(buffer_data_type),to-from);
		}
		gfile::size_type do_out(gfile::size_type from,gfile::size_type to)
		{
			if(from == to)return 0;
			return out->write(m_buf.get_at(from),sizeof(buffer_data_type),to-from);
		}

		bool do_pass()
		{
			gfile::size_type rin,rout;

			if(m_BufferLimit == 0) // case one
			{
				rin = do_in(m_BuffedRead,m_buf.size());
				m_BuffedRead += rin;

				if(m_BuffedRead == m_buf.size()) // try to cycle buffer
				{
					m_BufferLimit = m_buf.size();
					m_BuffedRead = 0;
					return true; // do second pass in case 2
				}    
				rout = do_out(m_BuffedWrite,m_BuffedRead);
				m_BuffedWrite += rout;

				return false;
			}
			else // buffer recycled
			{
				rin = do_in(m_BuffedRead,m_BuffedWrite);
				m_BuffedRead+= rin;

                rout = do_out(m_BuffedWrite,m_BufferLimit);
				m_BuffedWrite += rout;
				
                if(m_BuffedWrite == m_BufferLimit)
				{
					m_BuffedWrite = 0;
					m_BufferLimit = 0;
					return true; 
				}

				return false;
			}

			return true;
		}
	public:

		bool init(gfile* l_in,gfile* l_out,gfile::size_type buffer_size = (1024*4))
		{
			if(!(l_in && l_out))return false;
			if(!(*l_in && *l_out))return false;

			gfile::size_type sz = l_in->size();
			if(sz) buffer_size = (sz < buffer_size ? sz : buffer_size);

			m_buf.reset(buffer_size);
			
			in = l_in;
			out = l_out;

			return true;
		}

		void free()
		{
			in = 0;
			out = 0;
			m_BuffedRead=0;
			m_BuffedWrite=0;
			m_BufferLimit=0;	
		};

		bool pass()
		{
			GSYS_ASSERT(in);
			GSYS_ASSERT(out);
			GSYS_ASSERT(*in);
			GSYS_ASSERT(*out);

			if(in->eof())
			{
				
				return false;
			}
			while(do_pass())
			{
			}
			return true;
			return false;
		}

		bool execute()
		{
			GSYS_ASSERT(in);
			GSYS_ASSERT(out);
			GSYS_ASSERT(*in);
			GSYS_ASSERT(*out);

			while(!in->eof())
			{
				do_pass();
			}
			return true;
		}
	};
}