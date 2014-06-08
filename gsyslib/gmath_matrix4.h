
#pragma once

#include "gmath.h"
#include "g_vector3.h"

namespace gmath
{
	struct matrix4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14; 
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};
        // constructions
        matrix4(){}
        matrix4(float f11, float f12, float f13, float f14,
                float f21, float f22, float f23, float f24,
                float f31, float f32, float f33, float f34,
                float f41, float f42, float f43, float f44 )
        {
            _11 = f11; _12 = f12; _13 = f13; _14 = f14;
            _21 = f21; _22 = f22; _23 = f23; _24 = f24;
            _31 = f31; _32 = f32; _33 = f33; _34 = f34;
            _41 = f41; _42 = f42; _43 = f43; _44 = f44;
        }
        // access
        float& operator()(int x,int y){return m[x][y];}

        // initialisation
        // 
        //////////////////////////////////////////////////////////////
		void init(float f11, float f12, float f13, float f14,
                float f21, float f22, float f23, float f24,
                float f31, float f32, float f33, float f34,
                float f41, float f42, float f43, float f44 )
        {
            _11 = f11; _12 = f12; _13 = f13; _14 = f14;
            _21 = f21; _22 = f22; _23 = f23; _24 = f24;
            _31 = f31; _32 = f32; _33 = f33; _34 = f34;
            _41 = f41; _42 = f42; _43 = f43; _44 = f44;
        }

        void identity()
        {
            _11 = 1; _12 = 0; _13 = 0; _14 = 0;
            _21 = 0; _22 = 1; _23 = 0; _24 = 0;
            _31 = 0; _32 = 0; _33 = 1; _34 = 0;
            _41 = 0; _42 = 0; _43 = 0; _44 = 1;
        }
        void translation(float x,float y,float z)
        {
            _11 = 1; _12 = 0; _13 = 0; _14 = 0;
            _21 = 0; _22 = 1; _23 = 0; _24 = 0;
            _31 = 0; _32 = 0; _33 = 1; _34 = 0;
            _41 = x; _42 = y; _43 = z; _44 = 1;
        }
        void translation(vector3 const& vec){translation(vec.x,vec.y,vec.z);}
        void rotation_x(float a)
        {
            float cosa=cosf(a);
            float sina=sinf(a);
            _11 = 1;  _12 = 0;    _13 = 0; _14 = 0;
            _21 = 0; _22 =  cosa; _23 =-sina; _24 = 0;
            _31 = 0; _32 =  sina; _33 = cosa; _34 = 0;
            _41 = 0; _42 =  0;    _43 = 0; _44 = 1;
        }
        void rotation_y(float a)
        {
            float cosa=cosf(a);
            float sina=sinf(a);
            _11 = cosa; _12 = 0; _13 = sina; _14 = 0;
            _21 = 0; _22 = 1; _23 = 0; _24 = 0;
            _31 = -sina; _32 = 0; _33 = cosa; _34 = 0;
            _41 = 0; _42 = 0; _43 = 0; _44 = 1;
        }
        void rotation_z(float a)
        {
            float cosa=cosf(a);
            float sina=sinf(a);
            _11 = cosa; _12 = -sina; _13 = 0; _14 = 0;
            _21 = sina; _22 = cosa; _23 = 0; _24 = 0;
            _31 = 0; _32 = 0; _33 = 1; _34 = 0;
            _41 = 0; _42 = 0; _43 = 0; _44 = 1;
        }
        void scaling(float x,float y,float z)
        {
            _11 = x; _12 = 0; _13 = 0; _14 = 0;
            _21 = 0; _22 = y; _23 = 0; _24 = 0;
            _31 = 0; _32 = 0; _33 = z; _34 = 0;
            _41 = 0; _42 = 0; _43 = 0; _44 = 1;
        }
        void scaling(vector3 const& vec){scaling(vec.x,vec.y,vec.z);}
        void scaling(float a)
        {
            scaling(a,a,a);
        }
		// Used for view matrix
		matrix4& LookAtLH(vector3 const& Eye,vector3 const& At,vector3 const& Up)
		{
			vector3 za = vector3::normal(At-Eye);
			vector3 xa = vector3::normal(vector3::cross(Up,za));
			vector3 ya = vector3::cross(za,xa);

            init(xa.x,ya.x,za.x,0,
				 xa.y,ya.y,za.y,0,
				 xa.z,ya.z,za.z,0,
				 -vector3::dot(xa,Eye),-vector3::dot(ya,Eye),-vector3::dot(za,Eye),1);
			return *this;
		}
		matrix4& PerspectiveFovLH(float fov,float aspect,float zn,float zf)
		{
			float h = cosf(fov) / sinf(fov);
			float w = h / aspect;

			init(2*zn/w,0,0,0,
				 0,2*zn/h,0,0,
				 0,0,zf/(zf-zn),1,
				 0,0,zn*zf/(zn-zf),0);
			return *this;
		}
        ////////////////////////////////////////////////////////////////
        //operations

		void translate(float x,float y,float z)
		{
			// TODO optimise
			matrix4 m;
			m.translation(x,y,z);
			multiply(m);
		}
		void translate(vector3 v){translate(v.x,v.y,v.z);}
		

		void rotate_x(float f)
		{
			// TODO optimise
			matrix4 m;
			m.rotation_x(f);
			multiply(m);
		}
		void rotate_y(float f)
		{
			// TODO optimise
			matrix4 m;
			m.rotation_y(f);
			multiply(m);
		}
		void rotate_z(float f)
		{
			// TODO optimise
			matrix4 m;
			m.rotation_z(f);
			multiply(m);
		}

		void multiply(matrix4 const& mat)
		{
			 _11 = _11*mat._11 + _12*mat._21 + _13*mat._31 + _14*mat._41; 
			 _12 = _11*mat._12 + _12*mat._22 + _13*mat._32 + _14*mat._42; 
			 _13 = _11*mat._13 + _12*mat._23 + _13*mat._33 + _14*mat._43; 
			 _14 = _11*mat._14 + _12*mat._24 + _13*mat._34 + _14*mat._44;

			 _21 = _21*mat._11 + _22*mat._21 + _23*mat._31 + _24*mat._41; 
			 _22 = _21*mat._12 + _22*mat._22 + _23*mat._32 + _24*mat._42; 
			 _23 = _21*mat._13 + _22*mat._23 + _23*mat._33 + _24*mat._43; 
			 _24 = _21*mat._14 + _22*mat._24 + _23*mat._34 + _24*mat._44;

			 _31 = _31*mat._11 + _32*mat._21 + _33*mat._31 + _34*mat._41; 
			 _32 = _31*mat._12 + _32*mat._22 + _33*mat._32 + _34*mat._42; 
			 _33 = _31*mat._13 + _32*mat._23 + _33*mat._33 + _34*mat._43; 
			 _34 = _31*mat._14 + _32*mat._24 + _33*mat._34 + _34*mat._44;

			 _41 = _41*mat._11 + _42*mat._21 + _43*mat._31 + _44*mat._41; 
			 _42 = _41*mat._12 + _42*mat._22 + _43*mat._32 + _44*mat._42; 
			 _43 = _41*mat._13 + _42*mat._23 + _43*mat._33 + _44*mat._43; 
			 _44 = _41*mat._14 + _42*mat._24 + _43*mat._34 + _44*mat._44;
		}
		// 
		matrix4 & operator *=(matrix4 const& mat)
		{
			 multiply(mat);
			 return *this;
		}
		// produses invers matrix from current
		matrix4 inverse()
		{
			matrix4 mat;
			/////////////////////////////////////////////////////////
			/* Generated by Marple From code:
				with(LinearAlgebra);
				with(codegen);
				m:=Matrix([[_11, _12, _13, _14], [_21, _22, _23, _24], [_31, _32, _33, _34], [_41, _42, _43, _44]]);
				mi:=simplify(MatrixInverse(m));
				C(mi,optimized,ansi);
			*/
			float t3 = _43*_32;
			float t9 = _44*_32;
			float t14 = _21*_12;
			float t19 = _21*_42;
			float t24 = _21*_43;
			float t25 = _32*_14;
			float t27 = _21*_44;
			float t28 = _32*_13;
			float t30 = _12*_41;
			float t31 = _24*_33;
			float t33 = _23*_34;
			float t35 = _12*_43;
			float t36 = _31*_24;
			float t38 = _12*_44;
			float t39 = _31*_23;
			float t41 = _43*_11;
			float t42 = _22*_34;
			float t44 = _44*_31;
			float t45 = _13*_22;
			float t47 = t14*_43*_34-t14*_44*_33-t19*_13*_34+t19*_14*_33-t24*t25+t27*t28+t30*t31-t30*t33-t35*t36+t38*t39-t41*t42-t44*t45;
			float t48 = _41*_13;
			float t50 = _42*_11;
			float t52 = _41*_23;
			float t54 = _11*_24;
			float t56 = _43*_31;
			float t57 = _14*_22;
			float t59 = _41*_14;
			float t60 = _22*_33;
			float t62 = _44*_11;
			float t64 = _42*_14;
			float t67 = _42*_13;
			float t71 = _41*_24;
			float t73 = t48*t42+t50*t33+t52*t25+t3*t54+t56*t57-t59*t60+t62*t60-t64*t39-t50*t31+t67*t36-t62*_32*_23-t71*t28;
			float t75 = 1/(t47+t73);
			float t90 = _44*_13;
			float t92 = _43*_14;
			float t122 = _21*_14;
			float t124 = _21*_13;
			float t127 = _23*_11;
			float t136 = _13*_31;
			float t139 = _14*_31;
			float t145 = _31*_42;
			float t147 = _32*_21;
			float t150 = _34*_21;
			float t158 = _41*_32;
			float t176 = _32*_11;
			float t179 = _31*_12;
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
	};

	vector3 _inline operator * (vector3 const& vec, matrix4 const& mat)
	{
		float x = mat._11*vec.x+mat._21*vec.y+mat._31*vec.z+mat._41;
		float y = mat._21*vec.x+mat._22*vec.y+mat._32*vec.z+mat._42;
		float z = mat._31*vec.x+mat._23*vec.y+mat._33*vec.z+mat._43;
		return vector3(x,y,z);
	}
};


