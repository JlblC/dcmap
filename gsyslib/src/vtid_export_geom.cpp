
#include "common.h"

#ifndef __BORLANDC__


#include "../g_vtid_lib.h"

#include "../g_vector3.h"
#include "../g_vector4.h"
#include "../g_matrix4.h"
#include "../g_point.h"
#include "../g_rect.h"
#include "../g_fcolor.h"
#include "../g_quaternion.h"
#include "../g_sphere.h"
#include "../g_transform_quat.h"

#include "../g_pooled_string.h"
#include "../g_gstring.h"

namespace gsys
{
	namespace detail
	{
		void toggle(gsys_bool &v){v = !v;}
	}
}

static void reg_some()
{
	GSYS_VTID_BEGIN_DYNAMIC(gsys_bool)
	{
		GSYS_VTID_ALIAS("bool")
        GSYS_VTID_ALIAS("boolean")
        GSYS_VTID_CREATEBLE1(bool,false)
        GSYS_VTID_STRINGIZEBLE
        GSYS_VTID_SERIALIZEBLE
        GSYS_VTID_COPYABLE
        GSYS_VTID_EQUAL_COMPARABLE

		GSYS_VTID_PROCESSOR_PROC(gsys::detail::toggle,"toggle");
	}
	GSYS_VTID_END_DYNAMIC

    GSYS_VTID_BEGIN_DYNAMIC(void)
    {
      GSYS_VTID_ALIAS("void")
      GSYS_VTID_ALIAS("nil")
    }
    GSYS_VTID_END_DYNAMIC
}


void gsys::detail::register_strings()
{
    reg_some();

    GSYS_VTID_BEGIN_DYNAMIC(std::string)
    {
      GSYS_VTID_TYPENAME("string")
      GSYS_VTID_CREATEBLE
      GSYS_VTID_STRINGIZEBLE
      GSYS_VTID_SERIALIZEBLE
      GSYS_VTID_COPYABLE
      GSYS_VTID_COMPARABLE
    }
    GSYS_VTID_END_DYNAMIC

	GSYS_VTID_BEGIN_DYNAMIC(std::wstring)
	{
		GSYS_VTID_TYPENAME("wstring")
		GSYS_VTID_CREATEBLE
		GSYS_VTID_WIDE_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE
		GSYS_VTID_COPYABLE
		GSYS_VTID_COMPARABLE
	}
	GSYS_VTID_END_DYNAMIC

	GSYS_VTID_BEGIN_DYNAMIC(gsys::gstring)
	{
		GSYS_VTID_TYPENAME("gstring")
		GSYS_VTID_CREATEBLE
		GSYS_VTID_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE
		GSYS_VTID_COPYABLE
		GSYS_VTID_COMPARABLE
	}
	GSYS_VTID_END_DYNAMIC

	GSYS_VTID_BEGIN_DYNAMIC(gsys::wgstring)
	{
		GSYS_VTID_TYPENAME("wgstring")
		GSYS_VTID_CREATEBLE
		GSYS_VTID_WIDE_STRINGIZEBLE
		GSYS_VTID_SERIALIZEBLE
		GSYS_VTID_COPYABLE
		GSYS_VTID_COMPARABLE
	}
	GSYS_VTID_END_DYNAMIC

	GSYS_VTID_BEGIN_DYNAMIC(gsys::pooled_string)
	{
		GSYS_VTID_TYPENAME("pooled_string");
		GSYS_VTID_CREATEBLE;
		GSYS_VTID_STRINGIZEBLE;
		GSYS_VTID_SERIALIZEBLE;
		GSYS_VTID_COPYABLE;
		GSYS_VTID_COMPARABLE;
	}
	GSYS_VTID_END_DYNAMIC

	GSYS_VTID_BEGIN_DYNAMIC(gsys::pooled_wstring)
	{
		GSYS_VTID_TYPENAME("pooled_wstring");
		GSYS_VTID_CREATEBLE;
		GSYS_VTID_WIDE_STRINGIZEBLE;
		GSYS_VTID_SERIALIZEBLE;
		GSYS_VTID_COPYABLE;
		GSYS_VTID_COMPARABLE;
	}
	GSYS_VTID_END_DYNAMIC
}

#endif
