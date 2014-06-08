#pragma once


#include "g_gsys.h"
#include "g_rect.h"
#include "g_point.h"
#include "g_assert.h"
#include "g_array2d.h"

namespace gsys
{
	template<class T,int _W,int _H> 
	class static_array2d
	{
	public: // typedefs
		typedef T value_type;
		typedef gsys_ulong size_type;
		typedef gsys_ulong difference_type;

		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
		// additional
        typedef T data_type; 
	public: // data
		T values[_W][_H];
        
	public: // acess

		size_type width() const {GSYS_CFN;return _W;};
        size_type height() const {GSYS_CFN;return _H;};
        size_type pintch() const {GSYS_CFN;return _W;};
        size_type size() const {GSYS_CFN;return _W*_H;};

        T* buffer() const {GSYS_CFN;return (T*)values;};
	public: // constructors
		static_array2d(){};

       
	public: 

		// return row pointer
		pointer row(gsys_ulong y)
		{GSYS_CFN;
			GSYS_ASSERT(y < _H);
			return &values[y][0];
		}
	
		pointer operator [](size_type x)
		{GSYS_CFN;
			GSYS_ASSERT(x < _W);
			return (values[x]);    
		}
		const_pointer operator [](size_type x)const
		{GSYS_CFN;
			GSYS_ASSERT(x < _W);
			return (values[x]);    
		}


		// 2dim address in array
		reference operator ()(size_type x,size_type y)
		{GSYS_CFN;
			GSYS_ASSERT(x < _W);
			GSYS_ASSERT(y < _H);
			return (values[y][x]);    
		}
		const_reference operator ()(size_type x,size_type y) const
		{GSYS_CFN;
			GSYS_ASSERT(x < _W);
			GSYS_ASSERT(y < _H);
			return (values[y][x]);    
		}

		// 2dim address in array
		template<class _RTYPE>
		reference operator ()(tPoint<_RTYPE> const& pt) const
		{GSYS_CFN;
			return operator()((size_type)pt.x,(size_type)pt.y);
		}

	public:
		/*========================================================================*\
		    OPERATIONS
		\*========================================================================*/
		// fill entire array with spetifies value
		void fill(T const& f)
		{GSYS_CFN;
			// fill 
			for(size_type i=0;i<_W;i++)
				for(size_type j=0;j<_H;j++)
					values[i][j]=f;    
		}
	};
}