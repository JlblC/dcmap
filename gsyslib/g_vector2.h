
#pragma once
#ifndef G_VECTOR2_H
#define G_VECTOR2_H

#include "g_gsys.h"

#include "g_math.h"
#include "g_alg.h"
#include "g_text.h"
#include "g_binstream.h"
#include "boost/operators.hpp"
#include "g_point.h"

namespace gsys
{
	class vector2 :   
		boost::additive<vector2,
		boost::multiplicative2<vector2,float,
		boost::totally_ordered<vector2
		> > >
	{
	public:
		typedef gsys_real value_type;
		typedef gsys_ulong size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
		GSYSDETAIL_IMPLEMENT_ARRAY_CONTAINER;

		size_type size()const{return 2;}
		reference operator [](size_type i){GSYS_CFN;GSYS_ASSERT(i<size());return (&x)[i];}
		const_reference operator [](size_type i)const{GSYS_CFN;GSYS_ASSERT(i<size());return (&x)[i];}

/*====================================================================================*\
		Data access
\*====================================================================================*/
        union {
            struct {
		        value_type x;
		        value_type y;
            };
            value_type values[2];
        };

/*====================================================================================*\
		Initializations
\*====================================================================================*/
		// default constructor
		vector2(){}
		// values constructor
		vector2(value_type l_x,value_type l_y):x(l_x),y(l_y)
		{GSYS_CFN;
		}
		vector2& set(value_type l_x,value_type l_y)
		{GSYS_CFN;
			x=l_x;
			y=l_y;
			return *this;
		}

		vector2(tPoint<value_type>const&p)
		{GSYS_CFN;
			x=p.x;
			y=p.y;
		}

		tPoint<value_type>& as_point(){return reinterpret_ref_cast<tPoint<value_type> >(*this);}
		tPoint<value_type>const& as_point()const{return reinterpret_ref_cast<const tPoint<value_type> >(*this);}
/*====================================================================================*\
		Functions
\*====================================================================================*/

		float len()const
		{
			return g_sqrt(x*x+y*y);
		}
		float lensquared()const
		{
			return x*x+y*y;
		}

		// return dot product 
		float dot(vector2 const& v)const
		{
			return x*v.x+y*v.y;
		}

		vector2  get_cross ()const  // returns (y,-x)
		{
			return vector2(y,-x);
		}

/*====================================================================================*\
		Modification
\*====================================================================================*/

#if 1
		vector2 & norm()
        {GSYS_CFN;
            value_type L_squared, len;
            L_squared = (x * x) + (y * y);
			len = sqrtf(L_squared);
            x /= len;
            y /= len;
			return *this;
        }
#else
		vector2 & norm()
		{GSYS_CFN;
			value_type L_squared, one_over_L;
			L_squared = (x * x) + (y * y);
			one_over_L = detail::InvSqrt(L_squared);
			x *= one_over_L;
			y *= one_over_L;
			return *this;
		}
#endif
		vector2 & cross ()  // returns (y,-x)
		{
			exchange(x,y);
			y=-y;
			return *this;
		}

		vector2 & rotate(float fA)
        {GSYS_CFN;

			float sina = sinf(fA);
			float cosa = cosf(fA);

			float _x = x*cosa-y*sina;
			float _y = x*sina+y*cosa;

			x = _x;
			y = _y;
			return *this;
        }
/*====================================================================================*\
		Operators
\*====================================================================================*/
		vector2 operator -()const
		{GSYS_CFN;
			return vector2(-x,-y);
		}
		vector2 operator +()const
		{GSYS_CFN;
			return vector2(x,y);
		}
		
		// combines 2 vectors
		vector2& operator += (vector2 const& v)
		{
			x+=v.x;
			y+=v.y;
			return *this;
		}
		vector2& operator -= (vector2 const& v)
		{
			x-=v.x;
			y-=v.y;
			return *this;
		}

		// mutiplyes vector by scalar
		vector2& operator *= (value_type f)
		{GSYS_CFN;
			x*=f;
			y*=f;
			return *this;
		}
		vector2& operator /= (value_type f)
		{GSYS_CFN;
			x/=f;
			y/=f;
			return *this;
		}

		// compare vectors
        bool operator == (vector2 const& vt)const
            {return equ(x,vt.x) && equ(y,vt.y);}
        bool operator < (vector2 const& vt)const
            {return ((x<vt.x) || ((equ(x,vt.x) && (y < vt.y)))); }
        bool operator > (vector2 const& vt)const
            {return ((x>vt.x) || ((equ(x,vt.x) && (y > vt.y)))); }

		GSYSDETAIL_OSTREAM_BASIC_OBJ(vector2,os << "{" << v.x << "," << v.y << "}")

		GSYSDETAIL_ISTREAM_STD_BASIC_OBJ(vector2,DO >> ISN(v.x) >> D >> ISN(v.y) >> DC)

		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(vector2)
	};

	GSYS_SET_TYPENAME(vector2,"v2");

/*====================================================================================*\
		Constansts
\*====================================================================================*/
	const vector2 vector2_ZERO(0,0);
	const vector2 vector2_UNIT_X(1,0);
	const vector2 vector2_UNIT_Y(0,1);

/*====================================================================================*\
		Outclass Functions
\*====================================================================================*/
	// return dot product 
	inline vector2::value_type dot(vector2 const& v1,vector2 const& v2)
	{
		return v1.x*v2.x+v1.y*v2.y;
	}

	// Dot Product
	inline vector2::value_type operator % (vector2 const& v1,vector2 const& v2){return dot(v1,v2);}

	inline vector2 norm(vector2 const& vt){return vector2(vt).norm();}

	inline vector2 rotate(vector2 const& vt,float fA){return vector2(vt).rotate(fA);}

	inline vector2::value_type distance_squared(vector2 const& v1,vector2 const& v2)
	{GSYS_ASSERTREF(v1);GSYS_ASSERTREF(v2);
		float a=v1.x-v2.x;
		float b=v1.y-v2.y;
		return a*a+b*b;
	}

	// Linear interpolation
	inline vector2 & lerp(vector2 const& v1,vector2 const& v2,float tm,vector2 & res)
	{return res = v1 + (v2-v1)*tm;}
	inline vector2 lerp(vector2 const& v1,vector2 const& v2,float tm)
	{vector2 v;return lerp(v1,v2,tm,v);}

}

#endif
