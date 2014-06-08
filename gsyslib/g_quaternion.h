#pragma once
#ifndef G_QUATERNION_H
#define G_QUATERNION_H

#include "g_math.h"
#include "boost/operators.hpp"

#include "g_vector3.h"

namespace gsys
{
	struct quaternion :  
		boost::multiplicative1<quaternion,
		boost::equality_comparable1<quaternion
		> >
	{
		typedef gsys_real value_type;
		typedef uint size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
		/*====================================================================================*\
		        Data Access
		\*====================================================================================*/
        union
        {
			struct {
				value_type x;
				value_type y;
				value_type z;
				value_type w;
			};
			struct
			{
				value_type _vec3[3];
				value_type r;
			};
			value_type values[4];
        };

		size_type size()const{return 4;}
		reference operator [](size_type i){GSYS_ASSERT(i<4);return values[i];} 
		const_reference operator [](size_type i)const{GSYS_ASSERT(i<4);return values[i];} 
		GSYSDETAIL_IMPLEMENT_ARRAY_CONTAINER;

		bool operator == (quaternion const& vt)const
			{return fequ(x,vt.x) && fequ(y,vt.y) && fequ(z,vt.z) && fequ(w,vt.w);}

        vector3& vec3(){return *reinterpret_cast<vector3*>(_vec3);}
        vector3 const& vec3()const{return *reinterpret_cast<vector3 const*>(_vec3);}
		/*====================================================================================*\
		Construction
		\*====================================================================================*/
		quaternion(){}
		quaternion(value_type _x,value_type _y,value_type _z,value_type _w){x=_x;y=_y;z=_z;w=_w;}
		/*====================================================================================*\
		Initialisations
		\*====================================================================================*/
		quaternion& set(value_type l_x,value_type l_y,value_type l_z,value_type l_w)
		{GSYS_CFN;
			x=l_x;
			y=l_y;
			z=l_z;
			w=l_w;
			return *this;
		}
		inline quaternion& set(vector3 const& l_v,value_type l_w)
		{GSYS_CFN;
			x=l_v.x;
			y=l_v.y;
			z=l_v.z;
			w=l_w;
			return *this;
		}

		quaternion& identity()
		{
			w=1.0f;
			x=y=z=0.0f;
			return *this;
		}

		quaternion&   from_two_unit_dirs(const vector3& UnitFrom,const vector3& UnitTo)
		{
			float CosA = dot(UnitFrom, UnitTo); 
			if( CosA < - 0.99999f )
			{  // angle close to PI    ( can replaced by Bisect.lensquared() < 0.000001f ) ;
				vector3 CrossVec( 0, UnitFrom.x, -UnitFrom.y ); // cross with (1, 0, 0) 
				if( ( UnitFrom.z*UnitFrom.z ) > ( UnitFrom.y*UnitFrom.y ) )
				{
					// if (0, 1, 0) Cross > (1, 0, 0) Cross
					CrossVec.set( -UnitFrom.z, 0, UnitFrom.x ); // cross with (0 ,1, 0)
				}
				CrossVec.norm();
				set(CrossVec,0.0f);
			}
			else
			{
				vector3 Bisect( UnitFrom + UnitTo );
				Bisect.norm();
				set(cross(UnitFrom,Bisect),dot(UnitFrom, Bisect));
			}
			return *this;
		}

		quaternion& angle_axis_rotation_tri(vector3 const& angle,value_type _sind2,value_type _cosd2)
		{
			w = _cosd2;
			float s = _sind2;
			x=angle.x*s;
			y=angle.y*s;
			z=angle.z*s;
			return *this;
		}

		quaternion& angle_axis_rotation(vector3 const& angle,value_type axis)
		{
			return angle_axis_rotation_tri(angle, sinf(axis /2),cosf(axis/2));
		}

		quaternion& rotation_x(value_type axis)
		{
			return angle_axis_rotation(vector3_UNIT_X,axis);
		}


		quaternion& rotation_y(value_type axis)
		{
			return angle_axis_rotation(vector3_UNIT_Y,axis);
		}

		quaternion& rotation_z(value_type axis)
		{
			return angle_axis_rotation(vector3_UNIT_Z,axis);
		}

		quaternion& euler_rotation(value_type roll, value_type pitch, value_type yaw)
		{
			value_type cr, cp, cy, sr, sp, sy, cpcy, spsy;

			// calculate trig identities
			cr = cos(roll/2);

			cp = cos(pitch/2);
			cy = cos(yaw/2);

			sr = sin(roll/2);
			sp = sin(pitch/2);
			sy = sin(yaw/2);
			
			cpcy = cp * cy;
			spsy = sp * sy;

			w = cr * cpcy + sr * spsy;
			x = sr * cpcy - cr * spsy;
			y = cr * sp * cy + sr * cp * sy;
			z = cr * cp * sy - sr * sp * cy;
			return *this;
		}


		/*====================================================================================*\
		Modifications
		\*====================================================================================*/
			
		quaternion& rotate_x(value_type axis)
		{
			return *this *= quaternion().angle_axis_rotation(vector3_UNIT_X,axis);
		}

		quaternion& rotate_y(value_type axis)
		{
			return *this *= quaternion().angle_axis_rotation(vector3_UNIT_Y,axis);
		}

		quaternion& rotate_z(value_type axis)
		{
			return *this *= quaternion().angle_axis_rotation(vector3_UNIT_Z,axis);
		}

		quaternion& operator*=(quaternion const& quat)
		{
			value_type A, B, C, D, E, F, G, H;

			A = (w + x)*(quat.w + quat.x);
			B = (z - y)*(quat.y - quat.z);
			C = (w - x)*(quat.y + quat.z); 
			D = (y + z)*(quat.w - quat.x);
			E = (x + z)*(quat.x + quat.y);
			F = (x - z)*(quat.x - quat.y);
			G = (w + y)*(quat.w - quat.z);
			H = (w - y)*(quat.w + quat.z);

			w = B + (-E - F + G + H)/2;
			x = A - (E + F + G + H)/2; 
			y = C + (E - F + G - H)/2; 
			z = D + (E - F - G + H)/2;
			return *this;
		}

		quaternion&  conjugate()
		{
			x=-x;
			y=-y;
			z=-z;
			return *this;
		}

		quaternion&  build_conjugate(quaternion& qt)const
		{
			qt.x=-x;
			qt.y=-y;
			qt.z=-z;
			return qt;
		}
		quaternion  build_conjugate()const
		{
			quaternion qt; return build_conjugate(qt);
		}

		quaternion&  inverse()
		{
			conjugate();
			normalize();
			return *this;
		}

		quaternion&  build_inverse(quaternion& qt)const
		{
			build_conjugate(qt);
			qt.normalize();
			return qt;
		}	
		quaternion  build_inverse()const
		{
			quaternion qt;
			return build_inverse(qt);
		}

		quaternion&  UnitInverse()
		{
			GSYS_ASSERT(equ(SquaredLength(),1.0f));
			conjugate();
			return *this;
		}

		quaternion&  normalize()
		{
            value_type L_squared, one_over_L;
            L_squared = (w * w) + (x * x) + (y * y) + (z * z);
			one_over_L = detail::InvSqrt(L_squared);
            w = w * one_over_L;
            x = x * one_over_L;
            y = y * one_over_L;
            z = z * one_over_L;
			return *this;
		}
		quaternion&  norm()
		{
			return normalize();
		}
		/*====================================================================================*\
		Operations
		\*====================================================================================*/
		value_type Lenght()
		{
			return g_sqrt(x*x+y*y+z*z+w*w);
		}
		value_type SquaredLength()
		{
			return x*x+y*y+z*z+w*w;
		}

		vector3& xform(vector3 const& v, vector3 &res) const
		{
			vector3 uv, uuv;
			uv = cross(vec3(), v);
			uuv = cross(vec3(), uv);

			uv *= 2.0f*w;
			uuv *= 2.0f;

			res = v + uv + uuv;

			return res;
		}
		vector3 xform(vector3 const& v)const {vector3 vt;return xform(v,vt);}

		/*====================================================================================*\
		Static operations
		\*====================================================================================*/


		GSYSDETAIL_OSTREAM_BASIC_OBJ(quaternion,os<<'{'<<v.x<<','<<v.y<<','<<v.z<<','<<v.w<<'}')

		GSYSDETAIL_ISTREAM_STD_BASIC_OBJ(quaternion,DO>>v.x>>D>>v.y>>D>>v.z>>D>>v.w>>DC)

		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(quaternion)

	};
	GSYS_SET_TYPENAME(quaternion,"qt");


	// special points
	const quaternion quaternion_ZERO(0,0,0,0);
	const quaternion quaternion_IDENTITY(0,0,0,1);

	inline quaternion& slerp(quaternion const& from, quaternion const& to, float t, quaternion & res)
	{
		const quaternion::value_type epsilon = 1E-3f;

		quaternion::value_type         to1[4];
		//double        omega, cosom, sinom, scale0, scale1;
		quaternion::value_type        omega, cosom, sinom, scale0, scale1;

		// calc cosine
		cosom = from.x * to.x + from.y * to.y + from.z * to.z	+ from.w * to.w;
		// adjust signs (if necessary)
		if ( cosom <0.0 )
		{ 
			cosom = -cosom;
			to1[0] = - to.x;
			to1[1] = - to.y;
			to1[2] = - to.z;
			to1[3] = - to.w;
		}
		else 
		{
			to1[0] = to.x;
			to1[1] = to.y;
			to1[2] = to.z;
			to1[3] = to.w;
		}
		// calculate coefficients
		if ( (1.0f - cosom) > epsilon )
		{
			// standard case (slerp)
			omega = acosf(cosom);
			sinom = sinf(omega);
			GSYS_ASSERT(!fequ(sinom,0));
			scale0 = sinf((1.0f - t) * omega) / sinom;
			scale1 = sinf(t * omega) / sinom;
		} 
		else 
		{        
			// "from" and "to" quaternions are very close 
			//  ... so we can do a linear interpolation
			scale0 = 1.0f - t;
			scale1 = t;
		}
		// calculate final values
		res.x = scale0 * from.x + scale1 * to1[0];
		res.y = scale0 * from.y + scale1 * to1[1];
		res.z = scale0 * from.z + scale1 * to1[2];
		res.w = scale0 * from.w + scale1 * to1[3];

		res.norm();

		return res;
	}
	inline quaternion slerp(quaternion const& from, quaternion const& to, quaternion::value_type t)
	{
        quaternion q;
		return slerp(from,to,t,q);
	}

	// Linear interpolation
	inline quaternion & lerp(quaternion const& v1,quaternion const& v2,float tm,quaternion & res)
	{return slerp(v1,v2,tm,res);}
	inline quaternion lerp(quaternion const& v1,quaternion const& v2,float tm)
	{return slerp(v1,v2,tm);}

}

#endif