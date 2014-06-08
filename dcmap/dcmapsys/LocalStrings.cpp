#include "stdafx.h"
#include "LocalStrings.h"
#include "g_alg.h"
#include <g_std_file.h>
#include <g_file_istream.h>
#include "utils.h"

CDCLocalStrings g_SysStrings;

bool DCMAPAPI CDCLocalStrings::UpdateFrom(dcmapFCSTR szFileName)
{
	return Load(szFileName);
}

CDCLocalStrings::CDCLocalStrings()
{
    iCodePage = CP_UTF8;
    iCharSet = 0;	
    bTranslit = false;	
    bForceCharset=false;
}

bool CDCLocalStrings::Load(dcmapFCSTR szFileName)
{
	gsys::text_config_parser prs;

	boost::scoped_ptr<gsys::gfile> pfile(OpenFileReadZ(szFileName));
	if(!pfile || !pfile->valid())return false;
	if(!prs.parce(pfile.get()))return false;


	int FileCP = prs.is_utf8()?CP_UTF8:1251;

    iCodePage = CP_UTF8;
    iCharSet = 0;	
    bTranslit = false;	
    bForceCharset = false;	

    bool bConfigure=false;

	m_pCurrentStrings = &m_Strings;
	std::string item;
	std::string data;
	bool section;
	while(prs.record(item,section))
	{
		if(item.empty())continue;
		if(section)
		{
            if(bConfigure)
            {
                Configure();
                bConfigure = false;
            }
			if(item == "Strings")
			{
				m_pCurrentStrings = &m_Strings;
			}
            else if(item == "Locale")
            {
                m_pCurrentStrings = &m_Options;
                bConfigure = true;
            }
			else
			{
				m_Section = m_Sections.find(item);
				if(m_Section == m_Sections.end())
				{
					m_Section = m_Sections.insert(DataMap::value_type(item,new StringMap())).first;
				}		
				m_pCurrentStrings = m_Section->second;
			}
		}
		else if(m_pCurrentStrings)
		{
			prs.block(data);
			// UF8 to Unicode
			sdata dta;
			dta.wstr = ToWide(data,FileCP);
			dta.str = WideToString(dta.wstr);
						
			StringMap::iterator it = m_pCurrentStrings->find(item);
			if(it == m_pCurrentStrings->end())
			{
				(*m_pCurrentStrings)[item] = dta;
			}
			else
			{
				it->second = dta;
			}
		}
	} 
    if(bConfigure)Configure();
    return true;
}

void CDCLocalStrings::Configure()
{
    if(!gsys::lex_convert(iCodePage,m_Options["CodePage"].wstr))
        iCodePage = CP_UTF8;
    if(!gsys::lex_convert(iCharSet,m_Options["CharSet"].wstr))
        iCharSet = 0;	
    if(!gsys::lex_convert(bTranslit,m_Options["Translit"].wstr))  
        bTranslit = false;	
    if(!gsys::lex_convert(bForceCharset,m_Options["ForceCharset"].wstr))  
        bForceCharset = false;	
}

char const* DCMAPAPI CDCLocalStrings::GetString(char const* name)
{
	StringMap::iterator it = m_Strings.find(name);
	if(it == m_Strings.end()) return "";
	return it->second.str.c_str(); 
}

char const* DCMAPAPI CDCLocalStrings::GetIDString(int id)
{
	return "";
}

bool DCMAPAPI CDCLocalStrings::SelectSection(char const* name)
{
	m_pCurrentStrings=0;
	m_Section = m_Sections.find(name);
	if(m_Section == m_Sections.end())return false;
	m_pCurrentStrings = m_Section->second;
	return true;
}

CDCLocalStrings::sdata* CDCLocalStrings::GetSectionStringData(char const* name,char const* section)
{
	StringMap* m_pStrings = m_pCurrentStrings;

	if(section)
	{
		m_pStrings=0;
		DataMap::iterator it = m_Sections.find(section);
		if(it == m_Sections.end())return 0;
		m_pStrings = it->second;
	}

	if(!m_pStrings)return 0;
	StringMap::iterator it = m_pStrings->find(name);
	if(it == m_pStrings->end()) return 0;
	return &it->second;
}

char const* DCMAPAPI CDCLocalStrings::GetSectionString(char const* name,char const* section)
{
	sdata* pdta = GetSectionStringData(name,section);
	if(!pdta) return "";
	return pdta->str.c_str();
}

dcmapWCSTR DCMAPAPI CDCLocalStrings::GetStringW(char const* name)
{
	StringMap::iterator it = m_Strings.find(name);
	if(it == m_Strings.end()) return L"";
	return it->second.wstr.c_str(); 
}
dcmapWCSTR DCMAPAPI CDCLocalStrings::GetIDStringW(int id)
{
	return L"";
}
dcmapWCSTR DCMAPAPI CDCLocalStrings::GetSectionStringW(char const* name,char const* section)
{
	sdata* pdta = GetSectionStringData(name,section);
	if(!pdta) return L"";
	return pdta->wstr.c_str();
}
