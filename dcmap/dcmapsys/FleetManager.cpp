#include "StdAfx.h"
#include "FleetManager.h"
#include "../api/dcmap_script_helper.h"

DCMAP_REGISTER(CFleetManager,"PCoreService","FleetManager");
DCMAP_REGISTER(CFleetManager,"PService","FleetManager");

DCMAP_SCRIPT_REFLECTION(CFleetManager)
{
	DCMAP_SCRIPT_FUNC(MoveUnitToFleet);
	DCMAP_SCRIPT_FUNC(CanMoveUnitToFleet);

	DCMAP_SCRIPT_FUNC(MoveFleetTo);
	DCMAP_SCRIPT_FUNC(CanMoveFleetTo);
	DCMAP_SCRIPT_FUNC(CancelJump);
	DCMAP_SCRIPT_FUNC(CanCancelJump);

	DCMAP_SCRIPT_FUNC(CalculateFleet);
	DCMAP_SCRIPT_FUNC(DisbandUnit);
	DCMAP_SCRIPT_FUNC(DeleteFleet);

	DCMAP_SCRIPT_FUNC(UpdateFleet);

	DCMAP_SCRIPT_FUNC(RenameFleet);
	DCMAP_SCRIPT_FUNC(CanDisbandUnit);
	DCMAP_SCRIPT_FUNC(AddUnitToQueue);

	DCMAP_SCRIPT_FUNC(SetFleetIcon);
	DCMAP_SCRIPT_FUNC(CanExecuteUnitAction);
	DCMAP_SCRIPT_FUNC(ExecuteUnitAction);
	DCMAP_SCRIPT_FUNC(CancelUnitAction);

	DCMAP_SCRIPT_FUNC(UnpackContainer);
	DCMAP_SCRIPT_FUNC(CanUnpackContainer);

	DCMAP_SCRIPT_FUNC(CanHideFleet);
	DCMAP_SCRIPT_FUNC(HideFleet);

	DCMAP_SCRIPT_FUNC(ChangeBehavior);

	DCMAP_SCRIPT_FUNC(BeginUpdate);
	DCMAP_SCRIPT_FUNC(EndUpdate);

	DCMAP_SCRIPT_FUNC(UpdateFleetGroups);

	DCMAP_SCRIPT_FUNC(CanLoadCargo);
	DCMAP_SCRIPT_FUNC(LoadCargo);
	DCMAP_SCRIPT_FUNC(CanCancelCargoLoad);
	DCMAP_SCRIPT_FUNC(CancelCargoLoad);

	DCMAP_SCRIPT_FUNC(GetGarrison);
	DCMAP_SCRIPT_FUNC(GetBuildings);
}

CFleetManager::CFleetManager(void)
{
	m_iLockUpdate = 0;
	m_bNeedUpdateActions = false;
}

CFleetManager::~CFleetManager(void)
{
}

bool DCMAPAPI CFleetManager::Init( IDCMapCore* core,IDCMapWorkspace* workspace )
{
    m_pCore = core;
	m_pWorkspace = workspace;

	if(!Fleets.init(core))return false;
	if(!Units.init(core))return false;
	if(!Planets.init(core))return false;
	if(!Projects.init(core))return false;
	if(!Players.init(core))return false;
	if(!Actions.init(core))return false;
	if(!Groups.init(core))return false;
	if(!PlanetGroups.init(core))return false;
	if(!Components.init(core))return false;
	if(!ProjectActs.init(core))return false;
	if(!PlayerGroups.init(core))return false;
	if(!ActReq.init(core))return false;
	if(!UnitTags.init(core))return false;

	FleetGrouping = core->GetServiceTyped<IDCMapFleetGrouping>("FleetGrouping");
	if(!FleetGrouping)return false;
	ActionProcessor = core->GetServiceTyped<IDCMapActionProcessor>("ActionProcessor");
	if(!ActionProcessor)return false;

	PlanetCalculator = core->GetServiceTyped<IDCMapPlanetCalculator>("PlanetCalculator");
	if(!PlanetCalculator)return false;

	IDCMapPlanetCalculator* PlanetCalculator;


	return true;
}

bool DCMAPAPI CFleetManager::CanCancelJump(dcmapDBREF const& Fleet)
{
    if(!Fleets->SelectID(Fleet))return false;
    DCMapFleetInfo const* pFInfo = Fleets->GetData();
    if(pFInfo->flags & DCMAP_FF_InTransit)return false;
    if(!pFInfo->tta)return false;

	return true; // space warp spoil this calculations 

    int dx = pFInfo->x - pFInfo->from_x;
    int dy = pFInfo->y - pFInfo->from_y;
    dcmapREAL dist = sqrt((dcmapREAL)dx*dx+dy*dy);
    int tta = ceil(dist/pFInfo->fly_speed);

    return tta == pFInfo->tta;
}

bool DCMAPAPI CFleetManager::CanMoveFleetTo(dcmapDBREF const& Fleet,int x,int y)
{
    if(!Fleets->SelectID(Fleet))return false;
    DCMapFleetInfo const* pFInfo = Fleets->GetData();
    if(pFInfo->flags & DCMAP_FF_InTransit)return false;
    if((pFInfo->tta))return false;
	// get fleet open group
	if(!Players->SelectID(pFInfo->parent))return false;
	dcmapDBREF Opens = Players[DCMDB_PLAYER_OPEN_PLANETS];
	if(!Planets->SelectCoord(pFInfo->x,pFInfo->y))return false;
	dcmapDBREF Planet = *Planets->GetID();
	bool JumpFromOpen = PlanetGroups->SelectConnect(Opens,Planet);

	if(!JumpFromOpen)
	{
		if(!Planets->SelectCoord(x,y))return false;
		dcmapDBREF TargetPlanet = *Planets->GetID();
		if(!PlanetGroups->SelectConnect(Opens,TargetPlanet))return false;
	}

    int dx = pFInfo->x - x;
    int dy = pFInfo->y - y;
    dcmapREAL dist = sqrt((dcmapREAL)dx*dx+dy*dy);

    return dist <= pFInfo->fly_range;
}

bool DCMAPAPI CFleetManager::MoveFleetTo( dcmapDBREF const& Fleet,int x,int y )
{
    if(!CanMoveFleetTo(Fleet,x,y))return false;

    DCMapScopeBlockUpdates bupd(m_pCore);

	if(!Fleets->SelectID(Fleet))return false;
    DCMapFleetInfo FInfo = *Fleets->GetData();

    DCMapActionInfo ActInfo;
    Actions->FillDefaults(&ActInfo);

    ActInfo.fleet = Fleet;
    ActInfo.parent = FInfo.ruler;
    
    ActInfo.from_x = FInfo.x;
    ActInfo.from_y = FInfo.y;

    ActInfo.x = x;
    ActInfo.y = y;

    ActInfo.type = DCMACT_MOVE_FLEET;
    
    FInfo.from_x = FInfo.x;
    FInfo.from_y = FInfo.y;

    FInfo.x = x;
    FInfo.y = y;

    int dx = FInfo.x - FInfo.from_x;
    int dy = FInfo.y - FInfo.from_y;

    dcmapREAL dist = sqrt((dcmapREAL)dx*dx+dy*dy);
    FInfo.tta = ceil(dist/FInfo.fly_speed);

    Fleets->Update();
    Fleets->SetData(&FInfo);
    Fleets->Post(); 

    Actions->Insert();
    Actions->SetData(&ActInfo);
    Actions->Post(); 

	UpdateFleetGroups(Fleet);
    
	UpdateActions();
    return true;
}

bool DCMAPAPI CFleetManager::CancelJump(dcmapDBREF const& Fleet)
{
    if(!CanCancelJump(Fleet))return false;

    DCMapScopeBlockUpdates bupd(m_pCore);

    if(!Fleets->SelectID(Fleet))return false;
    DCMapFleetInfo FInfo = *Fleets->GetData();

    DCMapActionInfo ActInfo;
    Actions->FillDefaults(&ActInfo);

    ActInfo.fleet = Fleet;
    ActInfo.parent = FInfo.ruler;

    ActInfo.from_x = FInfo.from_x;
    ActInfo.from_y = FInfo.from_y;

    ActInfo.x = FInfo.from_x;
    ActInfo.y = FInfo.from_y;

    ActInfo.type = DCMACT_CANCEL_JUMP;

    FInfo.x = FInfo.from_x;
    FInfo.y = FInfo.from_y;

    FInfo.tta = 0;

    Fleets->Update();
    Fleets->SetData(&FInfo);
    Fleets->Post(); 

    Actions->Insert();
    Actions->SetData(&ActInfo);
    Actions->Post(); 

	UpdateFleetGroups(Fleet);

	UpdateActions();
    return true;
}

bool DCMAPAPI CFleetManager::CreateFleetAt(int x,int y,dcmapDBREF const& Owner,dcmapWCSTR name,dcmapDBREF* pResult)
{
	DCMapScopeBlockUpdates bupd(m_pCore);
	if(!Players->SelectID(Owner))return false;

	Fleets->Insert();

	Fleets[DCMDB_FLEET_X] = x;
	Fleets[DCMDB_FLEET_Y] = y;

	Fleets[DCMDB_FLEET_CAPTION] = name;
	Fleets[DCMDB_FLEET_NAME] = name;
	Fleets[DCMDB_FLEET_OWNER]  = Owner;
	Fleets[DCMDB_FLEET_RULER]  = Owner;
	Fleets[DCMDB_FLEET_LOADER] = Owner;
	Fleets[DCMDB_FLEET_TYPE] = DCMAP_FTP_Fleet;
	Fleets[DCMDB_FLEET_DIP] = DCMAP_DIP_MULT;
	Fleets[DCMDB_FLEET_TURN] = (int)Players[DCMDB_PLAYER_TURN];
	Fleets[DCMDB_FLEET_FOREIGN] = false;

	dcmapDBREF Fleet;

	if(!Fleets->Post(&Fleet))return false;

	DCMapActionInfo ActInfo;
	Actions->FillDefaults(&ActInfo);

	ActInfo.fleet = Fleet;
	ActInfo.parent = Owner;
	ActInfo.x = x;
	ActInfo.y = y;
	ActInfo.type = DCMACT_CREATE_NEW_FLEET;

	Actions->Insert();
	Actions->SetData(&ActInfo);
	Actions[DCMDB_ACTION_NEW_NAME] = name;
	Actions->Post(); 

	UpdateFleetGroups(Fleet);

	if(pResult)*pResult = Fleet;

	UpdateActions();
	return true;
}

bool DCMAPAPI CFleetManager::UpdateFleetGroups(dcmapDBREF const& Fleet)
{
	if(m_iLockUpdate)
	{
		m_setFleetsToGroup.insert(Fleet);
		return true;
	}
	return FleetGrouping->UpdateFleetGroups(Fleet);
}

bool DCMAPAPI CFleetManager::CalculateFleet(dcmapDBREF const& Fleet)
{
	if(m_iLockUpdate)
	{
		m_setFleetsToCalculate.insert(Fleet);
		return true;
	}

	if(!Fleets->SelectID(Fleet))return false;
	if((int)Fleets[DCMDB_FLEET_TYPE] == DCMAP_FTP_Queue)
	{
		return PlanetCalculator->CalculateQueue(Fleet,0,0);
	}

	// calculate fly range
	float fSpeed=std::numeric_limits<float>::infinity();
	float fRange=std::numeric_limits<float>::infinity();

	int ship_count = 0;
	int cargo_count = 0;
	int cargo_cells = 0;
	dcmapREAL weight=0;


	if(Units->SelectByParentID(Fleet))
	{
		do
		{
			if(Units[DCMDB_BUILDING_OUTDATED])continue;
			if(Units[DCMDB_BUILDING_DISBANDED])continue;

			if(Projects->SelectID(Units[DCMDB_BUILDING_PROJECT]))
			{
				if(Projects[DCMDB_PROJECT_IS_TRANSPORTABLE])
				{
					cargo_count++;
					continue; // no influence from transportable
				}
				cargo_cells += (int)Projects[DCMDB_PROJECT_TRANSPORT_CAPACITY];
				weight += (int)Projects[DCMDB_PROJECT_WEIGHT];

				fRange = min(fRange,(float)Projects[DCMDB_PROJECT_FLY_RANGE]);
				fSpeed = min(fSpeed,(float)Projects[DCMDB_PROJECT_FLY_SPEED]);
				ship_count++;
			}
		}
		while(Units->Next());
	}

	if(fSpeed == std::numeric_limits<float>::infinity()) fSpeed = 0;
	if(fRange == std::numeric_limits<float>::infinity()) fRange = 0;

	Fleets->Update();
	Fleets[DCMDB_FLEET_FLY_RANGE] = fRange;
	Fleets[DCMDB_FLEET_FLY_SPEED] = fSpeed;
	Fleets[DCMDB_FLEET_SHIP_COUNT] = ship_count;
	Fleets[DCMDB_FLEET_TRANSPORT_CAPACITY] = cargo_cells;
	Fleets[DCMDB_FLEET_CARGO_COUNT] = cargo_count;
	Fleets[DCMDB_FLEET_WEIGHT] = weight;
	Fleets->Post();

	return true;
}

bool DCMAPAPI CFleetManager::CanMoveUnitToFleet( dcmapDBREF const& Unit,dcmapDBREF const& Fleet )
{
	if(!Units->SelectID(Unit))return false;
	DCMapBuildingInfo const* pUnit = Units->GetData();
	if(!Fleets->SelectID(pUnit->parent))return false;
	DCMapFleetInfo const* pSrcFleet = Fleets->GetData();	
	
	if(!Fleets->SelectID(Fleet))return false;
	DCMapFleetInfo const* pDestFleet = Fleets->GetData();

	if(pDestFleet->x != pSrcFleet->x)return false;
	if(pDestFleet->y != pSrcFleet->y)return false;

	if(pDestFleet->parent != pSrcFleet->parent)return false;


	if(!Projects->SelectID(pUnit->project))return false;

	if(Projects[DCMDB_PROJECT_IS_TRANSPORTABLE])
	{	
		if(pDestFleet->type != DCMAP_FTP_Garrizon)
		{
			if( (pDestFleet->transport_capacity - pDestFleet->cargo_count) < 1) return false;
		}
	}
	else 
	{
		if(pSrcFleet->type != DCMAP_FTP_Garrizon)
		{
			int cap = Projects[DCMDB_PROJECT_TRANSPORT_CAPACITY];
			if( (pSrcFleet->transport_capacity - cap)-pSrcFleet->cargo_count < 0) return false;
		}
	}
	return true;
}

bool DCMAPAPI CFleetManager::MoveUnitToFleet( dcmapDBREF const& Unit,dcmapDBREF const& Fleet )
{
	if(!CanMoveUnitToFleet(Unit,Fleet))
	{
		if(CanLoadCargo(Unit,Fleet))return LoadCargo(Unit,Fleet);
		else return false;
	}

	DCMapScopeBlockUpdates bupd(m_pCore);

	if(!Fleets->SelectID(Fleet))return false;
	DCMapFleetInfo const* pDestFleet = Fleets->GetData();
	if(!Units->SelectID(Unit))return false;
	DCMapBuildingInfo const* pUnit = Units->GetData();


	DCMapActionInfo ActInfo;
	Actions->FillDefaults(&ActInfo);

	ActInfo.x = pDestFleet->x;
	ActInfo.y = pDestFleet->y;

	ActInfo.source_fleet = pUnit->parent;
	ActInfo.fleet = Fleet;
	ActInfo.parent = pDestFleet->ruler;
	ActInfo.building = Unit;
	ActInfo.type = DCMACT_MOVE_UNIT_TO_FLEET;

	Actions->Insert();
	Actions->SetData(&ActInfo);
	Actions->Post(); 

	Units->Update();
	Units[DCMDB_BUILDING_PARENT] = Fleet;
	Units->Post();

	CalculateFleet(ActInfo.source_fleet);
	CalculateFleet(ActInfo.fleet);

	UpdateActions();
	return true;
}

bool DCMAPAPI CFleetManager::DeleteFleet(dcmapDBREF const& Fleet)
{
	DCMapScopeBlockUpdates bupd(m_pCore);
	if(!Fleets->SelectID(Fleet))return false;

	DCMapActionInfo ActInfo;
	Actions->FillDefaults(&ActInfo);

	ActInfo.parent = Fleets[DCMDB_FLEET_RULER];
	ActInfo.type = DCMACT_DELETE_FLEET;
	ActInfo.fleet = Fleet;

	Actions->Insert();
	Actions->SetData(&ActInfo);
	Actions->Post(); 

	Fleets->Update();
	Fleets[DCMDB_FLEET_DELETED] = true; // disband unit
	Fleets->Post();

	UpdateFleetGroups(Fleet);

	UpdateActions();
	return false;
}

bool DCMAPAPI CFleetManager::CanDisbandUnit(dcmapDBREF const& Unit)
{
	return true;
}

bool DCMAPAPI CFleetManager::DisbandUnit(dcmapDBREF const& Unit)
{
	if(!CanDisbandUnit(Unit))return false;

	DCMapScopeBlockUpdates bupd(m_pCore);

	if(!Units->SelectID(Unit))return false;
	DCMapBuildingInfo const* pUnit = Units->GetData();

	if(!Fleets->SelectID(Units[DCMDB_BUILDING_PARENT]))return false;
	DCMapFleetInfo const* pFInfo = Fleets->GetData();

	DCMapActionInfo ActInfo;
	Actions->FillDefaults(&ActInfo);

	ActInfo.parent = pFInfo->ruler;

	if(Units[DCMDB_BUILDING_QUEUED])
		ActInfo.type = DCMACT_DROP_BUILDING_FROM_QUEUE;
	else
		ActInfo.type = DCMACT_DISBAND_UNIT;

	ActInfo.building = Unit;
	ActInfo.x = pFInfo->x;
	ActInfo.y = pFInfo->y;

	Actions->Insert();
	Actions->SetData(&ActInfo);
	Actions->Post(); 

	Units->Update();
	Units[DCMDB_BUILDING_DISBANDED] = true; // disband unit
	Units->Post();

	CalculateFleet(Units[DCMDB_BUILDING_PARENT]);

	UpdateActions();
	return true;
}

bool DCMAPAPI CFleetManager::SelectFleet(dcmapDBREF const* pFleet)
{
	if(pFleet) 
	{
		if(!UseFleet(*pFleet))return false;
		m_pWorkspace->BroadcastMessage(DCMM_FLEET_SELECT,(dcmapWPARAM)pFleet,0,this);
		return true;
	}
	else
	{
		m_pWorkspace->BroadcastMessage(DCMM_FLEET_SELECT,0,0,this);
		return true;
	}
}

bool DCMAPAPI CFleetManager::UseFleet( dcmapDBREF const& Fleet )
{
	if(!Fleets->SelectID(Fleet))return false;


	DCMapFleetInfo const* pFInfo = Fleets->GetData();
	if(pFInfo->flags & DCMAP_FF_InTransit)return false;
	if((pFInfo->tta))return false;
	// get fleet open group
	if(!Players->SelectID(pFInfo->parent))return false;
	m_Opens = Players[DCMDB_PLAYER_OPEN_PLANETS];

	if(!pFInfo->fly_speed)return false;

	m_JumpInfo.Fleet = Fleet;
	m_JumpInfo.Range = pFInfo->fly_range;
	m_JumpInfo.Speed = pFInfo->fly_speed;
	m_JumpInfo.MaxTurns = floor((pFInfo->fly_range/pFInfo->fly_speed)+0.5);
	m_JumpInfo.x = pFInfo->x;
	m_JumpInfo.y = pFInfo->y;

	if(!Planets->SelectCoord(pFInfo->x,pFInfo->y))return false;
	dcmapDBREF Planet = *Planets->GetID();
	m_JumpInfo.JumpFromOpen = PlanetGroups->SelectConnect(m_Opens,Planet);

	return true;
}

int DCMAPAPI CFleetManager::TestJump( int x,int y, bool* PlanetJumpable )
{
	int dx = m_JumpInfo.x - x;
	int dy = m_JumpInfo.y - y;
	dcmapREAL dist = sqrt((dcmapREAL)dx*dx+dy*dy);
	if(dist > m_JumpInfo.Range) return 0;

	bool jump = false;
	if(Planets->SelectCoord(x,y))
	{
		dcmapDBREF TargetPlanet = *Planets->GetID();
		jump = PlanetGroups->SelectConnect(m_Opens,TargetPlanet);
	}
	if(!m_JumpInfo.JumpFromOpen && !jump) return 0;

	if(PlanetJumpable)*PlanetJumpable = jump;
	return ceil(dist/m_JumpInfo.Speed);
}

bool DCMAPAPI CFleetManager::RenameFleet(dcmapDBREF const& Fleet,dcmapWCSTR name,dcmapWCSTR public_name/*=0*/)
{
	DCMapScopeBlockUpdates bupd(m_pCore);

	wstring new_name,new_public_name;

	if(!Fleets->SelectID(Fleet))return false;
	DCMapFleetInfo const* pInfo = Fleets->GetData();

	DCMapActionInfo ActInfo;
	Actions->FillDefaults(&ActInfo);

	ActInfo.fleet = Fleet;
	ActInfo.parent = pInfo->ruler;
	ActInfo.x = pInfo->x;
	ActInfo.y = pInfo->y;

	ActInfo.type = DCMACT_CHANGE_FLEET_NAME;

	if(name)wcsncpy(ActInfo.new_name,name,DCMapActionInfo::name_len);
	else wcsncpy(ActInfo.new_name,pInfo->name,DCMapActionInfo::name_len);

	wcsncpy(ActInfo.old_name,pInfo->name,DCMapActionInfo::name_len);

	if(public_name)wcsncpy(ActInfo.new_public_name,public_name,DCMapActionInfo::name_len);
	else wcsncpy(ActInfo.new_public_name,pInfo->public_name,DCMapActionInfo::name_len);

	wcsncpy(ActInfo.old_public_name,pInfo->public_name,DCMapActionInfo::name_len);

	if(!Fleets->SelectID(Fleet))return false;
	Fleets->Update();
	if(name)
	{
		Fleets[DCMDB_FLEET_NAME] = name;
		Fleets[DCMDB_FLEET_CAPTION] = name;
	}
	if(public_name)Fleets[DCMDB_FLEET_PUBLIC_NAME] = public_name;
	Fleets->Post(); 

	Actions->Insert();
	Actions->SetData(&ActInfo);
	Actions->Post(); 
	UpdateActions();


	UpdateFleetGroups(Fleet);
	return true;
}


bool DCMAPAPI CFleetManager::AddUnitToQueue(dcmapDBREF const& Planet,dcmapDBREF const& Project )
{
	DCMapScopeBlockUpdates bupd(m_pCore);

	if(!Projects->SelectID(Project))return false;
	if(!Planets->SelectID(Planet))return false;

	// create dummy building to put in queue
	dcmapDBREF refBld;
	dcmapDBREF refProj = *Projects->GetID();
	dcmapDBREF refQueue = Planets[DCMDB_PLANET_QUEUE];
	dcmapDBREF refOwner = Projects[DCMDB_PROJECT_OWNER];


	Units->Insert();
	Units[DCMDB_BUILDING_OWNER] = refQueue;
	Units[DCMDB_BUILDING_PROJECT] = refProj;
	Units[DCMDB_BUILDING_QUEUED] = true;
	Units[DCMDB_BUILDING_PLANETARY] = (bool)Projects[DCMDB_PROJECT_IS_BUILDING];
	Units->Post(&refBld);

	if(!Fleets->SelectID(refQueue))return false;
	Fleets->Update();
	Fleets[DCMDB_FLEET_SHIP_COUNT] = (int)Fleets[DCMDB_FLEET_SHIP_COUNT]+1;
	Fleets->Post();

	CalculateFleet(refQueue);

	// create action
	Actions->Insert();
	Actions[DCMDB_ACTION_TYPE]     =  DCMACT_ADD_BUILDING_TO_QUEUE;
	Actions[DCMDB_ACTION_PROJECT]  =  refProj;
	Actions[DCMDB_ACTION_PLANET]   =  Planet;
	Actions[DCMDB_ACTION_FLEET]    =  refQueue;
	Actions[DCMDB_ACTION_BUILDING] =  refBld;
	Actions[DCMDB_ACTION_X]		   =  (int)Planets[DCMDB_PLANET_X];
	Actions[DCMDB_ACTION_Y]        =  (int)Planets[DCMDB_PLANET_Y];
	Actions[DCMDB_ACTION_OWNER]    =  refOwner;
	Actions->Post();
	UpdateActions();

	return true;
}

bool DCMAPAPI CFleetManager::SetFleetIcon( dcmapDBREF const& Fleet,dcmapDBREF const& Icon )
{
	DCMapScopeBlockUpdates bupd(m_pCore);

	if(!Fleets->SelectID(Fleet))return false;
	bool group = ((int)Fleets[DCMDB_FLEET_TYPE] == DCMAP_FTP_Group);
	dcmapDBREF OldIcon = Fleets[DCMDB_FLEET_ICON];
	Fleets->Update();
	Fleets[DCMDB_FLEET_ICON] = Icon;
	Fleets->Post();

	if(group)
	{
		if(Fleets->SelectGroup(Fleet))
		do
		{
			if( (dcmapDBREF)Fleets[DCMDB_FLEET_ICON] != OldIcon)continue;

			if(((int)Fleets[DCMDB_FLEET_TYPE] == DCMAP_FTP_Group))
			{
				Fleets->PushGroupSelectState();
				SetFleetIcon(*Fleets->GetID(),Icon);
				Fleets->PopGroupSelectState();
			}
			else
			{
				SetFleetIcon(*Fleets->GetID(),Icon);
			}
		}
		while(Fleets->Next());
	}

	return true;
}

bool DCMAPAPI CFleetManager::CanExecuteUnitAction(dcmapDBREF const& Unit,dcmapDBREF const& Action )
{
	if(!ProjectActs->SelectID(Action))return false;
	DCMapProjectActInfo const* actinfo = ProjectActs->GetData();

	if(!Units->SelectID(Unit))return false;
	if(!Fleets->SelectID(Units[DCMDB_BUILDING_PARENT]))return false;

	if((int)Fleets[DCMDB_FLEET_TTA])return false;


	dcmapDBREF Owner = Fleets[DCMDB_FLEET_OWNER];
	if(!Players->SelectID(Owner))return false;
	dcmapDBREF OwnerGroup = Players[DCMDB_PLAYER_DIP_RELATIONS];

	int dip = -1;
	if(Planets->SelectCoord(Fleets[DCMDB_FLEET_X],Fleets[DCMDB_FLEET_Y]))
	{
		if(Players->SelectID(Planets[DCMDB_PLANET_OWNER]))
		{
			if(Owner == *Players->GetID())dip = DCMAP_DIP_MULT;
			else if(PlayerGroups->SelectConnect(OwnerGroup,*Players->GetID()))
			{
				dip = PlayerGroups[DCMDB_PLAYERSGROUP_DIPID];
			}
			else dip = DCMAP_DIP_UNDEFINED;
		}
	}

	switch(dip)
	{
	case -1: if( (actinfo->flags&DCMAP_PAF_PlanetEmpty) == 0)return false;break;
	case DCMAP_DIP_ENEMIES: if( (actinfo->flags&DCMAP_PAF_PlanetDipEnemy) == 0)return false;break;
	case DCMAP_DIP_UNDEFINED: if( (actinfo->flags&DCMAP_PAF_PlanetDipUndefined) == 0)return false;break;
	case DCMAP_DIP_NEUTRALS: if( (actinfo->flags&DCMAP_PAF_PlanetDipNeutrals) == 0)return false;break;
	case DCMAP_DIP_ALLY: if( (actinfo->flags&DCMAP_PAF_PlanetDipAlly) == 0)return false;break;
	case DCMAP_DIP_LORD: if( (actinfo->flags&DCMAP_PAF_PlanetDipLord) == 0)return false;break;
	case DCMAP_DIP_VASSAL: if( (actinfo->flags&DCMAP_PAF_PlanetDipVassal) == 0)return false;break;
	case DCMAP_DIP_MULT: return false;break;
	}

	return true;
}


bool DCMAPAPI CFleetManager::ExecuteUnitAction( dcmapDBREF const& Unit,int ActionID )
{
	DCMapScopeBlockUpdates bupd(m_pCore);

	if(!Units->SelectID(Unit))return false;
	dcmapDBREF Fleet = Units[DCMDB_BUILDING_PARENT];
	if(!Projects->SelectID(Units[DCMDB_BUILDING_PROJECT]))return false;
	if(!Fleets->SelectID(Fleet))return false;
	dcmapDBREF Owner = Fleets[DCMDB_FLEET_RULER];

	DCMapProjectActInfo const* actinfo=0;

	if(ProjectActs->SelectByParentID(Units[DCMDB_BUILDING_PROJECT]))
	do
	{
		DCMapProjectActInfo const* inf = ProjectActs->GetData();
		if(inf->actid == ActionID)
		{
			actinfo = inf;
			break;
		}
	}
	while(ProjectActs->Next());
	if(!actinfo)return false;

	if(!CanExecuteUnitAction(Unit,actinfo->id))return false;

	ActReq->Insert();

	ActReq[DCMDB_ACTREQ_ACTID] = ActionID;
	ActReq[DCMDB_ACTREQ_ACTION] = actinfo->id;
	ActReq[DCMDB_ACTREQ_UNIT] = Unit;
	ActReq[DCMDB_ACTREQ_FLEET] = Fleet;
	ActReq[DCMDB_ACTREQ_LOADER] = Owner;

	dcmapDBREF refActReq;
	ActReq->Post(&refActReq);


	// create action
	Actions->Insert();
	Actions[DCMDB_ACTION_TYPE]     =  DCMACT_STORE_ACTION;
	Actions[DCMDB_ACTION_BUILDING] =  Unit;
	Actions[DCMDB_ACTION_FLEET]    =  Fleet;
	Actions[DCMDB_ACTION_OWNER]    =  Owner;
	Actions[DCMDB_ACTION_ACTREQ]   =  refActReq;
	Actions->Post();
	UpdateActions();


	return true;
}

bool DCMAPAPI CFleetManager::CancelUnitAction( dcmapDBREF const& Unit,int ActionID )
{
	dcmapDBREF refActReq;

	if(!Units->SelectID(Unit))return false;
	dcmapDBREF Fleet = Units[DCMDB_BUILDING_PARENT];
	if(!Fleets->SelectID(Fleet))return false;
	dcmapDBREF Owner = Fleets[DCMDB_FLEET_RULER];

	DCMapScopeBlockUpdates bupd(m_pCore);

	if(ActReq->SelectByParentID(Unit))
	do
	{
		if(ActReq[DCMDB_ACTREQ_CANCELED])continue;
		int actid = ActReq[DCMDB_ACTREQ_ACTID];
		if(actid == ActionID)
		{
			ActReq->Update();
			ActReq[DCMDB_ACTREQ_CANCELED] = true;
			ActReq->Post(&refActReq);

			// create action
			Actions->Insert();
			Actions[DCMDB_ACTION_TYPE]     =  DCMACT_CANCEL_ACTION;
			Actions[DCMDB_ACTION_BUILDING] =  Unit;
			Actions[DCMDB_ACTION_FLEET]    =  Fleet;
			Actions[DCMDB_ACTION_OWNER]    =  Owner;
			Actions[DCMDB_ACTION_ACTREQ]   =  refActReq;
			Actions->Post();

			UpdateActions();
			return true;
		}
	}
	while(ActReq->Next());

	return false;
}

bool DCMAPAPI CFleetManager::UpdateFleet(dcmapDBREF const& Fleet)
{
	if(!Fleets->SelectID(Fleet))return false;
	DCMapScopeBlockUpdates bupd(m_pCore);

	Fleets->Update();
	Fleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_Ungroupped;
	Fleets->Post();

	CalculateFleet(Fleet);
	UpdateFleetGroups(Fleet);

	return true;
}

bool DCMAPAPI CFleetManager::CanHideFleet( dcmapDBREF const& Fleet,int iMode )
{
	if(!Fleets->SelectID(Fleet))return false;

	switch(iMode)
	{
		case DCMAP_HIDEFM_HIDE:
			if(Fleets[DCMDB_FLEET_HIDDEN])return false;
			if((int)Fleets[DCMDB_FLEET_CHANGES]!=0)return false;
			break;
		case DCMAP_HIDEFM_CANCEL_HIDE:
			if(Fleets[DCMDB_FLEET_HIDDEN])return false;
			if((int)Fleets[DCMDB_FLEET_CHANGES]!=1)return false;
			break;
		case DCMAP_HIDEFM_UNHIDE:
			if(!Fleets[DCMDB_FLEET_HIDDEN])return false;
			if((int)Fleets[DCMDB_FLEET_CHANGES]!=0)return false;
			break;
		case DCMAP_HIDEFM_CANCEL_UNHIDE:
			if(!Fleets[DCMDB_FLEET_HIDDEN])return false;
			if((int)Fleets[DCMDB_FLEET_CHANGES]!=2)return false;
			break;
		default:
			return false;
	}
	return true;
}

bool DCMAPAPI CFleetManager::HideFleet( dcmapDBREF const& Fleet,int iMode )
{
	if(!CanHideFleet(Fleet,iMode))return false;
	
	if(!Fleets->SelectID(Fleet))return false;
	dcmapDBREF Owner = Fleets[DCMDB_FLEET_RULER];

	DCMapScopeBlockUpdates bupd(m_pCore);

	Fleets->Update();

	switch(iMode)
	{
	case DCMAP_HIDEFM_HIDE:
		Fleets[DCMDB_FLEET_CHANGES]=1;
		break;
	case DCMAP_HIDEFM_CANCEL_HIDE:
		Fleets[DCMDB_FLEET_CHANGES]=0;
		break;
	case DCMAP_HIDEFM_UNHIDE:
		Fleets[DCMDB_FLEET_CHANGES]=2;
		break;
	case DCMAP_HIDEFM_CANCEL_UNHIDE:
		Fleets[DCMDB_FLEET_CHANGES]=0;
		break;
	default:
		return false;
	}

	Fleets->Post();

	// create action
	Actions->Insert();
	Actions[DCMDB_ACTION_TYPE]     =  DCMACT_HIDEFLEET;
	Actions[DCMDB_ACTION_FLEET]    =  Fleet;
	Actions[DCMDB_ACTION_OWNER]    =  Owner;
	Actions[DCMDB_ACTION_MODE]     =  iMode;
	Actions->Post();

	UpdateActions();
	return true;
}

bool DCMAPAPI CFleetManager::ChangeBehavior( dcmapDBREF const& Fleet,dcmapDWORD iBehavior )
{
	if(!Fleets->SelectID(Fleet))return false;
	dcmapDBREF Owner = Fleets[DCMDB_FLEET_RULER];

	dcmapDWORD obeh = Fleets[DCMDB_FLEET_BEHAVIOR];

	DCMapScopeBlockUpdates bupd(m_pCore);

	Fleets->Update();
	Fleets[DCMDB_FLEET_BEHAVIOR]=iBehavior;
	Fleets->Post();

	// create action
	Actions->Insert();
	Actions[DCMDB_ACTION_TYPE]     =  DCMACT_CHANGE_BEHAVIOR;
	Actions[DCMDB_ACTION_FLEET]    =  Fleet;
	Actions[DCMDB_ACTION_OWNER]    =  Owner;
	Actions[DCMDB_ACTION_MODE]     =  iBehavior;
	Actions[DCMDB_ACTION_OLD_MODE] =  obeh;
	Actions->Post();

	UpdateActions();
	return true;
}

bool DCMAPAPI CFleetManager::CanUnpackContainer(dcmapDBREF const& Container)
{
	if(!Units->SelectID(Container))return false;
	// test if tagged
	if(!Units[DCMDB_BUILDING_TAGGED])return false;

	if(!UnitTags->SelectByParentID(Container))return false;
	if(UnitTags[DCMDB_UNITTAG_UNWRAPPED])return false;

	int type = UnitTags[DCMDB_UNITTAG_TYPE];

	if(type == DCMAP_UTT_RESOURCE)return true;
	if(type == DCMAP_UTT_UNIT)return true;

	return false;
}

dcmapDBREF const* DCMAPAPI CFleetManager::GetGarrison(int x,int y)
{
	if(!Planets->SelectCoord(x,y))return &g_NullDBREF;
	return Planets->GetReference(DCMDB_PLANET_GARRISON);
}

dcmapDBREF const* DCMAPAPI CFleetManager::GetBuildings(int x,int y)
{
	if(!Planets->SelectCoord(x,y))return &g_NullDBREF;
	return Planets->GetReference(DCMDB_PLANET_BUILDINGS);
}

bool DCMAPAPI CFleetManager::UnpackContainer(dcmapDBREF const& Container)
{
	if(!CanUnpackContainer(Container))return false;

	if(!Units->SelectID(Container))return false;

	dcmapDBREF Fleet = Units[DCMDB_BUILDING_OWNER];
	if(!Fleets->SelectID(Fleet))return false;
	dcmapDBREF Owner =  Fleets[DCMDB_FLEET_RULER];

	if(!UnitTags->SelectByParentID(Container))return false;
	int type = UnitTags[DCMDB_UNITTAG_TYPE];

	DCMapScopeBlockUpdates bupd(m_pCore);

	if(type == DCMAP_UTT_UNIT) // move unit  into garrison
	{
		int x = Fleets[DCMDB_FLEET_X];
		int y = Fleets[DCMDB_FLEET_Y];

		int gameid = UnitTags[DCMDB_UNITTAG_ITEMID];

		dcmapDBREF Project = UnitTags[DCMDB_UNITTAG_PROJECT];
		if(!Projects->SelectID(Project))return false;

		if(Units->SelectGameID(gameid))Units->Update();
		else Units->Insert();

		Units[DCMDB_BUILDING_OWNER] = GetGarrison(x,y);
		Units[DCMDB_BUILDING_PROJECT] = Project;
		Units[DCMDB_BUILDING_CARAPACE] =  (int)Projects[DCMDB_PROJECT_CARAPACE];
		Units[DCMDB_BUILDING_GAMEID] = gameid; 
		Units[DCMDB_BUILDING_HP] =  100;
		Units->Post();
	}

	if(!Units->SelectID(Container))return false;
	Units->Update();
	Units[DCMDB_BUILDING_DISBANDED] = true;
	Units->Post();

	// create action
	Actions->Insert();
	Actions[DCMDB_ACTION_TYPE]     =  DCMACT_UNPACK_CONAINER;
	Actions[DCMDB_ACTION_FLEET]    =  Fleet;
	Actions[DCMDB_ACTION_OWNER]    =  Owner;
	Actions[DCMDB_ACTION_BUILDING] =  Container;
	Actions->Post();

	CalculateFleet(Fleet);
	UpdateActions();
	return true;
}

void CFleetManager::UpdateActions(bool need)
{
	if(m_iLockUpdate>0)
	{
		if(need) m_bNeedUpdateActions = true;
		return;
	}
	if(m_bNeedUpdateActions || need)
	{
		ActionProcessor->UpdateActions();
		m_bNeedUpdateActions = false;
	}
}


void DCMAPAPI CFleetManager::BeginUpdate()
{
	if(m_iLockUpdate == 0) m_pCore->LockDataUpdate();
	++m_iLockUpdate;
}

void DCMAPAPI CFleetManager::EndUpdate()
{
	--m_iLockUpdate;
	if(m_iLockUpdate<=0)
	{
		m_iLockUpdate = 0;

		_foreach(setDBREF,m_setFleetsToCalculate,it)
		{
			CalculateFleet(*it);
		}
		m_setFleetsToCalculate.clear();

		_foreach(setDBREF,m_setFleetsToGroup,it)
		{
			UpdateFleetGroups(*it);
		}
		m_setFleetsToGroup.clear();		

		UpdateActions(false);
		m_pCore->UnlockDataUpdate();
	}
}

bool DCMAPAPI CFleetManager::CanLoadCargo( dcmapDBREF const& Container,dcmapDBREF const& Fleet )
{
	if(!Units->SelectID(Container))return false;
	if(!Units[DCMDB_BUILDING_TAGGED])return false;
	if(!UnitTags->SelectByParentID(Container))return false;
	if(UnitTags[DCMDB_UNITTAG_LOADING])return false;

	// check source
	if(!Fleets->SelectID(Units[DCMDB_BUILDING_PARENT]))return false;
	dcmapDBREF SourceOwner = Fleets[DCMDB_FLEET_OWNER];

	int x = Fleets[DCMDB_FLEET_X];
	int y = Fleets[DCMDB_FLEET_Y];

	if((int)Fleets[DCMDB_FLEET_TYPE] != DCMAP_FTP_Garrizon)return false;

	if(!Fleets->SelectID(Fleet))return false;
	if(Fleets[DCMDB_FLEET_OWNER] == SourceOwner)return false;

	if((int)Fleets[DCMDB_FLEET_X] != x)return false;
	if((int)Fleets[DCMDB_FLEET_Y] != y)return false;

	return true;
}

bool DCMAPAPI CFleetManager::LoadCargo( dcmapDBREF const& Container,dcmapDBREF const& TargetFleet )
{
	if(!CanLoadCargo(Container,TargetFleet))
	{
		if(CanMoveUnitToFleet(Container,TargetFleet))return MoveUnitToFleet(Container,TargetFleet);
		else return false;
	}

	if(!Units->SelectID(Container))return false;
	dcmapDBREF Fleet = Units[DCMDB_BUILDING_OWNER];
	if(!Fleets->SelectID(Fleet))return false;
	dcmapDBREF Owner =  Fleets[DCMDB_FLEET_RULER];
	int x = Fleets[DCMDB_FLEET_X];
	int y = Fleets[DCMDB_FLEET_Y];


	if(!UnitTags->SelectByParentID(Container))return false;

	DCMapScopeBlockUpdates bupd(m_pCore);

	UnitTags->Update();
	UnitTags[DCMDB_UNITTAG_LOADING] = true;
	UnitTags[DCMDB_UNITTAG_LOAD_FLEET] = Fleet;
	UnitTags->Post();


	// create action
	Actions->Insert();
	Actions[DCMDB_ACTION_X]        =  x;
	Actions[DCMDB_ACTION_Y]        =  y;
	Actions[DCMDB_ACTION_TYPE]     =  DCMACT_LOAD_CARGO;
	Actions[DCMDB_ACTION_FLEET]    =  TargetFleet;
	Actions[DCMDB_ACTION_SOURCE_FLEET]    =  Fleet;
	Actions[DCMDB_ACTION_OWNER]    =  Owner;
	Actions[DCMDB_ACTION_BUILDING] =  Container;
	Actions->Post();

	UpdateActions();
	return true;
}

bool DCMAPAPI CFleetManager::CanCancelCargoLoad( dcmapDBREF const& Container )
{
	if(!Units->SelectID(Container))return false;
	if(!Units[DCMDB_BUILDING_TAGGED])return false;
	if(!UnitTags->SelectByParentID(Container))return false;
	if(!UnitTags[DCMDB_UNITTAG_LOADING])return false;
	return true;
}

bool DCMAPAPI CFleetManager::CancelCargoLoad( dcmapDBREF const& Container )
{
	if(!CanCancelCargoLoad(Container))return false;

	if(!Units->SelectID(Container))return false;
	dcmapDBREF Fleet = Units[DCMDB_BUILDING_OWNER];
	if(!Fleets->SelectID(Fleet))return false;
	dcmapDBREF Owner =  Fleets[DCMDB_FLEET_RULER];

	if(!UnitTags->SelectByParentID(Container))return false;
	dcmapDBREF TargetFleet = UnitTags[DCMDB_UNITTAG_LOAD_FLEET];

	DCMapScopeBlockUpdates bupd(m_pCore);

	UnitTags->Update();
	UnitTags[DCMDB_UNITTAG_LOADING] = false;
	UnitTags[DCMDB_UNITTAG_LOAD_FLEET] = dcmapDBREF();
	UnitTags->Post();

	// create action
	Actions->Insert();
	Actions[DCMDB_ACTION_TYPE]            =  DCMACT_CANCEL_CARGO_LOAD;
	Actions[DCMDB_ACTION_FLEET]           =  TargetFleet;
	Actions[DCMDB_ACTION_SOURCE_FLEET]    =  Fleet;
	Actions[DCMDB_ACTION_OWNER]           =  Owner;
	Actions[DCMDB_ACTION_BUILDING]        =  Container;
	Actions->Post();

	UpdateActions();
	return true;
}

