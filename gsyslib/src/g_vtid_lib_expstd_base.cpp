#include "common.h"

#include "../g_vtid_lib.h"
#include "../g_vtid_util.h"

#define DECL_BASE_ST(x) GSYS_VTID_BEGIN_DYNAMIC(x) GSYS_VTID_STRINGIZEBLE GSYS_VTID_SERIALIZEBLE GSYS_VTID_CREATEBLE1(x,0) GSYS_VTID_COPYABLE GSYS_VTID_COMPARABLE
#define DECL_BASE(x,y) DECL_BASE_ST(x) GSYS_VTID_END_DYNAMIC
#define DECL_BASE1(x,a1) DECL_BASE_ST(x)	GSYS_VTID_ALIAS(a1)	GSYS_VTID_END_DYNAMIC
#define DECL_BASE2(x,a1,a2) DECL_BASE_ST(x)	GSYS_VTID_ALIAS(a1)	GSYS_VTID_ALIAS(a2) GSYS_VTID_END_DYNAMIC
#define DECL_BASET(x) DECL_BASE(x,#x)


void gsys::vtid_lib::register_base_types()
{


    DECL_BASE1(gsys_char,"char")
    DECL_BASE1(gsys_short,"short")
    DECL_BASE1(gsys_long,"long")
	DECL_BASE1(gsys_double,"double")
	DECL_BASE1(gsys_unsigned_int,"uint")


  /*

	DECL_BASE2(gsys_float,"float","real")
	DECL_BASE2(gsys_unsigned_char,"byte","uchar")
	DECL_BASE2(gsys_unsigned_short,"ushort","word")
	DECL_BASE2(gsys_unsigned_long,"ulong","dword")
#ifdef GSYS_DETAIL_USE_64BIT_TYPES
	DECL_BASE2(gsys_long_long,"int64","longlong")
	DECL_BASE2(gsys_unsigned_long_long,"qword","uint64")
#endif
  */

    register_strings();
    register_helpers();
}

