#pragma once
#include "../dcmapsys.h"
#include <g_hash_map.h>
#include <g_alg.h>

#include <g_text_config_parser.h>

class CDCMapConfig :public IDCMapConfig
{
	typedef gsys::strimap<string,wstring> StringMap;
	typedef gsys::strimap<string,StringMap> DataMap;

	DataMap m_Data;

	sysstring m_strFilename;

	DataMap::iterator m_Section;

	wstring const& ReadStr(char const* szName);
	void WriteStr(char const* szName,std::wstring const& str);

public:
	CDCMapConfig(void);
public:
	~CDCMapConfig(void);

	bool Load(dcmapFCSTR szFile);
	bool Save(dcmapFCSTR szFile=0);
	void Clear();

	void DCMAPAPI RemoveSection(dcmapCSTR szSection);
	void DCMAPAPI RemoveEntry(dcmapCSTR szSection);

	void DCMAPAPI SelectSection(char const* szSection);

	char const* DCMAPAPI ReadString(char const* szSection);
	void DCMAPAPI WriteString(char const* szSection,char const* str);
	dcmapWCSTR DCMAPAPI ReadStringW(char const* szSection);
	void DCMAPAPI WriteStringW(char const* szSection,dcmapWCSTR str);

	bool DCMAPAPI ReadInteger(char const* szName,int* data){return gsys::lex_convert(*data,ReadStr(szName));}
	bool DCMAPAPI ReadUnsigned(char const* szName,dcmapDWORD* data){return gsys::lex_convert(*data,ReadStr(szName));}

	void DCMAPAPI WriteInteger(char const* szName,int data){WriteStr(szName,gsys::lex_cast<std::wstring>(data));}

	bool DCMAPAPI ReadFloat(char const* szName,double* data){return gsys::lex_convert(*data,ReadStr(szName));}
	void DCMAPAPI WriteFloat(char const* szName,double data){WriteStr(szName,gsys::lex_cast<std::wstring>(data));}

	bool DCMAPAPI ReadInt64(dcmapCSTR szName,dcmapINT64* data){return gsys::lex_convert(*data,ReadStr(szName));}
	void DCMAPAPI WriteInt64(dcmapCSTR szName,dcmapINT64 data){WriteStr(szName,gsys::lex_cast<std::wstring>(data));}

	bool DCMAPAPI ReadUnsignedInt64(dcmapCSTR szName,dcmapUINT64* data){return gsys::lex_convert(*data,ReadStr(szName));}
	void DCMAPAPI WriteUnsignedInt64(dcmapCSTR szName,dcmapUINT64 data){WriteStr(szName,gsys::lex_cast<std::wstring>(data));}

};
