//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PlanetContensProp.h"
#include "Interface.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sListView"
#pragma link "sButton"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TPlanetGarrisonFrame *PlanetGarrisonFrame;

static CFramePanelWrapper<TPlanetGarrisonFrame> sWrap("PListEditPropPage_planets","02EconomyInfo");

struct TBuildingItem : public TObject
{
	dcmapDBREF proj;
	dcmapDBREF bld;
	TListItem* item;
	std::vector<dcmapDBREF> m_vecBuildings;

	void Append(dcmapDBREF b)
	{
		if(m_vecBuildings.empty())m_vecBuildings.push_back(bld);
		m_vecBuildings.push_back(b);
	}

	void Init()
	{
		item->Data = this;
    }
};

//---------------------------------------------------------------------------
__fastcall TPlanetGarrisonFrame::TPlanetGarrisonFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------

void PostProcessList(TsListView* TargetView,bool sort)
{
	if(sort)
	{
		for(int i=0;i<TargetView->Items->Count;i++)
		{
			TBuildingItem* pItem = (TBuildingItem*)TargetView->Items->Item[i]->Data;

			if(!pItem->m_vecBuildings.empty())
			{
				pItem->item->Caption += L" ["+IntToStr((int)pItem->m_vecBuildings.size())+L"]";
			}
		}
	}
	if(sort) TargetView->AlphaSort();

	// wrap default col
	TListItem* pBottom = TargetView->Items->Item[TargetView->Items->Count-1];
	if(pBottom)
	{
		if(pBottom->Top > TargetView->Height-25)
		{
			 TargetView->Columns->Items[0]->Width = TargetView->Width - 22;
		}
		else
			TargetView->Columns->Items[0]->Width = TargetView->Width - 4;
	}
}

void TPlanetGarrisonFrame::UpdateList(TsListView *TargetView,dcmapDBREF refFleet,bool sort)
{
	TargetView->Items->BeginUpdate();
        TargetView->Items->Clear();

        if(m_pBuildings->SelectByParentID(refFleet))do
	{
		dcmapDBREF proj = m_pBuildings[DCMDB_BUILDING_PROJECT];
		dcmapDBREF bld = *m_pBuildings->GetID();

		if(!m_pProjects->SelectID(proj))continue;
		if(m_pBuildings[DCMDB_BUILDING_OUTDATED])continue;
		if(m_pBuildings[DCMDB_BUILDING_DISBANDED])continue;

		TBuildingItem* pItem = 0;

		String cap  = m_pProjects[DCMDB_PROJECT_NAME];
        	// todo: check hp
		if(sort)
		{
			TListItem* curitem = TargetView->FindCaption(0,cap,false,true,false);
			if(curitem)
			do
			{
				pItem = (TBuildingItem*)curitem->Data;
				if(pItem && pItem->proj == proj)
				{
					pItem->Append(bld);
					break;
				}
				else pItem = 0;
				curitem = TargetView->FindCaption(curitem->Index,cap,false,false,false);
			}
			while(curitem);
		}

		if(pItem)continue;

		pItem = new TBuildingItem;
		pItem->item = TargetView->Items->Add();
		pItem->item->Data = pItem;
		pItem->item->Caption = cap;
		pItem->proj = proj;
		pItem->bld = bld;
		pItem->Init();
	}
	while(m_pBuildings->Next());
        PostProcessList(TargetView,sort);

        TargetView->Items->EndUpdate();
}

void TPlanetGarrisonFrame::UpdateLists()
{
	UpdateList(GarrisonView,m_RefGarrizon,true);
	UpdateList(BuildingsView,m_RefBuildings,true);
	UpdateList(QueueView,m_RefQueue,false);
}

int TPlanetGarrisonFrame::UpdateData()
{
	if(!m_pPlanets->SelectID(m_Ref)) return -1;
	m_RefGarrizon = m_pPlanets[DCMDB_PLANET_GARRISON];
	m_RefBuildings = m_pPlanets[DCMDB_PLANET_BUILDINGS];
	m_RefQueue = m_pPlanets[DCMDB_PLANET_QUEUE];
	m_RefOwner   = m_pPlanets[DCMDB_PLANET_OWNER];
	if(!m_pFleets->SelectID(m_RefGarrizon))return -1;
	if(m_pFleets[DCMDB_FLEET_OUTDATED])return -1;

	if(!m_pCalc->ProcessPlanetParams(m_Ref,&m_PlanetInfo))return -1;

	UpdateLists();

	MakeProjectsMenu();

	Changed = false;
	return 0;
}

int TPlanetGarrisonFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
    case DCMM_SELECTION_DATA_CHANGED:
        m_Ref.id = wParam;
        m_Ref.age = lParam;
		return UpdateData();
	}
    return 0;
}

bool TPlanetGarrisonFrame::OnInit()
{
	if(!m_pPlanets.init(m_pCore))return false;
	if(!m_pPlayers.init(m_pCore))return false;
	if(!m_pFleets.init(m_pCore))return false;
	if(!m_pBuildings.init(m_pCore))return false;
	if(!m_pActions.init(m_pCore))return false;
	if(!m_pProjects.init(m_pCore))return false;

	m_pCalc = m_pCore->GetServiceTyped<IDCMapPlanetCalculator>("PlanetCalculator");
	if(!m_pCalc)return false;

	return true;
}

void __fastcall TPlanetGarrisonFrame::BuildingsViewDeletion(TObject *Sender, TListItem *Item)
{
	if(Item->Data) delete (TBuildingItem*)Item->Data;
}
//---------------------------------------------------------------------------
void TPlanetGarrisonFrame::OnShow()
{
}

void TPlanetGarrisonFrame::MakeProjectsMenu()
{
       	ClearMenuItems(BuildingsMenu);
       	ClearMenuItems(UnitsMenu);

	m_pPlayers->SelectID(m_RefOwner);

	int tu = m_pPlayers[DCMDB_PLAYER_TEHNOLOGY];

	if(m_pProjects->SelectByParentID(m_RefOwner))
	do
	{
		if((int)m_pProjects[DCMDB_PROJECT_REQ_TEHN_LEVEL] > tu)continue;
		//int req = m_pProjects[DCMDB_PROJECT_BUILD_REQ];
		//if(req&m_PlanetInfo.BuildMask != req)continue;

		TMenuItem* item = new TMenuItem(this);
		item->Caption = m_pProjects[DCMDB_PROJECT_CAPTION];
		item->Tag = m_pProjects[DCMDB_PROJECT_GAMEID];
		item->OnClick = AddClick;

		if(m_pProjects[DCMDB_PROJECT_IS_BUILDING])
			BuildingsMenu->Add(item);
		else
			UnitsMenu->Add(item);
	}
	while(m_pProjects->Next());

	InterfaceModule->HookMenu(ProjectsMenu);

	int row_size = 20;

	if(UnitsMenu->Count > 16)
	{
		int columns = UnitsMenu->Count/row_size;
		int rsz = UnitsMenu->Count/columns;

		for(int i=1;i<columns;i++)
			UnitsMenu->Items[i*rsz]->Break = mbBreak;
	}
}

void __fastcall TPlanetGarrisonFrame::AddClick(TObject *Sender)
{

	TMenuItem* Item = dynamic_cast<TMenuItem*>(Sender);
	if(!Item)return;
	if(m_pProjects->SelectGameID(m_RefOwner,Item->Tag))
	{
		// create dummy building to put in queue
		dcmapDBREF refBld;
		dcmapDBREF refProj = *m_pProjects->GetID();

		m_pBuildings->Insert();
		m_pBuildings[DCMDB_BUILDING_OWNER] = m_RefQueue;
		m_pBuildings[DCMDB_BUILDING_PROJECT] = refProj;
		m_pBuildings[DCMDB_BUILDING_QUEUED] = true;
		m_pBuildings[DCMDB_BUILDING_PLANETARY] = (bool)m_pProjects[DCMDB_PROJECT_IS_BUILDING];
		m_pBuildings->Post(&refBld);

                m_pFleets->SelectID(m_RefQueue);
                m_pFleets->Update();
                m_pFleets[DCMDB_FLEET_SHIP_COUNT] = (int)m_pFleets[DCMDB_FLEET_SHIP_COUNT]+1;
                 m_pFleets->Post();

		// create action
		m_pActions->Insert();
		m_pActions[DCMDB_ACTION_TYPE]     =  DCMACT_ADD_BUILDING_TO_QUEUE;
		m_pActions[DCMDB_ACTION_PROJECT]  =  refProj;
		m_pActions[DCMDB_ACTION_PLANET]   =  m_Ref;
		m_pActions[DCMDB_ACTION_FLEET]    =  m_RefQueue;
		m_pActions[DCMDB_ACTION_BUILDING] =  refBld;
		m_pActions[DCMDB_ACTION_OWNER]    =  m_RefOwner;
		m_pActions->Post();

		// add item
		TBuildingItem* pItem = new TBuildingItem;
		pItem->item = QueueView->Items->Add();
		pItem->proj = refProj;
		pItem->bld  = refBld;
		pItem->item->Caption = m_pProjects[DCMDB_PROJECT_CAPTION];
		pItem->Init();
	}
}
//---------------------------------------------------------------------------

