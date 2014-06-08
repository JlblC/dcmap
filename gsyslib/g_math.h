#pragma once
#ifndef G_MATH_H
#define G_MATH_H

#include <math.h>
#include "g_gsys.h"

#ifdef GSYS_FASTMATH_ENABLE
#	ifndef GSYS_FASTMATH_TRIG_TABLE_DENSITY
#	define GSYS_FASTMATH_TRIG_TABLE_DENSITY 6 // 64 per degrees
#	endif
#endif

#ifdef GSYS_DETAIL_NO_STD_FLOAT_MATH
    float inline cosf(float f){return cos(f);}
    float inline sinf(float f){return sin(f);}
    float inline sqrtf(float f){return sqrt(f);}
    float inline tanf(float f){return tan(f);}
    float inline acosf(float f){return acos(f);}
    float inline asinf(float f){return asin(f);}
    float inline atanf(float f){return atan(f);}
#endif

namespace gsys
{

    const float _pi = 3.1415926535897932384626433832795f;
    const double _d_pi = 3.1415926535897932384626433832795;

	template<typename T>T g_sqr(T v){return v*v;}

	template<class T> T deg2rad(T x){return x*((T)_d_pi)/((T)180.0);}
	template<class T> T rad2deg(T x){return x/((T)_d_pi)*((T)180.0);}

	template<class T> float deg2radf(T x){return deg2rad((float)x);}
	template<class T> double deg2radd(T x){return deg2rad((double)x);}

	namespace detail
	{
        #ifdef GSYS_DETAIL_INLINE_ASM_ENABLE
		inline void gFtoDWORD(dword &i, float f )
		{
			__asm	fld f;
			__asm	mov edx, i
			__asm	fistp [edx];
		}
        #else

        #endif

#ifdef GSYS_ENABLE_QUICK_SQRT
#	ifdef GSYS_i586
#       ifdef GSYS_DETAIL_INLINE_ASM_ENABLE
		inline float __fastcall grsqrtq(float x)
		{
			const float _0_47 = 0.47f;
			const float _1_47 = 1.47f;
			dword y;
			float r;
			_asm
			{
				mov     eax, 07F000000h+03F800000h // (ONE_AS_INTEGER<<1) + ONE_AS_INTEGER
				sub     eax, x
				sar     eax, 1

				mov     y, eax                      // y
				fld     _0_47                       // 0.47
				fmul    DWORD PTR x                 // x*0.47

				fld     DWORD PTR y
				fld     st(0)                       // y y x*0.47
				fmul    st(0), st(1)                // y*y y x*0.47

				fld     _1_47                       // 1.47 y*y y x*0.47
				fxch    st(3)                       // x*0.47 y*y y 1.47
				fmulp   st(1), st(0)                // x*0.47*y*y y 1.47
				fsubp   st(2), st(0)                // y 1.47-x*0.47*y*y
				fmulp   st(1), st(0)                // result
				fstp    y
				and     y, 07FFFFFFFh

			}
			r = *reinterpret_cast<float*>(&y);
			// optional
			return r;
		}
#               else
#               endif
#	else //GSYS_i586
#		error arhitecture not supported
#	endif	//GSYS_i586

		inline float __fastcall grsqrt(float x)
		{
			float r = grsqrtq(x);
			return (3.0f - x * (r * r)) * r * 0.5f; // adjust for higher accuracy
		}

		/*
		sqrt(x) = x / sqrt(x)
		*/
		inline float __fastcall gsqrt(float x)
		{
			return x * grsqrt(x);
		}

		inline float __fastcall gsqrtq(float x)
		{
			return x * grsqrtq(x);
		}
#endif // GSYS_ENABLE_QUICK_SQRT

		// used for nomalise operations
		inline float InvSqrt(const float x)
		{
			dword   tmp = ((0x3F800000u << 1) + (0x3F800000u) - *(dword*)&x) >> 1;
			float y = *(float*)&tmp;
			return y * (1.47f - 0.47f * x * y * y);
		}

#ifdef GSYS_FASTMATH_ENABLE
		extern float g_cos_table[];
		extern float g_sin_table[];
		extern float g_tan_table[];
#endif
	}

	inline float g_sqrt(float f)
	{
#ifdef GSYS_ENABLE_QUICK_SQRT
		return detail::grsqrtq(f);
#else
		return sqrtf(f);
#endif
	}

	inline double g_sqrt(double f){return sqrt(f);}
	inline double g_cos(double f){return cos(f);}
	inline double g_sin(double f){return sin(f);}

#ifdef GSYS_FASTMATH_ENABLE
	inline float g_cos(float f)
	{
		if(f < 0 || f > 2*_pi)return cosf(f);
		return detail::g_cos_table[int((f/(_pi*2))*(360<<GSYS_FASTMATH_TRIG_TABLE_DENSITY))];
	}
	inline float g_sin(float f)
	{
		if(f < 0 || f > 2*_pi)return sinf(f);
		return detail::g_sin_table[int((f/(_pi*2))*(360<<GSYS_FASTMATH_TRIG_TABLE_DENSITY))];
	}
	inline float g_tan(float f)
	{
		if(f < 0 || f > 2*_pi)return tanf(f);
		return detail::g_tan_table[int((f/(_pi*2))*(360<<GSYS_FASTMATH_TRIG_TABLE_DENSITY))];
	}
#else
	inline float g_cos(float f){return cosf(f);}
	inline float g_sin(float f){return sinf(f);}
	inline float g_tan(float f){return tanf(f);}
#endif // GSYS_FASTMATH_ENABLE

}

namespace gsys
{
	inline float inv_sqrt(float x)
	{
		return gsys::detail::InvSqrt(x);
	}

}


#endif
