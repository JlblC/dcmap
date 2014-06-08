#pragma once
#ifndef G_POINT_H
#define G_POINT_H

#include "g_gsys.h"
#include "g_alg.h"
#include "g_binstream.h"
#include "g_math.h"

#include "boost/operators.hpp"

namespace gsys
{
	template <typename T>
	struct tPoint : 
		boost::totally_ordered<tPoint<T>,
		boost::additive<tPoint<T>,
		boost::multipliable2<tPoint<T>,T,
		boost::dividable2<tPoint<T>,T
		> > > >
	{
		typedef T value_type;
		typedef gsys_ulong size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
		GSYSDETAIL_IMPLEMENT_ARRAY_CONTAINER;
		typedef tPoint<T> this_type;

		value_type x;
		value_type y;

		tPoint(){}
		tPoint(value_type _x,value_type _y):x(_x),y(_y){}

		template<typename _BindType>
        explicit tPoint(tPoint<_BindType> const& v):x((value_type)v.x),y((value_type)v.y){}

		template<typename _BindType>
		this_type& operator =(tPoint<_BindType> const& v)
		{
			x = (value_type)v.x;
			y = (value_type)v.y;
			return *this;
		}

		this_type& set(value_type _x,value_type _y)
		{
			x = _x;
			y = _y;
			return *this;
		}

		reference operator[](size_type n)
		{
			gsys_assert(n<2);
			return (&x)[n];
		}
		const_reference operator[](size_type n)const
		{
			gsys_assert(n<2);
			return (&x)[n];
		}
		size_type size()const{return 2;}

         bool operator == (tPoint<T> const& vt)const
            {return equ(x,vt.x) && equ(y,vt.y);}
		bool operator < (tPoint<T> const& vt)const
            {return ((x<vt.x) || ((equ(x,vt.x) && (y < vt.y)))); }

		tPoint<T>& operator *= (T v)
		{
			x*=v;
			y*=v;
			return *this;
		}	
		tPoint<T>& operator /= (T v)
		{
			x/=v;
			y/=v;
			return *this;
		}

		tPoint<T>& operator += (tPoint<T> const& v)
		{
			x+=v.x;
			y+=v.y;
			return *this;
		}
		tPoint<T>& operator -= (tPoint<T> const& v)
		{
			x-=v.x;
			y-=v.y;
			return *this;
		}

		T len()const
		{
			return g_sqrt(x*x+y*y);
		}
		//////////////////////////////////////////////////////////////////////////

		friend std::istream& operator>>(std::istream& is,tPoint<T> & v)
		{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
			is >> wsp_scip;
			if(is.peek() == '{')
			{
				wsp_delim D_BO('{');wsp_delim D_BC('}');wsp_delim D_D(',');\
					return is >> D_BO >> ISN(v.x) >> D_D >> ISN(v.y) >> D_BC;
			}
			else
			{
				gsys::wsp_delims D(":xX");
				is >> ISN(v.x) >> D >> ISN(v.y);
			}
			return is;
		}

	};

	template<class T>
	 std::ostream& operator<<(std::ostream& os,tPoint<T> const& v)
	{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
		return os << '{' << v.x << ',' << v.y << '}';		
	}

	template<class T>
	 binostream& operator<<(binostream& os,tPoint<T> const& v)
	{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
		return os << v.x << v.y;
	}

	template<class T>
	 binistream& operator>>(binistream& is,tPoint<T> & v)
	{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
		return is >> v.x >> v.y;
	}

	
	typedef tPoint<int>    Point;
	typedef tPoint<float>  FPoint;

    typedef tPoint<int>    gpoint;
	GSYS_SET_TYPENAME(gpoint,"pt");
    typedef tPoint<float>  fpoint;
	GSYS_SET_TYPENAME(fpoint,"ptf");
	typedef tPoint<word>  wpoint;
	GSYS_SET_TYPENAME(wpoint,"ptw");

}

#endif