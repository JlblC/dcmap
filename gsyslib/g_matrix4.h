#pragma once
#ifndef G_MATRIX_H
#define G_MATRIX_H

#include "g_gsys.h"
#include "g_matrix_operations.h"
#include "g_text.h"
#include "g_plane.h"

#pragma warning(push)
// '=' : conversion from 'double' to 'gsys::matrix4::value_type', possible loss of data
#pragma warning(disable : 4244 ) 

namespace gsys
{
	struct matrix4// : boost::multiplicative<matrix4>
	{
		enum layout
		{
			l_11, l_12, l_13, l_14,
			l_21, l_22, l_23, l_24,
			l_31, l_32, l_33, l_34,
			l_41, l_42, l_43, l_44,
			l_width=4,
			l_height=4,
			l_transpozed=0,
		};
/*====================================================================================*\
		types
\*====================================================================================*/
		typedef gsys_real value_type;
		typedef uint size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
		GSYSDETAIL_IMPLEMENT_ARRAY_CONTAINER;
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
				value_type _41, _42, _43, _44;
			};
			value_type m[4][4];
			value_type values[4*4];
		};
        reference operator()(size_type x,size_type y){return m[x][y];}
        reference operator[](size_type x){GSYS_ASSERT(x<4*4);return values[x];}
        const_reference operator()(size_type x,size_type y)const{return m[x][y];}
        const_reference operator[](size_type x)const{GSYS_ASSERT(x<4*4);return values[x];}
		size_type size()const{return 4*4;}
/*====================================================================================*\
		Constructions
\*====================================================================================*/
        // constructions
        matrix4(){}
        matrix4(value_type f11, value_type f12, value_type f13, value_type f14,
                value_type f21, value_type f22, value_type f23, value_type f24,
                value_type f31, value_type f32, value_type f33, value_type f34,
				value_type f41, value_type f42, value_type f43, value_type f44 ):
				_11(f11),_12(f12),_13(f13),_14(f14),
				_21(f21),_22(f22),_23(f23),_24(f24),
				_31(f31),_32(f32),_33(f33),_34(f34),
				_41(f41),_42(f42),_43(f43),_44(f44)
        {
		}
		matrix4(vector4 const& v1,vector4 const& v2,vector4 const& v3,vector4 const& v4)
		{	
			init(v1.x,v1.y,v1.z,v1.w,
				v2.x,v2.y,v2.z,v2.w,
				v3.x,v3.y,v3.z,v3.w,
				v4.x,v4.y,v4.z,v4.w);
		}

        matrix4(matrix4 const& m )
        {
#if 1
			memcpy(values,m.values,sizeof(value_type)*4*4);
#else
            _11 = m._11; _12 = m._12; _13 = m._13; _14 = m._14;
            _21 = m._21; _22 = m._22; _23 = m._23; _24 = m._24;
            _31 = m._31; _32 = m._32; _33 = m._33; _34 = m._34;
            _41 = m._41; _42 = m._42; _43 = m._43; _44 = m._44;
#endif
        }
		matrix4(quaternion const& quat){quaternion_rotation(quat);}
 
		matrix4 & operator = (matrix4 const& m )
		{
            _11 = m._11; _12 = m._12; _13 = m._13; _14 = m._14;
            _21 = m._21; _22 = m._22; _23 = m._23; _24 = m._24;
            _31 = m._31; _32 = m._32; _33 = m._33; _34 = m._34;
            _41 = m._41; _42 = m._42; _43 = m._43; _44 = m._44;
			return *this;
		}

/*====================================================================================*\
		Initialisations
\*====================================================================================*/
		matrix4& init(value_type f11, value_type f12, value_type f13, value_type f14,
                value_type f21, value_type f22, value_type f23, value_type f24,
                value_type f31, value_type f32, value_type f33, value_type f34,
                value_type f41, value_type f42, value_type f43, value_type f44 )
        {
            _11 = f11; _12 = f12; _13 = f13; _14 = f14;
            _21 = f21; _22 = f22; _23 = f23; _24 = f24;
            _31 = f31; _32 = f32; _33 = f33; _34 = f34;
            _41 = f41; _42 = f42; _43 = f43; _44 = f44;
			return *this;
        }

        matrix4& identity()
        {
            _11 = 1; _12 = 0; _13 = 0; _14 = 0;
            _21 = 0; _22 = 1; _23 = 0; _24 = 0;
            _31 = 0; _32 = 0; _33 = 1; _34 = 0;
            _41 = 0; _42 = 0; _43 = 0; _44 = 1;
			return *this;
        }
        matrix4& translation(value_type x,value_type y,value_type z)
        {
            _11 = 1; _12 = 0; _13 = 0; _14 = 0;
            _21 = 0; _22 = 1; _23 = 0; _24 = 0;
            _31 = 0; _32 = 0; _33 = 1; _34 = 0;
            _41 = x; _42 = y; _43 = z; _44 = 1;
			return *this;
        }
        matrix4& translation(vector3 const& vec){return translation(vec.x,vec.y,vec.z);}
        
		matrix4& rotation(value_type x,value_type y,value_type z)
		{
			//TODO: Optimise !!!
			identity();
			if(x) rotate_x(x);
			if(y) rotate_y(y);
			if(z) rotate_z(z);
			return *this;
		}		
		matrix4& rotation(vector3 const& v)
		{return rotation(v.x,v.y,v.z);}
		
		matrix4& rotation_x(value_type a)
        {
            value_type cosa=cosf(a);
            value_type sina=sinf(a);
            _11 = 1;  _12 = 0;    _13 = 0; _14 = 0;
            _21 = 0; _22 =  cosa; _23 =-sina; _24 = 0;
            _31 = 0; _32 =  sina; _33 = cosa; _34 = 0;
            _41 = 0; _42 =  0;    _43 = 0; _44 = 1;
			return *this;
        }
        matrix4& rotation_y(value_type a)
        {
            value_type cosa=cosf(a);
            value_type sina=sinf(a);
            _11 = cosa;  _12 = 0; _13 = sina; _14 = 0;
            _21 = 0;     _22 = 1; _23 = 0;    _24 = 0;
            _31 = -sina; _32 = 0; _33 = cosa; _34 = 0;
            _41 = 0;     _42 = 0; _43 = 0;    _44 = 1;
			return *this;
        }
        matrix4& rotation_z(value_type a)
        {
            value_type cosa=cosf(a);
            value_type sina=sinf(a);
            _11 = cosa; _12 = -sina; _13 = 0; _14 = 0;
            _21 = sina; _22 = cosa; _23 = 0; _24 = 0;
            _31 = 0; _32 = 0; _33 = 1; _34 = 0;
            _41 = 0; _42 = 0; _43 = 0; _44 = 1;
			return *this;
        }
        matrix4& scaling(value_type x,value_type y,value_type z)
        {
            _11 = x; _12 = 0; _13 = 0; _14 = 0;
            _21 = 0; _22 = y; _23 = 0; _24 = 0;
            _31 = 0; _32 = 0; _33 = z; _34 = 0;
            _41 = 0; _42 = 0; _43 = 0; _44 = 1;
			return *this;
        }
        matrix4& scaling(vector3 const& vec){return scaling(vec.x,vec.y,vec.z);}
        matrix4& scaling(value_type a){return scaling(a,a,a);}
		matrix4& rotation(quaternion const& quat)
		{
			m[0][3] = 0.0; 
			m[1][3] = 0.0;
			m[2][3] = 0.0; 

			m[3][0] = 0.0;
			m[3][1] = 0.0;
			m[3][2] = 0.0;

			m[3][3] = 1.0;
			
			return mat33_rotation(*this,quat);
/*
			value_type wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2; 
			// calculate coefficients
			x2 = quat.x + quat.x; y2 = quat.y + quat.y; 
			z2 = quat.z + quat.z;
			xx = quat.x * x2; xy = quat.x * y2; xz = quat.x * z2;
			yy = quat.y * y2; yz = quat.y * z2; zz = quat.z * z2;
			wx = quat.w * x2; wy = quat.w * y2; wz = quat.w * z2;
			m[0][0] = 1.0 - (yy + zz); m[1][0] = xy - wz;
			m[2][0] = xz + wy; m[3][0] = 0.0;

			m[0][1] = xy + wz; m[1][1] = 1.0 - (xx + zz);
			m[2][1] = yz - wx; m[3][1] = 0.0;

			m[0][2] = xz - wy; m[1][2] = yz + wx;
			m[2][2] = 1.0 - (xx + yy); m[3][2] = 0.0;

			// fill remaiding 
			m[0][3] = 0.0; m[1][3] = 0.0;
			m[2][3] = 0.0; m[3][3] = 1.0;
			return *this;
*/
		}
		matrix4& rotation_quaternion(quaternion const& quat){return rotation(quat);}
		matrix4& quaternion_rotation(quaternion const& quat){return rotation_quaternion(quat);}

		// Used for view matrix
		matrix4& LookAtLH(vector3 const& Eye,vector3 const& At,vector3 const& Up)
		{
			vector3 za = (At-Eye).norm();
			vector3 xa = cross(Up,za).norm();
			vector3 ya = cross(za,xa);

            init(xa.x,ya.x,za.x,0,
				 xa.y,ya.y,za.y,0,
				 xa.z,ya.z,za.z,0,
				 -dot(xa,Eye),-dot(ya,Eye),-dot(za,Eye),1);
			return *this;
		}
		matrix4& LookAtRH(vector3 const& Eye,vector3 const& At,vector3 const& Up)
		{
			vector3 za = (Eye-At).norm();
			vector3 xa = cross(Up,za).norm();
			vector3 ya = cross(za,xa);

			init(xa.x,ya.x,za.x,0,
				xa.y,ya.y,za.y,0,
				xa.z,ya.z,za.z,0,
				-dot(xa,Eye),-dot(ya,Eye),-dot(za,Eye),1);

			return *this;
		}
		matrix4& PerspectiveFovLH(value_type fov,value_type aspect,value_type zn,value_type zf)
		{
			value_type h = cosf(fov/2)/sinf(fov/2);
			value_type w = h / aspect;

			value_type q = zf/(zf-zn);

			init(w,0,0,0,
				 0,h,0,0,
				 0,0,q,1,
				 0,0,-zn*q,0);

			return *this;
		}

		matrix4& PerspectiveFovRH(value_type fov,value_type aspect,value_type zn,value_type zf)
		{
			value_type h = cosf(fov/2)/sinf(fov/2);
			value_type w = h / aspect;

			value_type q = zf/(zn-zf);

			init(w,0,0,0,
				0,h,0,0,
				0,0,q,-1,
				0,0,zn*q,0);

			return *this;
		}

		matrix4& PerspectiveLH(value_type w,value_type h,value_type zn,value_type zf)
		{
			init(2*zn/w,0,0,0,
				0,2*zn/h,0,0,
				0,0,zf/(zf-zn),1,
				0,0,zn*zf/(zn-zf),0);
			return *this;
		}		
		matrix4& PerspectiveRH(value_type w,value_type h,value_type zn,value_type zf)
		{
			init(2*zn/w,0,0,0,
				0,2*zn/h,0,0,
				0,0,zf/(zn-zf),-1,
				0,0,zn*zf/(zn-zf),0);
			return *this;
		}	

		matrix4& OrthoLH(value_type w,value_type h,value_type zn,value_type zf)
		{
			init(	2/w,  0,    0,           0,
					0,    2/h,  0,           0,
					0,    0,    1/(zf-zn),   0,
					0,    0,    zn/(zn-zf),  1);
			return *this;
		}

		matrix4& OrthoOffCenterLH(value_type l,value_type r,value_type b,value_type t,value_type zn,value_type zf)
		{
			init(2/(r-l),  0,    0,           0,
				0,    2/(t-b),  0,           0,
				0,    0,    1/(zf-zn),   0,
				(l+r)/(l-r),    (t+b)/(b-t),    zn/(zn-zf),  1);
			return *this;
		}

		matrix4& reflection(gsys::plane &P)
		{
			init(
				-2 * P.a * P.a + 1,  -2 * P.b * P.a,      -2 * P.c * P.a,        0,
				-2 * P.a * P.b,      -2 * P.b * P.b + 1,  -2 * P.c * P.b,        0,
				-2 * P.a * P.c,      -2 * P.b * P.c,      -2 * P.c * P.c + 1,    0,
				-2 * P.a * P.d,      -2 * P.b * P.d,      -2 * P.c * P.d,        1
				);
			return *this;
		}

/*====================================================================================*\
		Modifications
\*====================================================================================*/
		matrix4& translate(value_type x,value_type y,value_type z)
		{
			// TODO optimise
			matrix4 m;
			m.translation(x,y,z);
			multiply(m);
			return *this;
		}
		matrix4& translate(vector3 v){return translate(v.x,v.y,v.z);}

		matrix4& rotate_quaternion(quaternion const& quat)
		{
			matrix4 m;
			m.rotation_quaternion(quat);
			multiply(m);
			return *this;
		}

		matrix4& rotate(value_type x,value_type y,value_type z)
		{
			//TODO: Optimise !!!
			if(x) rotate_x(x);
			if(y) rotate_y(y);
			if(z) rotate_z(z);
			return *this;
		}		
		matrix4& rotate(vector3 const& v)
		{return rotate(v.x,v.y,v.z);}

		matrix4& rotate_x(value_type f)
		{
			value_type t1 = cosf(f);
			value_type t3 = sinf(f);
			value_type t;
			t = _12*t1+_13*t3;
			_13 = -_12*t3+_13*t1;
			_12=t;
			t = _22*t1+_23*t3;
			_23 = -_22*t3+_23*t1;
			_22=t;
			t = _32*t1+_33*t3;
			_33 = -_32*t3+_33*t1;
			_32=t;
			t = _42*t1+_43*t3;
			_43 = -_42*t3+_43*t1;
			_42=t;
			
			/*matrix4 m;
			m.rotation_x(f);
			multiply(m);*/
			return *this;
		}
		matrix4& rotate_y(value_type f)
		{
			value_type t1 = cos(f);
			value_type t3 = sin(f);
			value_type t;

			 t= _11*t1-_13*t3;
			_13 = _11*t3+_13*t1;
			_11=t;

			 t = _21*t1-_23*t3;
			_23 = _21*t3+_23*t1;
			_21=t;

			t = _31*t1-_33*t3;
			_33 = _31*t3+_33*t1;
			_31=t;

			t = _41*t1-_43*t3;
			_43 = _41*t3+_43*t1;
			_41=t;

/*			matrix4 m;
			m.rotation_y(f);
			multiply(m);
*/
			return *this;
		}
		matrix4& rotate_z(value_type f)
		{
			// TODO optimise
			matrix4 m;
			m.rotation_z(f);
			multiply(m);
			return *this;
		}
		matrix4& scale(value_type f)
		{
			// TODO optimise
			matrix4 m;
			m.scaling(f);
			multiply(m);
			return *this;
		}
		matrix4& scale(value_type x,value_type y,value_type z)
		{
			// TODO optimise
			matrix4 m;
			m.scaling(x,y,z);
			multiply(m);
			return *this;
		}
		matrix4& transpose()
		{
			matrix4 tm = *this;

            for(int i=0;i<4;i++)	
				for(int j=0;j<4;j++)
					m[i][j] = tm.m[j][i];
			return *this;
		}

		matrix4& multiply(matrix4 const& mat)
		{
			 matrix4 m;
			 build_muliplication(mat,m);
			 *this = m;
			 return *this;
		}
		// 
		matrix4 & operator *=(matrix4 const& mat)
		{
			 multiply(mat);
			 return *this;
		}
/*====================================================================================*\
		Procuctions - bilds objects from matrix
\*====================================================================================*/

		matrix4& build_transpose(matrix4& tm)const
		{
			for(int i=0;i<4;i++)	
				for(int j=0;j<4;j++)
					tm.m[i][j] = m[j][i];
			return tm;
		}

		matrix4 build_transpose()const
		{
			matrix4 tm;
			return build_transpose(tm);
		}

		// produses invers matrix from current
		matrix4 & build_inverse(matrix4 & mat) const
		{
			/////////////////////////////////////////////////////////
			/* Generated by Marple From code:
				with(LinearAlgebra);
				with(codegen);
				m:=Matrix([[_11, _12, _13, _14], [_21, _22, _23, _24], [_31, _32, _33, _34], [_41, _42, _43, _44]]);
				mi:=simplify(MatrixInverse(m));
				C(mi,optimized,ansi);
			*/
			value_type t3 = _43*_32;
			value_type t9 = _44*_32;
			value_type t14 = _21*_12;
			value_type t19 = _21*_42;
			value_type t24 = _21*_43;
			value_type t25 = _32*_14;
			value_type t27 = _21*_44;
			value_type t28 = _32*_13;
			value_type t30 = _12*_41;
			value_type t31 = _24*_33;
			value_type t33 = _23*_34;
			value_type t35 = _12*_43;
			value_type t36 = _31*_24;
			value_type t38 = _12*_44;
			value_type t39 = _31*_23;
			value_type t41 = _43*_11;
			value_type t42 = _22*_34;
			value_type t44 = _44*_31;
			value_type t45 = _13*_22;
			value_type t47 = t14*_43*_34-t14*_44*_33-t19*_13*_34+t19*_14*_33-t24*t25+t27*t28+t30*t31-t30*t33-t35*t36+t38*t39-t41*t42-t44*t45;
			value_type t48 = _41*_13;
			value_type t50 = _42*_11;
			value_type t52 = _41*_23;
			value_type t54 = _11*_24;
			value_type t56 = _43*_31;
			value_type t57 = _14*_22;
			value_type t59 = _41*_14;
			value_type t60 = _22*_33;
			value_type t62 = _44*_11;
			value_type t64 = _42*_14;
			value_type t67 = _42*_13;
			value_type t71 = _41*_24;
			value_type t73 = t48*t42+t50*t33+t52*t25+t3*t54+t56*t57-t59*t60+t62*t60-t64*t39-t50*t31+t67*t36-t62*_32*_23-t71*t28;
			value_type t75 = 1/(t47+t73);
			value_type t90 = _44*_13;
			value_type t92 = _43*_14;
			value_type t122 = _21*_14;
			value_type t124 = _21*_13;
			value_type t127 = _23*_11;
			value_type t136 = _13*_31;
			value_type t139 = _14*_31;
			value_type t145 = _31*_42;
			value_type t147 = _32*_21;
			value_type t150 = _34*_21;
			value_type t158 = _41*_32;
			value_type t176 = _32*_11;
			value_type t179 = _31*_12;
			mat.m[0][0] = -(-_44*_22*_33-t3*_24+_42*_24*_33-_42*_23*_34+t9*_23+_43*_22*_34)*t75;
			mat.m[0][1] = -(-t35*_34+t38*_33-t64*_33+t3*_14-t9*_13+t67*_34)*t75;
			mat.m[0][2] = -(_24*_43*_12-_23*_12*_44-t67*_24+t90*_22-t92*_22+t64*_23)*t75;
			mat.m[0][3] = (-t33*_12+t31*_12+t45*_34-_24*_32*_13-t57*_33+t25*_23)*t75;
			mat.m[1][0] = (t24*_34-t27*_33-t56*_24+t44*_23-t52*_34+t71*_33)*t75;
			mat.m[1][1] = -(t41*_34-t62*_33+t90*_31-t48*_34-t92*_31+t59*_33)*t75;
			mat.m[1][2] = (-t122*_43+t124*_44+t54*_43-t127*_44+t59*_23-t48*_24)*t75;
			mat.m[1][3] = -(t124*_34-t122*_33-t127*_34-t136*_24+t54*_33+t139*_23)*t75;
			mat.m[2][0] = -(_31*_22*_44-t145*_24-t147*_44-t42*_41+t150*_42+t71*_32)*t75;
			mat.m[2][1] = (-t30*_34+t38*_31+t50*_34+t158*_14-t64*_31-t62*_32)*t75;
			mat.m[2][2] = (-t14*_44+t30*_24+t19*_14-t50*_24+t62*_22-t59*_22)*t75;
			mat.m[2][3] = -(_34*_11*_22-t150*_12-t139*_22-t176*_24+t147*_14+t179*_24)*t75;
			mat.m[3][0] = (t19*_33-t24*_32+t52*_32+t56*_22-_41*_22*_33-t145*_23)*t75;
			mat.m[3][1] = (t30*_33-t35*_31-t158*_13-t50*_33+t67*_31+t3*_11)*t75;
			mat.m[3][2] = -(t41*_22-t24*_12-t48*_22-t50*_23+t19*_13+t30*_23)*t75;
			mat.m[3][3] = t75*(_33*_11*_22-_33*_21*_12-t136*_22-t176*_23+t147*_13+t179*_23);
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			return mat;
		}

		matrix4& build_muliplication(matrix4 const& mat,matrix4 & res)const
		{
			res._11 = _11*mat._11 + _12*mat._21 + _13*mat._31 + _14*mat._41; 
			res._12 = _11*mat._12 + _12*mat._22 + _13*mat._32 + _14*mat._42; 
			res._13 = _11*mat._13 + _12*mat._23 + _13*mat._33 + _14*mat._43; 
			res._14 = _11*mat._14 + _12*mat._24 + _13*mat._34 + _14*mat._44;

			res._21 = _21*mat._11 + _22*mat._21 + _23*mat._31 + _24*mat._41; 
			res._22 = _21*mat._12 + _22*mat._22 + _23*mat._32 + _24*mat._42; 
			res._23 = _21*mat._13 + _22*mat._23 + _23*mat._33 + _24*mat._43; 
			res._24 = _21*mat._14 + _22*mat._24 + _23*mat._34 + _24*mat._44;

			res._31 = _31*mat._11 + _32*mat._21 + _33*mat._31 + _34*mat._41; 
			res._32 = _31*mat._12 + _32*mat._22 + _33*mat._32 + _34*mat._42; 
			res._33 = _31*mat._13 + _32*mat._23 + _33*mat._33 + _34*mat._43; 
			res._34 = _31*mat._14 + _32*mat._24 + _33*mat._34 + _34*mat._44;

			res._41 = _41*mat._11 + _42*mat._21 + _43*mat._31 + _44*mat._41; 
			res._42 = _41*mat._12 + _42*mat._22 + _43*mat._32 + _44*mat._42; 
			res._43 = _41*mat._13 + _42*mat._23 + _43*mat._33 + _44*mat._43; 
			res._44 = _41*mat._14 + _42*mat._24 + _43*mat._34 + _44*mat._44;
			return res;
		}
		matrix4& build_muliplication(matrix4 const& mat)const
			{matrix4 tm;return build_muliplication(mat,tm);}

		matrix4& build_transpose_muliplication(matrix4 const& mat,matrix4 & res)const
		{
			res._11 = _11*mat._11 + _12*mat._21 + _13*mat._31 + _14*mat._41; 
			res._21 = _11*mat._12 + _12*mat._22 + _13*mat._32 + _14*mat._42; 
			res._31 = _11*mat._13 + _12*mat._23 + _13*mat._33 + _14*mat._43; 
			res._41 = _11*mat._14 + _12*mat._24 + _13*mat._34 + _14*mat._44;

			res._12 = _21*mat._11 + _22*mat._21 + _23*mat._31 + _24*mat._41; 
			res._22 = _21*mat._12 + _22*mat._22 + _23*mat._32 + _24*mat._42; 
			res._32 = _21*mat._13 + _22*mat._23 + _23*mat._33 + _24*mat._43; 
			res._42 = _21*mat._14 + _22*mat._24 + _23*mat._34 + _24*mat._44;

			res._13 = _31*mat._11 + _32*mat._21 + _33*mat._31 + _34*mat._41; 
			res._23 = _31*mat._12 + _32*mat._22 + _33*mat._32 + _34*mat._42; 
			res._33 = _31*mat._13 + _32*mat._23 + _33*mat._33 + _34*mat._43; 
			res._43 = _31*mat._14 + _32*mat._24 + _33*mat._34 + _34*mat._44;

			res._14 = _41*mat._11 + _42*mat._21 + _43*mat._31 + _44*mat._41; 
			res._24 = _41*mat._12 + _42*mat._22 + _43*mat._32 + _44*mat._42; 
			res._34 = _41*mat._13 + _42*mat._23 + _43*mat._33 + _44*mat._43; 
			res._44 = _41*mat._14 + _42*mat._24 + _43*mat._34 + _44*mat._44;
			return res;
		}
		matrix4& build_transpose_muliplication(matrix4 const& mat)const
		{matrix4 tm;return build_transpose_muliplication(mat,tm);}

		matrix4 inverse()const
		{
			matrix4 mat;
			return build_inverse(mat);
		}

		vector3& get_position(vector3 & pos)const
		{GSYS_CFN;
			pos.x = _41;
			pos.y = _42;
			pos.z = _43;
			return pos;
		}
		vector3 get_position()const{vector3 v;return get_position(v);}

		quaternion& build_quaternion(quaternion & quat)const
		{
			return 	mat33_build_quaternion(*this,quat);

			value_type  tr, s, q[4];
			int    i, j, k;

			int nxt[3] = {1, 2, 0};

			tr = m[0][0] + m[1][1] + m[2][2];

			// check the diagonal
			if (tr > 0.0)
			{
				s = sqrt (tr + 1.0);
				quat.w = s / 2.0;
				s = 0.5 / s;
				quat.x = (m[1][2] - m[2][1]) * s;
				quat.y = (m[2][0] - m[0][2]) * s;
				quat.z = (m[0][1] - m[1][0]) * s;
			} 
			else 
			{		
				// diagonal is negative
    			i = 0;
				if (m[1][1] > m[0][0]) i = 1;
				if (m[2][2] > m[i][i]) i = 2;

				j = nxt[i];
				k = nxt[j];


				s = sqrt ((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
			      
				q[i] = s * 0.5f;
			            
				if (s != 0.0f) s = 0.5f / s;


				q[3] = (m[j][k] - m[k][j]) * s;
				q[j] = (m[i][j] + m[j][i]) * s;
				q[k] = (m[i][k] + m[k][i]) * s;

				quat.x = q[0];
				quat.y = q[1];
				quat.z = q[2];
				quat.w = q[3];
			}
			return quat;
		}
		quaternion build_quaternion()const{quaternion q;return build_quaternion(q);}
		operator quaternion()const{quaternion q;return build_quaternion(q);}

		quaternion& get_rotation(quaternion & quat)const{ return build_quaternion(quat);}

		/////////////////////////////////////////////////////
		// Data processing
		/////////////////////////////////////////////////////
		void transform_vertexes(vector3* data,size_t vertexSize,int count) const
		{
			matrix4 const& mat = *this;
			for(int i=0;i<count;i++)
			{
				vector3 & vec = *data;
				matrix4::value_type x = mat._11*vec.x+mat._21*vec.y+mat._31*vec.z+mat._41;
				matrix4::value_type y = mat._12*vec.x+mat._22*vec.y+mat._32*vec.z+mat._42;
				matrix4::value_type z = mat._13*vec.x+mat._23*vec.y+mat._33*vec.z+mat._43;
				matrix4::value_type w = mat._14*vec.x+mat._24*vec.y+mat._34*vec.z+mat._44;
				vec.set(x,y,z);
				data = reinterpret_cast<vector3*>(reinterpret_cast<char*>(data) + vertexSize);
			}
		}

		/////////////////////////////////////////////////////
		// Constants

		static const matrix4 _identity;

		static inline matrix4 const&  mat_identity()
		{
			return _identity;
		}
		
		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(matrix4)
	};
	GSYS_SET_TYPENAME(matrix4,"m4");

	const matrix4 matrix4_IDENTITY(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

	inline matrix4 matrix4_identity(){return matrix4().identity();}

	matrix4 inline operator * (matrix4 const& m1,matrix4 const& m2)
	{
        matrix4 m;
		return m1.build_muliplication(m2,m);
	}

	vector3 inline operator * (vector3 const& vec, matrix4 const& mat)
	{
		matrix4::value_type x = mat._11*vec.x + mat._21*vec.y + mat._31*vec.z + mat._41;
		matrix4::value_type y = mat._12*vec.x + mat._22*vec.y + mat._32*vec.z + mat._42;
		matrix4::value_type z = mat._13*vec.x + mat._23*vec.y + mat._33*vec.z + mat._43;
		return vector3(x,y,z);
	}
	vector3 inline transform_normal (vector3 const& vec, matrix4 const& mat)
	{
		matrix4::value_type x = mat._11*vec.x+mat._21*vec.y+mat._31*vec.z;
		matrix4::value_type y = mat._12*vec.x+mat._22*vec.y+mat._32*vec.z;
		matrix4::value_type z = mat._13*vec.x+mat._23*vec.y+mat._33*vec.z;
		return vector3(x,y,z);
	}

	vector3 inline transform_coord(vector3 const& vec, matrix4 const& mat)
	{
		matrix4::value_type x = mat._11*vec.x+mat._21*vec.y+mat._31*vec.z+mat._41;
		matrix4::value_type y = mat._12*vec.x+mat._22*vec.y+mat._32*vec.z+mat._42;
		matrix4::value_type z = mat._13*vec.x+mat._23*vec.y+mat._33*vec.z+mat._43;
		matrix4::value_type w = mat._14*vec.x+mat._24*vec.y+mat._34*vec.z+mat._44;
		matrix4::value_type recip = 1.0f/w;
		return vector3(x*recip,y*recip,z*recip);
	}

	vector4 inline transform(vector3 const& vec, matrix4 const& mat)
	{
		matrix4::value_type x = mat._11*vec.x+mat._21*vec.y+mat._31*vec.z+mat._41;
		matrix4::value_type y = mat._12*vec.x+mat._22*vec.y+mat._32*vec.z+mat._42;
		matrix4::value_type z = mat._13*vec.x+mat._23*vec.y+mat._33*vec.z+mat._43;
		matrix4::value_type w = mat._14*vec.x+mat._24*vec.y+mat._34*vec.z+mat._44;
		return vector4(x,y,z,w);
	}

	vector4 inline transform(vector4 const& vec, matrix4 const& mat)
	{
		matrix4::value_type x = mat._11*vec.x+mat._21*vec.y+mat._31*vec.z+mat._41*vec.w;
		matrix4::value_type y = mat._12*vec.x+mat._22*vec.y+mat._32*vec.z+mat._42*vec.w;
		matrix4::value_type z = mat._13*vec.x+mat._23*vec.y+mat._33*vec.z+mat._43*vec.w;
		matrix4::value_type w = mat._14*vec.x+mat._24*vec.y+mat._34*vec.z+mat._44*vec.w;
		return vector4(x,y,z,w);
	}

	inline std::ostream& operator<<(std::ostream& os,matrix4 const& v)
	{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
        stream_array<matrix4::value_type> ar(&v[0],v.size());
		return os << ar;
	}

	inline std::istream& operator>>(std::istream& is,matrix4 & v)
	{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
        stream_array<matrix4::value_type>  ar(&v[0],v.size());
		return is >> ar;
	}


};
#pragma warning(pop)


#endif
