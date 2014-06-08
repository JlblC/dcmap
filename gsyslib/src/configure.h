#ifndef GSYS_INC_CONFIGURE_H
#define GSYS_INC_CONFIGURE_H
#pragma once

#include "../g_gsys_config.h"

#ifdef _DEBUG
#	define GSYS_DEBUG
#else
#	define GSYS_NDEBUG
#endif


#ifdef _WIN32
#	define GSYS_WINDOWS
#	define GSYS_WIN32
#	define GSYS_i586
#	define GSYS_i386
#endif

// Cpliler specifig config
#if defined(__BCPLUSPLUS__) || defined(__BORLANDC__)  // Borland C insane compiler ;)
#	include "borlandc_config.h"
#elif defined(_MSC_VER)
#	include "mcvs_config.h"
#elif defined(__GNUC__)
#	include "gnuc_config.h"
#endif


#ifdef GSYS_DETAIL_FORCE_FOR_LOCALSCOPE 
#	define for if(1)for
#endif

#ifdef GSYS_DISABLE_LINK
#	define GSYS_DISABLE_MEMORY_MANAGER 
#	define GSYS_DISABLE_CUSTOM_ASSERT
#	define GSYS_ENABLE_FORCE_INLINE
#endif

// to avoid boost error
//#define BOOST_NO_STRINGSTREAM

#if defined(GSYS_DEBUG) || defined(GSYS_FORCE_ASSERT)
#	define	GSYS_ENABLE_ASSERT
#endif

#ifdef GSYS_DISABLE_ASSERT
#	undef	GSYS_ENABLE_ASSERT
#endif

#ifdef GSYS_DISABLE_TIFF
#	undef GSYS_ENABLE_TIFF
#endif

#ifdef GSYS_DISABLE_PNG
#	undef GSYS_ENABLE_PNG
#endif

#ifdef GSYS_DISABLE_PYTHON
#	undef GSYS_ENABLE_PYTHON
#endif

#ifdef GSYS_DISABLE_MEMORY_MANAGER 
#	undef GSYS_ENABLE_MEMORY_MANAGER 
#endif

#ifdef GSYS_DISABLE_CUSTOM_ASSERT
#	undef GSYS_ENABLE_CUSTOM_ASSERT
#endif


#ifdef GSYS_ENABLE_FORCE_INLINE
#	define GSYSDETAIL_FORCE_INLINE
#	define GSYS_INC_API inline 
#else
#	define GSYS_INC_API  
#endif 

#ifdef GSYS_DISABLE_ZLIB
#	undef GSYS_ENABLE_ZLIB
#endif

// system constants
#define GSYS_MAX_PATH MAX_PATH

#define GSYS_ENABLE_VTID

// disable autolink of regexp library

#ifndef BOOST_ALL_NO_LIB
#	define BOOST_ALL_NO_LIB
#endif

#ifndef BOOST_PYTHON_STATIC_LINK
#	define BOOST_PYTHON_STATIC_LINK
#endif

#endif





