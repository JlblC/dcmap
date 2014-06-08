// gsys.h
#pragma once

#include "boost/lexical_cast.hpp"

//#include <direct.h>
//#include <io.h>

#include <memory.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <math.h>
#include <vector>


#include "g_assert.h"

#include "g_types.h"

/*
#define gsys_byte (::gtypes::gbyte)

//#define gsys_null 0
namespace gsys
{
    using namespace gtypes;

	// use this cast to 

	template<class Target, class Source>
	Target dyn_cast(Source s)
	{
		return dynamic_cast<Target>(s);	
	}


	dword __inline reinterpret_float2dword(float f){return *reinterpret_cast<dword*>(&f);}


    dword __inline MakeARGB(gsys_byte a,gsys_byte r,gsys_byte g, gsys_byte b)
        {return (dword)(((a&0xff)<<24)|((r&0xff)<<16)|((g&0xff)<<8)|(b&0xff));}

    dword __inline MakeXRGB(gsys_byte r,gsys_byte g, gsys_byte b)
        {return MakeARGB(0xff,r,g,b);}



    // Set current dir to dir from application started
    void SetAppStartupDirAsCurrent();

	// functions for compatiblity
	template<typename T>
	std::string __inline ToStr(T i){return boost::lexical_cast<std::string>(i);}
	std::string __inline IntToStr(int i){return ToStr(i);}
	std::string __inline WordToStr(word i){return ToStr(i);}
	std::string __inline DWordToStr(dword i){return ToStr(i);}

}

*/