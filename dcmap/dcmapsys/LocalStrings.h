#pragma once
#include "../dcmapsyslib.h"
#include <g_hash_map.h>
#include <g_alg.h>
#include <g_text_config_parser.h>
#include <g_gstring.h>

class CDCLocalStrings : public IDCMapLocalStringsEx
{
protected:
	int  iCodePage;
	int  iCharSet;
	bool bTranslit;

	struct sdata
	{
		std::string  str;
		std::wstring wstr;
		sdata(){}
		sdata(std::string  _str,std::wstring _wstr):str(_str),wstr(_wstr){}
	};
	typedef gsys::strimap<std::string,sdata> StringMap;
	typedef gsys::strimap<std::string,StringMap*> DataMap;

	//typedef gsys::strimap<gsys::gstring,gsys::gstring> StringMap;
	//typedef gsys::strimap<gsys::gstring,StringMap*> DataMap;

	DataMap m_Sections;
	StringMap m_Strings;
    StringMap m_Options;

	DataMap::iterator m_Section;

	StringMap* m_pCurrentStrings;

	sdata* GetSectionStringData(char const* name,char const* section);

    void Configure();

    bool bForceCharset;

public:
	bool Load(dcmapFCSTR szFileName);
public:
    CDCLocalStrings();

	bool DCMAPAPI UpdateFrom(dcmapFCSTR szFileName);

	dcmapCSTR DCMAPAPI GetString(dcmapCSTR name);
	dcmapCSTR DCMAPAPI GetIDString(int id);

	bool DCMAPAPI SelectSection(dcmapCSTR name);
	dcmapCSTR DCMAPAPI GetSectionString(dcmapCSTR name,dcmapCSTR section=0);

	dcmapWCSTR DCMAPAPI GetStringW(dcmapCSTR name);
	dcmapWCSTR DCMAPAPI GetIDStringW(int id);
	dcmapWCSTR DCMAPAPI GetSectionStringW(dcmapCSTR name,dcmapCSTR section=0);

    int ForceCharSet(){return bForceCharset?iCharSet:0;}

	int DCMAPAPI CopePage(){return iCodePage;}
	int DCMAPAPI CharSet(){return iCharSet;}
	int DCMAPAPI Translit(){return bTranslit;}
};

extern CDCLocalStrings g_SysStrings;