//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include "FleetSidebar.h"
#include "Utils.h"
#include "sDialogs.hpp"
#include <list>
#include "Interface.h"
#include "IconsListMan.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sTreeView"
#pragma link "sListView"
#pragma link "sPanel"
#pragma link "sSplitter"
#pragma link "sLabel"
#pragma link "sButton"
#pragma link "sBevel"
#pragma link "acAlphaImageList"
#pragma resource "*.dfm"

#include <map>

class TItemData;

int ExpandGroup(TFleetSideFrame* Frame,
                 TsTreeView* pView,
		 RestoreState* rest,
                 DCMapDataReference const& Ref,
                 TTreeNode* Node,
                 TItemData* Parent,
                 bool ByType);

TFleetSideFrame *FleetSideFrame;

static CFramePanelWrapper<TFleetSideFrame> sWrap("PFormPanelItem","FleetInfo");
static CFramePanelWrapper<TFleetSideFrame> sFrameBarWrap("PFrameBar","FleetBar");

//---------------------------------------------------------------------------

bool TFleetSideFrame::OnInit()
{
	if(!Fleets.init(m_pCore))return false;
	if(!Units.init(m_pCore))return false;
	if(!Planets.init(m_pCore))return false;
	if(!Projects.init(m_pCore))return false;
	if(!Players.init(m_pCore))return false;
	if(!Icons.init(m_pCore))return false;
	if(!Components.init(m_pCore))return false;
	if(!ProjectActs.init(m_pCore))return false;
	if(!ActReq.init(m_pCore))return false;
	if(!UnitTags.init(m_pCore))return false;

	m_pPlanetMan = m_pWorkspace->GetServiceTyped<IDCMapPlanetManager>("PlanetManager");
	if(!m_pPlanetMan)return false;

	m_pPlanetContextMan = m_pWorkspace->GetServiceTyped<IDCMapPlanetContextManager>("PlanetContextManager");
	if(!m_pPlanetContextMan) return false;

	m_pFleetMan = m_pWorkspace->GetServiceTyped<IDCMapFleetManager>("FleetManager");
	if(!m_pFleetMan)return false;

	m_pCalc = m_pCore->GetServiceTyped<IDCMapPlanetCalculator>("PlanetCalculator");
	if(!m_pCalc)return false;

	m_pIconsList = m_pCore->GetServiceTyped<TIconsList>("VCLIconsList");
	if(!m_pIconsList)return false;



	m_pPlayerContext = g_CoreUIManager.GetContextEx("players_record");
	m_pPlanetContext = g_CoreUIManager.GetContextEx("planets_record");


	FleetNameLabel->Caption = "";
	FleetSpeedLabel->Caption = "";
	FleetRangeLabel->Caption = "";
	FleetOwnerLabel->Caption = "";
	FleetInfoLabel->Caption = "";

	SetupColors();

	if(m_pPanelInterface->ModuleClass == "PFrameBar")
		DragBar->Visible = true;

	return true;
}

class TItemData
{
public:
	dcmapDBREF Ref;
	dcmapDBREF RefOwner;
	TTreeNode* Node;
	TFleetSideFrame* Frame;
	TItemData* Parent;
	TsTreeView* pView;
	bool ModGroup;
	bool DataExpanded;
	int  FleetType;
	bool ChildsUseCaptions;
	int IconIndex;
	bool Foreign;
	IDCMapCore* m_pCore;
	IDCMapWorkspace* m_pWorkspace;
        int NodeType;
public:
	DCMapDataReference const* Reference(){return &Ref;}
	TItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* _Item,TItemData* _Parent):
		Node(_Item),
		Ref(_Ref),
		Frame(_Frame),
		Parent(_Parent),
		DataExpanded(false),
		ModGroup(false),
		FleetType(0),
		IconIndex(-1),
                ChildsUseCaptions(true),
                Foreign(false),
                NodeType(NodeNone)
		{
			pView = (TsTreeView*)_Item->TreeView;
			Node->Data = this;
			Node->SelectedIndex=-1;
			m_pCore = Frame->m_pCore;
			m_pWorkspace = Frame->m_pWorkspace;
		}
	virtual bool Init()
	{
		Node->SelectedIndex=Node->ImageIndex;
		return true;
	}

	virtual bool Select(){return true;};
	virtual bool Deselect(){return true;};

	template<class T> T* GetAs()
	{
		T* p = dynamic_cast<T*>(this);
		if(p)return p;
		if(Parent)return Parent->GetAs<T>();
		return 0;
	};

	virtual void DblClick()
	{
	};

	virtual void Click()
	{
	};

	virtual void Edited(String& str)
	{

	};

	virtual bool OnContextPopup(TMenuItem* pMenu)
	{
		 return false;
	};

	virtual bool BeginEdit(String& str)
	{
    	return false;
	};
	virtual bool CanEdit()
	{
		return false;
	};

	virtual bool AutoExpand()
	{
		return false;
	};

	virtual String GetHint()
	{
		return String();;
	};

	bool Expanded()
	{
		return Node->Expanded;
	};
    
	bool Expandeble()
	{
		return Node->HasChildren;
	};
	virtual bool CanCollapse(){return true;};
	virtual bool CanExpand(){return Expandeble();};

	virtual int Expand(RestoreState* rest=0){return 0;};

	virtual bool HaveProperties(){return false;};
	virtual void ShowProperties(){};

	void Toggle()
	{
		if(!Node->Expanded)
		{
			if(Expand())Node->Expand(false);
		}
		else Node->Collapse(true);
	};

	virtual bool CanSetIcon(){return false;};
	virtual void SetIcon(dcmapDBREF IconRef)
	{
	}
};

class TPlanetItemData  : public TItemData
{
public:
	int x,y;
public:
	TPlanetItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TItemData(_Frame,_Ref,Item,_Parent)
	{
    	ChildsUseCaptions = false;

		if(Frame->Planets->SelectID(Ref))
		{

                        x = Frame->Planets[DCMDB_PLANET_X];
                        y = Frame->Planets[DCMDB_PLANET_Y];
			IconIndex = Frame->m_pIconsList->IconIndex(Frame->Planets[DCMDB_PLANET_ICON1]);

			Item->Text = Frame->Planets->GetDisplayName();
                        if(Frame->Planets[DCMDB_PLANET_MY])
                        {
                            Item->ImageIndex = FICO_MyPlanet;
						 }
                        else
                        {
                            Item->ImageIndex = FICO_AlienPlanet;
                        }
		}
	}
        bool HaveProperties(){return true;};
        void ShowProperties()
        {
                Frame->core()->GetCoreUI()->OpenListTableEdit("planets",&Ref);
        };
        void DblClick()
	{
             ShowProperties();
	};
		bool CanCollapse(){return false;};

        virtual String GetHint()
	{
            return Frame->m_pPlanetContextMan->GetCellHint(x,y);
	};
};

class TUnitItemData : public TItemData
{
public:
	int UnitWeight;
	int UnitCarapace;
	int UnitHP;
	String UnitName;
	int Main;
	int Sec;
	dcmapDBREF RefFleet;
	dcmapDBREF RefProject;
	dcmapDBREF RefComponent;
	float hp;
public:
	TUnitItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TItemData(_Frame,_Ref,Item,_Parent)
	{
		UnitWeight = Frame->Units[DCMDB_BUILDING_WEIGHT];
		UnitCarapace = Frame->Units[DCMDB_BUILDING_CARAPACE];
		UnitHP = Frame->Units[DCMDB_BUILDING_HP];

		RefProject = Frame->Units[DCMDB_BUILDING_PROJECT];
		RefFleet = Frame->Units[DCMDB_BUILDING_PARENT];
		RefOwner = Frame->Fleets[DCMDB_FLEET_OWNER];

     		int BuildingID=0;

		if(Frame->Projects->SelectID(RefProject))
		{
			UnitName = Frame->Projects[DCMDB_PROJECT_NAME];
			Main = Frame->Projects[DCMDB_PROJECT_SUPPORT_MAIN];
			Sec = Frame->Projects[DCMDB_PROJECT_SUPPORT_SECOND];
			BuildingID = Frame->Projects[DCMDB_PROJECT_GAMEID];
                        RefComponent = Frame->Projects[DCMDB_PROJECT_CHASSIS];
		}
		else
		{
                        if(Frame->Components->SelectGameID(Frame->Units[DCMDB_BUILDING_CARAPACE]))
                        {
                                RefComponent = *Frame->Components->GetID();
                        }
		}
                if(Frame->Components->SelectID(RefComponent))
                {
                	if(UnitName.IsEmpty())
				UnitName = Frame->Components[DCMDB_COMPONENT_NAME];

			Item->ImageIndex =  InterfaceModule->GetFleetImage(Frame->Components[DCMDB_COMPONENT_ICON_NAME]);
                }
                if(UnitName.IsEmpty())
			UnitName = g_pLocalStrings->GetSectionStringW(("c"+Frame->Units[DCMDB_BUILDING_CARAPACE].AnsiStr()).c_str(),"Components");
                if(UnitName.IsEmpty())
                        UnitName = L"Unknown carapace "+Frame->Units[DCMDB_BUILDING_CARAPACE].Str();

		Item->Text = UnitName;
	}
	virtual bool Select()
	{
		Frame->UpdateFleetInfo(Parent->Ref,&Ref);
		return true;
	};
        void ShowProperties()
        {
                Frame->core()->GetCoreUI()->OpenListTableEdit("buildings",&Ref);
        };
};

class TUnitGroupItemData : public TUnitItemData
{
public:
	int NumUnits;
	bool Foreign;
public:
	TUnitGroupItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TUnitItemData(_Frame,_Ref,Item,_Parent)
	{
		NumUnits=0;
			Foreign = _Parent->Foreign;

		if(Item->ImageIndex<0)
		{
			if(Foreign)
			{
				Item->ImageIndex = FICO_AlienUnit;
				Foreign = true;
			}
			else
			{
				Item->ImageIndex = FICO_MyUnit;
				Foreign = false;
			}
		}

		Ref.data = RefFleet.data + RefProject.data+UnitCarapace+UnitHP;
	}
        void AddSub(TUnitItemData* pData)
        {
			 //Ref.data += pData->Ref.data;
             DataExpanded=true;
             NumUnits++;
        }
        void Finished()
        {
             Node->Text += L" (x"+IntToStr(NumUnits)+L")";
        }
};

class TForeignUnitItemData : public TUnitItemData
{
public:
	TForeignUnitItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TUnitItemData(_Frame,_Ref,Item,_Parent)
	{
		 if(Item->ImageIndex<0)Item->ImageIndex = FICO_AlienUnit;

		 Foreign=true;
	}

	virtual String GetHint()
	{
		TVarRec rec[] = {TVarRec(UnitName),TVarRec(UnitWeight)};
		return String::Format(LSTRW(ForeignUnitHint),rec,sizeof(rec)/sizeof(TVarRec));
	};
};

class TQueueItemData : public TUnitItemData
{
public:
	TQueueItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TUnitItemData(_Frame,_Ref,Item,_Parent)
	{
		 if(Item->ImageIndex<0)Item->ImageIndex = FICO_MyUnit;

		 Item->Text += L" ("+Frame->Units[DCMDB_BUILDING_READY_IN].Str()+L")";
	}
	virtual String GetHint()
	{
	};
};

class TBuildingItemData : public TUnitItemData
{
public:
	TBuildingItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TUnitItemData(_Frame,_Ref,Item,_Parent)
	{
		 if(Item->ImageIndex<0)Item->ImageIndex = FICO_MyUnit;
	}
	virtual String GetHint()
	{
			DCMapVarValue val;
			return dcmapProcessFormatedStringDB(LSTRW(MyUnitHint),Frame->Units,&Ref,val);
	};
};

class TMyUnitItemData : public TUnitItemData
{
public:
	TMyUnitItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TUnitItemData(_Frame,_Ref,Item,_Parent)
	{
		 if(Item->ImageIndex<0)Item->ImageIndex = FICO_MyUnit;

		if(Frame->ActReq->SelectByParentID(Ref))
		do
		{
			 if(Frame->ActReq[DCMDB_ACTREQ_CANCELED])continue;
			 int actid = Frame->ActReq[DCMDB_ACTREQ_ACTID];

			 if(actid==3) Node->StateIndex = FICO_STATUS_Loading; // unload action
		}
		while(Frame->ActReq->Next());
	}

	virtual String GetHint()
	{
			DCMapVarValue val;
			return dcmapProcessFormatedStringDB(LSTRW(MyUnitHint),Frame->Units,&Ref,val);
	};
};

class TTaggedUnitItemData : public TMyUnitItemData
{
public:
	int TagType;
	int Amount;
	bool Unpackable;
	String SubCaption;
	int SubIcon;
        bool Loading;
        dcmapDBREF LoadFleetRef;
public:
	TTaggedUnitItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TMyUnitItemData(_Frame,_Ref,Item,_Parent)
	{
		SubIcon = -1;
		Loading = false;

		if(Frame->UnitTags->SelectByParentID(Ref))
		{
			Loading = Frame->UnitTags[DCMDB_UNITTAG_LOADING];
			LoadFleetRef = Frame->UnitTags[DCMDB_UNITTAG_LOAD_FLEET];

			if(Loading) Node->StateIndex = FICO_STATUS_Loading;

			TagType = Frame->UnitTags[DCMDB_UNITTAG_TYPE];
			Amount  = Frame->UnitTags[DCMDB_UNITTAG_AMOUNT];
			if(TagType == DCMAP_UTT_RESOURCE)
			{
				int resid = Frame->UnitTags[DCMDB_UNITTAG_ITEMID];
				SubIcon = resid;
				SubCaption = FloatToStrF(Amount,ffNumber,7,0);

                                Node->Text += L" ("+FloatToStrF(Amount,ffNumber,7,0)+L" ";

                                switch(resid)
                                {
                                	case 0: Node->Text += L"O";break;
                                	case 1: Node->Text += L"E";break;
                                	case 2: Node->Text += L"M";break;
                                	case 3: Node->Text += L"C";break;
                        	}
                                Node->Text += ")";

				Unpackable = !Frame->UnitTags[DCMDB_UNITTAG_UNWRAPPED];
			}
			else if(TagType == DCMAP_UTT_UNIT)
			{
				if(Frame->Projects->SelectID(Frame->UnitTags[DCMDB_UNITTAG_PROJECT]))
				{
					SubCaption = Frame->Projects[DCMDB_PROJECT_NAME];
                                        Node->Text += L" ("+SubCaption+")";
				}
				if(Frame->Components->SelectID(Frame->UnitTags[DCMDB_UNITTAG_CHASSIS]))
				{
					SubIcon =  InterfaceModule->GetFleetImage(Frame->Components[DCMDB_COMPONENT_ICON_NAME]);
				}
				Unpackable = !Frame->UnitTags[DCMDB_UNITTAG_UNWRAPPED];
			}
			else if(TagType == DCMAP_UTT_PART)
			{
				if(Frame->Components->SelectID(Frame->UnitTags[DCMDB_UNITTAG_CHASSIS]))
				{
                                	String Component = Frame->Components[DCMDB_COMPONENT_NAME];
                                        String Count = IntToStr(Amount);
                                        SubCaption = Component+ L" ["+Count+"] ";;
                                        Unpackable = false;

                                        Node->Text += L" (x"+Count+" "+Component+")";

				}
				SubIcon = FICO_Part;
			}
		}

		Item->HasChildren = true;
	}

	int Expand(RestoreState* rest=0);
	bool CanUnpack(){return Unpackable;}
};

class TUnitTagItemData : public TItemData
{
public:
	dcmapDBREF UnitRef;

	TUnitTagItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TItemData(_Frame,_Ref,Item,_Parent)
	{
		TTaggedUnitItemData* pTagged = dynamic_cast<TTaggedUnitItemData*>(Parent);
		if(!pTagged)return;

		Node->ImageIndex = pTagged->SubIcon;
		Node->Text = pTagged->SubCaption;
		UnitRef = pTagged->Ref;
	}
	bool CanUnpack()
	{
		TTaggedUnitItemData* pTagged = dynamic_cast<TTaggedUnitItemData*>(Parent);
		if(!pTagged)return false;
		return pTagged->CanUnpack();
	}
};

int TTaggedUnitItemData::Expand(RestoreState* rest)
{
	if(DataExpanded)return 1;

	if(Frame->UnitTags->SelectByParentID(Ref))
	{
		TUnitTagItemData* pdata=0;

		TTreeNode* Sub = pView->Items->AddChild(Node,"");

		pdata = new TUnitTagItemData(Frame,*Frame->UnitTags->GetID(),Sub,this);
		pdata->Init();
		if(rest && pdata->Ref == rest->top_ref) rest->top_item = pdata;
	}

	DataExpanded = true;
	return 1;
};

class TBaseFleetItemData : public TItemData
{
public:
	String FleetName;
	String Planet;
	String SourcePlanet;
	int x,y,from_x,from_y;
	bool FlySent;
	bool FlyReady;
	int TTA;
	int ShipCount;
	int CargoCount;
	int TransportCapacity;
	int Weight;
	float FlySpeed;
	float FlyRange;
	float Stealth;
	String Owner;
	bool Hidden;

public:
	TBaseFleetItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TItemData(_Frame,_Ref,Item,_Parent)
	{
		FleetName = Frame->Fleets[DCMDB_FLEET_NAME];
		String FleetCaption = Frame->Fleets[DCMDB_FLEET_CAPTION];

		RefOwner = Frame->Fleets[DCMDB_FLEET_OWNER];

		if(_Parent && _Parent->ChildsUseCaptions)
			Item->Text = FleetCaption.IsEmpty()? FleetName : FleetCaption;
		else
			Item->Text = FleetName;

		Item->HasChildren = Frame->Fleets[DCMDB_FLEET_SHIP_COUNT];
		FleetType = Frame->Fleets[DCMDB_FLEET_TYPE];

		Item->ImageIndex = Frame->Fleets[DCMDB_FLEET_LIST_IMAGE];

		ShipCount = Frame->Fleets[DCMDB_FLEET_SHIP_COUNT];
		Weight = Frame->Fleets[DCMDB_FLEET_WEIGHT];
		Owner = Frame->Fleets[DCMDB_FLEET_OWNER];
		CargoCount = Frame->Fleets[DCMDB_FLEET_CARGO_COUNT];
		TransportCapacity = Frame->Fleets[DCMDB_FLEET_TRANSPORT_CAPACITY];

		FlySpeed = Frame->Fleets[DCMDB_FLEET_FLY_SPEED];
		FlyRange = Frame->Fleets[DCMDB_FLEET_FLY_RANGE];
		Stealth = Frame->Fleets[DCMDB_FLEET_STEALTH];

		x = Frame->Fleets[DCMDB_FLEET_X];
		y = Frame->Fleets[DCMDB_FLEET_Y];
		from_x = Frame->Fleets[DCMDB_FLEET_FROM_X];
		from_y = Frame->Fleets[DCMDB_FLEET_FROM_Y];

		if(Frame->Fleets[DCMDB_FLEET_SCRIPT_ACTIVE])
		{
			if(Frame->Fleets[DCMDB_FLEET_SCRIPT_OK])
				Node->StateIndex = FICO_STATUS_ScriptOk;
			else
				Node->StateIndex = FICO_STATUS_ScriptFail;
		}

		DCMapFleetInfo const* finfo =  Frame->Fleets->GetData();

		Hidden = Frame->Fleets[DCMDB_FLEET_HIDDEN];
		//if(Hidden && Node->StateIndex == FICO_STATUS_None)
		if(Hidden)
			Node->StateIndex = FICO_STATUS_Hidden;


		IconIndex = Frame->m_pIconsList->IconIndex(Frame->Fleets[DCMDB_FLEET_ICON]);

		TTA = 0;
		FlySent = 0;
		FlyReady = 0;

		if(x && y)
		{
			if(Frame->Planets->SelectCoord(x,y))
			{
				 Planet = Frame->Planets->GetDisplayName();
			}
		}
	}

	virtual void DblClick()
	{
		TItemData::DblClick();
		if(Frame->Fleets->SelectID(Ref))
		{
			if(x && y)
			{
				Frame->m_pPlanetMan->SetCurrentPlanet(0,x,y,2);
			}
		}
	};
	virtual bool Select()
	{
		Frame->UpdateFleetInfo(Ref);
		return true;
	};

	virtual String GetHint()
	{
		TVarRec rec[] = {TVarRec(FleetName)};
		return String::Format(LSTRW(FleetHint),rec,sizeof(rec)/sizeof(TVarRec));
	};

        void ShowProperties()
        {
                Frame->core()->GetCoreUI()->OpenListTableEdit("fleets",&Ref);
		};

        bool CanSetIcon(){return true;}

	void SetIcon(dcmapDBREF IconRef)
	{
		if(Frame->m_pFleetMan->SetFleetIcon(Ref,IconRef));
	}
};

class TFleetItemData : public TBaseFleetItemData
{
public:

public:
	TFleetItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TBaseFleetItemData(_Frame,_Ref,Item,_Parent)
	{
	}
	struct SortSetItem
	{
		String Name;
		dcmapDBREF Prj;
		float hp;
		friend operator < (SortSetItem const& v1,SortSetItem const& v2)
		{
			int c = v1.Name.Compare(v2.Name);
			if(!c)
			{
			   if(v1.Prj.id == v2.Prj.id) return v1.hp < v2.hp;
			   return v1.Prj.id < v2.Prj.id;
			}
			else return c<0;
		};
	};

	virtual int Expand(RestoreState* rest=0)
	{
		if(DataExpanded)return 1;

		int Count=0;

		Frame->Fleets->SelectID(Ref);
		bool foreign = Frame->Fleets[DCMDB_FLEET_FOREIGN];

				typedef std::vector<dcmapDBREF> VecRef;
				typedef std::map<SortSetItem,VecRef> MapSort;
				MapSort mapSort;

		if(Frame->Units->SelectByParentID(Ref))
		do
		{
			if(Frame->Units[DCMDB_BUILDING_OUTDATED])continue;
			if(Frame->Units[DCMDB_BUILDING_DISBANDED])continue;

			Count++;
						TUnitItemData* pdata=0;

			if(Frame->Units[DCMDB_BUILDING_QUEUED])
						{
							TTreeNode* Sub = pView->Items->AddChild(Node,"");

						   if(Frame->Units[DCMDB_BUILDING_TAGGED])
								pdata = new TTaggedUnitItemData(Frame,*Frame->Units->GetID(),Sub,this);
						   else
								pdata = new TQueueItemData(Frame,*Frame->Units->GetID(),Sub,this);

							pdata->Init();

							if(rest && pdata->Ref == rest->top_ref) rest->top_item = pdata;

			}
						else
						{
							SortSetItem item;
							item.Prj = Frame->Units[DCMDB_BUILDING_PROJECT];
							item.hp = Frame->Units[DCMDB_BUILDING_HP];

							if(Frame->Projects->SelectID(item.Prj))
								item.Name = Frame->Projects[DCMDB_PROJECT_CAPTION];

							mapSort[item].push_back(*Frame->Units->GetID());
						}
		}
		while(Frame->Units->Next());

		_foreach(MapSort,mapSort,it)
		{
				 int n = it->second.size();
			 if(!n || !Frame->Units->SelectID(it->second[0]))continue;

			 if(n<=2)
			 {
							_foreach(VecRef,it->second,ref)
				{
								  if(!Frame->Units->SelectID(*ref))continue;

								  TTreeNode* Sub = pView->Items->AddChild(Node,"");
								  TUnitItemData* pdata;

				  if(Frame->Units[DCMDB_BUILDING_PLANETARY])pdata = new TBuildingItemData(Frame,*ref,Sub,this);
				  else if(foreign)pdata = new TForeignUnitItemData(Frame,*ref,Sub,this);
				  else
				  {
					  if(Frame->Units[DCMDB_BUILDING_TAGGED])
						  pdata = new TTaggedUnitItemData(Frame,*ref,Sub,this);
					  else
						  pdata = new TMyUnitItemData(Frame,*ref,Sub,this);
				  }

				  pdata->Init();
				  if(rest && pdata->Ref == rest->top_ref) rest->top_item = pdata;
								}
			 }
			 else // create units group
			 {
								TTreeNode* Sub = pView->Items->AddChild(Node,"");
                                TUnitItemData* pdata;

				TUnitGroupItemData* gdata = new TUnitGroupItemData(Frame,*Frame->Units->GetID(),Sub,this);
				gdata->Init();
				_foreach(VecRef,it->second,ref)
				{
					 if(Frame->Units->SelectID(*ref))
					 {
						TTreeNode* UnitSub = pView->Items->AddChild(Sub,"");

						if(Frame->Units[DCMDB_BUILDING_PLANETARY])pdata = new TBuildingItemData(Frame,*ref,UnitSub,gdata);
						else if(foreign)pdata = new TForeignUnitItemData(Frame,*ref,UnitSub,gdata);
						else
						{
							if(Frame->Units[DCMDB_BUILDING_TAGGED])
								pdata = new TTaggedUnitItemData(Frame,*ref,UnitSub,gdata);
							else
								pdata = new TMyUnitItemData(Frame,*ref,UnitSub,gdata);
						}

						pdata->Init();
						gdata->AddSub(pdata);

						if(rest && pdata->Ref == rest->top_ref) rest->top_item = pdata;
					 }
				}
				gdata->Finished();


				if(rest)
				{
				   ExpandedSet::iterator it =  rest->exp.find(gdata->Ref);
				   if(it != rest->exp.end())
				   {
						Sub->Expand(false);
				   }
				   if(gdata->Ref == rest->top_ref) rest->top_item = gdata;
				}
			 }
		}
		DataExpanded = true;
		return Count;
	};
};


class TBaseMyFleetItemData : public TFleetItemData
{
public:
	TBaseMyFleetItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TFleetItemData(_Frame,_Ref,Item,_Parent)
	{
		TTA = Frame->Fleets[DCMDB_FLEET_TTA];
		FlySent = TTA && !(bool)Frame->Fleets[DCMDB_FLEET_IN_TRANSIT];
		FlyReady = TTA==0;
	}

	virtual bool CanEdit(){return true;};
	virtual bool BeginEdit(String& str)
	{
		str = FleetName;
		return true;
	};

	virtual void Edited(String& str)
	{
		Node->Text = str;
		Frame->m_pFleetMan->RenameFleet(Ref,str.w_str());
	};

		virtual String GetHint()
	{
		TVarRec rec[] = {TVarRec(FleetName),TVarRec(Owner),
					TVarRec(ShipCount),TVarRec(TransportCapacity),TVarRec(CargoCount),
						TVarRec(Planet)};
		return String::Format(LSTRW(MyFleetHint),rec,sizeof(rec)/sizeof(TVarRec));
	};
};

class TMyFleetItemData : public TBaseMyFleetItemData
{
public:
	int Changes;
public:
	TMyFleetItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TBaseMyFleetItemData(_Frame,_Ref,Item,_Parent)
	{
		Changes = Frame->Fleets[DCMDB_FLEET_CHANGES];
	}
	bool HaveProperties(){return true;};

        virtual String GetHint()
	{
            DCMapVarValue val;

            val[L"FleetName"] = FleetName;
            val[L"Owner"] = Owner;
            val[L"Planet"] = Planet;
            val[L"ShipCount"] = ShipCount;
            val[L"TransportCapacity"] = TransportCapacity;
            val[L"CargoCount"] = CargoCount;
            val[L"FlySpeed"] = FlySpeed;
            val[L"FlyRange"] = FlyRange;

            return dcmapProcessFormatedString(LSTRW(MyFleetHint),val,0);
	};

};


class TMyFleetInTransitItemData : public TMyFleetItemData
{
public:
	TMyFleetInTransitItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TMyFleetItemData(_Frame,_Ref,Item,_Parent)
	{
               	if(x && y)Planet = IntToStr(x)+L":"+IntToStr(y);
                if(from_x && from_y) SourcePlanet = IntToStr(x)+L":"+IntToStr(y);
	}
       	virtual String GetHint()
	{
			DCMapVarValue val;

			val->SetNamedString(L"FleetName",FleetName.w_str());
			val->SetNamedString(L"Owner",Owner.w_str());
			val->SetNamedString(L"Planet",Planet.w_str());
			val->SetNamedNumber(L"ShipCount",ShipCount);
			val->SetNamedNumber(L"TransportCapacity",TransportCapacity);
			val->SetNamedNumber(L"CargoCount",CargoCount);
			val->SetNamedNumber(L"ShipCount",ShipCount);
			val->SetNamedNumber(L"FlySpeed",FlySpeed);
			val->SetNamedNumber(L"FlyRange",FlyRange);
			val->SetNamedString(L"SourcePlanet",SourcePlanet.w_str());
			val->SetNamedString(L"Planet",Planet.w_str());
			val->SetNamedNumber(L"TTA",TTA);

			return dcmapProcessFormatedString(LSTRW(MyFleetInTransitHint),val,0);
	};
};

class TQueueFleetItemData : public TBaseMyFleetItemData
{
public:
	TQueueFleetItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TBaseMyFleetItemData(_Frame,_Ref,Item,_Parent)
	{
        	NodeType = NodeQueue;
	}
	bool CanSetIcon(){return false;};
	bool CanEdit(){return false;};
	bool HaveProperties(){return false;};
	String GetHint(){};
};

class TGarrizonFleetItemData : public TMyFleetItemData
{
public:
	TGarrizonFleetItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TMyFleetItemData(_Frame,_Ref,Item,_Parent)
	{
        	NodeType = NodeGarrizon;
	}
	bool CanSetIcon(){return false;};
	bool CanEdit(){return false;};
	bool HaveProperties(){return false;};
	String GetHint(){};
};

class TBuildingsFleetItemData : public TBaseMyFleetItemData
{
public:
	TBuildingsFleetItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TBaseMyFleetItemData(_Frame,_Ref,Item,_Parent)
	{
        	NodeType = NodeBuildings;
	}
	bool CanSetIcon(){return false;};
	virtual bool CanEdit(){return false;};
	String GetHint(){};
};


class TForeignFleetItemData : public TFleetItemData
{
public:
	TForeignFleetItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TFleetItemData(_Frame,_Ref,Item,_Parent)
	{
        	Foreign = true;
	}
	String GetHint()
	{
		TVarRec rec[] = {TVarRec(FleetName),TVarRec(Owner),
                	TVarRec(ShipCount),TVarRec(Weight),
                        TVarRec(Planet)};
		return String::Format(LSTRW(ForeignFleetHint),rec,sizeof(rec)/sizeof(TVarRec));
	};
};

class TForeignFleetInTransitItemData : public TForeignFleetItemData
{
public:
	TForeignFleetInTransitItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TForeignFleetItemData(_Frame,_Ref,Item,_Parent)
	{
        	if(x && y)Planet = IntToStr(x)+L":"+IntToStr(y);
				if(from_x && from_y) SourcePlanet = IntToStr(x)+L":"+IntToStr(y);

			Item->HasChildren = false;
	}
       	virtual String GetHint()
	{
		TVarRec rec[] = {TVarRec(FleetName),TVarRec(Owner),
                		TVarRec(ShipCount),TVarRec(Weight),
                                TVarRec(SourcePlanet),TVarRec(Planet),
                                TVarRec(TTA)};
		return String::Format(LSTRW(ForeignFleetInTransitHint),rec,sizeof(rec)/sizeof(TVarRec));
	};
};

class TFleetGroupItemData : public TBaseFleetItemData
{
public:
	TFleetGroupItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TBaseFleetItemData(_Frame,_Ref,Item,_Parent)
	{
		int grp = Frame->Fleets[DCMDB_FLEET_GROUP];
		if(grp == DCMAP_FGRP_BlockNameGroup ||
		   grp == DCMAP_FGRP_NameGroup)
		{
			ModGroup = true;
			Item->Text = Frame->Fleets[DCMDB_FLEET_CAPTION].Str()
				+ " ("+IntToStr(Frame->Fleets->GroupSize(_Ref))+")";
		}
		Item->HasChildren = true;
	}
	bool AutoExpand(){return true;};
	bool CanSetIcon(){return true;};
	bool HaveProperties(){return false;};

	virtual int Expand(RestoreState* rest=0)
	{
		if(DataExpanded)return 1;

                int Count = ExpandGroup(Frame,pView,rest,Ref,Node,this,false);

		DataExpanded = true;
		return Count;
	};

};

class TPlanetGroupItemData : public TFleetGroupItemData
{
public:
	TPlanetGroupItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TFleetGroupItemData(_Frame,_Ref,Item,_Parent)
	{
			 ChildsUseCaptions = false;

			 if(Frame->Planets->SelectCoord(x,y))
			 {
				IconIndex = Frame->m_pIconsList->IconIndex(Frame->Planets[DCMDB_PLANET_ICON1]);
			 }
	}
		bool CanSetIcon(){return false;};
		bool HaveProperties(){return true;};
		void ShowProperties()
        {
				if(Frame->Planets->SelectCoord(x,y))
				{
					Frame->core()->GetCoreUI()->OpenListTableEdit("planets",Frame->Planets->GetID());
				}
		};
		virtual bool OnContextPopup(TMenuItem* pMenu)
		{
		  if(!Frame->Planets->SelectCoord(x,y))return false;

		  Frame->m_pPlanetContext->SetMenu(pMenu);
		  IDCMapVarValue* pval =  Frame->m_pPlanetContext->ContextValue();
		  pval->Clear();
		  pval->AddDBRef(Frame->Planets->GetID(),Frame->Planets);
		  return Frame->m_pPlanetContext->Setup();
		};

        String GetHint()
        {
            return Frame->m_pPlanetContextMan->GetCellHint(x,y);
        }
};

class TPlayerGroupItemData : public TFleetGroupItemData
{
public:
	TPlayerGroupItemData(TFleetSideFrame* _Frame,DCMapDataReference const& _Ref,TTreeNode* Item,TItemData* _Parent):
		TFleetGroupItemData(_Frame,_Ref,Item,_Parent)
	{

	}
	bool CanSetIcon(){return true;};

   	bool HaveProperties(){return true;};
	void ShowProperties()
	{
		  Frame->core()->GetCoreUI()->OpenListTableEdit("players",&RefOwner);
	};
	virtual bool OnContextPopup(TMenuItem* pMenu)
	{
		  Frame->m_pPlayerContext->SetMenu(pMenu);

		  IDCMapVarValue* pval =  Frame->m_pPlayerContext->ContextValue();
		  pval->Clear();
		  pval->AddDBRef(&RefOwner,Frame->Players);
		  return Frame->m_pPlayerContext->Setup();
	};
};

int ExpandGroup(TFleetSideFrame* Frame,
                 TsTreeView* pView,
                 RestoreState* rest,
                 DCMapDataReference const& Ref,
                 TTreeNode* Node,
                 TItemData* Parent,bool ExpandByType=false)
{

    int Count=0;
    if(Frame->Fleets->SelectGroup(Ref))
    do
    {
            if(Frame->Fleets[DCMDB_FLEET_OUTDATED])continue;

            Count++;
            int group = Frame->Fleets[DCMDB_FLEET_GROUP];
            int type = Frame->Fleets[DCMDB_FLEET_TYPE];
            int tta = Frame->Fleets[DCMDB_FLEET_TTA];
            TTreeNode* Sub;
            if(Node) Sub = pView->Items->AddChild(Node,"");
			else Sub = pView->Items->Add(0,"");
            //Index++;
            //Sub->Indent = Item->Indent + 1;
            TItemData* data=0;

            bool foreign = Frame->Fleets[DCMDB_FLEET_FOREIGN];

            if(type == DCMAP_FTP_Fleet)
            {
                    if(foreign)
                    {
                    	if(tta)
							data = new TForeignFleetInTransitItemData(Frame,*Frame->Fleets->GetID(),Sub,Parent);
                    	else
                            data = new TForeignFleetItemData(Frame,*Frame->Fleets->GetID(),Sub,Parent);
                    }
                    else
                    {
                    	if(tta)
                            data = new TMyFleetInTransitItemData(Frame,*Frame->Fleets->GetID(),Sub,Parent);
                    	else
                            data = new TMyFleetItemData(Frame,*Frame->Fleets->GetID(),Sub,Parent);
                    }
            }
            else if(type == DCMAP_FTP_Group)
            {
                    if(group == DCMAP_FGRP_PlanetGroup)
						data = new TPlanetGroupItemData(Frame,*Frame->Fleets->GetID(),Sub,Parent);
					else if(group == DCMAP_FGRP_PlayerGroup)
                    	data = new TPlayerGroupItemData(Frame,*Frame->Fleets->GetID(),Sub,Parent);
					else
						data = new TFleetGroupItemData(Frame,*Frame->Fleets->GetID(),Sub,Parent);
            }
            else if(type == DCMAP_FTP_Garrizon)
            {
                    data = new TGarrizonFleetItemData(Frame,*Frame->Fleets->GetID(),Sub,Parent);
            }
            else if(type == DCMAP_FTP_Buildings)
            {
                    data = new TBuildingsFleetItemData(Frame,*Frame->Fleets->GetID(),Sub,Parent);
            }
            else if(type == DCMAP_FTP_Queue)
            {
                    data = new TQueueFleetItemData(Frame,*Frame->Fleets->GetID(),Sub,Parent);
			}
			else // unknown fleet
			{
				 delete Sub;
				 continue;
			}

			data->Init();

            if(rest)
            {
               ExpandedSet::iterator it =  rest->exp.find(data->Ref);
               if(it != rest->exp.end() ||
               	(ExpandByType && data->NodeType >=0 && rest->OpenByType[data->NodeType]) )
               {
                            Frame->Fleets->PushGroupSelectState();
                            int sz = data->Expand(rest);
                            Sub->Expand(false);
                            Count+=sz;
                            Frame->Fleets->PopGroupSelectState();
               }
               if(data->Ref == rest->top_ref) rest->top_item = data;
            }
    }
    while(Frame->Fleets->Next());
	return Count;
}


void TFleetSideFrame::UpdateFleets()
{
	 UpdateFleets(*Fleets->GetRootGroup(),FleetView,&m_RestFleets);
}

void TFleetSideFrame::UpdateFleets(DCMapDataReference const& RRef,TsTreeView* pView, RestoreState* rest)
{
	m_pIconsList->UpdateIcons();
	if(pView->Items->Count>1)UpdateRest(pView,rest);

	pView->Items->BeginUpdate();
	pView->Items->Clear();

	int Count = ExpandGroup(this,pView,rest,RRef,0,0,false);

	if(rest->top_item) pView->TopItem = rest->top_item->Node;
	pView->Items->EndUpdate();
	if(rest->top_item) pView->TopItem = rest->top_item->Node;
}

void TFleetSideFrame::UpdatePlanetFleets(dcmapDBREF refPlanet,TsTreeView* pView, RestoreState* rest)
{
	if(Planets->SelectID(refPlanet))
	{
		dcmapDBREF RRef = Planets[DCMDB_PLANET_FLEETS];

		if(pView->Items->Count>1)UpdateRest(pView,rest);

		pView->Items->BeginUpdate();
		pView->Items->Clear();

                TTreeNode* PlanetNode = pView->Items->Add(0,"");
        	TItemData* PlanetData = new TPlanetItemData(this,refPlanet,PlanetNode,0);
		PlanetData->Init();

                if(rest && rest->top_ref == PlanetData->Ref) rest->top_item = PlanetData;

		int Count = ExpandGroup(this,pView,rest,RRef,PlanetNode,PlanetData,true);
                PlanetNode->Expand(false);

		if(rest->top_item)	pView->TopItem = rest->top_item->Node;
		pView->Items->EndUpdate();
		if(rest->top_item) 	pView->TopItem = rest->top_item->Node;
	}
}

void __fastcall TFleetSideFrame::FleetViewMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TsListView* pView = (TsListView*)Sender;

 /*	THitTests ht = pView->GetHitTestInfoAt(X,Y);
	if(ht.Contains(htOnStateIcon) )
	{
		TListItem* Item = pView->GetItemAt(X,Y);
		if(Item->StateIndex == 0)
		{
			pView->Items->BeginUpdate();
			((TItemData*)Item->Data)->Expand(Item);
			pView->Items->EndUpdate();
		}
		else if(Item->StateIndex == 1)
		{
			pView->Items->BeginUpdate();
			((TItemData*)Item->Data)->Collapce(Item);
			pView->Items->EndUpdate();
		}
	} */
}
//---------------------------------------------------------------------------

typedef std::list<TFleetItemData*>     ListFleets;
typedef std::list<TUnitItemData*>      ListUnits;
typedef std::list<TItemData*>          ListItems;
typedef std::list<TBuildingItemData*>  ListBuildings;
typedef std::list<TQueueItemData*>     ListQueue;
typedef std::list<TFleetGroupItemData*>     ListFleetGroups;

class TSelectedItems
{
public:
	TFleetSideFrame* m_Frame;
	TsTreeView* pView;

	TItemData* pItem;

	ListItems  m_Items;

	ListFleets m_Fleets;
	ListUnits  m_Units;
	ListBuildings m_Buildings;
	ListQueue  m_Queue;
        ListFleetGroups m_FleetGroups;

	bool SelFleetsFlySent;
	bool SelFleetsFlyReady;

	bool SelCanHideFleet;
	bool SelCanCancelHideFleet;
	bool SelCanUnhideFleet;
	bool SelCanCancelUnhideFleet;

	bool SelFleets;
	bool SelMyFleets;
	bool SelUnits;
	bool SelFleetGroups;
	bool SelCanUnpack;
	bool SelLoading;
	int Type;
	bool SelCanLoad;

	dcmapDBREF m_refSingleOwner;
	dcmapDBREF m_refSingleFleet;

	bool m_bSingleOwner;
	bool m_bSingleFleet;
	bool m_bSingleLocation;
	bool m_bExpandToUnits;
	bool m_bSingleType;
	int m_x,m_y;

	int m_iUnitGroup;

	TSelectedItems(TFleetSideFrame* Frame,TsTreeView* View){Init(Frame,View);}
	TSelectedItems(){Clear();};

	void Clear()
	{
		m_Frame = 0;
		pView = 0;

		m_Fleets.clear();
		m_Units.clear();
		m_Items.clear();
		m_Buildings.clear();
		m_Queue.clear();
		m_FleetGroups.clear();
		pItem = 0;

		SelFleetsFlySent=false;
		SelFleetsFlyReady=false;
		SelFleets=false;
		SelUnits=false;
		m_bExpandToUnits = false;
		m_iUnitGroup=0;

		m_refSingleOwner.Invalidate();
		m_refSingleFleet.Invalidate();
		m_bSingleOwner=true;
		m_bSingleLocation=true;
		m_bSingleFleet=true;
		m_bSingleType=true;
		SelMyFleets=false;
		SelFleetGroups=false;
		Type=DCMAP_FTP_Unknown;

		SelCanHideFleet=false;
		SelCanCancelHideFleet=false;
		SelCanUnhideFleet=false;
		SelCanCancelUnhideFleet=false;

		SelCanUnpack=false;
		SelLoading=false;
		SelCanLoad=false;

		m_x=0;
		m_y=0;
	}

	void ExpandToUnits(TItemData* Item)
	{
		Item->Expand();

		int n = Item->Node->Count;
		for(int i=0;i<n;i++)
		{
		   AddItem((TItemData*)Item->Node->Item[i]->Data);
		}
	}

   void ExpandToUnits()
   {
	   if(m_bExpandToUnits)return;
	   m_bExpandToUnits = true;

	   if(m_iUnitGroup==1)
	   {
			TItemData* group = m_Items.front();
			m_Items.pop_front();
			m_iUnitGroup=0;
			ExpandToUnits(group);
	   }

	   _foreach(ListFleets,m_Fleets,it)
	   {
			ExpandToUnits(*it);
	   }
   }

	void AddItem(TItemData* Item)
	{
		// Unit groups auto expantion if more than one item selected
		TUnitGroupItemData* pUnitGroup = dynamic_cast<TUnitGroupItemData*>(Item);
		if(pUnitGroup)
		{
			if(m_bExpandToUnits || !m_Items.empty())
			{
                if(m_iUnitGroup==1)
				{
					TItemData* group = m_Items.front();
					m_Items.pop_front();
					m_iUnitGroup=0;
					ExpandToUnits(group);
				}
				ExpandToUnits(pUnitGroup);
			}
			else
			{
				m_Items.push_front(Item);
				m_iUnitGroup++;
			}
			//SelUnits = true;
			if(!pUnitGroup->Foreign) SelUnits = true;
		}
		else
		{
			if(m_iUnitGroup==1)
			{
				TItemData* group = m_Items.front();
				m_Items.pop_front();
				m_iUnitGroup=0;
				ExpandToUnits(group);
			}
			m_Items.push_front(Item);
		}

		TFleetGroupItemData* FleetGroup = dynamic_cast<TFleetGroupItemData*>(Item);
		if(FleetGroup)
                {
                	m_FleetGroups.push_front(FleetGroup);
                        SelFleetGroups=true;
                }

		TFleetItemData* pFleet = dynamic_cast<TFleetItemData*>(Item);
		if(pFleet)
		{
				m_Fleets.push_front(pFleet);
				SelFleets=true;

				if(m_bExpandToUnits)ExpandToUnits(pFleet);

				TMyFleetItemData* pMyFleet = dynamic_cast<TMyFleetItemData*>(Item);
				if(pMyFleet)
				{
					SelMyFleets=true;

					SelFleetsFlySent |= pMyFleet->FlySent;
					SelFleetsFlyReady |= pMyFleet->FlyReady;

					SelCanHideFleet         |=  pMyFleet->Stealth && !pMyFleet->Hidden && !pMyFleet->Changes;
					SelCanCancelHideFleet   |=  pMyFleet->Stealth && !pMyFleet->Hidden && pMyFleet->Changes==1;
					SelCanUnhideFleet       |=  pMyFleet->Hidden && !pMyFleet->Changes;
					SelCanCancelUnhideFleet |=  pMyFleet->Hidden && pMyFleet->Changes==2;
				}
		}
		else
		{
				TUnitItemData* pUnit = dynamic_cast<TUnitItemData*>(Item);
				if(pUnit)
				{
						pFleet = Item->GetAs<TFleetItemData>();
						if(!pFleet)return;
				}

				TMyUnitItemData* pMyUnit = dynamic_cast<TMyUnitItemData*>(Item);
				if(pMyUnit)
				{
						m_Units.push_front(pMyUnit);
						SelUnits = true;
				}

				TTaggedUnitItemData* pTagged = dynamic_cast<TTaggedUnitItemData*>(Item);
				if(pTagged)
				{
					SelCanUnpack |= pTagged->CanUnpack();
					SelLoading |=  pTagged->Loading;
					SelCanLoad |= !pTagged->Loading;
				}

				TUnitTagItemData* pTag = dynamic_cast<TUnitTagItemData*>(Item);
				if(pTag) SelCanUnpack |= pTag->CanUnpack();


				TBuildingItemData* pBuilding = dynamic_cast<TBuildingItemData*>(Item);
				if(pBuilding)
				{
						m_Buildings.push_front(pBuilding);
				}

				TQueueItemData* pQueue = dynamic_cast<TQueueItemData*>(Item);
				if(pQueue)
				{
						m_Queue.push_front(pQueue);
				}
		}

		if(pFleet)
		{
			if(m_x==0 && m_y==0)
			{
					m_x = pFleet->x;
					m_y = pFleet->y;
			}
			else if(m_x != pFleet->x || m_y != pFleet->y) m_bSingleLocation = false;

			if(Type==DCMAP_FTP_Unknown)Type = Item->FleetType;
			else if(Type != Item->FleetType)m_bSingleType=false;

                    if(m_refSingleFleet.Valid())
                    {
                            if(pFleet->Ref != m_refSingleFleet)m_bSingleFleet=false;
                    }
                    else m_refSingleFleet = pFleet->Ref;
                }

		if(m_refSingleOwner.Valid())
		{
				if(Item->RefOwner.Valid() && m_refSingleOwner != Item->RefOwner)
				{
						m_bSingleOwner=false;
				}
		}
		else m_refSingleOwner = Item->RefOwner;
	}

	void Init(TFleetSideFrame* Frame,TItemData* Item)
	{
		Clear();
		m_Frame = Frame;
		pView = 0;
		AddItem(Item);
		pItem = Item;
	}

	void Init(TFleetSideFrame* Frame,TsTreeView* View)
	{
		Clear();
		m_Frame = Frame;
		pView = View;

		if(!Frame)return;
		if(!View)return;

		int n = pView->SelectionCount;
		for(int i=0;i<n;i++)
		{
			TItemData* Item = (TItemData*)(pView->Selections[i]->Data);
						AddItem(Item);
		}
		if(!m_Items.empty()) pItem = m_Items.front();
	}
};

class TDragFleetItems: public TDCMapDragObject
{
public:
	TSelectedItems selItems;
	struct TiFace : public IDCMapFleetDragObject
	{
		TSelectedItems* pItems;

		dcmapCSTR DCMAPAPI Name(){return "Fleet";}
		int DCMAPAPI NumFleets()
		{
			return pItems->m_Fleets.size();
		};
		DCMapDataReference const* DCMAPAPI Fleet(int n)
		{
			ListFleets::iterator it = pItems->m_Fleets.begin();
			for(int i=0;i<n-1;++n){if(it == pItems->m_Fleets.end())return 0;++it;}
			return &(*it)->Ref;
		}
	} iFace;

	TSelectedItems* Selection(){return &selItems;}

	IDCMapDragObject* pObj;
	IDCMapDragObject* Interface(){return pObj;}
	IDCMapModule* Module(){return selItems.m_Frame->Module();};
	TDragFleetItems(TFleetSideFrame* Frame,TsTreeView* View):pObj(0),selItems(Frame,View)
	{
		iFace.pItems = &selItems;
		pObj = &iFace;
	}
};

//---------------------------------------------------------------------------
__fastcall TFleetSideFrame::TFleetSideFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
	m_pLastHintNode = 0;
	ShowBuildings = false;
	m_pSelection = new TSelectedItems;
	m_pCurrentView = 0;
	m_bEditMode=false;
	m_bBlockUpdate=false;
	m_bNeedUpdate=false;
}
__fastcall TFleetSideFrame::~TFleetSideFrame()
{
        delete m_pSelection;
}

void __fastcall TFleetSideFrame::FleetViewStartDrag(TObject *Sender,
	  TDragObject *&DragObject)
{
	TsTreeView* pView = (TsTreeView*)Sender;
	DragObject = new TDragFleetItems(this,pView);
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::FleetViewDblClick(TObject *Sender)
{
	TsTreeView* pView = (TsTreeView*)Sender;
        TPoint pos = pView->ScreenToClient(Mouse->CursorPos);

	TTreeNode* pNode =  pView->GetNodeAt(pos.x,pos.y);
	THitTests ht = pView->GetHitTestInfoAt(pos.x,pos.y);

	if(pNode)
	{
		((TItemData*)pNode->Data)->DblClick();
	}
}
//---------------------------------------------------------------------------
void TFleetSideFrame::UpdateSel(TItemData* Data,TsTreeView* pView)
{
	m_pCurrentView = pView;
}


void TFleetSideFrame::UpdateFleetInfo(DCMapDataReference const& Fleet,DCMapDataReference const* pUnit)
{
	UnitNameLabel->Caption = "";

	CurrentFleet = Fleet;
	if(Fleets->SelectID(Fleet))
	{
		FleetNameLabel->Caption = Fleets[DCMDB_FLEET_NAME];
		FleetSpeedLabel->Caption = FloatToStrF((float)Fleets[DCMDB_FLEET_FLY_SPEED],ffFixed,7,2);
		FleetRangeLabel->Caption = FloatToStrF((float)Fleets[DCMDB_FLEET_FLY_RANGE],ffFixed,7,2);
		if(Players->SelectID(Fleets[DCMDB_FLEET_OWNER]))
		{
			FleetOwnerLabel->Caption = Players->GetWideName();
		}
		else  FleetOwnerLabel->Caption = L"";

		if(Fleets[DCMDB_FLEET_TTA])
		{
			SendFleetBtn->Visible = false;
			CancelJumpBtn->Enabled = m_pFleetMan->CanCancelJump(*Fleets->GetID());
			CancelJumpBtn->Visible = true;

			FleetInfoLabel->Caption =
				(String)L" летит из " +
				CoordToStr(Fleets[DCMDB_FLEET_FROM_X],Fleets[DCMDB_FLEET_FROM_Y])+
				L" в " + CoordToStr(Fleets[DCMDB_FLEET_X],Fleets[DCMDB_FLEET_Y])+
				L" ходов : " + Fleets[DCMDB_FLEET_TTA].Str();
		}
		else
		{
			CancelJumpBtn->Visible = false;
			SendFleetBtn->Visible = true;

			FleetInfoLabel->Caption =
				(String)L"находится в " +
				 CoordToStr(Fleets[DCMDB_FLEET_X],Fleets[DCMDB_FLEET_Y]);

			DCMapDataReference Unit;
			if(pUnit) Unit = *pUnit;
			else
			{

			}
			if(Units->SelectID(Unit))
			{
				DCMapDataReference Prj = Units[DCMDB_BUILDING_PROJECT];
				if(Projects->SelectID(Prj))
				{
					UnitNameLabel->Caption = Projects[DCMDB_PROJECT_NAME];
				}
			}
		}
		if(Fleets[DCMDB_FLEET_FOREIGN])
		{
			CancelJumpBtn->Visible = false;
			SendFleetBtn->Visible = false;
		}
	}
}



void __fastcall TFleetSideFrame::SendFleetBtnMouseLeave(TObject *Sender)
{
	if(Mouse->Capture == SendFleetBtn->Handle)
	{
		//SendFleetBtn->BeginDrag(true,0);
	}
}
//---------------------------------------------------------------------------

int TFleetSideFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	switch(Msg)
	{
	case DCMM_DATA_CHANGED:
		if(Fleets.actualize_state()
			|| Units.actualize_state()
			|| UnitTags.actualize_state()
			|| m_bNeedUpdate)
		{
			if(m_bBlockUpdate || m_bEditMode)
			{
				m_bNeedUpdate=true;
				break;
			}
			m_bNeedUpdate=false;
			UpdateFleets(*Fleets->GetRootGroup(),FleetView,&m_RestFleets);
			UpdateFleetInfo(CurrentFleet);

			m_pPlanetMan->GetCurrentPlanetCoords(&m_planetX,&m_planetY);
			DCMapDataReference ref;
			m_pPlanetMan->GetCurrentPlanet(&ref);
			UpdatePlanetFleets(ref,PlanetFleetView,&m_RestPlanet);

		}
		break;
		case DCMM_PLANET_CHANGED:
		{
			m_pPlanetMan->GetCurrentPlanetCoords(&m_planetX,&m_planetY);
			DCMapDataReference ref;
			m_pPlanetMan->GetCurrentPlanet(&ref);
			UpdatePlanetFleets(ref,PlanetFleetView,&m_RestPlanet);
		}
		break;
	case DCMM_SKIN_CHANGED:
		SetupColors();
		break;
	}
	return 0;
}

void __fastcall TFleetSideFrame::CancelJumpBtnClick(TObject *Sender)
{
	if(SendFleetBtn->Dragging())return;

	if(Fleets->SelectID(CurrentFleet))
	{
		m_pSelection->Init(this,m_pCurrentView);

		CancelFleetsJump(m_pSelection);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::SendFleetBtnClick(TObject *Sender)
{
	String Coords;
	if(sInputQuery(L"Отправка флота",L"Введите координаты планеты",Coords))
	{
		TPoint pt = StrToPoint(Coords);
		if(pt.x>0 && pt.y>0)
		{
			m_pSelection->Init(this,m_pCurrentView);
			SendFleetsTo(m_pSelection,pt.x,pt.y);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::FleetViewKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
	if(m_bEditMode)return;

	TsTreeView* pView = dynamic_cast<TsTreeView*>(Sender);

	TSelectedItems selItems(this,pView);

	if(Key == VK_DELETE)
	{
		DeleteItems(&selItems);
	}

/*	TListItem* Item = pView->Selected;
	if(!Item)return;
	TItemData* Parent = ((TItemData*)Item->Data)->Parent;
	if(!Parent)return;
	pView->Selected=NULL;
	pView->ItemIndex = Parent->Item->Index;
*/
	// build selection info
}
//---------------------------------------------------------------------------

void TFleetSideFrame::LoadConfig(DCMapConfigWrapper cfg)
{
	cfg.section(m_pPanelInterface->ModuleName);

	PlanetViewHeight = PlanetFleetView->Height;
	cfg["PlanetsHeight"] >> PlanetViewHeight;

	int v;
	if(cfg["Height"] >> v)
		Height = v;

	m_RestFleets.LoadConfig("RestFleets",cfg);
	m_RestPlanet.LoadConfig("RestPlanet",cfg);
}

void TFleetSideFrame::SaveConfig(DCMapConfigWrapper cfg)
{
	cfg.section(m_pPanelInterface->ModuleName);

	cfg["Height"] << Height;
	cfg["PlanetsHeight"] << PlanetFleetView->Height;

	UpdateRest(FleetView,&m_RestFleets);
	UpdateRest(PlanetFleetView,&m_RestPlanet);

	m_RestFleets.SaveConfig("RestFleets",cfg);
	m_RestPlanet.SaveConfig("RestPlanet",cfg);
}

void TFleetSideFrame::UpdateRest(TsTreeView* pView,RestoreState* rest)
{
	if(pView->TopItem)
	{
			TItemData* data = (TItemData*)pView->TopItem->Data;
            rest->top_item = 0;
			rest->top_ref = data->Ref;
	}
	else  rest->top =0;

	int sz = pView->Items->Count;
	if(sz)rest->exp.clear();
	for(int i=0;i<sz;i++)
	{
		TTreeNode* node = pView->Items->Item[i];
		TItemData* data = (TItemData*)node->Data;
		if(node->Expanded)
		{
		    rest->exp.insert(data->Ref);
		}
                if(node->HasChildren && data->NodeType>=0)
                {
                        rest->OpenByType[data->NodeType] = node->Expanded;
                }
	}
}

void RestoreState::Clear()
{
      for (int i = 0; i < NodeTypeNum; i++) {OpenByType[i]=false;}
}

void RestoreState::SaveConfig(String Name,DCMapConfigWrapper cfg)
{
	cfg[Name+"TopRef"] << DBRefToStr(top_ref.data);
	TStringList* pList = new TStringList;
	for(ExpandedSet::iterator it = exp.begin();it!=exp.end();++it)
	{
		pList->Add(DBRefToStr(*it));
	}
	cfg[Name+"Exp"] << (TStrings*)pList;

        String Opened;
        for (int i = 0; i < NodeTypeNum; i++) {Opened += OpenByType[i]?L'1':L'0';}
        cfg[Name+"OpenByType"] << Opened;
	delete pList;
}

void RestoreState::LoadConfig(String Name,DCMapConfigWrapper cfg)
{
	Clear();

	String StrTopRef;
	cfg[Name+"TopRef"] >> StrTopRef;
        if(!StrTopRef.IsEmpty())
			top_ref = DBRefFromStr(StrTopRef);
        else top_ref.Invalidate();
        top_item=0;

	TStringList* pList = new TStringList;
	String str;
	cfg[Name+"Exp"] >> (TStrings*)pList;
	int sz = pList->Count;
	for(int i=0;i<sz;i++)
	{
		exp.insert(DBRefFromStr(pList->Strings[i]));
	}
	delete pList;

        String Opened;
	cfg[Name+"OpenByType"] >> Opened;
        for (int i = 0; i < Min(NodeTypeNum,Opened.Length()); i++)
        {
               OpenByType[i] = Opened[i+1] != L'0';
        }
}

void TFleetSideFrame::OnShow()
{
	UpdateFleets();
	PlanetFleetView->Height = PlanetViewHeight;

	BottomPanel->Top = Height;
	DragBar->Top = Height;
}


void __fastcall TFleetSideFrame::DragBarMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	MoverLastMouse = Y;
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::DragBarMouseMove(TObject *Sender,
	  TShiftState Shift, int X, int Y)
{
	if(Shift.Contains(ssLeft))
	{
		int dist = Y - MoverLastMouse;
		int nh = Height + dist;
		if(nh > this->Constraints->MinHeight)
			Height = nh;
	}
}
//---------------------------------------------------------------------------


void TFleetSideFrame::SetupColors()
{
	int BG = clWindow;

	if(sFrameAdapter->SkinData->SkinManager->Active)
	{
	  String BgSection = "EDIT";

	  DynamicArray<Smaskdata::TsGeneralData > & gd =
			sFrameAdapter->SkinData->SkinManager->gd;

	  int Index = sFrameAdapter->SkinData->SkinManager->GetSkinIndex(BgSection);
	  BG = gd[Index].Color;
	}
	 //if(FleetIcons2->BkColor != BG)
	 {
		 //FleetIcons2->BkColor = BG;
		 //FleetIcons2->CopyImages(FleetIcons);
		 //FleetView->Images = FleetIcons2;
		 //PlanetFleetView->Images = FleetIcons2;
	 }
}

void __fastcall TFleetSideFrame::FleetViewExpanding(TObject *Sender,
	  TTreeNode *Node, bool &AllowExpansion)
{
	//
	TItemData* data = (TItemData*)Node->Data;

	if(data && data->CanExpand() && data->Expand())
	{
		AllowExpansion = true;
	}
}
//---------------------------------------------------------------------------



void __fastcall TFleetSideFrame::FleetViewChanging(TObject *Sender,
	  TTreeNode *Node, bool &AllowChange)
{
	//Node->TreeView->se
	TItemData* data = (TItemData*)Node->Data;
	AllowChange = data->Select();
}
//---------------------------------------------------------------------------


void __fastcall TFleetSideFrame::FleetViewMouseMove(TObject *Sender, TShiftState Shift,
		  int X, int Y)
{
	TTreeView* pView = dynamic_cast<TTreeView*>(Sender);
	if(!pView)return;

	TTreeNode* pNode =  pView->GetNodeAt(X,Y);
	THitTests ht = pView->GetHitTestInfoAt(X,Y);

	if(m_pLastHintNode != pNode)
	{
		Application->CancelHint();

		m_pLastHintNode = pNode;

		if(!pNode)
		{
			pView->Hint = "";
			return;
		}
		TItemData* pItem = (TItemData*)pNode->Data;
		if(!pItem)return;

		pView->AutoExpand = pItem->AutoExpand();

		bool ok = ht.Contains(htOnItem) ||
			ht.Contains(htOnIcon) ||
			ht.Contains(htOnLabel) ||
			ht.Contains(htOnStateIcon);

		if(ok)
		{
			TItemData* pItem = (TItemData*)pNode->Data;

			pView->ShowHint = true;
			pView->Hint = pItem->GetHint();
		}
		else pView->Hint = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::FleetViewClick(TObject *Sender)
{
	TsTreeView* pView = (TsTreeView*)Sender;
	TTreeNode* Item = pView->Selected;
	if(Item)
	{
		((TItemData*)Item->Data)->Click();
	}

}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::FleetViewDeletion(TObject *Sender, TTreeNode *Node)
{
	TItemData* pItem = (TItemData*)Node->Data;
	if(pItem) delete pItem;
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::FleetViewDragOver(TObject *Sender, TObject *Source,
		  int X, int Y, TDragState State, bool &Accept)
{
	Accept = false;
	TsTreeView* pView = (TsTreeView*)Sender;
	TDragFleetItems* DragItems = dynamic_cast<TDragFleetItems*>(Source);
	if(!DragItems)return;
	TSelectedItems* selItems = DragItems->Selection();
	TTreeNode* pNode =  pView->GetNodeAt(X,Y);
	THitTests ht = pView->GetHitTestInfoAt(X,Y);

	bool units = !selItems->m_Units.empty() && selItems->m_bSingleOwner && selItems->m_bSingleLocation;

	if(!pNode) // create new fleet
	{
		return;
	}
	TItemData* pItem = (TItemData*)pNode->Data;
	TMyFleetItemData* pFleet = dynamic_cast<TMyFleetItemData*>(pItem);
	if(pFleet)// move units & combine fleets
	{
		// check units at same planet
		if(units && pFleet->x == selItems->m_x && pFleet->y == selItems->m_y &&
			(pFleet->FleetType == DCMAP_FTP_Fleet || pFleet->FleetType == DCMAP_FTP_Garrizon) )
		{
			if(selItems->m_Units.size()==1)
			{
				if(m_pFleetMan->CanMoveUnitToFleet(selItems->m_Units.front()->Ref,pFleet->Ref))
					Accept = true;
			}
			else Accept = true;
			return;
		}
	}
}
//---------------------------------------------------------------------------
static bool MoveUnits(IDCMapFleetManager*  pFleetMan, ListUnits& Units,dcmapDBREF const& Fleet)
{
	pFleetMan->BeginUpdate();

	for(int i=0;i<256;i++)
	{
		int moved=0;
		ListUnits::iterator it = Units.begin();
		while(it != Units.end())
		{
			 if(pFleetMan->MoveUnitToFleet((*it)->Ref,Fleet))
			 {
				 it = Units.erase(it);
				 moved++;
			 }
			 else ++it;
		}
		if(!moved)break;
	}
	pFleetMan->EndUpdate();
	return Units.empty();
}

void __fastcall TFleetSideFrame::FleetViewDragDrop(TObject *Sender, TObject *Source,
		  int X, int Y)
{
	DCMapScopeBlockUpdates bupd(m_pCore);

	TsTreeView* pView = (TsTreeView*)Sender;
	TDragFleetItems* DragItems = dynamic_cast<TDragFleetItems*>(Source);
	if(!DragItems)return;
	TSelectedItems* selItems = DragItems->Selection();

	bool units = !selItems->m_Units.empty() && selItems->m_bSingleOwner && selItems->m_bSingleLocation;

	TTreeNode* pNode =  pView->GetNodeAt(X,Y);
	THitTests ht = pView->GetHitTestInfoAt(X,Y);
	if(!pNode) // create new fleet
	{
		return;
	}
	TItemData* pItem = (TItemData*)pNode->Data;
	TMyFleetItemData* pFleet = dynamic_cast<TMyFleetItemData*>(pItem);
	if(pFleet)// move units & combine fleets
	{
		// move units from fleet to fleet
		if(units)
		{
			MoveUnits(m_pFleetMan,selItems->m_Units,pFleet->Ref);
		}
	}
}
//---------------------------------------------------------------------------
void TFleetSideFrame::DeleteItems(TSelectedItems* selItems)
{
	DCMapScopeBlockUpdates bupd(m_pCore);

	selItems->ExpandToUnits();

	int n = selItems->m_Units.size();
	n += selItems->m_Queue.size();
	n += selItems->m_Buildings.size();

	m_pFleetMan->BeginUpdate();

	if(n)
	{

		IDCMapVarValueScopedPtr pVal = dcmapCreateVarValue();
		pVal->SetNamedNumber(L"count",n);
		if(!m_pCore->GetCoreUI()->InputDialog("DisbandUnitConfirmation",pVal)) return;

		for(int i=0;i<2;i++)
		{
			ListUnits::iterator it = selItems->m_Units.begin();
			while(it != selItems->m_Units.end())
			{
				 if(m_pFleetMan->DisbandUnit((*it)->Ref))
				 {
					 it = selItems->m_Units.erase(it);
				 }
				 else ++it;
			}
		}

		{
			ListBuildings::iterator it = selItems->m_Buildings.begin();
			while(it != selItems->m_Buildings.end())
			{
				if(m_pFleetMan->DisbandUnit((*it)->Ref))
				{
					it = selItems->m_Buildings.erase(it);
				}
				else ++it;
			}
		}

		{
			ListQueue::iterator it = selItems->m_Queue.begin();
			while(it != selItems->m_Queue.end())
			{
				if(m_pFleetMan->DisbandUnit((*it)->Ref))
				{
					it = selItems->m_Queue.erase(it);
				}
				else ++it;
			}
		}
	}


	{
		ListFleets::iterator it = selItems->m_Fleets.begin();
		while(it != selItems->m_Fleets.end())
		{
			if((*it)->FleetType == DCMAP_FTP_Fleet &&
			   m_pFleetMan->DeleteFleet((*it)->Ref))
			{
				it = selItems->m_Fleets.erase(it);
			}
			else ++it;
		}
	}

	m_pFleetMan->EndUpdate();
}

void TFleetSideFrame::CreateNewFleet(TSelectedItems* selItems)
{
	DCMapScopeBlockUpdates bupd(m_pCore);

	if(!selItems->m_bSingleLocation || !selItems->m_bSingleOwner) return;

	dcmapDBREF NewFleet;
	String NewString = L"Fleet";
	if(sInputQuery(L"Создание флота", L"Имя нового флота", NewString))
	{
		m_pFleetMan->BeginUpdate();
		 if(m_pFleetMan->CreateFleetAt(selItems->m_x,selItems->m_y,
						selItems->m_refSingleOwner,NewString.w_str(),&NewFleet))
		 {
			selItems->ExpandToUnits();
			MoveUnits(m_pFleetMan,selItems->m_Units,NewFleet);
		 }
		m_pFleetMan->EndUpdate();
	}
}

void __fastcall TFleetSideFrame::FleetViewContextPopup(TObject *Sender, TPoint &MousePos,
		  bool &Handled)
{
	DCMapScopeBlockUpdates bupd(m_pCore);

	m_pPlayerContext->ClearItems();
	m_pPlanetContext->ClearItems();

	TsTreeView* pView = (TsTreeView*)Sender;

	TTreeNode* pNode =  pView->GetNodeAt(MousePos.x,MousePos.y);
	THitTests ht = pView->GetHitTestInfoAt(MousePos.x,MousePos.y);
	TPoint PopupPos = pView->ClientToScreen(MousePos);

	if(pNode)
	{
		if(pNode->Selected)
		{
			m_pSelection->Init(this,pView);
		}
		else
		{
			m_pSelection->Init(this,(TItemData*)(pNode->Data));
		}
	}
	else
	{
		m_pSelection->Init(this,pView);
	}
	if(m_pSelection->m_Items.empty())return;
        bool SingleItem = m_pSelection->m_Items.size()==1;


	FlyToMenu->Visible = false;
	CancelJumpMenu->Visible = false;

	HideFleetMenu->Visible = false;
	CancelHideFleetMenu->Visible = false;
	UnhideFleetMenu->Visible = false;
	CancelUnhideFleetMenu->Visible = false;

	if(m_pSelection->m_bSingleType)
	{
		if(m_pSelection->Type == DCMAP_FTP_Queue)
		{
			MakeProjectsMenu(m_pSelection);
			QueuePopup->Popup(PopupPos.x,PopupPos.y);
			Handled = true;
			return;
		}

		if(m_pSelection->Type == DCMAP_FTP_Fleet)
		{
			FlyToMenu->Visible = m_pSelection->SelFleetsFlyReady;
			CancelJumpMenu->Visible = m_pSelection->SelFleetsFlySent;

			HideFleetMenu->Visible = m_pSelection->SelCanHideFleet;
			CancelHideFleetMenu->Visible = m_pSelection->SelCanCancelHideFleet;
			UnhideFleetMenu->Visible = m_pSelection->SelCanUnhideFleet;
			CancelUnhideFleetMenu->Visible = m_pSelection->SelCanCancelUnhideFleet;
		}
	}

	DisbandMenu->Visible = m_pSelection->SelFleets && m_pSelection->SelFleetsFlyReady
						 || m_pSelection->SelUnits || !m_pSelection->m_Buildings.empty()
						 || !m_pSelection->m_Queue.empty();

	bool CanSetIcon = false;
	_foreach(ListItems,m_pSelection->m_Items,it)
	{
		CanSetIcon |= (*it)->CanSetIcon();
	}
	SetIconMenu->Visible = CanSetIcon;

	MoveToFleetMenu->Visible = m_pSelection->SelUnits;
	RenameMenu->Visible = !m_pSelection->m_Items.empty() && m_pSelection->m_Items.front()->CanEdit();

	PropertiesMenu->Visible = m_pSelection->m_Items.size()==1 &&
								m_pSelection->pItem->HaveProperties();

	bool NeedHook=false;

	if(m_pSelection->SelUnits)
	{
		int r = MakeMoveToFleetMenu(m_pSelection);
		MoveToFleetMenu->Visible = r>0;
		NeedHook |= r>1;
	}
	else MoveToFleetMenu->Visible  = false;

	CreateFleetFromSelMenu->Visible = MoveToFleetMenu->Visible;

	if(SingleItem)
	{
		NeedHook |=m_pSelection->pItem->OnContextPopup(FleetPopup->Items);
	}

	if(m_pSelection->SelMyFleets || m_pSelection->SelUnits)
		m_pSelection->ExpandToUnits();

	UnpackMenu->Visible = m_pSelection->SelCanUnpack;

	CancelCargoLoadMenu->Visible = m_pSelection->SelLoading;

	NeedHook |=MakeActionMenu(m_pSelection);

	if(NeedHook)InterfaceModule->HookMenu(FleetPopup);

	FleetPopup->Popup(PopupPos.x,PopupPos.y);
	Handled = true;
}
//---------------------------------------------------------------------------

int TFleetSideFrame::SendFleetsTo(TSelectedItems* pSel,int x,int y)
{
	DCMapScopeBlockUpdates bupd(m_pCore);

	int sent=0;
	m_pFleetMan->BeginUpdate();
	_foreach(ListFleets,pSel->m_Fleets,it)
	{
		if(m_pFleetMan->MoveFleetTo((*it)->Ref,x,y))
		{
			++sent;
		}
	}
	m_pFleetMan->EndUpdate();
	return sent;
}

int TFleetSideFrame::CancelFleetsJump(TSelectedItems* pSel)
{
	DCMapScopeBlockUpdates bupd(m_pCore);

	int sent=0;
	m_pFleetMan->BeginUpdate();
	_foreach(ListFleets,pSel->m_Fleets,it)
	{
		if(m_pFleetMan->CancelJump((*it)->Ref))
		{
			++sent;
		}
	}
	m_pFleetMan->EndUpdate();

	return sent;
}


void __fastcall TFleetSideFrame::FlyToMenuClick(TObject *Sender)
{
	String Coords;
	if(sInputQuery(L"Отправка флота",L"Введите координаты планеты",Coords))
	{
		TPoint pt = StrToPoint(Coords);
		if(pt.x>0 && pt.y>0)
		{
			SendFleetsTo(m_pSelection,pt.x,pt.y);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::CancelJumpMenuClick(TObject *Sender)
{
	int sent=0;

	m_pFleetMan->BeginUpdate();
	_foreach(ListFleets,m_pSelection->m_Fleets,it)
	{
		if(m_pFleetMan->CancelJump((*it)->Ref));
		{
			++sent;
		}
	}
	m_pFleetMan->EndUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::CreateFleetFromSelMenuClick(TObject *Sender)
{
	CreateNewFleet(m_pSelection);
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::DisbandMenuClick(TObject *Sender)
{
	DeleteItems(m_pSelection);
}

int TFleetSideFrame::MakeMoveToFleetMenu(TSelectedItems* Sel)
{
	if(!m_pSelection->m_bSingleOwner)return 0;
	if(!m_pSelection->m_bSingleLocation)return 0;

	// clear old menu
	ClearMenuItems(MoveToFleetMenu,1);

	dcmapDBREF TestUnit;

	int Count=0;

	bool cargo=false;

	if(Sel->m_Units.size()==1)
	{
		TestUnit = m_pSelection->m_Units.front()->Ref;
		if(!Units->SelectID(TestUnit))return 0;
		if(!Projects->SelectID(Units[DCMDB_BUILDING_PROJECT]))return 0;
		if(Projects[DCMDB_PROJECT_IS_TRANSPORTABLE])
			cargo = true;
	}
	else
	{

	}

	if(!cargo)
	{
		MoveToNewFleetMenu->Visible = true;
		Count++;
	}
	else MoveToNewFleetMenu->Visible = false;

	if(Planets->SelectCoord(m_pSelection->m_x,m_pSelection->m_y))
	{
		if(Fleets->SelectGroup(Planets[DCMDB_PLANET_FLEETS]))
		do
		{
			if(m_pSelection->m_bSingleFleet &&
					m_pSelection->m_refSingleFleet == *Fleets->GetID())continue;

			if(Fleets[DCMDB_FLEET_OWNER] != m_pSelection->m_refSingleOwner)continue;

			int ftype = Fleets[DCMDB_FLEET_TYPE];
			if(ftype != DCMAP_FTP_Fleet && ftype != DCMAP_FTP_Garrizon)continue;

			if(Fleets[DCMDB_FLEET_OUTDATED])continue;
			if(Fleets[DCMDB_FLEET_DELETED])continue;

			if(TestUnit.Valid())
			{
				Fleets->PushGroupSelectState();
				bool can = m_pFleetMan->CanMoveUnitToFleet(TestUnit,*Fleets->GetID());
				Fleets->PopGroupSelectState();
				if(!can)continue;
			}

			TMenuItem* pSubItem = new TMenuItem(MoveToFleetMenu);
			pSubItem->Caption = Fleets[DCMDB_FLEET_NAME];
			pSubItem->ImageIndex = Fleets[DCMDB_FLEET_LIST_IMAGE];
			pSubItem->OnClick = MoveToFleetMenuItemClick;
			pSubItem->Hint = DBRefToStr(*Fleets->GetID());
			MoveToFleetMenu->Add(pSubItem);
			Count++;
		}
		while(Fleets->Next());

		if(Sel->SelCanLoad)
		{
			TMenuItem* pSubItem = new TMenuItem(MoveToFleetMenu);
			pSubItem->Caption = L"-";
			MoveToFleetMenu->Add(pSubItem);
			Count++;

			if(Fleets->SelectGroup(Planets[DCMDB_PLANET_FLEETS]))
			do
			{
				if(Fleets[DCMDB_FLEET_OWNER] == m_pSelection->m_refSingleOwner)continue;

				if((int)Fleets[DCMDB_FLEET_TYPE] != DCMAP_FTP_Fleet)continue;
				if(Fleets[DCMDB_FLEET_OUTDATED])continue;
				if(Fleets[DCMDB_FLEET_DELETED])continue;

				TMenuItem* pSubItem = new TMenuItem(MoveToFleetMenu);
				pSubItem->Caption = Fleets[DCMDB_FLEET_NAME];
				pSubItem->ImageIndex = Fleets[DCMDB_FLEET_LIST_IMAGE];
				pSubItem->OnClick = MoveToFleetMenuItemClick;
				pSubItem->Hint = DBRefToStr(*Fleets->GetID());
				MoveToFleetMenu->Add(pSubItem);
				Count++;
			}
			while(Fleets->Next());
		}
	}
	return Count;
}


//---------------------------------------------------------------------------
void __fastcall TFleetSideFrame::MoveToFleetMenuItemClick(TObject *Sender)
{
	DCMapScopeBlockUpdates bupd(m_pCore);
	TMenuItem* pItem = (TMenuItem*)Sender;
	dcmapDBREF FleetRef = DBRefFromStr(pItem->Hint);

	m_pSelection->ExpandToUnits();
	if(Fleets->SelectID(FleetRef))
	{
		MoveUnits(m_pFleetMan,m_pSelection->m_Units,FleetRef);
	}
}
void __fastcall TFleetSideFrame::FleetViewEditing(TObject *Sender, TTreeNode *Node,
		  bool &AllowEdit)
{
	TsTreeView* pView = (TsTreeView*)Sender;
	TItemData* Item = (TItemData*)(Node->Data);

	String Text = Node->Text;
	if(Item->BeginEdit(Text))
	{
		if(Node->Text == Text)
		{
			AllowEdit = true;
			m_bEditMode=true;
			return;
        }
	}
	AllowEdit = false;
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::FleetViewEdited(TObject *Sender, TTreeNode *Node,
		  UnicodeString &S)
{
	DCMapScopeBlockUpdates bupd(m_pCore);
	TItemData* Item = (TItemData*)(Node->Data);
	Item->Edited(S);
	m_bEditMode=false;
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::RenameMenuClick(TObject *Sender)
{
	DCMapScopeBlockUpdates bupd(m_pCore);
	TItemData* Item = 	m_pSelection->pItem;
	String Text = Item->Node->Text;
	if(Item->BeginEdit(Text))
	{
		if(1)
		{
			if(sInputQuery(L"Переименование флота",L"Введите новое имя флота \""+Text+"\"",Text))
			{
				Item->Edited(Text);
			}
		}
		else
		{
			Item->Node->Text = Text;
			m_bEditMode=true;
			Item->Node->EditText();
		}
	}
}

bool TFleetSideFrame::MakeProjectsMenu(TSelectedItems* Sel)
{
	ClearMenuItems(BuildingsMenu);
	ClearMenuItems(UnitsMenu);

	if(!Sel->m_bSingleLocation || !Sel->m_bSingleOwner)return false;
 	Players->SelectID(Sel->m_refSingleOwner);
	int tu = Players[DCMDB_PLAYER_TEHNOLOGY];

        if(!Planets->SelectCoord(Sel->m_x,Sel->m_y))return false;
        IDCMapPlanetCalculator::PlanetParamInfo ProdInfo;
        if(!m_pCalc->ProcessPlanetParams(*Planets->GetID(),&ProdInfo))return false;

	if(Projects->SelectByParentID(Sel->m_refSingleOwner))
	do
	{
        	if(!Components->SelectID(Projects[DCMDB_PROJECT_CHASSIS]))continue;

		if((int)Projects[DCMDB_PROJECT_REQ_TEHN_LEVEL] > tu)continue;

		int req = Components[DCMDB_COMPONENT_BUILD_REQ];
		int unique = Components[DCMDB_COMPONENT_BUILD_UNIQUE];

		if((req&ProdInfo.BuildMask) != req)continue;
		if((unique&ProdInfo.BuildUnique) != 0)continue;

		TMenuItem* item = new TMenuItem(this);
		item->Caption = Projects[DCMDB_PROJECT_CAPTION];
		item->Tag = Projects[DCMDB_PROJECT_GAMEID];
		item->OnClick = QueueAddClick;

		int Carapace = Projects[DCMDB_PROJECT_CARAPACE];
		int BuildingId =  Projects[DCMDB_PROJECT_GAMEID];

		int image = InterfaceModule->GetFleetImage(Components[DCMDB_COMPONENT_ICON_NAME]);
		item->ImageIndex = image;

		if(Projects[DCMDB_PROJECT_IS_BUILDING])
			BuildingsMenu->Add(item);
		else
			UnitsMenu->Add(item);
	}
	while(Projects->Next());

	WrapMenu(UnitsMenu);

	InterfaceModule->HookMenu(QueuePopup);
        return true;
}

void __fastcall TFleetSideFrame::QueueAddClick(TObject *Sender)
{
	DCMapScopeBlockUpdates bupd(m_pCore);

	TSelectedItems* Sel = m_pSelection;
	TMenuItem* Item = dynamic_cast<TMenuItem*>(Sender);
	if(!Item)return;

	if(!Sel->m_bSingleLocation || !Sel->m_bSingleOwner)return ;
	if(!Planets->SelectCoord(Sel->m_x,Sel->m_y))return ;


	if(Projects->SelectGameID(Sel->m_refSingleOwner,Item->Tag))
	{
			m_pFleetMan->AddUnitToQueue(*Planets->GetID(),*Projects->GetID());
	}
}

//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::SetIconMenuClick(TObject *Sender)
{
	ClearMenuItems(SetIconMenu,1);
	m_pIconsList->CreateMenuItems(SetIconMenu,OnSetIcon,1);
	InterfaceModule->HookMenu(SetIconMenu);
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::OnSetIcon(TObject *Sender)
{
	DCMapScopeBlockUpdates bupd(m_pCore);
	TMenuItem* pItem = (TMenuItem*)Sender;

	dcmapDBREF IconRef;

	if(!pItem->Hint.IsEmpty())
	{
		IconRef = DBRefFromStr(pItem->Hint);
		if(!Icons->SelectID(IconRef))return;
	}
	//
	_foreach(ListItems,m_pSelection->m_Items,it)
	{
		if((*it)->CanSetIcon())(*it)->SetIcon(IconRef);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::FleetViewAdvancedCustomDrawItem(TCustomTreeView *Sender,
		  TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage,
		  bool &PaintImages, bool &DefaultDraw)
{
	if(Stage == cdPostPaint)
	{
		TsTreeView* TV =  (TsTreeView*)Sender;
		TItemData* pItem = (TItemData*)Node->Data;

		TRect DispRect = Node->DisplayRect(True);

		int AtPos = DispRect.Right;

		if(pItem->IconIndex >=0 )
		{
			InterfaceModule->IconsImageList->Draw(TV->Canvas,AtPos,DispRect.Top+3,pItem->IconIndex);
			AtPos+= 11;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::PropertiesMenuClick(TObject *Sender)
{
	if(m_pSelection->pItem)
    {
    	Application->ProcessMessages();
		m_pSelection->pItem->ShowProperties();
    }
}
//---------------------------------------------------------------------------


void __fastcall TFleetSideFrame::FleetViewCollapsing(TObject *Sender, TTreeNode *Node,
		  bool &AllowCollapse)
{
	TItemData* data = (TItemData*)Node->Data;
	if(data && data->CanCollapse())
	{
		AllowCollapse = true;
	}
        else AllowCollapse = false;
}

int TFleetSideFrame::MakeActionMenu(TSelectedItems* Sel)
{
	typedef  std::set<int> ActSet;
	ActSet Actions;
	ActSet CancelActions;

	_foreach(ListUnits,Sel->m_Units,it)
	{
		TUnitItemData* pUnit = (*it);
                ActSet lca;

        	if(ActReq->SelectByParentID(pUnit->Ref))
                do
				{
					 if(ActReq[DCMDB_ACTREQ_CANCELED])continue;
					 int actid = ActReq[DCMDB_ACTREQ_ACTID];
                     CancelActions.insert(actid);
                     lca.insert(actid);
                }
                while(ActReq->Next());

		if(ProjectActs->SelectByParentID(pUnit->RefProject))
		do
		{
                	int actid = ProjectActs[DCMDB_PROJECTACT_ACTID];

                        if(lca.find(actid) != lca.end())continue;

			if(m_pFleetMan->CanExecuteUnitAction(pUnit->Ref,*ProjectActs->GetID()))
				Actions.insert(actid);
		}
		while(ProjectActs->Next());
	}
	ClearMenuItems(FleetPopup->Items,0,UnitActionCancelMenuClick);
	ClearMenuItems(FleetPopup->Items,0,UnitActionMenuClick);

	g_pLocalStrings->SelectSection("UnitActions");

	_foreach(ActSet,CancelActions,it)
	{
		int id = *it;
		TMenuItem* item = new TMenuItem(this);
		item->Caption = g_pLocalStrings->GetSectionStringW(AnsiString(L"ac"+IntToStr(id)).c_str());
		if(item->Caption.IsEmpty())item->Caption = L"ac"+IntToStr(id);
		item->Tag = id;
		item->OnClick = UnitActionCancelMenuClick;
		FleetPopup->Items->Insert(0,item);
	}
	_foreach(ActSet,Actions,it)
	{
		int id = *it;
		TMenuItem* item = new TMenuItem(this);
		item->Caption = g_pLocalStrings->GetSectionStringW(AnsiString(L"as"+IntToStr(id)).c_str());
		if(item->Caption.IsEmpty())item->Caption = L"as"+IntToStr(id);
		item->Tag = id;
		item->OnClick = UnitActionMenuClick;
		FleetPopup->Items->Insert(0,item);
	}
	return Actions.size() + CancelActions.size();
}

//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::UnitActionMenuClick(TObject *Sender)
{
	TMenuItem* Item = dynamic_cast<TMenuItem*>(Sender);
	DCMapScopeBlockUpdates bupd(m_pCore);

	m_pSelection->ExpandToUnits();

	m_pFleetMan->BeginUpdate();
	_foreach(ListUnits,m_pSelection->m_Units,it)
	{
		TUnitItemData* pUnit = (*it);
		m_pFleetMan->ExecuteUnitAction(pUnit->Ref,Item->Tag);
	}
	m_pFleetMan->EndUpdate();
}
void __fastcall TFleetSideFrame::UnitActionCancelMenuClick(TObject *Sender)
{
	TMenuItem* Item = dynamic_cast<TMenuItem*>(Sender);
	DCMapScopeBlockUpdates bupd(m_pCore);

	m_pSelection->ExpandToUnits();

	m_pFleetMan->BeginUpdate();
	_foreach(ListUnits,m_pSelection->m_Units,it)
	{
		TUnitItemData* pUnit = (*it);
		m_pFleetMan->CancelUnitAction(pUnit->Ref,Item->Tag);
	}
	m_pFleetMan->EndUpdate();
}

void __fastcall TFleetSideFrame::HideFleetMenuClick(TObject *Sender)
{
	TMenuItem* Item = dynamic_cast<TMenuItem*>(Sender);
	DCMapScopeBlockUpdates bupd(m_pCore);

	m_pFleetMan->BeginUpdate();
	_foreach(ListFleets,m_pSelection->m_Fleets,it)
	{
		m_pFleetMan->HideFleet((*it)->Ref,Item->Tag);
	}
	m_pFleetMan->EndUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::FleetViewChange(TObject *Sender, TTreeNode *Node)

{
	UpdateSel((TItemData*)Node->Data,(TsTreeView*)Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::UnpackMenuClick(TObject *Sender)
{
	std::set<dcmapDBREF> UnitsToUnpack;

	_foreach(ListItems,m_pSelection->m_Items,it)
	{
		TTaggedUnitItemData* pUnit = dynamic_cast<TTaggedUnitItemData*>(*it);
		if(pUnit && pUnit->CanUnpack())
		{
			if(m_pFleetMan->CanUnpackContainer(pUnit->Ref))
			   UnitsToUnpack.insert(pUnit->Ref);
		}
		TUnitTagItemData* pTag = dynamic_cast<TUnitTagItemData*>(*it);
		if(pTag && pTag->CanUnpack())
		{
			if(m_pFleetMan->CanUnpackContainer(pTag->UnitRef))
			   UnitsToUnpack.insert(pTag->UnitRef);
		}
	}
	if(UnitsToUnpack.empty())return;

	DCMapVarValue Val;
	Val[L"count"] = UnitsToUnpack.size();

	if(m_pCore->GetCoreUI()->InputDialog("UnpackContainerConfirmation",Val))
	{
		m_pFleetMan->BeginUpdate();
		_foreach(std::set<dcmapDBREF>,UnitsToUnpack,it)
		{
			 m_pFleetMan->UnpackContainer(*it);
		}
		m_pFleetMan->EndUpdate();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFleetSideFrame::CancelCargoLoadMenuClick(TObject *Sender)
{
	std::set<dcmapDBREF> UnitsTo;

	_foreach(ListItems,m_pSelection->m_Items,it)
	{
        TTaggedUnitItemData* pUnit = 0;
		TUnitTagItemData* pTag = dynamic_cast<TUnitTagItemData*>(*it);
		if(pTag) pUnit = dynamic_cast<TTaggedUnitItemData*>(pTag->Parent);
		else pUnit = dynamic_cast<TTaggedUnitItemData*>(*it);

		if(pUnit && pUnit->Loading)
		{
			if(m_pFleetMan->CanCancelCargoLoad(pUnit->Ref))
			   UnitsTo.insert(pUnit->Ref);
		}
	}
	if(UnitsTo.empty())return;

	m_pFleetMan->BeginUpdate();
	_foreach(std::set<dcmapDBREF>,UnitsTo,it)
	{
			 m_pFleetMan->CancelCargoLoad(*it);
	}
	m_pFleetMan->EndUpdate();
}
//---------------------------------------------------------------------------


