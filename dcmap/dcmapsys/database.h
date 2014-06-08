#pragma once
#include "../dcmapsyslib.h"

class CDCMapDatabase: public IDCMapDatabaseEx
{
	IDCMapCore* m_pCore;
	IDCMapSys* m_pSys;
	typedef std::map<std::string,IDCMapDataStorage*> MapStorages;
	MapStorages m_mapStorages;

	int m_iDataUpdateLock;
	bool m_bNeedUpdate;

	void LoadStorageModules(dcmapCSTR Classname);
public:
	CDCMapDatabase();
public:
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	void* DCMAPAPI CastTo( DCMapInterfaceID idInterface );

	bool DCMAPAPI Load( dcmapFCSTR szLocation,dcmapFCSTR szTemplateLocation,int Flags,int LoadFlags);
	bool DCMAPAPI SaveWithBackup(dcmapFCSTR pDir,dcmapFCSTR pBackupDir,int NumBackups,int Flags, int SaveFlags);
	bool DCMAPAPI Save(dcmapFCSTR szLocation,int Flags, int SaveFlags);

	bool DCMAPAPI LoadEmpty();
	IDCMapDataSource* DCMAPAPI GetDataSource(dcmapCSTR pName);
	IDCMapDataStorage* DCMAPAPI GetDataStorage(dcmapCSTR pName);

	bool  DCMAPAPI OnCreate(DCMapObjectCreationParams const* params);
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	dcmapLRESULT DCMAPAPI OnMessage(dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender);
	bool DCMAPAPI DataChanged();

	void DCMAPAPI OnDataUpdate();
	void DCMAPAPI LockDataUpdate();
	void DCMAPAPI UnlockDataUpdate();
	void DCMAPAPI PostInit();

	IDCMapVarValue* DCMAPAPI ListTables();
};
