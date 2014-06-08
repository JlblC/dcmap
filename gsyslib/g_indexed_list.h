#pragma once

#include "g_gsys.h"
#include "g_data_buffer.h"
#include "g_text.h"
#include "g_alg.h"
#include <limits>
#include <vector>

namespace gsys
{
/*
 *	  Dynamic list with fast add & remove elements witch access by id and
 *    id structure restore
 */
	template<typename T,
		typename TSZ=gsys_ulong,
		typename TAG = gsys_char,
		TAG inv_tag = -1,
		TAG def_tag = 0>
	class indexed_list
	{
	public:
		typedef T value_type;
		typedef TSZ size_type;
		typedef TAG tag_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
	public:
		static size_type  invalid_id(){return std::numeric_limits<size_type>::max();}
		static tag_type   invalid_tag(){return inv_tag;}
		static tag_type   default_tag(){return def_tag;}
		static size_type  min_id(){return 0;}
		static size_type  max_id(){return min(0xffffff,std::numeric_limits<size_type>::max()-1);}
	private:
		struct entry
		{
			size_type   next;
			size_type   prev;
			tag_type    tag;
			value_type  data;
		};
	private:
		raw_data_buffer<entry> m_rawData;
		// data storage
		size_type m_szStart;
		size_type m_szEnd;
		size_type m_szFree;
		size_type m_szSize;
	private:
		size_type alloc_pos()
		{
			if(m_szFree	== invalid_id())
			{
				m_rawData.grow(1);
				return (size_type)m_rawData.size()-1;
			}
			else
			{
				size_type sz = m_szFree;
                m_szFree = m_rawData[sz].next;
				if(m_szFree	!= invalid_id())
				{
					m_rawData[m_szFree].prev = invalid_id();
				}
				return sz;
			}
		}

		size_type alloc_pos(size_type where)
		{
			GSYS_ASSERT(where < max_id());
			size_type sz = (size_type)m_rawData.size();
			if(sz < where) // grow array
			{
				m_rawData.resize(where+1);
				// fill free entries 
				for(size_type i=sz;i<where;i++)
				{
					m_rawData[i].prev = i-1;
					m_rawData[i].next = i+1;
					m_rawData[i].tag = invalid_tag();
				}
				m_rawData[sz].prev = invalid_id();
				m_rawData[where-1].next = m_szFree;
				if(m_szFree != invalid_id())
				{
					m_rawData[m_szFree].prev = where-1;
				}
				m_szFree = sz;
				m_rawData[where].tag = invalid_tag();
				return where;
			}
			else if(sz == where)
			{
				m_rawData.grow(1);
				m_rawData.back().tag = invalid_tag();
				return where;
			}				 
			else
			{
				GSYS_ASSERT(m_rawData[where].tag == invalid_tag());
				if(m_rawData[where].prev == invalid_id()) m_szFree = m_rawData[where].next;
				else
				{
					m_rawData[m_rawData[where].prev].next = m_rawData[where].next;
				}
				if(m_rawData[where].next != invalid_id())
					m_rawData[m_rawData[where].next].prev = m_rawData[where].prev;
				return where;
			}
		}

		void free(size_type szEntry)
		{
			GSYS_ASSERTREF(m_rawData[szEntry]);
			m_rawData[szEntry].tag = invalid_tag();
			m_rawData[szEntry].next = m_szFree;
			if(m_szFree != invalid_id()) m_rawData[m_szFree].prev = szEntry;
            m_rawData[szEntry].data.~T();
			m_rawData[szEntry].prev = invalid_id();
			m_szFree = szEntry;
			m_szSize--;
		}

		pointer __restore_element(size_type id)
		{
			size_type szNewPos = alloc_pos(id); // allocate position
			GSYS_ASSERT(m_rawData[szNewPos].tag == invalid_tag());
			m_rawData[szNewPos].prev =  m_szEnd;
			m_rawData[szNewPos].next =  (size_type)invalid_id();
			m_rawData[szNewPos].tag  =  default_tag();
			if(m_szEnd == invalid_id()) m_szStart = szNewPos;
			else m_rawData[m_szEnd].next = szNewPos;
			m_szEnd = szNewPos;
			m_szSize++;
			return &(m_rawData[szNewPos].data);
		}

	public: // iterators

		class iterator
		{
			friend class indexed_list;
			friend class const_iterator;
			entry* m_pt;
			size_type m_ind;
			iterator(entry* pt,size_type ind):m_pt(pt),m_ind(ind){}

            static tag_type  invalid_tag(){return inv_tag;}
			static size_type  invalid_id(){return std::numeric_limits<TSZ>::max();}
		public:

			typedef std::bidirectional_iterator_tag iterator_category;
			typedef T value_type;
			typedef size_t difference_type;
			typedef T* pointer;
			typedef T& reference;


			iterator():m_ind(invalid_id()),m_pt(0){}
			iterator(iterator const& it):m_pt(it.m_pt),	m_ind(it.m_ind){}

			operator size_type (){return m_ind;}
			//operator ID(){return ID(m_ind);}

			iterator& operator =(iterator const& it)
			{
				m_pt = it.m_pt;
				m_ind = it.m_ind;
				return *this;
			}

			reference operator*(){GSYS_CFN;GSYS_ASSERT(m_ind != invalid_id());return m_pt[m_ind].data;}
			pointer operator->(){GSYS_CFN;GSYS_ASSERT(m_ind != invalid_id());return &m_pt[m_ind].data;}

			bool operator == (iterator const& it){return m_ind == it.m_ind;}
			bool operator != (iterator const& it){return m_ind != it.m_ind;}

			iterator& operator++()
			{GSYS_CFN;
				GSYS_ASSERT(m_ind != invalid_id());
				m_ind = m_pt[m_ind].next;
				return *this;
			}
			iterator& operator--()
			{GSYS_CFN;
				GSYS_ASSERT(m_ind != invalid_id());
				m_ind = m_pt[m_ind].prev;
				return *this;
			}
		};
		class const_iterator
		{
			friend class indexed_list;
			friend class iterator;
			entry const* m_pt;
			size_type m_ind;
			const_iterator(entry const* pt,size_type ind):m_pt(pt),m_ind(ind){}

        	static tag_type  invalid_tag(){return inv_tag;}
			static size_type  invalid_id(){return std::numeric_limits<TSZ>::max();}

		public:
			const_iterator():m_ind(invalid_id()),m_pt(0){}
			const_iterator(const_iterator const& it):m_pt(it.m_pt),	m_ind(it.m_ind){}
			const_iterator(iterator const& it):m_pt(it.m_pt), m_ind(it.m_ind){}

			operator size_type (){return m_ind;}
			//operator ID (){return ID(m_ind);}

			const_iterator& operator =(const_iterator const& it)
			{
				m_pt = it.m_pt;
				m_ind = it.m_ind;
				return *this;
			}

			const_reference operator*()
			{GSYS_CFN;GSYS_ASSERT(m_ind != invalid_id());return m_pt[m_ind].data;}
			const_pointer operator->()
			{GSYS_CFN;GSYS_ASSERT(m_ind != invalid_id());return &m_pt[m_ind].data;}

			bool operator == (const_iterator const& it){return m_ind == it.m_ind;}
			bool operator != (const_iterator const& it){return m_ind != it.m_ind;}

			const_iterator& operator++()
			{GSYS_CFN;
				GSYS_ASSERT(m_ind != invalid_id());
				m_ind = m_pt[m_ind].next;
				return *this;
			}
			const_iterator& operator--()
			{GSYS_CFN;
				GSYS_ASSERT(m_ind != invalid_id());
				m_ind = m_pt[m_ind].prev;
				return *this;
			}
		};
	public:
		indexed_list():
			m_szStart(invalid_id()),
			m_szEnd(invalid_id()),
			m_szFree(invalid_id()),
			m_szSize(0)
		{
		}

		indexed_list(indexed_list<value_type> const& lst):
			m_szStart(invalid_id()),
			m_szEnd(invalid_id()),
			m_szFree(invalid_id()),
			m_szSize(0)	
		{
			_const_foreach(typename indexed_list<value_type>,lst,it)	push_back(*it);
		}

		template<class InputIterator>
		indexed_list(InputIterator First,InputIterator Last):
			m_szStart(invalid_id()),
			m_szEnd(invalid_id()),
			m_szFree(invalid_id()),
			m_szSize(0)	
		{
			while(First != Last)
			{
				push_back(*First);
                ++First;			
			}
		}

		~indexed_list()
		{
			// class destructors on all data
			size_type next = m_szStart;
			while(next != invalid_id())
			{
				m_rawData[next].data.~T();
				next = m_rawData[next].next;
			}
		}

		reference front(){return m_rawData[m_szStart].data;}
		const_reference front()const{return m_rawData[m_szStart].data;}

		reference back(){return m_rawData[m_szEnd].data;}
		const_reference back()const{return m_rawData[m_szEnd].data;}

		reference operator[](size_type sz){GSYS_ASSERT(sz < capacity() && sz >= min_id());return m_rawData[sz].data;}
		const_reference operator[](size_type sz)const{GSYS_ASSERT(sz < capacity() && sz >= min_id());return m_rawData[sz].data;}

		tag_type& tag(size_type sz){GSYS_ASSERT(sz < capacity() && sz >= min_id());return m_rawData[sz].tag;}
		tag_type  tag(size_type sz)const{GSYS_ASSERT(sz < capacity() && sz >= min_id());return m_rawData[sz].tag;}

		bool is_free(size_type sz)const
		{
			GSYS_ASSERT(sz >= min_id());
			if(sz >= capacity())return true;
			return m_rawData[sz].tag == invalid_tag();
		}

		indexed_list<T>& operator =( indexed_list<T>& lst)
		{
			clear();
			_const_foreach(typename indexed_list<value_type>,lst,it)	push_back(*it);
			return *this;
		}

		iterator begin(){return iterator(m_rawData.get(),m_szStart);}
		const_iterator begin()const{return const_iterator(m_rawData.get(),m_szStart);}

		iterator end(){return iterator(m_rawData.get(),invalid_id());}
		const_iterator end()const{return const_iterator(m_rawData.get(),invalid_id());}

		bool empty()const{return m_szSize==0;}
		size_type size()const{return m_szSize;}
		size_type capacity()const{return (size_type)m_rawData.size();}

		void clear()
		{
			// class destructors on all data
			size_type next = m_szStart;
			while(next != invalid_id())
			{
				m_rawData[next].data.~T();
				next = m_rawData[next].next;
			}
			m_szFree = invalid_id();
			m_szStart = invalid_id();
			m_szEnd = invalid_id();

			m_szSize = 0;
			m_rawData.reset(0);            
		}
		void reserve(size_type size)
		{
			size_type sz = (size_type)m_rawData.size();
			if(sz < size) // grow array
			{
				m_rawData.resize(size);
				// fill free entries 
				for(size_type i=sz;i<size;i++)
				{
					m_rawData[i].prev = i-1;
					m_rawData[i].next = i+1;
					m_rawData[i].tag = invalid_tag();
				}
				m_rawData[sz].prev = invalid_id();
				m_rawData[size-1].next = m_szFree;
				m_szFree = sz;
			}
		}

		iterator erase(iterator const& Where){return erase(Where.m_ind);}
		iterator erase(size_type szWhere)
		{
			if(szWhere >= m_rawData.size())return end();
			GSYS_ASSERT(szWhere>=0);
			GSYS_ASSERTREF(m_rawData[szWhere]);

			GSYS_ASSERT(m_rawData[szWhere].tag != invalid_tag());

			size_type prev = m_rawData[szWhere].prev;
			size_type next = m_rawData[szWhere].next;

			free(szWhere);

			if(prev != invalid_id())
				m_rawData[prev].next = next;
			else m_szStart = next;

			if(next != invalid_id())
			{
				m_rawData[next].prev = prev;
				return iterator(m_rawData.get(),next);
			}
			else
			{	
				m_szEnd = prev;
				return iterator(m_rawData.get(),invalid_id());
			}
		}

		void pop_back()
		{
			GSYS_ASSERT(m_szEnd != invalid_id());

			size_type prev = m_rawData[m_szEnd].prev;
			free(m_szEnd);

			if(prev != invalid_id())m_rawData[prev].next = invalid_id();
			else m_szStart = invalid_id();
			m_szEnd = prev;
		}

		void pop_front()
		{
			GSYS_ASSERT(m_szStart != invalid_id());

			size_type next = m_rawData[m_szStart].next;
			free(m_szStart);

			if(next != invalid_id())m_rawData[next].prev = invalid_id();
			else m_szEnd = invalid_id();
			m_szStart = next;
		}

		iterator insert(iterator const& Where, value_type const& Val)
		{
			size_type szNewPos = alloc_pos(); // allocate position
			m_rawData[szNewPos].tag =  default_tag();

			if(Where.m_ind == invalid_id()) // insert to end
			{
				if(m_szEnd != invalid_id())
					m_rawData[m_szEnd].next = szNewPos;

				m_rawData[szNewPos].prev =  m_szEnd;
				m_rawData[szNewPos].next =  invalid_id();
				m_szEnd = szNewPos;
			}
			else // insert before pos
			{
				m_rawData[szNewPos].prev =  m_rawData[Where.m_ind].prev;
				m_rawData[szNewPos].next =  Where.m_ind;
                m_rawData[Where.m_ind].prev = szNewPos;
			}

			if(m_rawData[szNewPos].prev == invalid_id()) // this it first element
					m_szStart = szNewPos;
			else m_rawData[m_rawData[szNewPos].prev].next = szNewPos;

			// allocate new 
            new ((void*)(&m_rawData[szNewPos].data)) value_type(Val);

			m_szSize++;
			return iterator(m_rawData.get(),szNewPos);
		}

		void push_back(value_type const& Val)
		{
			size_type szNewPos = alloc_pos(); // allocate position
			m_rawData[szNewPos].prev =  m_szEnd;
			m_rawData[szNewPos].next =  invalid_id();
			m_rawData[szNewPos].tag =  default_tag();

			if(m_szEnd == invalid_id()) m_szStart = szNewPos;
			else m_rawData[m_szEnd].next = szNewPos;

			m_szEnd = szNewPos;

            new ((void*)(&(m_rawData[szNewPos].data))) value_type(Val);
			m_szSize++;
		}

		void push_front(value_type const& Val)
		{
			size_type szNewPos = alloc_pos(); // allocate position
			m_rawData[szNewPos].prev =  invalid_id();
			m_rawData[szNewPos].next =  m_szStart;
			m_rawData[szNewPos].tag =  default_tag();

			if(m_szStart == invalid_id())m_szEnd = szNewPos;
			else m_rawData[m_szStart].prev = szNewPos;

			m_szStart = szNewPos;

            new ((void*)(&m_rawData[szNewPos].data)) value_type(Val);
			m_szSize++;
		}	

		size_type back_id()const{return m_szEnd;}
		size_type front_id()const{return m_szStart;}

		tag_type& back_tag(){GSYS_ASSERT(sz < capacity() && sz >= min_id());return m_rawData[m_szEnd].tag;}
		tag_type  back_tag()const{GSYS_ASSERT(sz < capacity() && sz >= min_id());return m_rawData[m_szEnd].tag;}

		tag_type& front_tag(){GSYS_ASSERT(sz < capacity() && sz >= min_id());return m_rawData[m_szStart].tag;}
		tag_type  front_tag()const{GSYS_ASSERT(sz < capacity() && sz >= min_id());return m_rawData[m_szStart].tag;}

		bool can_restore(size_type id)const
		{
			if(id > max_id())return false;
			return is_free(id);
		}

		pointer restore_element(size_type id)
		{
			pointer pt = __restore_element(id);
			if(!pt)return 0;
			new ((void*)(pt)) value_type();
			return pt;
		}

		void restore_element(size_type id,value_type const& Val)
		{
			pointer pt = __restore_element(id);
			if(!pt)return;
			new ((void*)(pt)) value_type(Val);
		}

		void order_swap(iterator i1,iterator i2)
		{
			GSYS_ASSERT(i1.m_pt == m_rawData.get());
			GSYS_ASSERT(i1.m_pt == i2.m_pt);

			order_swap((size_type)i1,(size_type)i2);
		}

		void order_swap(size_type e1,size_type e2)
		{
			entry& v1 = m_rawData[e1];
			entry& v2 = m_rawData[e2];

			GSYS_ASSERT(v1.tag != invalid_tag());
			GSYS_ASSERT(v2.tag != invalid_tag());

			if(m_szStart == e1)m_szStart = e2;
			else if(m_szStart == e2)m_szStart = e1;

			if(m_szEnd == e1)m_szEnd = e2;
			else if(m_szEnd == e2)m_szEnd = e1;

			if(v1.next == e2)
			{
				if(v1.prev != invalid_id())m_rawData[v1.prev].next = e2;
				if(v2.next != invalid_id())m_rawData[v2.next].prev = e1;

				v2.prev = v1.prev;
				v1.next = v2.next;

				v2.next = e1;
				v1.prev = e2;

			}
			else if(v2.next == e1)
			{
				if(v2.prev != invalid_id())m_rawData[v2.prev].next = e1;
				if(v1.next != invalid_id())m_rawData[v1.next].prev = e2;

				v1.prev = v2.prev;
				v2.next = v1.next;

				v1.next = e2;
				v2.prev = e1;
			}
			else
			{
				if(v1.prev != invalid_id())m_rawData[v1.prev].next = e2;
				if(v2.prev != invalid_id())m_rawData[v2.prev].next = e1;

				if(v1.next != invalid_id())m_rawData[v1.next].prev = e2;
				if(v2.next != invalid_id())m_rawData[v2.next].prev = e1;

				exchange(v1.prev,v2.prev);
				exchange(v1.next,v2.next);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		bool check_structure()
		{
			std::vector<bool> vec;
			vec.resize(capacity(),false);
			// check free

			size_type szfree=0;

			size_type pos = m_szFree;
			size_type pre = invalid_id();
			while(pos != invalid_id())
			{
				if(vec[pos]) 
					return false; // loop posible
				vec[pos] = true;
				if(m_rawData[pos].tag != invalid_tag()) 
					return false;

				if(pre != m_rawData[pos].prev)
					return false; // invalid back ref
				pre = pos;
				pos = m_rawData[pos].next;
				szfree++;
			}

			// check size
			size_type sz=0;

			pos = m_szStart;
			pre = invalid_id();
			while(pos != invalid_id())
			{
				if(pos >= capacity())return false;
				if(vec[pos]) return false; // loop posible
				vec[pos] = true;
				if(m_rawData[pos].tag == invalid_tag()) return false;
				if(pre != m_rawData[pos].prev)return false; // invalid back ref
				pre = pos;
				pos = m_rawData[pos].next;
				sz++;
			}
			if(m_szEnd != pre) return false;
			if(sz != m_szSize)return false;

			if(sz+szfree != m_rawData.size())return false;

			for(size_type i=0;i<capacity();i++) // check for full coverage
			{
				if(!vec[i])return false;
			}
			return true;
		}

	};
	// allow to save & load to stream as container
	GSYSDETAIL_STREAM_CONTAINER(indexed_list)

/*
	template<class T>
	binostream& operator<<(binostream& os,indexed_list<T> const& v)
	{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
		return os.put_rawdata(v);
	}

	template<class T>
	binistream& operator>>(binistream& is,indexed_list<T> & v)
	{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
		return is.get_rawdata(v);	
	}
*/

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



}