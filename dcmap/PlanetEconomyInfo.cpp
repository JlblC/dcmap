//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PlanetEconomyInfo.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sLabel"
#pragma link "acPNG"
#pragma link "sGroupBox"
#pragma resource "*.dfm"
TPlanetEconomyInfoFrame *PlanetEconomyInfoFrame;

static CFramePanelWrapper<TPlanetEconomyInfoFrame> sWrap("PFrameBar","PlanetEconomyInfo");

//---------------------------------------------------------------------------
__fastcall TPlanetEconomyInfoFrame::TPlanetEconomyInfoFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------
bool TPlanetEconomyInfoFrame::OnInit()
{
	if(!m_pPlanets.init(m_pCore))return false;
	if(!m_pPlayers.init(m_pCore))return false;

	m_pPlanetMan = m_pWorkspace->GetServiceTyped<IDCMapPlanetManager>("PlanetManager");
	if(!m_pPlanetMan)return false;

	m_pCalc = m_pCore->GetServiceTyped<IDCMapPlanetCalculator>("PlanetCalculator");
	if(!m_pCalc)return false;

	return true;
}

int TPlanetEconomyInfoFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	switch (Msg)
	{
	case DCMM_CONFIG_CHANGED:
		break;
	case DCMM_PLANET_CHANGED:
	case DCMM_DATA_CHANGED:
	  UpdateData();
	  break;
	}
	return 0;
}

String FI2STR(dcmapREAL v){return FloatToStrF(v,ffNumber,7,0);}

void TPlanetEconomyInfoFrame::ShowData(bool show)
{
	BasisImg->Visible = show;
	SecondImg->Visible = show;
	SecondImg->Visible = show;
	MoneyImg->Visible = show;
	ProductionImg->Visible = show;

	BasisLabel->Visible = show;
	SecondLabel->Visible = show;
	MoneyLabel->Visible = show;
	ProductionLabel->Visible = show;

	PeopleGroupBox->Visible = show;
	Corruption->Visible = show;
	CorruptionLabel->Visible = show;;

}

void TPlanetEconomyInfoFrame::UpdateData()
{
	DataReference ref;
	if(!m_pPlanetMan->GetCurrentPlanet(&ref))
	{
		ShowData(false);
		return;
	}
	if(!m_pCalc->ProcessPlanetParams(ref,&m_PlanetInfo))
	{
		ShowData(false);
		return;
	}
	ShowData(true);

	InterfaceModule->RsrsImageList->Draw(BasisImg->Canvas,0,0,m_PlanetInfo.basis_ind,true);
	BasisImg->Repaint();
	InterfaceModule->RsrsImageList->Draw(SecondImg->Canvas,0,0,m_PlanetInfo.second_ind,true);
	SecondImg->Repaint();
	InterfaceModule->RsrsImageList->Draw(MoneyImg->Canvas,0,0,3,true);
	SecondImg->Repaint();

	BasisLabel->Caption   =  IntToStr((int)m_PlanetInfo.basis);
	SecondLabel->Caption  =  IntToStr((int)m_PlanetInfo.second);
	MoneyLabel->Caption  =  IntToStr((int)m_PlanetInfo.credits);
	ProductionLabel->Caption =   IntToStr((int)m_PlanetInfo.production);

	PeopleFree->Caption =     FI2STR(m_PlanetInfo.people_free);
	PeopleTotal->Caption =   FI2STR(m_PlanetInfo.people);
	PeopleGrowth->Caption =   (Floor(m_PlanetInfo.growth*100)>0?L"+":L"")+FI2STR(m_PlanetInfo.growth)+L"%";

	Corruption->Caption =   IntToStr(m_PlanetInfo.corruption)+L"%";
}




