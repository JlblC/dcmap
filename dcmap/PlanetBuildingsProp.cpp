//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PlanetBuildingsProp.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma link "sCheckBox"
#pragma link "sRadioButton"
#pragma link "sBitBtn"
#pragma link "sGroupBox"
#pragma link "sLabel"
#pragma resource "*.dfm"
TPlanetBuildingsFrame *PlanetBuildingsFrame;

static CFramePanelWrapper<TPlanetBuildingsFrame> sWrap("PListEditPropPage_planets","01BuildingsInfo");

//---------------------------------------------------------------------------
__fastcall TPlanetBuildingsFrame::TPlanetBuildingsFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TPlanetBuildingsFrame::OnApply()
{
	if(!Changed)return;
	DCMapPlanetInfo inf;
	if(m_pPlanets->SelectID(m_Ref))
	{
	   inf = *m_pPlanets->GetData();
	}
	else return;

    inf.mines = MinesEdit->Value;
    inf.biofarms = BiofarmsEdit->Value;
	inf.powerplants = PowerPlantsEdit->Value;

    inf.factories = FactoriesEdit->Value;
    inf.compressors = CompressorsEdit->Value;
	inf.governors = GovernorsEdit->Value;


    m_pPlanets->Update();

	m_pPlanets->SetData(&inf);

    m_pPlanets[DCMDB_PLANET_HAS_BARRACKS] =  BarracksCheck->Checked;
    m_pPlanets[DCMDB_PLANET_HAS_SHIPYARD] =  ShipyardCheck->Checked;
    m_pPlanets[DCMDB_PLANET_HAS_COMPONENTS] =  ComponentsCheck->Checked;
    m_pPlanets[DCMDB_PLANET_HAS_CUSTOMS] =  CustomsCheck->Checked;
    m_pPlanets[DCMDB_PLANET_HAS_RESEARCH] =  ResearchCheck->Checked;
    m_pPlanets[DCMDB_PLANET_HAS_DESIGN] =  DesignCheck->Checked;
    m_pPlanets[DCMDB_PLANET_HAS_DESIGN] =  DesignCheck->Checked;
    m_pPlanets[DCMDB_PLANET_HAS_PALACE] =  PalaceCheck->Checked;

    m_pPlanets[DCMDB_PLANET_INDUSTRIAL] = IndustrialRadio->Checked;
    m_pPlanets[DCMDB_PLANET_BOUNDS] = BounceRadio->Checked;

    m_pPlanets[DCMDB_PLANET_HW] = HWCheck->Checked;
    m_pPlanets[DCMDB_PLANET_UNSAFE] = UnsafeCheck->Checked;

	m_pPlanets->Post();

    Changed = false;
    if(HWChanged)
    {
		HWChanged = false;
		if(!m_pPlanets->SelectID(m_Ref)) return;
		m_pCalc->CalculateCorruption(m_pPlanets[DCMDB_PLANET_OWNER]);
		UpdateData();
	}
}

void TPlanetBuildingsFrame::OnCansel()
{
	
}

bool TPlanetBuildingsFrame::OnInit()
{
	if(!m_pPlanets.init(m_pCore,"planets"))return false;
	if(!m_pPlayers.init(m_pCore,"players"))return false;
	if(!m_pRaces.init(m_pCore))return false;

	m_pCalc = m_pCore->GetServiceTyped<IDCMapPlanetCalculator>("PlanetCalculator");
	if(!m_pCalc)return false;

	return true;
}

int TPlanetBuildingsFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
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

String FI2STR(dcmapREAL v){return FloatToStrF(v,ffNumber,7,0);}

int TPlanetBuildingsFrame::UpdateData()
{
	if(!m_pPlanets->SelectID(m_Ref)) return -1;

	m_Inf = *m_pPlanets->GetData();


	DCMapDataReference owner = m_pPlanets[DCMDB_PLANET_OWNER];
	if(!m_pPlayers->SelectID(owner))return -1;
	bool cr = m_pPlayers[DCMDB_PLAYER_CALCULATECORRUPTION];

	CorruptionEdit->Visible = cr;
	CorruptionEdit->Text = m_pPlanets[DCMDB_PLANET_CORRUPTION].Str()+"%";

	ParamsGroupBox->Visible = false;

	if(m_pRaces->SelectID(m_pPlayers[DCMDB_PLAYER_RACE]))
	{
		int basis = int(m_pRaces[DCMDB_RACE_BASIS]);
		int second = int(m_pRaces[DCMDB_RACE_SECOND]);

		IDCMapPlanetCalculator::PlanetProdInfo pp;
		m_pCalc->ProcessPlanet(m_Ref,&pp);

		InterfaceModule->RsrsImageList->Draw(BasisImg->Canvas,0,0,basis,true);
		BasisImg->Repaint();
		InterfaceModule->RsrsImageList->Draw(SecondImg->Canvas,0,0,second,true);
		SecondImg->Repaint();
		InterfaceModule->RsrsImageList->Draw(CreditsImg->Canvas,0,0,3,true);
		SecondImg->Repaint();

		BasisLabel->Caption = FI2STR(pp.basis);
		SecondLabel->Caption = FI2STR(pp.second);
		CreditsLabel->Caption = FI2STR(pp.credits);

		PopMaxValue->Caption = FI2STR(pp.people_max);
		PopFreeValue->Caption = FI2STR(pp.people_free);
		ProductionValue->Caption = FI2STR(pp.production);

		PopCurValue->Caption = FI2STR(m_pPlanets[DCMDB_PLANET_POPULATION]);

		CorruptValue->Caption = m_pPlanets[DCMDB_PLANET_CORRUPTION].Str()+"%";;

		ParamsGroupBox->Visible = true;
	}
	else ParamsGroupBox->Visible = false;


	BarracksCheck->Checked = m_pPlanets[DCMDB_PLANET_HAS_BARRACKS];
	ShipyardCheck->Checked = m_pPlanets[DCMDB_PLANET_HAS_SHIPYARD];
	ComponentsCheck->Checked = m_pPlanets[DCMDB_PLANET_HAS_COMPONENTS];
	CustomsCheck->Checked = m_pPlanets[DCMDB_PLANET_HAS_CUSTOMS];
	ResearchCheck->Checked = m_pPlanets[DCMDB_PLANET_HAS_RESEARCH];
	DesignCheck->Checked = m_pPlanets[DCMDB_PLANET_HAS_DESIGN];
    PalaceCheck->Checked = m_pPlanets[DCMDB_PLANET_HAS_PALACE];

    MiningRadio->Checked  = true;
    IndustrialRadio->Checked = m_pPlanets[DCMDB_PLANET_INDUSTRIAL];
    BounceRadio->Checked = m_pPlanets[DCMDB_PLANET_BOUNDS];

    DCMapPlanetInfo const* pinf = m_pPlanets->GetData();

    MinesEdit->Value = pinf->mines;
    BiofarmsEdit->Value = pinf->biofarms;
    PowerPlantsEdit->Value = pinf->powerplants;

    FactoriesEdit->Value = pinf->factories;
    CompressorsEdit->Value = pinf->compressors;
    GovernorsEdit->Value = pinf->governors;

	HWCheck->Checked = m_pPlanets[DCMDB_PLANET_HW];
	UnsafeCheck->Checked = m_pPlanets[DCMDB_PLANET_UNSAFE];

	Changed = false;
	HWChanged=false;

	return 0;
}

void __fastcall TPlanetBuildingsFrame::DataChange(TObject *Sender)
{
	Changed = true;
}
//---------------------------------------------------------------------------

void __fastcall TPlanetBuildingsFrame::HWCheckClick(TObject *Sender)
{
	Changed   = true;
	HWChanged = true;
}
//---------------------------------------------------------------------------


void __fastcall TPlanetBuildingsFrame::CalculateCorruptionBtnClick(
	  TObject *Sender)
{
	OnApply();
	if(!m_pPlanets->SelectID(m_Ref)) return;
	m_pCalc->CalculateCorruption(m_pPlanets[DCMDB_PLANET_OWNER]);
	UpdateData();
}
//---------------------------------------------------------------------------



