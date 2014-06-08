#pragma once
#ifndef G_MATRIX3_H
#define G_MATRIX3_H

#include "g_gsys.h"
#include "g_matrix_operations.h"
#include "g_text.h"

namespace gsys
{
	struct matrix3
	{
		// standard matrix layout
		enum layout
		{
			l_11, l_12, l_13,
			l_21, l_22, l_23,
			l_31, l_32, l_33,
			l_width=3,
			l_height=3,
			l_transpozed=0,
		};
		// 3d rotation matrix
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
				value_type _11, _12, _13;
				value_type _21, _22, _23;
				value_type _31, _32, _33;
			};
			value_type m[3][3];
			value_type values[3*3];
		};
		reference operator()(size_type x,size_type y){return m[x][y];}
		reference operator[](size_type x){GSYS_ASSERT(x<3*3);return values[x];}
		const_reference operator()(size_type x,size_type y)const{return m[x][y];}
		const_reference operator[](size_type x)const{GSYS_ASSERT(x<3*3);return values[x];}
		size_type size()const{return 3*3;}

		/*====================================================================================*\
		Initialisations
		\*====================================================================================*/

		matrix3& rotation(quaternion const& quat)
		{
			return mat33_rotation(*this,quat);
		}

		quaternion& build_quaternion(quaternion & quat)const
		{
			return mat33_build_quaternion(*this,quat);

			value_type  tr, s, q[4];
			int    i, j, k;

			int nxt[3] = {1, 2, 0};

			tr = m[0][0] + m[1][1] + m[2][2];

			// check the diagonal
			if (tr > 0.0)
			{
				s = sqrt (tr + 1.0f);
				quat.w = s / 2.0f;
				s = 0.5f / s;
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
	};
	GSYS_SET_TYPENAME(matrix3,"m3");


	struct matrix3t
	{
		// standard matrix layout
		enum layout
		{
			l_11, l_21, l_31,
			l_12, l_22, l_32,
			l_13, l_23, l_33,
			l_width=3,
			l_height=3,
			l_transpozed=1,
		};
		// 3d rotation matrix
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
			};
			value_type m[3][3];
			value_type values[3*3];
		};
		reference operator()(size_type x,size_type y){return m[x][y];}
		reference operator[](size_type x){GSYS_ASSERT(x<3*3);return values[x];}
		const_reference operator()(size_type x,size_type y)const{return m[x][y];}
		const_reference operator[](size_type x)const{GSYS_ASSERT(x<3*3);return values[x];}
		size_type size()const{return 3*3;}

		/*====================================================================================*\
		Initialisations
		\*====================================================================================*/

		matrix3t& rotation(quaternion const& quat)
		{
			return mat33_rotation(*this,quat);
		}
	};
	GSYS_SET_TYPENAME(matrix3t,"m3");

}
#endif