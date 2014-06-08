#pragma once
#ifndef G_RECT_H
#define G_RECT_H

#include "g_alg.h"
#include "g_point.h"
#include "g_binstream.h"
#include "boost/operators.hpp"

namespace gsys
{
	template<class T> 
	struct tRect : 
		boost::equality_comparable<tRect<T>,
		boost::multiplicative2<tRect<T>, T,
		boost::multiplicative2<tRect<T>, tPoint<T>
		> > >
    {
		typedef T value_type;
		typedef gsys_ulong size_type;
		typedef tRect<value_type> this_type;
		typedef tPoint<value_type> point_type;
        union
        {
            struct
            {
				value_type left,top;
				value_type right,bottom;
            };
            value_type   values[4];
        };

        tRect()
        {
           // top = bottom = left = right = T(0);
        }

        tRect(value_type val)
        {
            top = bottom = left = right = val;
        }

        tRect(value_type aleft, value_type atop, value_type aright, value_type abottom)
        {
            left = aleft; 
            top =  atop ;
            right = aright;
            bottom =  abottom;
        }

        this_type& set(value_type aleft, value_type atop, value_type aright, value_type abottom)
        {
            left = aleft; 
            top =  atop ;
            right = aright;
            bottom =  abottom;
			return *this;
        }

		this_type& init_by_center_abs_size(value_type x,value_type y,value_type w,value_type h)
		{
			left=x-w/2;
			top=y-h/2;
			right=x+w/2;
			bottom=y+h/2;
			return *this;
		}
		this_type& init_by_center_abs_size(point_type const& base,point_type const& size)
			{return init_by_center_abs_size(base.x,base.y,size.x,size.y);}

		this_type& init_by_center_size(value_type x,value_type y,value_type w,value_type h)
		{
			left=x-w;
			top=y-h;
			right=x+w;
			bottom=y+h;
			return *this;
		}
		this_type& init_by_center_size(point_type const& base,point_type const& size)
			{return init_by_center_size(base.x,base.y,size.x,size.y);}
		this_type& init_by_center_size(value_type x,value_type y,value_type w){return init_by_center_size(x,y,w,w);}

		this_type& init_by_size(value_type x,value_type y,value_type w,value_type h)
		{
			left=x;
			top=y;
			right=left+w;
			bottom=top+h;
			return *this;
		}
		this_type& init_by_size(tPoint<value_type> const& base,tPoint<value_type> const& size)
			{return init_by_size(base.x,base.y,size.x,size.y);}

        template<class _AT> 
		tRect(const tRect<_AT> &rc):
			left((value_type)rc.left),
			top((value_type)rc.top),
			right((value_type)rc.right),
			bottom((value_type)rc.bottom){}

		template<class _AT> 
		this_type& operator = (const tRect<_AT> &rc)
		{
			left = (value_type)rc.left;
			top = (value_type)rc.top;
			right = (value_type)rc.right;
			bottom = (value_type)rc.bottom;
			return *this;
		}

        value_type height()const{return gsys::abs<value_type>(bottom-top);}
        value_type width()const{return gsys::abs<value_type>(right-left);}

		point_type origin()const{return point_type(left,top);}

		point_type size()const{return point_type(width(),height());}

		point_type middle_point()const
		{
			point_type t(left+width()/2,top+height()/2);
			return t;
		}
		point_type center()const
		{
			return middle_point();
		}

		point_type& courner1_ref(){return *((point_type*)values);}
		point_type& courner2_ref(){return *((point_type*)(values+2));}

		point_type const& courner1_ref()const{return *((point_type const*)values);}
		point_type const& courner2_ref()const{return *((point_type const*)(values+2));}

        // shows whatever rect area is gsys_null 
        bool IsNull()const
        {
            if(top==bottom)return true;
            if(left==right)return true;
            return false;
        }
		/* Modifications */

        // normalises the rect 
        this_type& Normalise()
        {
			if(top > bottom) std::swap(top,bottom);
			if(left > right) std::swap(left,right);
			return *this;
        }
		// makes left < right, top<bottom
		this_type& Rectify(){return Normalise();}
		// grows rect by given values
		this_type& grow(value_type x){return grow(x,x);}
		// grows rect by given values
		this_type& grow(value_type x,value_type y)
		{
			left-=x;
			right+=x;
			top-=y;
			bottom+=y;
			return *this;
		}
 		// offset rect by given values
		this_type& offset(value_type x,value_type y)
		{
			left+=x;
			right+=x;
			top+=y;
			bottom+=y;
			return *this;
		}
		this_type& offset(point_type const& pt)
		{
			return offset(pt.x,pt.y);
		}

		tRect<int> GetOverlapGrid(value_type dx,value_type dy)const
		{
			tRect<int> rc;

			rc.left = ftoi(floorf(float(left)/dx));
			rc.top = ftoi(floorf(float(top)/dy));

			rc.right = ftoi(ceilf(float(right)/dx));
			rc.bottom = ftoi(ceilf(float(bottom)/dy));
			return rc;
		}
		tRect<int> GetOverlapGrid(value_type d)const{return GetOverlapGrid(d,d);}
		tRect<int> GetContainsGrid(value_type dx,value_type dy)const
		{
			tRect<int> rc;

			rc.left = ftoi(ceilf(float(left)/dx));
			rc.top = ftoi(ceilf(float(top)/dy));

			rc.right = ftoi(floorf(float(right)/dx));
			rc.bottom = ftoi(floorf(float(bottom)/dy));

			return rc;
		}
		tRect<int> GetContainsGrid(value_type d)const{return GetContainsGrid(d,d);}

        // bound rect (param) must have been normalised!
        this_type Bound(const this_type &rc)const
        {
            return this_type(bound(left,rc.left,rc.right),
                            bound(top,rc.top,rc.bottom),    
                            bound(right,rc.left,rc.right),
                            bound(bottom,rc.top,rc.bottom));
        }

        // create rect contains intersections of this rects
        // gsys_null rect if whey does not intersect
        this_type Intersect(const this_type &rc)const
        {
            tRect<value_type> irc;
            irc.left = max(rc.left,left);
            irc.right = min(rc.right,right);
            if(irc.left > irc.right) return tRect<value_type>(0);

            irc.top = max(rc.top,top);
            irc.bottom = min(rc.bottom,bottom);
            if(irc.top > irc.bottom) return tRect<value_type>(0);

            return irc;
        }

		bool Contains(value_type x, value_type y)const
		{
			return x>=left&&x<=right&&y>=top&&y<=bottom;
		}
		bool Contains(point_type const& pt)const{return Contains(pt.x,pt.y);}

		bool Contains(this_type const& rc)const
		{
			return Contains(rc.left,rc.top) && Contains(rc.right,rc.bottom);
		}

		// left*=x;  top*=y
		// rifgr*=x; bottom*=y
		this_type & mult(value_type x,value_type y)
		{
			left *= x;
			right *= x;
			top *= y;
			bottom *= y;
			return *this;
		}

		operator bool() // rect = false if it have zero size
		{
			return !(left == right && top == bottom);
		}

		this_type & operator *= (value_type const& v)
		{
			left *= v;
			right *= v;
			top *= v;
			bottom *= v;
			return *this;
		}

		this_type & operator *= (point_type const& v)
		{
			left *= v.x;
			right *= v.x;
			top *= v.y;
			bottom *= v.y;
			return *this;
		}

		this_type & operator /= (value_type const& v)
		{
			left/=v;
			right/=v;
			top/=v;
			bottom/=v;
			return *this;
		}

		this_type & operator /= (point_type const& v)
		{
			left /= v.x;
			right /= v.x;
			top /= v.y;
			bottom /= v.y;
			return *this;
		}

        // compares rects
        bool operator == (const this_type& rc)const
        {
            return equ(rc.bottom,bottom) && equ(rc.top,top) && 
                   equ(rc.left,left) &&  equ(rc.right,right);
        }
    };
    typedef tRect<int>    Rect;
    typedef tRect<float>  FRect;

	typedef tRect<int> grect;
	GSYS_SET_TYPENAME(grect,"rc");
	typedef tRect<float> frect;
	GSYS_SET_TYPENAME(frect,"rcf");
	typedef tRect<word> wrect;
	GSYS_SET_TYPENAME(wrect,"rcw");

	template<class T>
	 std::ostream& operator<<(std::ostream& os,tRect<T> const& v)
	{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
		return os << "{" << v.left << "," << v.top << "," << v.right << ","<< v.bottom<< "}";	
	}

	template<class T>
	std::istream& operator>>(std::istream& is,tRect<T> & v)
	{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
		wsp_delim D_BO('{');wsp_delim D_BC('}');wsp_delim D_D(',');\
		char ch = is.peek();
		if(ch == 'c')
		{
			is.ignore();
			typename tRect<T>::value_type v1,v2,cx,cy;
			is >> D_BO >> ISN(v1) >> D_D >> ISN(v2) >> D_D >> ISN(cx) >> D_D >> ISN(cy) >> D_BC;
			v.init_by_center_abs_size(v1,v2,cx,cy);
		}
		else if(ch == 's')
		{
			is.ignore();
			typename tRect<T>::value_type v1,v2,cx,cy;
			is >> D_BO >> ISN(v1) >> D_D >> ISN(v2) >> D_D >> ISN(cx) >> D_D >> ISN(cy) >> D_BC;
			v.init_by_size(v1,v2,cx,cy);
		}
		else
		{
			is>>D_BO>>ISN(v.left)>>D_D>>ISN(v.top)>>D_D>>ISN(v.right)>>D_D>>ISN(v.bottom)>>D_BC;
		}
		return is;
	}

	template<class T>
	 binostream& operator<<(binostream& os,tRect<T> const& v)
	{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
		return os << v.left << v.top << v.right << v.bottom; 
	}

	template<class T>
	 binistream& operator>>(binistream& is,tRect<T> & v)
	{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
		return is >> v.left >> v.top >> v.right >> v.bottom; 
	}

	inline frect& lerp(frect const& c1,frect const& c2,gsys_real amount,frect & res)
	{
		res.left   = c1.left + (c2.left-c1.left)*amount;
		res.top    = c1.top + (c2.top-c1.top)*amount;
		res.right  = c1.right + (c2.right-c1.right)*amount;
		res.bottom = c1.bottom + (c2.bottom-c1.bottom)*amount;
		return res;
	}
	inline frect lerp(frect const& c1,frect const& c2,gsys_real amount)
	{frect cl;lerp(c1,c2,amount,cl);return cl;}

}

#endif