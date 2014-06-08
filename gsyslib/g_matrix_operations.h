#pragma once
#ifndef G_MATRIX_OPERATIONS_H
#define G_MATRIX_OPERATIONS_H

#include "g_gsys.h"
#include "g_alg.h"
#include "g_vector3.h"
#include "g_vector4.h"
#include "g_quaternion.h"
#include "g_plane.h"

namespace gsys
{
	// universal 3d rotation by quaternion
	template<class mt>
	mt& mat33_pre_scale(mt& mat,vector3 const& scale)
	{
		mat[mt::l_11] *= scale.x; 
		mat[mt::l_21] *= scale.x;
		mat[mt::l_31] *= scale.x;

		mat[mt::l_12] *= scale.y; 
		mat[mt::l_22] *= scale.y;
		mat[mt::l_32] *= scale.y;

		mat[mt::l_13] *= scale.z; 
		mat[mt::l_23] *= scale.z;;
		mat[mt::l_33] *= scale.z;; 
		return mat;
	}
	template<class mt>
	mt& mat33_pre_scale(mt& mat,float scale)
	{
		mat[mt::l_11] *= scale; 
		mat[mt::l_21] *= scale;
		mat[mt::l_31] *= scale;

		mat[mt::l_12] *= scale; 
		mat[mt::l_22] *= scale;
		mat[mt::l_32] *= scale;

		mat[mt::l_13] *= scale; 
		mat[mt::l_23] *= scale;
		mat[mt::l_33] *= scale; 
		return mat;
	}


	template<class mt>
	mt& mat33_rotation(mt& mat,quaternion const& quat)
	{
		mt::value_type wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2; 
		// calculate coefficients
		x2 = quat.x + quat.x; y2 = quat.y + quat.y; 
		z2 = quat.z + quat.z;
		xx = quat.x * x2; xy = quat.x * y2; xz = quat.x * z2;
		yy = quat.y * y2; yz = quat.y * z2; zz = quat.z * z2;
		wx = quat.w * x2; wy = quat.w * y2; wz = quat.w * z2;
		mat[mt::l_11] = 1.0f - (yy + zz); 
		mat[mt::l_21] = xy - wz;
		mat[mt::l_31] = xz + wy;

		mat[mt::l_12] = xy + wz; 
		mat[mt::l_22] = 1.0f - (xx + zz);
		mat[mt::l_32] = yz - wx;

		mat[mt::l_13] = xz - wy; 
		mat[mt::l_23] = yz + wx;
		mat[mt::l_33] = 1.0f - (xx + yy); 

		return mat;
	}

	template<class mt>
	mt& mat34_rotation(mt & m,quaternion const& quat)
	{
		m[mt::l_41] = 0.0; 
		m[mt::l_42] = 0.0;
		m[mt::l_43] = 0.0; 
		return mat33_rotation(m,quat);
	}

	template<class mt>
	mt& mat34_rot_trans(mt & m,quaternion const& quat,vector3 const& vt)
	{
		mat33_rotation(m,quat);

		m[mt::l_41] = vt.x; 
		m[mt::l_42] = vt.y;
		m[mt::l_43] = vt.z; 

		return m;
	}

	template<class mt>
	mt& mat34_scale_rot_trans(mt & m,vector3 const& scale,quaternion const& quat,vector3 const& vt)
	{
		mat33_rotation(m,quat);
		mat33_pre_scale(m,scale);

		m[mt::l_41] = vt.x; 
		m[mt::l_42] = vt.y;
		m[mt::l_43] = vt.z; 

		return m;
	}
	template<class mt>
	mt& mat34_scale_rot_trans(mt & m,float scale,quaternion const& quat,vector3 const& vt)
	{
		mat33_rotation(m,quat);
		mat33_pre_scale(m,scale);

		m[mt::l_41] = vt.x; 
		m[mt::l_42] = vt.y;
		m[mt::l_43] = vt.z; 

		return m;
	}

	template<class mt>
	quaternion& mat33_build_quaternion(mt const& m,quaternion & quat)
	{
		typename mt::value_type  tr, s;

		tr = m[mt::l_11] + m[mt::l_22] + m[mt::l_33];
		// check the diagonal
		if (tr > 0.0)
		{
			s = sqrt (tr + 1.0f);
			quat.w = s / 2.0f;
			s = 0.5f / s;
			quat.x = (m[mt::l_23] - m[mt::l_32]) * s;
			quat.y = (m[mt::l_31] - m[mt::l_13]) * s;
			quat.z = (m[mt::l_12] - m[mt::l_21]) * s;
		} 
		else 
		{		
			int    i, j, k;
			const int nxt[3] = {1, 2, 0};

			// diagonal is negative
			i = 0;
			if (m(1,1) > m(0,0)) i = 1;
			if (m(2,2) > m(i,i)) i = 2;

			j = nxt[i];
			k = nxt[j];


			s = sqrt ((m(i,i) - (m(j,j) + m(k,k))) + 1.0f);

			quat[i] = s * 0.5f;

			if (s != 0.0f) s = 0.5f / s;

			if(mt::l_transpozed)
			{
				quat[3] = (m(k,j) - m(j,k)) * s;
			}
			else
			{
				quat[3] = (m(j,k) - m(k,j)) * s;
			}
			quat[j] = (m(j,i) + m(i,j)) * s;
			quat[k] = (m(k,i) + m(i,k)) * s;
		}
		return quat;
	}

	template<class tm1,class tm2>
	void mat_transfer(tm1 const& m1,tm2 & m2)
	{
		if(tm1::l_transpozed == tm2::l_transpozed)
		{
			int w = min((int)tm1::l_width,(int)tm2::l_width);
			int h = min((int)tm1::l_height,(int)tm2::l_height);

			for(int j=0;j<h;j++)
			{
				for(int i=0;i<w;i++) 
				{
					m2(i,j) = m1(i,j);
				}
			}
		}
		else
		{
			int w = min((int)tm1::l_width,(int)tm2::l_height);
			int h = min((int)tm1::l_height,(int)tm2::l_width);

			for(int j=0;j<h;j++)
			{
				for(int i=0;i<w;i++) 
				{
					m2(j,i) = m1(i,j);
				}
			}
		}
		// identity rest
	}

}
#endif