#pragma once
#ifndef G_GSYS_ASSERT_H
#define G_GSYS_ASSERT_H


#ifdef GSYS_ENABLE_ASSERT 
#	ifdef GSYS_ENABLE_CUSTOM_ASSERT
#		define BOOST_ENABLE_ASSERT_HANDLER
#	endif
#else
#	undef GSYS_ENABLE_CUSTOM_ASSERT
#	define BOOST_DISABLE_ASSERTS
#endif

#include "boost/assert.hpp"
#include "boost/current_function.hpp"

namespace gsys
{
	void DebugOutput(const char* str);
	bool ValidatePt(const void* pt, int size);
	bool ValidateCodePt(const void* pt);
	bool ValidateROPt(const void* pt, int size);

	template<class T> bool tValidatePt(T* pt){return ValidatePt(pt,sizeof(T));}
	void Assert(char const* exp,char const * func, char const* file, int line);
}

#ifdef GSYS_DEBUG
#	define GSYS_DEBUG_OUTPUT(x) gsys::DebugOutput(x)
#else
#	define GSYS_DEBUG_OUTPUT(x)
#endif

#ifdef GSYS_ENABLE_ASSERT
#	ifdef GSYS_ENABLE_POINTER_VALIDATION
#		define GSYS_VALIDATE_PT(x,s) gsys::ValidatePt(x,s)
#		define GSYS_VALIDATE_CODE(x) gsys::ValidateCodePt(x)
#		define GSYS_VALIDATE_ROPT(x) gsys::ValidateROPt(x)
#	else
#		define GSYS_VALIDATE_PT(x,s) (x != 0)
#		define GSYS_VALIDATE_CODE(x) (x != 0)
#		define GSYS_VALIDATE_ROPT(x) (x != 0)
#	endif
#	ifdef GSYS_ENABLE_CUSTOM_ASSERT
#		define GSYS_ASSERT_FN(exp,func,file,line) gsys::Assert(exp,func,file,line)
#		define GSYS_ASSERT_AT(_exp) GSYS_ASSERT_FN(#_exp,BOOST_CURRENT_FUNCTION,__FILE__, __LINE__)
#	else
#		define GSYS_ASSERT_AT(_exp) assert(_exp)
#	endif
#	ifdef GSYS_ASSERT_NOTIFICATION_HANDLER
#		define GSYS_ASSERT(_exp) if(!(_exp)){GSYS_ASSERT_NOTIFICATION_HANDLER(#_exp,BOOST_CURRENT_FUNCTION,__FILE__, __LINE__);\
							GSYS_ASSERT_AT(_exp);}
#	else
#		define GSYS_ASSERT(_exp) {if(!(_exp)){GSYS_ASSERT_AT(_exp);};}
#	endif
#   define GSYS_ASSERTOBJ(x) GSYS_ASSERT(GSYS_VALIDATE_PT(reinterpret_cast<const void*>(x),sizeof(*(x))))
#   define GSYS_ASSERTREF(x) GSYS_ASSERT(GSYS_VALIDATE_PT(reinterpret_cast<const void*>(&(x)),sizeof(x)))
#   define GSYS_ASSERTPT(x,s) GSYS_ASSERT(GSYS_VALIDATE_PT(x,s))
#   define GSYS_ASSERTPTR(x) GSYS_ASSERT(GSYS_VALIDATE_PT(x,0))
#   define GSYS_ASSERTSTR(x) GSYS_ASSERT(GSYS_VALIDATE_PT(x,0))
#   define GSYS_ASSERTARRAY(x,n) GSYS_ASSERT(GSYS_VALIDATE_PT(reinterpret_cast<const void*>(x),sizeof(*(x))*n))
#   define GSYS_ASSERT_CODE(x) GSYS_ASSERT(GSYS_VALIDATE_CODE(x))
#   define GSYS_ASSERT_CALLBACK(x) GSYS_ASSERT_CODE(x)
#   define GSYS_CFN GSYS_ASSERTOBJ(this)
#   define GSYS_ASSERTOBJ_NULL(x) if(x!=0){GSYS_ASSERTOBJ(x);}
#   define GSYS_ASSERTPT_NULL(x,s) if(x!=0){GSYS_ASSERTPT(x,s);}
#   define GSYS_ASSERTPTR_NULL(x) if(x!=0){GSYS_ASSERTPTR(x);}
#   define GSYS_ASSERTSTR_NULL(x) if(x!=0){GSYS_ASSERTSTR(x);}
#   define GSYS_IF_ASSERT(x) x
#   define GSYS_IS_ASSERT
#	define gsys_assert(x) GSYS_ASSERT(x)
#else
#   define GSYS_ASSERT(x) ((void)0)
#   define GSYS_ASSERTOBJ(x) ((void)0)
#   define GSYS_ASSERTREF(x) ((void)0)
#   define GSYS_ASSERTPT(x,s) ((void)0)
#   define GSYS_ASSERTPTR(x) ((void)0)
#   define GSYS_ASSERTARRAY(x,n) ((void)0)
#   define GSYS_ASSERTSTR(x) ((void)0)
#   define GSYS_ASSERT_CODE(x) ((void)0)
#   define GSYS_ASSERT_CALLBACK(x) ((void)0)
#   define GSYS_ASSERTOBJ_NULL(x) ((void)0)
#   define GSYS_ASSERTPT_NULL(x,s) ((void)0)
#   define GSYS_ASSERTPTR_NULL(x) ((void)0)
#   define GSYS_ASSERTSTR_NULL(x) ((void)0)
#   define GSYS_CFN
#   define GSYS_IF_ASSERT(x)
#   undef  GSYS_IS_ASSERT
#	define gsys_assert(x) ((void)0)
#endif




#endif
