#pragma once
#ifndef GSYS_H
#define GSYS_H

// main file for gsys library
#define GSYS_INCLUDED

// configure library
#include "./src/configure.h"

#ifndef GSYS_DISABLE_STATIC_ASSERT
#	include "boost/static_assert.hpp"
#	define GSYS_STATIC_ASSERT(x)  BOOST_STATIC_ASSERT(x)
#else
#	define GSYS_STATIC_ASSERT(x)
#endif


// gtypes defines basic types used in gsys
#include "g_types.h"
namespace gsys{using namespace gtypes;}

#include "boost/type_traits.hpp"

// memory manager
#include "g_memory_manager.h"

#include "g_gsys_assert.h"

// basic utility 
#include "g_basic.h"
#include "g_utility.h"

// std include
#include <string>
namespace gsys{GSYS_SET_TYPENAME(std::string,"S");}
#include <istream>
#include <ostream>

namespace gsys{using namespace std;}


#endif // GSYS_H

