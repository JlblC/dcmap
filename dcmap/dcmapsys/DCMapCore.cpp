#include "stdafx.h"
#include "DCMapCore.h"

#include "PlanetDataStorage.h"
#include "DCMapWorkspace.h"
#include "dcmapsystem.h"
#include "g_file_util.h"
#include "LocalStrings.h"
#include "DCMapConsole.h"
#include "database.h"
#include <g_file_util.h>

#include "ScriptSystem.h"
#include <g_file_util.h>

#include "../api/dcmap_script_helper.h"


DCMAPSYSEXPORT IDCMapWorkspaceEx* DCMAPLIBAPI CreateDCMapWorkspace()
{
	return g_dcmapCore.CreateWorkspace();
}
DCMAPSYSEXPORT void DCMAPLIBAPI DestroyDCMapWorkspace(IDCMapWorkspaceEx* pWorkspace)
{
	g_dcmapCore.DestroyWorkspace(pWorkspace);
}

DCMAP_SCRIPT_REFLECTION(CDCMapCore)
{
	DCMAP_SCRIPT_FUNC(LockDataUpdate);
	DCMAP_SCRIPT_FUNC(UnlockDataUpdate);
	DCMAP_SCRIPT_FUNC(CreateModule);
	DCMAP_SCRIPT_FUNC(DestroyModule);
	DCMAP_SCRIPT_FUNC(GetService);
	DCMAP_SCRIPT_PROPERTY(Database);

	DCMAP_SCRIPT_PROPERTY_R(UI);
}

void* DCMAPAPI CDCMapCore::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(IDCMapCoreEx);
	DCMAP_MODULE_SUPPORTS(IDCMapCore);
	DCMAP_MODULE_SUPPORTS(IDCMapModuleManager);
	DCMAP_MODULE_SUPPORTS(IDCMapModule);
	DCMAP_MODULE_SUPPORTS(IDCMapScriptable);
	DCMAP_MODULE_SUPPORTS(IDCMapObject);
	return 0;
}

CDCMapCore g_dcmapCore;

CDCMapCore::CDCMapCore(void)
{
	m_pCoreUI=0;
	m_pCurrentWorkspace=0;
	m_pCurrentDatabase=0;
	m_iDataUpdateLock=0;
	m_bNeedUpdate=false;
	m_bUsebackup=true;
	m_bCompressDatabase=false;
	m_iNumBackups=3;
	m_bUseHardlinks=false;
	m_bInitialized=false;
}

CDCMapCore::~CDCMapCore(void)
{
	//Destroy();
}

IDCMapConsole* DCMAPAPI CDCMapCore::GetConsole()
{
	return &g_Console;
}


void DCMAPAPI CDCMapCore::Destroy()
{
	if(!m_bInitialized)return;

	BroadcastMessage(DCMM_PRE_DESTROY,0,0,0);
	g_ScriptSystem.Terminate();

	_foreach(SetWorkspaces,m_Workspaces,it)
	{
		delete *it;
	}
	m_Workspaces.clear();

	if(m_pCurrentDatabase)
	{
		m_pCurrentDatabase->Destroy();
		m_pCurrentDatabase = 0;
	}

	_foreach(ObjectsSet,m_Objects,it)
	{
		(*it)->SaveConfig(&m_Config);
		if(m_pCoreUI)m_pCoreUI->DestroyModuleItems((*it));
		(*it)->Destroy();
	}
	m_Objects.clear();	

	m_bInitialized = false;
}


void CDCMapCore::AddDataStorage(const char* name,IDCMapDataStorage* pStor)
{
	//pStor->Init(this,0);
	//m_mapStorages[name] = pStor;
}

void CDCMapCore::LoadCoreModules(char const* PClass)
{
	std::vector<string> strs;
	g_dcmapSys.GetClassPluginNamesT(PClass,strs);
	for(int i=0;i<strs.size();i++)
	{
		IDCMapModule* mod = CreateModule(PClass,strs[i].c_str());
		if(mod)m_Daemons[strs[i]] = mod;
	}
}

void CDCMapCore::LoadCoreServices(char const* PClass)
{
	std::vector<string> strs;
	g_dcmapSys.GetClassPluginNamesT(PClass,strs);
	for(int i=0;i<strs.size();i++)
	{
		IDCMapModule* mod = CreateModule(PClass,strs[i].c_str());
		if(mod)m_Services[strs[i]] = mod;
	}
}

void DCMAPAPI CDCMapCore::InitModules()
{
	m_pCurrentDatabase = (CDCMapDatabase*)g_dcmapSys.OnNewObject(new CDCMapDatabase);
	if(m_pCurrentDatabase)
	{
		m_pCurrentDatabase->Init(this,0);
	}

	LoadCoreModules("PCoreDaemon");
	LoadCoreModules("PCoreDemon");
	m_bInitialized = true;
}

void CDCMapCore::Init()
{
}

IDCMapDataSource* DCMAPAPI CDCMapCore::GetDataSource(char const* pName)
{
	if(m_pCurrentDatabase)
	{
		return m_pCurrentDatabase->GetDataSource(pName);
	}
	return 0;
}

void DCMAPAPI CDCMapCore::LoadDatabaseFrom(dcmapFCSTR pDir)
{
	sysstring Dir =  pDir?pDir:GetAppStartupDir()+dcmapFT("data\\");

	if(m_pCurrentDatabase)
	{
		sysstring TemplateDir = GetAppStartupDir()+dcmapFT("tpl\\");

		int Flags=0;
		int LoadFlags = DCMAP_DSLF_LAZY;
		m_pCurrentDatabase->Load(Dir.c_str(),TemplateDir.c_str(),Flags,LoadFlags);
	}
}

void DCMAPAPI CDCMapCore::SaveDatabaseTo(dcmapFCSTR pDir)
{
	if(m_pCurrentDatabase)
	{
		int Flags = 0;
		int SaveFlags = m_bCompressDatabase?DCMAP_DSLF_GZIPPED:0;

		m_pCurrentDatabase->Save(pDir,Flags,SaveFlags);
	}
}

IDCMapWorkspaceEx* DCMAPAPI CDCMapCore::CreateWorkspace()
{
	//CDCMapWorkspace* Wrsp = new CDCMapWorkspace;
	//m_Workspaces.insert(Wrsp);
	//return Wrsp;
	return 0;
}

void DCMAPAPI CDCMapCore::DestroyWorkspace(IDCMapWorkspaceEx* pWrsp)
{
	m_Workspaces.erase((CDCMapWorkspace*)pWrsp);
	delete (CDCMapWorkspace*)pWrsp;
}

void DCMAPAPI CDCMapCore::OnDataUpdate()
{
	if(m_iDataUpdateLock <= 0)
	{
		BroadcastMessage(DCMM_DATA_PREPROCESS,0,0,0,DCMAP_BRADCAST_CORE_ONLY);
		BroadcastMessage(DCMM_DATA_PROCESS,0,0,0,DCMAP_BRADCAST_CORE_ONLY);
		BroadcastMessage(DCMM_DATA_POSTPROCESS,0,0,0,DCMAP_BRADCAST_CORE_ONLY);
		BroadcastMessage(DCMM_DATA_CHANGED,0,0,0,DCMAP_BRADCAST_CORE_ONLY);

		m_bNeedUpdate=false;
		_foreach(SetWorkspaces,m_Workspaces,it)
		{
			(*it)->OnDataUpdate();
		}
	}
	else
	{
		m_bNeedUpdate=true;
	}
}

void DCMAPAPI CDCMapCore::OnConfigChange()
{
	m_bNeedUpdate=false;
	BroadcastMessage(DCMM_CONFIG_CHANGED,0,0,0,DCMAP_BRADCAST_CORE_ONLY);

	_foreach(SetWorkspaces,m_Workspaces,it)
	{
		(*it)->OnConfigChange();
	}
}


void DCMAPAPI CDCMapCore::LockDataUpdate()
{
	m_iDataUpdateLock++;
}
void DCMAPAPI CDCMapCore::UnlockDataUpdate()
{
	m_iDataUpdateLock--;
	if(m_iDataUpdateLock<=0)
	{
		if(m_bNeedUpdate)OnDataUpdate();
		m_iDataUpdateLock=0;
	}
}
void DCMAPAPI CDCMapCore::ForceDataUpdate()
{
    if(m_bNeedUpdate)
    {
        m_bNeedUpdate=false;
        _foreach(SetWorkspaces,m_Workspaces,it)
        {
            (*it)->OnDataUpdate();
        }   
    }
}

void DCMAPAPI CDCMapCore::ConfigureFromFile(dcmapFCSTR szFilename,
                                            dcmapFCSTR szDefaultsFilename,
                                            dcmapFCSTR szUpdateFilename)
{
	bool NeedSave = false;
    if(szFilename)
    {
        sysstring Filename = GetAppStartupDir()+szFilename;
        if(szDefaultsFilename)
        {
            sysstring DefFilename = GetAppStartupDir()+szDefaultsFilename;
            if(gsys::FileExists(DefFilename))
            {
                m_Config.Load(DefFilename.c_str());
				dcmapDeleteFile(DefFilename.c_str());
				NeedSave=true;
            }
        }
        if(gsys::FileExists(Filename))m_Config.Load(Filename.c_str());
        else 
        {
            sysstring TplFilename = GetAppStartupDir()+dcmapFT("tpl\\")+szFilename;
            if(gsys::FileExists(TplFilename))
            {
                m_Config.Load(TplFilename.c_str());
				NeedSave=true;
            }
        }
		if(szUpdateFilename)
        {
            sysstring UpdFilename = GetAppStartupDir()+szUpdateFilename;
            if(gsys::FileExists(UpdFilename))
            {
                m_Config.Load(UpdFilename.c_str());
				dcmapDeleteFile(UpdFilename.c_str());
				NeedSave=true;
            }
        }
		if(NeedSave) m_Config.Save(Filename.c_str());
    }
}

void DCMAPAPI CDCMapCore::SaveConfig(dcmapFCSTR Filename)
{
	if(Filename)m_Config.Save(Filename);
}

IDCMapModule* DCMAPAPI CDCMapCore::CreateModule(char const* czClass, char const* czName)
{
	IDCMapObject* pObj = g_dcmapSys.CreateObject(czClass,czName);
	if(!pObj)return 0;

	IDCMapModule* pModule = pObj->GetFinalModule();
	if(!pModule)
	{
		g_dcmapSys.DestroyObject(pObj);
		return 0;
	}

	if(!pModule->Init(this,0))
	{
        g_dcmapSys.DestroyObject(pObj);
		return 0;
	}
	m_Objects.insert(pModule);
	pModule->LoadConfig(&m_Config);
	return pModule;
}

IDCMapModule* DCMAPAPI CDCMapCore::AddModule(IDCMapModule* pObj)
{
	if(!pObj)return 0;
	if(!pObj->Init(this,0))
	{
		return 0;
	}
	m_Objects.insert(pObj);
	pObj->LoadConfig(&m_Config);
	return pObj;
}

void DCMAPAPI CDCMapCore::DestroyWorkspaceModule(IDCMapModule* pObj)
{
	if(!pObj)return;
	m_pCoreUI->DestroyModuleItems(pObj);
	pObj->Destroy();
}


void DCMAPAPI CDCMapCore::DestroyModule(IDCMapModule* pObj)
{
	pObj->SaveConfig(&m_Config);
	m_pCoreUI->DestroyModuleItems(pObj);
	m_Objects.erase(pObj);
	pObj->Destroy();
}

void DCMAPAPI CDCMapCore::PostInit()
{
	LoadCoreModules("PUtility");
	LoadCoreModules("PCoreUtility");

	BroadcastMessage(DCMM_POST_INIT,0,0,0,DCMAP_BRADCAST_CORE_ONLY);
}

void DCMAPAPI CDCMapCore::SaveWithBackup(dcmapFCSTR pDir,dcmapFCSTR pBackupDir,int )
{
	sysstring Dir =  pDir?pDir:GetAppStartupDir()+dcmapFT("data\\");
	sysstring Backup = pBackupDir?pBackupDir:GetAppStartupDir()+dcmapFT("backup\\");

	int iNumBackups = 0;
	if(m_bUsebackup) iNumBackups = m_iNumBackups;

	if(m_pCurrentDatabase)
	{
		if(!m_pCurrentDatabase->DataChanged())return;

		int Flags = DCMAP_DBSLF_SAVE_ONLY_MODIFIED;
		if(m_bUseHardlinks) Flags |= DCMAP_DBSLF_USE_HARDLINKS;

		int SaveFlags = m_bCompressDatabase?DCMAP_DSLF_GZIPPED:0;

		m_pCurrentDatabase->SaveWithBackup(Dir.c_str(),Backup.c_str(),iNumBackups,Flags,SaveFlags);
	}
}

void DCMAPAPI CDCMapCore::ConfigureSystem()
{
	g_dcmapSys.ConfigureSystemOptions(&m_Config);
	LoadConfig();
}

void CDCMapCore::LoadConfig()
{
	DCMapConfigWrapper cfg(&m_Config);
	cfg.section("Core");

	cfg.section("DataBase");
	cfg["NumBackups"] >> m_iNumBackups;
	cfg["UseBackups"] >> m_bUsebackup;
	cfg["UseHardlinks"] >> m_bUseHardlinks;
	cfg["CompressDatabase"] >> m_bCompressDatabase;
}

IDCMapModule* DCMAPAPI CDCMapCore::GetService(const char* Name)
{
	MapModules::iterator it = m_Services.find(Name);
	if(it != m_Services.end())return it->second;

	IDCMapModule* mod = CreateModule("PCoreService",Name);
	if(mod)m_Services[Name] = mod;
	if(!mod)
	{
		MapModules::iterator it = m_Daemons.find(Name);
		if(it != m_Daemons.end())return it->second;
	}
	return mod;	
}

IDCMapLocalStrings* g_pLocalStrings=&g_SysStrings;
IDCMapLocalStringsEx* DCMAPAPI CDCMapCore::LoadSystemLocalStrings(dcmapFCSTR Filename)
{
	g_SysStrings.Load(Filename);
	g_pLocalStrings = &g_SysStrings; 
	return &g_SysStrings;
}

bool ProcessMsgResult(dcmapLRESULT ret,dcmapLRESULT& result,dcmapDWORD Options)
{
	result |= ret;
	if((Options & DCMAP_BRADCAST_STOP_ON_FAIL) && (result & DCMM_FAIL))return false;
	if((Options & DCMAP_BRADCAST_STOP_ON_SUCCESS) && (result & DCMM_SUCCESS))return false;
	if((Options & DCMAP_BRADCAST_STOP_ON_PROCESSED) && (result & DCMM_PROCESSED))return false;
	return true;
}

dcmapLRESULT DCMAPAPI CDCMapCore::BroadcastMessage(int Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapModule* pSender,dcmapDWORD Options)
{
	dcmapLRESULT result=0;

	dcmapLRESULT ret;

	ret = OnMessage(Msg,wParam,lParam,pSender);
	if(!ProcessMsgResult(ret,result,Options))return result;

	_foreach(ObjectsSet,m_Objects,it)
	{
		ret = (*it)->OnMessage(Msg,wParam,lParam,pSender);
		if(!ProcessMsgResult(ret,result,Options))return result;
	}

	if((Options&DCMAP_BRADCAST_CORE_ONLY) == 0)
	{
		_foreach(SetWorkspaces,m_Workspaces,it)
		{
			ret = (*it)->BroadcastMessage(Msg,wParam,lParam,pSender,Options);
			if(!ProcessMsgResult(ret,result,Options))return result;
		}
	}

	return result;
}

bool DCMAPAPI CDCMapCore::LoadScriptModule(dcmapFCSTR szFileName)
{
	return g_ScriptSystem.LoadScriptModuleFromFile(szFileName,this);
}

void DCMAPAPI CDCMapCore::LoadScriptModules(dcmapFCSTR szDir)
{
	std::vector<sysstring> vecFiles;
	sysstring loc = sysstring(szDir)+dcmapFT("*.dcm");
	ListDirectory(loc.c_str(),vecFiles);
	for(int i=0;i<vecFiles.size();i++)
	{
		sysstring str = szDir+vecFiles[i];
		LoadScriptModule(str.c_str());
	}
}

IDCMapVarValue* DCMAPAPI CDCMapCore::CreateVarValue()
{
    return dcmapCreateVarValue();
}

dcmapLRESULT DCMAPAPI CDCMapCore::OnMessage( dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender )
{
	switch(Msg)
	{
	case DCMM_CONFIG_CHANGED:
		LoadConfig();
		break;
	}
	return 0;
}

DCMapSystemOptions const* DCMAPAPI CDCMapCore::GetSystemOptions()
{
	return g_dcmapSys.GetSystemOptions();
}

void DCMAPAPI CDCMapCore::LoadDatabase()
{
	LoadDatabaseFrom(0);
}

void DCMAPAPI CDCMapCore::SaveDatabase()
{
	SaveWithBackup(0,0,0);
}

void DCMAPAPI CDCMapCore::Configure()
{
	ConfigureFromFile(L"DCMap.ini",
		L"cfgdef.ini",
		L"cfgupd.ini");
	ConfigureSystem();
}

IDCMapDatabase* DCMAPAPI CDCMapCore::GetDatabase()
{
	return m_pCurrentDatabase;
}

bool DCMAPAPI CDCMapCore::SetDatabase( IDCMapDatabase* database )
{
	return false;
}

