#pragma once
#include "../dcmapsyslib.h"
#include "ModuleManager.h"

class CDCMapWorkspace :	public ModuleManagerImpl<IDCMapWorkspaceEx>
{
	bool m_bActive;
	bool m_bNeedDataUpdate;
	bool m_bNeedReconfigure;

	IDCMapWorkspaceUI* m_pUI;

public:
	CDCMapWorkspace(IDCMapFrameEx* pFrame,IDCMapCoreEx* pCore);
	~CDCMapWorkspace(void);
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);

protected:
	typedef std::map<std::string,IDCMapModule*> MapModules;
	MapModules m_Services;
public:
	IDCMapModule* DCMAPAPI CreateModule(char const* czClass, char const* czName);
	void DCMAPAPI DestroyModule(IDCMapModule* pObj);
	IDCMapModule* DCMAPAPI GetService(const char* Name);
public:

	IDCMapWorkspaceUI* DCMAPAPI GetUI(){return m_pUI;}
	IDCMapCore* DCMAPAPI GetCore();


	void DCMAPAPI SetConfigFile(dcmapFCSTR Filename);
	void DCMAPAPI ConfigureFromFile(dcmapFCSTR Filename=0);
	void DCMAPAPI SaveConfig(dcmapFCSTR Filename=0);

	void DCMAPAPI SetUI(IDCMapWorkspaceUI* pWrsp)
	{
		m_pUI = pWrsp;
	}

	dcmapLRESULT DCMAPAPI BroadcastMessage(int MSG,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapModule* pSender,dcmapDWORD Options=0);

	IDCMapConfig* DCMAPAPI GetConfig(){return &m_Config;}
	void DCMAPAPI PostInit();

	bool DCMAPAPI SelectModuleByID(char const* czModuleID);
	IDCMapModule* DCMAPAPI Module();
	bool DCMAPAPI Next();

	void DCMAPAPI OnActivate(); 
	void DCMAPAPI OnDeactivate(); 

	void DCMAPAPI OnDataUpdate();
	void DCMAPAPI OnConfigChange();

	IDCMapFrame* DCMAPAPI GetFrame(){return m_pFrame;}
	IDCMapFrameUI* DCMAPAPI GetFrameUI();
	void* DCMAPAPI CastTo( DCMapInterfaceID idInterface );

};
