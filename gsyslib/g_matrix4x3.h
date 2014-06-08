#pragma once
#ifndef G_MATRIX4X3_H
#define G_MATRIX4X3_H

#include "g_gsys.h"
#include "g_base_matrix.h"
#include "g_text.h"
#include "g_transform_quat.h"
#include "g_matrix_operations.h"

namespace gsys
{
	struct matrix4x3
	{
		enum layout
		{
			l_11, l_21, l_31,l_41,
			l_12, l_22, l_32,l_42,
			l_13, l_23, l_33,l_43,
			l_width=4,
			l_height=3,
			l_transpozed=1,
		};
		/*====================================================================================*\
		types
		\*====================================================================================*/
		typedef gsys_real value_type;
		typedef uint size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
			/*====================================================================================*\
			Data access
			\*====================================================================================*/
			union
		{
			struct
			{
				value_type _11, _12, _13, _14;
				value_type _21, _22, _23, _24;
				value_type _31, _32, _33, _34;
			};
			value_type m[4][3];
			value_type values[4*3];
		};
		size_type size()const{return 4*3;}
		reference operator()(size_type x,size_type y){return m[x][y];}
		reference operator[](size_type x){GSYS_ASSERT(x<size());return values[x];}
		const_reference operator()(size_type x,size_type y)const{return m[x][y];}
		const_reference operator[](size_type x)const{GSYS_ASSERT(x<size());return values[x];}

		matrix4x3& identity()
		{
			_11 = 1; _12 = 0; _13 = 0; _14=0;
			_21 = 0; _22 = 1; _23 = 0; _24=0;
			_31 = 0; _32 = 0; _33 = 1; _34=0;
			return *this;
		}	

		matrix4x3& translation(value_type x,value_type y,value_type z)
		{
			_11 = 1; _12 = 0; _13 = 0; _14=x;
			_21 = 0; _22 = 1; _23 = 0; _24=y;
			_31 = 0; _32 = 0; _33 = 1; _34=z;
			return *this;
		}

		matrix4x3& translation(vector3 const& v)
		{
			_11 = 1; _12 = 0; _13 = 0; _14=v.x;
			_21 = 0; _22 = 1; _23 = 0; _24=v.y;
			_31 = 0; _32 = 0; _33 = 1; _34=v.z;
			return *this;
		}

		matrix4x3& scaling(value_type x,value_type y,value_type z)
		{
			_11 = x; _12 = 0; _13 = 0; _14=0;
			_21 = 0; _22 = y; _23 = 0; _24=0;
			_31 = 0; _32 = 0; _33 = z; _34=0;
			return *this;
		}

		matrix4x3& scaling(vector3 const& vt)
		{
			_11 = vt.x; _12 = 0; _13 = 0; _14=0;
			_21 = 0; _22 = vt.y; _23 = 0; _24=0;
			_31 = 0; _32 = 0; _33 = vt.z; _34=0;
			return *this;
		}

		matrix4x3& rotation(quaternion const& quat)
		{
			return mat34_rotation(*this,quat);
			return *this;
		}

		// perform rotation than translations
		matrix4x3& rot_trans(quaternion const& quat,vector3 const& vt)
		{
			return mat34_rot_trans(*this,quat,vt);
		}

		matrix4x3& rot_trans(transform_quat const& tr)
		{
			return rot_trans(tr.rotation,tr.position);
		}	

		// perform scaling than rotation than translations
		matrix4x3& scale_rot_trans(vector3 const& scale,quaternion const& quat,vector3 const& vt)
		{
			return mat34_scale_rot_trans(*this,scale,quat,vt);
		}
		matrix4x3& scale_rot_trans(float scale,quaternion const& quat,vector3 const& vt)
		{
			return mat34_scale_rot_trans(*this,scale,quat,vt);
		}

		matrix4x3& pre_scale(vector3 const& scale)
		{
			return mat33_pre_scale(*this,scale);
		}

		matrix4x3& pre_scale(float scale)
		{
			return mat33_pre_scale(*this,scale);
		}

		quaternion& build_quaternion(quaternion & quat)const
		{
			return 	mat33_build_quaternion(*this,quat);
		}
		quaternion build_quaternion()const{quaternion qt;return build_quaternion(qt);}

		vector3& get_position(vector3 & pos)const
		{
			pos.x = _14;
			pos.y = _24;
			pos.z = _34;
			return pos;
		}
		vector3 get_position()const{vector3 v;return get_position(v);}

		matrix4x3& build_muliplication(matrix4x3 const& mat,matrix4x3 & res)const
		{
			res._11 = _11*mat._11 + _21*mat._12 + _31*mat._13;
			res._21 = _11*mat._21 + _21*mat._22 + _31*mat._23; 
			res._31 = _11*mat._31 + _21*mat._32 + _31*mat._33;

			res._12 = _12*mat._11 + _22*mat._12 + _32*mat._13;
			res._22 = _12*mat._21 + _22*mat._22 + _32*mat._23; 
			res._32 = _12*mat._31 + _22*mat._32 + _32*mat._33;

			res._13 = _13*mat._11 + _23*mat._12 + _33*mat._13; 
			res._23 = _13*mat._21 + _23*mat._22 + _33*mat._23; 
			res._33 = _13*mat._31 + _23*mat._32 + _33*mat._33; 

			res._14 = _14*mat._11 + _24*mat._12 + _34*mat._13 + mat._14; 
			res._24 = _14*mat._21 + _24*mat._22 + _34*mat._23 + mat._24; 
			res._34 = _14*mat._31 + _24*mat._32 + _34*mat._33 + mat._34; 

			return res;
		}

		matrix4& build_muliplication(matrix4 const& mat,matrix4 & res)const
		{
			res._11 = _11*mat._11 + _21*mat._12 + _31*mat._13;
			res._12 = _12*mat._11 + _22*mat._12 + _32*mat._13;
			res._13 = _13*mat._11 + _23*mat._12 + _33*mat._13; 
			res._14 = _14*mat._11 + _24*mat._12 + _34*mat._13 + mat._14; 

			res._21 = _11*mat._21 + _21*mat._22 + _31*mat._23; 
			res._22 = _12*mat._21 + _22*mat._22 + _32*mat._23; 
			res._23 = _13*mat._21 + _23*mat._22 + _33*mat._23; 
			res._24 = _14*mat._21 + _24*mat._22 + _34*mat._23 + mat._24; 

			res._31 = _11*mat._31 + _21*mat._32 + _31*mat._33;
			res._32 = _12*mat._31 + _22*mat._32 + _32*mat._33;
			res._33 = _13*mat._31 + _23*mat._32 + _33*mat._33; 
			res._34 = _14*mat._31 + _24*mat._32 + _34*mat._33 + mat._34; 

			res._41 = _11*mat._41 + _21*mat._42 + _31*mat._43;
			res._42 = _12*mat._41 + _21*mat._42 + _31*mat._43;
			res._43 = _13*mat._41 + _21*mat._42 + _31*mat._43;
			res._44 = _14*mat._41 + _21*mat._42 + _31*mat._43 + mat._44;

			return res;
		}

		vector3& xform(vector3 const& vec, vector3& res)const
		{
			res.x = _11*vec.x+_12*vec.y+_13*vec.z+_14;
			res.y = _21*vec.x+_22*vec.y+_23*vec.z+_24;
			res.z = _31*vec.x+_32*vec.y+_33*vec.z+_34;
			return res;
		}
		vector3 xform(vector3 const& vec)const{vector3 vt;return xform(vec,vt);}

		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(matrix4x3)
	};
	GSYS_SET_TYPENAME(matrix4x3,"m43");

	matrix4x3 inline operator * (matrix4x3 const& m1,matrix4x3 const& m2)
	{
		matrix4x3 m;
		return m1.build_muliplication(m2,m);
	}
}

#endif