// dcmapsys.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../dcmapsyslib.h"
#include "dcmapsystem.h"
#include "DCMapCore.h"
#include "DCMapWorkspace.h"
#include "DCMapFrame.h"
#include "time.h"
#include "LocalStrings.h"
#include <boost/format.hpp>
#include "utils.h"
#include "database.h"
#include "scriptsystem.h"
#include "g_file_util.h"
#include "../api/dcmap_script_helper.h"

DCMAP_SCRIPT_REFLECTION(CDCMapSys)
{
	DCMAP_SCRIPT_FUNC(SelectPlugin);
	DCMAP_SCRIPT_FUNC(SelectClass);
	DCMAP_SCRIPT_FUNC(Next);

	DCMAP_SCRIPT_PROPERTY_R(PluginName);
	DCMAP_SCRIPT_PROPERTY_R(PluginClass);
	DCMAP_SCRIPT_PROPERTY_R(PluginParam);

	DCMAP_SCRIPT_PROPERTY_R(UI);

	DCMAP_SCRIPT_FUNC(GetPluginNameByAlias);
	DCMAP_SCRIPT_FUNC(CreateFrame);
	DCMAP_SCRIPT_FUNC(CreateCore);
	DCMAP_SCRIPT_FUNC(CreateWorkspace);

	// need specify parent interface
	func(L"CreateObject",&IDCMapSysEx::CreateObject);
	DCMAP_SCRIPT_FUNC(DestroyObject);
}

// platform functions
bool DetectWine();
void GetSystemParams(DCMapSystemOptions* sysparams);


void RegisterStandart();

struct Val
{
	char ch[12];
	int i;
	char ch2[8];
	unsigned short sh;
};

template<class T>
int GetType(){return 0;}

template<>
int GetType<dcmapWORD>()
{
	return 1;
}

bool PlatformInit();
void PlatformTerminate();

CDCMapSys g_dcmapSys;
IDCMapSys* g_pDCMapSys = &g_dcmapSys;
bool g_dcmapSystemReady=false;
bool g_dcmapModulesReady=false;

DCMAPSYSEXPORT IDCMapCoreEx* DCMAPLIBAPI dcmapsysCreateCore()
{
	if(!g_dcmapSystemReady)dcmapsysInit();
	if(!g_dcmapModulesReady)dcmapsysInitModules();
	g_dcmapCore.Init();

	return &g_dcmapCore;
}

DCMAPSYSEXPORT IDCMapDatabaseEx* DCMAPLIBAPI dcmapsysCreateDatabase()
{
	if(!g_dcmapSystemReady)dcmapsysInit();
	if(!g_dcmapModulesReady)dcmapsysInitModules();
	return g_dcmapSys.CreateDatabase();
}

DCMAPSYSEXPORT IDCMapSysEx* DCMAPLIBAPI dcmapsysInit(dcmapDWORD ver)
{
	if(ver != DCMAP_API_VERSION)return 0;	
	srand( (unsigned)time( NULL ) );
	if(!PlatformInit())return 0;

	g_dcmapSystemReady=true;

	if(!g_dcmapSys.Initialize())return 0;

	return &g_dcmapSys;
}

DCMAPSYSEXPORT void DCMAPLIBAPI dcmapsysTerminate()
{
	g_dcmapCore.Destroy();
	PlatformTerminate();
}


DCMAPSYSEXPORT IDCMapSysEx* DCMAPLIBAPI GetDCMapSys()
{
	return &g_dcmapSys;
}

void LoadModules(dcmapFCSTR szDir);

DCMAPSYSEXPORT void DCMAPLIBAPI dcmapsysInitModules()
{
	if(!g_dcmapSystemReady)dcmapsysInit();

	g_ScriptSystem.Init();

	// load & register internal modules
	if(g_pDCMapAutoRegisterHelperManager)g_pDCMapAutoRegisterHelperManager->RegisterModules(g_pDCMapSys);
	{
		sysstring Dir =  GetAppStartupDir()+dcmapFT("modules\\");
		LoadModules(Dir.c_str());
	}
	// load & register external modules
	g_dcmapModulesReady = true;
}

DCMAPSYSEXPORT IDCMapLocalStringsEx* DCMAPLIBAPI dcmapsysLocalize(dcmapFCSTR szDir)
{
	if(!szDir)
	{
		sysstring Dir =  GetAppStartupDir()+dcmapFT("dcmap.lng");
		g_SysStrings.Load(Dir.c_str());
	}
	else if(dcmapFileExists(szDir))
	{
		g_SysStrings.Load(szDir);
	}
	else
	{
		sysstring Dir =  GetAppStartupDir()+szDir;
		g_SysStrings.Load(Dir.c_str());
	}
	return &g_SysStrings;
}

void LoadPlugins(dcmapFCSTR szDir);

DCMAPSYSEXPORT void DCMAPLIBAPI dcmapsysLoadPlugins(dcmapFCSTR szDir)
{
	CDCMapConfig PlugConfig;
	if(PlugConfig.Load((sysstring(szDir)+ dcmapFT("plugins.ini")).c_str()))
	{
		LoadPlugins(szDir);

		DCMapConfigWrapper cfg(&PlugConfig);
		cfg.section("AddPlugLoc");
		int num=0;
		cfg["Count"] >> num;
		for(int i=0;i<num;i++)
		{
			sysstring Location;
			cfg[boost::str(boost::format("Location%d")%i).c_str()] >> Location;
			dcmapsysLoadPlugins(Location.c_str());
		}	
	}
	else
	{
		LoadPlugins(szDir);
	}
}

DCMAPSYSEXPORT IDCMapCoreEx* DCMAPLIBAPI GetDCMapCore()
{
	return &g_dcmapCore;
}

CDCMapSys::CDCMapSys():
	m_bSelected(false),
	m_bRegistered(false),
	m_pLocStr(&g_SysStrings),
	m_iCurrentPlugin(0)
{

}

CDCMapSys::~CDCMapSys()
{
	
}

char const* DCMAPAPI CDCMapSys::GetPluginNameByAlias(char const* czClass, char const* czAlias)
{
	ModuleClassData* pDta=0;
	{
		ClassesMap::iterator it = m_mapClasses.find(czClass);
		if(it == m_mapClasses.end())return "";
		pDta = it->second;
	}
	AliasMap::iterator it = pDta->mapAliases.find(czAlias);		
	if(it == pDta->mapAliases.end())return "";
	return it->second.c_str();
}


void DCMAPAPI CDCMapSys::RegisterAlias(char const* czClass, char const* czAlias,char const* czModule)
{
	if(!czClass)czClass="";
	if(!czAlias || !czAlias[0])return;
	if(!czModule || !czModule[0])return;

	m_bSelected = false;
	ModuleClassData* pDta=0;
	{
		ClassesMap::iterator it = m_mapClasses.find(czClass);
		if(it == m_mapClasses.end())
		{
			pDta = new ModuleClassData;
			m_mapClasses[czClass] = pDta;
		}
		else pDta = it->second;
	}
	if(!pDta) return;

	AliasMap::iterator it = pDta->mapAliases.find(czAlias);		
	if(it == pDta->mapAliases.end())
	{
		pDta->mapAliases[czAlias] = czModule;
	}
}

void DCMAPAPI CDCMapSys::RegisterPlugin(char const* czClass, char const* czName,
							 dcmapCreateCallback pCreateCallback,
							 void* pUserData,char const* czParam)
{
	if(m_bRegistered)return;
	if(!pCreateCallback)return;

	if(!czClass)czClass="";
	if(!czName)czName="";

	_ASSERTSTR(czClass);
	_ASSERTSTR(czName);

	m_bSelected = false;
	ModuleClassData* pDta=0;
	{
		ClassesMap::iterator it = m_mapClasses.find(czClass);
		if(it == m_mapClasses.end())
		{
			pDta = new ModuleClassData;
			m_mapClasses[czClass] = pDta;
		}
		else pDta = it->second;
	}
	if(!pDta) return;

	dcmapWCSTR descr = m_pLocStr->GetSectionStringW(((std::string)czClass+"::"+czName).c_str(),"modules");
	if(!descr[0])descr=0;
	ModuleData dta = {pUserData,pCreateCallback,czParam?czParam:"",descr,m_iCurrentPlugin};
	ModulesMap::value_type ins(czName,dta);

	ModulesMap::iterator it = pDta->mapPlugins.find(czName);		
	if(it == pDta->mapPlugins.end())
	{
		pDta->mapPlugins.insert(ins);
	}
	else
	{
		if(!(it->second == dta))
		{
			pDta->mapPlugins.insert(it,ins);
		}
	}
}

IDCMapObject* CDCMapSys::CreateObject(dcmapCSTR szClass,dcmapCSTR szName,ModuleData const& pData)
{
	DCMapObjectCreationParams params;

	_ASSERT(pData.iPluginIndex < vecPlugins.size());
	PluginData & PlugData = vecPlugins[pData.iPluginIndex];

	memset(&params,0,sizeof(params));
	params.cbSize = sizeof(params);
	params.szClass = szClass;
	params.szName = szName;
	params.szParams = pData.strParam.c_str();
	params.pUserData = pData.pUserData;
	params.pDCMapSys = this;
	params.szModuleDir = PlugData.strHomeDir.c_str();

	IDCMapObject* pModule = pData.call(params);
	if(!pModule) return 0;
	_ASSERTOBJ(pModule);

	return pModule;
}

IDCMapObject* DCMAPAPI CDCMapSys::CreateObject(dcmapCSTR czClass, dcmapCSTR czName)
{
	if(!czClass)czClass="";

	_ASSERTSTR(czClass);
	_ASSERTSTR(czName);

	ModuleClassData* pDta=0;
	{
		ClassesMap::iterator it = m_mapClasses.find(czClass);
		if(it == m_mapClasses.end())return 0;
		pDta = it->second;
	}
	if(!pDta) return 0;
	ModulesMap::iterator it = pDta->mapPlugins.find(czName);		
	if(it == pDta->mapPlugins.end())return 0;
	IDCMapObject* pObj = CreateObject(czClass,czName,it->second);
	if(!pObj) return 0;
	return pObj;
}
void DCMAPAPI CDCMapSys::DestroyObject(IDCMapObject* pModule)
{
	if(!pModule)return;
	_ASSERTOBJ(pModule);
	pModule->Destroy();
}

bool DCMAPAPI CDCMapSys::SelectPlugin(char const* czClass,char const* czName)
{
	if(!czClass)czClass="";

	_ASSERTSTR(czClass);
	_ASSERTSTR(czName);

	m_bSelected = false;
	ClassesMap::iterator it = m_mapClasses.find(czClass);
	if(it == m_mapClasses.end())return false;

	m_itModuleCurrent = it->second->mapPlugins.find(czName);
	if(m_itModuleCurrent == it->second->mapPlugins.end())
	{
		czName = GetPluginNameByAlias(czClass,czName);
		m_itModuleCurrent = it->second->mapPlugins.find(czName);
		if(m_itModuleCurrent == it->second->mapPlugins.end())
		{
			return false;
		}
	}
	m_itModuleEnd = m_itModuleCurrent;
	return true;
}

DCMapSystemOptions const* DCMAPAPI CDCMapSys::GetSystemOptions()
{
	return &m_SysOptions;
}

bool DCMAPAPI CDCMapSys::SelectClass(char const* czClass)
{
	if(!czClass)czClass="";

	_ASSERTSTR(czClass);

	m_bSelected = false;

	ClassesMap::iterator it = m_mapClasses.find(czClass);
	if(it == m_mapClasses.end())return false;

	m_itModuleCurrent = it->second->mapPlugins.begin();
	m_itModuleEnd      = it->second->mapPlugins.end();

	m_bSelected = m_itModuleCurrent != m_itModuleEnd;
	return m_bSelected;
}

bool DCMAPAPI CDCMapSys::Next()
{
	if(!m_bSelected)return false;
	++m_itModuleCurrent;
	m_bSelected = m_itModuleCurrent != m_itModuleEnd;
	return m_bSelected;
}
char const* DCMAPAPI CDCMapSys::GetPluginName()
{
	return m_itModuleCurrent->first.c_str();
}
char const* DCMAPAPI CDCMapSys::GetPluginClass()
{
	return 0;
}
char const* DCMAPAPI CDCMapSys::GetPluginParam()
{
	return m_itModuleCurrent->second.strParam.c_str();
}

dcmapWCSTR DCMAPAPI CDCMapSys::GetPluginLocalName()
{
	dcmapWCSTR str = m_itModuleCurrent->second.strLocalName;
	if(str)return str;
	static wstring ststr;
	ststr = ToWide(m_itModuleCurrent->first);
	return ststr.c_str();
}

void CDCMapSys::ConfigureSystemOptions( IDCMapConfig* pConfig )
{
	memset(&m_SysOptions,0,sizeof(m_SysOptions));
	m_SysOptions.cbSize = sizeof(m_SysOptions);
	// defaults

	bool bDetectWine;

	// read params
	DCMapConfigWrapper cfg(pConfig);
	cfg.section("Core");
	cfg["WineWorkaround"] >> m_SysOptions.bWine;
	cfg["DetectWine"] >> bDetectWine;
	cfg["NumPlanets"] >> g_DCMapNumPlanets;

	cfg["DevMode"] >> m_SysOptions.bDevMode;

	// process
	if(g_DCMapNumPlanets > DCMapNumPlanets)g_DCMapNumPlanets = DCMapNumPlanets;
	if(g_DCMapNumPlanets < 2)g_DCMapNumPlanets = 2;

	if(!m_SysOptions.bWine && bDetectWine)
		m_SysOptions.bWine = DetectWine();

	GetSystemParams(&m_SysOptions);

	if(m_SysOptions.bWine)
	{
		m_SysOptions.bNoGdiPlus = true;
		m_SysOptions.bNoSkins = true;
	}
}

IDCMapObject* DCMAPAPI CDCMapSys::OnNewObject( IDCMapObject* pModule )
{
	DCMapObjectCreationParams params;
	memset(&params,0,sizeof(params));
	params.pDCMapSys = this;

	if(!pModule->OnCreate(&params))
	{
		DestroyObject(pModule);
		return 0;
	}
	return pModule;
}

IDCMapDatabaseEx* DCMAPAPI CDCMapSys::CreateDatabase()
{
	IDCMapDatabaseEx* pDB =  (IDCMapDatabaseEx*)OnNewObject(new CDCMapDatabase);

	if(!pDB->Init(0,0))
	{
		DestroyObject(pDB);
		return 0;
	}
	return pDB;
}

bool CDCMapSys::Initialize()
{
	vecPlugins.resize(1);
	PluginData &dta = vecPlugins.front();

	dta.strModuleName = L"dcmap";
	dta.strModuleFileName = gsys::GetAppFilenameW();
	dta.strHomeDir = gsys::GetAppStartupDirW();
	dta.strConfigFileName = dta.strHomeDir + L"dcmap.ini";
	dta.strStringsFileName = dta.strHomeDir + L"dcmap.lng";

	return true;
}

void CDCMapSys::StartPluginLoad(dcmapFCSTR szModuleFilename,dcmapFCSTR szModuleName,dcmapFCSTR szHomeDir,bool module)
{
	PluginData dta;

	dta.strModuleName = szModuleName;
	dta.strModuleFileName = szModuleFilename;
	dta.strHomeDir = szHomeDir;

	sysstring base = gsys::ExtractFilePath(dta.strModuleFileName);

	dta.strConfigFileName = base+dta.strModuleName+L".ini";
	dta.strStringsFileName = base+dta.strModuleName+L".lng";

	vecPlugins.push_back(dta);
	m_iCurrentPlugin = vecPlugins.size()-1;
}

void CDCMapSys::EndPluginLoad()
{
	m_iCurrentPlugin = 0;
}

IDCMapScriptSystem* DCMAPAPI CDCMapSys::GetScriptSystem()
{
	return &g_ScriptSystem;
}

void* DCMAPAPI CDCMapSys::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(CDCMapSys);
	DCMAP_MODULE_SUPPORTS(IDCMapSysEx);
	DCMAP_MODULE_SUPPORTS(IDCMapObject);

	{if(DCMAP_INTERFACE_CMP(idInterface,IDCMapScriptSystem))return static_cast<IDCMapScriptSystem*>(&g_ScriptSystem);};
	return 0;
}

IDCMapVarValue* DCMAPAPI CDCMapSys::CreateVarValue()
{
	return g_ScriptSystem.CreateVarValue();
}

dcmapCSTR DCMAPAPI  CDCMapSys::WideToStr(dcmapWCSTR str,int CP)
{
	return dcmapWideToStr(str,CP);
}

dcmapWCSTR DCMAPAPI CDCMapSys::StrToWide(dcmapCSTR str,int CP)
{
	return dcmapStrToWide(str,CP);
}

IDCMapCoreEx* DCMAPAPI CDCMapSys::CreateCore(dcmapWCSTR szName,dcmapWCSTR szTemplate)
{
	return dcmapsysCreateCore();
}

IDCMapFrameEx* DCMAPAPI CDCMapSys::CreateFrame( IDCMapCore* pCore,dcmapWCSTR szName,dcmapWCSTR szTemplate )
{
	CDCMapFrame* pFrame = new CDCMapFrame();

	if(m_pUI)
	{
		IDCMapFrameUI* pUI = m_pUI->CreateFrameUI(pFrame,pCore);	
		pFrame->SetUI(pUI);
	}
	return pFrame;
}

IDCMapWorkspaceEx* DCMAPAPI CDCMapSys::CreateWorkspace( IDCMapFrame* pFrame,IDCMapCore* pCore,dcmapWCSTR szName,dcmapWCSTR szTemplate )
{
	CDCMapWorkspace* pWorkspace = new CDCMapWorkspace(dcmap_cast<IDCMapFrameEx*>(pFrame),dcmap_cast<IDCMapCoreEx*>(pCore));

	wstring tpl,config;

	bool NewSession=false;
	if(!szTemplate || !szTemplate[0])
	{
		if(!szName || !szName[0])
		{
			tpl = L"default";
			NewSession=true;
		}
	}
	else
	{
		tpl = szTemplate;
		NewSession=true;
	}

	if(!szName || !szName[0])
	{
		config = L"worksp";
	}
	else config = szName;

	config = L"Sessions\\"+config+L".ini";
	tpl = L"Templates\\"+tpl+L".ini";

	if(NewSession)
	{
		pWorkspace->SetConfigFile(tpl.c_str());
		pWorkspace->ConfigureFromFile();
		pWorkspace->SetConfigFile(config.c_str());
	}
	else
	{
		pWorkspace->SetConfigFile(config.c_str());
		pWorkspace->ConfigureFromFile();
	}

	if(m_pUI)
	{
		IDCMapWorkspaceUI* pUI = m_pUI->CreateWorkspaceUI(pWorkspace,pFrame,pCore);	
		if(pUI)pWorkspace->SetUI(pUI);
	}
	return pWorkspace;

}