
#define GSYS_DETAIL_WCHAR_T_SIZE 2
#define GSYS_DETAIL_UNICODE_OS  


// Options specifig to borland compiler
#undef  _stricmp
#define _stricmp stricmp

#undef  _wcsicmp
#define _wcsicmp wcsicmp

#define GSYS_CALLBACK __cdecl
#define GSYS_API      __cdecl


#define GSYS_DETAIL_BDS2006
#define GSYS_DETAIL_BORLAND

#define GSYS_DETAIL_NO_STD_FLOAT_MATH

#define GSYS_DETAIL__INT64_SUPPORT

#define GSYS_DETAIL_LIB_OMF

#define GSYS_DETAIL_NO_UINT

// Test to avoid errors
#define GSYS_DISABLE_PNG

#define GSYS_DETAIL_FORCE_FOR_LOCALSCOPE

#ifndef GSYS_DISABLE_LINK
#	pragma comment(lib ,"gsyslib_bcb.lib")
#endif

#define BOOST_WINDOWS
#define GSYS_WINDOWS

#define GSYS_DISABLE_MEMORY_MANAGER

#define GSYS_DISABLE_STATIC_ASSERT

