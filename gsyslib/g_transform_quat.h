#pragma once
#ifndef G_TRANSFORM_QUAT_H
#define G_TRANSFORM_QUAT_H

#include "g_gsys.h"
#include "g_quaternion.h"
#include "g_vector3.h"
#include "g_matrix4.h"
#include "g_transform2d.h"

namespace gsys
{
	struct transform_quat
	{
/*====================================================================================*\
		Data Access
\*====================================================================================*/
		quaternion rotation;
		vector3   position;

/*====================================================================================*\
 Initialization 
\*====================================================================================*/
	transform_quat(){}

	transform_quat(vector3 pos,quaternion rot):
			position(pos),
			rotation(rot){}

	transform_quat(transform2_iang const& tr)
	{
		position.z=0;
		position.xy_ref() = tr.pos;
		quaternion qt;
		rotation.rotation_z(tr.iang.as_rad());
	}
	explicit transform_quat(matrix4 const& mat)
	{
		from_matrix(mat);
	}
	transform_quat& identity()
	{
		rotation.identity();
		position.set(0,0,0);
		return *this;
	}

	transform_quat& from_matrix(matrix4 const& mat)
	{
		mat.build_quaternion(rotation);
		mat.get_position(position);
		return *this;
	}
/*====================================================================================*\
		Operations
\*====================================================================================*/
		// Operations
		vector3& xform( vector3 const&v, vector3 &res)const
		{
			rotation.xform(v, res);
			res += position;
			return res;
		}
		vector3 xform(vector3 const& v)const{vector3 lv;return xform(v,lv);}
	
		matrix4& build_matrix4(matrix4 &res)const
		{
			res.rotation_quaternion(rotation);
			res.translate(position);
            return res;
		}
		matrix4 build_matrix4(void)const{matrix4 lm;return build_matrix4(lm);}
		operator matrix4()const{matrix4 lm;return build_matrix4(lm);}

		transform2_iang& make_t2i(transform2_iang &res)const
		{
			res.pos = position.xy_ref();
			vector3 vt;
			rotation.xform(gsys::vector3_UNIT_X,vt);
			res.iang.from_unitdir(vt.xy_ref().norm());
			return res;
		}
		transform2_iang make_t2i()const{transform2_iang lv;return make_t2i(lv);}

/*====================================================================================*\
		Standard Conversion
\*====================================================================================*/
		GSYSDETAIL_OSTREAM_STD_BASIC_OBJ(transform_quat,DO<<v.position<<D<<v.rotation<<DC);
		GSYSDETAIL_ISTREAM_STD_BASIC_OBJ(transform_quat,DO>>v.position>>D>>v.rotation>>DC);
		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(transform_quat);
	};
	GSYS_SET_TYPENAME(transform_quat,"trqt");
	
	// transform vector3 
	inline vector3 operator * (vector3 const& v,transform_quat const& t)
		{return t.xform(v);}

	// transform vector3 
	inline transform_quat operator * (transform_quat const& t1,transform_quat const& t2)
	{
		transform_quat q;
		q.rotation = t1.rotation * t2.rotation;
        q.position = t1.position + t1.rotation.xform(t2.position);
		return q;
	}

	inline transform_quat& slerp(transform_quat const& t1,transform_quat const& t2,
				float tm, transform_quat& res)
	{
		slerp(t1.rotation,t2.rotation,tm,res.rotation);
		lerp(t1.position,t2.position,tm,res.position);
		return res;
	}
	inline transform_quat slerp(transform_quat const& t1,transform_quat const& t2, gsys_real time)
		{transform_quat lt;return slerp(t1,t2,time,lt);}

}

#endif //G_TRANSFORM_QUAT_H
