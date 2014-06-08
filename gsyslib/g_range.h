#pragma once
#ifndef G_RANGE_H
#define G_RANGE_H
#include "g_gsys.h"

#include "g_vector2.h"

namespace gsys
{
	template<typename T>
	struct range :	boost::additive2<vector2,gsys_real>
	{
		typedef T value_type;
		typedef gsys_unsigned size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
		GSYSDETAIL_IMPLEMENT_ARRAY_CONTAINER;

		value_type from,to;

		reference operator[](size_type n)
		{
			gsys_assert(n<2);
			return (&from)[n];
		}
		const_reference operator[](size_type n)const
		{
			gsys_assert(n<2);
			return (&from)[n];
		}
		size_type size()const{return 2;}


		range(){}
		range(value_type a, value_type b):from(a),to(b){}
		range& set(value_type a, value_type b){from = a;to=b;return *this;}

		range& normalize()
		{
			if(from > to)exchange(from,to);
			return *this;
		}

		value_type diff()const{return to-from;}
		value_type absdiff()const{return abs(to-from);}

		range& operator += (value_type v)
		{
			from += v;
			to += v;
			return *this;
		}

		range& operator -= (value_type v)
		{
			from -= v;
			to -= v;
			return *this;
		}

		GSYSDETAIL_OSTREAM_BASIC_OBJ(range<T>,os << "{" << v.from << "," << v.to << "}");
		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(range<T>);

		friend inline std::istream& operator>>(std::istream& is,range<T> & v)
		{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
		gsys::wsp_delims D("-xX");
			is >> wsp_scip;
			if(is.peek() == '{')
			{
				tPoint<value_type> vt(0,0);
				is >> vt;
				v.from = vt.x;
				v.to = vt.y;
			}
			else 
				is >>  ISN(v.from) >> D >> ISN(v.to);
			return is;
		}
	};
	typedef range<gsys_real>      frange;
	typedef range<gsys_int>       irange;
	typedef range<gsys_unsigned>  urange;
	GSYS_SET_TYPENAME(frange,"frg");
	GSYS_SET_TYPENAME(irange,"irg");
	GSYS_SET_TYPENAME(urange,"urg");
}

#endif