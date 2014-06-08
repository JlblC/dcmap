#ifndef __INCLUDED_g_iconv_h__
#define __INCLUDED_g_iconv_h__
#pragma once

#include "g_gsys.h"
#include <iconv.h>

namespace gsys
{
	template<typename _From,typename _To>
	class iconv
	{
		iconv_t conv;
	public:
		iconv():conv(0){}
		iconv(char const* from,char const* to):conv(0)
		{
			init(from,to);
		}
		bool valid(){return conv!=0;}
		~iconv()
		{
			if(conv)iconv_close(conv);
		}
		bool init(char const* from,char const* to)
		{
			if(conv)iconv_close(conv);
			conv = iconv_open(to, from);
			if(conv == (iconv_t)(-1))
			{
				conv = 0;
				return false;
			}
			return true;
		}

	};

}

#endif // __INCLUDED_g_iconv_h__

