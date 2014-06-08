p// dcmapsys.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../dcmapsyslib.h"
#include "dcmapcore.h"
#include "utils.h"

#include "parceutil.h"

using namespace std;
using namespace gsys;

class CCSVTableExport : public IDCMapExportProcessor
{
	string m_strTable;
	IDCMapDataStorage* m_pData;
	IDCMapCore* m_pCore;
	IDCMapDataSource* m_pSource;

	typedef std::vector<std::wstring> VecRecords;
	typedef std::vector<wchar_t> VecDelims;
	typedef vector<int> VecIndexes;

	wstring m_strParams;

	typedef vector<IDCMapDataStorage*> VecRelations;
	VecRelations TableRel;

	IDCMapExpressionTableFilterProcessor* m_pFilter;

	bool DCMAPAPI SetDataSource(IDCMapDataSource* pSource)
	{
		if(pSource)m_pSource = pSource;
		else m_pSource = m_pData;
		return true;
	}

	bool DCMAPAPI SetFilterExpr(dcmapDCSTR szExpr)
	{
		if(m_pFilter)m_pCore->DestroyModule(m_pFilter);
		m_pFilter=0;
		if(!szExpr || !szExpr[0])return true;

		m_pFilter = m_pCore->CreateModuleTyped<IDCMapExpressionTableFilterProcessor>("PModules","ExprTableFilterProc");
		if(!m_pFilter)return false;
		m_pFilter->Setup(m_strTable.c_str(),szExpr);
		return true;
	}

	bool DCMAPAPI SetTextParam(dcmapDCSTR szParam)
	{
		m_strParams.clear();
		if(szParam)m_strParams = szParam;
		if(m_strParams.empty())m_strParams = L"all";
		if(wcsicmp(m_strParams.c_str(),L"all")!=0)return true;

		m_strParams=L"";
		if(!m_pData)return false;

		int n = m_pData->NumColumns();
		bool pre = false;
		for(int i=1;i<n;i++)
		{
			char const* nm = m_pData->ColumnName(i);
			int param = m_pData->ColumnParams(i);
			if(nm && nm[0] && !(param&DCMAP_COLUMN_HIDDEN) )
			{
				if(pre)m_strParams+=L";";
				m_strParams += ToWide(nm);
				pre = true;
			}
		}
		return true;
	}

	bool DCMAPAPI SetDataSourceName(const char* szName)
	{
		m_strTable = szName;
		m_pData = m_pCore->GetDataSourceTyped<IDCMapDataStorage>(m_strTable.c_str());
		if(!m_pData)return false;
		if(!m_pSource)m_pSource=m_pData;

		// Init indexes
		int n = m_pData->NumColumns();  

		TableRel.clear();
		TableRel.resize(n,0);

		for(int i=0;i<n;i++)
		{
			if(m_pData->ColumnDataType(i) == DCMDT_Reference)
			{
				TableRel[i] =
					m_pCore->GetDataSourceTyped<IDCMapDataStorage>(m_pData->ColumnDataLinkage(i));
			}
		}	
		return true;
	}

	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pCore=core;
		m_pSource=0;
		return true;
	}

	bool DCMAPAPI Export(IDCMapProcessRecordReseiver* pRsvr,dcmapDWORD options)
	{
		wistringstream tis(m_strParams.c_str());
		VecRecords rec;
		VecDelims delims;
		if(!ReadCVSLineQueryDiv(tis,rec,delims))
		{
			return false;
		}
		VecIndexes ind;
		int n = rec.size();
		ind.resize(n,-1);
		for(int i=0;i<n;i++)
		{
			string cn = WideToString(rec[i]);
			if(cn.data()) strlwr((char*)cn.data()); 
			ind[i] = m_pData->ColumnID(cn.c_str());
		}
		pRsvr->AddRecord(m_strParams.c_str());

		int cnt = 0;
		vector<dcmapDBREF> refs;
		refs.reserve(m_pSource->NumRecords());
		if(m_pSource->SelectAll())
		do{ 
			refs.push_back(*m_pSource->GetID());
			cnt++;
		} while(m_pSource->Next());

		pRsvr->SetupProgress(cnt);

		for(int j=0;j<cnt;j++)
		{
			pRsvr->UpdateProgress(j);
			
			if(m_pFilter)
			{
				if(!m_pFilter->Process(&refs[j]))continue;
			}

			if(!m_pData->SelectID(refs[j]))continue;
			wstring output;
			for(int i=0;i<n;i++)
			{
				wstring str;
				if(ind[i] < 0)
				{
					str = rec[i];
				}
				else if(TableRel[ind[i]])
				{
					IDCMapDataStorage* pStor = TableRel[ind[i]];
					dcmapDBREF const* ref = m_pData->GetReference(ind[i]);
					if(ref && pStor->SelectID(*ref))
					{
						str = pStor->GetWideName();
					}
					if(pStor == m_pData)
					{
						if(!m_pData->SelectID(refs[j]))continue;
					}
				}
				else
				{
					str = m_pData->GetWideString(ind[i]);
				}
				if(delims.size()>i)
				{
					output += EncodeCSVItem(str,delims[i]);
					output+=delims[i];
				}
				else output += EncodeCSVItem(str);
			}
			pRsvr->AddRecord(output.c_str());
		} 
		pRsvr->UpdateProgress(cnt);

		return true;
	}
	dcmapDWORD DCMAPAPI GetExportParams(){return DCMAP_EXPORT_UNIVERSAL;}
	dcmapWCSTR DCMAPAPI GetDisplayName(){return L"Ёкспорт CSV";}
	dcmapWCSTR DCMAPAPI GetFileFilters(){return L"файлы CSV|*.csv";}
	dcmapWCSTR DCMAPAPI GetDefaultExt(){return  L"csv";}

	void* DCMAPAPI CastTo(char const* idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapExportProcessor);
		return 0;
	}

	char const* DCMAPAPI GetConfigSectionName()
	{
		static string ConfigSectionParams;
		ConfigSectionParams = "CSVExport_"+m_strTable;
		return ConfigSectionParams.c_str();
	}
public:
	CCSVTableExport()
	{
		m_pFilter=0;
	}
};

DCMAP_REGISTER(CCSVTableExport,"PTextExport","UNI_CSVExport")

