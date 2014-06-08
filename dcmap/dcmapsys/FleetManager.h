#pragma once
#include "../dcmapsys.h"

class CFleetManager : public IDCMapFleetManager
{
    IDCMapFleetsDataStoragePtr Fleets;
	IDCMapBuildingsDataStoragePtr Units;
	IDCMapPlanetDataStoragePtr Planets;
	IDCMapProjectDataStoragePtr Projects;
	IDCMapPlayersDataStoragePtr Players;
	IDCMapActionsDataStoragePtr Actions;
	IDCMapGroupsDataStoragePtr  Groups;
	IDCMapPlanetsGroupsDataStoragePtr  PlanetGroups;
	IDCMapPlayersGroupsDataStoragePtr  PlayerGroups;
	IDCMapComponentsDataStoragePtr Components;
	IDCMapProjectActsDataStoragePtr ProjectActs;
	IDCMapActionsRequestedDataStoragePtr ActReq;
	IDCMapUnitTagsDataStoragePtr UnitTags;

    IDCMapCore* m_pCore;
	IDCMapWorkspace* m_pWorkspace;

	dcmapDBREF m_Opens;

	IDCMapFleetGrouping* FleetGrouping;
	IDCMapActionProcessor* ActionProcessor;
	IDCMapPlanetCalculator* PlanetCalculator;

	FleetJumpInfo m_JumpInfo;

	typedef std::set<dcmapDBREF> setDBREF;

	setDBREF m_setFleetsToCalculate;
	setDBREF m_setFleetsToGroup;

	int m_iLockUpdate;
	bool m_bNeedUpdateActions;

	void UpdateActions(bool need=true);

public:
	CFleetManager(void);
	virtual ~CFleetManager(void);

	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface){
		DCMAP_MODULE_SUPPORTS(IDCMapFleetManager);return 0;}

public:

	bool DCMAPAPI CreateFleetAt(int x,int y,dcmapDBREF const& Owner,dcmapWCSTR name,dcmapDBREF* pResult);
	bool DCMAPAPI MoveUnitToFleet(dcmapDBREF const& Unit,dcmapDBREF const& Fleet);
	bool DCMAPAPI CanMoveUnitToFleet( dcmapDBREF const& Unit,dcmapDBREF const& Fleet );

	bool DCMAPAPI MoveFleetTo(dcmapDBREF const& Fleet,int x,int y);
	bool DCMAPAPI CanMoveFleetTo(dcmapDBREF const& Fleet,int x,int y);
    bool DCMAPAPI CancelJump(dcmapDBREF const& Fleet);
    bool DCMAPAPI CanCancelJump(dcmapDBREF const& Fleet);

	bool DCMAPAPI CalculateFleet(dcmapDBREF const& Fleet);
	bool DCMAPAPI UpdateFleetGroups(dcmapDBREF const& Fleet);

	bool DCMAPAPI DisbandUnit(dcmapDBREF const& Unit);
	bool DCMAPAPI DeleteFleet(dcmapDBREF const& Fleet);

	bool DCMAPAPI UpdateFleet(dcmapDBREF const& Fleet);

	bool DCMAPAPI RenameFleet(dcmapDBREF const& Fleet,dcmapWCSTR name,dcmapWCSTR public_name=0);

	bool DCMAPAPI CanDisbandUnit(dcmapDBREF const& Unit);
	bool DCMAPAPI AddUnitToQueue(dcmapDBREF const& Planet,dcmapDBREF const& Project );

	bool DCMAPAPI SetFleetIcon(dcmapDBREF const& Fleet,dcmapDBREF const& Icon );

	bool DCMAPAPI CanExecuteUnitAction(dcmapDBREF const& Unit,dcmapDBREF const& Action );
	bool DCMAPAPI ExecuteUnitAction(dcmapDBREF const& Unit,int ActionID);
	bool DCMAPAPI CancelUnitAction(dcmapDBREF const& Unit,int ActionID);

	bool DCMAPAPI CanHideFleet(dcmapDBREF const& Fleet,int ActionID);
	bool DCMAPAPI HideFleet(dcmapDBREF const& Fleet,int ActionID);

	bool DCMAPAPI ChangeBehavior(dcmapDBREF const& Fleet,dcmapDWORD iBehavior);

	bool DCMAPAPI UnpackContainer(dcmapDBREF const& Container);
	bool DCMAPAPI CanUnpackContainer(dcmapDBREF const& Container);

	bool DCMAPAPI CanLoadCargo(dcmapDBREF const& Container,dcmapDBREF const& Fleet);
	bool DCMAPAPI LoadCargo(dcmapDBREF const& Container,dcmapDBREF const& Fleet);
	bool DCMAPAPI CanCancelCargoLoad(dcmapDBREF const& Container);
	bool DCMAPAPI CancelCargoLoad(dcmapDBREF const& Container);

	bool DCMAPAPI SelectFleet(dcmapDBREF const* pFleet);
	bool DCMAPAPI UseFleet(dcmapDBREF const& Fleet);
	int DCMAPAPI TestJump(int x,int y, bool* PlanetJumpable );
	FleetJumpInfo const* DCMAPAPI GetJumpInfo(){return &m_JumpInfo;}

	void DCMAPAPI BeginUpdate();
	void DCMAPAPI EndUpdate();
	dcmapDBREF const* DCMAPAPI GetGarrison(int x,int y);
	dcmapDBREF const* DCMAPAPI GetBuildings(int x,int y);
};
