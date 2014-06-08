#include "StdAfx.h"
#include "Utils.h"
#include "ScriptModule.h"
#include "ScriptSystem.h"
#include "DCMapCore.h"
#include "utils.h"

CScriptSystem g_ScriptSystem;
CModuleParser g_GlobalModule; 

void CScriptSystem::Init()
{
	g_GlobalModule.SetVariable(L"dcmapdir",CVarValue(GetAppStartupDir()));

	DCMapConfigWrapper cfg(GetDCMapCore());

	cfg.section("NetSettings");
	string str;
	if(cfg["GameURL"] >> str)
		g_GlobalModule.SetVariable(L"gameurl",CVarValue(str));

}

void CScriptSystem::Terminate()
{
	g_GlobalModule.Destruct();
}

IDCMapScriptModule* DCMAPAPI CScriptSystem::LoadScriptModuleFromFile(dcmapFCSTR szFileName,IDCMapCore* pCore,int Falgs)
{
	if(!pCore)pCore = GetDCMapCore();

	CScriptModule* pModule = new CScriptModule;
	if(!pModule->LoadFromFile(szFileName))
	{
		delete pModule;
		return 0;
	}

	if(!((CDCMapCore*)pCore)->AddModule(pModule))
	{
		delete pModule;
		return 0;
	}

	return pModule;
}

IDCMapScriptModule* DCMAPAPI CScriptSystem::LoadScriptModuleFromText(dcmapFCSTR szText,IDCMapCore* pCore,int Falgs)
{
	if(!pCore)pCore = GetDCMapCore();

	CScriptModule* pModule = new CScriptModule;
	if(!pModule->LoadSource(szText))
	{
		delete pModule;
		return 0;
	}

	if(!((CDCMapCore*)pCore)->AddModule(pModule))
	{
		delete pModule;
		return 0;
	}
	return pModule;
}

IDCMapVarValue* DCMAPAPI CScriptSystem::CreateVarValue()
{
	return new CDCMapVarValue;
}

dcmapWCSTR DCMAPAPI CScriptSystem::ProcessFormatedString( dcmapWCSTR Format,IDCMapVarValue* Values,int Flags )
{
	static CVarValue s_res;

	CDCMapVarValue* val = (CDCMapVarValue*)Values;

	if(!Values || !Format || !Format[0])return L"";
	CBlockParser prs;

	if(val)prs.UseVariable(*val);

	wiparsestream is(Format);
	prs.ParseString(true,is,s_res,false);

	return s_res.as_string().c_str();
}

dcmapWCSTR DCMAPAPI CScriptSystem::ProcessFormatedStringDB( dcmapWCSTR Format,IDCMapDataSource* pDataStorage,dcmapDBREF const* pDBRef, IDCMapVarValue* Values,int Flags )
{
	static CVarValue s_res;
	CDCMapVarValue* val = (CDCMapVarValue*)Values;
	if(!Values || !Format || !Format[0])return L"";
	CDBBlockParser prs;

	prs.Rights(PRSR_READ);

	if(pDataStorage) prs.SetTable(0,pDataStorage->GetStorage());
	if(pDBRef)prs.SelectID(*pDBRef);

	if(val)prs.UseVariable(*val);

	wiparsestream is(Format);
	prs.ParseString(true,is,s_res,false);

	return s_res.as_string().c_str();
}

bool DCMAPAPI CScriptSystem::CallGlobalProc( dcmapWCSTR Proc,IDCMapVarValue* Params,IDCMapVarValue* Result )
{
	return g_GlobalModule.CallProc(Proc,Params,Result);
}

DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapProcessFormatedString(dcmapWCSTR Format,IDCMapVarValue* Values,int Flags)
{
	return g_ScriptSystem.ProcessFormatedString(Format,Values,Flags);
}

DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapProcessFormatedStringDB(dcmapWCSTR Format,IDCMapDataSource* pDataStorage,dcmapDBREF const* pDBRef,IDCMapVarValue* Values,int Flags)
{
	return g_ScriptSystem.ProcessFormatedStringDB(Format,pDataStorage,pDBRef,Values,Flags);
}