#include "StdAfx.h"
#include "RecordExport.h"
#include "utils.h"
#include <sstream>
#include "Parser.h"
using namespace gsys;

struct CRecordExportService : public IDCMapRecordExport
{
	IDCMapCore* m_pCore;

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapRecordExport);
		return 0;
	}

	bool DCMAPAPI ExportClipboard(dcmapCSTR strTable,dcmapDCSTR strFormat,dcmapDBREF const* CurrentRecord)
	{
		wstring dta = m_pCore->GetCoreUI()->ClipboardGetW();
		if(dta.empty())return false;

		CModuleParser prs;
		prs.Init(m_pCore,0);
		wiparsestream is(strFormat);
		prs.Rights(PRSR_READ);
		prs.SetReadOnly(true);
		prs.SetTable(strTable);

		if(CurrentRecord)
		{
			if(!prs.SelectID(*CurrentRecord))return false;
		}
		prs.SetDefaultValue(dta);
		prs.SetReadOnly(true);

		CVarValue res;
		try
		{
			prs.ParseString(true,is,res,false);
		}
		catch (...){return false;}

		m_pCore->GetCoreUI()->ClipboardPutW(res.to_string().c_str());
		return true;
	}
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pCore = core;
		return true;
	}
};
DCMAP_REGISTER(CRecordExportService,"PCoreService","RecordExport");

struct CExpressionFilterProcessor : public IDCMapExpressionTableFilterProcessor
{
    wstring m_strExpr;
    CModuleParser m_Parser;
	IDCMapCore* m_pCore;
    wiparsestream is;

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pCore = core;
        m_Parser.Rights(PRSR_FILTER);
        m_Parser.InitObject();
        m_Parser.Init(core,workspace);
		m_Parser.SetReadOnly(true);
		return true;
	}

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapExpressionTableFilterProcessor);
		return 0;
	}

	bool DCMAPAPI Setup(dcmapCSTR strTable,dcmapDCSTR wstrExpression)
	{
		if(!strTable || !strTable[0])return false;
		if(!wstrExpression || !wstrExpression[0])return false;

		m_strExpr = wstrExpression;
		//wcslwr((dcmapWSTR)m_strExpr.data());

		if(!m_Parser.SetTable(strTable))return false;
		m_Parser.SetReadOnly(true);
        is.setup(m_strExpr);
		return true;
	}

	bool DCMAPAPI Process(dcmapDBREF const* Record)
	{
		if(m_strExpr.empty())return false;
		CVarValue res;
        is.clear();
		is.seekg(0);
		try
		{
			m_Parser.SelectID(*Record);
			m_Parser.Parse(true,is,res,false);
		}
		catch (...){return false;}
		return res.to_bool();
	}
};

DCMAP_REGISTER(CExpressionFilterProcessor,"PModules","ExprTableFilterProc");


