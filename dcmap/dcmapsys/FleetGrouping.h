#pragma once
#include "../dcmapsys.h"
#include "FleetStorage.h"
#include "utils.h"

class CGroupManager;
struct GroupDef;
struct GroupRuleDef;
struct GroupManagerParams;
class CRootGroupManager;

typedef bool (*GroupAppoveProc)(DCMapFleetInfo const* pinf,GroupManagerParams const* Param);
typedef CGroupManager* (*GroupManagerCreateProc)(GroupManagerParams const* Param);

struct GroupManagerParams
{
	GroupManagerParams():SortIndex(FGI_Regular),Icon(FICO_Group),Group(DCMAP_FGRP_AutoGroup){}
	int SortIndex;
	int Icon;
	int Group;
};
struct GroupRuleDef
{
	GroupDef* pChildGroupDefDef;
	GroupAppoveProc Approve;
	GroupManagerCreateProc Creator;
	GroupManagerParams Params;

	GroupRuleDef():pChildGroupDefDef(0),Approve(0),Creator(0){}

	void Set(GroupDef* Childs,
			GroupAppoveProc ap,
			GroupManagerCreateProc cr,
			int sort=0,
			int icon=-1,
			int group = -1)
	{
		pChildGroupDefDef = Childs;
		Approve = ap;
		Creator = cr;
		if(sort)Params.SortIndex = sort;
		if(icon>=0)Params.Icon = icon;
		if(group>=0)Params.Group = group;
	}
};

struct GroupDef
{
	vector<GroupRuleDef> childs;
};

class CFleetGrouping :  public IDCMapFleetGrouping
{
    IDCMapPlanetDataStoragePtr   Planets;
    IDCMapPlayersDataStoragePtr   Players;
	IDCMapBuildingsDataStoragePtr Units;
    IDCMapFleetsDataStoragePtr    Fleets;

    IDCMapCore* m_pCore;

	GroupDef m_GroupDef;
	CRootGroupManager* m_Root;

	bool Initialize();
public:
    CFleetGrouping(void);
    virtual ~CFleetGrouping(void);

    bool DCMAPAPI Init( IDCMapCore* core,IDCMapWorkspace* workspace );
    bool DCMAPAPI ProcessFleets();
    bool DCMAPAPI ProcessFleet(DCMapFleetInfo* pinf);

	int DCMAPAPI OnMessage(int Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender);

	bool DCMAPAPI UpdateFleetGroups(dcmapDBREF const& ref);

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface){
		DCMAP_MODULE_SUPPORTS(IDCMapFleetGrouping);return 0;}
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);

	void PreProcessFleets();


};
