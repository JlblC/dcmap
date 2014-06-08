#pragma once
#include "g_geometry.h"
#include "g_point2.h"
#include "boost/operators.hpp"
#include <algorithm>

namespace gsys
{
	struct edge2
	{
		point2 org;
		point2 dest;

		edge2(point2 const& l_org,point2 const& l_dest)
		{
			org = l_org;
			dest = l_dest;
		}

		edge2& flip()
		{
			std::swap(org,dest);
			return *this;
		}
		edge2& rot()
		{
			point2 m = 0.5*(org + dest);
			point2 v = dest - org;
			point2 n(v.y,-v.x);
			org = m - 0.5*n;
			dest = m + 0.5*n;
			return *this;
		}

		int intersect(edge2 const& e, float &t)
		{
			point2 a = org;
			point2 b = dest;
			point2 c = e.org;
			point2 d = e.dest;
			point2 n = point2((d-c).y,(c-d).x);
			float denom = point2::dot(n,b-a);
			if(equ(denom,float(0.0)))
			{	
				int aclass = org.classify(e.org,e.dest);
				if((aclass==LEFT) || (aclass==RIGHT)) return PARALLEL;
				else return COLLINEAR;
			}
			float num = point2::dot(n,a-c);
			t = -num/denom;
			return SKEW;
		}

		bool operator < (edge2 const& e)const
		{
			return (org < e.org) || ((equ(org,e.org)) && (dest < e.dest));
		}
	};
};