#pragma once
#ifndef G_ALG_H
#define G_ALG_H

#include "gsys.h"
#include "boost/lexical_cast.hpp"
#include <string>
#include <vector>
#include "g_utility.h"
#include "g_gstring.h"
#include "g_text_alg.h"

#ifndef CP_UTF8
#define CP_UTF8 65001
#endif

namespace gsys
{
	namespace detail
	{
		union INTORFLOAT
		{
			float f;
			int i;
		};
		inline int& cast_ftoi_ref(float& f){return reinterpret_ref_cast<int>(f);}

	}


    // returns absolute value
    template<class T> T abs(T a){return a>=0?a:-a;}
	template<> inline gsys_uint abs(gsys_uint a){return a;}
	template<> inline gsys_ulong abs(gsys_ulong a){return a;}
	template<> inline gsys_word abs(gsys_word a){return a;}
	template<> inline gsys_byte abs(gsys_byte a){return a;}
	template<> inline gsys_float abs(gsys_float a)
	{
		detail::cast_ftoi_ref(a) &= 0x7fffffff;
		return a;
	}

#undef max
	template<class _T1,class _T2> _T1 max(_T1 a,_T2 b){return a>b?a:b;}
#undef min
    template<class _T1,class _T2> _T1 min(_T1 a,_T2 b){return a<b?a:b;}

    template<class T> T mod(T x){return x<0?-x:x;}

    // returns bounded value
    template<typename T,typename _T1,typename _T2>
	T bound(T a, _T1 min, _T2 max)
    { 
        if(a>max) a=(T)max;
        if(a<min) a=(T)min;
        return a;
    }

	// tests if value at range  
    template<typename T,typename _T1,typename _T2>
    bool at_range(T a, _T1 min, _T2 max)
    { 
        if(a>max) return false;
        if(a<min) return false;
        return true;
    }
	// tests if value in range between 
    template<typename T,typename _T1,typename _T2>
    bool in_range(T a, _T1 min, _T2 max)
    { 
        if(a>=max) return false;
        if(a<=min) return false;
        return true;
    }

	gsys_float inline frand(){return gsys_float(rand())/RAND_MAX;}

	gsys_float inline rfrand(gsys_float from,gsys_float to)
	{
		return (gsys_float(rand())/RAND_MAX)*(to-from)+from;
	}

	gsys_int inline irand(gsys_int num)
	{
		return gsys_int(rand())*num/RAND_MAX;
	}

	gsys_int inline rirand(gsys_int from,gsys_int to)
	{
		return (gsys_int(rand())*(to-from)/RAND_MAX)+from;
	}

	// returns sign of var
	template<class T>
	inline T csgn(T x){return x==0?0:(x>=(T)(0)?(T)(1):(T)(-1));}
	template<class T>
	inline T sign(T x){return x>=0?(T)(1):(T)(-1);}

	// togle - toggle mutiple type of values beetwin true and false
	template<typename T>void togle(T & v){v=v?(T)(0):(T)(1);}

	template<> __inline void togle<bool>(bool & v){v=v?false:true;}
	template<> __inline void togle<std::string>(std::string & v){v = (v=="true")?("false"):("true");}

	// exchange to objects given by reference
	template<typename T>
	void exchange(T& v1,T& v2)
	{
		T t(v1);
		v1=v2;
		v2=t;
	}

	inline float lerp(float c1,float c2,gsys_real amount)
	{
		return c1+(c2-c1)*amount;
	}

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

	template <typename T,typename I>
	void IndexVertors(std::vector<T> const& vt,std::vector<T> & _v,std::vector<I> & _i)
	{
		_v.clear();
		_i.clear();
#if 0//#ifdef_DEBUG   // too slow in debug mode
		_v = vt;
		return;
#endif
		for(uint i=0;i<vt.size();i++)
		{
			bool add = true;
			// find vector
			for(I j=0;j<_v.size();j++)
			{
				if(_v[j] == vt[i])
				{
					add=false;
					_i.push_back(j);
					break;
				}
			}
			if(add)
			{
				_i.push_back((I)_v.size());
				_v.push_back(vt[i]);
			}
		}
#if 0
		// check reason for indexing
		if(_i.size()*sizeof(I)+_v.size()*sizeof(T) >= vt.size()*sizeof(T))
		{
			_v.clear();
			_i.clear();
			_v = vt;
		}
#endif
	}

	template <typename T,typename I>
	void UnIndexVertors(std::vector<T> const& _v,std::vector<I> const& _i,std::vector<T> & vt)
	{
		if(!_v.size())return;
		if(!_i.size())
		{
			vt = _v;
		}
		else
		{
			for(size_t i=0;i<_i.size();i++)
			{
				vt.push_back(_v[_i[i]]);
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
	// math functions

  //  const float float_cmp_epsilon = 1E-8f;
  
	const float float_cmp_epsilon = 1E-6f;

    bool __inline fequ(float v1,float v2,float epsilon = float_cmp_epsilon)
    {
        if(((v1 - epsilon) <= v2) && (v2 <= (v1 + epsilon)))
			return true;
		else return false;
    }

	template<typename T> bool equ(T const v1,T const v2){return v1 == v2;}

	template<> __inline bool equ<float>(float const v1,float const v2){return fequ(v1,v2);}


	//typedef unsigned __int16 index;

	template<typename T>
	void objcpy(T* dst, T const* src, gsys_size_t num)
	{
		for(int i=0;i<num;i++)
		{
			*dst = *src;
			dst++;
			src++;
		}
	}

	int inline _ftoi(float f)
	{
		detail::INTORFLOAT bias;
		bias.i = (23 + 127) << 23;
		detail::INTORFLOAT& r = reinterpret_ref_cast<detail::INTORFLOAT>(f);
		r.f += bias.f;
		r.i-=bias.i;
		return r.i;
	}

    // functions
#ifdef GSYS_DETAIL_INLINE_ASM_ENABLE
    int inline ftoi(float f)
    {
        int i;
        __asm	fld f;
        __asm	fistp [i];
        return i;
    }
    void inline ftoi(int *i, float f)
    {
        __asm	fld f;
        __asm	mov edx, i
        __asm	fistp [edx];
    }
#else
    int inline ftoi(float f)
    {
		return gsys::_ftoi(f);
	}
    void inline ftoi(int *i, float f)
    {
		*i = gsys::_ftoi(f);
    }
#endif
}

#endif