#pragma once
//#define _stricmp stricmp

#define GSYS_CALLBACK __cdecl
#define GSYS_API      __cdecl

#define GSYS_CDECL    __cdecl

#define GSYS_DETAIL_WCHAR_T_SIZE 2
#define GSYS_DETAIL_UNICODE_OS  

#if _MSC_VER >= 1300 && 0
#	define GSYS_DETAIL__W64_SUPPORT
#	define GSYS_DETAIL_HAS_STDEXT_HASH_MAP
#endif

#if _MSC_VER >= 1400 && !defined(_CRT_SECURE_NO_DEPRECATE)
#	define _CRT_SECURE_NO_DEPRECATE
#endif 

#define GSYS_DETAIL_TYPES_INT64_SUPPORT

#define GSYS_DETAIL_TYPES_LONG_LONG_SUPPORT

#define GSYS_DETAIL_INLINE_ASM_ENABLE

#define GSYS_DETAIL_NO_UINT


#ifdef GSYS_DEBUG
#define GSYSLIB_DEBUG
#endif

/*
#if defined(_AFX) || defined(GSYS_MFC) 
#	include "mfc_config.h"
#else
#	if !defined(GSYS_DISABLE_LINK) & !defined(GSYS_INTERNAL)
#		ifdef GSYSLIB_DEBUG
#			if defined(_DLL)
#				pragma comment(lib ,"gsyslib_debug.lib")
#			else
#				pragma comment(lib ,"gsyslib_debug_sl.lib")
#			endif 
#		else
#			ifdef GSYSLIB_PRODUCTION
#				if defined(_DLL)
#					pragma comment(lib ,"gsyslib_production.lib")
#				else
#					pragma comment(lib ,"gsyslib_production_sl.lib")
#				endif 
#			else
#				if defined(_DLL)
#					pragma comment(lib ,"gsyslib.lib")
#				else
#					pragma comment(lib ,"gsyslib_sl.lib")
#				endif 
#			endif
#		endif
#	endif
#endif
*/
#define GSYS_WINDOWS

#define GSYS_DETAIL_HAVE__COUNTER__ 1

