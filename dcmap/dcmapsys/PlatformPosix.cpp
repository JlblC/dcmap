#include "stdafx.h"
#include "g_iconv.h"
#include "../dcmapbase.h"

static gsys::iconv<dcmapCHAR,dcmapDCHAR> utf8to16("UTF-8","UTF-16");
static gsys::iconv<dcmapDCHAR,dcmapCHAR> utf16to8("UTF-16","UTF-8");

static gsys::iconv<dcmapDCHAR,dcmapWCHAR> utf16to32("UTF-16","UTF-32");
static gsys::iconv<dcmapWCHAR,dcmapDCHAR> utf32to16("UTF-32","UTF-16");

static gsys::iconv<dcmapCHAR,dcmapWCHAR> utf8to32("UTF-8","UTF-32");
static gsys::iconv<dcmapWCHAR,dcmapCHAR> utf32to8("UTF-32","UTF-8");

bool PlatformInit()
{
	return true;
}

void PlatformTerminate()
{
	
}

string StrW2C(wstring const& ws)
{
	
}

wstring StrC2W(string const& ws)
{
	
}

dstring StrW2D(wstring const& ws)
{
	
}

wstring StrD2W(dstring const& ds)
{

}

string StrD2C(dstring const& ws)
{

}

dstring StrC2D(string const& ws)
{

}

void dcsncpy(dcmapDSTR to,dcmapDCSTR from,size_t n)
{
	std::char_traits<dcmapDCHAR>::copy(to,from,n);

size_t dcslen(dcmapDCSTR str)
{
	return std::char_traits<dcmapDCHAR>::length(to);
}