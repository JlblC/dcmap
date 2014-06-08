//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PlayerProp.h"
#include "sDialogs.hpp"
#include "IconsListMan.h"
#include "Utils.h"
#include "Interface.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sComboBox"
#pragma link "sEdit"
#pragma link "sGroupBox"
#pragma link "sSpeedButton"
#pragma link "sCheckBox"
#pragma link "sLabel"
#pragma link "sComboBoxes"
#pragma link "sPanel"
#pragma link "sColorSelect"
#pragma link "sRadioButton"
#pragma link "sMemo"
#pragma resource "*.dfm"

TPlayerEditPropFrame *PlayerEditPropFrame;

static CFramePanelWrapper<TPlayerEditPropFrame> sWrap("PListEditPropPage_players","01BaseInfo");

const int MaxDipShowing = 6;

//---------------------------------------------------------------------------
__fastcall TPlayerEditPropFrame::TPlayerEditPropFrame(TComponent* Owner)
    : TBaseModuleFrame(Owner)
{
	Caption = L"Параметры";
}
//---------------------------------------------------------------------------
void TPlayerEditPropFrame::OnShow()
{
	DipRadio->Items->AddObject(L"Не определены",(TObject*)DCMAP_DIP_UNDEFINED);

    int n=0;
    if(m_pDip->SelectAll())
    do
    {
    	if(n<=MaxDipShowing)
        {
    		DipRadio->Items->AddObject(m_pDip->GetWideName(),0);
        }
        else
        {
        	OtherDip->Items->AddObject(m_pDip->GetWideName(),0);
        }
        n++;
    }while(m_pDip->Next());

    if(OtherDip->Items->Count == 0)
    {
        OtherDip->Enabled=false;
        OtherDipCheck->Enabled=false;
    }
}

bool TPlayerEditPropFrame::OnInit()
{
	if(!m_pDip.init(m_pCore,"dip"))return false;

    DCMAP_GET_STORAGE(m_pCore,m_pRaces,IDCMapRacesDataStorage,races);
    DCMAP_GET_STORAGE(m_pCore,m_pPlayers,IDCMapPlayersDataStorage,players);
    m_pOrg = m_pCore->GetDataSourceTyped<IDCMapOrganizationDataStorage>("organizations");
    m_pIcons = m_pCore->GetDataSourceTyped<IDCMapIconsDataStorage>("icons");
    if(!m_pOrg)return false;
    if(!m_pIcons)return false;

    TIconsList* pIconsList = m_pCore->GetServiceTyped<TIconsList>("VCLIconsList");
    if(!pIconsList)return false;
    pIconsList->FillItems(IconCombo);
    return true;
}

int TPlayerEditPropFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
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

void TPlayerEditPropFrame::OnCansel()
{
    Changed = false;
}

void TPlayerEditPropFrame::OnApply()
{
    if(!Changed)return;
    DCMapPlayerInfo inf;

    bool insert=false;
    if(m_pPlayers->SelectID(m_Ref))
    {
       inf = *m_pPlayers->GetData();
    }
    else
    {
        m_pPlayers->FillDefaults(&inf);
        insert=true;
    }

    if(!RaceComboBox->Text.IsEmpty())
    {
		m_pRaces->RetrieveByWideName(RaceComboBox->Text.w_str(),&(inf.race));
    }
    else inf.race.Invalidate();

    if(!OrgComboBox->Text.IsEmpty())
    {
		m_pOrg->RetrieveByWideName(OrgComboBox->Text.w_str(),&(inf.organization));
    }
    else inf.organization.Invalidate();

	//strncpy(inf.name,NameEdit->Text.c_str(),DCMapPlayerNameLen);

    inf.dip.Invalidate();
    
    if(DipRadio->ItemIndex >= 0)
    {
		if(m_pDip->SelectWideName(DipRadio->Items->Strings[DipRadio->ItemIndex].w_str()))
        {
            inf.dip = *m_pDip->GetID();
        }
    }
    else if(OtherDip->ItemIndex >= 0)
    {
		if(m_pDip->SelectWideName(OtherDip->Items->Strings[OtherDip->ItemIndex].w_str()))
        {
            inf.dip = *m_pDip->GetID();
		}
    }

    //inf.dip = (EDCMapDiplomatyRelationType)DipRadio->Items->Objects[DipRadio->ItemIndex];

    inf.relation_type = 0;
    inf.relation_type |= RelMultCheckBox->Checked?1:0;
    inf.relation_type |= VassalCheckBox->Checked?2:0;

	if(inf.relation_type && m_pPlayers->SelectWideName(RelationComboBox->Text.w_str()))
    {
        inf.relation = m_pPlayers->GetData()->id;
    }
    else inf.relation.Invalidate();

    inf.color1 = PlayerColor1->ColorValue;
    inf.color2 = PlayerColor2->ColorValue;

	if(m_pIcons->SelectWideName(IconCombo->Text.w_str()))
    {
       inf.icon = *m_pIcons->GetID();
    }
    else inf.icon.Invalidate();

    if(!m_pPlayers->SelectID(m_Ref))return;
    if(!m_pPlayers->Update())return;
	m_pPlayers->SetData(&inf);
	m_pPlayers->SetWideNotes(NotesMemo->Text.w_str());

	m_pPlayers->SetWideName(NameEdit->Text.w_str());

    m_pPlayers->Post();
}

void TPlayerEditPropFrame::UpdateData()
{
    if(!m_pPlayers->SelectID(m_Ref)) return;

    DCMapPlayerInfo const* pinf = m_pPlayers->GetData();

    NameEdit->OnChange = 0;
    NameEdit->Text = pinf->name;
    NameEdit->OnChange = NameEditChange;

	if(pinf->game_id > 0)
		SURL->URL = L"http://www.the-game.ru/ru/frames/playerinfo/on/"+IntToStr((int)pinf->game_id);
	else
	SURL->URL = L"http://www.the-game.ru/ru/frames/playersinfo/on/0?q="+NameEdit->Text;

	NotesMemo->Text = m_pPlayers->GetNotes();

    if(m_pDip->SelectID(pinf->dip))
    {
        DipRadio->ItemIndex = DipRadio->Items->IndexOf(m_pDip->GetWideName());
        OtherDip->ItemIndex = OtherDip->Items->IndexOf(m_pDip->GetWideName());
        if(DipRadio->ItemIndex < 0 && OtherDip->ItemIndex >= 0) OtherDipCheck->Checked=true;
    }
    else
    {
       DipRadio->ItemIndex = 0;
    }

    RaceComboBox->Items->Clear();
    if(m_pRaces->SelectAll())
    {
       do
       {
            RaceComboBox->Items->Add(m_pRaces->GetData()->name);
       }while(m_pRaces->Next());
    }
    if(m_pRaces->SelectID(pinf->race))
    {
        RaceComboBox->Text = m_pRaces->GetData()->name;
    }
    else RaceComboBox->Text = "";

    IconCombo->ItemIndex = -1;
    if(m_pIcons->SelectID(pinf->icon))
    {
        IconCombo->ItemIndex = IconCombo->Items->IndexOf(m_pIcons->GetWideName());
    }
    if(IconCombo->ItemIndex < 0)IconCombo->ItemIndex = 0;

    ///////////////////////////////////////////////////
    OrgComboBox->Items->Clear();
    if(m_pOrg->SelectAll())
    {
       do
       {
            OrgComboBox->Items->Add(m_pOrg->GetData()->name);
       }while(m_pOrg->Next());
    }
    if(m_pOrg->SelectID(pinf->organization))
    {
        OrgComboBox->Text = m_pOrg->GetData()->name;
    }
	else OrgComboBox->Text = L"";
    ///////////////////////////////////////////////////////

    DataReference crel      = pinf->relation;
    int crel_type = pinf->relation_type;

    if(crel_type == 1)
    {
        RelMultCheckBox->Checked = true;
        VassalCheckBox->Checked = false;
        RelationComboBox->Enabled = true;
    }
    else if(crel_type == 2)
    {
        RelMultCheckBox->Checked = false;
        VassalCheckBox->Checked = true;
        RelationComboBox->Enabled = true;
    }
    else if(crel_type == 3)
    {
        RelMultCheckBox->Checked = true;
        VassalCheckBox->Checked = true;
        RelationComboBox->Enabled = true;
    }
    else
    {
        RelMultCheckBox->Checked = false;
        VassalCheckBox->Checked = false;
        RelationComboBox->Enabled = false;
    }

    RelationComboBox->Clear();
    m_pPlayers->SelectAll();
    do
    {
        DCMapPlayerInfo const* pl = m_pPlayers->GetData();
        if(pl->id != m_Ref)
            RelationComboBox->Items->Add(pl->name);
    }while(m_pPlayers->Next());

    if(crel_type && m_pPlayers->SelectID(crel))
    {
        RelationComboBox->ItemIndex = RelationComboBox->Items->IndexOf(m_pPlayers->GetData()->name);
    }
    else RelationComboBox->ItemIndex=-1;

    PlayerColor1->ColorValue = pinf->color1;
    PlayerColor2->ColorValue = pinf->color2;

    Changed = false;
}

void __fastcall TPlayerEditPropFrame::DataChange(TObject *Sender)
{
    Changed = true;
}
//---------------------------------------------------------------------------

void __fastcall TPlayerEditPropFrame::RelMultCheckBoxClick(TObject *Sender)
{
    RelationComboBox->Enabled = RelMultCheckBox->Checked || VassalCheckBox->Checked;
    Changed=true;
}
//---------------------------------------------------------------------------

void __fastcall TPlayerEditPropFrame::sSpeedButton1Click(TObject *Sender)
{
    if(RaceComboBox->Text.Length())
    {
		if(m_pRaces->RetrieveByWideName(RaceComboBox->Text.w_str()))
        {
            DataReference ref = *m_pRaces->GetID();
            m_pCore->GetCoreUI()->OpenListTableEdit("races",&ref);
        }
    }
}
//---------------------------------------------------------------------------


bool TPlayerEditPropFrame::OnDataTest()
{
	if(!Changed)return true;
	if(m_pPlayers->SelectWideName(NameEdit->Text.w_str()))
	{
        if(*m_pPlayers->GetID() != m_Ref)
        {
            Dialogs::TMsgDlgButtons Buttons;Buttons << mbOK;
            sMessageDlg(L"Ошибка",L"Существует другой игрок с таким именем!",mtError,Buttons,0);
            NameEdit->SetFocus();
            return false;
        }
    }
    return true;
}


void __fastcall TPlayerEditPropFrame::sSpeedButton2Click(TObject *Sender)
{
    if(OrgComboBox->Text.Length())
    {
        if(m_pOrg->RetrieveByWideName(OrgComboBox->Text.w_str()))
        {
            DataReference ref = *m_pOrg->GetID();
            m_pCore->GetCoreUI()->OpenListTableEdit("organizations",&ref);
        }
    }    
}
//---------------------------------------------------------------------------


void __fastcall TPlayerEditPropFrame::DipRadioClick(TObject *Sender)
{
	if(DipRadio->ItemIndex>=0)
    {
        OtherDipCheck->Checked=false;
        //OtherDip->ItemIndex=-1;
    }
    Changed=true;
}
//---------------------------------------------------------------------------

void __fastcall TPlayerEditPropFrame::OtherDipCheckClick(TObject *Sender)
{
    DipRadio->ItemIndex=-1;
    Changed=true;
}
//---------------------------------------------------------------------------

void __fastcall TPlayerEditPropFrame::OtherDipChange(TObject *Sender)
{
	if(OtherDip->ItemIndex>=0)
    {
        DipRadio->ItemIndex=-1;
        OtherDipCheck->Checked=true;
    }
    Changed=true;
}
//---------------------------------------------------------------------------


void __fastcall TPlayerEditPropFrame::NameEditChange(TObject *Sender)
{
	SetNewKeyValue(NameEdit->Text);
	Changed=true;
}
//---------------------------------------------------------------------------

