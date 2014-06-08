
#include "common.h"

#include "../g_vtid_lib.h"
#include "../g_vtid_var.h"

#include "../g_vector2.h"
#include "../g_vector3.h"
#include "../g_vector4.h"
#include "../g_matrix4.h"
#include "../g_point.h"
#include "../g_rect.h"
#include "../g_fcolor.h"
#include "../g_quaternion.h"
#include "../g_transform2d.h"
#include "../g_transform_quat.h"
#include "../g_sphere.h"
#include "../g_range.h"
#include "boost/regex.hpp"

/*
template <class charT, class traits >
std::basic_istream<charT,traits> & operator >> (std::basic_istream<charT,traits>& is,
												boost::basic_regex<charT,traits>& r)
{
	std::basic_string<charT,traits> str;
	is >> str;
	r.assign(str);
	return is;
}

template <class charT, class traits >
std::basic_ostream<charT,traits>& operator << (std::basic_ostream<charT,traits>& os, 
											   boost::basic_regex<charT,traits> const& r)
{
	return os << r.str();
}
*/


std::istream & operator >> (std::istream& is,
							boost::regex& r)
{
	std::string str;
	is >> str;
	r.assign(str);
	return is;
}

std::ostream& operator << (std::ostream& os, 
						   boost::regex const& r)
{
	return os << r.str();
}




#define REG_POINT(T,alias)\
	GSYS_VTID_BEGIN_DYNAMIC(gsys::tPoint<T>)\
		GSYS_VTID_ALIAS(alias)\
		GSYS_VTID_CREATEBLE1(gsys::tPoint<T>, gsys::tPoint<T>(0,0))\
		GSYS_VTID_COPYABLE\
		GSYS_VTID_SERIALIZEBLE\
		GSYS_VTID_STRINGIZEBLE\
		GSYS_VTID_DECL_AUTONAME(x)\
		GSYS_VTID_DECL_AUTONAME(y)\
	GSYS_VTID_END_DYNAMIC  

#define REG_RECT(T,alias)\
	GSYS_VTID_BEGIN_DYNAMIC(gsys::tRect<T>)\
		GSYS_VTID_CREATEBLE1(gsys::tRect<T>,gsys::tRect<T>(0,0,0,0))\
		GSYS_VTID_COPYABLE\
		GSYS_VTID_STRINGIZEBLE\
		GSYS_VTID_SERIALIZEBLE\
		GSYS_VTID_ALIAS(alias)\
		GSYS_VTID_DECL_AUTONAME(left)\
		GSYS_VTID_DECL_AUTONAME(top)\
		GSYS_VTID_DECL_AUTONAME(right)\
		GSYS_VTID_DECL_AUTONAME(bottom)\
		GSYS_VTID_MEMBER_PROC4(set,"{}",T,T,T,T)\
		GSYS_VTID_MEMBER_PROC4(init_by_size,"s{}",T,T,T,T)\
		GSYS_VTID_MEMBER_PROC4(init_by_center_abs_size,"c{}",T,T,T,T)\
	GSYS_VTID_END_DYNAMIC  

void gsys::vtid_lib::register_foundamental()
{
	REG_RECT(gsys_int,"rect");
	REG_RECT(gsys_float,"frect");
	REG_RECT(gsys_word,"wrect");
	REG_RECT(gsys_dword,"dwrect");
	REG_RECT(gsys_short,"srect");
	REG_RECT(gsys_long,"lrect");
	REG_RECT(gsys_byte,"btrect");
	REG_RECT(gsys_char,"chrect");
	REG_RECT(gsys_bool,"brect");
	REG_RECT(gsys_qword,"qrect");

	REG_POINT(gsys_int,"point");
	REG_POINT(gsys_float,"fpoint");
	REG_POINT(gsys_word,"wpoint");
	REG_POINT(gsys_dword,"dwpoint");
	REG_POINT(gsys_short,"spoint");
	REG_POINT(gsys_long,"lpoint");
	REG_POINT(gsys_byte,"btpoint");
	REG_POINT(gsys_char,"chpoint");
	REG_POINT(gsys_bool,"bpoint");
	REG_POINT(gsys_qword,"qpoint");
}

void gsys::vtid_lib::register_color()
{
	GSYS_VTID_BEGIN_DYNAMIC(gsys::dwcolor)
		GSYS_VTID_ALIAS("dwcolor")
		GSYS_VTID_CREATEBLE1(gsys::dwcolor,gsys::dwcl_black)
		GSYS_VTID_COPYABLE
		GSYS_VTID_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE

		GSYS_VTID_DECL_AUTONAME(r)
		GSYS_VTID_DECL_AUTONAME(g)
		GSYS_VTID_DECL_AUTONAME(b)
		GSYS_VTID_DECL_AUTONAME(a)

		GSYS_VTID_DECL_ARRAY(values,"element")
	
	GSYS_VTID_END_DYNAMIC  

	GSYS_VTID_BEGIN_DYNAMIC(gsys::fcolor)
		GSYS_VTID_ALIAS("fcolor")
		GSYS_VTID_ALIAS("color")
		GSYS_VTID_CREATEBLE1(gsys::fcolor,gsys::fclBlack)
		GSYS_VTID_COPYABLE
		GSYS_VTID_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE

		GSYS_VTID_DECL_AUTONAME(r)
		GSYS_VTID_DECL_AUTONAME(g)
		GSYS_VTID_DECL_AUTONAME(b)
		GSYS_VTID_DECL_AUTONAME(a)

		GSYS_VTID_DECL_ARRAY(values,"element")

	GSYS_VTID_END_DYNAMIC  
}

template<class T>
void register_range()
{
	GSYS_VTID_BEGIN_DYNAMIC(T)
		GSYS_VTID_CREATEBLE
		GSYS_VTID_COPYABLE
		GSYS_VTID_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE

		GSYS_VTID_DECL_AUTONAME(from)
		GSYS_VTID_DECL_AUTONAME(to)
	GSYS_VTID_END_DYNAMIC  
};

void gsys::vtid_lib::register_geometry()
{
	GSYS_VTID_BEGIN_DYNAMIC(gsys::matrix4)
	{
		GSYS_VTID_ALIAS("matrix4");
		GSYS_VTID_ALIAS("mat4");
		GSYS_VTID_CREATEBLE0(gsys::matrix4);
		GSYS_VTID_COPYABLE;
		GSYS_VTID_STRINGIZEBLE;
		GSYS_VTID_SERIALIZEBLE;

		GSYS_VTID_DECL_AUTONAME(_11);
		GSYS_VTID_DECL_AUTONAME(_12);
		GSYS_VTID_DECL_AUTONAME(_13);
		GSYS_VTID_DECL_AUTONAME(_14);

		GSYS_VTID_DECL_AUTONAME(_21);
		GSYS_VTID_DECL_AUTONAME(_22);
		GSYS_VTID_DECL_AUTONAME(_23);
		GSYS_VTID_DECL_AUTONAME(_24);

		GSYS_VTID_DECL_AUTONAME(_31);
		GSYS_VTID_DECL_AUTONAME(_32);
		GSYS_VTID_DECL_AUTONAME(_33);
		GSYS_VTID_DECL_AUTONAME(_34);

		GSYS_VTID_DECL_AUTONAME(_41);
		GSYS_VTID_DECL_AUTONAME(_42);
		GSYS_VTID_DECL_AUTONAME(_43);
		GSYS_VTID_DECL_AUTONAME(_44);

		GSYS_VTID_DECL_ARRAY(values,"element")
	}
	GSYS_VTID_END_DYNAMIC 

	GSYS_VTID_BEGIN_DYNAMIC(gsys::vector2)
		GSYS_VTID_ALIAS("vector2")
		GSYS_VTID_ALIAS("vec2")
		GSYS_VTID_CREATEBLE1(gsys::vector2,gsys::vector2_ZERO)
		GSYS_VTID_COPYABLE
		GSYS_VTID_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE

		GSYS_VTID_DECL_AUTONAME(x)
		GSYS_VTID_DECL_AUTONAME(y)

		GSYS_VTID_MEMBER_PROC(norm,"norm")
		GSYS_VTID_DECL_ARRAY(values,"element")

	GSYS_VTID_END_DYNAMIC  

	GSYS_VTID_BEGIN_DYNAMIC(gsys::vector3)
		GSYS_VTID_ALIAS("vector3")
		GSYS_VTID_ALIAS("vec3")
		GSYS_VTID_CREATEBLE1(gsys::vector3,gsys::vector3_ZERO)
		GSYS_VTID_COPYABLE
		GSYS_VTID_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE

		GSYS_VTID_DECL_AUTONAME(x)
		GSYS_VTID_DECL_AUTONAME(y)
		GSYS_VTID_DECL_AUTONAME(z)

		GSYS_VTID_DECL_ARRAY(values,"element")
		GSYS_VTID_MEMBER_PROC(norm,"norm")
	GSYS_VTID_END_DYNAMIC  

	GSYS_VTID_BEGIN_DYNAMIC(gsys::vector4)
		GSYS_VTID_ALIAS("vector4")
		GSYS_VTID_ALIAS("vec4")
		GSYS_VTID_CREATEBLE1(gsys::vector4,gsys::vector4_ZERO)
		GSYS_VTID_COPYABLE
		GSYS_VTID_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE

		GSYS_VTID_DECL_AUTONAME(x)
		GSYS_VTID_DECL_AUTONAME(y)
		GSYS_VTID_DECL_AUTONAME(z)
		GSYS_VTID_DECL_AUTONAME(w)


		GSYS_VTID_DECL_ARRAY(values,"element")
	GSYS_VTID_END_DYNAMIC  

	GSYS_VTID_BEGIN_DYNAMIC(gsys::quaternion)
		GSYS_VTID_ALIAS("quaternion")
		GSYS_VTID_ALIAS("quat")
		GSYS_VTID_CREATEBLE1(gsys::quaternion,gsys::quaternion_IDENTITY)
		GSYS_VTID_COPYABLE
		GSYS_VTID_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE

		GSYS_VTID_DECL_AUTONAME(x)
		GSYS_VTID_DECL_AUTONAME(y)
		GSYS_VTID_DECL_AUTONAME(z)
		GSYS_VTID_DECL_AUTONAME(w)

		GSYS_VTID_DECL_ARRAY(values,"element")
	GSYS_VTID_END_DYNAMIC  

	GSYS_VTID_BEGIN_DYNAMIC(gsys::sphere)
	{
		GSYS_VTID_ALIAS("sphere3")
			GSYS_VTID_ALIAS("sphere")
			GSYS_VTID_COPYABLE
			GSYS_VTID_STRINGIZEBLE
			GSYS_VTID_SERIALIZEBLE

			GSYS_VTID_DECLA(center);
			GSYS_VTID_DECLA(radius);

			//		GSYS_VTID_MEMBER_FUNC(len,"len",gsys::vector2::value_type)

			GSYS_VTID_CREATEBLE1(gsys::sphere,gsys::sphere(gsys::vector3_ZERO,0))
	}
	GSYS_VTID_END_DYNAMIC

	GSYS_VTID_BEGIN_DYNAMIC(gsys::transform_quat)
	{
		GSYS_VTID_ALIAS("transform_quat")
			GSYS_VTID_COPYABLE
			GSYS_VTID_STRINGIZEBLE
			GSYS_VTID_SERIALIZEBLE
			GSYS_VTID_CREATEBLE1(gsys::transform_quat,gsys::transform_quat(gsys::vector3_ZERO,gsys::quaternion_IDENTITY))

			GSYS_VTID_DECL(position,"position");
		GSYS_VTID_DECL(position,"pos");
		GSYS_VTID_DECL(rotation,"rotation");
		GSYS_VTID_DECL(rotation,"rot");
	}
	GSYS_VTID_END_DYNAMIC 

	GSYS_VTID_BEGIN_DYNAMIC(gsys::iang_t)
		GSYS_VTID_ALIAS("iang_t")
		GSYS_VTID_ALIAS("iang")
		GSYS_VTID_CREATEBLE1(gsys::iang_t,0)
		GSYS_VTID_COPYABLE
		GSYS_VTID_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE
	GSYS_VTID_END_DYNAMIC  

	GSYS_VTID_BEGIN_DYNAMIC(gsys::transform2_iang)
		GSYS_VTID_ALIAS("transform2_iang")
		GSYS_VTID_ALIAS("trns2")
		GSYS_VTID_CREATEBLE
		GSYS_VTID_COPYABLE
		GSYS_VTID_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE
	GSYS_VTID_END_DYNAMIC  

	GSYS_VTID_BEGIN_DYNAMIC(gsys::transform22d)
		GSYS_VTID_ALIAS("transform22d")
		GSYS_VTID_ALIAS("trns22d")
		GSYS_VTID_CREATEBLE
		GSYS_VTID_COPYABLE
		GSYS_VTID_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE
	GSYS_VTID_END_DYNAMIC 

	register_range<frange>();
	register_range<irange>();
	register_range<urange>();

	GSYS_VTID_BEGIN_DYNAMIC(::boost::regex)
	{
		GSYS_VTID_TYPENAME("regex")
		GSYS_VTID_CREATEBLE
		GSYS_VTID_COPYABLE
		GSYS_VTID_STRINGIZEBLE;
	}
	GSYS_VTID_END_DYNAMIC 
}

#include "g_vtid_ref_map.h"

void gsys::vtid_lib::register_helpers()
{
	GSYS_VTID_BEGIN_DYNAMIC(::gsys::vtid_ref_map)
	{
		GSYS_VTID_USE_CLASS_ACCESS_PROC(OnGetVar)
	}
	GSYS_VTID_END_DYNAMIC

	GSYS_VTID_BEGIN_DYNAMIC(::gsys::vtid_valmap)
	{
		GSYS_VTID_USE_CLASS_ACCESS_PROC(OnSetValue);
	}
	GSYS_VTID_END_DYNAMIC

	GSYS_VTID_BEGIN_DYNAMIC(::gsys::vtid_const)
	{
		GSYS_VTID_USE_CLASS_ACCESS_PROC(OnDefConst);
	}
	GSYS_VTID_END_DYNAMIC
}


