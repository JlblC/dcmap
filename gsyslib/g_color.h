#pragma once
#ifndef G_COLOR_H
#define G_COLOR_H

#include "g_gsys.h"
#include "g_assert.h"
#include "g_text.h"
#include "g_alg.h"
#include "g_binstream.h"

#include "boost/operators.hpp"

#pragma pack(push)
#pragma pack(1)

namespace gsys
{
	using namespace gtypes;
    // constants
	struct dwcolor: public
				boost::multiplicative2<dwcolor,gsys_float,
				boost::multiplicative<dwcolor,
				boost::modable2<dwcolor,gsys_float,
				boost::modable<dwcolor,
				boost::totally_ordered<dwcolor
				> > > > >
   {
	   typedef gsys_byte value_type;
	   typedef ulong size_type;
	   GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
	   GSYSDETAIL_IMPLEMENT_STATIC_ARRAY_CONTAINER(values,4);

////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Data Access
       union
       {
            struct
            {
				value_type b;
                value_type g;
                value_type r;
                value_type a;
			};
			struct 
			{
				struct _rgb
				{
					value_type b;
					value_type g;
					value_type r;
				}rgb;
			};
            gsys_dword data;
			value_type values[4];
	   };

	   // g_opengl related

	   gsys_char const* as_chars()const{return (gsys_char const*)values;} 
	   gsys_char * as_chars(){return(gsys_char*)values;}

	   //operator g_char const*()const{return (g_char const*)values;} 
 	   //operator g_char *(){return (g_char *)values;}
       //operator dword()const{return data;}

	  gsys_dword as_rgb()const{return ((dword)(((gsys_byte)(r)|((word)((gsys_byte)(g))<<8))|(((dword)(gsys_byte)(b))<<16)));}
	  gsys_dword as_argb()const{return data;}
////////////////////////////////////////////////////////////////////////////////////

        dwcolor(){}

        dwcolor(dword dt){data=dt;}
        dwcolor(value_type _r,value_type _g, value_type _b, value_type _a=255):
            r(_r),g(_g),b(_b),a(_a){}

		bool operator == (dwcolor const& c) const
		{
			return c.data == data;
		}
		bool operator < (dwcolor const& c) const
		{
			return c.data < data;
		}
		//////////////////////////////////////////// operations
        dwcolor& set(value_type _r,value_type _g, value_type _b, value_type _a=255)
        {
            r = _r;
            g = _g;
            b = _b;
            a = _a;
 			return *this;
       }
		dwcolor& setrgb(gsys_dword rgb)
		{
			r =  GSYS_LOBYTE(rgb);
			g =  GSYS_LOBYTE(((gsys_word)(rgb)) >> 8);
			b =  GSYS_LOBYTE((rgb)>>16);
			a = 255;
			return *this;
		}
        dwcolor& setf(gsys_real _r,gsys_real _g, gsys_real _b, gsys_real _a=1.0f)
        {
            r = (value_type)(_r*255.0f);
            g = (value_type)(_g*255.0f);
            b = (value_type)(_b*255.0f);
            a = (value_type)(_a*255.0f);
 			return *this;
       }

		dwcolor& invert()
		{
			r = 255U-r;
			g = 255U-g;
			b = 255U-b;
			return *this;
		}

		dwcolor& operator *= (gsys_real f)
		{
			r = (value_type)(r*f);
			g = (value_type)(g*f);
			b = (value_type)(b*f);
			return *this;
		}

		dwcolor& operator *= (dwcolor const& cl)
		{
			r = (value_type)(r*(cl.r/255.0f));
			g = (value_type)(g*(cl.g/255.0f));
			b = (value_type)(b*(cl.b/255.0f));
			return *this;
		}

		dwcolor& operator %= (float f)
		{
			r = (value_type)(r*f);
			g = (value_type)(g*f);
			b = (value_type)(b*f);
			a = (value_type)(a*f);
			return *this;
		}

		dwcolor& operator %= (dwcolor const& cl)
		{
			r = (value_type)(r*(cl.r/255.0f));
			g = (value_type)(g*(cl.g/255.0f));
			b = (value_type)(b*(cl.b/255.0f));
			a = (value_type)(a*(cl.a/255.0f));
			return *this;
		}


		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(dwcolor);
    };

	inline std::ostream& operator<<(std::ostream& os,dwcolor const& cl)
	{
		return os << "d{" << (int)cl.r << "," << (int)cl.g << "," << (int)cl.b << "," << (int)cl.a << "}";
	}
	inline std::istream& operator>>(std::istream& is,dwcolor & cl)
	{
		int r=0,g=0,b=0,a=255;
        wsp_delim w_bo('{');
        wsp_delim w_bc('}');
        wsp_delim w_d(',');

        is >> ws;
        char c = is.peek();

        if(c == 'd') // use desimal 0-255 format
        {
            is.ignore(); // default processing
        }
        else if(c == 'f') // use floating point f 0.0-1.0 format
        {
			is.ignore();
            gsys_real fR,fG,fB,fA;
		    is >> w_bo >> ISN(fR) >> w_d >> ISN(fG) >> w_d >> ISN(fB) >> ws;
			c = is.peek();
            if(c == ','){is >> w_d >> ISN(fA) >> w_bc;}
            else if(c == '}'){is >> w_bc;fA = 1.0f;}
            else {is.setstate( std::ios::failbit);return is;};
		    cl.setf(fR,fG,fB,fA);
			return is;
        }
        else if(c != '{') // unknown format
        {
            is.setstate( std::ios::failbit );
            return is;
        }

		is >> w_bo >> ISN(r) >> w_d >> ISN(g) >> w_d >> ISN(b) >> ws;
		c = is.peek(); // alpha component optional
		if(c == ','){is >> w_d >> ISN(a) >> w_bc;}
		else if(c == '}'){is >> w_bc;a = 255;}
		else {is.setstate( std::ios::failbit);return is;};
		cl.set(r,g,b,a);
		return is;
	}

	inline dwcolor& lerp(dwcolor c1,dwcolor c2,gsys_real f,dwcolor &r)
	{
		r.r = c1.r+dwcolor::value_type((c2.r-c1.r)*f);
		r.g = c1.g+dwcolor::value_type((c2.g-c1.g)*f);
		r.b = c1.b+dwcolor::value_type((c2.b-c1.b)*f);
		r.a = c1.a+dwcolor::value_type((c2.a-c1.a)*f);
		return r;
	}
	inline dwcolor lerp(dwcolor c1,dwcolor c2,gsys_real f)
		{dwcolor r;return lerp(c1,c2,f,r);}

	inline dwcolor& lerp_rgb(dwcolor c1,dwcolor c2,gsys_real f,dwcolor &r)
	{
		r.r = c1.r+dwcolor::value_type((c2.r-c1.r)*f);
		r.g = c1.g+dwcolor::value_type((c2.g-c1.g)*f);
		r.b = c1.b+dwcolor::value_type((c2.b-c1.b)*f);
		r.a = 255;
		return r;
	}
	inline dwcolor lerp_rgb(dwcolor c1,dwcolor c2,gsys_real f)
	{dwcolor r;return lerp_rgb(c1,c2,f,r);}

	// Basic Color Constans
	const dwcolor dwcl_transparent(0,0,0,0);

	const dwcolor dwcl_aliceblue(240,248,255);	
	const dwcolor dwcl_darkslategray(47,79,79);	
	const dwcolor dwcl_lightsalmon(255,160,122);	
	const dwcolor dwcl_palevioletred(219,112,147);

	const dwcolor dwcl_antiquewhite(250,235,215);	
	const dwcolor dwcl_darkturquoise(0,206,209);	
	const dwcolor dwcl_lightseagreen(32,178,170);	
	const dwcolor dwcl_papayawhip(255,239,213);

	const dwcolor dwcl_aqua(0,255,255);	
	const dwcolor dwcl_darkviolet(148,0,211);	
	const dwcolor dwcl_lightskyblue(135,206,250);	
	const dwcolor dwcl_peachpuff(255,239,213);

	const dwcolor dwcl_aquamarine(127,255,212);	
	const dwcolor dwcl_deeppink(255,20,147);	
	const dwcolor dwcl_lightslategray(119,136,153);	
	const dwcolor dwcl_peru(205,133,63);

	const dwcolor dwcl_azure(240,255,255);	
	const dwcolor dwcl_deepskyblue(0,191,255);	
	const dwcolor dwcl_lightsteelblue(176,196,222);	
	const dwcolor dwcl_pink(255,192,203);

	const dwcolor dwcl_beige(245,245,220);	
	const dwcolor dwcl_dimgray(105,105,105);	
	const dwcolor dwcl_lightyellow(255,255,224);	
	const dwcolor dwcl_plum(221,160,221);

	const dwcolor dwcl_bisque(255,228,196);	
	const dwcolor dwcl_dodgerblue(30,144,255);	
	const dwcolor dwcl_lime(0,255,0);	
	const dwcolor dwcl_powderblue(176,224,230);

	const dwcolor dwcl_black(0,0,0);	
	const dwcolor dwcl_firebrick(178,34,34);	
	const dwcolor dwcl_limegreen(50,205,50);	
	const dwcolor dwcl_purple(128,0,128);

	const dwcolor dwcl_blanchedalmond(255,255,205);	
	const dwcolor dwcl_floralwhite(255,250,240);	
	const dwcolor dwcl_linen(250,240,230);	
	const dwcolor dwcl_red(255,0,0);
	const dwcolor dwcl_blue(0,0,255);	
	const dwcolor dwcl_forestgreen(34,139,34);	
	const dwcolor dwcl_magenta(255,0,255);	
	const dwcolor dwcl_rosybrown(188,143,143);

	const dwcolor dwcl_blueviolet(138,43,226);	
	const dwcolor dwcl_fuchsia(255,0,255);	
	const dwcolor dwcl_maroon(128,0,0);	
	const dwcolor dwcl_royalblue(65,105,225);

	const dwcolor dwcl_brown(165,42,42);	
	const dwcolor dwcl_gainsboro(220,220,220);	
	const dwcolor dwcl_mediumaquamarine(102,205,170);	
	const dwcolor dwcl_saddlebrown(139,69,19);

	const dwcolor dwcl_burlywood(222,184,135);	
	const dwcolor dwcl_ghostwhite(248,248,255);	
	const dwcolor dwcl_mediumblue(0,0,205);	
	const dwcolor dwcl_salmon(250,128,114);

	const dwcolor dwcl_cadetblue(95,158,160);	
	const dwcolor dwcl_gold(255,215,0);	
	const dwcolor dwcl_mediumorchid(186,85,211);	
	const dwcolor dwcl_sandybrown(244,164,96);

	const dwcolor dwcl_chartreuse(127,255,0);	
	const dwcolor dwcl_goldenrod(218,165,32);	
	const dwcolor dwcl_mediumpurple(147,112,219);	
	const dwcolor dwcl_seagreen(46,139,87);

	const dwcolor dwcl_chocolate(210,105,30);	
	const dwcolor dwcl_gray(128,128,128);	
	const dwcolor dwcl_mediumseagreen(60,179,113);	
	const dwcolor dwcl_seashell(255,245,238);

	const dwcolor dwcl_coral(255,127,80);	
	const dwcolor dwcl_green(0,128,0);	
	const dwcolor dwcl_mediumslateblue(123,104,238);	
	const dwcolor dwcl_sienna(160,82,45);

	const dwcolor dwcl_cornflowerblue(100,149,237);	
	const dwcolor dwcl_greenyellow(173,255,47);	
	const dwcolor dwcl_mediumspringgreen(0,250,154);	
	const dwcolor dwcl_silver(192,192,192);

	const dwcolor dwcl_cornsilk(255,248,220);	
	const dwcolor dwcl_honeydew(240,255,240);	
	const dwcolor dwcl_mediumturquoise(72,209,204);	
	const dwcolor dwcl_skyblue(135,206,235);

	const dwcolor dwcl_crimson(220,20,60);	
	const dwcolor dwcl_hotpink(255,105,180);	
	const dwcolor dwcl_mediumvioletred(199,21,133);	
	const dwcolor dwcl_slateblue(106,90,205);

	const dwcolor dwcl_cyan(0,255,255);	
	const dwcolor dwcl_indianred(205,92,92);	
	const dwcolor dwcl_midnightblue(25,25,112);	
	const dwcolor dwcl_slategray(112,128,144);

	const dwcolor dwcl_darkblue(0,0,139);	
	const dwcolor dwcl_indigo(75,0,130);	
	const dwcolor dwcl_mintcream(245,255,250);	
	const dwcolor dwcl_snow(255,250,250);

	const dwcolor dwcl_darkcyan(0,139,139);	
	const dwcolor dwcl_ivory(255,240,240);	
	const dwcolor dwcl_mistyrose(255,228,225);	
	const dwcolor dwcl_springgreen(0,255,127);

	const dwcolor dwcl_darkgoldenrod(184,134,11);	
	const dwcolor dwcl_khaki(240,230,140);	
	const dwcolor dwcl_moccasin(255,228,181);	
	const dwcolor dwcl_steelblue(70,130,180);

	const dwcolor dwcl_darkgray(169,169,169);	
	const dwcolor dwcl_lavender(230,230,250);	
	const dwcolor dwcl_navajowhite(255,222,173);	
	const dwcolor dwcl_tan(210,180,140);

	const dwcolor dwcl_darkgreen(0,100,0);	
	const dwcolor dwcl_lavenderblush(255,240,245);	
	const dwcolor dwcl_navy(0,0,128);	
	const dwcolor dwcl_teal(0,128,128);

	const dwcolor dwcl_darkkhaki(189,183,107);	
	const dwcolor dwcl_lawngreen(124,252,0);	
	const dwcolor dwcl_oldlace(253,245,230);	
	const dwcolor dwcl_thistle(216,191,216);

	const dwcolor dwcl_darkmagenta(139,0,139);	
	const dwcolor dwcl_lemonchiffon(255,250,205);	
	const dwcolor dwcl_olive(128,128,0);	
	const dwcolor dwcl_tomato(253,99,71);

	const dwcolor dwcl_darkolivegreen(85,107,47);	
	const dwcolor dwcl_lightblue(173,216,230);	
	const dwcolor dwcl_olivedrab(107,142,35);	
	const dwcolor dwcl_turquoise(64,224,208);

	const dwcolor dwcl_darkorange(255,140,0);	
	const dwcolor dwcl_lightcoral(240,128,128);	
	const dwcolor dwcl_orange(255,165,0);	
	const dwcolor dwcl_violet(238,130,238);

	const dwcolor dwcl_darkorchid(153,50,204);	
	const dwcolor dwcl_lightcyan(224,255,255);
	const dwcolor dwcl_orangered(255,69,0);	
	const dwcolor dwcl_wheat(245,222,179);

	const dwcolor dwcl_darkred(139,0,0);	
	const dwcolor dwcl_lightgoldenrodyellow(250,250,210);	
	const dwcolor dwcl_orchid(218,112,214);	
	const dwcolor dwcl_white(255,255,255);

	const dwcolor dwcl_darksalmon(233,150,122);	
	const dwcolor dwcl_lightgreen(144,238,144);	
	const dwcolor dwcl_palegoldenrod(238,232,170);	
	const dwcolor dwcl_whitesmoke(245,245,245);

	const dwcolor dwcl_darkseagreen(143,188,143);	
	const dwcolor dwcl_lightgrey(211,211,211);	
	const dwcolor dwcl_palegreen(152,251,152);	
	const dwcolor dwcl_yellow(255,255,0);

	const dwcolor dwcl_darkslateblue(72,61,139);	
	const dwcolor dwcl_lightpink(255,182,193);	
	const dwcolor dwcl_paleturquoise(175,238,238);
}

#pragma pack(pop)

#endif
