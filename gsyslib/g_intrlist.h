#pragma once

#include "g_gsys.h"

namespace gsys
{
	template<typename T> class gintrlist;
	template<typename T> class frwd_gintrlist;

	class gintrlist_base : noncopyable
	{
		template<typename T> friend class gintrlist;
		template<typename T> friend class frwd_gintrlist;
		gintrlist_base* m_pPrev;
		gintrlist_base* m_pNext;
		void disconnect()
		{GSYS_CFN;
			if(m_pPrev)m_pPrev->m_pNext = m_pNext;
			if(m_pNext)m_pNext->m_pPrev = m_pPrev;
			m_pPrev=gsys_null;
			m_pNext=gsys_null;
		}
		void insert_after(gintrlist_base* ref)
		{GSYS_CFN;
			GSYS_ASSERTOBJ(ref);
			disconnect();
			m_pPrev = ref;
			m_pNext = ref->m_pNext;
			ref->m_pNext = this;
			GSYS_ASSERTOBJ(m_pNext);
			m_pNext->m_pPrev = this;
		}
		void insert_before(gintrlist_base* ref)
		{GSYS_CFN;
			disconnect();
			m_pNext = ref;
			m_pPrev= ref->m_pPrev;
			ref->m_pPrev = this;
			GSYS_ASSERTOBJ(m_pPrev);
			m_pPrev->m_pNext = this;
		}
	public:
		gintrlist_base()
		{GSYS_CFN;
			m_pPrev = m_pNext = gsys_null;
		}
		~gintrlist_base()
		{GSYS_CFN;
			disconnect();
		}
	};
/*
	template<typename T>
	class frwd_gintrlist : noncopyable
	{
	public:

	private:
		static inline T* next(T*ref)
		{
			if(ref)
				if(ref->m_pNext)
					return (T*)(ref->m_pNext);
			return gsys_null;
		}
	private:
		T* m_pBegin;
	public:
		class iterator
		{
			friend class frwd_gintrlist;
			T* ref;
			iterator(T * ptr)
			{GSYS_CFN;
				ref = ptr;
			}
			void next()
			{GSYS_CFN;
				ref = frwd_gintrlist::next(ref);
			}
		public:
			iterator(iterator const& it):ref(it.ref){}
			iterator& operator =(iterator const& it)
			{GSYS_CFN;ref = it.ref;	return *this;}
			T* get(){return ref;} 
			iterator& operator ++()
			{GSYS_CFN;
				next();
				return *this;
			}
			iterator operator++(int)
			{GSYS_CFN;
				iterator tmp=*this;
				next();
				return tmp;
			}
			T* operator ->(){GSYS_CFN;return ref;}
			T& operator *(){GSYS_CFN;return *ref;}
			operator == (iterator const& it){GSYS_CFN;return ref == it.ref;}
			operator != (iterator const& it){GSYS_CFN;return ref != it.ref;}
		};
	public:
		frwd_gintrlist():m_pBegin(0){}
		iterator begin(){GSYS_CFN;return iterator(m_pBegin);}
		iterator end(){GSYS_CFN;return iterator(0);}

		iterator push_front(T* ref)
		{GSYS_CFN;
			if(m_pBegin)m_pBegin->m_pPrev = ref;
			ref->m_pNext=m_pBegin;
			ref->m_pPrev=m_pBegin;
			m_pBegin = ref;
			return iterator(ref);
		}

		void erase(T* ptr)
		{
			static_cast<gintrlist_base*>(ptr)->disconnect();
		}
	};
*/
    template<typename T>
	class gintrlist : noncopyable
	{
		// start ref
		gintrlist_base m_Begin;
		static T* next(T* tref)
		{
			gintrlist_base* ref = (gintrlist_base*)tref;
			if(ref)
				if(ref->m_pNext)
					return (T*)(ref->m_pNext);
			return gsys_null;
		}		
		static const T* next(const T* tref)
		{
			gintrlist_base const* ref = (gintrlist_base const*)tref;
			if(ref)
				if(ref->m_pNext)
					return (T*)(ref->m_pNext);
			return gsys_null;
		}
		static T* prev(T* tref)
		{
			gintrlist_base* ref = (gintrlist_base*)tref;
			if(ref)
				return (T*)(ref->m_pPrev);
			return gsys_null;
		}
		static const T* prev(T const * tref)
		{
			gintrlist_base const* ref = (gintrlist_base const*)tref;
			if(ref)
				return (T*)(ref->m_pPrev);
			return gsys_null;
		}	public:

	public:
		class iterator
		{
			friend class gintrlist;
		public:
			typedef T* pointer;
			typedef T& reference;
			typedef forward_iterator_tag iterator_category;
			typedef T value_type; 
			typedef void difference_type;
		private:
			T* ref;
			iterator(T * ptr)
			{GSYS_CFN;
				ref = ptr;
			}
			void next()
			{GSYS_CFN;
				T* r1 = gintrlist::next(ref);
				if(r1) ref = r1;
			}
		public:
			iterator(iterator const& it):ref(it.ref){}
			iterator& operator =(iterator const& it)
			{GSYS_CFN;ref = it.ref;	return *this;}
			T* get(){return ref;} 
			iterator& operator ++()
			{GSYS_CFN;
				next();
				return *this;
			}
			iterator operator++(int)
			{GSYS_CFN;
				iterator tmp=*this;
				next();
				return tmp;
			}
			iterator& operator --()
			{GSYS_CFN;
				T* r1 = gintrlist::prev(ref);
				if(r1) ref = r1;
				return *this;
			}
			T* operator ->(){GSYS_CFN;return ref;}
			T& operator *(){GSYS_CFN;return *ref;}
			bool operator == (iterator const& it){GSYS_CFN;return ref == it.ref;}
			bool operator != (iterator const& it){GSYS_CFN;return ref != it.ref;}
		};
		class const_iterator
		{
			friend class gintrlist;
		public:
			typedef const T* pointer;
			typedef const T& reference;
			typedef forward_iterator_tag iterator_category;
			typedef T value_type; 
			typedef void difference_type;
		private:
			pointer ref;
			const_iterator(pointer ptr)
			{GSYS_CFN;
			ref = ptr;
			}
			void next()
			{GSYS_CFN;
			pointer r1 = gintrlist::next(ref);
			if(r1) ref = r1;
			}
		public:
			const_iterator(iterator const& it):ref(it.ref){}
			const_iterator& operator =(const_iterator const& it)
			{GSYS_CFN;ref = it.ref;	return *this;}
			pointer get(){return ref;} 
			const_iterator& operator ++()
			{GSYS_CFN;
			next();
			return *this;
			}
			const_iterator operator++(int)
			{GSYS_CFN;
			const_iterator tmp=*this;
			next();
			return tmp;
			}
			const_iterator& operator --()
			{GSYS_CFN;
			T* r1 = gintrlist::prev(ref);
			if(r1) ref = r1;
			return *this;
			}
			pointer operator ->(){GSYS_CFN;return ref;}
			reference operator *(){GSYS_CFN;return *ref;}
			bool operator == (const_iterator const& it){GSYS_CFN;return ref == it.ref;}
			bool operator != (const_iterator const& it){GSYS_CFN;return ref != it.ref;}
		};
	public:
		gintrlist()
		{GSYS_CFN;
			m_Begin.m_pNext = &m_Begin;
			m_Begin.m_pPrev = &m_Begin;
		}
		~gintrlist()
		{GSYS_CFN;
			clean();
		}

		T* front()
		{
			return (T*)m_Begin.m_pNext;
		}
		T* back()
		{
			return (T*)m_Begin.m_pPrev;
		}

		T* at(int p)
		{
			int i=0;
			_foreach(gintrlist<T>,*this,it)
			{
				if(i==p)return &(*it);
				i++;
			}
			return 0;
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
					((gintrlist_base*)obj)->disconnect();
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

		iterator erase(iterator it)
		{
			iterator next = it;
			if(next != end())
			next++;
			it->disconnect();
			return it;
		}

		iterator begin(){GSYS_CFN;return iterator((T*)m_Begin.m_pNext);}
		iterator end(){GSYS_CFN;return iterator((T*)&m_Begin);}

		const_iterator begin()const{GSYS_CFN;return const_iterator((T*)m_Begin.m_pNext);}
		const_iterator end()const{GSYS_CFN;return const_iterator((T*)&m_Begin);}

		iterator insert_after(iterator pos,T* tref)
		{GSYS_CFN;
			gintrlist_base* ref = (gintrlist_base*)tref;
			if(m_Begin.m_pNext == (T*)&m_Begin)
			{
				ref->insert_after(&m_Begin);
				return iterator(ref);
			}
			if(pos.ref == &m_End)return iterator((T*)&m_Begin);
			ref->insert_after(pos.ref);
			return iterator(ref);
		}
		iterator insert_before(iterator pos,T* tref)
		{GSYS_CFN;
			gintrlist_base* ref = (gintrlist_base*)tref;
			if(m_Begin.m_pNext == &m_Begin)
			{
				ref->insert_after(&m_Begin);
				return iterator(tref);
			}
			ref->insert_before((gintrlist_base*)pos.ref);
			return iterator(tref);
		}
		iterator push_back(T* ref){GSYS_CFN;return insert_before(end(),ref);}
		iterator push_front(T* ref){GSYS_CFN;return insert_before(begin(),ref);}
		iterator insert(iterator pos,T* ref){GSYS_CFN;return insert_after(pos,ref);}

		void join(gintrlist<T> & lst)
		{
			if(lst.empty())return;
			m_Begin.m_pPrev->m_pNext = lst.m_Begin.m_pNext; // join middle
			lst.m_Begin.m_pNext->m_pPrev = m_Begin.m_pPrev; // join middle

			m_Begin.m_pPrev = lst.m_Begin.m_pPrev; // setup last point
			m_Begin.m_pPrev->m_pNext = &m_Begin; // setup last point

			lst.m_Begin.m_pNext = &lst.m_Begin; // clear lst
			lst.m_Begin.m_pPrev = &lst.m_Begin; // clear lst
		}

		bool empty()const{return m_Begin.m_pNext == &m_Begin;}

		int size()const
		{
			int count = 0;
			for(gintrlist_base* i = m_Begin.m_pNext;i!=&m_Begin;i=i->m_pNext)count++;
			return count;
		}
		int count()const{return size();}
	};
}
