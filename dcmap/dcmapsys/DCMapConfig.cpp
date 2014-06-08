#include "stdafx.h"
#include "DCMapConfig.h"
#include "g_alg.h"
#include <g_std_file.h>
#include <g_file_istream.h>
#include "utils.h"

CDCMapConfig::CDCMapConfig(void)
{
	m_Section = m_Data.end();
}

CDCMapConfig::~CDCMapConfig(void)
{
}

bool CDCMapConfig::Load(dcmapFCSTR szFileName)
{
	m_strFilename = szFileName;

	gsys::text_config_parser prs;

	boost::scoped_ptr<gsys::gfile> pfile(OpenFileReadZ(szFileName));
	if(!pfile || !pfile->valid())return false;
	if(!prs.parce(pfile.get()))return false;

	int FileCP = prs.is_utf8()?CP_UTF8:1251;

	std::string item;
	std::string data;
	bool section;
	while(prs.record(item,section))
	{
		if(section) SelectSection(item.c_str());
		else
		{
			prs.block(data);
			WriteStr(item.c_str(),ToWide(data,FileCP));
		}
	} 
	return true;
}

bool CDCMapConfig::Save(dcmapFCSTR szFileName)
{
	gsys::std_file file((szFileName),gsys::gfile::ModeWrite);
	if(!file.valid())return false;
	gsys::ogfilestream os(&file);

	os << "\xEF\xBB\xBF";

	_foreach(DataMap,m_Data,it)
	{
		//if(it->second.empty())continue;

		os << "["<<it->first<<"]\r\n";
		_foreach(StringMap,it->second,jt)
		{
			os << "\t" << jt->first << " = "; 

			string out = WideToString(jt->second,CP_UTF8);

			int p = out.find_first_of("\\\t\"\r\n ");
			if(p >= 0)
			{
				gsys::quote(out);
				os << out;
			}
			else os << out;
			os << "\r\n";
		}
	}
	return true;
}

void CDCMapConfig::Clear()
{
	m_Data.clear();
}

void DCMAPAPI CDCMapConfig::SelectSection(char const* szSection)
{
	m_Section = m_Data.find(szSection);
	if(m_Section == m_Data.end())
	{
		m_Section = m_Data.insert(DataMap::value_type(szSection,StringMap())).first;
	}
}

std::wstring const& CDCMapConfig::ReadStr(char const* szName)
{
	static std::wstring estr;

	if(m_Section == m_Data.end())return estr;
	StringMap::iterator it = m_Section->second.find(szName);
	if(it == m_Section->second.end()) return estr;
	return it->second; 
}

void CDCMapConfig::WriteStr(char const* szName,std::wstring const& str)
{
	if(m_Section == m_Data.end())return;
	StringMap::iterator it = m_Section->second.find(szName);
	if(it == m_Section->second.end())
	{
		m_Section->second[szName] = str;
	}
	else
	{
		it->second = str;
	}
}

dcmapWCSTR DCMAPAPI CDCMapConfig::ReadStringW(char const* szName)
{
    if(!szName)return dcmapWT("");
	//static std::string estr;
	if(m_Section == m_Data.end())return 0;
	StringMap::iterator it = m_Section->second.find(szName);
	if(it == m_Section->second.end()) return 0;
	return it->second.c_str(); 
}

void DCMAPAPI CDCMapConfig::WriteStringW(char const* szName,dcmapWCSTR str)
{
    if(str)WriteStr(szName,str);
}

dcmapCSTR DCMAPAPI CDCMapConfig::ReadString(char const* szName)
{
    if(!szName)return "";
	static std::string estr;
	if(m_Section == m_Data.end())return 0;
	StringMap::iterator it = m_Section->second.find(szName);
	if(it == m_Section->second.end()) return 0;
	estr = WideToString(it->second);
	return estr.c_str(); 
}
void DCMAPAPI CDCMapConfig::WriteString(char const* szName,dcmapCSTR str)
{
	WriteStr(szName,ToWide(str));
}

void DCMAPAPI CDCMapConfig::RemoveSection( dcmapCSTR szSection )
{
	DataMap::iterator Section = m_Data.find(szSection);
	if(Section == m_Data.end())return;

	if(m_Section == Section)
	{
		Section = m_Data.erase(Section);

		m_Section = m_Data.end();
	}
	else
	{
		Section = m_Data.erase(Section);
	}
}

void DCMAPAPI CDCMapConfig::RemoveEntry( dcmapCSTR szSection )
{

}


