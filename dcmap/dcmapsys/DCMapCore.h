#pragma once

#include "../dcmapsyslib.h"
#include "ModuleManager.h"


class CDCMapWorkspace;
class CDCMapDatabase;


class CDCMapCore :	public ModuleManagerImpl<IDCMapCoreEx>
{
	IDCMapCoreUIEx* m_pCoreUI;

	typedef std::set<CDCMapWorkspace*> SetWorkspaces;
	SetWorkspaces m_Workspaces;

	void AddDataStorage(dcmapCSTR name,IDCMapDataStorage* pStor);
	void LoadCoreModules(dcmapCSTR PClass);
	void LoadCoreServices(char const* PClass);

	int m_iDataUpdateLock;
	bool m_bNeedUpdate;

	bool m_bUseHardlinks;
	bool m_bCompressDatabase;

	bool m_bInitialized;

	void LoadConfig();

	IDCMapWorkspace* m_pCurrentWorkspace;
	CDCMapDatabase* m_pCurrentDatabase;
protected:
	bool m_bUsebackup;
	int  m_iNumBackups;
public:
	IDCMapModule* DCMAPAPI GetService(dcmapCSTR Name);
public:
	CDCMapCore(void);
	~CDCMapCore(void);
public:
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);

	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	IDCMapModule* DCMAPAPI AddModule(IDCMapModule* pObj);
	void DCMAPAPI LoadScriptModules(dcmapFCSTR szDir);
	bool DCMAPAPI LoadScriptModule(dcmapFCSTR szFileName);

	IDCMapDataSource* DCMAPAPI GetDataSource(dcmapCSTR pName);

	void DCMAPAPI LoadDatabaseFrom(dcmapFCSTR pDir);
	void DCMAPAPI SaveDatabaseTo(dcmapFCSTR pDir);

	void DCMAPAPI LoadDatabase();
	void DCMAPAPI SaveDatabase();

	IDCMapDatabase* DCMAPAPI GetDatabase();;
	bool DCMAPAPI SetDatabase(IDCMapDatabase* database);

	void Init();

	IDCMapWorkspaceEx* DCMAPAPI CreateWorkspace();
	void DCMAPAPI DestroyWorkspace(IDCMapWorkspaceEx* pWrsp);

	void DCMAPAPI OnDataUpdate();
	void DCMAPAPI LockDataUpdate();
	void DCMAPAPI UnlockDataUpdate();
    void DCMAPAPI ForceDataUpdate();

	void DCMAPAPI OnConfigChange();

	IDCMapConfig* DCMAPAPI GetConfig(){return &m_Config;}

    void DCMAPAPI ConfigureFromFile(dcmapFCSTR szFilename,
                                    dcmapFCSTR szDefaultsFilename,
                                    dcmapFCSTR szUpdateFilename);

	void DCMAPAPI Configure();

	void DCMAPAPI ConfigureSystem();

	void DCMAPAPI SaveConfig(dcmapFCSTR Filename=0);

	void DCMAPAPI SetCoreUI(IDCMapCoreUIEx* pCoreUI){m_pCoreUI = pCoreUI;}

	IDCMapCoreUI* DCMAPAPI GetCoreUI(){return m_pCoreUI;}
	IDCMapCoreUI* DCMAPAPI GetUI(){return m_pCoreUI;}


	IDCMapModule* DCMAPAPI CreateModule(dcmapCSTR czClass, dcmapCSTR czName);
	void DCMAPAPI DestroyModule(IDCMapModule* pObj);
	void DCMAPAPI DestroyWorkspaceModule(IDCMapModule* pObj);

	bool DCMAPAPI SelectModuleByID(dcmapCSTR czModuleID){return false;};
	IDCMapModule* DCMAPAPI Module(){return 0;};
	bool DCMAPAPI Next(){return false;};
	virtual void DCMAPAPI PostInit();

	void DCMAPAPI Destroy();
	void DCMAPAPI SaveWithBackup(dcmapFCSTR pDir,dcmapFCSTR pBackupDir,int NumBackups);

	IDCMapLocalStringsEx* DCMAPAPI LoadSystemLocalStrings(dcmapFCSTR Filename);

	void DCMAPAPI SetCurrentWorkspace(IDCMapWorkspace* wsp)
	{
		m_pCurrentWorkspace = wsp;
	}
	IDCMapWorkspace* DCMAPAPI GetCurrentWorkspace()
	{
		return m_pCurrentWorkspace;
	}

	dcmapLRESULT DCMAPAPI BroadcastMessage(int MSG,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapModule* pSender=0,dcmapDWORD Options=0);

	IDCMapConsole* DCMAPAPI GetConsole();

    IDCMapVarValue* DCMAPAPI CreateVarValue();

	dcmapLRESULT DCMAPAPI OnMessage(dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender);
	DCMapSystemOptions const* DCMAPAPI GetSystemOptions();
	void DCMAPAPI InitModules();
};
extern CDCMapCore g_dcmapCore;
