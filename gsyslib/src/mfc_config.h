
#ifndef GSYS_DISABLE_LINK
#	ifdef GSYS_DEBUG
#		pragma comment(lib ,"gsyslib_debug_mfc.lib")
#	else
#		pragma comment(lib ,"gsyslib_mfc.lib")
#	endif
#endif

#define GSYS_WINDOWS

#define NOMINMAX

#ifndef _AFXDLL
#	define _AFXDLL
#endif

#undef max
template<class _T1,class _T2> _T1 max(_T1 a,_T2 b){return a>b?a:b;}
#undef min
template<class _T1,class _T2> _T1 min(_T1 a,_T2 b){return a<b?a:b;}


#include "g_memory_manager.h"
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

//#define GSYS_DISABLE_MEMORY_MANAGER
