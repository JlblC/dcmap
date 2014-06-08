#include "common.h"
#include "g_platform.h"
#include "../g_text.h"

void gsys::DebugOutput(const char* str)
{
   OutputDebugString(str);
}
namespace boost
{
	void assertion_failed(char const * expr, char const * function, char const * file, long line)
	{
		gsys::Assert(expr,function,file,line);
	}
}

bool gsys::ValidatePt(const void* pt, int size)
{
	if(!pt)return false;

	//if(IsBadReadPtr(pt,size))return false;
	//if(IsBadWritePtr((LPVOID)pt,size))return false;

	return true;
}

bool gsys::ValidateCodePt(const void* pt)
{
	if(!pt)return false;

	if(IsBadCodePtr((FARPROC)pt))return false;

	return true;
}

bool gsys::ValidateROPt(const void* pt, int size)
{
	if(!pt)return false;

	//if(IsBadReadPtr(pt,size))return false;

	return true;
}

void gsys::Assert(char const* exp,char const * func, char const* file, int line)
{ 
	string st = string(file)+"("+to_str(line)+")"+"  Assertion failed in "+func+" at "+ exp+"\n";
#ifdef _DEBUG
	DebugOutput(st.c_str());
#endif
    DebugBreak();
}






