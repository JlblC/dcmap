//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PlanetProp.h"
#include "dcmapsyslib.h"
#include "CoreUIManager.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sFrameAdapter"
#pragma link "sLabel"
#pragma link "sComboBox"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma link "sGauge"
#pragma link "sPanel"
#pragma link "sMemo"
#pragma link "sFrameBar"
#pragma link "sScrollBox"
#pragma link "sSpeedButton"
#pragma link "sColorSelect"
#pragma link "sComboBoxes"
#pragma resource "*.dfm"
TPlanetParamFrame *PlanetParamFrame;
//---------------------------------------------------------------------------
__fastcall TPlanetParamFrame::TPlanetParamFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
	NameChanged=false;
}
//---------------------------------------------------------------------------
static CFramePanelWrapper<TPlanetParamFrame> sWrap("PListEditPropPage_planets","00BaseInfo");

void TPlanetParamFrame::OnApply()
{
	if(!Changed)return;
	DCMapPlanetInfo inf;
	if(m_pPlanets->SelectID(m_Ref))
	{
	   inf = *m_pPlanets->GetData();
	}
	else return;

    //m_pPlanets->BeginUpdate();

    //strncpy(inf.caption,NameEdit->Text.c_str(),DCMapPlanetCaptionLen);

    inf.o = EditO->Value;
    inf.e = EditE->Value;
    inf.m = EditM->Value;
    inf.t = EditT->Value;
    inf.s = EditS->Value;

	inf.turn = EditTurn->Value;

	inf.color1 = PlanetColor1->ColorValue;
    inf.color2 = PlanetColor2->ColorValue;

	if(m_pIcons->SelectWideName(IconCombo1->Text.w_str()))
    {
       inf.icon1 = *m_pIcons->GetID();
    }
    else inf.icon1.Invalidate();

	if(m_pIcons->SelectWideName(IconCombo2->Text.w_str()))
    {
       inf.icon2 = *m_pIcons->GetID();
    }
    else inf.icon2.Invalidate();

    if(OwnerComboBox->Text.Length())
    {
		m_pPlayers->RetrieveByWideName(OwnerComboBox->Text.w_str(),&inf.owner);
    }
    else
    {
        inf.owner.Invalidate();
    }

    if(!m_pPlanets->SelectID(m_Ref))return;
    if(!m_pPlanets->Update())return;

    m_pPlanets->SetData(&inf);
    m_pPlanets->SetWideNotes(CommentMemo->Text.w_str());
    m_pPlanets->SetWideName(NameEdit->Text.w_str());

    m_pPlanets[DCMDB_PLANET_KEEP_NAME] = KeepNameBtn->Down;
    m_pPlanets[DCMDB_PLANET_SYNCHRONIZE_NAME] = SynchronizeBtn->Down;

    dcmapDBREF RefPlanet;
    m_pPlanets->Post(&RefPlanet);

    if(NameChanged && SynchronizeBtn->Enabled && SynchronizeBtn->Down)
    {
         m_pActions->Insert();
         m_pActions[DCMDB_ACTION_TYPE] = DCMACT_CHANGE_PLANET_NAME;
         m_pActions[DCMDB_ACTION_PLANET] = RefPlanet;
         m_pActions[DCMDB_ACTION_OLD_NAME] = OldPlanetName;
         m_pActions[DCMDB_ACTION_NEW_NAME] = NameEdit->Text;
         m_pActions[DCMDB_ACTION_OWNER] = inf.loader;
         m_pActions->Post();
    }
    Changed = false;
    NameChanged = false;

}

void TPlanetParamFrame::OnCansel()
{
	
}

bool TPlanetParamFrame::OnInit()
{
    if(!m_pPlanets.init(m_pCore))return false;
    if(!m_pPlayers.init(m_pCore))return false;
    if(!m_pIcons.init(m_pCore))return false;
    if(!m_pActions.init(m_pCore))return false;

    TIconsList* pIconsList = m_pCore->GetServiceTyped<TIconsList>("VCLIconsList");
    if(!pIconsList)return false;

    pIconsList->FillItems(IconCombo1);
	pIconsList->FillItems(IconCombo2);

    return true;
}

int TPlanetParamFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
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

void TPlanetParamFrame::UpdateData()
{
	if(!m_pPlanets->SelectID(m_Ref)) return;
	DCMapPlanetInfo const* pinf = m_pPlanets->GetData();

	OwnerComboBox->Items->Clear();
	if(m_pPlayers->SelectAll())
	{
		do
		{
			OwnerComboBox->Items->Add(m_pPlayers->GetWideName());
		}
		while(m_pPlayers->Next());
	}

    NameEdit->OnChange=0;
    OldPlanetName = pinf->caption;
	NameEdit->Text = pinf->caption;
    NameEdit->OnChange=NameEditChange;

    EditO->Value = pinf->o;
    EditE->Value = pinf->e;
    EditM->Value = pinf->m;
    EditT->Value = pinf->t;
	EditS->Value = pinf->s;

	EditTurn->Value =  pinf->turn;

    BaseName = IntToStr(pinf->x)+":"+IntToStr(pinf->y);

    PlanetColor1->ColorValue = pinf->color1;
    PlanetColor2->ColorValue = pinf->color2;

    KeepNameBtn->Down = pinf->Flags & DCMAP_PF_KeepName;

    SynchronizeBtn->Visible = pinf->Flags & DCMAP_PF_My;
    SynchronizeBtn->Down = pinf->Flags & DCMAP_PF_SynchronizeName;

    CommentMemo->Text = m_pPlanets->GetWideNotes();

    if(m_pPlayers->SelectID(pinf->owner))
    {
        OwnerComboBox->Text = m_pPlayers->GetWideName();
    }
    else
    {
        OwnerComboBox->Text = "";
    }

    IconCombo1->ItemIndex = -1;
    if(m_pIcons->SelectID(pinf->icon1))
    {
        IconCombo1->ItemIndex = IconCombo1->Items->IndexOf(m_pIcons->GetWideName());
    }
    if(IconCombo1->ItemIndex < 0)IconCombo1->ItemIndex = 0;

    IconCombo2->ItemIndex = -1;
    if(m_pIcons->SelectID(pinf->icon2))
    {
        IconCombo2->ItemIndex = IconCombo2->Items->IndexOf(m_pIcons->GetWideName());
    }
    if(IconCombo2->ItemIndex < 0)IconCombo2->ItemIndex = 0;

    Changed = false;
    NameChanged=false;
}

void __fastcall TPlanetParamFrame::DataChange(TObject *Sender)
{
	Changed = true;
}
//---------------------------------------------------------------------------

void __fastcall TPlanetParamFrame::sSpeedButton1Click(TObject *Sender)
{
    if(OwnerComboBox->Text.Length())
    {
        if(m_pPlayers->RetrieveByWideName(OwnerComboBox->Text.w_str()))
        {
            DataReference ref = m_pPlayers->GetData()->id;
            GetDCMapCore()->GetCoreUI()->OpenListTableEdit("players",&ref);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TPlanetParamFrame::NameEditChange(TObject *Sender)
{
    if(NameEdit->Text.IsEmpty())
    	SetNewKeyValue(BaseName);
    else
    	SetNewKeyValue(BaseName+L" - "+NameEdit->Text);
    Changed=true;
    NameChanged=true;
}
//---------------------------------------------------------------------------

