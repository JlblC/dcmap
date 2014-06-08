#include "StdAfx.h"
#include "RegExpRecordImport.h"
#include "utils.h"
#include <boost/regex.hpp>
#include <sstream>
using namespace gsys;

#include "Parser.h";


struct CRecordImportService : public IDCMapRecordImport
{
	IDCMapCore* m_pCore;

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapRecordImport);
		return 0;
	}

	bool DCMAPAPI  ImportClipboard(dcmapCSTR strTable, dcmapDBREF const* CurrentRecord)
	{
		dcmapDCSTR dta = m_pCore->GetCoreUI()->ClipboardGetW();
		if(!dta || !dta[0])return false;

		IDCMapDataStorage* pData = m_pCore->GetDataSourceTyped<IDCMapDataStorage>(strTable);
		if(!pData)return false;

		DCMapConfigWrapper cfg(m_pCore->GetConfig());
		cfg.section((string)"Table_"+strTable);
		int num=0;
		cfg["RegExConfNum"] >> num;

		for(int i=0;i<num;i++)
		{
			wstring regexp;
			string format;
			cfg["RegExConfRegExp"+gsys::to_str(i)] >> regexp;
			/*if(imp.Parse(dta.c_str(),regexp.c_str()))
			{
				if(imp.Import(CurrentRecord))
				{
					return true;
				}
			}*/
			CModuleParser prs;
			prs.Init(m_pCore,0);
			prs.Rights(PRSR_READ|PRSR_DB_WRITE);
			wiparsestream is(regexp);
			prs.SetTable(strTable);

			if(CurrentRecord)prs.SelectID(*CurrentRecord);
			prs.SetDefaultValue(dta);

			CVarValue res;

			try
			{
				prs.Parse(true,is,res);
			}
			catch (die_exception& e){continue;}
			catch(std::exception& e)
			{
				m_pCore->GetCoreUI()->ErrorMsg(ToWide(e.what()).c_str());
				return false;				
			}
			return prs.Post();
		}
		m_pCore->GetCoreUI()->ErrorMsg(LSTRW(DataNotRecognized));
		return false;
	}
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pCore = core;
		return true;
	}
	bool DCMAPAPI  ImportSequence(dcmapCSTR strTable,dcmapDCSTR format, dcmapDCSTR data)
	{
		if(!data || !data[0])return false;
		CRegExpRecordImport imp;

		IDCMapDataStorage* pData = m_pCore->GetDataSourceTyped<IDCMapDataStorage>(strTable);
		if(!pData)return false;
		imp.Init(m_pCore,pData);

		return imp.ImportSeq(data,format);
	}
};

DCMAP_REGISTER(CRecordImportService,"PCoreService","RecordImport");


CRegExpRecordImport::CRegExpRecordImport(void)
{
}

CRegExpRecordImport::~CRegExpRecordImport(void)
{
}

bool DCMAPAPI CRegExpRecordImport::Init(IDCMapCore* pCore,IDCMapDataStorage* pData)
{
	m_pCore=pCore;
	m_pData = pData;
	if(!m_pData)return false;

	// Init indexes
	int n = m_pData->NumColumns();

	TableRel.clear();
	TableRel.resize(n,0);

	keyind.clear();
	for(int i=0;i<n;i++)
	{
		if(m_pData->ColumnParams(i) & DCMAP_COLUMN_INDEX)
			keyind.push_back(i);
		if(m_pData->ColumnDataType(i) == DCMDT_Reference)
		{
			TableRel[i] = pCore->GetDataSourceTyped<IDCMapDataStorage>(m_pData->ColumnDataLinkage(i));
		}
	}
	return true;
}

void DCMAPAPI CRegExpRecordImport::SetVariableW(dcmapCSTR strName,dcmapDCSTR strValue)
{
	m_vars[strName] = strValue;
}

void DCMAPAPI CRegExpRecordImport::SetVariable(dcmapCSTR strName,dcmapCSTR strValue)
{
	m_vars[strName] = ToWide(strValue);
}

bool DCMAPAPI CRegExpRecordImport::Import(dcmapDBREF const* CurrentRecord)
{
	if(CurrentRecord)
	{
		if(!m_pData->SelectID(*CurrentRecord))return false;
		m_pData->Update();
	}
	else
	{
		m_pData->EditIndex();
		for(int i=0;i<keyind.size();i++)
		{
			if(ind[keyind[i]]<0)return false;
			m_pData->SetWideString(keyind[i],m_vecInfo[ind[keyind[i]]].second.c_str());
		}
		if(m_pData->SelectIndex()) // update record
		{
			m_pData->Update();
		}
		else // Add new record
		{
			m_pData->Insert();
		}
	}
	for(int i=0;i<m_vecInfo.size();i++)
	{
		int n = m_vecInfo[i].first;
		if(TableRel[n])
		{
			dcmapDBREF ref;
			TableRel[n]->RetrieveByWideName(m_vecInfo[i].second.c_str(),&ref);
			m_pData->SetReference(n,ref);
		}
		else
		{
			m_pData->SetWideString(n,m_vecInfo[i].second.c_str());
		}
	}
	return m_pData->Post();
}

bool CRegExpRecordImport::ParseRegExp(wistream& is,dcmapDCSTR strBegin,dcmapDCSTR strEnd,boost::wcmatch &rvars)
{
	wstring rexp;
	is >> wistream_perl_quoted(rexp);
	if(is.fail())return false;

	// read mode chars
	bool passtrough=false;
	bool seq=false;
	while(!is.fail())
	{
		wchar_t mod = is.peek();
		if(mod == L';')break;
		if(mod == L'p')passtrough = true;
		if(mod == L's')seq = true;
		is.ignore();
	}
	bool r = false;
	try
	{
		r = boost::regex_search(strBegin,strEnd,rvars,boost::wregex(rexp));
	}
	catch (boost::regex_error &e)
	{
		return false;
	}
	if(!r && !passtrough)return false;
	if(seq)
	{
		m_strSeqBegin = rvars.suffix().first;
		m_strSeqEnd = rvars.suffix().second;
	}
	return true;
}

int CRegExpRecordImport::ParseValue(wistream& is,boost::wcmatch &rvars,wstring& result,int& type)
{
	bool exists=false;
	if(is.peek() == L'$') // some variable
	{
		is.ignore();
		bool re = wchar_classify::numeric(is.peek());
		wstring name;
		is >> wistream_numb_keyword(name)>>wsp_nl_scip;
		int n=0;
		if(re && gsys::lex_convert(n,name))
		{
			if(rvars.size()>=n)
			{
				result = rvars[n].str();
				exists = rvars[n].matched;
			}
			else
			{
				exists = false;
				result=L"";
				type=-1;
			}
		}
		else
		{
			MapVars::iterator it = m_vars.find(WideToString(name));
			if(it != m_vars.end())
			{
				result = it->second;
				exists = true;
				type=1;
			}
			else
			{
				result = L"";
				type=0;
			}
		}
	}
	else if(is.peek() == '\'' || is.peek() == '\"')
	{
		wstring val;
		is >> wistream_quoted(val);
		result = val;
		exists = true;
		type=1;
	}
	else 
	{
		wstring val;
		is >> wistream_numb_keyword(val);
		if(is.fail())return false;
		result = val;
		exists = true;
		type=1;
	}
	is >> wsp_nl_scip;
	if(is.peek() == L'{') // simple case block selects first of second expr.
	{
		is.ignore();is>>wsp_nl_scip;
		wstring sr1,sr2;
		int t1,t2;
		int r1=-1;
		int r2=-1;
		r1 = ParseValue(is,rvars,sr1,t1);
		if(r1==0)return 0;
		if(is.peek() == ',')
		{
			is.ignore();is>>wsp_nl_scip;
			r2 = ParseValue(is,rvars,sr2,t2);
		}
		if(is.peek() != '}')return 0;
		is.ignore();is>>wsp_nl_scip;
		if(exists)
		{
			result =  sr1;
			type = t1;
			return r1;
		}
		else
		{
			type = t2;
			result =  sr2;
			return r2;
		}
	}
	if(!exists)return -1;
	return 1;
}

bool CRegExpRecordImport::Parse(wistream& is,dcmapDCSTR strBegin,dcmapDCSTR strEnd)
{
	boost::wcmatch rvars;
	wchar_t ps=0;
	ps = is.peek();
	if(wchar_classify::unicode_marker(ps))is.ignore();

	is >> wsp_nl_scip;
	while(!is.fail() && !is.eof())
	{
		ps = is.peek();
		if(ps == L'/') // Begin of regexp 
		{
			if(!ParseRegExp(is,strBegin,strEnd,rvars))return false;
		}
		else if(tchar_is_alpha(ps))
		{
			wstring key;
			is >> wistream_keyword(key)>>wsp_nl_scip;
			if(is.fail())return false;
			int id = m_pData->ColumnID(WideToString(key).c_str());
			if(id<0)return false;

			bool rewrite=false;
			ps = is.peek();
			if(ps == L':')
			{
				rewrite=true;
				is.ignore();
			}

			ps = is.peek();
			if(ps == L'=') // assigment operation
			{
				is.ignore();is>>wsp_nl_scip;
				wstring value;
				int type=-1;
				int r = ParseValue(is,rvars,value,type);
				if(r)
				{
					if((type >= 0 && !value.empty() && value != L"0") || rewrite)
					{
						m_vecInfo.push_back(PInf(id,value));
						ind[id] = m_vecInfo.size()-1; 
					}
				}
				else return false;
			}
		}
		else return false;
		is>>wsp_nl_scip;
		ps = is.peek();
		if(ps != L';') break;
		is.ignore();
		is >> wsp_nl_scip;
	}
	return true;
}

bool DCMAPAPI CRegExpRecordImport::Parse(dcmapDCSTR strData,dcmapDCSTR strRegExp)
{
	int sz = wcslen(strData);	

	m_strSeqBegin=strData;
	m_strSeqEnd=strData+sz;

	
	ind.clear();
	ind.resize(m_pData->NumColumns(),-1);
	m_vecInfo.clear();
	std::wistringstream is(strRegExp);
	return Parse(is,m_strSeqBegin,m_strSeqEnd);
}

bool DCMAPAPI CRegExpRecordImport::ImportSeq(dcmapDCSTR strData,dcmapDCSTR strRegExp)
{
	int sz = wcslen(strData);	

	m_strSeqBegin=strData;
	m_strSeqEnd=strData+sz;

	int n=0;
	m_pCore->LockDataUpdate();

	while(1)
	{
		ind.clear();
		ind.resize(m_pData->NumColumns(),-1);
		m_vecInfo.clear();
		std::wistringstream is(strRegExp);
		if(!Parse(is,m_strSeqBegin,m_strSeqEnd))break;
		if(!Import(0))break;
		n++;
	}
	m_pCore->UnlockDataUpdate();
	return n;
}