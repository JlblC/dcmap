#pragma once
#ifndef G_MATRIX3X4_H
#define G_MATRIX3X4_H

#include "g_gsys.h"
#include "g_alg.h"
#include "g_vector3.h"
#include "g_vector4.h"
#include "g_quaternion.h"
#include "g_text.h"
#include "g_base_matrix.h"
#include "g_matrix3.h"
#include "g_matrix4x3.h"
#include "g_matrix_operations.h"

namespace gsys
{
	struct matrix3x4
	{
		enum layout
		{
			l_11, l_12, l_13,
			l_21, l_22, l_23,
			l_31, l_32, l_33,
			l_41, l_42, l_43,			
			l_width=3,
			l_height=4,
			l_transpozed=0,
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
				value_type _11, _12, _13;
				value_type _21, _22, _23;
				value_type _31, _32, _33;
				value_type _41, _42, _43;
			};
			value_type m[3][4];
			value_type values[3*4];
		};
		size_type size()const{return 3*4;}
		reference operator()(size_type x,size_type y){return m[x][y];}
		reference operator[](size_type x){GSYS_ASSERT(x<size());return values[x];}
		const_reference operator()(size_type x,size_type y)const{return m[x][y];}
		const_reference operator[](size_type x)const{GSYS_ASSERT(x<size());return values[x];}
		/*====================================================================================*\
		Data Access
		\*====================================================================================*/

		matrix3& as_matrix3t(){return *(matrix3*)values;}
		matrix3 const& as_matrix3t()const{return *(matrix3 const*)values;}

		vector3& as_transpose(){return *(vector3*)&_41;}
		vector3 const& as_transpose()const{return *(vector3 const*)&_41;}

		/*====================================================================================*\
		Initialisations
		\*====================================================================================*/
		matrix3x4& init(value_type f11, value_type f12, value_type f13,
					  value_type f21, value_type f22, value_type f23,
					  value_type f31, value_type f32, value_type f33,
					  value_type f41, value_type f42, value_type f43)
		{
			_11 = f11; _12 = f12; _13 = f13;
			_21 = f21; _22 = f22; _23 = f23;
			_31 = f31; _32 = f32; _33 = f33;
			_41 = f41; _42 = f42; _43 = f43;
			return *this;
		}

		matrix3x4& identity()
		{
			_11 = 1; _12 = 0; _13 = 0;
			_21 = 0; _22 = 1; _23 = 0;
			_31 = 0; _32 = 0; _33 = 1;
			_41 = 0; _42 = 0; _43 = 0;
			return *this;
		}
		matrix3x4& translation(value_type x,value_type y,value_type z)
		{
			_11 = 1; _12 = 0; _13 = 0;
			_21 = 0; _22 = 1; _23 = 0;
			_31 = 0; _32 = 0; _33 = 1;
			_41 = x; _42 = y; _43 = z;
			return *this;
		}

		matrix3x4& rotation(quaternion const& quat)
		{
			return mat34_rotation(*this,quat);
		}

		matrix3x4& rot_trans(quaternion const& quat,vector3 const& vt)
		{
			return mat34_rot_trans(*this,quat,vt);
		}

		matrix3x4& scale_rot_trans(vector3 const& scale,quaternion const& quat,vector3 const& vt)
		{
			return mat34_scale_rot_trans(*this,scale,quat,vt);
		}

		matrix3x4& scale_rot_trans(float scale,quaternion const& quat,vector3 const& vt)
		{
			return mat34_scale_rot_trans(*this,scale,quat,vt);
		}



		/*====================================================================================*\
		Procuctions - builds objects from matrix
		\*====================================================================================*/
		matrix4x3& build_transpose(matrix4x3 & res)const
		{
			for(int i=0;i<3;i++)	
				for(int j=0;j<4;j++)
					res.m[j][i] = m[i][j];
			return res;		
		}


		matrix3x4& build_muliplication(matrix3x4 const& mat,matrix3x4 & res)const
		{
			res._11 = _11*mat._11 + _12*mat._21 + _13*mat._31; 
			res._12 = _11*mat._12 + _12*mat._22 + _13*mat._32; 
			res._13 = _11*mat._13 + _12*mat._23 + _13*mat._33; 

			res._21 = _21*mat._11 + _22*mat._21 + _23*mat._31; 
			res._22 = _21*mat._12 + _22*mat._22 + _23*mat._32; 
			res._23 = _21*mat._13 + _22*mat._23 + _23*mat._33; 

			res._31 = _31*mat._11 + _32*mat._21 + _33*mat._31; 
			res._32 = _31*mat._12 + _32*mat._22 + _33*mat._32; 
			res._33 = _31*mat._13 + _32*mat._23 + _33*mat._33; 

			res._41 = _41*mat._11 + _42*mat._21 + _43*mat._31; 
			res._42 = _41*mat._12 + _42*mat._22 + _43*mat._32; 
			res._43 = _41*mat._13 + _42*mat._23 + _43*mat._33; 

			return res;
		}

		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(matrix3x4)
	};
	GSYS_SET_TYPENAME(matrix3x4,"m34");


	struct matrix3tx4
	{
		enum layout
		{
			l_11, l_21, l_31,
			l_12, l_22, l_32,
			l_13, l_23, l_33,

			l_41, l_42, l_43,			
			l_width=3,
			l_height=4,
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
				value_type _11, _12, _13;
				value_type _21, _22, _23;
				value_type _31, _32, _33;
				value_type _41, _42, _43;
			};
			value_type m[3][4];
			value_type values[3*4];
		};
		size_type size()const{return 3*4;}
		reference operator()(size_type x,size_type y){return m[x][y];}
		reference operator[](size_type x){GSYS_ASSERT(x<size());return values[x];}
		const_reference operator()(size_type x,size_type y)const{return m[x][y];}
		const_reference operator[](size_type x)const{GSYS_ASSERT(x<size());return values[x];}
		/*====================================================================================*\
		Data Access
		\*====================================================================================*/
		
		matrix3t& as_matrix3t(){return *(matrix3t*)values;}
		matrix3t const& as_matrix3t()const{return *(matrix3t const*)values;}

		vector3& as_transpose(){return *(vector3*)&_41;}
		vector3 const& as_transpose()const{return *(vector3 const*)&_41;}

		/*====================================================================================*\
		Initialisations
		\*====================================================================================*/
		matrix3tx4& init(value_type f11, value_type f12, value_type f13,
			value_type f21, value_type f22, value_type f23,
			value_type f31, value_type f32, value_type f33,
			value_type f41, value_type f42, value_type f43)
		{
			_11 = f11; _12 = f12; _13 = f13;
			_21 = f21; _22 = f22; _23 = f23;
			_31 = f31; _32 = f32; _33 = f33;
			_41 = f41; _42 = f42; _43 = f43;
			return *this;
		}

		matrix3tx4& identity()
		{
			_11 = 1; _12 = 0; _13 = 0;
			_21 = 0; _22 = 1; _23 = 0;
			_31 = 0; _32 = 0; _33 = 1;
			_41 = 0; _42 = 0; _43 = 0;
			return *this;
		}
		matrix3tx4& translation(value_type x,value_type y,value_type z)
		{
			_11 = 1; _12 = 0; _13 = 0;
			_21 = 0; _22 = 1; _23 = 0;
			_31 = 0; _32 = 0; _33 = 1;
			_41 = x; _42 = y; _43 = z;
			return *this;
		}

		matrix3tx4& rotation(quaternion const& quat)
		{
			return mat34_rotation(*this,quat);
		}

		matrix3tx4& rot_trans(quaternion const& quat,vector3 const& vt)
		{
			return mat34_rot_trans(*this,quat,vt);
		}

		matrix3tx4& scale_rot_trans(vector3 const& scale,quaternion const& quat,vector3 const& vt)
		{
			return mat34_scale_rot_trans(*this,scale,quat,vt);
		}

		matrix3tx4& scale_rot_trans(float scale,quaternion const& quat,vector3 const& vt)
		{
			return mat34_scale_rot_trans(*this,scale,quat,vt);
		}

		/*====================================================================================*\
		Procuctions - builds objects from matrix
		\*====================================================================================*/
		matrix3x4& build_transpose(matrix3x4 & res)const
		{
			for(int i=0;i<3;i++)	
				for(int j=0;j<3;j++)
					res.m[j][i] = m[i][j];
			
			for(int j=0;j<3;j++)res.m[j][3] = m[j][3];			
			return res;		
		}

		matrix3tx4& build_muliplication(matrix3tx4 const& mat,matrix3tx4 & res)const
		{
			res._11 = _11*mat._11 + _12*mat._21 + _13*mat._31; 
			res._12 = _11*mat._12 + _12*mat._22 + _13*mat._32; 
			res._13 = _11*mat._13 + _12*mat._23 + _13*mat._33; 

			res._21 = _21*mat._11 + _22*mat._21 + _23*mat._31; 
			res._22 = _21*mat._12 + _22*mat._22 + _23*mat._32; 
			res._23 = _21*mat._13 + _22*mat._23 + _23*mat._33; 

			res._31 = _31*mat._11 + _32*mat._21 + _33*mat._31; 
			res._32 = _31*mat._12 + _32*mat._22 + _33*mat._32; 
			res._33 = _31*mat._13 + _32*mat._23 + _33*mat._33; 

			res._41 = _41*mat._11 + _42*mat._21 + _43*mat._31; 
			res._42 = _41*mat._12 + _42*mat._22 + _43*mat._32; 
			res._43 = _41*mat._13 + _42*mat._23 + _43*mat._33; 

			return res;
		}

		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(matrix3tx4)
	};
	GSYS_SET_TYPENAME(matrix3tx4,"m3t4");
}

#endif