//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PlayerToolbar.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sComboBox"
#pragma link "sLabel"
#pragma resource "*.dfm"
TPlayerToolbarFrame *PlayerToolbarFrame;

static CFramePanelWrapper<TPlayerToolbarFrame> sWrap("PToolbarItem","PlayerToolbarItem");

//---------------------------------------------------------------------------
__fastcall TPlayerToolbarFrame::TPlayerToolbarFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------
bool TPlayerToolbarFrame::OnInit()
{
	if(!m_pPlayers.init(m_pCore))return false;
	if(!m_pPlanets.init(m_pCore))return false;
	if(!m_pRaces.init(m_pCore))return false;

	m_pCalc = m_pCore->GetServiceTyped<IDCMapPlanetCalculator>("PlanetCalculator");
	if(!m_pCalc)return false;

	return true;
}
int TPlayerToolbarFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	switch (Msg)
	{
	case DCMM_DATA_CHANGED:
	  if(m_pPlayers.actualize_state() ||
		 m_pPlanets.actualize_state() ||
	     m_pRaces.actualize_state())
	  {
		  UpdateList();
      }
	  break;
	}
	return 0;
}

void TPlayerToolbarFrame::LoadConfig(DCMapConfigWrapper cfg)
{
	cfg.section("PlayerEconomics");
	cfg["Player"] >> LastPlayer;
	UpdateList();
}

void TPlayerToolbarFrame::SaveConfig(DCMapConfigWrapper cfg)
{
	cfg.section("PlayerEconomics");
	cfg["Player"] << LastPlayer;
}
void TPlayerToolbarFrame::UpdateList()
{
	PlayerCombo->Sorted = false;
	PlayerCombo->Items->Clear();

	if(m_pPlayers->SelectAll())
	do
	{
		if((bool)m_pPlayers[DCMDB_PLAYER_CALCULATECORRUPTION])
			PlayerCombo->Items->Add(m_pPlayers->GetWideName());
	}
	while(m_pPlayers->Next());
	PlayerCombo->Sorted = true;
	int ind = PlayerCombo->Items->IndexOf(LastPlayer);
	if(ind<0)ind = 0;
	PlayerCombo->ItemIndex = ind;
	PlayerComboChange(this);
}

void __fastcall TPlayerToolbarFrame::PlayerComboChange(TObject *Sender)
{
	LastPlayer = PlayerCombo->Items->Strings[PlayerCombo->ItemIndex];

	if(m_pPlayers->SelectWideName(LastPlayer.w_str()))
	{
		if(m_pRaces->SelectID(m_pPlayers[DCMDB_PLAYER_RACE]))
		{
			//IDCMapPlanetCalculator::PlayerProdInfo pp;
			//m_pCalc->CalculatePlayerIncome(*m_pPlayers->GetID(),&pp);

			int basis = int(m_pRaces[DCMDB_RACE_BASIS]);
			int second = int(m_pRaces[DCMDB_RACE_SECOND]);

			//InterfaceModule->RsrsImageList->Draw(BasisImg->Picture->Bitmap->Canvas,0,0,basis,true);
			InterfaceModule->RsrsImageList->Draw(BasisImg->Canvas,0,0,basis,true);
			BasisImg->Repaint();
			InterfaceModule->RsrsImageList->Draw(SecondImg->Canvas,0,0,second,true);
			SecondImg->Repaint();
			InterfaceModule->RsrsImageList->Draw(CreditsImg->Canvas,0,0,3,true);
			SecondImg->Repaint();

			BasisLabel->Caption = FloatToStrF(m_pPlayers[DCMDB_PLAYER_MAIN],ffNumber,7,0);
			BasisGain->Caption =  L"+"+FloatToStrF(m_pPlayers[DCMDB_PLAYER_MAIN_GAIN],ffNumber,7,0);
			BasisSpent->Caption = L"-"+FloatToStrF(m_pPlayers[DCMDB_PLAYER_MAIN_SPENT],ffNumber,7,0);

			SecondLabel->Caption = FloatToStrF(m_pPlayers[DCMDB_PLAYER_SECOND],ffNumber,7,0);
			SecondGain->Caption =  L"+"+FloatToStrF(m_pPlayers[DCMDB_PLAYER_SECOND_GAIN],ffNumber,7,0);
			SecondSpent->Caption = L"-"+FloatToStrF(m_pPlayers[DCMDB_PLAYER_SECOND_SPENT],ffNumber,7,0);


			CreditsLabel->Caption = FloatToStrF(m_pPlayers[DCMDB_PLAYER_MONEY],ffNumber,7,0);
			CreditsGain->Caption =  L"+"+FloatToStrF(m_pPlayers[DCMDB_PLAYER_MONEY_GAIN],ffNumber,7,0);
			CreditsSpent->Caption = L"-"+FloatToStrF(m_pPlayers[DCMDB_PLAYER_MONEY_SPENT],ffNumber,7,0);
		}
	}
}
//---------------------------------------------------------------------------
