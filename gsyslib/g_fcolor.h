#pragma once
#ifndef G_FCOLOR_H
#define G_FCOLOR_H

#include "g_gsys.h"
#include "g_assert.h"
#include "g_text.h"
#include "g_alg.h"
#include "g_binstream.h"
#include "boost/operators.hpp"

#include "g_color.h"

#include "g_vector4.h"

#include <limits>

namespace gsys
{
	struct fcolor:
		boost::additive<gsys::fcolor,
		boost::multiplicative2<gsys::fcolor,gsys_real,
		boost::equality_comparable<gsys::fcolor
		> > >
	{
		typedef gsys_real value_type;
		typedef ulong size_type;
		typedef value_type const*  const_pointer;

		///////////////////////////////////////////////////////////////////////////////
		// Data Access
		//////////////////////////////////////////////////////////////////////////////
		union
		{
			struct
			{
				value_type r;
				value_type g;
				value_type b;
				value_type a;
			};
			value_type values[4];
		};

		//operator const_pointer()const{return values;}

		size_type size()const{return 4;}
		value_type operator [](size_type i)const{GSYS_ASSERT(i<size());return values[i];} 

		vector4 & vector4_ref(){return reinterpret_ref_cast<vector4>(*this);}
		vector4 const& vector4_ref()const{return reinterpret_const_ref_cast<vector4>(*this);}

		vector3 & vector3_ref(){return reinterpret_ref_cast<vector3>(*this);}
		vector3 const& vector3_ref()const{return reinterpret_const_ref_cast<vector3>(*this);}
		///////////////////////////////////////////////////////////////////////////////
		// Construction
		//////////////////////////////////////////////////////////////////////////////

		fcolor(){}

		fcolor(dwcolor cl)
		{
			r = ((value_type)cl.r)/255.0f;
			g = ((value_type)cl.g)/255.0f;
			b = ((value_type)cl.b)/255.0f;
			a = ((value_type)cl.a)/255.0f;
		}

		operator dwcolor()const
		{
			dwcolor cl;
			cl.r = gbyte(bound(r,0.0f,1.0f)*255.0f);
			cl.g = gbyte(bound(g,0.0f,1.0f)*255.0f);
			cl.b = gbyte(bound(b,0.0f,1.0f)*255.0f);
			cl.a = gbyte(bound(a,0.0f,1.0f)*255.0f);
			return cl;
		}

		fcolor(value_type l_r,value_type l_g,value_type l_b,value_type l_a=1.0f)
		{
			r = l_r;
			g = l_g;
			b = l_b;
			a = l_a;
		}

		fcolor(value_type l_cl)
		{
			r = l_cl;
			g = l_cl;
			b = l_cl;
			a = 1.0f;
		}

		///////////////////////////////////////////////////////////////////////////////
		// Initialization
		//////////////////////////////////////////////////////////////////////////////

		fcolor& set(value_type l_r,value_type l_g,value_type l_b,value_type l_a=1.0f)
		{
			r = l_r;
			g = l_g;
			b = l_b;
			a = l_a;
			return *this;
		}
		fcolor& set(value_type l_v,value_type l_a=1.0f)
		{
			r = l_v;
			g = l_v;
			b = l_v;
			a = l_a;
			return *this;
		}
		///////////////////////////////////////////////////////////////////////////////
		// Operations
		//////////////////////////////////////////////////////////////////////////////

		fcolor& invert()
		{
			r = 1.0f-r;
			g = 1.0f-g;
			b = 1.0f-b;
			return *this;
		}

		fcolor& clamp()
		{
			r = gsys::bound(r,0.0f,1.0f);
			g = gsys::bound(g,0.0f,1.0f);
			b = gsys::bound(b,0.0f,1.0f);
			a = gsys::bound(a,0.0f,1.0f);
			return *this;
		}

		fcolor & operator += (fcolor const& c)
		{
			r += c.r;
			g += c.g;
			b += c.b;
			a += c.a;
			return *this;
		}

		fcolor & operator -= (fcolor const& c)
		{
			r -= c.r;
			g -= c.g;
			b -= c.b;
			a -= c.a;
			return *this;
		}

		fcolor & operator *= (float  a)
		{
			r *= a;
			g *= a;
			b *= a;
			a *= a;
			return *this;
		}

		fcolor & operator /= (float  a)
		{
			r /= a;
			g /= a;
			b /= a;
			a /= a;
			return *this;
		}
		bool operator == (fcolor const& c)
		{
			return equ(r,c.r) && equ(g,c.g) && equ(b,c.b) && equ(a,c.a);
		}
		bool operator != (fcolor const& c)
		{
			return !equ(r,c.r) || !equ(g,c.g) || !equ(b,c.b) || !equ(a,c.a);
		}

		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(fcolor);
	};

	inline fcolor& lerp(fcolor const& c1,fcolor const& c2,gsys_real amount,fcolor & res)
	{
		res.r = c1.r + (c2.r-c1.r)*amount;
		res.g = c1.g + (c2.g-c1.g)*amount;
		res.b = c1.b + (c2.b-c1.b)*amount;
		res.a = c1.a + (c2.a-c1.a)*amount;
		return res;
	}
	inline fcolor lerp(fcolor const& c1,fcolor const& c2,gsys_real amount)
	{fcolor cl;lerp(c1,c2,amount,cl);return cl;}

	inline std::ostream& operator<<(std::ostream& os,fcolor const& cl)
	{
		return os << "f{" << cl.r << "," << cl.g << "," << cl.b << "," << cl.a << "}";		
	}
	inline std::istream& operator>>(std::istream& is,fcolor & cl)
	{
		is >> ws;
		char c = is.peek();
		if(c == 'd') // desima; processing
		{
			dwcolor dwcl;
			is >> dwcl;
			if(!is.fail())cl = dwcl;
		}
		else if(c == 'f'){is.ignore();} // default processing
		else if(c != '{')
		{
			float l_v; is >> l_v;
			cl = l_v;
		}
		else
		{
			fcolor::value_type r,g,b,a;
			wsp_delim d_bo('{');
			wsp_delim d_bc('}');
			wsp_delim d_d(',');

			is >> d_bo >> r >> d_d >> g >> d_d >> b >> wsp_scip;
			c = is.peek(); // alpha component optional
			if(c == ',')
				is >> d_d >> a >> d_bc;
			else if(c == '}')
			{
				is.ignore();
				a = 1.0f;
			}
			else{is.setstate( std::ios::failbit );return is;}
			cl.set(r,g,b,a);
		}
		return is;
	}

	// Wrap for fcolor
	const fcolor fclWhite(dwcl_white);
	const fcolor fclBlack(dwcl_black);
	const fcolor fclRed(dwcl_red);
	const fcolor fclLightGreen(dwcl_lightgreen);
	const fcolor fclGreen(dwcl_green);
	const fcolor fclBlue(dwcl_blue);
	const fcolor fclDarkBlue(dwcl_darkblue);
	const fcolor fclTransparent(dwcl_transparent);

	const fcolor fclInvalid(numeric_limits<fcolor::value_type>::infinity());
};


#endif
