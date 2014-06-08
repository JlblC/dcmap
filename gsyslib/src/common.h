#pragma once

// Standart C++ headers
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <map>

//#include <direct.h>
//#include <tchar.h>
//#include <io.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <fcntl.h>
#include <stdarg.h>

#define GSYS_INTERNAL

#include "../g_gsys.h"
#include "../g_alg.h"
#include "../g_assert.h"

#define BOOST_FILESYSTEM_NO_LIB

using namespace std;

namespace gsys{namespace detail{}}
using namespace gsys;
using namespace gsys::detail;

#ifndef MAKEFOURCC
#	define MAKEFOURCC(ch0, ch1, ch2, ch3)  \
	((dword)(gsys_byte)(ch0) | ((dword)(gsys_byte)(ch1) << 8) |  \
	((dword)(gsys_byte)(ch2) << 16) | ((dword)(gsys_byte)(ch3) << 24 ))
#endif




