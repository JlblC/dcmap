//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LayerProp.h"
#include "sDialogs.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sEdit"
#pragma link "sComboBox"
#pragma link "sCheckBox"
#pragma resource "*.dfm"
TLayerEditFrame *LayerEditFrame;

static CFramePanelWrapper<TLayerEditFrame> sWrap("PListEditPropPage_layers","BaseInfo");

//---------------------------------------------------------------------------
__fastcall TLayerEditFrame::TLayerEditFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
    Caption = "Параметры";
}

int TLayerEditFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
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

bool TLayerEditFrame::OnInit()
{
	if(!m_pLayers.init(m_pCore,"layers"))return false;

    g_pSys->GetClassPluginNames("PUserLayer",ModuleCombo->Items);

    return true;
}

void TLayerEditFrame::UpdateData()
{
	if(!m_pLayers->SelectID(m_Ref)) return;

    DCMapLayerInfo const* pinf = m_pLayers->GetData();

    NameEdit->OnChange = 0;
    NameEdit->Text = pinf->name;
    NameEdit->OnChange = NameEditChange;

    ModuleCombo->ItemIndex = ModuleCombo->Items->IndexOf(pinf->module);

	if(ModuleCombo->ItemIndex < 0)ModuleCombo->ItemIndex = 0;

	HiddenCheck->Checked = m_pLayers[DCMDB_LAYER_HIDDEN];

	Changed=false;
}

void TLayerEditFrame::OnApply()
{
    if(!Changed)return;

    if(!m_pLayers->SelectID(m_Ref)) return;

    //DCMaplayerInfo inf;
    //inf = *m_pLayers->GetData();

    m_pLayers->Update();
	m_pLayers->SetWideName(NameEdit->Text.w_str());

	m_pLayers[DCMDB_LAYER_MODULE] = ModuleCombo->Text;
	m_pLayers[DCMDB_LAYER_HIDDEN] = HiddenCheck->Checked;

    m_pLayers->Post();

	Changed=false;
}

bool TLayerEditFrame::OnDataTest()
{
    if(!Changed)return true;
    if(m_pLayers->SelectWideName(NameEdit->Text.w_str()))
    {
        if(*m_pLayers->GetID() != m_Ref)
        {
            Dialogs::TMsgDlgButtons Buttons;Buttons << mbOK;
            sMessageDlg("Ошибка","Существует другой слой с таким именем!",mtError,Buttons,0);
            NameEdit->SetFocus();
            return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------
void __fastcall TLayerEditFrame::NameEditChange(TObject *Sender)
{
    SetNewKeyValue(NameEdit->Text);
    Changed=true;

}
//---------------------------------------------------------------------------
void __fastcall TLayerEditFrame::OnChange(TObject *Sender)
{
	Changed = true;
}
//---------------------------------------------------------------------------
