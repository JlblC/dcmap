/*================================================================
	              Boxes in 3d classes
------------------------------------------------------------------


==================================================================*/

#pragma once
#include "g_gsys.h"
#include "g_matrix4.h"
#include "g_vector3.h"
#include "g_sphere.h"

namespace gsys
{
	// aabb class
	struct box3_aabb
	{
		vector3 vmin;
		vector3 vmax;
		/////////////////////////////////////////////////

		box3_aabb(){}
		box3_aabb(vector3 const& l_vmin,vector3 const& l_vmax):
			vmin(l_vmin),
			vmax(l_vmax)
		{
		}

		explicit box3_aabb(const vector3* points, gsys_uint n ): vmin(1e33f, 1e33f, 1e33f), vmax(-1e33f, -1e33f, -1e33f)
		{
			for(unsigned i=0;i<n;i++)Merge(points[i]);
		}
		explicit box3_aabb(const box3_aabb* boxes, gsys_uint n ): vmin(1e33f, 1e33f, 1e33f), vmax(-1e33f, -1e33f, -1e33f)
		{
			for(unsigned i=0;i<n;i++)Merge(boxes[i]);
		}

		vector3& GetCentroid(vector3& vec)const{vec = 0.5f*(vmin+vmax);return vec;}
		vector3 GetCentroid()const{return 0.5f*(vmin+vmax);}

		void invalidate()
		{
			vmin.set(1e33f, 1e33f, 1e33f);
			vmax.set(-1e33f, -1e33f, -1e33f);
		}

		void Merge(const box3_aabb& box)
		{
			Merge(box.vmin);
			Merge(box.vmax);
			/*
			vmin.x = min(vmin.x, box.vmin.x);
			vmin.y = min(vmin.y, box.vmin.y);
			vmin.z = min(vmin.z, box.vmin.z);
			vmax.x = max(vmax.x, box.vmax.x);
			vmax.y = max(vmax.y, box.vmax.y);
			vmax.z = max(vmax.z, box.vmax.z);*/
		}

		// Extend box, so it contains vertex
		void Merge(const vector3& vec)
		{
			vmin.x = min(vmin.x, vec.x);
			vmin.y = min(vmin.y, vec.y);
			vmin.z = min(vmin.z, vec.z);
			vmax.x = max(vmax.x, vec.x);
			vmax.y = max(vmax.y, vec.y);
			vmax.z = max(vmax.z, vec.z);
		}

		void Merge(const sphere& sp)
		{
			vmin.x = min(vmin.x, sp.center.x-sp.radius);
			vmin.y = min(vmin.y, sp.center.y-sp.radius);
			vmin.z = min(vmin.z, sp.center.z-sp.radius);
			vmax.x = max(vmax.x, sp.center.x+sp.radius);
			vmax.y = max(vmax.y, sp.center.y+sp.radius);
			vmax.z = max(vmax.z, sp.center.z+sp.radius);
		}
		vector3 point(int i)const{return vector3((i&1)?vmin.x:vmax.x,(i&2)?vmin.y:vmax.y,(i&4)?vmin.z:vmax.z);}

	};

	inline box3_aabb& transform_coord(box3_aabb const& box,matrix4 const& mat, box3_aabb& res)
	{
		res.invalidate();
		for(int i=0;i<8;i++)
			res.Merge(transform_coord(box.point(i),mat));
		return res;
	}
	inline box3_aabb& transform(box3_aabb const& box,matrix4 const& mat, box3_aabb& res)
	{
		res.invalidate();
		for(int i=0;i<8;i++)
			res.Merge(transform(box.point(i),mat).xyz_ref());
		return res;
	}

	inline box3_aabb transform(box3_aabb const& box,matrix4 const& mat){box3_aabb lbox;return transform(box,mat,lbox);}

//	typedef box3_aabb bbox3;
//	typedef box3_aabb box;
}