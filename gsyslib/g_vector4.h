#pragma once
#ifndef G_VECTOR4_H
#define G_VECTOR4_H

#include "g_text.h"
#include "boost/operators.hpp"
#include "g_vector3.h"

namespace gsys
{
	struct vector4 :  
		boost::additive<vector4,
		boost::multiplicative2<vector4,float,
		boost::equality_comparable<vector4
		> > >
	{
		typedef gsys_real value_type;
		typedef uint size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
		GSYSDETAIL_IMPLEMENT_ARRAY_CONTAINER;

		size_type size()const{return 4;}
		reference operator [](size_type i){GSYS_CFN;GSYS_ASSERT(i<size());return values[i];}
		const_reference operator [](size_type i)const{GSYS_CFN;GSYS_ASSERT(i<size());return values[i];}

/*====================================================================================*\
		Data access
\*====================================================================================*/
        union {
            struct {
		        value_type x;
		        value_type y;
		        value_type z;
		        value_type w;
            };
            value_type values[4];
        };

		vector2& xy_ref(){return reinterpret_ref_cast<vector2>(*this);}
		vector2 const& xy_ref()const{return reinterpret_ref_cast<const vector2>(*this);}

		vector3& xyz_ref(){return reinterpret_ref_cast<vector3>(*this);}
		vector3 const& xyz_ref()const{return reinterpret_ref_cast<const vector3>(*this);}

/*====================================================================================*\
		Initializations
\*====================================================================================*/
		// default constructor
		vector4(){}
		// copy constructor
		vector4(vector4 const& v)
		{
			x=v.x;
			y=v.y;
			z=v.z;
			w=v.w;
		}
		// values constructor
		vector4(value_type l_x,value_type l_y,value_type l_z,value_type l_w)
		{
			x = l_x;
			y = l_y;
			z = l_z;
			w = l_w;
		}
		// assigment  operator
		vector4& operator = (vector4 const& v)
		{
			x=v.x;
			y=v.y;
			z=v.z;
			w=v.w;
			return *this;
		}

		value_type lensquared()const
		{GSYS_CFN;
			return x*x+y*y+z*z+w*w;
		}

		value_type len()const
		{GSYS_CFN;
			return g_sqrt(x*x+y*y+z*z+w*w);
		}

		// values constructor
		vector4& set(value_type l_x,value_type l_y,value_type l_z,value_type l_w)
		{
			x = l_x;
			y = l_y;
			z = l_z;
			w = l_w;
			return *this;
		}
/*====================================================================================*\
		Operators
\*====================================================================================*/
		// normalise vector
		vector4 & norm()
		{GSYS_CFN;
			value_type L_squared, one_over_L;
			L_squared = (x * x) + (y * y) + (z * z)+(w * w);
			one_over_L = detail::InvSqrt(L_squared);
			x = x * one_over_L;
			y = y * one_over_L;
			z = z * one_over_L;
			w = w * one_over_L;
			return *this;
		}

		vector4 operator -()const{GSYS_CFN;return vector4(-x,-y,-z,-w);}
		vector4 const& operator +()const{GSYS_CFN;return *this;}

		// compare vectors
        bool operator == (vector4 const& vt)const
            {return equ(x,vt.x) && equ(y,vt.y) && equ(z,vt.z) && equ(w,vt.w);}
		// combines 2 vectors
		vector4& operator += (vector4 const& v)
		{
			x+=v.x;
			y+=v.y;
			z+=v.z;
			w+=v.w;
			return *this;
		}
		vector4& operator -= (vector4 const& v)
		{
			x-=v.x;
			y-=v.y;
			z-=v.z;
			w-=v.w;
			return *this;
		}

		//mutiplyes vector by scalar
		vector4& operator *= (value_type f)
		{GSYS_CFN;
			x*=f;
			y*=f;
			z*=f;
			w*=f;
			return *this;
		}

		GSYSDETAIL_OSTREAM_STD_BASIC_OBJ(vector4,DO<<v.x<<D<<v.y<<D<<v.z<<D<<v.w<<DC)
		GSYSDETAIL_ISTREAM_STD_BASIC_OBJ(vector4,DO>>ISN(v.x)>>D>>ISN(v.y)>>D>>ISN(v.z)>>D>>ISN(v.w)>>DC)

		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(vector4)
	};
	GSYS_SET_TYPENAME(vector4,"v4");


	const vector4 vector4_ZERO(0,0,0,0);
	const vector4 vector4_UNIT_X(1,0,0,0);
	const vector4 vector4_UNIT_Y(0,1,0,0);
	const vector4 vector4_UNIT_Z(0,0,1,0);
	const vector4 vector4_UNIT_W(0,0,1,1);

	// Linear interpolation
	inline vector4 & lerp(vector4 const& v1,vector4 const& v2,float tm,vector4 & res)
	{return res = v1 + (v2-v1)*tm;}
	inline vector4 lerp(vector4 const& v1,vector4 const& v2,float tm)
	{vector4 v;return lerp(v1,v2,tm,v);}
}

#endif