
#pragma once
#include "g_gsys.h"
#include "g_file.h"
namespace gsys 
{
	class gfdir
	{
	public:
		typedef gfile_char_t char_type;
		typedef std::basic_string<char_type> string_type;
		gfdir(){}
		virtual ~gfdir(){}
	public:
		virtual gfile* get_file(char_type const* name,gfile::FileMode mode = gfile::ModeRead)=0;
		virtual bool exists(char_type const* name)=0;
		virtual bool valid()=0;
	};
}