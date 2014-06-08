/*================================================================
	              Camera Frustum Class
------------------------------------------------------------------
class gsys::camera_frustrum_nb
	represents camera frustrum
	without clipping by near & far clip plames 

==================================================================*/
#pragma once
#include "g_gsys.h"
#include "g_assert.h"

#include "g_matrix4.h"
#include "g_vector3.h"
#include "g_plane.h"
#include "g_sphere.h"
#include "g_box3.h"
#include "g_rect.h"

namespace gsys
{
	class camera_frustum
	{
		void make_planes();
	public:
		vector3 m_corners[8];    // corners of the view frustum
		union
		{
			struct
			{
				plane m_planeLeft;
				plane m_planeRight;
				plane m_planeTop;
				plane m_planeBottom;
				///////////////////// - additionals(not used in some calculations)
				plane m_planeNear;
				plane m_planeFar;
			};
			plane  m_planes[6];    // planes of the view frustum
		};
	public:
		camera_frustum(){}
		void init(matrix4 const& matInvViewProj,frect const& frProjSpace);
		void init(matrix4 const& matView,
				matrix4 const& matProj,
				frect const& frProjSpace);

		void init(matrix4 const& matView,matrix4 const& matProj)
		{init(matView,matProj,frect(-1,-1,1,1));}

		void init(matrix4 const& matInvViewProj)
		{init(matInvViewProj,frect(-1,-1,1,1));}

		void transform_by(matrix4 const& mat);
		void transform_reflection_by(matrix4 const& mat);

		int test_swept_sphere(sphere const& s,vector3 const& sweepDir)const;
		int test_aabb(box3_aabb const& b)const;
		int test_sphere(sphere const& s)const;
		bool test_aabb_nb(box3_aabb const& b)const;
		bool test_sphere_nb(sphere const& s)const
		{
			bool bOutside=false;
			// Check boundary vertices against all 6 frustum planes, 
			// and store result (1 if outside) in a bitfield
			for( int iPlane = 0; iPlane < 4; iPlane++ )
			{
				if( (m_planes[iPlane].a * s.center.x +
					m_planes[iPlane].b * s.center.y +
					m_planes[iPlane].c * s.center.z +
					m_planes[iPlane].d) < -s.radius)
				{
					bOutside = true;
				}
			}
			// If any point is inside all 6 frustum planes, it is inside
			// the frustum, so the object must be rendered.
			if(!bOutside)return true;

			return false;
		}
	};

	class camera_frustum_nb : public camera_frustum
	{
	public:
		// test acclusion witn aabb
		bool test_aabb(box3_aabb const& b)const{return camera_frustum::test_aabb_nb(b);}
		bool test_sphere(sphere const& s)const{return camera_frustum::test_sphere_nb(s);}
	};

	// alias
	typedef camera_frustum_nb frustum;

}
