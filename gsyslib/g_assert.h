#pragma once

#include "g_gsys_assert.h"

#ifdef GSYS_ENABLE_ASSERT
#	undef _ASSERT
#	define _ASSERT(_exp) GSYS_ASSERT(_exp)
#   define _ASSERTOBJ(x) GSYS_ASSERTOBJ(x)
#   define _ASSERTREF(x) GSYS_ASSERTREF(x)
#   define _ASSERTPT(x,s) GSYS_ASSERTPT(x,s)
#   define _ASSERTPTR(x) GSYS_ASSERTPTR(x) 
#   define _ASSERTSTR(x) GSYS_ASSERTSTR(x)
#   define _CFN GSYS_CFN
#   define _ASSERTOBJ_NULL(x) GSYS_ASSERTOBJ_NULL(x) 
#   define _ASSERTPT_NULL(x,s) GSYS_ASSERTPT_NULL(x,s)
#   define _ASSERTPTR_NULL(x) GSYS_ASSERTPTR_NULL(x) 
#   define _ASSERTSTR_NULL(x) GSYS_ASSERTSTR_NULL(x) 
#   define _IF_ASSERT(x) x
#   define  _ASSERT_ENABLE
#   define  _assert(x) gsys_assert(x)
#   define  _assertp(x) GSYS_ASSERTOBJ(x)
#else
#	undef _ASSERT
#   define _ASSERT(x) ((void)0)
#   define _ASSERTOBJ(x) ((void)0)
#   define _ASSERTREF(x) ((void)0)
#   define _ASSERTPT(x,s) ((void)0)
#   define _ASSERTPTR(x) ((void)0)
#   define _ASSERTSTR(x) ((void)0)
#   define _ASSERTOBJ_NULL(x) ((void)0)
#   define _ASSERTPT_NULL(x,s) ((void)0)
#   define _ASSERTPTR_NULL(x) ((void)0)
#   define _ASSERTSTR_NULL(x) ((void)0)
#   define _CFN
#   define _IF_ASSERT(x)
#   undef  _ASSERT_ENABLE
#   define  _assert(x) ((void)0)
#   define  _assertp(x) ((void)0)
#endif
