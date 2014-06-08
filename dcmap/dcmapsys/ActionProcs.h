#pragma once
#include "../dcmapsys.h"

struct CActionProc
{
	IDCMapActionsDataStoragePtr Actions;
	IDCMapFleetsDataStoragePtr Fleets;
	IDCMapBuildingsDataStoragePtr Units;
	IDCMapBuildingsDataStoragePtr Garrison;
	IDCMapPlanetDataStoragePtr Planets;
	IDCMapProjectDataStoragePtr Projects;
	IDCMapPlayersDataStoragePtr Players;
	IDCMapActionsRequestedDataStoragePtr ActReq;
	CActionProcessor* proc;
	CActionProc(CActionProcessor* p):proc(p),
		Actions(p->Actions),
		Fleets(p->Fleets),
		Units(p->Units),
		Garrison(p->Garrison),
		Planets(p->Planets),
		Projects(p->Projects),
		Players(p->Players),
		ActReq(p->ActReq){}

	virtual ~CActionProc(){}

	virtual dcmapWCSTR name()=0;
	virtual bool Req(DCMapActionInfo const* pAct,LPXNode node){return false;};
	virtual bool Responce(DCMapActionInfo const* pAct,int return_id){return false;};
	virtual void Error(DCMapActionInfo const* pAct){};

	virtual bool Execute(DCMapActionInfo const* pAct){return false;};
	virtual bool Undo(DCMapActionInfo const* pAct){return false;};
};

struct CActMoveUnitToFleet : public CActionProc
{	//GET /planet/?planetid=670:505&action=move_unit_to_fleet&fleet_id=0&unit_id=29556654 HTTP/1.1\r\n
	CActMoveUnitToFleet(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"move_unit_to_fleet";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Fleets->SelectID(pAct->fleet))return false;
		if(!Units->SelectID(pAct->building))return false;
		int gid = Fleets[DCMDB_FLEET_GAMEID];
		if(gid==0)
		{
			if(!Fleets[DCMDB_FLEET_GARRISON])return false;
		}
		int ugid = Units[DCMDB_BUILDING_GAMEID];
		if(ugid==0)return false;

		node->AppendChild(L"planetid",(gsys::to_wstr(pAct->x)+L":"+gsys::to_wstr(pAct->y)).c_str());  
		node->AppendChild(L"fleet_id",ToWStr(gid).c_str()); 
		node->AppendChild(L"unit_id",ToWStr(ugid).c_str());
		return true;
	};
};


struct CActMoveFleet : public CActionProc
{
	CActMoveFleet(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"move_fleet";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Fleets->SelectID(pAct->fleet))return false;
		int gid = Fleets[DCMDB_FLEET_GAMEID];
		if(gid==0)return false;

		node->AppendChild(L"fleet_id",(ToWStr(gid)).c_str()); 
		node->AppendChild(L"move_to",(ToWStr(pAct->x)+L":"+ToWStr(pAct->y)).c_str());  
		return true;
	};
};

//http://www.the-game.ru/planet/?planetid=653:523&action=cancel_jump&fleet_id=1012201
struct CActCancelJump : public CActionProc
{
	CActCancelJump(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"cancel_jump";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Fleets->SelectID(pAct->fleet))return false;
		int gid = Fleets[DCMDB_FLEET_GAMEID];
		if(gid==0)return false;
		node->AppendChild(L"fleet_id",(gsys::to_wstr(gid)).c_str()); 
		return true;
	};
};
//GET /frames/planet_fleets/on/planet/?planetid=670:505&action=create_new_fleet&new_fleet_name=Fleet555 HTTP/1.1\r\n
struct CActCreateNewFleet : public CActionProc
{
	CActCreateNewFleet(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"create_new_fleet";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Fleets->SelectID(pAct->fleet))return false;

		node->AppendChild(L"planetid",(gsys::to_wstr(pAct->x)+L":"+gsys::to_wstr(pAct->y)).c_str());  
		node->AppendChild(L"new_fleet_name",pAct->new_name);  

		return true;
	};
	bool Responce(DCMapActionInfo const* pAct,int return_id)
	{
		if(Fleets->SelectID(pAct->fleet))
		{
			Fleets->Update();
			Fleets[DCMDB_FLEET_GAMEID] = return_id;
			Fleets->Post();
		}
		return true;
	};
};

struct CActChangeFleetName : public CActionProc
{
	CActChangeFleetName(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"change_fleet_name";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Fleets->SelectID(pAct->fleet))return false;
		int gid = Fleets[DCMDB_FLEET_GAMEID];
		if(gid==0)return false;
		node->AppendChild(L"fleet_id",(gsys::to_wstr(gid)).c_str()); 

		node->AppendChild(L"planetid",(gsys::to_wstr(pAct->x)+L":"+gsys::to_wstr(pAct->y)).c_str());  
		node->AppendChild(L"name",pAct->new_name);  
		node->AppendChild(L"public_name",pAct->new_public_name);  

		return true;
	};
};

//GET /frames/planet_buildings/on/planet/?planetid=654:523&action=drop_building_from_que&building_id=38155838 HTTP/1.1\r\n
struct CActDropBuildingFromQue : public CActionProc
{
	CActDropBuildingFromQue(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"drop_building_from_que";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Units->SelectID(pAct->building))return false;
		int gid = Units[DCMDB_BUILDING_GAMEID];
		if(gid==0)return false;

		node->AppendChild(L"planetid",(gsys::to_wstr(pAct->x)+L":"+gsys::to_wstr(pAct->y)).c_str());  
		node->AppendChild(L"building_id",(gsys::to_wstr(gid)).c_str());  

		return true;
	};
};

struct CActDemolishBuilding : public CActionProc
{
	CActDemolishBuilding(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"demolish_building";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Units->SelectID(pAct->building))return false;
		int gid = Units[DCMDB_BUILDING_GAMEID];
		if(gid==0)return false;

		node->AppendChild(L"planetid",(gsys::to_wstr(pAct->x)+L":"+gsys::to_wstr(pAct->y)).c_str());  
		node->AppendChild(L"unit_id",(gsys::to_wstr(gid)).c_str());  

		return true;
	};
};

struct CActAddBuildingToQueue : public CActionProc
{
	CActAddBuildingToQueue(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"add_building_to_que";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Planets->SelectID(pAct->planet))return false;
		if(!Projects->SelectID(pAct->project))return false;

		node->AppendChild(L"planetid",Planets->GetWideName()); 
		node->AppendChild(L"building_id",Projects[DCMDB_PROJECT_GAMEID]); 
		return true;
	};
	bool Responce(DCMapActionInfo const* pAct,int return_id)
	{
		if(Units->SelectID(pAct->building))
		{
			Units->Update();
			Units[DCMDB_BUILDING_GAMEID] = return_id;
			Units->Post();
		}
		return true;
	};
};

// DCMACT_STORE_ACTION
struct CActStoreAction : public CActionProc
{
	CActStoreAction(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"store_action";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Units->SelectID(pAct->building))return false;
		if(!Units[DCMDB_BUILDING_GAMEID])return false;
		if(!ActReq->SelectID(pAct->actreq))return false;

		node->AppendChild(L"unit_id",Units[DCMDB_BUILDING_GAMEID]); 
		node->AppendChild(L"action_id",ActReq[DCMDB_ACTREQ_ACTID]); 
		return true;
	};
	bool Responce(DCMapActionInfo const* pAct,int return_id)
	{
		if(ActReq->SelectID(pAct->actreq))
		{
			ActReq->Update();
			ActReq[DCMDB_ACTREQ_GAMEID] = return_id;
			ActReq->Post();
		}
		return true;
	};
};

// DCMACT_CANCEL_ACTION
struct CActCancelAction : public CActionProc
{
	CActCancelAction(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"cancel_action";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Units->SelectID(pAct->building))return false;
		if(!ActReq->SelectID(pAct->actreq))return false;

		int gid = ActReq[DCMDB_ACTREQ_GAMEID];
		if(gid==0)return false;

		node->AppendChild(L"action_id",(gsys::to_wstr(gid)).c_str());
		return true;
	};
};

// GET /frames/planetinfo/on/planet/?action=change_planet_name&planetid=670%3A505&newname=Dark HTTP/1.1\r\n
// DCMACT_CHANGE_PLANET_NAME
struct CActChangePlanetNameAction : public CActionProc
{
	CActChangePlanetNameAction(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"change_planet_name";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Planets->SelectID(pAct->planet))return false;
		node->AppendChild(L"planetid",Planets->GetWideName()); 
		node->AppendChild(L"newname",pAct->new_name); 
		return true;
	};
};

// DCMACT_HIDEFLEET
struct CActHideFleet : public CActionProc
{
	CActHideFleet(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"hidefleet";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Fleets->SelectID(pAct->fleet))return false;
		if(!Fleets[DCMDB_FLEET_GAMEID])return false;

		switch(pAct->mode)
		{
		case DCMAP_HIDEFM_HIDE:
			node->AppendChild(L"mode",L"hide_fleet"); 
			break;
		case DCMAP_HIDEFM_CANCEL_HIDE:
			node->AppendChild(L"mode",L"cancelhide_fleet"); 
			break;
		case DCMAP_HIDEFM_UNHIDE:
			node->AppendChild(L"mode",L"unhide_fleet"); 
			break;
		case DCMAP_HIDEFM_CANCEL_UNHIDE:
			node->AppendChild(L"mode",L"cancelunhide_fleet"); 
			break;
		default:
			return false;
		}
		node->AppendChild(L"fleet_id",Fleets[DCMDB_FLEET_GAMEID]); 
		return true;
	};
	bool Responce(DCMapActionInfo const* pAct,int return_id)
	{

		return true;
	};
};

//DCMACT_CHANGE_BEHAVIOR
//GET /planet/?planetid=670%3A505&action=change_behaviour&fleet_id=278428&behavtrgt_-1=3&behavtrgt_0=1&behavtrgt_1=2&behavtrgt_2=1&behavtrgt_3=2&behavtrgt_4=4 HTTP/1.1\r\n
struct CActChnageBehavior : public CActionProc
{
	CActChnageBehavior(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"change_behaviour";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Fleets->SelectID(pAct->fleet))return false;
		if(!(int)Fleets[DCMDB_FLEET_GAMEID])return false;

		node->AppendChild(L"fleet_id",Fleets[DCMDB_FLEET_GAMEID]); 

		dcmapDWORD beh = pAct->mode;
		for(int i=5;--i>=-1;)
		{
			int mode = beh & 0x0f;
			beh = beh >> 4;
			node->AppendChild((L"behavtrgt_"+gsys::to_wstr(i)).c_str(),(gsys::to_wstr(mode)).c_str()); 
		}
		return true;
	};
	bool Responce(DCMapActionInfo const* pAct,int return_id)
	{

		return true;
	};
};

//GET /planet/?planetid=670:514&action=unpack_container&building_id=31110166 HTTP/1.1\r\n
struct CActUnpackContainer : public CActionProc
{
	CActUnpackContainer(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"unpack_container";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Units->SelectID(pAct->building))return false;
		if(!(int)Units[DCMDB_BUILDING_GAMEID])return false;

		node->AppendChild(L"building_id",Units[DCMDB_BUILDING_GAMEID]); 
		return true;
	};
	bool Responce(DCMapActionInfo const* pAct,int return_id)
	{
		return true;
	};
};

// GET /planet/?planetid=670:514&action=move_unit_to_fleet&fleet_id=allien_1683099&unit_id=30912695 HTTP/1.1\r\n
struct CActLoadCargo : public CActionProc
{
	CActLoadCargo(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"move_unit_to_fleet";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Fleets->SelectID(pAct->fleet))return false;
		if(!Units->SelectID(pAct->building))return false;
		int gid = Fleets[DCMDB_FLEET_GAMEID];
		if(gid==0)
		{
			if(!Fleets[DCMDB_FLEET_GARRISON])return false;
		}
		int ugid = Units[DCMDB_BUILDING_GAMEID];
		if(ugid==0)return false;

		node->AppendChild(L"planetid",(gsys::to_wstr(pAct->x)+L":"+gsys::to_wstr(pAct->y)).c_str());  
		node->AppendChild(L"fleet_id",(L"allien_"+ToWStr(gid)).c_str()); 
		//node->AppendChild(L"fleet_id",(ToWStr(gid)).c_str()); 
		node->AppendChild(L"unit_id",ToWStr(ugid).c_str());
		return true;
	};
};

// GET /frames/planet_fleets/on/planet/?planetid=670:514&action=cancel_cargo_load&building_id=30912695 HTTP/1.1\r\n
struct CActCancelCargoLoad : public CActionProc
{
	CActCancelCargoLoad(CActionProcessor* p):CActionProc(p){}
	dcmapWCSTR name(){return L"cancel_cargo_load";}
	bool Req(DCMapActionInfo const* pAct,LPXNode node)
	{
		if(!Units->SelectID(pAct->building))return false;
		if(!(int)Units[DCMDB_BUILDING_GAMEID])return false;

		node->AppendChild(L"building_id",Units[DCMDB_BUILDING_GAMEID]); 
		return true;
	};
	bool Responce(DCMapActionInfo const* pAct,int return_id)
	{
		return true;
	};
};