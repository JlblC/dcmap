//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FleetProp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sEdit"
#pragma link "BaseModule"
#pragma link "sComboBox"
#pragma link "sGroupBox"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TFleetEditFrame *FleetEditFrame;

static CFramePanelWrapper<TFleetEditFrame> sWrap("PListEditPropPage_fleets","00BaseInfo");

//---------------------------------------------------------------------------
__fastcall TFleetEditFrame::TFleetEditFrame(TComponent* Owner)
	: Inherited(Owner)
{
	BehaviorChanged=false;
}
//---------------------------------------------------------------------------
bool TFleetEditFrame::OnInit()
{
	if(!m_pPlanets.init(m_pCore))return false;
	if(!m_pPlayers.init(m_pCore))return false;
	if(!m_pIcons.init(m_pCore))return false;
	if(!m_pActions.init(m_pCore))return false;
	if(!m_pFleets.init(m_pCore))return false;

	m_pFleetMan = m_pCore->GetServiceTyped<IDCMapFleetManager>("FleetManager");
	if(!m_pFleetMan)return false;
	m_pCalc = m_pCore->GetServiceTyped<IDCMapPlanetCalculator>("PlanetCalculator");
	if(!m_pCalc)return false;


    return true;
}

void TFleetEditFrame::OnCreate()
{
	BehBoxes[5] = UnknownBehCombo;
	BehBoxes[3] = NeutralsBehCombo;
	BehBoxes[1] = VassalBehCombo;
	BehBoxes[4] = EnemiesBehCombo;
	BehBoxes[2] = AllyBehCombo;
	BehBoxes[0] = LordBehCombo;
}

static int FromBeh[] =   {0,2,1,0,3,0};
static int ToBeh[]     = {3,2,1,4,0,0};

static int FromBehStealth[] =   {0,3,2,1,4,0};
static int ToBehStealth[]     = {5,3,2,1,4,0};

static dcmapDWORD BehDefaults =   0x313444;


void TFleetEditFrame::OnDataUpdate()
{
	if(!m_pFleets->SelectID(m_DataRef))return;

	NameEdit->Text = m_pFleets[DCMDB_FLEET_NAME];
	ExtNameEdit->Text = m_pFleets[DCMDB_FLEET_EXTNAME];

	m_bStealth = (float)m_pFleets[DCMDB_FLEET_STEALTH]>0;

	if(NameEdit->Text ==  ExtNameEdit->Text)
	   ExtNameEdit->Text = L"";

	dcmapDWORD beh = m_pFleets[DCMDB_FLEET_BEHAVIOR];

	if(!beh)beh =  BehDefaults;

	dcmapWCSTR szItems = m_bStealth?LSTRW(FleetBehaviorItemsStealth):LSTRW(FleetBehaviorItems);

	for (int i = 0; i < 6; i++)
	{
		 unsigned v = beh & 0x0f;
		 if(v>5)continue;

		 BehBoxes[i]->Items->Text =szItems;
		 BehBoxes[i]->ItemIndex = m_bStealth?FromBehStealth[v]:FromBeh[v];
		 beh = beh >> 4;
	}

	Changed=false;
	NameChanged=false;
	BehaviorChanged=false;
}

void TFleetEditFrame::OnApply()
{
	if(!Changed)return;
	if(!m_pFleets->SelectID(m_DataRef))return;

	DCMapScopeBlockUpdates bupd(m_pCore);

	if(NameChanged)
	{
	   if(NameEdit->Text ==  ExtNameEdit->Text)
			m_pFleetMan->RenameFleet(m_DataRef,NameEdit->Text.w_str(),L"");
	   else
			m_pFleetMan->RenameFleet(m_DataRef,NameEdit->Text.w_str(),
											   ExtNameEdit->Text.w_str());
		NameChanged=false;
	}

	if(BehaviorChanged)
	{
		dcmapDWORD beh = 0;
		for (int i = 6; --i >= 0;)
		{
			 unsigned v = BehBoxes[i]->ItemIndex;
			 if(v>4)continue;
			 beh = (beh << 4) | (m_bStealth?ToBehStealth[v]:ToBeh[v]);
		}
		m_pFleetMan->ChangeBehavior(m_DataRef,beh);
		BehaviorChanged = false;
	}
	m_pFleets->Update();


	m_pFleets->Post(&m_DataRef);
	m_pFleetMan->UpdateFleet(m_DataRef);

	Changed=false;
}
void TFleetEditFrame::OnCansel()
{

}


void __fastcall TFleetEditFrame::ExtNameEditChange(TObject *Sender)
{
	Changed=true;
        NameChanged=true;
}
//---------------------------------------------------------------------------

void __fastcall TFleetEditFrame::BehComboChange(TObject *Sender)
{
	BehaviorChanged=true;
	Changed=true;
}
//---------------------------------------------------------------------------

