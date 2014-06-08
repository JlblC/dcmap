#include "common.h"
#include "../g_camera_frustum.h"

void gsys::camera_frustum::init(matrix4 const& matInvViewProj,frect const& fr)
{
	m_corners[0] = vector3(fr.right,fr.bottom,  0.0f); // xyz
	m_corners[1] = vector3(fr.left,fr.bottom,  0.0f); // Xyz
	m_corners[2] = vector3(fr.right,fr.top,  0.0f); // xYz
	m_corners[3] = vector3(fr.left,fr.top,  0.0f); // XYz

	m_corners[4] = vector3(fr.right,fr.bottom,  1.0f); // xyZ
	m_corners[5] = vector3(fr.left,fr.bottom,  1.0f); // XyZ
	m_corners[6] = vector3(fr.right,fr.top,  1.0f); // xYZ
	m_corners[7] = vector3(fr.left,fr.top,  1.0f); // XYZ

	transform_by(matInvViewProj);
}


void gsys::camera_frustum::init(matrix4 const& matView,
											 matrix4 const& matProj,
											 frect const& fr)
{GSYS_CFN;
	matrix4 m = matView * matProj;
	m = m.inverse();
	init(m,fr);
}

void gsys::camera_frustum::make_planes()
{
	m_planeNear.init_vectors(m_corners[0],m_corners[1],m_corners[2]); //Near
	m_planeFar.init_vectors(m_corners[6],m_corners[7],m_corners[5]); //Far
	m_planeLeft.init_vectors(m_corners[2],m_corners[6],m_corners[4]); //Left
	m_planeRight.init_vectors(m_corners[7],m_corners[3],m_corners[5]); //Right
	m_planeTop.init_vectors(m_corners[2],m_corners[3],m_corners[6]); //Top
	m_planeBottom.init_vectors(m_corners[1],m_corners[0],m_corners[4]); //Bottom
}

void gsys::camera_frustum::transform_reflection_by(matrix4 const& mat)
{GSYS_CFN;
	transform_by(mat);
	for(int i=0;i<6;i++)
	{
		m_planes[i].n() = -m_planes[i].n();
		m_planes[i].d = -m_planes[i].d; 
	}
}

void gsys::camera_frustum::transform_by(matrix4 const& mat)
{GSYS_CFN;
	
	for(int i=0;i<8;i++)
	{
		m_corners[i] = transform_coord(m_corners[i],mat);
	}
	make_planes();
}

int gsys::camera_frustum::test_sphere(sphere const& s)const
{
	bool bOutside=false;
	// Check boundary vertices against all 6 frustum planes, 
	// and store result (1 if outside) in a bitfield
	for( int iPlane = 0; iPlane < 6; iPlane++ )
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

int gsys::camera_frustum::test_aabb(box3_aabb const& b)const
{
	//return true;
	// build array of oob vertex
	vector3 vdis = b.vmax - b.vmin;
	vector3 vt[8];
	vt[0] = b.vmin + vector3(0,0,0);;
	vt[1] = b.vmin + vector3(vdis.x,0,0);
	vt[2] = b.vmin + vector3(0,vdis.y,0);
	vt[3] = b.vmin + vector3(vdis.x,vdis.y,0);
	vt[4] = b.vmin + vector3(0,0,vdis.z);;
	vt[5] = b.vmin + vector3(vdis.x,0,vdis.z);
	vt[6] = b.vmin + vector3(0,vdis.y,vdis.z);
	vt[7] = b.vmin + vector3(vdis.x,vdis.y,vdis.z);

	gsys_byte bOutside[8];
	memset( &bOutside,0, sizeof(bOutside) );

	// Check boundary vertices against all 6 frustum planes, 
	// and store result (1 if outside) in a bitfield
	for( int iPoint = 0; iPoint < 8; iPoint++ )
	{
		for( int iPlane = 0; iPlane < 6; iPlane++ )
		{
			if( m_planes[iPlane].a * vt[iPoint].x +
				m_planes[iPlane].b * vt[iPoint].y +
				m_planes[iPlane].c * vt[iPoint].z +
				m_planes[iPlane].d < 0)
			{
				bOutside[iPoint] |= (1 << iPlane);
			}
		}
		// If any point is inside all 6 frustum planes, it is inside
		// the frustum, so the object must be rendered.
		if( bOutside[iPoint] == 0 )
			return 1;
	}
	//return false;

	if( (bOutside[0] & bOutside[1] & bOutside[2] & bOutside[3] & 
		bOutside[4] & bOutside[5] & bOutside[6] & bOutside[7]) != 0 )
	{
		return 0; // it surely ouside
	}

	// i'm not sure:
	return 0;
}

static bool SweptSpherePlaneIntersect(float& t0, float& t1, const plane& pl, sphere const& sp, const vector3& sweepDir)
{
	float b_dot_n = pl.dot_coord(sp.center);
	float d_dot_n = pl.dot_normal(sweepDir);

	if (d_dot_n == 0.f)
	{
		if (b_dot_n <= sp.radius)
		{
			//  effectively infinity
			t0 = 0.f;
			t1 = 1e32f;
			return true;
		}
		else
			return false;
	}
	else
	{
		float tmp0 = ( sp.radius - b_dot_n) / d_dot_n;
		float tmp1 = (-sp.radius - b_dot_n) / d_dot_n;
		t0 = min(tmp0, tmp1);
		t1 = max(tmp0, tmp1);
		return true;
	}
}

int gsys::camera_frustum::test_swept_sphere(sphere const& sp,vector3 const& sweepDir)const
{
	//  algorithm -- get all 12 intersection points of the swept sphere with the view frustum
	//  for all points >0, displace sphere along the sweep direction.  if the displaced sphere
	//  is inside the frustum, return TRUE.  else, return FALSE
	float displacements[12];
	int cnt = 0;
	float a, b;
	bool inFrustum = false;

	for (int i=0; i<6; i++)
	{
		if (SweptSpherePlaneIntersect(a, b, m_planes[i], sp, sweepDir))
		{
			if (a>=0.f)
				displacements[cnt++] = a;
			if (b>=0.f)
				displacements[cnt++] = b;
		}
	}

	for (int i=0; i<cnt; i++)
	{
		sphere displacedSphere(sp);
		displacedSphere.center += (sweepDir)*displacements[i];
		displacedSphere.radius *= 1.1f;
		inFrustum |= test_sphere(displacedSphere) != 0;
	}
	return inFrustum;
}


bool gsys::camera_frustum::test_aabb_nb(box3_aabb const& b)const
{GSYS_CFN;
	//return true;
	// build array of oob vertex
	vector3 vdis = b.vmax - b.vmin;
	vector3 vt[8];
	vt[0] = b.vmin + vector3(0,0,0);;
	vt[1] = b.vmin + vector3(vdis.x,0,0);
	vt[2] = b.vmin + vector3(0,vdis.y,0);
	vt[3] = b.vmin + vector3(vdis.x,vdis.y,0);
	vt[4] = b.vmin + vector3(0,0,vdis.z);;
	vt[5] = b.vmin + vector3(vdis.x,0,vdis.z);
	vt[6] = b.vmin + vector3(0,vdis.y,vdis.z);
	vt[7] = b.vmin + vector3(vdis.x,vdis.y,vdis.z);

	gsys_byte bOutside[8];
	memset( &bOutside,0, sizeof(bOutside) );

	// Check boundary vertices against all 6 frustum planes, 
	// and store result (1 if outside) in a bitfield
	for( int iPoint = 0; iPoint < 8; iPoint++ )
	{
		for( int iPlane = 0; iPlane < 4; iPlane++ )
		{
			if( m_planes[iPlane].a * vt[iPoint].x +
				m_planes[iPlane].b * vt[iPoint].y +
				m_planes[iPlane].c * vt[iPoint].z +
				m_planes[iPlane].d < 0)
			{
				bOutside[iPoint] |= (1 << iPlane);
			}
		}
		// If any point is inside all 6 frustum planes, it is inside
		// the frustum, so the object must be rendered.
		if( bOutside[iPoint] == 0 )
			return true;
	}
	//return false;

	if( (bOutside[0] & bOutside[1] & bOutside[2] & bOutside[3] & 
		bOutside[4] & bOutside[5] & bOutside[6] & bOutside[7]) != 0 )
	{
		return false; // it surely ouside
	}

	// i'm not sure:
	return true;
}

