#pragma once
#ifndef G_GVECTOR_H
#define G_GVECTOR_H
#include "g_gsys.h"
#include "detail/g_iterators.h"

/*
 *	Simple template class for vector analog to STL
 *  Currently provide only minimal functonality
 */

namespace gsys
{
	template<typename T>
	class gvector
	{
	public:
		typedef gvector<T> this_type ;
		typedef T value_type;
		typedef gsys_size_t size_type;
		typedef gsys_pt_diff difference_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
	private:
		static const size_type m_cInitalSize=4;
		static const size_type m_cGrowSize=4;
	private:
		pointer m_pBegin;
		pointer m_pEnd;
		pointer m_pBufferEnd;
	public: // iterators
		typedef detail::ptr_iterator<T> iterator;
		typedef detail::const_ptr_iterator<T> const_iterator;
	private:
		void _new(pointer pt){new ((void*)pt) T();}
		void _new(pointer pt,const_reference _val){new ((void*)pt) T(_val);}
		void _delete(pointer pt){(pt)->~T();}
		void _new(pointer pt,size_type num){for(pointer p=pt;p<pt+num;p++)_new(p);}
		void _new(pointer pt,size_type num,const_reference _val){for(pointer p=pt;p<pt+num;p++)_new(p,_val);}
		void _delete(pointer pt,size_type num)
		{
			for(pointer p=pt;p<pt+num;p++)
				_delete(p);
		}
		void _delete(){_delete(m_pBegin,size());}

		virtual pointer _galloc(size_type sz){return galloc_raw_objects<T>(sz);}
		virtual void _gfree(pointer p){return gfree(p);}

		void _alloc(size_type new_sz)
		{
			m_pBegin=_galloc(new_sz);
			m_pEnd=m_pBegin;
			m_pBufferEnd=m_pBegin+new_sz;
		}

		void _realloc(size_type new_sz)
		{
			pointer pNewBegin = _galloc(new_sz);
			size_type sz = size();
			for(pointer d=pNewBegin,s=m_pBegin;s<m_pEnd;d++,s++)
			{
				new ((void*)d) T(*s);
				(s)->~T();
			}
			_gfree(m_pBegin);
			m_pBegin = pNewBegin;
			m_pEnd = pNewBegin+sz;
			m_pBufferEnd = pNewBegin+new_sz;
		}

		size_type calc_new_capacity(size_type current,size_type add,size_type initial, size_type grow)
		{
			return current+((add)/grow+1)*grow;
		}

		void _grow(size_type sz)
		{
			if(!m_pBegin) // create base 
			{
				_alloc(sz);
			}
			if( (gsys_pt_diff)sz > m_pBufferEnd-m_pEnd)
			{
				_realloc(calc_new_capacity(capacity(),sz,m_cInitalSize,m_cGrowSize));
			}
			m_pEnd+=sz;
		}	
	public:
		gvector():m_pBegin(gsys_null),m_pEnd(gsys_null),m_pBufferEnd(gsys_null){}
		virtual ~gvector()
		{
			clear();
		}
		this_type& operator = (this_type const& vt)
		{
			clear();
			size_type v_sz = vt.size();
			if(v_sz)
			{
				m_pBegin = _galloc(v_sz);
				m_pEnd = m_pBegin+v_sz;
				m_pBufferEnd = m_pEnd;
				for(size_type i=0;i<v_sz;i++)_new(m_pBegin+i,vt[i]);
			}
			return *this;
		}
		gvector(this_type const& vt):m_pBegin(gsys_null),m_pEnd(gsys_null),m_pBufferEnd(gsys_null)
		{
			size_type v_sz = vt.size();
			if(v_sz)
			{
				m_pBegin = _galloc(v_sz);
				m_pEnd = m_pBegin+v_sz;
				m_pBufferEnd = m_pEnd;
				for(size_type i=0;i<v_sz;i++)_new(m_pBegin+i,vt[i]);
			}
		}

	public:
		reference operator[](size_type n){return m_pBegin[n];}
		const_reference operator[](size_type n)const{return m_pBegin[n];}

		size_type size()const{return m_pEnd-m_pBegin;}
		size_type capacity( ) const{return m_pBufferEnd-m_pBegin;}

		reference back( ){return *(m_pEnd-1);}
		const_reference back( )const{return *(m_pEnd-1);}

		reference front( ){return *m_pBegin;}
		const_reference front( )const{return *m_pBegin;}

		iterator begin(){return iterator(m_pBegin);}
		const_iterator begin()const{return const_iterator(m_pBegin);}

		iterator end(){return iterator(m_pEnd);}
		const_iterator end()const{return const_iterator(m_pEnd);}

		void clear()
		{
			// call destructors
			if(m_pBegin)
			{
				_delete();
				_gfree(m_pBegin);
				m_pBegin=gsys_null;
				m_pEnd=gsys_null;
				m_pBufferEnd=gsys_null;
			}
		}

		void resize(size_type _Newsize)
		{
			if(_Newsize > size())
			{
				size_type sz = size();
				size_type num = _Newsize - size();
				_grow(num);
				_new(m_pBegin+sz,num);
			}
			else if(_Newsize == 0)clear();
			else if(_Newsize < size())
			{
				size_type dif = size() - _Newsize;
				pointer pt = (m_pEnd - dif);
				_delete(pt,dif);
				m_pEnd = m_pBegin+_Newsize;
			}
		}

		void resize(size_type _Newsize,const_reference _Val)
		{
			if(_Newsize > size())
			{
				size_type sz = size();
				size_type num = _Newsize - sz;
				_grow(num);
				_new(m_pBegin+sz,num,_Val);
			}
			else if(_Newsize == 0)clear();
			else if(_Newsize < size())
			{
				size_type dif = size() - _Newsize;
				pointer pt = (m_pEnd - dif);
				_delete(pt,dif);
				m_pEnd = m_pBegin+_Newsize;
			}		
		}

		iterator erase(iterator _Where)
		{
			// delete value
			pointer pt = _Where.m_ptr;
			// move array
			for(pointer p=pt;p<(m_pEnd-1);p++)
			{
				_delete(p);
				_new(p,*(p+1));
			}
			_delete(m_pEnd-1);
			m_pEnd-=1;
			return iterator(pt);
		}

		void push_back(value_type const& val)
		{
			resize(size()+1,val);
		}
	};
}


#endif



