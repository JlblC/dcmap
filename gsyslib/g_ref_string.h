#pragma once

#include "g_gsys.h"
#include "g_staticstring.h"
#include <string>

namespace gsys
{
	class ref_string
	{
	private:
		char* m_ptrData;
		bool  m_bOwnHandle;
	public:
		ref_string():
			m_ptrData(0),
			m_bOwnHandle(false)
		{
		}
		~ref_string()
		{
			reset();
		}
		ref_string(char const* pt)
		{
			m_ptrData = const_cast<char*>(pt);
			m_bOwnHandle = false;
		}
		ref_string(ref_string const& ref)
		{
			if(ref.m_bOwnHandle)make_copy(ref.m_ptrData);
			else 
			{
				m_ptrData =ref.m_ptrData;
				m_bOwnHandle = false;
			}
		}

		ref_string& make_copy(char const* pt)
		{
			reset();
			size_t len = strlen(pt);
			m_ptrData = new char[len+1];
			strcpy(m_ptrData,pt);
			return *this;
		}

		void reset()
		{
			if(m_bOwnHandle)
				delete[] m_ptrData;
			m_ptrData = 0;
		}
		const char* get(){return m_ptrData;}
		const char* c_str(){return m_ptrData;}

		bool operator < (ref_string const& ref)const
			{return strcmp(m_ptrData,ref.m_ptrData) < 0;}
		bool operator > (ref_string const& ref)const
			{return strcmp(m_ptrData,ref.m_ptrData) > 0;}
		bool operator == (ref_string const& ref)const
			{return strcmp(m_ptrData,ref.m_ptrData) == 0;}
	};
}
