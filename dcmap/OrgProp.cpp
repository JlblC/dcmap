//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OrgProp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sEdit"
#pragma link "sGroupBox"
#pragma link "sColorSelect"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
#include "sDialogs.hpp"
TOrgPropFrame *OrgPropFrame;

static CFramePanelWrapper<TOrgPropFrame> sWrap("PListEditPropPage_organizations","BaseInfo");

//---------------------------------------------------------------------------
__fastcall TOrgPropFrame::TOrgPropFrame(TComponent* Owner)
    : TBaseModuleFrame(Owner)
{
    m_bChanged = false;
}
//---------------------------------------------------------------------------
void TOrgPropFrame::OnShow()
{
	DipRadio->Items->AddObject(LSTRW(DIP_UNDEFINED),(TObject*)DCMAP_DIP_UNDEFINED);
	DipRadio->Items->AddObject(LSTRW(DIP_NEUTRALS),(TObject*)DCMAP_DIP_NEUTRALS);
	DipRadio->Items->AddObject(LSTRW(DIP_ALLY),(TObject*)DCMAP_DIP_ALLY);
	DipRadio->Items->AddObject(LSTRW(DIP_ENEMIES),(TObject*)DCMAP_DIP_ENEMIES);
	DipRadio->Items->AddObject(LSTRW(DIP_POTENTIAL_ALLY),(TObject*)DCMAP_DIP_POTENTIAL_ALLY);
	DipRadio->Items->AddObject(LSTRW(DIP_POTENTIAL_ENEMIES),(TObject*)DCMAP_DIP_POTENTIAL_ENEMIES);
}

bool TOrgPropFrame::OnInit()
{
   m_pOrg = m_pCore->GetDataSourceTyped<IDCMapOrganizationDataStorage>("organizations");
   if(!m_pOrg)return false;
   return true;                       
}

int TOrgPropFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
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

void TOrgPropFrame::OnCansel()
{
    m_bChanged = false;
}

void TOrgPropFrame::OnApply()
{
    if(!m_bChanged)return;
    DCMapOrganizationInfo inf;

    if(!m_pOrg->SelectID(m_Ref)) return;
    
    inf = *m_pOrg->GetData();

	inf.dip = (EDCMapDiplomatyRelationType)DipRadio->Items->Objects[DipRadio->ItemIndex];
    inf.color1 = OrgColor1->ColorValue;
    inf.color2 = OrgColor2->ColorValue;

	m_pOrg->Update();
	m_pOrg->SetData(&inf);
	m_pOrg->SetWideName(NameEdit->Text.w_str());
	m_pOrg->Post();
}

void TOrgPropFrame::UpdateData()
{
    if(!m_pOrg->SelectID(m_Ref)) return;

    DCMapOrganizationInfo const* pinf = m_pOrg->GetData();

    NameEdit->OnChange = 0;
    NameEdit->Text = pinf->name;
    NameEdit->OnChange = NameEditChange;

    OrgColor1->ColorValue = pinf->color1;
    OrgColor2->ColorValue = pinf->color2;

    for(int i=0;i<DipRadio->Items->Count;i++)
    {
        if(DipRadio->Items->Objects[i] == (TObject*)pinf->dip)
        {
           DipRadio->ItemIndex = i;
           break;
        }
    }
    m_bChanged = false;
}


void __fastcall TOrgPropFrame::NameEditChange(TObject *Sender)
{
    SetNewKeyValue(NameEdit->Text);
    m_bChanged=true;
}
//---------------------------------------------------------------------------

bool TOrgPropFrame::OnDataTest()
{
    if(!m_bChanged)return true;
	if(m_pOrg->SelectWideName(NameEdit->Text.w_str()))
    {
		if(*m_pOrg->GetID() != m_Ref)
        {
            Dialogs::TMsgDlgButtons Buttons;Buttons << mbOK;
            sMessageDlg("Ошибка","Существует другая организация с таким именем!",mtError,Buttons,0);
            NameEdit->SetFocus();
            return false;
        }
    }
    return true;
}
void __fastcall TOrgPropFrame::DataChange(TObject *Sender)
{
    m_bChanged=true;
}
//---------------------------------------------------------------------------

