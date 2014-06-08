#pragma once
#include <boost/format.hpp>
#include "../dcmapsyslib.h"
#include "g_file_util.h"


template<class T> dcmap::string ToStr8(T const& d)
{
	return gsys::to_str(d);
}

#define ToStr ToStr8

string WideToString(wstring wstr,int Codepage=-1);
wstring ToWide(string str,int Codepage=-1);

#if DCMAP_WCHAR_T_SIZE == 2

template<class T> dcmap::wstring ToStr16(T const& d)
{
	return gsys::to_wstr(d);
}
template<class T> dcmap::wstring ToStr32(T const& d)
{
	return gsys::to_wstr(d);
}

#define ToWStr ToStr16
#define ToDStr ToStr32

#define dcsncpy wcsncpy
#define dcslen  wcslen

// string type conversions

#define StrD2C(x) WideToString(x) 
#define StrC2D(x) ToWide(x) 

#define StrW2C(x) WideToString(x) 
#define StrC2W(x) ToWide(x) 

#define StrD2W(x) (x) 
#define StrW2D(x) (x) 

#else

dstring StrW2D(wstring const& ws);
wstring StrD2W(dstring const& ds);
string StrD2C(dstring const& ws);
dstring StrC2D(string const& ws);
string StrW2C(wstring const& ws);
wstring StrC2W(string const& ws);

void dcsncpy(dcmapDSTR to,dcmapDCSTR from,size_t n);
size_t dcslen(dcmapDCSTR to);

template<class T> dcmap::dstring ToStr16(T const& d)
{
	return StrW2D(gsys::to_wstr(d));
}
template<class T> dcmap::wstring ToStr32(T const& d)
{
	return gsys::to_wstr(d);
}

#define ToWStr ToStr32
#define ToDStr ToStr16

#endif

#ifdef DCMAP_UNICODE_SYS

#define ToFStr ToWStr
#define ToUStr ToWStr

#else
#define ToFStr ToStr
#define ToUStr ToStr
#endif


//std::string WideToString(dcmapDCSTR wstr,int CodePage=-1);
//std::wstring ToWide(dcmapCSTR str,int CodePage=-1);

sysstring ToSys(dcmapCSTR str,int CodePage=-1);
dstring ToDStr(std::string str,int CodePage=-1);

sysstring GetAppStartupDir();



bool StrToCoords(dcmapCSTR str,dcmapPOINT& pt);


typedef std::vector<sysstring> VecFiles;
bool ListDirectory(dcmapFCSTR filename, VecFiles &vec);
void DestroyDir(dcmapFCSTR pDir);
bool dcmapDeleteFile(dcmapFCSTR pDir);
bool dcmapRename(dcmapFCSTR pFrom,dcmapFCSTR pTo);
bool dcmapMakeDir(dcmapFCSTR pDir);
bool dcmapFileExists(dcmapFCSTR pDir);

bool dcmapHardLinkFile(dcmapDCSTR from,dcmapDCSTR to,bool FailIfExists);
bool dcmapCopyFile(dcmapFCSTR from,dcmapFCSTR to,bool FailIfExists);

boost::format inline format(const std::string & f_string) {
	using namespace boost::io;
	using namespace boost;
	boost::format fmter(f_string);
	fmter.exceptions( all_error_bits ^ ( too_many_args_bit | too_few_args_bit )  );
	return fmter;
}

boost::wformat inline wformat(const std::wstring & f_string) {
	using namespace boost::io;
	using namespace boost;
	boost::wformat fmter(f_string);
	fmter.exceptions( all_error_bits ^ ( too_many_args_bit | too_few_args_bit )  );
	return fmter;
}

dcmapWCSTR inline c_str(boost::wformat& f)
{
	static wstring wstr;
	wstr = boost::str(f);
	return wstr.c_str();
}

dcmapCSTR inline c_str(boost::format& f)
{
	static string wstr;
	wstr = boost::str(f);
	return wstr.c_str();
}

namespace gsys{class gfile;}
using gsys::gfile;

gfile* OpenFileReadZ(dcmapFCSTR Filename);
gfile* OpenFileCreateZ(dcmapFCSTR Filename,bool z);
