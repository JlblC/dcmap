// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

//#ifndef _DEBUG
//#	define GSYS_DISABLE_LINK
//#endif
#define GSYS_DISABLE_MEMORY_MANAGER

#define STRICT

#include <g_indexed_list.h>

#include <map>
#include <set>
#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>

#define DCMAPSYS_INTERNAL

#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1 
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1

#ifdef _WIN32
#include "platform.h"
#endif

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

extern int g_DCMapNumPlanets;

extern dcmapDBREF g_NullDBREF;


