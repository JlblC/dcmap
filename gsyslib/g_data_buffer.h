#pragma once
#ifndef G_DATA_BUFFER_H
#define G_DATA_BUFFER_H
#include "g_gsys.h"
#include <memory.h>

namespace gsys
{
	class raw_buffer_allocator : public noncopyable
	{
		gsys_byte* m_pBuffer;
	public:
		raw_buffer_allocator():m_pBuffer(0){}
		~raw_buffer_allocator()
		{
			if(m_pBuffer)gfree(m_pBuffer);
		}
		gsys_byte* alloc(uint size)
		{
			m_pBuffer = (gsys_byte*)galloc(size);
			return m_pBuffer;
		}
		gsys_byte* realloc(uint size)
		{
			if(m_pBuffer)m_pBuffer = (gsys_byte*)grealloc(m_pBuffer,size);
			else m_pBuffer = (gsys_byte*)galloc(size);
			return m_pBuffer;
		}
		void free()
		{
			if(m_pBuffer)gfree(m_pBuffer);
			m_pBuffer=0;
		}
		gsys_byte* get(){return m_pBuffer;}
		gsys_byte const* get()const{return m_pBuffer;}
	};

	template<class T, gsys_ulong start_size=256,gsys_ulong grow_size=256>
	class raw_data_buffer
	{
		GSYS_STATIC_ASSERT(grow_size > 0);

		typedef raw_data_buffer<T,start_size,grow_size> this_type;
		
	public:
		typedef T value_type;
		typedef gsys::gsys_ulong size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS

		typedef pointer iterator;
		typedef const_pointer const_iterator;
	protected:
		// data storage
		raw_buffer_allocator buf;
		size_type m_szSize;
		size_type m_szCapacity;
	private:
		
		void _alloc()
		{
			if(m_szCapacity)buf.alloc(sizeof(value_type)*m_szCapacity);
			else buf.free();
		}
		void _free()
		{
			buf.free();
		}

		void _expand(size_type szNewSize)
		{
			m_szCapacity = szNewSize;
			if(grow_size)
			{
				m_szCapacity += grow_size/2;
				m_szCapacity += m_szCapacity%grow_size;
			}
		}
	public:
		// copy constructor
		raw_data_buffer(this_type const& rbuf):m_szCapacity(0),m_szSize(0){operator=(rbuf);}

		this_type& operator = (this_type const& rbuf)
		{
			if(rbuf.m_szSize)
			{
				reset(rbuf.m_szSize);
				rbuf.raw_copy_to(pointer(buf.get()));
			}
			else
			{
				reset(0);
			}
			return *this;
		}
		raw_data_buffer():
			m_szCapacity(start_size),
			m_szSize(0)
		{
			_alloc();
		}
		raw_data_buffer(size_type inital_capacity):
			m_szCapacity(inital_capacity),
			m_szSize(0)
		{
			_alloc();
		}
		~raw_data_buffer()
		{
			_free();
		}

		iterator begin(){return pointer(buf.get());}
		const_iterator begin()const{return const_pointer(buf.get());}
		iterator end(){return pointer(buf.get())+m_szSize;}
		const_iterator end()const{return const_pointer(buf.get())+m_szSize;}

		void reset(size_type szNewSize=0)
		{
			if(szNewSize > m_szCapacity)// resize array
			{
				_expand(szNewSize);
				_free();
				_alloc();
			}
			m_szSize = szNewSize;
		}
		void reserve(size_type szNewSize)
		{
			if(szNewSize > m_szCapacity)// resize array
			{
				_expand(szNewSize);
				buf.realloc(sizeof(value_type)*m_szCapacity);
			}
		}

		template<class InputIterator>
		void init(InputIterator First,InputIterator Last)
		{
			reset(0);
			while(First != Last)
			{
				append(&(*First));
                ++First;			
			}
		}
		template<class InputIterator>
		void init(InputIterator First,InputIterator Last,size_type szSize)
		{
			reset(szSize);
			value_type* pt = pointer(buf.get());
			for(int i=0;i<szSize;i++)
			{
				if(First == Last) break;
				::memcpy(pt,&(*First),sizeof(value_type));
				pt++;
                ++First;			
			}
		}

		void clear()
		{
			reset(0);
			//_free();
			//m_szSize = 0;
			//m_szCapacity = 0;
		}

		void resize(size_type szNewSize)
		{
			if(szNewSize > m_szCapacity)// resize array
			{
				_expand(szNewSize);
				buf.realloc(sizeof(value_type)*m_szCapacity);
			}
			m_szSize = szNewSize;
		}

		void resize(size_type szNewSize,value_type const& val)
		{
			if(szNewSize > m_szCapacity)// resize array
			{
				_expand(szNewSize);
				buf.realloc(sizeof(value_type)*m_szCapacity);
			}
			for(size_type i=m_szSize;i<szNewSize;i++)
			{
				(pointer(buf.get()))[i] = val;
			}
			m_szSize = szNewSize;
		}

		reference grow(size_type szAddSize=1)
		{
			size_type sz = m_szSize;
			resize(m_szSize + szAddSize);
			return pointer(buf.get())[sz];
		}

		void append(value_type const* data,size_type szSize=1)
		{
			size_type szNewSize = m_szSize + szSize;
			size_type szCurrSize = m_szSize;
			resize(szNewSize); // alloc additional space
			::memcpy(pointer(buf.get())+szCurrSize,data,szSize*sizeof(value_type));
		}

		void push_back(value_type const& data)
		{
			size_type szNewSize = m_szSize + 1;
			size_type szCurrSize = m_szSize;
			resize(szNewSize); // alloc additional space
			pointer(buf.get())[szCurrSize] = data;
		}
		void push_back(value_type const* data,size_type count)
		{
			append(data,count)
		}
		void push_back()
		{
			resize(m_szSize + 1);
		}
		void pop_back()
		{
			if(m_szSize)m_szSize--;
		}

		pointer get_at(size_type pos){return pointer(buf.get())+pos;}
		const_pointer get_at(size_type pos)const{return const_pointer(buf.get())+pos;}

		pointer get(){return pointer(buf.get());}
		const_pointer get()const{return const_pointer(buf.get());}

		reference front(){return pointer(buf.get())[0];}
		const_reference front()const{return const_pointer(buf.get())[0];}

		reference back(){return pointer(buf.get())[m_szSize-1];}
		const_reference back()const{return const_pointer(buf.get())[m_szSize-1];}

		reference operator[](size_type szPos){return *get_at(szPos);}
		const_reference operator[](size_type szPos)const{return *get_at(szPos);}

		size_type size()const{return m_szSize;}
		size_type raw_size()const{return m_szSize*sizeof(value_type);}

		bool empty()const{return m_szSize==0;}
		// oparations
		void raw_copy_to(value_type* pt)const{::memcpy(pt,buf.get(),raw_size());}
		void raw_copy_from(value_type const* pt,size_type sz){::memcpy(buf.get(),pt,sz*sizeof(value_type));}

		void raw_copy_to(value_type* dst,size_type pos,size_type size)const
		{
			::memcpy(dst,pointer(buf.get())+pos,size*sizeof(value_type));
		}
	};

	typedef raw_data_buffer<gsys_byte> byte_buffer;
	typedef raw_data_buffer<char> char_buffer;
}

#endif