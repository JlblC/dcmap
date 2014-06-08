#pragma once

#ifndef G_PLANE_H
#define G_PLANE_H

#include "g_gsys.h"
#include "g_vector3.h"
//#include "g_matrix4.h"
#include "g_line3.h"

namespace gsys
{
	struct plane
	{
		typedef gsys_real value_type;
		
		union
		{
			struct{value_type a,b,c,d;};
			gsys_real values[4];
		};

		vector3& normal_ref(){return *reinterpret_cast<vector3*>(values);}
		vector3 const& normal_ref()const{return *reinterpret_cast<vector3 const*>(values);}

		vector3& n(){return normal_ref();}
		vector3 const& n()const{return normal_ref();}
		
		// initialisation
		plane& init_vector_normal(vector3 const& p,vector3 const& n)
		{
			a = n.x;
			b = n.y;
			c = n.z;
			d = -(a*p.x+b*p.y+c*p.z);
			return *this;
		}
		plane& init_vectors(vector3 const& v1,vector3 const& v2,vector3 const& v3)
		{
			vector3 n = cross(v2 - v3,v2 - v1).norm();
			a = n.x;
			b = n.y;
			c = n.z;
			d = -(a*v1.x+b*v1.y+c*v1.z);
			return *this;
		}
		float distantce_to_point(vector3 const& c)const{return (a*c.x+b*c.y+c*c.z+d);}

		float dist_to_intersection(gsys::ray3 const& ray)const
		{
			return gsys::abs((gsys::dot(normal_ref(),ray.origin)+d)/ gsys::dot(normal_ref(),ray.direction));
		}

		float dot_coord(vector3 const& v)const
		{
			return a*v.x + b*v.y + c*v.z + d;
		}
		float dot_normal(vector3 const& v)const
		{
			return a*v.x + b*v.y + c*v.z;
		}

	};
}

#endif