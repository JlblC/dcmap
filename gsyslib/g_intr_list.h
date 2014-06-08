#pragma once

#include "g_gsys.h"
#include "g_text.h"

namespace gsys
{
	//class intr_list_entry;
	template<class _Value, class _EntryType> class intr_list;

	template<typename T>
	class intr_list_entry : noncopyable
	{
		friend class intr_list<T>;
	private:
		intr_list_entry<T>* m_pNext;
		intr_list_entry<T>* m_pPrev;

		void in_unlink()
		{
			if(m_pNext) m_pNext->m_pPrev = m_pPrev;
			if(m_pPrev) m_pPrev->m_pNext = m_pNext;
			//m_pNext = m_pPrev = gsys_null;
		}
		void unlink()
		{
			if(m_pNext) m_pNext->m_pPrev = m_pPrev;
			if(m_pPrev) m_pPrev->m_pNext = m_pNext;
			m_pNext = m_pPrev = gsys_null;
		}
	public:
		intr_list_entry():m_pNext(gsys_null),m_pPrev(gsys_null){}
		~intr_list_entry()
		{
			if(m_pNext) m_pNext->m_pPrev = m_pPrev;
			if(m_pPrev) m_pPrev->m_pNext = m_pNext;
		}
	};


	template<class _Value, class _EntryType = intr_list_entry<_Value> > 
	class intr_list : noncopyable
	{
	public:
		typedef _EntryType entry_type;
		typedef _Value value_type;
		typedef gsys_uint size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
	private:
		entry_type m_Ent;
	private:
		static entry_type* ent_next(entry_type const* ent){return ent->m_pNext;}
		static entry_type* ent_prev(entry_type const* ent){return ent->m_pPrev;}
		static void ent_unlink(entry_type* ent){ent->in_unlink();}

	public: // iterators
		class iterator
		{
			friend class intr_list;
			entry_type* m_pEntry;
			iterator(entry_type* ent):m_pEntry(ent){}
		public:
			iterator():m_pEntry(gsys_null){}

			iterator& operator ++()
			{GSYS_CFN;GSYS_ASSERTOBJ(m_pEntry);
				m_pEntry = gsys::intr_list<value_type,entry_type>::ent_next(m_pEntry);
				return *this;
			}

			iterator& operator --()
			{GSYS_CFN;GSYS_ASSERTOBJ(m_pEntry);
				m_pEntry = gsys::intr_list<value_type,entry_type>::ent_prev(m_pEntry);
				return *this;
			}

			reference operator*(){return *static_cast<value_type*>(m_pEntry);}
			pointer operator->(){return static_cast<value_type*>(m_pEntry);}

			bool operator == (iterator it)const{return m_pEntry == it.m_pEntry;}
			bool operator != (iterator it)const{return m_pEntry != it.m_pEntry;}
		};
		class const_iterator
		{
			friend class intr_list;
			entry_type const* m_pEntry;
			const_iterator(entry_type const* ent):m_pEntry(ent){}
		public:
			const_iterator():m_pEntry(gsys_null){}

			const_iterator& operator ++()
			{GSYS_CFN;GSYS_ASSERTOBJ(m_pEntry);
				m_pEntry = gsys::intr_list<value_type,entry_type>::ent_next(m_pEntry);
				return *this;
			}
			const_iterator& operator --()
			{GSYS_CFN;GSYS_ASSERTOBJ(m_pEntry);
				m_pEntry = gsys::intr_list<value_type,entry_type>::ent_prev(m_pEntry);
				return *this;
			}

			const_reference operator*(){return *static_cast<const_pointer>(m_pEntry);}
			const_pointer operator->(){return static_cast<const_pointer>(m_pEntry);}

			bool operator == (const_iterator it)const{return m_pEntry == it.m_pEntry;}
			bool operator != (const_iterator it)const{return m_pEntry != it.m_pEntry;}
		};
		//typedef iterator reverse_itertor;
		//typedef const_iterator const_reverse_itertor;

	public:
		intr_list()
		{
			m_Ent.m_pNext = &m_Ent;
			m_Ent.m_pPrev = &m_Ent;
		}
		size_type size()const
		{// count size
			size_type sz = 0;
			entry_type* end = &m_Ent;
			entry_type* it = m_Ent.m_pNext;
			while(it != end)
			{
				it = it->m_pNext;
				sz++;
			}
			return sz;
		}
		void clear()
		{
			entry_type* end = &m_Ent;
			entry_type* it = m_Ent.m_pNext;
			while(it != end)
			{
				entry_type* itn = it->m_pNext;
				it->unlink(); // unlink or destroy entry
				it = itn;
			}			
		}

		iterator begin(){return iterator(m_Ent.m_pNext);}
		const_iterator begin()const{return const_iterator(m_Ent.m_pNext);}

		iterator end(){return iterator(&m_Ent);}
		const_iterator end()const{return const_iterator(&m_Ent);}

		reference front(){GSYS_CFN;return *static_cast<pointer>(m_Ent.m_pNext);}
		const_reference front()const{GSYS_CFN;return *static_cast<const_pointer>(m_Ent.m_pNext);}

		reference back(){GSYS_CFN;return *static_cast<pointer>(m_Ent.m_pPrev);}
		const_reference back()const{GSYS_CFN;return *static_cast<const_pointer>(m_Ent.m_pPrev);}

		void push_back(value_type* pt)
		{
			ent_unlink(pt);
			pt->m_pNext = &m_Ent;
			pt->m_pPrev = m_Ent.m_pPrev;
			m_Ent.m_pPrev->m_pNext = pt;
			m_Ent.m_pPrev = pt;
		}

		void push_front(value_type* pt)
		{
			ent_unlink(pt);
			pt->m_pPrev = &m_Ent;
			pt->m_pNext = m_Ent.m_pNext;
			m_Ent.m_pNext->m_pPrev = pt;
			m_Ent.m_pNext = pt;
		}

		iterator erase(iterator _Where)
		{
			iterator it(_Where.m_pEntry->m_pNext);
			delete _Where.m_pEntry;
			return it;
		}
	};
	GSYSDETAIL_STREAM_CONTAINER(intr_list)

}