//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "math.h"
#include "GeoValue.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sComboBox"
#pragma link "sRadioButton"
#pragma link "sLabel"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TGeoValueFrame *GeoValueFrame;

static CFramePanelWrapper<TGeoValueFrame> sWrap("PFrameBar","GeoValue");

//---------------------------------------------------------------------------
__fastcall TGeoValueFrame::TGeoValueFrame(TComponent* Owner)
    : TBaseModuleFrame(Owner)
{
	m_pFilter=0;
    m_iPreHeight=69;
    Collapsed=false;
}

bool TGeoValueFrame::OnInit()
{
	ClearView();

	m_pPlanetMan = m_pWorkspace->GetServiceTyped<IDCMapPlanetManager>("PlanetManager");
	if(!m_pPlanetMan)return false;

    if(!m_pPlanets.init(m_pCore,"planets"))return false;
    if(!m_pPlayers.init(m_pCore,"players"))return false;
    if(!m_pRaces.init(m_pCore,"races"))return false;
    if(!m_pGeo.init(m_pCore,"geo"))return false;

	m_pGeoCalc = m_pWorkspace->GetServiceTyped<IDCMapGeoValueCalculator>("GeoValueCalculator");
	if(!m_pGeoCalc)return false;

    UpdateListData();
    UpdateData();

    return true;
}

void TGeoValueFrame::UpdateListData()
{
    RaceList->Items->Clear();
    PlayerList->Items->Clear();
    if(m_pRaces->SelectAll())
    {
        do
        {
			if(m_pRaces->GetData()->tmin != m_pRaces->GetData()->tmax)
				RaceList->Items->Add(m_pRaces->GetWideName());
		}while(m_pRaces->Next());
	}
	if(m_pPlayers->SelectAll())
	{
		do
		{
			DCMapPlayerInfo const* pinf = m_pPlayers->GetData();
			if(m_pRaces->SelectID(pinf->race))
			{
				if(m_pRaces->GetData()->tmin != m_pRaces->GetData()->tmax)
					PlayerList->Items->Add(pinf->name);
			}
        }while(m_pPlayers->Next());
    }
    RaceList->ItemIndex=RaceList->Items->IndexOf(LastRace);
    PlayerList->ItemIndex=PlayerList->Items->IndexOf(LastPlayer);

    RaceRadioClick(this);
}

//---------------------------------------------------------------------------
void __fastcall TGeoValueFrame::PlayerListChange(TObject *Sender)
{
	if(m_pPlayers->SelectWideName(PlayerList->Text.w_str()))
	{
		LastPlayer = PlayerList->Text;
		DCMapPlayerInfo const* pinf = m_pPlayers->GetData();
		if(m_pRaces->SelectID(pinf->race))
		{
			m_RaceInfo = *m_pRaces->GetData();
            RaceList->Text = m_RaceInfo.name;
			LastRace =  m_RaceInfo.name;
			UpdateData();
		}
		else
		{
			m_pRaces->FillDefaults(&m_RaceInfo);
		}
		RaceChanged();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGeoValueFrame::RaceListChange(TObject *Sender)
{
	if(m_pRaces->SelectWideName(RaceList->Text.w_str()))
    {
        LastRace = RaceList->Text;
        m_RaceInfo = *m_pRaces->GetData();
        PlayerList->Text="";
        LastPlayer="";
		UpdateData();
	}
	else
	{
		m_pRaces->FillDefaults(&m_RaceInfo);
	}
	RaceChanged();
}
//---------------------------------------------------------------------------
void __fastcall TGeoValueFrame::RaceRadioClick(TObject *Sender)
{
    if(PlayerRadio->Checked)
    {
        PlayerList->Visible=true;
        RaceList->Visible=false;
        if(PlayerList->ItemIndex < 0)
        {
            PlayerList->ItemIndex=0;
            LastPlayer = PlayerList->Text;
        }
        PlayerListChange(Sender);
    }
    else if(RaceRadio->Checked)
    {
        PlayerList->Visible=false;
        RaceList->Visible=true;
        if(RaceList->ItemIndex < 0)
        {
            RaceList->ItemIndex=0;
            LastRace = RaceList->Text;
        }
        RaceListChange(Sender);
    }
}
//---------------------------------------------------------------------------
int TGeoValueFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
    case DCMM_DATA_CHANGED:
	  UpdateListData();
    case DCMM_PLANET_CHANGED:
      UpdateData();
      break;
    }

    return 0;
}

static float fmin(double a,double b){return a<b?a:b;}

void TGeoValueFrame::RaceChanged()
{
	bool r = m_RaceInfo.id.Valid();
	BasisImg->Visible = r;
	SecondImg->Visible = r;

	if(r)
	{
		InterfaceModule->RsrsImageList->Draw(BasisImg->Canvas,0,0,m_RaceInfo.basis,true);
		BasisImg->Repaint();
		InterfaceModule->RsrsImageList->Draw(SecondImg->Canvas,0,0,m_RaceInfo.second,true);
		SecondImg->Repaint();
	}
	m_pGeoCalc->OnMessage(DCMM_CURRENT_RACE_CHANGED,m_RaceInfo.id.id,m_RaceInfo.id.age,m_pPanelInterface);
	if(!m_pFilter)return;
	m_pFilter->OnMessage(DCMM_USER+0,m_RaceInfo.id.id,m_RaceInfo.id.age,m_pPanelInterface);

	UpdateData();
}

void TGeoValueFrame::ClearView()
{
    CBLabel->Caption = "";
    CSLabel->Caption = "";
    MBLabel->Caption = "";
    MSLabel->Caption = "";
    GrowthLabel->Caption= "";

    UpImage->Visible = false;
    DownImage->Visible = false;
    MiddleImage->Visible = false;
}

void TGeoValueFrame::UpdateData()
{
	if(!m_pFilter)
	{
		if(m_pWorkspace->SelectModuleByID("CGeoValFilter"))
		{
			m_pFilter = m_pWorkspace->Module()->Cast<IDCMapPlanetColorProvider>();
			RaceChanged();
		}
	}
	if(!m_pFilter)return;

    ClearView();

    // calculate growth
    float gr=0,cb=0,cs=0,mb=0,ms=0;
    int r;
    DataReference ref;

    if(m_pPlanetMan->GetCurrentPlanet(&ref))
     	r = m_pGeoCalc->CalculatePlanet(ref,&gr,&mb,&cb,&ms,&cs);
    else
    {
		int x,y;
        if(!m_pPlanetMan->GetCurrentPlanetCoords(&x,&y))return;
        if(!m_pGeo->SelectCoord(x,y))return;
     	r = m_pGeoCalc->CalculateGeo(&m_pGeo->GetData()->geo,&gr,&mb,&cb,&ms,&cs);
    }

    if(r)
    {
        if(gr >= 0)
		{
			TVarRec rec[] = {TVarRec(FloatToStrF(gr,ffFixed,7,2))};
			GrowthLabel->Caption =  String::Format(LSTRW(PopulationGrowPers),rec,sizeof(rec)/sizeof(TVarRec));
			if(r>0)UpImage->Visible = true;
            else MiddleImage->Visible = true;
        }
        else
        {
			TVarRec rec[] = {TVarRec(FloatToStrF(-gr,ffFixed,7,2))};
			GrowthLabel->Caption =  String::Format(LSTRW(PopulationExtinctionPers),rec,sizeof(rec)/sizeof(TVarRec));
			DownImage->Visible = true;
        }

        CBLabel->Caption = FloatToStr(cb);
        CSLabel->Caption = FloatToStr(cs);
        MBLabel->Caption = FloatToStr(mb);
		MSLabel->Caption = FloatToStr(ms);

	}
}

void TGeoValueFrame::LoadConfig(DCMapConfigWrapper cfg)
{
    cfg.section("GeoValueFrame");
    cfg["Race"] >> LastRace;
    cfg["Player"] >> LastPlayer;
    bool r=false;
    cfg["PlayerMode"] >> r;
    cfg["Collapsed"] >> Collapsed;
    if(Collapsed)
    {
        Collapsed = false;
        CollapseButtonClick(this);
    }

    UpdateListData();
    PlayerRadio->Checked = r;
    RaceRadioClick(this);
}

void TGeoValueFrame::SaveConfig(DCMapConfigWrapper cfg)
{
    cfg.section("GeoValueFrame");
    cfg["Race"] << LastRace;
    cfg["Player"] << LastPlayer;
    cfg["PlayerMode"] << PlayerRadio->Checked;
    cfg["Collapsed"] << Collapsed;
}

void __fastcall TGeoValueFrame::CollapseButtonClick(TObject *Sender)
{
	int h = m_iPreHeight;
    m_iPreHeight = Height;
    Height = h;
    Collapsed = !Collapsed;

	CollapseButton->Visible = !Collapsed;
	ExpandButton->Visible = Collapsed;
}
//---------------------------------------------------------------------------



