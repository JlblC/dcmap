#pragma once

#include "g_sys.h"
#include "g_math.h"
#include "boost/operators.hpp"


namespace gsys
{
	struct point2 : 
		boost::multiplicative2<point2,float,
		boost::totally_ordered<point2,
		boost::additive1<point2
		> > >
	{
	   union {
            struct {
		        float x;
		        float y;
            };
            float v[2];
        };
        float operator [](uint i)const{GSYS_ASSERT(i<2);return v[i];} 

		// constructors
		point2(float l_x,float l_y){x=l_x; y=l_y; };
		point2(){};

		point2& operator = (point2 const& v)
		{
			x = v.x;
			y = v.y; 
			return *this;
		}

		//////////////////////////////////////////////////////////////////////
         bool operator == (point2 const& vt)const
            {return equ(x,vt.x) && equ(y,vt.y);}
		bool operator < (point2 const& vt)const
            {return ((x<vt.x) || ((equ(x,vt.x) && (y < vt.y)))); }
 //       bool operator > (point2 const& vt)const
 //           {return ((x>vt.x) || ((equ(x,vt.x) && (y > vt.y)))); }

		point2& operator *= (float f)
		{
			x*=f;
			y*=f;
			return *this;
		}
		point2& operator /= (float f)
		{
			x/=f;
			y/=f;
			return *this;
		}
		point2& operator += (point2 const& v)
		{
			x+=v.x;
			y+=v.y;
			return *this;
		}
		point2& operator -= (point2 const& v)
		{
			x-=v.x;
			y-=v.y;
			return *this;
		}

		float mod()const
		{
			return g_sqrt(x*x+y*y);
		}
		float lenght()const{return mod();}
		// return dot product 
		float dot(point2 const& v)const
		{
			return x*v.x+y*v.y;
		}

/*
                       /
                      + BEYOND
                     /
                    /
                   /
                  + DESTINATION
                 *
     + LEFT     *
               *
              *
             + BETWEEN      + RIGHT
            *
           *
          *
         *
        + ORIGIN
       /
      /
     /
    /
   + BEHIND
  /
 /
/
*/
		int classify(point2 const& p0, point2 const& p1)const
		{
			point2 p2 = *this;
			point2 a = p1 - p0;
			point2 b = p2 - p0;

			float sa = a.x*b.y - b.x*a.y;
			if(sa > 0.0) return LEFT;
			if(sa < 0.0) return RIGHT;
			if((a.x*b.x < 0.0) || (a.y * b.y < 0.0)) return BEHIND;
			if(a.lenght() < b.lenght()) return BEYOND;
			if(p0 == p2) return ORIGIN;
			if(p1 == p2) return DESTINATION;
			return BETWEEN;
		}

		// Dot Product
		static float dot(point2 const& v1,point2 const& v2)
		{
			return v1.dot(v2);
		}
	};
};



