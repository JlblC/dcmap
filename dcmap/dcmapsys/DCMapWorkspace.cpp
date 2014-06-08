#include "StdAfx.h"
#include "DCMapWorkspace.h"
#include "DCMapCore.h"
#include "DCMapSystem.h"
#include <g_file_util.h>

#include "../api/dcmap_script_helper.h"

DCMAP_SCRIPT_REFLECTION(CDCMapWorkspace)
{
	DCMAP_SCRIPT_FUNC(CreateModule);
	DCMAP_SCRIPT_FUNC(DestroyModule);
	DCMAP_SCRIPT_FUNC(GetService);

	DCMAP_SCRIPT_PROPERTY_R(UI);
	DCMAP_SCRIPT_PROPERTY_R(Core);
}

CDCMapWorkspace::CDCMapWorkspace(IDCMapFrameEx* pFrame,IDCMapCoreEx* pCore)
{
	m_pCore = pCore;
	m_pFrame = pFrame;
	m_pWorkspace = this;
	m_pUI=0;

	m_itCurrentMod = m_mapModules.end();
	m_itEndMod = m_mapModules.end();
	m_bActive=false;
}

CDCMapWorkspace::~CDCMapWorkspace(void)
{
	BroadcastMessage(DCMM_PRE_DESTROY,0,0,0);
	_foreach(ObjectsSet,m_Objects,it)
	{
		(*it)->SaveConfig(&m_Config);
		g_dcmapCore.DestroyWorkspaceModule(*it);
	}
	m_Objects.clear();
}

IDCMapCore* DCMAPAPI CDCMapWorkspace::GetCore()
{
	return &g_dcmapCore;
}

void DCMAPAPI CDCMapWorkspace::SetConfigFile(dcmapDCSTR Filename)
{
	m_strConfigFile = Filename;
}

void DCMAPAPI CDCMapWorkspace::ConfigureFromFile(dcmapDCSTR szFilename)
{
    wstring BaseFilename;
	if(szFilename && szFilename[0])BaseFilename = szFilename;
    else BaseFilename = m_strConfigFile;

    wstring Filename = gsys::GetAppStartupDirW()+BaseFilename;
    wstring TplFilename = gsys::GetAppStartupDirW()+L"tpl\\"+BaseFilename;

    if(gsys::FileExists(Filename))
    {
        m_Config.Load(Filename.c_str());
    }
    else
    {
        m_Config.Load(TplFilename.c_str());
    }
}

void DCMAPAPI CDCMapWorkspace::SaveConfig(dcmapDCSTR szFilename)
{
	wstring cfg = m_strConfigFile;
	if(szFilename && szFilename[0])cfg = szFilename;

	_foreach(ObjectsSet,m_Objects,it)
	{
		(*it)->SaveConfig(&m_Config);
	}

    if(!cfg.empty())m_Config.Save((gsys::GetAppStartupDirW()+cfg).c_str());
}


IDCMapModule* DCMAPAPI CDCMapWorkspace::CreateModule(char const* czClass, char const* czName)
{
	if(!czClass || !czClass[0])return 0;
	if(!czName || !czName[0])return 0;

	IDCMapObject* pObj = g_dcmapSys.CreateObject(czClass,czName);
	if(!pObj)return 0;

	IDCMapModule* pModule = pObj->GetFinalModule(); // problem here
	if(!pModule)
	{
		g_dcmapSys.DestroyObject(pObj);
		return 0;
	}

	if(!pModule->Init(&g_dcmapCore,this))
	{
		g_dcmapSys.DestroyObject(pObj);
		return 0;
	}
	char const* szID = pModule->ModuleID();
	if(szID && szID[0])
	{
		m_mapModules.insert(StrModPair(szID,pModule));
	}
	m_mapModules.insert(StrModPair(std::string(czClass)+"/"+czName,pModule));

	m_itCurrentMod = m_mapModules.end();
	m_itEndMod = m_mapModules.end();

	m_Objects.insert(pModule);
	pModule->LoadConfig(&m_Config);

	return pModule;
}


void DCMAPAPI CDCMapWorkspace::DestroyModule(IDCMapModule* pObj)
{
	pObj->SaveConfig(&m_Config);
	UnregisterMod(pObj);
	g_dcmapCore.DestroyWorkspaceModule(pObj);
}
bool ProcessMsgResult(dcmapLRESULT ret,dcmapLRESULT& result,dcmapDWORD Options);

dcmapLRESULT DCMAPAPI CDCMapWorkspace::BroadcastMessage(int Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapModule* pSender,dcmapDWORD Options)
{
	dcmapLRESULT result = 0;
	dcmapLRESULT ret;

	ret = OnMessage(Msg,wParam,lParam,pSender);
	if(!ProcessMsgResult(ret,result,Options))return result;

	_foreach(ObjectsSet,m_Objects,it)
	{
		ret = (*it)->OnMessage(Msg,wParam,lParam,pSender);
		if(!ProcessMsgResult(ret,result,Options))return result;
	}
	return result;
}

bool DCMAPAPI CDCMapWorkspace::SelectModuleByID(char const* czModuleID)
{
	m_itCurrentMod = m_mapModules.lower_bound(czModuleID);
	m_itEndMod = m_mapModules.upper_bound(czModuleID);
	return m_itCurrentMod!=m_itEndMod;
}

IDCMapModule* DCMAPAPI CDCMapWorkspace::Module()
{
	if(m_itCurrentMod==m_itEndMod)return 0;
	return m_itCurrentMod->second;
}

bool DCMAPAPI CDCMapWorkspace::Next()
{
	if(m_itCurrentMod==m_itEndMod)return false;
	++m_itCurrentMod;
	if(m_itCurrentMod==m_itEndMod)return false;
	return true;
}

void DCMAPAPI CDCMapWorkspace::PostInit()
{
	_foreach(ObjectsSet,m_Objects,it)
	{
		(*it)->OnMessage(DCMM_POST_INIT,0,0,0);
	}
}

void DCMAPAPI CDCMapWorkspace::OnActivate()
{
	if(m_bActive)return;
	m_bActive=true;
	if(m_bNeedReconfigure)
	{
		m_bNeedReconfigure = false;
		BroadcastMessage(DCMM_CONFIG_CHANGED,0,0,0);
	}
	if(m_bNeedDataUpdate)
	{
		OnDataUpdate();
	}

	g_dcmapCore.SetCurrentWorkspace(this);
	BroadcastMessage(DCMM_WORKSPACE_ACTIVATE,0,0,0);
}

void DCMAPAPI CDCMapWorkspace::OnDeactivate()
{
	if(!m_bActive)return;
	BroadcastMessage(DCMM_WORKSPACE_DEACTIVATE,0,0,0);
	m_bActive=false;
	if(g_dcmapCore.GetCurrentWorkspace() == this)
			g_dcmapCore.SetCurrentWorkspace(0);
}

void DCMAPAPI CDCMapWorkspace::OnDataUpdate()
{
	if(m_bActive)
	{
		m_bNeedDataUpdate = false;
		BroadcastMessage(DCMM_DATA_PREPROCESS,0,0,0);
		BroadcastMessage(DCMM_DATA_PROCESS,0,0,0);
		BroadcastMessage(DCMM_DATA_POSTPROCESS,0,0,0);
		BroadcastMessage(DCMM_DATA_CHANGED,0,0,0);
	}
	else
	{
		m_bNeedDataUpdate = true;
	}
}
void DCMAPAPI CDCMapWorkspace::OnConfigChange()
{
	if(m_bActive)
	{
		m_bNeedReconfigure = false;
		BroadcastMessage(DCMM_CONFIG_CHANGED,0,0,0);
	}
	else
	{
		m_bNeedReconfigure = true;
	}
}

IDCMapModule* DCMAPAPI CDCMapWorkspace::GetService(const char* Name)
{
	MapModules::iterator it = m_Services.find(Name);
	if(it != m_Services.end())return it->second;
	IDCMapModule* mod = CreateModule("PService",Name);
	if(mod)m_Services[Name] = mod;
	return mod;	
}

IDCMapFrameUI* DCMAPAPI CDCMapWorkspace::GetFrameUI()
{
	return m_pFrame?m_pFrame->GetUI():0;
}

void* DCMAPAPI CDCMapWorkspace::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(CDCMapWorkspace);
	DCMAP_MODULE_SUPPORTS(IDCMapWorkspaceEx);
	DCMAP_MODULE_SUPPORTS(IDCMapWorkspace);
	DCMAP_MODULE_SUPPORTS(IDCMapScriptable);
	DCMAP_MODULE_SUPPORTS(IDCMapObject);
	return 0;
}