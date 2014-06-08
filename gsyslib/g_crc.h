#pragma once

#include "g_gsys.h"

namespace gsys 
{
	class crc32_counter
	{
		crc_t m_crc;
	public:
		crc32_counter()
		{
			clear();
		}
		crc_t crc(){return m_crc;}
		void clear(){m_crc = 0;}

		void count(void const* dta,ulong size)
		{
			gsys_byte const* ptr = (gsys_byte*)dta;
			for(uint i=0;i<size;i++)
			{
				m_crc+= *ptr;
				m_crc = m_crc << 1;
				ptr++;
			}
		}
		template<class T>
			crc32_counter & operator << (T const& dta){count(&dta,sizeof(dta));return *this;} 

	};
}
