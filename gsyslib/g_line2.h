#pragma once
#ifndef G_LINE2_H
#define G_LINE2_H

#include "g_gsys.h"
#include "g_vector2.h"

namespace gsys
{
	struct ray2
	{
		vector2 origin;
		vector2 direction;  
	};

	enum point_classification{ptcl_LEFT,ptcl_RIGHT,ptcl_BEYOND,ptcl_BEHIND,ptcl_BETWEEN,ptcl_ORIGIN,ptcl_DESTINATION};

	struct line2
	{
		vector2 origin;
		vector2 destination;  
	};

	struct segment2
	{
		vector2 origin;
		vector2 destination;  

		segment2(){}
		segment2(vector2 const& vtStart,vector2 const& vtEnd):
				 origin(vtStart),
			     destination(vtEnd){}

		vector2 direction()const{return destination-origin;}

		 int classify_pt(vector2 const& pt)const
		 {
			vector2 a = destination-origin;
			vector2 b = pt-origin;
			gsys_real sa = a.x*b.y - b.x*a.y;
			if(sa > 0)return ptcl_LEFT;
			if(sa < 0)return ptcl_RIGHT;
			if((a.x*b.x < 0) || (a.y*b.y) < 0.0) return ptcl_BEHIND;
			if(a.lensquared() < b.lensquared()) return ptcl_BEYOND;
			if(origin == pt)return ptcl_ORIGIN;
			if(destination == pt)return ptcl_DESTINATION;
			return ptcl_BETWEEN;
		 }
		 gsys_real square_dist_ro(vector2 const& vt)
		 {
			vector2   n = (destination-origin).cross().norm();
			gsys_real l = (destination-origin).lensquared();
			gsys_real c = dot(origin,destination);
			gsys_real d = dot(n,vt)-c;
			gsys_real dd = d*d;
			gsys_real a = (vt-destination).lensquared();
			gsys_real b = (vt-origin).lensquared();
			if(a<b)exchange(a,b);
			if(dd+l > a) return dd;
			return b;
		 }
		 gsys_real dist_ro(vector2 const& vt)
		 {
			 vector2   n = (destination-origin).cross().norm();
			 gsys_real l = (destination-origin).lensquared();
			 gsys_real c = dot(n,origin);
			 gsys_real d = dot(n,vt)-c;
			 gsys_real dd = d*d;
			 gsys_real a = (vt-destination).lensquared();
			 gsys_real b = (vt-origin).lensquared();
			 if(a<b)exchange(a,b);
			 if(dd+l > a) return abs(d);
			 return sqrt(b);
		 }
	};

	typedef segment2 edge2;


}

namespace gsys
{
	namespace detail
	{
		bool inline intersect(segment2 const& s1,segment2 const& s2,gsys_real& r)
		{
			vector2 n(s2.destination.y-s2.origin.y,s2.origin.x-s2.destination.x);
			gsys_real denom = dot(n,s1.direction());
			if(denom == 0)return false;
			r = -dot(n,s1.origin-s2.origin)/denom;
			return true;
		}
	}
	bool inline find_intersection_as_lines(segment2 const& s1,segment2 const& s2,vector2& r)
	{
		gsys_real s;
		if(!detail::intersect(s2,s1,s))return false;
		r = s2.origin+s2.direction()*s;
		return true;
	}

	bool inline has_intersection(segment2 const& s1,segment2 const& s2)
	{
		gsys_real s;
		if(!detail::intersect(s2,s1,s))return false;
		if(s<0 || s > 1)return false;
		detail::intersect(s1,s2,s);
		if(s<0 || s > 1)return false;
		return true;
	}

	bool inline find_intersection(segment2 const& s1,segment2 const& s2,vector2& r)
	{
		gsys_real s;
		if(!detail::intersect(s2,s1,s))return false;
		if(s<0 || s > 1)return false;
		detail::intersect(s1,s2,s);
		if(s<0 || s > 1)return false;
		r = s1.origin+s1.direction()*s;
		return true;
	}
}

#endif // G_LINE3_H
