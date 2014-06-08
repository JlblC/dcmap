//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DipProp.h"
#include "sDialogs.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sEdit"
#pragma link "sColorSelect"
#pragma link "sSpeedButton"
#pragma link "sGroupBox"
#pragma resource "*.dfm"
TDipEditPropFrame *DipEditPropFrame;

static CFramePanelWrapper<TDipEditPropFrame> sWrap("PListEditPropPage_dip","BaseInfo");

//---------------------------------------------------------------------------
__fastcall TDipEditPropFrame::TDipEditPropFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
	Caption = "Параметры";
}
//---------------------------------------------------------------------------

bool TDipEditPropFrame::OnInit()
{
	if(!m_pDip.init(m_pCore,"dip"))return false;


	DipRadio->Items->AddObject(LSTRW(DIP_UNDEFINED),(TObject*)DCMAP_DIP_UNDEFINED);
	DipRadio->Items->AddObject(LSTRW(DIP_NEUTRALS),(TObject*)DCMAP_DIP_NEUTRALS);
	DipRadio->Items->AddObject(LSTRW(DIP_ALLY),(TObject*)DCMAP_DIP_ALLY);
	DipRadio->Items->AddObject(LSTRW(DIP_ENEMIES),(TObject*)DCMAP_DIP_ENEMIES);
	DipRadio->Items->AddObject(LSTRW(DIP_LORD),(TObject*)DCMAP_DIP_LORD);
	DipRadio->Items->AddObject(LSTRW(DIP_VASSAL),(TObject*)DCMAP_DIP_VASSAL);
    DipRadio->Items->AddObject(LSTRW(DIP_MULT),(TObject*)DCMAP_DIP_MULT);

    return true;
}

int TDipEditPropFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
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

void TDipEditPropFrame::OnCansel()
{
    Changed = false;
}

void TDipEditPropFrame::OnApply()
{
    if(!Changed)return;
    DCMapDipInfo inf;

    bool insert=false;
    if(!m_pDip->SelectID(m_Ref))return;
    inf = *m_pDip->GetData();
    m_pDip->Update();

    inf.color1 = DipColor->ColorValue;
    inf.dipid = DipRadio->ItemIndex;

    m_pDip->SetData(&inf);
	m_pDip->SetWideName(NameEdit->Text.w_str());
    m_pDip->Post();
    Changed = false;
}

void TDipEditPropFrame::UpdateData()
{
    if(!m_pDip->SelectID(m_Ref)) return;

    DCMapDipInfo const* pinf = m_pDip->GetData();

    DipColor->ColorValue = pinf->color1;

    NameEdit->OnChange = 0;
    NameEdit->Text = pinf->name;
    NameEdit->OnChange = NameEditChange;

    DipRadio->ItemIndex = pinf->dipid;

    Changed = false;
}

bool TDipEditPropFrame::OnDataTest()
{
    if(!Changed)return true;
    if(m_pDip->SelectWideName(NameEdit->Text.w_str()))
    {
        if(*m_pDip->GetID() != m_Ref)
        {
            Dialogs::TMsgDlgButtons Buttons;Buttons << mbOK;
            sMessageDlg("Ошибка","Существует другое дип. отношение с таким именем!",mtError,Buttons,0);
            NameEdit->SetFocus();
            return false;
        }
    }
    return true;
}
void __fastcall TDipEditPropFrame::NameEditChange(TObject *Sender)
{
    SetNewKeyValue(NameEdit->Text);
    Changed=true;	
}
//---------------------------------------------------------------------------
void __fastcall TDipEditPropFrame::OnChange(TObject *Sender)
{
	Changed=true;	
}
//---------------------------------------------------------------------------
