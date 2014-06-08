#pragma once

#include "g_gsys.h"

namespace gsys
{
	template<typename T> class gintrlist;
	class gintrlist_base : noncopyable
	{
		friend class gintrlist;
		gintrlist_base* m_pPrev;
		gintrlist_base* m_pNext;
		void disconnect()
		{_CFN;
			if(m_pPrev)m_pPrev->m_pNext = m_pNext;
			if(m_pNext)m_pNext->m_pPrev = m_pPrev;
			m_pPrev=null;
			m_pNext=null;
		}
		void insert_after(gintrlist_base* ref)
		{_CFN;
			_ASSERTOBJ(ref);
			disconnect();
			m_pPrev = ref;
			m_pNext = ref->m_pNext;
			ref->m_pNext = this;
			_ASSERTOBJ(m_pNext);
			m_pNext->m_pPrev = this;
		}
		void insert_before(gintrlist_base* ref)
		{_CFN;
			disconnect();
			m_pNext = ref;
			m_pPrev= ref->m_pPrev;
			ref->m_pPrev = this;
			_ASSERTOBJ(m_pPrev);
			m_pPrev->m_pNext = this;
		}
	public:
		gintrlist_base()
		{_CFN;
			m_pPrev = m_pNext = null;
		}
		virtual ~gintrlist_base()
		{_CFN;
			disconnect();
		}
	};

    template<typename T>
	class gintrlist : noncopyable
	{
		// start ref
		gintrlist_base m_Begin;
		static T* next(T*ref)
		{
			if(ref)
				if(ref->m_pNext)
					return (T*)(ref->m_pNext);
			return null;
		}
		static T* prev(T*ref)
		{
			if(ref)
				return (T*)(ref->m_pPrev);
			return null;
		}
	public:
		class iterator
		{
			friend class gintrlist;
			T* ref;
			iterator(T * ptr)
			{_CFN;
				ref = ptr;
			}
			void next()
			{_CFN;
				T* r1 = gintrlist::next(ref);
				if(r1) ref = r1;
			}
		public:
			iterator(iterator const& it)
			{
				ref = it.ref;
			}
			iterator& operator =(iterator const& it)
			{_CFN;
				ref = it.ref;
				return *this;
			}
			T* get(){return ref;} 
			iterator& operator ++()
			{_CFN;
				next();
				return *this;
			}
			iterator operator++(int)
			{_CFN;
				iterator tmp=*this;
				next();
				return tmp;
			}
			iterator& operator --()
			{_CFN;
				T* r1 = gintrlist::prev(ref);
				if(r1) ref = r1;
				return *this;
			}
			T* operator ->(){_CFN;return ref;}
			T& operator *(){_CFN;return *ref;}
			operator == (iterator const& it){_CFN;return ref == it.ref;}
			operator != (iterator const& it){_CFN;return ref != it.ref;}
		};
	public:
		gintrlist()
		{_CFN;
			m_Begin.m_pNext = &m_Begin;
			m_Begin.m_pPrev = &m_Begin;
		}
		~gintrlist()
		{_CFN;
			clean();
		}
		void clean()
		{
			// there is 2 ways - disconnect all refernced data from list
			// or destroy all data
			iterator it = begin();
			while(it != end())
			{
				T* obj = it.get();
				it++;
				#if 1
					obj->disconnect();
				#else
					delete obj;
				#endif
			}
		}

		// removes object from list
		void remove(T* ref)
		{
			ref->disconnect();
		}

		iterator begin(){_CFN;return iterator((T*)m_Begin.m_pNext);}
		iterator end(){_CFN;return iterator((T*)&m_Begin);}

		iterator insert_after(iterator pos,T* ref)
		{_CFN;
			if(m_Begin.m_pNext == (T*)&m_Begin)
			{
				ref->insert_after(&m_Begin);
				return iterator(ref);
			}
			if(pos.ref == &m_End)return iterator((T*)&m_Begin);
			ref->insert_after(pos.ref);
			return iterator(ref);
		}
		iterator insert_before(iterator pos,T* ref)
		{_CFN;
			if(m_Begin.m_pNext == &m_Begin)
			{
				ref->insert_after(&m_Begin);
				return iterator(ref);
			}
			ref->insert_before(pos.ref);
			return iterator(ref);
		}
		iterator push_back(T* ref){_CFN;return insert_before(end(),ref);}
		iterator push_front(T* ref){_CFN;return insert_before(begin(),ref);}
		iterator insert(iterator pos,T* ref){_CFN;return insert_after(pos,ref);}
	};
}
