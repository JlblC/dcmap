#pragma once

#include "../dcmapsyslib.h"
#include <map>
#include <string>
#include <g_gstring.h>
#include <g_hash_map.h>

class CDCLocalStrings;

class CDCMapSys : public IDCMapSysEx
{
	CDCLocalStrings* m_pLocStr;

	struct PluginData
	{
		sysstring strModuleName;
		sysstring strModuleFileName;
		sysstring strConfigFileName;
		sysstring strStringsFileName;
		sysstring strHomeDir;
	};
	std::vector<PluginData> vecPlugins;

	int m_iCurrentPlugin;

	struct ModuleData
	{
		void* pUserData;
		dcmapCreateCallback pCreateCallback;
		gsys::gstring strParam;
		dcmapWCSTR strLocalName; 
		int iPluginIndex;
		IDCMapObject* call(DCMapObjectCreationParams const& params)const{return pCreateCallback(&params);}

		friend bool operator == (ModuleData const& v1,ModuleData const& v2)
		{
			return v1.pCreateCallback == v2.pCreateCallback &&
				   v1.pUserData == v2.pUserData &&
				   v1.strParam == v2.strParam;
		}
	};

	typedef gsys::strimultimap<std::string,ModuleData> ModulesMap;
	typedef gsys::strimap<std::string,std::string> AliasMap;

	struct ModuleClassData
	{
		ModulesMap mapPlugins;
		AliasMap   mapAliases;
	};
	typedef std::map<std::string,ModuleClassData*> ClassesMap;

	ClassesMap m_mapClasses;

	ModulesMap::iterator m_itModuleCurrent;
	ModulesMap::iterator m_itModuleEnd;
	bool m_bSelected;

	bool m_bRegistered;

	DCMapSystemOptions m_SysOptions;

	IDCMapObject* CreateObject(dcmapCSTR szClass,dcmapCSTR szName,ModuleData const& pData);

	IDCMapSysUI* m_pUI;

public:
	CDCMapSys();
	~CDCMapSys();

	void ConfigureSystemOptions(IDCMapConfig* pConfig);
public:
	bool Initialize();
	void DCMAPAPI Destroy(){}; //disbale destroy

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);


	IDCMapScriptSystem* DCMAPAPI GetScriptSystem();

	IDCMapObject* DCMAPAPI OnNewObject(IDCMapObject* pModule);

	void DCMAPAPI RegisterPlugin(char const* czClass, char const* czName,
								dcmapCreateCallback pCreateCallback,
								void* pUserData,char const* czParam=0);

	void DCMAPAPI RegisterAlias(char const* czClass, char const* czAlias,char const* czModule);

	char const* DCMAPAPI GetPluginNameByAlias(char const* czClass, char const* czAlias);

	IDCMapObject* DCMAPAPI CreateObject(char const* czClass, char const* czName);
	void DCMAPAPI DestroyObject(IDCMapObject* pModule);

	bool DCMAPAPI SelectPlugin(char const* czClass,char const* czName);

	bool DCMAPAPI SelectClass(char const* czClass);
	bool DCMAPAPI Next();
	char const* DCMAPAPI GetPluginName();
	char const* DCMAPAPI GetPluginClass();
	char const* DCMAPAPI GetPluginParam();

	IDCMapVarValue* DCMAPAPI CreateVarValue();

	dcmapWCSTR DCMAPAPI GetPluginLocalName();
	DCMapSystemOptions const* DCMAPAPI GetSystemOptions();

	IDCMapDatabaseEx* DCMAPAPI CreateDatabase();

	void StartPluginLoad(dcmapFCSTR szModuleFilename,dcmapFCSTR szModuleName,dcmapFCSTR szHomeDir,bool module);
	void EndPluginLoad();

	void DCMAPAPI SetUI(IDCMapSysUI* pUI)
	{
		m_pUI = pUI;
	}
	IDCMapSysUI*  DCMAPAPI GetUI()
	{
		return m_pUI;
	}

	IDCMapCoreEx* DCMAPAPI CreateCore(dcmapWCSTR szName,dcmapWCSTR szTemplate);
	IDCMapFrameEx* DCMAPAPI CreateFrame(IDCMapCore* pCore,dcmapWCSTR szName,dcmapWCSTR szTemplate);
	IDCMapWorkspaceEx* DCMAPAPI CreateWorkspace(IDCMapFrame* pFrame,IDCMapCore* pCore,dcmapWCSTR szName,dcmapWCSTR szTemplate);


	dcmapCSTR DCMAPAPI  WideToStr(dcmapWCSTR str,int CP=0);
	dcmapWCSTR DCMAPAPI StrToWide(dcmapCSTR str,int CP=0);

};
extern CDCMapSys g_dcmapSys;