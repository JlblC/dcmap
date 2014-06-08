#include "StdAfx.h"
#include "FleetGrouping.h"
#include "../api/dcmap_script_helper.h"

DCMAP_REGISTER(CFleetGrouping,"PCoreService","FleetGrouping");

DCMAP_SCRIPT_REFLECTION(CFleetGrouping)
{
	DCMAP_SCRIPT_FUNC(ProcessFleets);
}


class CGroupManager
{
protected:
	typedef vector<CGroupManager*> VecManagers;
	struct GroupRefs
	{
		GroupRefs(){}
		GroupRefs(dcmapDBREF const& ref):GRef(ref){}
		dcmapDBREF GRef;
		VecManagers ChildMans;
	};

	IDCMapFleetsDataStoragePtr Fleets;
	GroupDef* m_pDef;
	wstring m_GroupName;
	dcmapDBREF ParentGroupRef;
	GroupManagerParams const* m_Params;

	void SetParentGroup(DCMapFleetInfo * pinf,dcmapDBREF const& GRef)
	{
		pinf->parentgroup = GRef;
		if(pinf->group == DCMAP_FGRP_Ungroupped)
		{
			pinf->group = DCMAP_FGRP_Groupped;
		}
	}
	virtual bool Initialize(IDCMapFleetsDataStoragePtr& pFleets,dcmapDBREF const& ParentGrp,
		GroupDef* pGroupDef,GroupManagerParams const* Params)
	{
		m_Params = Params;
		ParentGroupRef = ParentGrp;
		Fleets = pFleets;
		m_pDef = pGroupDef;
		return true;
	}

	void ProcessGroup(DCMapFleetInfo * pinf,dcmapDBREF const& GRef,VecManagers& mans)
	{
		if(m_pDef)
		{
			int sz=m_pDef->childs.size();
			for(int i=0;i<sz;i++)
			{
				GroupRuleDef& rule = m_pDef->childs[i];
				if(rule.Approve && rule.Approve(pinf,&rule.Params))
				{
					if(!mans.size()) mans.resize(sz,0);
					if(!mans[i])
					{
						if(!rule.Creator)return;

						mans[i] = rule.Creator(&rule.Params); 
						if(!mans[i] ||
							!mans[i]->Initialize(Fleets,GRef,rule.pChildGroupDefDef,&rule.Params))
						{
							return;
						}
					}
					mans[i]->Process(pinf);
					return;
				}
			}
		}
		// if none of childs takes it:
		SetParentGroup(pinf,GRef);
	}

	void SetupNewGroup()
	{
		Fleets[DCMDB_FLEET_EXTNAME] = m_GroupName;
		Fleets[DCMDB_FLEET_LIST_IMAGE] = m_Params->Icon;
		Fleets[DCMDB_FLEET_SORTINDEX] = m_Params->SortIndex;
	}
public:
	virtual ~CGroupManager()
	{

	}
	virtual void Process(DCMapFleetInfo * pinf)=0;
};

class CSimpleGroupManager : public CGroupManager
{
	typedef CGroupManager Inherited;
protected:
	dcmapDBREF GRef;
	VecManagers ChildMans;
	wstring m_GroupCaption;
public:
	CSimpleGroupManager(){}
	CSimpleGroupManager(wstring const& name,wstring const& caption):m_GroupCaption(caption)
	{
		m_GroupName = name;
	}

	virtual bool Initialize(IDCMapFleetsDataStoragePtr& pFleets,dcmapDBREF const& ParentGrp,
		GroupDef* pGroupDef,GroupManagerParams const* Params)
	{
		if(!Inherited::Initialize(pFleets,ParentGrp,pGroupDef,Params))return false;

		IDCMapFleetsDataStoragePtr Fleets(pFleets);
		if(Fleets->SelectGroup(ParentGrp))
			do 
			{
				if((wstring)Fleets[DCMDB_FLEET_EXTNAME] == m_GroupName)
				{
					GRef = *Fleets->GetID();
					return true;
				}
			} while(Fleets->Next());

			Fleets->Insert();
			SetupNewGroup();
			Fleets[DCMDB_FLEET_NAME] = m_GroupName;
			Fleets[DCMDB_FLEET_CAPTION] = m_GroupCaption;
			Fleets[DCMDB_FLEET_PARENTGROUP] = ParentGrp;
			Fleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_AutoGroup;
			Fleets[DCMDB_FLEET_TYPE] = DCMAP_FTP_Group;
			return Fleets->Post(&GRef);
	}
	void Process(DCMapFleetInfo * pinf)
	{
		ProcessGroup(pinf,GRef,ChildMans);
	}
	virtual ~CSimpleGroupManager()
	{
		_foreach(VecManagers,ChildMans,it)
		{
			if(*it) delete *it;
		}
	}
};

class COwnerGroupManager : public CGroupManager
{
	typedef CGroupManager Inherited;
protected:
	typedef map<dcmapDBREF,GroupRefs> MapOwners;
	MapOwners m_mapOwners;
	IDCMapPlayersDataStoragePtr Players;
	IDCMapDipDataStoragePtr Dip;
public:
	COwnerGroupManager()
	{
		m_GroupName = L"owners";
	}
	virtual ~COwnerGroupManager()
	{
		_foreach(MapOwners,m_mapOwners,owners)
		{
			_foreach(VecManagers,owners->second.ChildMans,it)
			{
				if(*it) delete *it;
			}
		}
	}

	virtual bool Initialize(IDCMapFleetsDataStoragePtr& pFleets,dcmapDBREF const& ParentGrp,
		GroupDef* pGroupDef,GroupManagerParams const* Params)
	{
		if(!Inherited::Initialize(pFleets,ParentGrp,pGroupDef,Params))return false;
		if(!Players.init(GetDCMapCore()))return false;
		if(!Dip.init(GetDCMapCore()))return false;

		IDCMapFleetsDataStoragePtr Fleets(pFleets);
		if(Fleets->SelectGroup(ParentGrp))
			do 
			{
				if((wstring)Fleets[DCMDB_FLEET_EXTNAME] == m_GroupName)
				{
					m_mapOwners[Fleets[DCMDB_FLEET_OWNER]] = GroupRefs(*Fleets->GetID());
				}
			} while(Fleets->Next());

			return true;
	}
	void Process(DCMapFleetInfo * pinf)
	{
		MapOwners::iterator it = m_mapOwners.find(pinf->parent);
		if(it != m_mapOwners.end())
		{
			ProcessGroup(pinf,it->second.GRef,it->second.ChildMans);
		}
		else
		{
			GroupRefs refs;
			if(Players->SelectID(pinf->parent))
			{
				int groupico=FICO_Group_Undefined;
				int groupsort=FGI_Alien_Undefined;

				if(Dip->SelectID(Players[DCMDB_PLAYER_DIP]))
				{
					switch((int)Dip[DCMDB_DIP_DIPID])
					{
					case DCMAP_DIP_MULT:
						groupico=FICO_Group_My;
						groupsort=FGI_Regular;
						break;
					case DCMAP_DIP_ENEMIES:
						groupico=FICO_Group_Enemy;
						groupsort=FGI_Alien_Enemy;
						break;
					case DCMAP_DIP_NEUTRALS:
						groupico=FICO_Group_Neutrals;
						groupsort=FGI_Alien_Neutrals;
						break;
					case DCMAP_DIP_LORD:
					case DCMAP_DIP_VASSAL:
					case DCMAP_DIP_ALLY:
						groupico=FICO_Group_Ally;
						groupsort=FGI_Alien_Ally;
						break;
					}
				}

				Fleets->Insert();
				SetupNewGroup();
				Fleets[DCMDB_FLEET_NAME] = Players->GetWideName();
				Fleets[DCMDB_FLEET_CAPTION] = Players->GetWideName();
				Fleets[DCMDB_FLEET_OWNER] = pinf->parent;				
				Fleets[DCMDB_FLEET_PARENTGROUP] = ParentGroupRef;
				Fleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_PlayerGroup;
				Fleets[DCMDB_FLEET_TYPE] = DCMAP_FTP_Group;
				Fleets[DCMDB_FLEET_LIST_IMAGE] = groupico;
				Fleets[DCMDB_FLEET_SORTINDEX] = groupsort;
				Fleets->Post(&refs.GRef);
				m_mapOwners[pinf->parent] = refs;
				ProcessGroup(pinf,m_mapOwners[pinf->parent].GRef,
					m_mapOwners[pinf->parent].ChildMans);
			}
		}
	}
};

class CNameDivGroupManager : public CGroupManager
{
	typedef CGroupManager Inherited;
protected:
	typedef map<wstring,GroupRefs> MapNameDiv;
	MapNameDiv m_mapNameDiv;
	wstring m_strDividers;
public:
	CNameDivGroupManager()
	{
		m_GroupName = L"namediv";
		m_strDividers = L":";
	}
	virtual ~CNameDivGroupManager()
	{
		_foreach(MapNameDiv,m_mapNameDiv,owners)
		{
			_foreach(VecManagers,owners->second.ChildMans,it)
			{
				if(*it) delete *it;
			}
		}
	}
	virtual bool Initialize(IDCMapFleetsDataStoragePtr& pFleets,dcmapDBREF const& ParentGrp,GroupDef* pGroupDef,
		GroupManagerParams const* Params)
	{
		if(!Inherited::Initialize(pFleets,ParentGrp,pGroupDef,Params))return false;

		IDCMapFleetsDataStoragePtr Fleets(pFleets);
		if(Fleets->SelectGroup(ParentGrp))
			do 
			{
				if((wstring)Fleets[DCMDB_FLEET_EXTNAME] == m_GroupName)
				{
					m_mapNameDiv[Fleets[DCMDB_FLEET_NAME]] = GroupRefs(*Fleets->GetID());
				}
			} while(Fleets->Next());

			return true;
	}

	static bool Appove(DCMapFleetInfo const* pinf,GroupManagerParams const* Params)
	{
		int div = wstring(pinf->caption).find_first_of(L":");
		if(div == wstring::npos)return false;
		if(div == 0)return false;
		int l = wcslen(pinf->caption);
		if(div >= l-1)return false;
		return true;
	}
	static CGroupManager* Create(GroupManagerParams const* Params)
	{
		return new CNameDivGroupManager();
	}

	void Process(DCMapFleetInfo * pinf)
	{
		wstring cap = pinf->caption;
		// extract name part
		int l = cap.length();
		int div = cap.find_first_of(m_strDividers);

		wstring gname = cap.substr(0,div);
		wstring rest = cap.substr(div+1,wstring::npos);

		// update name & caption
		wcsncpy(pinf->caption,rest.c_str(),DCMapFleetInfo::caption_len);

		MapNameDiv::iterator it = m_mapNameDiv.find(gname);
		if(it != m_mapNameDiv.end())
		{
			ProcessGroup(pinf,it->second.GRef,it->second.ChildMans);
		}
		else
		{
			GroupRefs refs;
			Fleets->Insert();
			SetupNewGroup();
			Fleets[DCMDB_FLEET_NAME] = gname;
			Fleets[DCMDB_FLEET_CAPTION] = cap.substr(0,div+1);
			Fleets[DCMDB_FLEET_PARENTGROUP] = ParentGroupRef;
			Fleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_BlockNameGroup;
			Fleets[DCMDB_FLEET_TYPE] = DCMAP_FTP_Group;
			Fleets->Post(&refs.GRef);
			m_mapNameDiv[gname] = refs;

			ProcessGroup(pinf,m_mapNameDiv[gname].GRef,
				m_mapNameDiv[gname].ChildMans);
		}
	}
};


class CAutoNameGroupManager : public CGroupManager
{
	typedef CGroupManager Inherited;
	VecManagers ChildMans;
public:
	static bool Appove(DCMapFleetInfo const* pinf,GroupManagerParams const* Params)
	{
		return true;
	}
	static CGroupManager* Create(GroupManagerParams const* Params)
	{
		return new CAutoNameGroupManager();
	}
	CAutoNameGroupManager()
	{
		m_GroupName = L"autoname";
	}

	virtual bool Initialize(IDCMapFleetsDataStoragePtr& pFleets,dcmapDBREF const& ParentGrp,
		GroupDef* pGroupDef,GroupManagerParams const* Params)
	{
		return Inherited::Initialize(pFleets,ParentGrp,pGroupDef,Params);
	}

	void MakeCaption(DCMapFleetInfo * pinf)
	{
		wstring cap = str(wformat(L"[%d:%d]")%pinf->x%pinf->y);
		wcsncpy(pinf->caption,cap.c_str(),DCMapFleetInfo::caption_len);
	}

	void Process(DCMapFleetInfo * pinf)
	{
		dcmapDBREF GRef;
		if(Fleets->SelectGroupNames(ParentGroupRef,FGI_Regular,pinf->caption))
		{
			if((int)Fleets[DCMDB_FLEET_GROUP] == DCMAP_FGRP_NameGroup) // add to this group
			{
				// change fleet caption
				MakeCaption(pinf);
				SetParentGroup(pinf,*Fleets->GetID());
			}
			else if((int)Fleets[DCMDB_FLEET_GROUP] == DCMAP_FGRP_Groupped) // craete new auto name group
			{
				dcmapDBREF UpdRef = *Fleets->GetID();

				dcmapDBREF GRef;
				Fleets->Insert();
				SetupNewGroup();
				Fleets[DCMDB_FLEET_NAME] = pinf->caption;
				Fleets[DCMDB_FLEET_CAPTION] = pinf->caption;
				Fleets[DCMDB_FLEET_PARENTGROUP] = ParentGroupRef;
				Fleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_NameGroup;
				Fleets[DCMDB_FLEET_TYPE] = DCMAP_FTP_Group;
				Fleets->Post(&GRef);

				if(Fleets->SelectID(UpdRef))
				{
					DCMapFleetInfo sData = *Fleets->GetData();
					MakeCaption(&sData);
					SetParentGroup(&sData,GRef);

					Fleets->Update();
					Fleets->SetData(&sData);
					Fleets->Post();
				}
				MakeCaption(pinf);
				SetParentGroup(pinf,GRef);
			}
		}
		else
		{
			SetParentGroup(pinf,ParentGroupRef);
		}	
	}

};


struct CRootGroupManager : public CSimpleGroupManager
{
	bool Initialize(IDCMapFleetsDataStoragePtr& pFleets,dcmapDBREF const& ParentGrp,GroupDef* pGroupDef)
	{
		GRef = ParentGrp;
		Fleets = pFleets;
		m_pDef = pGroupDef;
		return true;		
	}
};


bool AnyGroupAppove(DCMapFleetInfo const* pinf,GroupManagerParams const* Param){return true;}

bool ForeignGroupAppove(DCMapFleetInfo const* pinf,GroupManagerParams const* Param)
	{return (pinf->flags & DCMAP_FF_Foreign);}

CGroupManager* ForeignGroupManagerCreator(GroupManagerParams const* Param)
	{return new CSimpleGroupManager(LSTRW(AliensGroupName),LSTRW(AliensGroupName));}

bool GarrisonsGroupAppove(DCMapFleetInfo const* pinf,GroupManagerParams const* Param)
	{return (pinf->type == DCMAP_FTP_Garrizon);}

bool BuildingsGroupAppove(DCMapFleetInfo const* pinf,GroupManagerParams const* Param)
{return (pinf->type == DCMAP_FTP_Buildings);}

bool QueueGroupAppove(DCMapFleetInfo const* pinf,GroupManagerParams const* Param)
{return (pinf->type == DCMAP_FTP_Queue);}

CGroupManager* GarrisonsGroupManagerCreator(GroupManagerParams const* Param)
	{return new CSimpleGroupManager(LSTRW(GarrisonsGroupName),LSTRW(GarrisonsGroupName));}

bool PlanetsGroupAppove(DCMapFleetInfo const* pinf,GroupManagerParams const* Param)
{
	return (pinf->group == DCMAP_FGRP_PlanetGroup );
}

CGroupManager* PlanetsGroupManagerCreator(GroupManagerParams const* Param)
{
	return new CSimpleGroupManager(LSTRW(PlanetsGroupName),LSTRW(PlanetsGroupName));
}


bool OwnerGroupAppove(DCMapFleetInfo const* pinf,GroupManagerParams const* Param)
{return pinf->parent.Valid();}

CGroupManager* OwnerGroupManagerCreator(GroupManagerParams const* Param)
{return new COwnerGroupManager();}

CGroupManager* AutoNameGroupManagerCreator(GroupManagerParams const* Param)
{return new CAutoNameGroupManager();}

CFleetGrouping::CFleetGrouping(void)
{
	static GroupDef AlienGroupDef;
	AlienGroupDef.childs.resize(1);
	AlienGroupDef.childs[0].Set(0,OwnerGroupAppove,OwnerGroupManagerCreator,FGI_FirstGroup);

	static GroupDef OwnerGroupDef;
	OwnerGroupDef.childs.resize(4);

	OwnerGroupDef.childs[0].Set(0,PlanetsGroupAppove,PlanetsGroupManagerCreator,FGI_Planets,FICO_MyPlanet);
	OwnerGroupDef.childs[1].Set(0,GarrisonsGroupAppove,GarrisonsGroupManagerCreator,FGI_Garrison,FICO_Garrison);
	OwnerGroupDef.childs[2].Set(0,CNameDivGroupManager::Appove,CNameDivGroupManager::Create,FGI_Regular,FICO_Group_My);
	OwnerGroupDef.childs[3].Set(0,CAutoNameGroupManager::Appove,CAutoNameGroupManager::Create,FGI_Regular,FICO_Group_My);

	m_GroupDef.childs.resize(4);

	// drop this fleets
	m_GroupDef.childs[0].Set(0,BuildingsGroupAppove,0);
	m_GroupDef.childs[1].Set(0,QueueGroupAppove,0);

	m_GroupDef.childs[2].Set(&AlienGroupDef,ForeignGroupAppove,ForeignGroupManagerCreator,FGI_LastGroup,FICO_Group);
	m_GroupDef.childs[3].Set(&OwnerGroupDef,OwnerGroupAppove,OwnerGroupManagerCreator);


	m_Root=0;
}

CFleetGrouping::~CFleetGrouping(void)
{
	if(m_Root)delete m_Root;
}

bool DCMAPAPI CFleetGrouping::Init( IDCMapCore* core,IDCMapWorkspace* workspace )
{
    m_pCore = core;
    if(!Fleets.init(core,"fleets"))return false;
    if(!Planets.init(core,"planets"))return false;
    if(!Players.init(core,"players"))return false;
	if(!Units.init(core,"buildings"))return false;
    return true;
}

bool CFleetGrouping::Initialize()
{
	if(m_Root)return true;

	dcmapDBREF RootGroup = *Fleets->GetRootGroup();
	if(!RootGroup.Valid())
	{
		Fleets->Insert();
		Fleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_RootGroup;
		Fleets->Post(&RootGroup);
	}

	IDCMapFleetsDataStoragePtr pFleets(Fleets);
	m_Root= new CRootGroupManager;
	m_Root->Initialize(pFleets,RootGroup,&m_GroupDef);

	return true;
}

bool DCMAPAPI CFleetGrouping::ProcessFleets()
{
    DCMapScopeBlockUpdates sb(m_pCore);
	PreProcessFleets();

    // get or create root group
    // get all the ungroupped
    vector<dcmapDBREF> Ungr;
	vector<dcmapDBREF> Groups;
    if(Fleets->SelectAll())
    do
    {
        DCMapFleetInfo const* fi = Fleets->GetData();

		if(fi->flags & DCMAP_FF_Garrison)
		{
			int i=0;
		}

		if(fi->flags& DCMAP_FF_Outdated)
		{
			if((fi->group == DCMAP_FGRP_Groupped) || true)
				Ungr.push_back(fi->id);	
			continue;
		}
		switch(fi->group)
		{
		case DCMAP_FGRP_Ungroupped:
			Ungr.push_back(fi->id);
			break;
		case DCMAP_FGRP_Groupped:
		case DCMAP_FGRP_UserGroup:
		case DCMAP_FGRP_RootGroup:
			break;
		default:
			Groups.push_back(fi->id);
			break;
		}
    }
    while(Fleets->Next());

	if(Ungr.empty())return true;

	Initialize();

    _foreach(vector<dcmapDBREF>,Ungr,it)
	{
		if(Fleets->SelectID((*it)))
		{
			DCMapFleetInfo fi = *Fleets->GetData();
			ProcessFleet(&fi);
		}
	}
	_foreach(vector<dcmapDBREF>,Groups,it)
	{
		if(Fleets->SelectID(*it))
		{
			if(!Fleets->GroupSize(*it))
			{
				Fleets->Delete();
			}
		}
	}
    return true;
}

bool DCMAPAPI CFleetGrouping::UpdateFleetGroups(dcmapDBREF const& ref)
{
	if(!Fleets->SelectID(ref))return false;
	DCMapScopeBlockUpdates sb(m_pCore);

	DCMapFleetInfo fi = *Fleets->GetData();

	fi.group = DCMAP_FGRP_Ungroupped;
	fi.parentgroup.Invalidate();
	fi.parentgroup2.Invalidate();
	fi.parentgroup3.Invalidate();
	wcsncpy(fi.caption,fi.name,DCMapFleetInfo::caption_len);

	Fleets->Update();
	Fleets->SetData(&fi);
	Fleets->Post();

	Initialize();

	return ProcessFleet(&fi);
}

bool CFleetGrouping::ProcessFleet(DCMapFleetInfo* pinf)
{
	if(pinf->flags& DCMAP_FF_Outdated || pinf->flags& DCMAP_FF_Deleted)
	{
		Fleets->SelectID(pinf->id);
		Fleets->Update();
		Fleets[DCMDB_FLEET_PARENTGROUP1] = dcmapDBREF();
		Fleets[DCMDB_FLEET_PARENTGROUP2] = dcmapDBREF();
		Fleets[DCMDB_FLEET_PARENTGROUP3] = dcmapDBREF();
		Fleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_Ungroupped;
		Fleets->Post();
		return true;
	}
    if(!Planets->RetrieveByCoords(pinf->x,pinf->y))return false;
    dcmapDBREF PlanetGroup = Planets[DCMDB_PLANET_FLEETS];

    if(!Fleets->SelectID(PlanetGroup)) // create planet group & link it to planet
    {
		DCMapFleetInfo ppinf;
		Fleets->FillDefaults(&ppinf);
		ppinf.group = DCMAP_FGRP_PlanetGroup;
		ppinf.x = pinf->x;
		ppinf.y = pinf->y;
		ppinf.parent = Planets[DCMDB_PLANET_OWNER];
		ppinf.list_image = FICO_MyPlanet;
		ppinf.type = DCMAP_FTP_Group;

		wstring name = boost::str(wformat(LSTRW(PlanetGroupNameFormat)) 
			% pinf->x % pinf->y % (dcmapWCSTR)Planets[DCMDB_PLANET_CAPTION]);

		wstring caption = boost::str(wformat(LSTRW(PlanetGroupCaptionFormat)) 
			% pinf->x % pinf->y % (dcmapWCSTR)Planets[DCMDB_PLANET_CAPTION]);

		wcsncpy(ppinf.name,name.c_str(),DCMapFleetInfo::name_len);
		wcsncpy(ppinf.caption,caption.c_str(),DCMapFleetInfo::caption_len);

		if(Planets[DCMDB_PLANET_MY] && Players->SelectID(ppinf.parent))
		{
			m_Root->Process(&ppinf);
		}

		Fleets->Insert();
		Fleets->SetData(&ppinf);
        if(!Fleets->Post(&PlanetGroup))return false;

        Planets->Update();
        Planets[DCMDB_PLANET_FLEETS] = PlanetGroup;
        if(!Planets->Post())return false;

    }       
    pinf->parentgroup2 = PlanetGroup; // group2 is planet group

	if(pinf->flags & DCMAP_FF_Foreign)
	{
		if(pinf->tta)
		{
			pinf->list_image=FICO_Alien_InTransit;
			pinf->sortindex=FGI_Alien;
		}
		else
		{
			switch(pinf->dip)
			{
			case DCMAP_DIP_ENEMIES:
				pinf->list_image=FICO_Alien_Enemy;
				pinf->sortindex=FGI_Alien_Enemy;
				break;
			case DCMAP_DIP_NEUTRALS:
				pinf->list_image=FICO_Alien_Neutrals;
				pinf->sortindex=FGI_Alien_Neutrals;
				break;
			case DCMAP_DIP_MULT:
			case DCMAP_DIP_LORD:
			case DCMAP_DIP_VASSAL:
			case DCMAP_DIP_ALLY:
				pinf->list_image=FICO_Alien_Ally;
				pinf->sortindex=FGI_Alien_Ally;
				break;
			default:
				pinf->list_image=FICO_Alien_Undefined;
				pinf->sortindex=FGI_Alien_Undefined;
				break;
			}
		}
	}
	else if(pinf->type == DCMAP_FTP_Garrizon)
	{
		pinf->list_image = FICO_Garrison;
		pinf->sortindex = FGI_Garrison;
	}
	else if(pinf->type == DCMAP_FTP_Buildings)
	{
		pinf->list_image = FICO_Buildings;
		pinf->sortindex = FGI_Buildings;
	}
	else if(pinf->type == DCMAP_FTP_Queue)
	{
		pinf->list_image = FICO_Queue;
		pinf->sortindex = FGI_Queue;
	}
	else if(pinf->tta)
	{
		pinf->list_image=FICO_My_InTransit;
		pinf->sortindex=FGI_Transit;
	}
	else
	{
		pinf->list_image=FICO_My;
		pinf->sortindex=FGI_Regular;
	}

	m_Root->Process(pinf);
	pinf->group = DCMAP_FGRP_Groupped;

	if(!Fleets->SelectID(pinf->id))return false;
	Fleets->Update();
	Fleets->SetData(pinf);
	Fleets->Post();
    return true;
}


int DCMAPAPI CFleetGrouping::OnMessage( int Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender )
{
	switch(Msg)
	{
	case DCMM_EXECUTE:
		{
			m_pCore->LockDataUpdate();
			ProcessFleets();
			m_pCore->UnlockDataUpdate();
			return 0;
		}
		break;
	default:
		break;
	}
	return 0;
}

void CFleetGrouping::PreProcessFleets()
{
	DCMapScopeBlockUpdates sb(m_pCore);

	typedef vector<dcmapDBREF> VecRef;
	VecRef vecHist,vecOutdated,vecInvalid,vecOutdatedUnits;

	// process outdated fleets
	if(Fleets->SelectAll())
		do
		{
			int group = Fleets[DCMDB_FLEET_GROUP];
			switch(group)
			{
			case DCMAP_FGRP_Ungroupped:
			case DCMAP_FGRP_Groupped:
				break;
			default:
				continue; // do not process groups
				break;
			}	
			
			dcmapDBREF flr = *Fleets->GetID();
			if(!Players->SelectID(Fleets[DCMDB_FLEET_LOADER])) // invalid fleet
			{
				vecInvalid.push_back(flr);
				continue;
			}
			int turn = Players[DCMDB_PLAYER_TURN];
			int flturn = Fleets[DCMDB_FLEET_TURN];

			if(Fleets[DCMDB_FLEET_FOREIGN])
			{
				if(flturn < turn) // mark as history fleet
				{
					vecHist.push_back(flr);
					continue;
				}
			}
			else
			{
				if(flturn < turn) // mark as history fleet
				{
					vecOutdated.push_back(flr);
					continue;
				}
			}
			if(Units->SelectByParentID(flr))do
			{
				if(Units[DCMDB_BUILDING_OUTDATED])vecOutdatedUnits.push_back(*Units->GetID());
				else if((int)Units[DCMDB_BUILDING_TURN] < turn)
				{
					vecOutdatedUnits.push_back(*Units->GetID());
				}
			}
			while(Units->Next());
		}
		while(Fleets->Next());

		_foreach(VecRef,vecInvalid,it)
		{	
			if(Fleets->SelectID(*it))Fleets->Delete();
		}

		_foreach(VecRef,vecOutdatedUnits,it)
		{	
			if(Units->SelectID(*it))
			{
				Units->Update();
				Units[DCMDB_BUILDING_OUTDATED] = true;
				Units->Post();
			}
		}

		_foreach(VecRef,vecHist,it)
		{	
			if(Fleets->SelectID(*it))
			{
				Fleets->Update();
				//Fleets[DCMDB_FLEET_HISTORY] = true;
				Fleets[DCMDB_FLEET_OUTDATED] = true;
				Fleets[DCMDB_FLEET_PARENTGROUP1] = dcmapDBREF();
				Fleets[DCMDB_FLEET_PARENTGROUP2] = dcmapDBREF();
				Fleets[DCMDB_FLEET_PARENTGROUP3] = dcmapDBREF();
				Fleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_Ungroupped;
				Fleets->Post();
			}
		}	
		_foreach(VecRef,vecOutdated,it)
		{	
			if(Fleets->SelectID(*it))
			{
				Fleets->Update();
				//Fleets[DCMDB_FLEET_HISTORY] = true;
				Fleets[DCMDB_FLEET_OUTDATED] = true;
				Fleets[DCMDB_FLEET_PARENTGROUP1] = dcmapDBREF();
				Fleets[DCMDB_FLEET_PARENTGROUP2] = dcmapDBREF();
				Fleets[DCMDB_FLEET_PARENTGROUP3] = dcmapDBREF();
				Fleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_Ungroupped;
				Fleets->Post();
			}
		}
}