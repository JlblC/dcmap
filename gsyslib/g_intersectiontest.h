/*=======================================================================
Intersection Test Library
------------------------------------------------------------------------
This file handle intersection & collision betwen various objects


====================================================================*/
#pragma once
#include "g_gsys.h"
#include "g_math.h"
#include "g_sphere.h"
#include "g_line3.h"
#include "g_triangle3.h"

namespace gsys{

	inline bool intersection_test(ray3 const& ray,triangle3 const& tri)
	{
		// Find vectors for two edges sharing vert0
		vector3 edge1 = tri.v1 - tri.v0;
		vector3 edge2 = tri.v2 - tri.v0;

		// Begin calculating determinant - also used to calculate U parameter
		vector3 pvec = cross(ray.direction,edge2);

		// If determinant is near zero, ray lies in plane of triangle
		float det = dot(edge1,pvec);

		vector3 tvec;
		if( det > 0 )
		{
			tvec = ray.origin - tri.v0;
		}
		else
		{
			tvec = tri.v0 - ray.origin;
			det = -det;
		}
		if( det < 0.0001f )  return false;

		// Calculate U parameter and test bounds
		float u = dot(tvec, pvec );
		if( u < 0.0f || u > det )
			return false;

		// Prepare to test V parameter
		vector3 qvec = cross(tvec, edge1);

		// Calculate V parameter and test bounds
		float v = dot(ray.direction,qvec);
		if( v < 0.0f || u + v > det )
			return false;

		return true;
	}

	gsys_real inline distance_squared(vector3 const& rkPoint, ray3 const& rkRay)
	{
		vector3 kDiff = rkPoint - rkRay.origin;
		gsys_real fT = dot(kDiff,rkRay.direction);

		if ( fT > 0.0f )
		{
			fT /= rkRay.direction.lensquared();
			kDiff -= fT*rkRay.direction;
		}
		return kDiff.lensquared();
	}

	inline bool intersection_test(ray3 const& ray,sphere const& sph)
	{
		gsys_real fSqrDist = distance_squared(sph.center,ray);
		return fSqrDist <= sph.radius*sph.radius;
	}

	// get distance from ray origin to first intersection with sphere 
	// ray direction must be normalized
	bool inline find_intersection(ray3 const& ray,sphere const& sph,float &fDist)
	{
		// set up quadratic Q(t) = a*t^2 + 2*b*t + c
		vector3 kDiff = ray.origin - sph.center;
		gsys_real fB = kDiff % ray.direction;
		gsys_real fC = kDiff.lensquared() - g_sqr(sph.radius);

		gsys_real afT[2];
		gsys_real fDiscr = fB*fB - fC;
		if ( fDiscr < 0.0f )
		{
			// no intersection
			return false;
		}
		else if ( fDiscr > 0.0f )
		{
			gsys_real fRoot = g_sqrt(fDiscr);
			afT[0] = -fB - fRoot;
			afT[1] = -fB + fRoot;

			if ( afT[0] >= 0.0f )
			{
				fDist = min(afT[0],afT[1]);
				return true;
			}
			else 
			{
				if ( afT[1] >= 0.0f )
				{
					fDist = afT[1];
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			afT[0] = -fB;
			if ( afT[0] >= 0.0f )
			{
				fDist = afT[0];
				return true;
			}
			else
			{
				return false;
			}
		}
	}
/*
	bool inline find_intersection(ray3 const& ray,sphere const& sph,float &fDist)
	{
		// set up quadratic Q(t) = a*t^2 + 2*b*t + c
		vector3 kDiff = ray.origin - sph.center;
		gsys_real fA = ray.direction.lensquared();
		gsys_real fB = kDiff % ray.direction;
		gsys_real fC = kDiff.lensquared() - g_sqr(sph.radius);

		gsys_real afT[2];
		gsys_real fDiscr = fB*fB - fA*fC;
		if ( fDiscr < 0.0f )
		{
			// no intersection
			return false;
		}
		else if ( fDiscr > 0.0f )
		{
			gsys_real fRoot = g_sqrt(fDiscr);
			gsys_real fInvA = 1.0f/fA;
			afT[0] = (-fB - fRoot)*fInvA;
			afT[1] = (-fB + fRoot)*fInvA;

			if ( afT[0] >= 0.0f )
			{
				fDist = min(afT[0],afT[1]);
				return true;
			}
			else 
			{
				if ( afT[1] >= 0.0f )
				{
					fDist = afT[1];
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			afT[0] = -fB/fA;
			if ( afT[0] >= 0.0f )
			{
				fDist = afT[0];
				return true;
			}
			else
			{
				return false;
			}
		}
	}
*/

/*
	inline bool intersection_test(ray3 const& ray,sphere const& sph)
	{
		vector3 vDiff = sph.center - ray.origin;
		float fT = dot(vDiff,ray.direction);
		if(fT <= 0.0f) return false;

		fT /= ray.direction.lensquared();
		vDiff -= fT*ray.direction;
		return vDiff.lensquared() < sph.radius*sph.radius;
	}
*/
	inline bool intersection_test(segment3 const& seg,sphere const& sph)
	{
		float rr = sph.radius * sph.radius;

		vector3 kDiff = sph.center - seg.origin;
		float fT =  dot(kDiff,seg.destination);

		if ( fT > 0.0f )
		{
			float fSqrLen= seg.destination.lensquared();
			if ( fT >= fSqrLen )
			{
				fT = 1.0f;
				kDiff -= seg.destination;
			}
			else
			{
				fT /= fSqrLen;
				kDiff -= fT*seg.destination;
			}
		}

		return(kDiff.lensquared() <= rr);
	}

	inline bool intersection_test_sphere(
		vector3 const& pos1,float r1,
		vector3 const& pos2,float r2)
	{
		vector3 relPos = pos1 - pos2;
		float dist = relPos.x * relPos.x + relPos.y * relPos.y + relPos.z * relPos.z;
		float minDist = r1 + r2;
		return dist <= minDist * minDist;
	}

	inline bool collision_test_sphere(
		vector3 const& pos1,float r1,vector3 const& vel1,
		vector3 const& pos2,float r2,vector3 const& vel2)
	{
		//Initialize the return value
		//*t = 0.0f;

		// Relative velocity
		vector3    dv    = vel2 - vel1;
		// Relative position
		vector3    dp    = pos2 - pos1;
		//Minimal distance squared
		float r = r1 + r2;
		//dP^2-r^2
		float pp = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z - r*r;
		//(1)Check if the spheres are already intersecting
		if ( pp < 0 ) return true;
		//dP*dV
		float pv    = dp.x * dv.x + dp.y * dv.y + dp.z * dv.z;
		//(2)Check if the spheres are moving away from each other
		if ( pv >= 0 ) return false;

		//dV^2
		float vv = dv.x * dv.x + dv.y * dv.y + dv.z * dv.z;
		//(3)Check if the spheres can intersect within 1 frame
		if ( (pv + vv) <= 0 && (vv + 2 * pv + pp) >= 0 ) return false;

		//tmin = -dP*dV/dV*2 
		//the time when the distance between the spheres is minimal
		float tmin = -pv/vv;

		//Discriminant/(4*dV^2) = -(dp^2-r^2+dP*dV*tmin)
		return ( pp + pv * tmin > 0 );
	}
}