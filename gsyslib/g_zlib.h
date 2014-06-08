#ifndef GSYS_INC_G_ZLIB_H
#define GSYS_INC_G_ZLIB_H
#pragma once

#include "g_gsys.h"


#ifdef GSYS_ENABLE_ZLIB
#	if defined(GSYS_ZLIB_STATIC_LINK)
#		if defined(_DEBUG)
#			pragma comment(lib ,"zlibstat_d.lib")
#		else
#			if defined(_DLL)
#				pragma comment(lib ,"zlibstat.lib")
#			else
#				pragma comment(lib ,"zlibstat_sl.lib")
#			endif 
#		endif
#	else 
#		pragma comment(lib ,"zlibwapi.lib")
#	endif
#	define ZLIB_WINAPI
#	include "src/zlib/zlib.h"
#endif

#endif


