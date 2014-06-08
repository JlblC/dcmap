p// dcmapsys.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../dcmapsys.h"
#include "utils.h"
#include "XMLite.h"

using namespace std;
using namespace gsys;

class CXMLExport : public IDCMapExportProcessor
{
	string m_strTable;
	IDCMapDataStorage* m_pData;
	IDCMapDataSource* m_pSource;
	IDCMapCore* m_pCore;
	IDCMapExpressionTableFilterProcessor* m_pFilter;

public:
	CXMLExport():m_pFilter(0),m_pData(0),m_pSource(0)
	{

	}
	void* DCMAPAPI CastTo(char const* idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapExportProcessor);
		return 0;
	}
	dcmapDWORD DCMAPAPI GetExportParams(){return DCMAP_EXPORT_UNIVERSAL;}
	dcmapWCSTR DCMAPAPI GetDisplayName(){return L"Ёкспорт XML";}
	dcmapWCSTR DCMAPAPI GetFileFilters(){return L"файлы XML|*.xml";}
	dcmapWCSTR DCMAPAPI GetDefaultExt(){return  L"xml";}

	bool DCMAPAPI SetTextParam(dcmapDCSTR szParam){return true;}
	bool DCMAPAPI SetFilterExpr(dcmapDCSTR szExpr){return true;}

	char const* DCMAPAPI GetConfigSectionName();

	bool DCMAPAPI Export(IDCMapProcessRecordReseiver* pRsvr,dcmapDWORD options);
	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	bool DCMAPAPI SetDataSourceName(const char* szName);
	bool DCMAPAPI SetDataSource(IDCMapDataSource* pSource);

};
DCMAP_REGISTER(CXMLExport,"PTextExport","XMLExport")



bool DCMAPAPI CXMLExport::Init( IDCMapCore* core,IDCMapWorkspace* workspace )
{
	m_pCore=core;
	m_pSource=0;
	return true;
}

bool DCMAPAPI CXMLExport::SetDataSourceName( const char* szName )
{
	m_strTable = szName;
	m_pData = m_pCore->GetDataSourceTyped<IDCMapDataStorage>(m_strTable.c_str());
	if(!m_pData)return false;
	if(!m_pSource)m_pSource=m_pData;
	return true;
}

bool DCMAPAPI CXMLExport::SetDataSource( IDCMapDataSource* pSource )
{
	if(pSource)
	{
		m_pSource = pSource;
	}
	else m_pSource = m_pData;
	return true;
}

bool DCMAPAPI CXMLExport::Export( IDCMapProcessRecordReseiver* pRsvr,dcmapDWORD options )
{
	XDoc doc;
	LPXNode root = doc.AppendChild(L"dcmap");
	LPXNode database = root->AppendChild(L"data"); // implicit notation
	LPXNode table = database->AppendChild(ToWide(m_pData->Name()).c_str()); // implicit notation
	wstring RecordName = ToWide(m_pData->RecordName());
	
	vector<wstring> Cols;
	int ColCount = m_pData->NumColumns();
	Cols.resize(ColCount);
	bool pre = false;
	for(int i=1;i<ColCount;i++)
	{
		char const* nm = m_pData->ColumnName(i);
		int param = m_pData->ColumnParams(i);
		if(nm && nm[0] && !(param&DCMAP_COLUMN_HIDDEN) )
		{
			Cols[i] = ToWide(nm);
		}
	}

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
		if(m_pFilter && !m_pFilter->Process(&refs[j]))continue;
		if(!m_pData->SelectID(refs[j]))continue;

		LPXNode record = table->AppendChild(RecordName.c_str());

		for(int i=1;i<ColCount;i++)
		{
			if(!Cols[i].empty())
			{
				LPXNode val = record->AppendChild(Cols[i].c_str(),m_pData->GetWideString(i));
			}
		}
	}

	DISP_OPT opt;
	opt.tab_chars = L"  ";
	pRsvr->AddRecord(doc.GetXML(&opt).c_str());
	return true;
}

char const* DCMAPAPI CXMLExport::GetConfigSectionName()
{
	static string ConfigSectionParams;
	ConfigSectionParams = "XMLExport_"+m_strTable;
	return ConfigSectionParams.c_str();
}