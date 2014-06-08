#include "StdAfx.h"
#include "ActionProcessor.h"
#include <g_alg.h>
#include "xmlutils.h"
#include "utils.h"
#include "ActionProcs.h"

#include "../api/dcmap_script_helper.h"

DCMAP_SCRIPT_REFLECTION(CActionProcessor)
{
	process(L"MakeRequest",&CActionProcessor::ScriptMakeRequest);	
	process(L"ProcessResponse",&CActionProcessor::ScriptProcessResponse);	
	process(L"DropActions",&CActionProcessor::ScriptDropActions);	
	process(L"HaveActions",&CActionProcessor::ScriptHaveActions);	

	DCMAP_SCRIPT_PROPERTY(Online);
	DCMAP_SCRIPT_FUNC(UpdateActions);
	DCMAP_SCRIPT_FUNC(SendPlayerActions);
}
DCMAP_REGISTER(CActionProcessor,"PCoreService","ActionProcessor");


int CActionProcessor::ScriptMakeRequest(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
{
	dcmapDBREF const *pRef = pParams->GetDBRef(0);
	dcmapWCSTR player=0;
	if(!pRef) player = pParams->GetString(0);

	int nSent;
	dcmapWCSTR res = MakeRequest(player,pRef,0,&nSent);
	pResult->AddString(res);
	pResult->AddNumber(nSent);
	return DCMEXEC_SUCCESS;
}

int CActionProcessor::ScriptProcessResponse(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
{
	dcmapDBREF const *pRef = pParams->GetDBRef(0);
	dcmapWCSTR player=0;
	if(!pRef) player = pParams->GetString(0);
	dcmapWCSTR text = pParams->GetString(1);
	pResult->AddNumber(ProcessResponse(player,pRef,text));
	return DCMEXEC_SUCCESS;
}

int CActionProcessor::ScriptDropActions(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
{
	dcmapDBREF const *pRef = pParams->GetDBRef(0);
	dcmapWCSTR player=0;
	if(!pRef) player = pParams->GetString(0);
	DropActions(player,pRef);
	return DCMEXEC_SUCCESS;
}


int CActionProcessor::ScriptHaveActions(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
{
	dcmapDBREF const *pRef = pParams->GetDBRef(0);
	dcmapWCSTR player=0;
	if(!pRef) player = pParams->GetString(0);
	pResult->AddNumber(HaveActions(player,pRef));
	return DCMEXEC_SUCCESS;
}


CActionProcessor::CActionProcessor(void)
{
	m_bSendOnline = false;
	for(int i=0;i<DCMACT_NUMBER;i++)m_Procs[i]=0; 

	m_pScriptModule = 0;
}

void DCMAPAPI CActionProcessor::LoadConfig(IDCMapConfig* pConfig)
{
	DCMapConfigWrapper cfg(pConfig);
	cfg.section("ActionProcessor");
	//bool bSendOnline = false;
	cfg["Online"] >> m_bSendOnline;
	//SetOnline(bSendOnline);
}

void DCMAPAPI CActionProcessor::SaveConfig(IDCMapConfig* pConfig)
{
	DCMapConfigWrapper cfg(pConfig);
	cfg.section("ActionProcessor");
	cfg["Online"] << m_bSendOnline;	
}

CActionProcessor::~CActionProcessor(void)
{
	for(int i=0;i<DCMACT_NUMBER;i++)if(m_Procs[i])delete m_Procs[i]; 
}


bool DCMAPAPI CActionProcessor::Init( IDCMapCore* core,IDCMapWorkspace* workspace )
{
	m_pCore = core;
	if(!Fleets.init(core))return false;
	if(!Units.init(core))return false;
	if(!Planets.init(core))return false;
	if(!Projects.init(core))return false;
	if(!Players.init(core))return false;
	if(!Actions.init(core))return false;
	if(!ActReq.init(core))return false;

	m_pScripts = g_pDCMapSys->GetScriptSystem();

	m_Procs[DCMACT_MOVE_FLEET] = new CActMoveFleet(this);
	m_Procs[DCMACT_ADD_BUILDING_TO_QUEUE] = new CActAddBuildingToQueue(this);
	m_Procs[DCMACT_MOVE_UNIT_TO_FLEET] = new CActMoveUnitToFleet(this);
	m_Procs[DCMACT_CANCEL_JUMP] = new CActCancelJump(this);
	m_Procs[DCMACT_CREATE_NEW_FLEET] = new CActCreateNewFleet(this);
	m_Procs[DCMACT_DEMOLISH_BUILDING] = new CActDemolishBuilding(this);
	m_Procs[DCMACT_CHANGE_FLEET_NAME] = new CActChangeFleetName(this);
	m_Procs[DCMACT_DROP_BUILDING_FROM_QUEUE] = new CActDropBuildingFromQue(this);
	m_Procs[DCMACT_STORE_ACTION] = new CActStoreAction(this);
	m_Procs[DCMACT_CANCEL_ACTION] = new CActCancelAction(this);
	m_Procs[DCMACT_CHANGE_PLANET_NAME] = new CActChangePlanetNameAction(this);
	m_Procs[DCMACT_HIDEFLEET] = new CActHideFleet(this);
	m_Procs[DCMACT_CHANGE_BEHAVIOR] = new CActChnageBehavior(this);
	m_Procs[DCMACT_UNPACK_CONAINER] = new CActUnpackContainer(this);
	m_Procs[DCMACT_LOAD_CARGO] = new CActLoadCargo(this);
	m_Procs[DCMACT_CANCEL_CARGO_LOAD] = new CActCancelCargoLoad(this);

	return true;
}

dcmapDCSTR DCMAPAPI CActionProcessor::MakeRequest(dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID,int ForTurn,int* pActionsSent)
{
	if(pActionsSent)*pActionsSent = 0;
	DCMapScopeBlockUpdates sb(m_pCore);

    bool bSend = false;
    
    dcmapDBREF User;

	if(!pPlayerID || !Players->SelectID(*pPlayerID))
		if(!szPlayer || !szPlayer[0] || !Players->SelectWideName(szPlayer))return L"";


	User = *Players->GetID();

	std::vector<dcmapDBREF> vecActs;
    if(Actions->SelectByParentID(User))
		do{vecActs.push_back(*Actions->GetID());}while(Actions->Next());


	if(vecActs.empty())return L"";

	XDoc doc;
	LPXNode root = doc.AppendChild(L"x-dc-perform");

	int n_actions=0;

	for(int i=0;i<vecActs.size();i++)
	{
		if(!Actions->SelectID(vecActs[i]))continue;
		DCMapActionInfo const* pAct = Actions->GetData();
		if(pAct->type >= DCMACT_NUMBER)continue;	
		if(!m_Procs[pAct->type])continue;

		LPXNode act = root->AppendChild(L"act");

		int send_id = pAct->id.id;

		if(m_Procs[pAct->type]->Req(pAct,act))
		{
			act->AppendAttr(L"name",m_Procs[pAct->type]->name());
			act->AppendAttr(L"id",gsys::to_wstr(send_id).c_str());
			n_actions++;
		}
		else
		{
			root->RemoveChild(act);
			break;
		}

		Actions->Update();
		Actions[DCMDB_ACTION_SEND_ID] = send_id;
		Actions->Post();
	}
	if(pActionsSent)*pActionsSent = n_actions;

	if(n_actions)
	{
		Result = doc.GetXML();
		return Result.c_str();
	}
	return L"";
}

int DCMAPAPI CActionProcessor::ProcessResponse(dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID,dcmapDCSTR rText)
{
	DCMapScopeBlockUpdates sb(m_pCore);

    dcmapDBREF User;
	if(!pPlayerID || !Players->SelectID(*pPlayerID))
		if(!szPlayer || !szPlayer[0] || !Players->SelectWideName(szPlayer))return false;

    XDoc doc;
	doc.Load(rText);

	LPXNode root = doc.GetChild(L"dc");
	if(!root)return false;

	int err = CheckGameXMLErrors(root);
	if(err != GAMEXML_OK)
	{
		return err;
	}

	LPXNode frame = root->GetChild(L"iframe");
	if(!frame)return false;
	
	int cnt = frame->GetChildCount();
	for(int i=0;i<cnt;i++)
	{
		LPXNode act = frame->GetChild(i);
		if(!act || wcsicmp(act->name.c_str(),L"act")!=0)continue;

		int id = xml_cast<int>(act->GetAttrValue(L"id"));
		dcmapWCSTR result = act->GetAttrValue(L"result");
		int return_id = xml_cast<int>(act->GetAttrValue(L"return-id"));

		if(result && wcsicmp(result,L"ok")==0) // process ok
		{
			if(Actions->SelectSendID(id))
			{
				DCMapActionInfo const* pAct = Actions->GetData();
				if(pAct->type < DCMACT_NUMBER)
				{
					if(m_Procs[pAct->type])
					{
						m_Procs[pAct->type]->Responce(pAct,return_id);
					}					
				}
				Actions->Delete();
			}
		}
		else if(result && wcsicmp(result,L"error")==0)
		{
			if(Actions->SelectSendID(id))
			{
				DCMapActionInfo const* pAct = Actions->GetData();
				if(pAct->type < DCMACT_NUMBER)
				{
					if(m_Procs[pAct->type])
					{
						m_Procs[pAct->type]->Error(pAct);
					}					
				}
				Actions->Delete();
			}		
		}
		// process error
	}

/*
	=  XMLNode::parseString(rText,L"<iframe>",&Results);
    if(Results.error != eXMLErrorNone)
    {
        return false; // xml not parsed
    }
*/

    return true;
}

bool DCMAPAPI CActionProcessor::DropActions( dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID/*=0*/ )
{
	DCMapScopeBlockUpdates sb(m_pCore);

	dcmapDBREF User;
	if(!pPlayerID || !Players->SelectID(*pPlayerID))
		if(!szPlayer || !szPlayer[0] || !Players->SelectWideName(szPlayer))return false;

	User = *Players->GetID();

	std::vector<dcmapDBREF> vecActs;
	if(Actions->SelectByParentID(User))
		do{vecActs.push_back(*Actions->GetID());}while(Actions->Next());

	for(int i=0;i<vecActs.size();i++)
	{
		if(!Actions->SelectID(vecActs[i]))continue;
		Actions->Delete();
	}
	return true;
}

bool DCMAPAPI CActionProcessor::HaveActions( dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID/*=0*/ )
{
	DCMapScopeBlockUpdates sb(m_pCore);

	if(!pPlayerID || !Players->SelectID(*pPlayerID))
		if(!szPlayer || !szPlayer[0] || !Players->SelectWideName(szPlayer))return false;

	return Actions->SelectByParentID(*Players->GetID());
}

int CActionProcessor::SendPlayerActions(dcmapDBREF const& PlayerRef)
{
	DCMapScopeBlockUpdates sb(m_pCore);

	DCMapVarValue val,res;
	val->AddDBRef(&PlayerRef,Players);

	if(!m_pScriptModule)
		m_pScriptModule = m_pCore->GetServiceTyped<IDCMapScriptModule>("dcm_sys");

	if(m_pScriptModule)
		if(!m_pScriptModule->CallProc(L"DCXMLSendPlayerOrders",val,res))return 0;

	return res->GetNumber();
}

int CActionProcessor::SendActions(bool Force)
{
	DCMapScopeBlockUpdates sb(m_pCore);

	std::set<dcmapDBREF> DirtyPlayers;

	if(Actions->SelectAll())
	do
	{
		DirtyPlayers.insert(Actions[DCMDB_ACTION_OWNER]);
	}
	while(Actions->Next());

	_foreach(std::set<dcmapDBREF>,DirtyPlayers,it) // 
	{
		SendPlayerActions(*it);
	}
	return 1;
}

void DCMAPAPI CActionProcessor::UpdateActions( bool Force )
{
	if(m_bSendOnline || Force)
	{
		SendActions(Force);
	}
}

void CActionProcessor::SetOnline( bool val )
{
	if(m_bSendOnline == val)return;
	m_bSendOnline = val;

	if(m_bSendOnline)
	{
		SendActions(true);
	}
	else
	{
		
	}
}

void* DCMAPAPI CActionProcessor::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(IDCMapActionProcessor);return 0;
}