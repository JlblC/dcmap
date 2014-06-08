#pragma once

#include "g_gsys.h"
#include "g_rect.h"
#include "g_point.h"
#include "g_assert.h"
#include "g_binstream.h"

namespace gsys
{
    /////////////////////////////////////////////////////////////////////////////////////// 
	template<class T> 
	class array2d
    {
	public: // typedefs
		typedef array2d<T> this_type;
		typedef T             value_type;
		typedef gsys_size_t   size_type;
		typedef gsys_size_t   difference_type;

		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
		// additional
        typedef T data_type; 
	public: // iterators	
		class iterator
		{
			friend class array2d;
			pointer m_ptr;
			pointer m_EndlPtr;
			size_type m_szLine;
			size_type m_szOfs;
#ifdef GSYS_ENABLE_ASSERT //-----------------------------------------------------------------------------
			pointer m_pStopPtr;
			iterator(pointer ptr,size_type line=0, size_type ofs=0,pointer stop=0):
					m_ptr(ptr),m_szLine(line),m_szOfs(ofs),
					m_EndlPtr(ptr + (line-ofs)),
					m_pStopPtr(stop) 
			{GSYS_CFN;if(!m_pStopPtr)m_pStopPtr=m_ptr;}
#else //----------------------------------------------------------------------------------------
			iterator(pointer ptr,size_type line=0, size_type ofs=0):
					m_ptr(ptr),m_szLine(line),m_szOfs(ofs),
					m_EndlPtr(ptr + (line-ofs))	{GSYS_CFN;}
#endif //---------------------------------------------------------------------------------------
		public:
			iterator(): m_ptr(gsys_null),m_EndlPtr(gsys_null),m_szLine(0),m_szOfs(0){GSYS_CFN;}
			iterator & operator ++(){GSYS_CFN;
				GSYS_ASSERT(m_ptr < m_pStopPtr);
				m_ptr++;
				if(m_ptr >= m_EndlPtr){
					m_ptr += m_szOfs;
					m_EndlPtr += m_szLine;
				}
				return *this;
			}
			reference operator *(){GSYS_CFN;GSYS_ASSERT(m_ptr != m_pStopPtr);return *m_ptr;}
			pointer operator ->(){GSYS_CFN;GSYS_ASSERT(m_ptr != m_pStopPtr);return m_ptr;}
			bool operator == (iterator const& it)const{GSYS_CFN;return m_ptr == it.m_ptr;}
			bool operator != (iterator const& it)const{GSYS_CFN;return m_ptr != it.m_ptr;}
			bool operator > (iterator const& it)const{GSYS_CFN;return m_ptr > it.m_ptr;}
			bool operator < (iterator const& it)const{GSYS_CFN;return m_ptr < it.m_ptr;}
			bool operator >= (iterator const& it)const{GSYS_CFN;return m_ptr >= it.m_ptr;}
			bool operator <= (iterator const& it)const{GSYS_CFN;return m_ptr <= it.m_ptr;}
		};
//=================================================================================================
		class const_iterator
		{
			friend class array2d;
			pointer m_ptr;
			pointer m_EndlPtr;
			size_type m_szLine;
			size_type m_szOfs;
#ifdef GSYS_ENABLE_ASSERT //-----------------------------------------------------------------------------
			pointer m_pStopPtr;
			const_iterator(pointer ptr,size_type line=0, size_type ofs=0,pointer stop=0):
					m_ptr(ptr),m_szLine(line),m_szOfs(ofs),
					m_EndlPtr(ptr + (line-ofs)),
					m_pStopPtr(stop) 
		{GSYS_CFN;if(!m_pStopPtr)m_pStopPtr=m_ptr;}
#else //----------------------------------------------------------------------------------------
			const_iterator(pointer ptr,size_type line=0, size_type ofs=0):
					m_ptr(ptr),m_szLine(line),m_szOfs(ofs),
					m_EndlPtr(ptr + (line-ofs))	{GSYS_CFN;}
#endif //---------------------------------------------------------------------------------------
		public:
			const_iterator(): m_ptr(gsys_null),m_EndlPtr(gsys_null),m_szLine(0),m_szOfs(0){GSYS_CFN;}
			const_iterator & operator ++(){GSYS_CFN;
				GSYS_ASSERT(m_ptr < m_pStopPtr);
				m_ptr++;
				if(m_ptr >= m_EndlPtr){
					m_ptr += m_szOfs;
					m_EndlPtr += m_szLine;
				}
				return *this;
			}
			const_reference operator *()const{GSYS_CFN;GSYS_ASSERT(m_ptr != m_pStopPtr);return *m_ptr;}
			const_pointer operator ->()const{GSYS_CFN;GSYS_ASSERT(m_ptr != m_pStopPtr);return m_ptr;}
			bool operator == (const_iterator const& it)const{GSYS_CFN;return m_ptr == it.m_ptr;}
			bool operator != (const_iterator const& it)const{GSYS_CFN;return m_ptr != it.m_ptr;}
			bool operator > (const_iterator const& it)const{GSYS_CFN;return m_ptr > it.m_ptr;}
			bool operator < (const_iterator const& it)const{GSYS_CFN;return m_ptr < it.m_ptr;}
			bool operator >= (const_iterator const& it)const{GSYS_CFN;return m_ptr >= it.m_ptr;}
			bool operator <= (const_iterator const& it)const{GSYS_CFN;return m_ptr <= it.m_ptr;}
		};
	public: // functions
#ifdef GSYS_ENABLE_ASSERT //-----------------------------------------------------------------------------
		iterator begin(){GSYS_CFN;return iterator(m_pBuffer,m_iPintch,m_iPintch-m_iWidth,
			m_pBuffer+m_iPintch*m_iHeight-(m_iPintch-m_iWidth));}
		const_iterator begin()const{GSYS_CFN;return const_iterator(m_pBuffer,m_iPintch,m_iPintch-m_iWidth,
			m_pBuffer+m_iPintch*m_iHeight-(m_iPintch-m_iWidth));}
#else //----------------------------------------------------------------------------------------
		iterator begin(){GSYS_CFN;return iterator(m_pBuffer,m_iPintch,m_iPintch-m_iWidth);}
		const_iterator begin()const{GSYS_CFN;return const_iterator(m_pBuffer,m_iPintch,m_iPintch-m_iWidth);}
#endif //---------------------------------------------------------------------------------------
		iterator end(){GSYS_CFN;return iterator(m_pBuffer+m_iPintch*(m_iHeight));}	
		const_iterator end()const{GSYS_CFN;return const_iterator(m_pBuffer+m_iPintch*(m_iHeight));}	

		reference front(){return *m_pBuffer();}
		const_reference front()const{return *m_pBuffer();}

    protected:
        size_type m_iWidth;
        size_type m_iHeight;
        size_type m_iPintch;
        pointer m_pBuffer;
        bool m_bOwnHandle;
    public:
        size_type width() const {GSYS_CFN;return m_iWidth;};
        size_type height() const {GSYS_CFN;return m_iHeight;};
        size_type pintch() const {GSYS_CFN;return m_iPintch;};
        size_type size() const {GSYS_CFN;return m_iHeight*m_iPintch;};
		bool empty() const {GSYS_CFN;return !m_iWidth || !m_iHeight;};

		size_type raw_rowsize() const {GSYS_CFN;return m_iPintch*sizeof(value_type);};

        T* buffer() const {GSYS_CFN;return m_pBuffer;};

        array2d()
        {GSYS_CFN;
            m_bOwnHandle = false;
            m_iWidth=0;
            m_iHeight=0;
			m_iPintch=0;
            m_pBuffer=gsys_null;
        }
        array2d(size_type width,size_type height,T* pt=0)
        {GSYS_CFN;
            m_bOwnHandle = false;
			init(width,height,pt);
        }
        array2d(size_type width,size_type height,size_type pintch,T* pt=0)
        {GSYS_CFN;
            m_bOwnHandle = false;
			init(width,height,pintch,pt);
        }
        array2d(array2d<T> const& l_arr)
        {GSYS_CFN;
			m_bOwnHandle=false;
            operator = (l_arr);
        }
        ~array2d()
        {GSYS_CFN;
            free();
        }

		void make_copy(array2d<T> const& l_arr)
		{GSYS_CFN;
			GSYS_ASSERT(&l_arr != this);
			free();
            m_iWidth=l_arr.m_iWidth;
            m_iHeight=l_arr.m_iHeight;
			m_iPintch = m_iWidth;
			m_bOwnHandle = true;

			// copy buffer contens (this is independed array)
			// allocate memory
            m_pBuffer = (T*)operator new(sizeof(T)*m_iPintch*m_iHeight); 
			// construct objects
			pointer pt = m_pBuffer;
			for(size_type i = 0;i<m_iHeight;++i)
			{
				for(size_type j = 0;j<m_iWidth;++j,++pt)
				{
					new ((void*)pt) T(l_arr(j,i));
				}
				pt += m_iPintch-m_iWidth;
			}
		}
		void make_ref(array2d<T> const& l_arr)
		{GSYS_CFN;
			m_iWidth=l_arr.m_iWidth;
			m_iHeight=l_arr.m_iHeight;
			m_iPintch=l_arr.m_iPintch;
			m_bOwnHandle = false;
			m_pBuffer = l_arr.m_pBuffer;
		}
        array2d<T> const& operator =(array2d<T> const& l_arr)
        {GSYS_CFN;
			free();
			if(l_arr.m_bOwnHandle)	make_copy(l_arr);
			else make_ref(l_arr);
            return *this;
        }

		void unlink_handle()
		{
			m_bOwnHandle = false;
		}

        // free up handle if owned
        void free()
        {GSYS_CFN;
            if(m_bOwnHandle)
			{
				// destruct opjects
				pointer pt = m_pBuffer;
				for(int i = (int)m_iHeight;--i>=0;)
				{
					for(int j = (int)m_iWidth;--j>=0;pt++)
					{
						(pt)->~T();
					}
					pt += m_iPintch-m_iWidth;
				}
				// deallocate objects
				gfree(m_pBuffer);
			}
			m_bOwnHandle = false;
			m_iWidth=0;
			m_iHeight=0;
			m_iPintch=0;
			m_pBuffer=gsys_null;
        }
        void init(size_type width,size_type height,T* pt=0)
		{GSYS_CFN;
			init(width,height,width,pt);
		}

        void init(size_type width,size_type height,size_type pintch,T* pt=0)
        {GSYS_CFN;
			GSYS_ASSERT(pintch >= width);
            free();
            m_iHeight = height;
			m_iPintch = pintch;
            m_iWidth = width;
            if(pt)
            {
                m_pBuffer = pt;
                m_bOwnHandle = false;
            }
            else
            {
				// allocate memory
                m_pBuffer =  galloc_raw_objects<T>(m_iPintch*m_iHeight); 
				// construct objects
				pointer ptr = m_pBuffer;
				for(int i = (int)m_iHeight;--i >= 0;)
				{
					for(int j = (int)m_iWidth;--j >= 0;ptr++)
					{
						new ((void*)ptr) T;
					}
					ptr += m_iPintch-m_iWidth;
				}
                m_bOwnHandle = true;
            }
        }
		// return row pointer
		pointer row(size_type y)
		{GSYS_CFN;
			GSYS_ASSERT(y < m_iHeight);
			return m_pBuffer+y*m_iPintch;
		}
		// return row pointer
		const_pointer row(size_type y)const
		{GSYS_CFN;
			GSYS_ASSERT(y < m_iHeight);
			return m_pBuffer+y*m_iPintch;
		}
		// create array represents a 'window' in orginal array
		//   that does not have owned handle
		template<class _RTYPE>
		array2d<T> window(gsys::tRect<_RTYPE> const& rc)const
		{GSYS_CFN;
			// avoid empty arrays with size 
			if(rc.width() == 0 || rc.height() == 0 )return array2d<T>();

			// ensure that window within rect
			GSYS_ASSERT(gsys::tRect<_RTYPE>(0,0,(_RTYPE)width(),(_RTYPE)height()).Contains(rc));

			pointer s = m_pBuffer+rc.top*m_iPintch+rc.left;
			return array2d<T>(rc.width(),rc.height(),m_iPintch,s);
		}

		template<class _RTYPE>
		array2d<T> safe_window(gsys::tRect<_RTYPE> const& rc)const
		{GSYS_CFN;
			// ensure that window within rect
			return window(gsys::tRect<_RTYPE>(0,0,(_RTYPE)width(),(_RTYPE)height()).Bound(rc));
		}

		/*========================================================================*\
		 *	DATA ACCESS
		\*========================================================================*/

		// 2dim address in array
		reference get(size_type x,size_type y)
		{	GSYS_CFN;
			GSYS_ASSERT(x < (ulong)m_iWidth);
			GSYS_ASSERT(y < (ulong)m_iHeight);
			return *(m_pBuffer+y*m_iPintch+x);    
		}
		const_reference get(size_type x,size_type y) const
		{	GSYS_CFN;
			GSYS_ASSERT(x < (ulong)m_iWidth);
			GSYS_ASSERT(y < (ulong)m_iHeight);
			return *(m_pBuffer+y*m_iPintch+x);    
		}

		// 2dim address in array
		template<class _RTYPE>reference operator ()(tPoint<_RTYPE> const& pt) 
			{return get((size_type)pt.x,(size_type)pt.y);}
		template<class _RTYPE> const_reference operator ()(tPoint<_RTYPE> const& pt) const
			{return get((size_type)pt.x,(size_type)pt.y);}
		// 2dim address in array
		reference operator ()(size_type x,size_type y){return get(x,y);}
		const_reference operator ()(size_type x,size_type y) const{return get(x,y);}

		// plain address in array
		reference operator ()(size_type x) const
		{GSYS_CFN;
			GSYS_ASSERT(x < size());
			return m_pBuffer[x];    
		}
		
		/*========================================================================*\
		    OPERATIONS
		\*========================================================================*/
		// fill entire array with spetifies value
		void fill(T const& f)
		{GSYS_CFN;
			// fill 
			for(size_type i=0;i<m_iHeight;i++)
			{
				for(size_type j=0;j<m_iWidth;j++)
				{
					*(m_pBuffer+i*m_iPintch+j)=f;    
				}
			}
		}
		// copy contens from oter array
		void fill(array2d<T> const& f)
		{GSYS_CFN;
			// TODO: Optimise
			GSYS_ASSERT(m_iHeight <= f.m_iHeight && m_iWidth <= f.m_iWidth);
			// fill 
			for(size_type i=0;i<m_iHeight;i++)
			{
				const_pointer src_row = f.row(i);
				pointer dst_row = row(i);
				for(size_type j=0;j<m_iWidth;j++)
				{
					dst_row[j] = src_row[j];
				}
			}
		}
		// fill row values in bound of single row
		void fill_row(size_type x,size_type y,T const& f,size_type sz)
		{GSYS_CFN;
			GSYS_ASSERT(x<m_iWidth);
			T* rw = row(y)+x;
			int end = bound(x+sz,0u,m_iWidth)-x;
			for(int i=0;i<end;i++)
			{
				rw[i] = f;
			}
		}
		// past array of  walues at row
		void past_row(size_type x,size_type y,T const* f,size_type sz)
		{GSYS_CFN;
			GSYS_ASSERT(x<m_iWidth);
			T* rw = row(y)+x;
			int end = bound(x+sz,0u,m_iWidth)-x;
			for(int i=0;i<end;i++)
			{
				rw[i] = f[i];
			}
		}
		// shift array by given values
		void shift_up(size_type y,const_reference f=T())
		{GSYS_CFN;
			if(y>=m_iHeight){fill(f);return;}
			// copy rows
			for(size_type i=y;i<m_iHeight;i++)
			{
				for(size_type j=0;j<m_iWidth;j++)
				{
					operator()(j,i-y) = operator()(j,i);    
				}
			}
			// fill rest
			for(size_type i=m_iHeight-y;i<m_iHeight;i++)
			{
				for(size_type j=0;j<m_iWidth;j++)
				{
					operator()(j,i)= f;    
				}
			}
		}	

		// shift array by given values
		void shift_down(size_type y,const_reference f=T())
		{GSYS_CFN;
			if(y>=m_iHeight){fill(f);return;}
			// copy rows
			for(size_type i=m_iHeight-y;--i>0;)
			{
				for(size_type j=0;j<m_iWidth;j++)
				{
					operator()(j,i+y) = operator()(j,i);    
				}
			}
			// fill rest
			for(size_type i=0;i<y;i++)
			{
				for(size_type j=0;j<m_iWidth;j++)
				{
					operator()(j,i)= f;    
				}
			}
		}	

		// shift array left by given values
		void shift_left(size_type x,const_reference f=T())
		{GSYS_CFN;
			if(x>=m_iWidth){fill(f);return;};
			// free rows
			for(size_type i=0;i<m_iHeight;i++)
				for(size_type j=0;j<=x;j++)
					operator()(j,i).~T();
			// shift rows
			for(size_type i=0;i<m_iHeight;i++)
				memmove(row(i),row(i)+x,m_iWidth-x);
			// fill rest
			for(size_type i=0;i<m_iHeight;i++)
				for(size_type j=m_iWidth-x;j<m_iWidth;j++)
					operator()(j,i) = f;  
		}	
		void shift_right(size_type x,const_reference f=T())
		{GSYS_CFN;
			if(x>=m_iWidth){fill(f);return;};
			// free rows
			for(size_type i=0;i<m_iHeight;i++)
				for(size_type j=m_iWidth-x;j<m_iWidth;j++)
					operator()(j,i).~T();
			// shift rows
			for(size_type i=0;i<m_iHeight;i++)
				memmove(row(i)+x,row(i),m_iWidth-x);
			// fill rest
			for(size_type i=0;i<m_iHeight;i++)
				for(size_type j=0;j<x;j++)
					operator()(j,i) = f;  
		}	
		// shift array by given values
		void shift(int x,int y,const_reference f=T())
		{GSYS_CFN;
			if(y<0)shift_up(-y,f);
			else if(y>0)shift_down(y,f);

			if(x<0)shift_left(-x,f);
			else if(x>0)shift_right(x,f);

		}
 		// raw writes to memory location no copy constructors & etc called !!!
		// use only for raw data !!!
		void raw_write(T* pt,int w,int h,int p)
		{GSYS_CFN;
			// TODO: Implement raw_write
		}
		// raw reads from memory location no copy constructors & etc called !!!
		// use only for raw data !!!
		void raw_read(T* pt,int w,int h,int p)
		{GSYS_CFN;
			// TODO: Implement raw_read
		}

		void mirror_vertical()
		{
			value_type* first_row = m_pBuffer;
			value_type* last_row = m_pBuffer+(m_iHeight-1)*m_iPintch;

			size_type num = (m_iHeight/2); // number of replaces

            for(uint i=0;i<num;i++)
			{
				value_type* end_frow = first_row+m_iWidth;
				while(first_row < end_frow) // flip rows
				{
					::gsys::exchange(first_row[0],last_row[0]);
					first_row++;
					last_row++;
				}
				first_row += m_iPintch-m_iWidth;
				last_row -= m_iPintch+m_iWidth;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Serialization support
   public:

	   friend inline gsys::binostream& operator << (gsys::binostream &os, this_type const& a)
	   {
		   // Save only own data window as raw data 
		   os << a.width() << a.height();
		   for(size_type i=0;i<a.height();i++)
		   {
			   const_pointer pt = a.row(i);
			   for(size_type j=0;j<a.width();j++) os << pt[j];
		   }
		   return os;
	   }

	   friend inline gsys::binistream& operator >> (gsys::binistream& is,this_type &a)
	   {
		   size_type w,h;
		   is >> w >> h;
		   a.init(w,h);
		   // Load
		   for(int i=0;i<h;i++)
		   {
			   pointer pt = a.row(i);
			   for(int j=0;j<w;j++) is >> pt[j];
		   }
		   return is;
	   }   

	   friend inline std::ostream& operator << (std::ostream& os,this_type const& a)
	   {
		   // Save only own data window as raw data 
		   os << "{ {" << a.width() << ";" << a.height() << "}";
		   for(size_type i=0;i<a.height();i++)
		   {
			   os << "," << "{";
			   const_pointer pt = a.row(i);
			   os << pt[0];
			   for(size_type j=1;j<a.width();j++) os << "," << pt[j];
			   os << "}";
		   }
		   return os;
	   }

	   friend inline std::istream& operator >> (std::istream& is, this_type &a)
	   {
		   wsp_delim D_BO('{');wsp_delim D_BC('}');wsp_delim D_D(',');
		   size_type w,h;
		   is >> D_BO;
		   is >> D_BO >> w >> D_D >> h >> D_BC;

		   a.init(w,h);
		   // Load
		   for(size_type i=0;i<h;i++)
		   {
			   is >> D_D >> D_BO;
			   pointer pt = a.row(i);
			   is >> pt[0];
			   for(size_type j=1;j<w;j++)is >> D_D >> pt[j];
			   is >> D_BC;
		   }
		   is >> D_BC;
    		return is;
	   }   
   };

   template<typename T>
	   void bordered_copy(gsys::array2d<T>& d,gsys::array2d<T> const&s)
   {
	   typedef typename gsys::array2d<T> dtype;	
	   typedef typename dtype::size_type stype;	

	   stype b_x = (d.width()-s.width())/2;
	   stype b_y = (d.height()-s.height())/2;

	   dtype wnd = d.window(tRect<stype>(b_x,b_y,d.width()-b_x,d.height()-b_y));
	   wnd.fill(s);

	   for(stype i=0;i<b_y;i++)
	   {
		   typename dtype::pointer drow;
		   typename dtype::const_pointer srow;
		   srow = s.row(0);
		   drow = d.row(i)+b_x;
		   objcpy(drow,srow,s.width());

		   srow = s.row(s.height()-1);
		   drow = d.row(d.height()-1-i)+b_x;
		   objcpy(drow,srow,s.width());
	   }
	   for(stype i=0;i<s.height();i++)
	   {
		   for(stype j=0;j<b_x;j++)
		   {
			   d(j,i+b_y) = s(0,i);
			   d(d.width()-1-j,i+b_y) = s(s.width()-1,i);
		   }
	   }
   }

};