#include "g_polygon2.h"

namespace gsys
{
	bool GSYS_INC_API point_inside_convex_polygon(polygon2 & poly,vector2 const& pt)
	{
		for(gsys_size_t i=0;i<poly.size();poly.advance_cw(),i++)
		{
			edge2 e = poly.edge();
			if(e.classify_pt(pt) == ptcl_LEFT)
			{
				return false;
			}
		}
		return true;
	}

	bool GSYS_INC_API clip_polygon2_to_edge2(polygon2& s,edge2 const& e,polygon2& r)
	{
		r.clear();
		vector2 crossingPT;
		for(gsys_size_t i=0;i<s.size();s.advance_cw(),i++)
		{
			vector2 org = s.v();
			vector2 dest = s.cw();
			bool orgIsInside =  e.classify_pt(org) != ptcl_LEFT;
			bool destIsInside = e.classify_pt(dest)!= ptcl_LEFT;

			if(orgIsInside != destIsInside)
			{
                edge2 ed(org,dest);
				bool r = find_intersection_as_lines(e,ed,crossingPT);
			}

			if(orgIsInside && destIsInside)
            {
                r.insert_after(dest);
            }
			else if(orgIsInside && !destIsInside)
			{
				if(org != crossingPT)r.insert_after(crossingPT);
			}
			else if(!orgIsInside && destIsInside)
			{
				r.insert_after(crossingPT);
				if(dest != crossingPT)r.insert_after(dest);
			}
		}
		return r.size()>0;
	}

	bool GSYS_INC_API clip_polygon2(polygon2&s,polygon2 &p,polygon2& result)
	{
		polygon2 polyTemp;
		polygon2* q = &s;
		polygon2* r = &result;
		for(gsys_size_t i=0;i<p.size();p.advance_cw(),i++)
		{
			edge2 e= p.edge();
			if(clip_polygon2_to_edge2(*q,e,*r))
			{
				q = r;
				if(r != &polyTemp)r = &polyTemp;
				else r = &result;
			}
			else
			{
				result.clear();
				return false;
			}
		}
		if(q != &result)result = *q;
		return true;
	}

	void GSYS_INC_API polygon2::build_circumscribed_rect(frect & rc)
	{
		rc.init_by_center_size(v().as_point(),fpoint(0,0));
		advance_cw();

		for(gsys_size_t i=1;i<size();advance_cw(),i++)
		{
			vector2& v = polygon2::v();
            rc.left = min(v.x,rc.left);
			rc.right = max(v.x,rc.right);
			rc.top = min(v.y,rc.top);
			rc.bottom = max(v.y,rc.bottom);
		}
	}

/*
	bool GSYS_INC_API clip_polygon2(polygon2&s,polygon2 &p,polygon2& result)
	{
		polygon2 polyTemp;
		polygon2 q = s;
		polygon2 r;
		for(gsys_size_t i=0;i<p.size();p.advance_cw(),i++)
		{
			edge2 e= p.edge();
			if(clip_polygon2_to_edge2(q,e,r))
			{
				q = r;
			}
			else
			{
				result.clear();
				return false;
			}
		}
		result = r;
		return true;
	}
*/
}
