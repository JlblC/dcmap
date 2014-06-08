#pragma once

#include "g_gsys.h"
#include "g_data_buffer.h"
#include "g_indexed_list.h"

namespace gsys
{
	template <class T>
	class ext_heap
	{
	public:
		typedef T value_type;
		typedef ulong size_type;
	private:
		struct entry
		{
			size_type szBegin;
			size_type szEnd;
		};
		indexed_list<entry> m_lstEntries;
		size_type m_szHeapSize;
	public:
		
		void init(size_type szSize)
		{
			m_szHeapSize = szSize;
			reset();
		}

		size_type size(){return m_szHeapSize;}

		void reset()
		{
			m_lstEntries.clear();

			// add terminating entry
			entry ent = {m_szHeapSize,m_szHeapSize};
			m_lstEntries.push_back(ent);
		}

		void free(ID idBlock)
		{
			m_lstEntries.erase(idBlock);
		}

		size_type mem_pos(ID idMem)
		{
			return m_lstEntries[idMem].szBegin;
		}

		size_type mem_size(ID idMem)
		{
			return m_lstEntries[idMem].szBegin - m_lstEntries[idMem].szEnd;
		}

		ID alloc(size_type szSize)
		{
			// find first free block
			indexed_list<entry>::iterator it = m_lstEntries.begin();
			
			size_type szLastSize = 0;

			while(it != m_lstEntries.end())
			{
				size_type sz = it->szBegin - szLastSize;
				if(sz <= szSize) // add new element
				{
					entry ent = {szLastSize,szLastSize+sz};
					return m_lstEntries.insert(it,ent);
				}
				szLastSize = it->szEnd;
				it++;
			}	
			return InvalidID;
		}
	};
}