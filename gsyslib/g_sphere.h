#pragma once
#ifndef G_SPHERE_H
#define G_SPHERE_H

#include "g_vector3.h"
#include "g_matrix4.h"

namespace gsys
{
	struct sphere
	{
		vector3 center;
		float   radius;  

		sphere(){}
		sphere(vector3 vtCenter,float fRadius):
			center(vtCenter),
			radius(fRadius)
		{}
		explicit sphere( const vector3* points,int num );
		
		operator bool(){return !equ(radius,0.0f);}

		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(sphere);

		GSYSDETAIL_OSTREAM_BASIC_OBJ(sphere,os << '{' << v.center << ',' << v.radius << '}');

		GSYSDETAIL_ISTREAM_STD_BASIC_OBJ(sphere,DO>>v.center>>D>>v.radius>>DC);

	};
	GSYS_SET_TYPENAME(sphere,"sphere");
}

inline gsys::sphere::sphere( const vector3* points,int num )
{
	//GSYS_ASSERTPT(points);
	//_GSYS_ASSERT(num > 0);
	radius = 0.f;
	center = points[0];

	for(int i=1;i<num;i++)
	{
		const vector3& tmp = points[i];
		vector3 cVec = tmp - center;
		float d = dot( cVec, cVec );
		if ( d > radius*radius )
		{
			d = sqrtf(d);
			float r = 0.5f * (d+radius);
			float scale = (r-radius) / d;
			center = center + scale*cVec;
			radius = r;
		}
	}
}

#endif