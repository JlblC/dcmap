#include "common.h"

#include "g_platform.h"

namespace gsys{ namespace detail{

	wchar_t const* StrToWide(char const* src, int Codepage )
	{
		if(!Codepage)Codepage = CP_UTF8;

		static gsys::raw_data_buffer<wchar_t,512,512> buffer;
		if(!buffer.size())buffer.reset(512);

		if(!MultiByteToWideChar(Codepage,0,src,-1,buffer.get(),buffer.size()))
		{
			int size = MultiByteToWideChar(Codepage,0,src,-1,0,0)+1;
			buffer.reset(size);
			MultiByteToWideChar(Codepage,0,src,-1,buffer.get(),buffer.size());
		}
		return buffer.get();
	}

	char const* WideToStr(wchar_t const* src, int Codepage )
	{
		if(!Codepage)Codepage = CP_UTF8;

		static gsys::raw_data_buffer<char,512,512> buffer;
		if(!buffer.size())buffer.reset(512);

		if(!WideCharToMultiByte(Codepage,0,src,-1,buffer.get(),buffer.size(),0,0))
		{
			int size = WideCharToMultiByte(Codepage,0,src,-1,0,0,0,0)+1;
			buffer.reset(size);
			WideCharToMultiByte(Codepage,0,src,-1,buffer.get(),buffer.size(),0,0);
		}
		return buffer.get();
	}
}}