#ifndef G_TYPES_H
#define G_TYPES_H
#pragma once

#include <typeinfo>

namespace gsys
{
#ifdef GSYS_USE_DOUBLE_REAL_NUMBERS
	typedef double gsys_real;
#else
	typedef float gsys_real;
#endif

	typedef void gsys_void;

    typedef long gsys_long;
    typedef unsigned long gsys_unsigned_long;
    typedef int gsys_int;
    typedef unsigned int gsys_unsigned_int;
	typedef unsigned gsys_unsigned;
    typedef short gsys_short;
    typedef unsigned short gsys_unsigned_short;
    typedef float gsys_float;
    typedef double gsys_double;
    typedef unsigned char gsys_unsigned_char;
    typedef unsigned short gsys_unsigned_short;
    typedef char gsys_char;
    typedef unsigned char gsys_unsigned_char;

    typedef bool gsys_bool;

    typedef unsigned long gsys_ulong;
    typedef unsigned long gsys_dword;
    typedef unsigned int gsys_uint;
    typedef unsigned char gsys_byte;
    typedef unsigned short gsys_word;

	typedef void* gsys_handle;
	static const gsys_handle invalid_handle = 0;

#ifdef GSYS_DETAIL__W64_SUPPORT
	typedef _w64 int  gsys_pt_diff;
	typedef _w64 unsigned int gsys_size_t;

#else
	typedef int  gsys_pt_diff;
	typedef unsigned int gsys_size_t;
#endif

	const gsys_pt_diff invalid_pt_diff = 0xffffffff;

#if defined(GSYS_DETAIL_TYPES_LONG_LONG_SUPPORT)
#	define GSYS_DETAIL_USE_64BIT_TYPES
	typedef long long gsys_long_long;
	typedef unsigned long long gsys_unsigned_long_long;
	typedef unsigned long long gsys_qword;
	typedef long long gsys_int64;
#elif defined(GSYS_DETAIL__INT64_SUPPORT)
#	define GSYS_DETAIL_USE_64BIT_TYPES
	typedef __int64 gsys_long_long;
	typedef unsigned __int64 gsys_unsigned_long_long;
	typedef unsigned __int64 gsys_qword;
	typedef __int64 gsys_int64;
#endif

	template<class T>
    char const* gsys_typename()
    {
        char const* name = typeid(T).name();
        return name;
    }

	#define GSYS_SET_TYPENAME(t,n)template<>inline char const* gsys_typename<t>(){return n"`";}

	GSYS_SET_TYPENAME(gsys_void,"")

	GSYS_SET_TYPENAME(gsys_float,"f")
	GSYS_SET_TYPENAME(gsys_int,"i")
	GSYS_SET_TYPENAME(gsys_uint,"u")
	GSYS_SET_TYPENAME(gsys_long,"l")
	GSYS_SET_TYPENAME(gsys_dword,"D")
	GSYS_SET_TYPENAME(gsys_double,"d")
	GSYS_SET_TYPENAME(gsys_word,"W")
	GSYS_SET_TYPENAME(gsys_byte,"B")
	GSYS_SET_TYPENAME(gsys_bool,"b")
	GSYS_SET_TYPENAME(gsys_char,"c")
	GSYS_SET_TYPENAME(gsys_short,"s")
#ifdef GSYS_DETAIL_USE_64BIT_TYPES
	GSYS_SET_TYPENAME(gsys_long_long,"l64")
	GSYS_SET_TYPENAME(gsys_unsigned_long_long,"ul64")
#endif
}

namespace gtypes
{
	typedef gsys::gsys_qword qword;
    typedef gsys::gsys_dword dword;
    typedef gsys::gsys_word word;

#ifdef GSYS_DETAIL_NO_UINT
    typedef gsys::gsys_uint uint;
#endif

#ifndef GSYS_DO_NOT_TYPEDEF_ULONG
    typedef gsys::gsys_ulong ulong;
#endif
    typedef gsys::gsys_byte gbyte;
    typedef signed char  g_char;

 #ifdef GSYS_DETAIL__W64_SUPPORT
    typedef _w64 int  pt_diff;
 #else

    typedef int  pt_diff;
 #endif

    typedef qword crc_t;

	// constants

	const ulong ulong_MAX = (ulong)0xfffffffful;
	const dword dword_MAX = (dword)0xfffffffful;
	const word  word_MAX  = 0xfffful;

	#define gsys_null nullptr
};

//typedef gtypes::gbyte gsys_byte;

#endif //G_TYPES_H
