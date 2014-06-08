#include "StdAfx.h"
#include "Parser.h"
#include "Utils.h"
#include "ScriptModule.h"
#include <g_std_file.h>
#include <g_text.h>
#include <g_file_util.h>
#include "Keywords.h"

using namespace gsys;

CScriptModule::CScriptModule()
{
    m_pRootModule = this;
    m_Rights = PRSR_ALL;
}

struct CAddMenuKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt,wiparsestream &is)
{
	CScriptModule* mod = cnt->ScriptModule();
	if(!mod)throw(parse_exception(is,PERR_INVALID_CONTEXT));
	return mod->AddMenu(param);
}};
static KeywordAutoReg<CAddMenuKW> sCAddMenuKW(L"add_menu");

bool CScriptModule::LoadSource(dcmapWCSTR szBuffer,dcmapFCSTR szFileName)
{
	if(!szBuffer)return false;
	buffer = szBuffer;
	stream.setup(buffer,szFileName);

	if(szFileName && szFileName[0])
	{
		wstring PluginName = StripEXT(ExtractFileName(wstring(szFileName)));
		wstring PluginBase = GetAppStartupDirW()+L"plugins/"+PluginName;
		wstring PluginDir = PluginBase+L"/";

		SetVariable(L"modname",CVarValue(PluginName));
		SetVariable(L"modbase",CVarValue(PluginBase));
		SetVariable(L"moddir",CVarValue(PluginDir));

		g_pLocalStrings->UpdateFrom((PluginBase+L".lng").c_str());
	}

	return true;
}

bool CScriptModule::LoadFromFile(dcmapFCSTR szFileName)
{
	dcmapWCSTR buf = dcmapLoadText(szFileName,DCMapCodePage);
	if(!buf)return false;
	return LoadSource(buf,szFileName);
}

bool CScriptModule::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
    if(!CModuleParser::Init(core,workspace))return false;
	Execute();
	return true;
}

void CScriptModule::Execute()
{
	if(m_pCore)m_pCore->LockDataUpdate();
	CVarValue val;
	try
	{
		if(m_pCore && m_pCore->GetCoreUI())
		{
			pRsrv = m_pCore->GetCoreUI()->GetConsoleFidder();
		}
		Parse(true,stream,val);
	}
	catch(parse_exception e)
	{
		if(m_pCore)m_pCore->GetCoreUI()->ErrorMsg(e.info());
	}
	if(m_pCore)m_pCore->UnlockDataUpdate();
	if(pRsrv)pRsrv->Hide();
	stream.setup(buffer);
}

void CScriptModule::Execute(CVarValue val)
{
    CVarValue param;
    CVarValue res;
    m_pCore->LockDataUpdate();
    SafeExecute(val,param,res);
	m_pCore->UnlockDataUpdate();
	if(pRsrv)pRsrv->Hide();
	stream.setup(buffer);
}

CVarValue CScriptModule::AddMenu(CVarValue const& params)
{
	IDCMapUIItem* pItem = m_pCore->GetCoreUI()->AddMenuItem(WideToString(params[0].to_string()).c_str(),
								params[1].to_string().c_str(),this,m_vecHandlers.size());

	if(params.size() > 2)
		m_vecHandlers.push_back(params[2]);
	else 
		m_vecHandlers.push_back(CVarValue());

	return CVarValue(pItem);
}

void CScriptModule::AddContextMenu(CVarValue const& params)
{
	IDCMapUIContext* pContext =  m_pCore->GetCoreUI()->GetContext(WideToString(params[0].to_string()).c_str());
	if(!pContext)return;

	pContext->AddMenuItem(WideToString(params[1].to_string()).c_str(),
		params[2].to_string().c_str(),
		this,m_vecHandlers.size());

	if(params.size() > 4)
		m_vecHandlers.push_back(ActionHandler(params[3],params[4]));
	else if(params.size() > 3)
		m_vecHandlers.push_back(ActionHandler(params[3]));
	else 
		m_vecHandlers.push_back(CVarValue());
}

IDCMapObject* DCMAPAPI ScriptModuleCreateCallback(DCMapObjectCreationParams const* pCreationParams)
{
    CVarValue* creator = reinterpret_cast<CVarValue*>(pCreationParams->pUserData);
    if(!creator || !creator->callable())return NULL;

    CVarValue params,res;
    try
    {
        if(!creator->call(params,res))return NULL;
    }
    catch (...)
    {
        return 0;
    }
    if(res.type() != VT_OBJECT)return NULL;
    IDCMapModule* pMod = res.get_object_module();

    if(pMod)
    {
        res.add_object_ref();
    }
    return pMod;
}

bool CScriptModule::RegisterModule(CVarValue const& mod,string mclass,string mname)
{
	if(mod.type() != VT_MODULE && mod.type() != VT_SUB) 
		return false;

	IDCMapSys* pSys = GetDCMapSys();

	CVarValue* pCreator = new CVarValue(mod);
	pSys->RegisterPlugin(mclass.c_str(),mname.c_str(),
		ScriptModuleCreateCallback,pCreator);

	return true;      
}

bool CScriptModule::ParseCustomKeyword(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt)
{
    CVarValue params;
	if(kw == L"add_context_menu")
	{
		cnt->ParseParams(active,is,params,3,5);

		if(active)AddContextMenu(params);
		res = true;
		if(!expr)cnt->ContinueExpression(active,is,res);
		return true;
	}
    else if(kw == L"html_form")
    {
        cnt->ParseParams(active,is,params,1,2);
        if(active)
        {
            IDCMapUIItem* pItem = m_pCore->GetCoreUI()->CreateUIModule("HTMLFrame",this,0);
            if(!pItem)throw parse_exception(is,PERR_INTERNAL);

            IDCMapUIHTMLFrame* html_frame = pItem->Cast<IDCMapUIHTMLFrame>();
            if(!html_frame){pItem->Destroy();throw parse_exception(is,PERR_INTERNAL);}
            
            html_frame->LoadHTML(params[0].as_string().c_str(),params[1].as_string().c_str());

            res = m_pCore->GetCoreUI()->ShowModal(html_frame,0,0);          
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }
    else if(kw == L"register_module")
    {
        cnt->ParseParams(active,is,params,3);
        if(active)
        {
            if(params.size()<3)throw parse_exception(is,PERR_INVALID_PARAMS);

            string mclass = WideToString(params[0].as_string());
            string mname  = WideToString(params[1].as_string());

            if(params[2].type() != VT_MODULE && params[2].type() != VT_SUB) 
                throw parse_exception(is,PERR_INVALID_PARAMS);

            IDCMapSys* pSys = GetDCMapSys();

            CVarValue* pCreator = new CVarValue(params[2]);
            pSys->RegisterPlugin(mclass.c_str(),mname.c_str(),
                        ScriptModuleCreateCallback,pCreator);

            res = CVarValue(true);       
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }
	return CDBBlockParser::ParseCustomKeyword(active,kw,is,res,expr,cnt);
}

