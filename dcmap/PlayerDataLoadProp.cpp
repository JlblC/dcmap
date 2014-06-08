//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PlayerDataLoadProp.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sCheckBox"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma link "sGroupBox"
#pragma link "sLabel"
#pragma link "sRadioButton"
#pragma resource "*.dfm"
TPlayerAdvPropFrame *PlayerAdvPropFrame;

static CFramePanelWrapper<TPlayerAdvPropFrame> sWrap("PListEditPropPage_players","02AdvInfo");

//---------------------------------------------------------------------------
__fastcall TPlayerAdvPropFrame::TPlayerAdvPropFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
	PasswordChanged = false;
}
//---------------------------------------------------------------------------
bool TPlayerAdvPropFrame::OnInit()
{
	if(!m_pPlayers.init(m_pCore,"players"))return false;
	if(!m_pRaces.init(m_pCore,"races"))return false;
    return true;
}

int TPlayerAdvPropFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
    case DCMM_SELECTION_DATA_CHANGED:
        m_Ref.id = wParam;
        m_Ref.age = lParam;
        UpdateData();
        break;
    }
    return 0;
}

void TPlayerAdvPropFrame::OnCansel()
{
    Changed = false;
}

void TPlayerAdvPropFrame::OnApply()
{
    if(!Changed)return;
    //DCMapPlayerInfo inf;

	bool insert=false;
    if(m_pPlayers->SelectID(m_Ref))
    {
       //inf = *m_pPlayers->GetData();
    }
    else
    {
    	return;
    }
	m_pPlayers->Update();

	if(PasswordChanged)
	{
		m_pPlayers[DCMDB_PLAYER_PASSWORD] = PassEdit->Text;
    	PasswordChanged = false;
	}

	m_pPlayers[DCMDB_PLAYER_LOGIN] = NameEdit->Text;
	m_pPlayers[DCMDB_PLAYER_CALCULATECORRUPTION] = CorruptionCalcCheck->Checked;
    m_pPlayers[DCMDB_PLAYER_WEBACCESS] = WebAccessCheck->Checked;

    m_pPlayers[DCMDB_PLAYER_USER_ID] = UserIDEdit->Value;
    m_pPlayers[DCMDB_PLAYER_SCRIPTING_FLEET_ID] = FleetIDEdit->Value;

	  m_pPlayers[DCMDB_PLAYER_MANAGE] = 		Manage->Checked  ;
	  m_pPlayers[DCMDB_PLAYER_IMPORT_TRUSTED] = 	ImportTrusted->Checked  ;
	  m_pPlayers[DCMDB_PLAYER_IMPORT_TRUSTED] = 	ImportTrusted->Checked  ;
	  m_pPlayers[DCMDB_PLAYER_ADD_EMPTY] = 	AddEmpty->Checked  ;
	  m_pPlayers[DCMDB_PLAYER_ADD_OCCUPIED] = 	AddOccupied->Checked  ;
	  m_pPlayers[DCMDB_PLAYER_OVERWRITE_EMPTY] = 	OverwriteEmpty->Checked  ;
	  m_pPlayers[DCMDB_PLAYER_OVERWRITE_OCCUPIED] = 	OverwriteOccupied->Checked  ;
	  m_pPlayers[DCMDB_PLAYER_CLEAR_OCCUPIED] = 	ClearOccupied->Checked  ;
	  m_pPlayers[DCMDB_PLAYER_IMPORT_DIP] = 	ImportDip->Checked  ;
	  m_pPlayers[DCMDB_PLAYER_IMPORT_CONTENTS] = 	ImportContents->Checked  ;
	  m_pPlayers[DCMDB_PLAYER_IMPORT_FLEETS] = 	ImportFleets->Checked  ;
	  m_pPlayers[DCMDB_PLAYER_IMPORT_ALIEN_FLEETS] = 	ImportAlienFleets->Checked  ;

	  m_pPlayers[DCMDB_PLAYER_IMPORT_REGION] = ImportRegionEdit->Text;

    m_pPlayers->Post();
}

void TPlayerAdvPropFrame::UpdateData()
{
    if(!m_pPlayers->SelectID(m_Ref)) return;

    //DCMapPlayerInfo const* pinf = m_pPlayers->GetData();

	NameEdit->Text = m_pPlayers[DCMDB_PLAYER_LOGIN];
	/*if(!NameEdit->Text.IsEmpty() && PassEdit->Text.IsEmpty())
	{
		PassEdit->Text = L"******";
	}
	*/
	CorruptionCalcCheck->Checked = m_pPlayers[DCMDB_PLAYER_CALCULATECORRUPTION];
	WebAccessCheck->Checked = m_pPlayers[DCMDB_PLAYER_WEBACCESS];

	UserIDEdit->Value = m_pPlayers[DCMDB_PLAYER_USER_ID];
	FleetIDEdit->Value = m_pPlayers[DCMDB_PLAYER_SCRIPTING_FLEET_ID];

	Manage->Checked = m_pPlayers[DCMDB_PLAYER_MANAGE];
	ImportTrusted->Checked = m_pPlayers[DCMDB_PLAYER_IMPORT_TRUSTED];
	AddEmpty->Checked = m_pPlayers[DCMDB_PLAYER_ADD_EMPTY];
	AddOccupied->Checked = m_pPlayers[DCMDB_PLAYER_ADD_OCCUPIED];
	OverwriteEmpty->Checked = m_pPlayers[DCMDB_PLAYER_OVERWRITE_EMPTY];
	OverwriteOccupied->Checked = m_pPlayers[DCMDB_PLAYER_OVERWRITE_OCCUPIED];
	ClearOccupied->Checked = m_pPlayers[DCMDB_PLAYER_CLEAR_OCCUPIED];
	ImportDip->Checked = m_pPlayers[DCMDB_PLAYER_IMPORT_DIP];
	ImportContents->Checked = m_pPlayers[DCMDB_PLAYER_IMPORT_CONTENTS];
	ImportFleets->Checked = m_pPlayers[DCMDB_PLAYER_IMPORT_FLEETS];
	ImportAlienFleets->Checked = m_pPlayers[DCMDB_PLAYER_IMPORT_ALIEN_FLEETS];
	ImportRegionEdit->Text = m_pPlayers[DCMDB_PLAYER_IMPORT_REGION];

	PassText = m_pPlayers[DCMDB_PLAYER_PASSWORD];

	if(!PassText.IsEmpty())PassEdit->Text = L"******";
	else PassEdit->Text = "";

	m_RaceRef = m_pPlayers[DCMDB_PLAYER_RACE];

	CorruptionCalcCheckClick(this);
	UpdateChecks(this);
	Changed = false;
	PasswordChanged = false;
}
void __fastcall TPlayerAdvPropFrame::DataChange(TObject *Sender)
{
	Changed = true;
}
//---------------------------------------------------------------------------

bool TPlayerAdvPropFrame::CalculateIncome()
{
	if(!m_pRaces->SelectID(m_RaceRef))return false;

	IDCMapPlanetCalculator* pCalc = m_pCore->GetServiceTyped<IDCMapPlanetCalculator>("PlanetCalculator");
	if(!pCalc)return false;

	int basis = int(m_pRaces[DCMDB_RACE_BASIS]);
	int second = int(m_pRaces[DCMDB_RACE_SECOND]);

	IDCMapPlanetCalculator::PlayerProdInfo pp;
	pCalc->CalculatePlayerIncome(m_Ref,&pp);

	//InterfaceModule->RsrsImageList->Draw(BasisImg->Picture->Bitmap->Canvas,0,0,basis,true);
	InterfaceModule->RsrsImageList->Draw(BasisImg->Canvas,0,0,basis,true);
	BasisImg->Repaint();
	InterfaceModule->RsrsImageList->Draw(SecondImg->Canvas,0,0,second,true);
	SecondImg->Repaint();
	InterfaceModule->RsrsImageList->Draw(CreditsImg->Canvas,0,0,3,true);
	SecondImg->Repaint();

	BasisLabel->Caption = "+"+FloatToStrF(pp.basis,ffNumber,7,0);
	SecondLabel->Caption = "+"+FloatToStrF(pp.second,ffNumber,7,0);
	CreditsLabel->Caption = "+"+FloatToStrF(pp.credits,ffNumber,7,0);
}
//---------------------------------------------------------------------------

void __fastcall TPlayerAdvPropFrame::CorruptionCalcCheckClick(TObject *Sender)
{
	Changed = true;
	if(CorruptionCalcCheck->Checked)
		IncomeBox->Visible = CalculateIncome();
	else
        IncomeBox->Visible = false;
}       
//---------------------------------------------------------------------------

void __fastcall TPlayerAdvPropFrame::PassEditChange(TObject *Sender)
{
	PasswordChanged = true;
	Changed = true;
}
//---------------------------------------------------------------------------


void __fastcall TPlayerAdvPropFrame::UpdateChecks(TObject *Sender)
{
	Changed = true;
//	NameEdit->Enabled = WebAccessCheck->Checked;
//	PassEdit->Enabled = WebAccessCheck->Checked;

	ImportTrusted->Enabled = WebAccessCheck->Checked;

	Manage->Enabled = WebAccessCheck->Checked;
	ImportDip->Enabled = WebAccessCheck->Checked;
	ImportFleets->Enabled = WebAccessCheck->Checked;
	ImportAlienFleets->Enabled = WebAccessCheck->Checked;
	ImportContents->Enabled = WebAccessCheck->Checked;

	AddEmpty->Enabled = ImportTrusted->Enabled && ImportTrusted->Checked;
	AddOccupied->Enabled = ImportTrusted->Enabled && ImportTrusted->Checked;
	OverwriteEmpty->Enabled = ImportTrusted->Enabled && ImportTrusted->Checked;
	OverwriteOccupied->Enabled = ImportTrusted->Enabled && ImportTrusted->Checked;
	ClearOccupied->Enabled = ImportTrusted->Enabled && ImportTrusted->Checked;

	ImportRegionEdit->Enabled = ImportTrusted->Enabled && ImportTrusted->Checked;

/*	ReplaceAll->Enabled = WebAccessCheck->Checked;
	UpdateAll->Enabled = WebAccessCheck->Checked;
	UpdateActual->Enabled = WebAccessCheck->Checked;
	UpdateMy->Enabled = WebAccessCheck->Checked;
*/

/*	FleetIDEdit->Enabled = WebAccessCheck->Checked;
	ImportAllPlanets->Enabled = WebAccessCheck->Checked;
	ImportActualPlanets->Enabled = ImportAllPlanets->Enabled && ! ImportAllPlanets->Checked;

	UpdateAllPlanets->Enabled = WebAccessCheck->Checked && ImportAllPlanets->Checked;
	UpdateAllPlanets->Enabled = WebAccessCheck->Checked && ImportAllPlanets->Checked;

	UpdateActualPlanets->Enabled = ImportAllPlanets->Enabled &&
						 (ImportActualPlanets->Checked || ImportAllPlanets->Checked) &&
						 !(UpdateAllPlanets->Checked && UpdateAllPlanets->Enabled);
*/
}
//---------------------------------------------------------------------------


