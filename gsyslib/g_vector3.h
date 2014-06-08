#pragma once
#ifndef G_VECTOR3_H
#define G_VECTOR3_H

#include <ostream>
#include "g_math.h"
#include "g_text.h"
#include "g_binstream.h"
//#include "g_color.h"
#include "boost/operators.hpp"

#include "g_vector2.h"

namespace gsys
{
	struct vector3:  
		boost::additive<vector3,
		boost::multiplicative2<vector3,float,
		boost::equality_comparable<vector3
		> > >
	{
	typedef gsys_real value_type;
	typedef ulong size_type;
	GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
	GSYSDETAIL_IMPLEMENT_STATIC_ARRAY_CONTAINER(values,3);

/*====================================================================================*\
		Data access
\*====================================================================================*/
        union {
            struct {
		        value_type x;
		        value_type y;
		        value_type z;
            };
            value_type values[3];
        };
		vector2& xy_ref(){return reinterpret_ref_cast<vector2>(*this);}
		vector2 const& xy_ref()const{return reinterpret_ref_cast<const vector2>(*this);}
/*====================================================================================*\
		Initializations
\*====================================================================================*/
		vector3(){}
		// values constructor
		vector3(value_type l_x,value_type l_y,value_type l_z):
			x(l_x),
			y(l_y),
			z(l_z)
		{GSYS_CFN;
		}
		// assigment  operator
		vector3& set(value_type l_x,value_type l_y,value_type l_z)
		{GSYS_CFN;
			x=l_x;
			y=l_y;
			z=l_z;
			return *this;
		}
/*====================================================================================*\
		Conversion
\*====================================================================================*/
		operator bool()const
		{GSYS_CFN;
			return !(x==0&&y==0&&z==0);
		}
/*====================================================================================*\
		Modifications
\*====================================================================================*/

		vector3 operator -()const
		{GSYS_CFN;
			return vector3(-x,-y,-z);
		}
		vector3 operator +()const
		{GSYS_CFN;
			return vector3(x,y,z);
		}

		// combines 2 vectors
		vector3& operator += (vector3 const& v)
		{GSYS_CFN;
			x+=v.x;
			y+=v.y;
			z+=v.z;
			return *this;
		}
		vector3& operator -= (vector3 const& v)
		{GSYS_CFN;GSYS_ASSERTREF(v);
			x-=v.x;
			y-=v.y;
			z-=v.z;
			return *this;
		}
		// mutiplyes vector by scalar
		vector3& operator *= (value_type f)
		{GSYS_CFN;
			x*=f;
			y*=f;
			z*=f;
			return *this;
		}
		vector3& operator /= (value_type f)
		{GSYS_CFN;
			x/=f;
			y/=f;
			z/=f;
			return *this;
		}

		// normalise vector
        vector3 & norm()
        {GSYS_CFN;
            value_type L_squared, one_over_L;
            L_squared = (x * x) + (y * y) + (z * z);
			one_over_L = detail::InvSqrt(L_squared);
            x = x * one_over_L;
            y = y * one_over_L;
            z = z * one_over_L;
			return *this;
        }

/*====================================================================================*\
		Operations
\*====================================================================================*/

		// compare vectors
        bool operator == (vector3 const& vt)const
            {GSYS_CFN;return fequ(x,vt.x) && fequ(y,vt.y) && fequ(z,vt.z);}


		// return cross product 
		vector3 cross(vector3 const& v)const
		{GSYS_CFN;GSYS_ASSERTREF(v);
			vector3 vt;
			vt.x = y * v.z - z * v.y;
			vt.y = z * v.x - x * v.z;
			vt.z = x * v.y - y * v.x;
			return vt;
		}
		value_type dot(vector3 const& v)const
		{GSYS_CFN;GSYS_ASSERTREF(v);
			return x*v.x+y*v.y+z*v.z;
		}

		value_type lensquared()const
		{GSYS_CFN;
			return x*x+y*y+z*z;
		}
		value_type len()const
		{GSYS_CFN;
			return g_sqrt(x*x+y*y+z*z);
		}

/*
		//-----------------------------------------------------------------------------
		// Turns a vector into RGBA form. Used to encode vectors into
		// a height map. 
		//-----------------------------------------------------------------------------
		dwcolor make_rgba()const
		{GSYS_CFN;
			vector3 v=*this;
			v.norm();
			gsys_byte r = (gsys_byte)( 127.0f * v.x + 128.0f );
			gsys_byte g = (gsys_byte)( 127.0f * v.y + 128.0f );
			gsys_byte b = (gsys_byte)( 127.0f * v.z + 128.0f );
			gsys_byte a = (gsys_byte)( 255.0f * len() );
    
			return dwcolor(r,g,b,a);
		}

		//-----------------------------------------------------------------------------
		// Turns a normalised vector into RGBA form. Used to encode vectors into
		// a height map. 
		//-----------------------------------------------------------------------------
		dwcolor make_rgba(value_type fHeight=1.0f)const
		{GSYS_CFN;
			vector3 const&v=*this;
			dword r = (dword)( 127.0f * v.x + 128.0f );
			dword g = (dword)( 127.0f * v.y + 128.0f );
			dword b = (dword)( 127.0f * v.z + 128.0f );
			dword a = (dword)( 255.0f * fHeight );
    
			return dwcolor(r,g,b,a);
		}	
*/

		GSYSDETAIL_OSTREAM_STD_BASIC_OBJ(vector3,DO<<v.x<<D<<v.y<<D<<v.z<<DC);
		GSYSDETAIL_ISTREAM_STD_BASIC_OBJ(vector3,DO>>ISN(v.x)>>D>>ISN(v.y)>>D>>ISN(v.z)>>DC);
		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(vector3);
	};
	GSYS_SET_TYPENAME(vector3,"v3");

/*====================================================================================*\
		Constansts
\*====================================================================================*/
	const vector3 vector3_ZERO(0,0,0);
	const vector3 vector3_UNIT_X(1,0,0);
	const vector3 vector3_UNIT_Y(0,1,0);
	const vector3 vector3_UNIT_Z(0,0,1);

/*====================================================================================*\
		Outclass Functions
\*====================================================================================*/

	inline vector3 cross(vector3 const& v1,vector3 const& v2)
	{
		GSYS_ASSERTREF(v1);
		GSYS_ASSERTREF(v2);
		vector3 vt;
		vt.x = v1.y * v2.z - v1.z * v2.y;
		vt.y = v1.z * v2.x - v1.x * v2.z;
		vt.z = v1.x * v2.y - v1.y * v2.x;
		return vt;	
	}

	inline vector3::value_type dot(vector3 const& v1,vector3 const& v2)
	{
		GSYS_ASSERTREF(v1);
		GSYS_ASSERTREF(v2);
		return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
	}

	// Returns angle between two vectors in the range 0 to PI radians. 
	inline vector3::value_type angle(vector3 const& v1,vector3 const& v2)
	{GSYS_ASSERTREF(v1);GSYS_ASSERTREF(v2);
		vector3::value_type f = acosf(bound(dot(v1,v2)/(v1.len()*v2.len()),-1.0f,+1.0f));
		GSYS_ASSERT(!_isnan(f));
		return f;
	}

	inline vector3::value_type distance_squared(vector3 const& v1,vector3 const& v2)
	{GSYS_ASSERTREF(v1);GSYS_ASSERTREF(v2);
		float a=v1.x-v2.x;
		float b=v1.y-v2.y;
		float c=v1.z-v2.z;
		return a*a+b*b+c*c;
	}	

	// Cross Product
	inline vector3 operator * (vector3 const& v1,vector3 const& v2){return cross(v1,v2);}
	// Dot Product
	inline vector3::value_type operator % (vector3 const& v1,vector3 const& v2){return dot(v1,v2);}

	// Linear interpolation
	inline vector3 & lerp(vector3 const& v1,vector3 const& v2,float tm,vector3 & res)
	{return res = v1 + (v2-v1)*tm;}
	inline vector3 lerp(vector3 const& v1,vector3 const& v2,float tm)
	{vector3 v;return lerp(v1,v2,tm,v);}

}


#endif
