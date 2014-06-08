#ifndef dcmapapi_configure_h__
#define dcmapapi_configure_h__
#pragma once

#define DCMAP_W64

#if defined(_STRING_) || defined(_GLIBCXX_STRING)
#define DCMAP_STD_STRING
#endif

#ifdef _WINDOWS_
#define DCMAP_WINAPI
#endif

#define DCMAP_IMPL_HELPERS
#define DCMAP_TEMPLATE_HELPERS

//====================================================================
#if defined(_WIN32) || defined(_MSC_VER) || defined(__BORLANDC__)  // Windows port
//====================================================================
	#define  DCMAP_WINDOWS
	#define  DCMAP_UNICODE_SYS

	#define DCMAPAPI     __cdecl
	#define DCMAPDLLAPI  __cdecl

	#define DCMAP_INT64 __int64

	#define DCMAP_WCHAR_T_SIZE 2 

	#ifdef DCMAPSYS_INTERNAL
	#define DCMAPSYSEXPORT __declspec(dllexport) 
	#else
	#define DCMAPSYSEXPORT __declspec(dllimport) 
	#endif 

	#define DCMAPLIBAPI __cdecl

#endif

//====================================================================
#if defined(__GNUC__)    // Linux port
//====================================================================
	#define DCMAPAPI  
	#define DCMAPDLLAPI

	#define DCMAP_GNUC

	#define DCMAP_POSIX

	#define DCMAP_INT64 long long

	#undef  _stricmp
	#undef  stricmp
	#define stricmp strcasecmp
	#define _stricmp strcasecmp

	#define DCMAP_WCHAR_T_SIZE 4 

	#ifdef DCMAPSYS_INTERNAL
	#define DCMAPSYSEXPORT __attribute__ ((__visibility__("default")))
	#else
	#define DCMAPSYSEXPORT 
	#endif 

	#define DCMAPLIBAPI 

#endif


//====================================================================
#if defined(_MSC_VER)    // MSVC
//====================================================================

	#undef  DCMAP_W64
	#define DCMAP_W64 __w64

	#define DCMAP_MSVC

#endif

//====================================================================
#if defined(__BORLANDC__)  // BDS
//====================================================================
	#undef  _stricmp
	#define _stricmp stricmp

	#define DCMAP_BORLANDC

	#ifdef INC_VCL
	#	define DCMAP_VCL
	#endif

#endif

//====================================================================
#if defined(__GNUC__)    // Gnu C++
//====================================================================
	

#endif


#endif // dcmapapi_configure_h__
