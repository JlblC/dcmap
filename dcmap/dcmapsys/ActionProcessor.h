#pragma once
#include "../dcmapsys.h"

struct _tagXMLNode;
typedef _tagXMLNode XNode, *LPXNode;

struct CActionProc;

class CActionProcessor : public IDCMapActionProcessor
{
	friend struct CActionProc;
    IDCMapActionsDataStoragePtr Actions;
    IDCMapFleetsDataStoragePtr Fleets;
    IDCMapBuildingsDataStoragePtr Units;
    IDCMapBuildingsDataStoragePtr Garrison;
    IDCMapPlanetDataStoragePtr Planets;
    IDCMapProjectDataStoragePtr Projects;
    IDCMapPlayersDataStoragePtr Players;
	IDCMapActionsRequestedDataStoragePtr ActReq;
	IDCMapScriptSystem* m_pScripts;
	IDCMapScriptModule* m_pScriptModule;

	gsys::wgstring Result;
	IDCMapCore* m_pCore;

	CActionProc* m_Procs[DCMACT_NUMBER];

	bool m_bSendOnline;

	int SendActions(bool Force);

public:
    CActionProcessor(void);
    virtual ~CActionProcessor(void);

    bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	void DCMAPAPI SaveConfig(IDCMapConfig* pConfig);
	void DCMAPAPI LoadConfig(IDCMapConfig* pConfig);

    dcmapDCSTR DCMAPAPI MakeRequest(dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID=0,int ForTurn=0,int* pActionsSent=0);
	int DCMAPAPI ProcessResponse(dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID,dcmapDCSTR rText);
	bool DCMAPAPI DropActions(dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID=0);
	bool DCMAPAPI HaveActions(dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID=0);
	void DCMAPAPI UpdateActions(bool Force);

    void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
public:
	bool GetOnline() const { return m_bSendOnline; }
	void SetOnline(bool val);
public:
	int ScriptHaveActions(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	int ScriptSendOnline(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	int ScriptMakeRequest(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	int ScriptProcessResponse(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	int ScriptDropActions(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);

	int SendPlayerActions(dcmapDBREF const& PlayerRef);
};
