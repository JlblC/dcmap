//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PlanetInfo.h"
#include "dcmapsyslib.h"

#include "sGraphUtils.hpp"
#include "Clipbrd.hpp"
#include "Utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sLabel"
#pragma link "sFrameAdapter"
#pragma link "sBevel"
#pragma link "sButton"
#pragma link "sEdit"
#pragma link "sSpeedButton"
#pragma link "sPanel"
#pragma resource "*.dfm"
TPlanetInfoFrame *PlanetInfoFrame;

static CFramePanelWrapper<TPlanetInfoFrame> sWrap("PFrameBar","PlanetInfo");

//---------------------------------------------------------------------------
__fastcall TPlanetInfoFrame::TPlanetInfoFrame(TComponent* Owner)
    : TBaseModuleFrame(Owner)
{
    m_iPreHeight=85;
    Collapsed=false;
    m_iCurrentCopyMode=0;
}
//---------------------------------------------------------------------------

bool TPlanetInfoFrame::OnInit()
{
	m_pPlanetMan = m_pWorkspace->GetServiceTyped<IDCMapPlanetManager>("PlanetManager");
    m_pRecordExport = m_pCore->GetServiceTyped<IDCMapRecordExport>("RecordExport");
	if(!m_pPlanetMan)return false;
	if(!m_pRecordExport)return false;

	if(!m_pPlanets.init(m_pCore))return false;
	if(!m_pPlayers.init(m_pCore))return false;
	if(!m_pGeo.init(m_pCore))return false;

    DCMapConfigWrapper cfg(m_pCore->GetConfig());
	cfg.section("MapNavView");
    int num=0;
    cfg["ClipbrdExportNum"] >> num;
    for(int i=0;i<num;i++)
    {
    	TMenuItem* Item = new TMenuItem(this);
        UnicodeString val;
        cfg["ClipbrdExportName"+IntToStr(i)] >> val;
        Item->Caption = val;
        Item->Tag = i;
        Item->OnClick = CopyMenuClick;
        Item->GroupIndex=1;
        Item->RadioItem=true;
        CopyMenu->Items->Add(Item);
    }
    
    UpdateData();

    return true;
}

int TPlanetInfoFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
    case DCMM_CONFIG_CHANGED:
    	//OnInit();
        break;
    case DCMM_PLANET_CHANGED:
    case DCMM_DATA_CHANGED:
      UpdateData();
      break;
    }

    return 0;
}

void TPlanetInfoFrame::SetBoxHeight(TShape* sh,int h)
{
	h*= (BoxShape->Height+1)*0.01f;
    sh->Top = sh->Top+sh->Height-h;
    sh->Height = h;
}

void TPlanetInfoFrame::UpdateData()
{
    NameLabel->Caption = "";
    OwnerLabel->Caption = "";
    SetBoxHeight(OBox,0);
    SetBoxHeight(EBox,0);
    SetBoxHeight(MBox,0);
    SetBoxHeight(TBox,0);
	SetBoxHeight(SBox,0);
	SetBoxHeight(CorruptionOpBox,0);

	CorruptionBox->Height = 0;

    OLabel->Caption = "0";
    ELabel->Caption = "0";
    MLabel->Caption = "0";
    TLabel->Caption = "0";
    SLabel->Caption = "0";

    OLabel2->Caption = "0";
    ELabel2->Caption = "0";
    MLabel2->Caption = "0";
    TLabel2->Caption = "0";
    SLabel2->Caption = "0";

    WebGoStandart->URL = "";
	WebGoPaid->URL = "";
	WebGoUser->URL = "";


    //WebGoStandart->Enabled = false;
    //WebGoPaid->Enabled = false;


    CoordEdit->Text="";

    m_refOwner.Invalidate();
    m_refPlanet.Invalidate();

    DCMapPlanetGeoInfo const* geo=0;

    int x,y;
    DataReference ref;
    if(!m_pPlanetMan->GetCurrentPlanet(&ref))
    {
      if(!m_pPlanetMan->GetCurrentPlanetCoords(&x,&y))return;
      if(m_pGeo->SelectCoord(x,y))geo = &m_pGeo->GetData()->geo;
    }
    else
    {
        if(!m_pPlanets->SelectID(ref))return;
		const DCMapPlanetInfo* pinf = m_pPlanets->GetData();

        x = pinf->x;
        y = pinf->y;

        m_refPlanet = *m_pPlanets->GetID();

        if(m_pPlayers->SelectID(pinf->owner))
        {
			//OwnerLabel->Caption =  CutText(OwnerLabel->Canvas,m_pPlayers->GetWideName(),OwnerLabel->Width);
			OwnerLabel->Caption = m_pPlayers->GetWideName();
			m_refOwner = pinf->owner;
			if((bool)m_pPlayers[DCMDB_PLAYER_CALCULATECORRUPTION])
			{
				CorruptionBox->Height = (pinf->corruption+1)*((BoxShape->Height+2)*0.01f);
				SetBoxHeight(CorruptionOpBox,100-pinf->corruption);
			}
			int user_id = m_pPlayers[DCMDB_PLAYER_USER_ID];
			if(user_id)
				WebGoUser->URL = L"http://www.the-game.ru/ru/frames/playerinfo/on/"+IntToStr(user_id);
			else
				WebGoUser->URL = L"http://www.the-game.ru/ru/frames/playersinfo/on/0?q="+ m_pPlayers[DCMDB_PLAYER_NAME];
		}

        NameLabel->Caption =  CutText(NameLabel->Canvas,pinf->caption,NameLabel->Width);

        CoordEdit->Text=IntToStr(pinf->x)+":"+IntToStr(pinf->y);
        geo = &pinf->geo;

        if(!(geo->o||geo->e||geo->m||geo->t||geo->s))
        {
        	if(m_pGeo->SelectCoord(x,y))geo = &m_pGeo->GetData()->geo;
		}

	 }

     if(geo)
     {
        OLabel->Caption = IntToStr(geo->o);
        ELabel->Caption = IntToStr(geo->e);
        MLabel->Caption = IntToStr(geo->m);
        TLabel->Caption = IntToStr(geo->t);
        SLabel->Caption = IntToStr(geo->s);

        OLabel2->Caption = IntToStr(geo->o);
        ELabel2->Caption = IntToStr(geo->e);
        MLabel2->Caption = IntToStr(geo->m);
        TLabel2->Caption = IntToStr(geo->t);
        SLabel2->Caption = IntToStr(geo->s);

        SetBoxHeight(OBox,geo->o);
        SetBoxHeight(EBox,geo->e);
        SetBoxHeight(MBox,geo->m);
        SetBoxHeight(TBox,geo->t);
		SetBoxHeight(SBox,geo->s);
	 }

    CoordEdit->Text=IntToStr(x)+":"+IntToStr(y);

    WebGoPaid->URL = (String)"http://www.the-game.ru/planet/?planetid="+
             IntToStr(x)+":"+IntToStr(y);

    WebGoStandart->URL = (String)"http://free.the-game.ru/planet/?planetid="+
             IntToStr(x)+":"+IntToStr(y);

    WebGoStandart->Enabled = true;
    WebGoPaid->Enabled = true;

}

void TPlanetInfoFrame::OnShow()
{
    //UpdateData();
    //CoordEdit->Visible = true;
    m_pCore->GetCoreUI()->UpdateSkins();
}

void __fastcall TPlanetInfoFrame::CopyButtonClick(TObject *Sender)
{
    DataReference ref;
    String Text;
    if(m_pPlanetMan->GetCurrentPlanet(&ref))
    {
        if(!m_pPlanets->SelectID(ref))return;
        DCMapConfigWrapper cfg(m_pCore->GetConfig());
		cfg.section("MapNavView");
        UnicodeString val;
        cfg["ClipbrdExportFormat"+IntToStr(m_iCurrentCopyMode)] >> val;
		if(!val.IsEmpty())
        	m_pRecordExport->ExportClipboard("planets",val.w_str(),&ref);
    }
    else
    {
       int x,y;
       if(m_pPlanetMan->GetCurrentPlanetCoords(&x,&y))
      	   Text=IntToStr(x)+":"+IntToStr(y);
    	ClipboardPutW(Text);
    }
}
//---------------------------------------------------------------------------

void __fastcall TPlanetInfoFrame::CopyMenuClick(TObject *Sender)
{
	((TMenuItem*)Sender)->Checked = true;
    m_iCurrentCopyMode = ((TMenuItem*)Sender)->Tag;
    CopyButtonClick(Sender);
}
//---------------------------------------------------------------------------
void TPlanetInfoFrame::LoadConfig(DCMapConfigWrapper cfg)
{
	cfg.section("PlanetInfo");
	if(cfg["CopyMode"] >> m_iCurrentCopyMode)
    {
		//if(CopyMode == "Geo")GeoMenuItem->Checked=true;
		//else if(CopyMode == "All")AllDataMenuItem->Checked=true;
		//else CoordsMenuItem->Checked=true;
        for(int i=0;i<CopyMenu->Items->Count;i++)
        {
            if(CopyMenu->Items->Items[i]->Tag == m_iCurrentCopyMode)
            {
                CopyMenu->Items->Items[i]->Checked = true;
            }
        }
    }
    cfg["Collapsed"] >> Collapsed;
    if(Collapsed)
    {
        Collapsed = false;
        CollapseButtonClick(this);
	}
	CollapseButton->Visible = !Collapsed;
	ExpandButton->Visible = Collapsed;
}

void TPlanetInfoFrame::SaveConfig(DCMapConfigWrapper cfg)
{
	cfg.section("PlanetInfo");
    cfg["CopyMode"] << m_iCurrentCopyMode;
    cfg["Collapsed"] << Collapsed;
}
//---------------------------------------------------------------------------

void __fastcall TPlanetInfoFrame::OwnerLabelDblClick(TObject *Sender)
{
	if(!m_refOwner.ValidID())return;
	m_pCore->GetCoreUI()->OpenListTableEdit("players",&m_refOwner);
}
//---------------------------------------------------------------------------

void __fastcall TPlanetInfoFrame::NameLabelDblClick(TObject *Sender)
{
	if(!m_refPlanet.ValidID())return;
	m_pCore->GetCoreUI()->OpenListTableEdit("planets",&m_refPlanet);
}
//---------------------------------------------------------------------------

void __fastcall TPlanetInfoFrame::CollapseButtonClick(TObject *Sender)
{
	Collapsed = !Collapsed;
    if(Collapsed)ExPanel->Visible = false;
	int h = m_iPreHeight;
    m_iPreHeight = Height;
    Height = h;
	if(!Collapsed)ExPanel->Visible = true;

	CollapseButton->Visible = !Collapsed;
	ExpandButton->Visible = Collapsed;
}
//---------------------------------------------------------------------------

void __fastcall TPlanetInfoFrame::CoordEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    if(Key == VK_RETURN)
    {
    	int show_mode = Shift.Contains(ssShift)?2:1;
    	TPoint pt = StrToPoint(CoordEdit->Text);

    	if(m_pPlanets->SelectCoord(pt.x,pt.y))
        {
           m_pPlanetMan->SetCurrentPlanet(m_pPlanets->GetID(),pt.x,pt.y,show_mode);
        }
        else
        {
           m_pPlanetMan->SetCurrentPlanet(0,pt.x,pt.y,show_mode);
        }
        m_pWorkspace->GetUI()->ActivateView();
    }
}
//---------------------------------------------------------------------------

