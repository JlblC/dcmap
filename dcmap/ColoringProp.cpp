//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ColoringProp.h"
#include "sDialogs.hpp"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sEdit"
#pragma link "sComboBox"
#pragma link "sColorSelect"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TColoringPropframe *ColoringPropframe;

static CFramePanelWrapper<TColoringPropframe> sWrap("PListEditPropPage_coloring","BaseInfo");

//---------------------------------------------------------------------------
__fastcall TColoringPropframe::TColoringPropframe(TComponent* Owner)
    : TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TColoringPropframe::OnShow()
{
   IDCMapSys* pSys = g_pSys;
   // Get aviable coloring plugins

   PluginList_LoadClass("PPlanetColorProvider",PlanetComboBox->Items);
   PluginList_LoadClass("PPlanetColorProvider",BackgroundComboBox->Items);
   PluginList_LoadClass("PPlanetColorProvider",BorderComboBox->Items);
   PluginList_LoadClass("PPlanetColorProvider",PlanetBorderComboBox->Items);
   PluginList_LoadClass("PPlanetColorProvider",MarkerComboBox->Items);

   String NoItem = g_pLocalStrings->GetStringW("NoDBItem");

   PlanetComboBox->Items->Insert(0,NoItem);
   BackgroundComboBox->Items->Insert(0,NoItem);
   BorderComboBox->Items->Insert(0,NoItem);
   PlanetBorderComboBox->Items->Insert(0,NoItem);
   MarkerComboBox->Items->Insert(0,NoItem);
}

bool TColoringPropframe::OnInit()
{
   m_pColoring = m_pCore->GetDataSourceTyped<IDCMapColoringDataStorage>("coloring");
   if(!m_pColoring)return false;

   return true;
}

int TColoringPropframe::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
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

void TColoringPropframe::OnCansel()
{
    Changed = false;
}

void TColoringPropframe::OnApply()
{
    if(!Changed)return;
    DCMapColoringInfo inf;

    if(!m_pColoring->SelectID(m_Ref)) return;
    
	inf = *m_pColoring->GetData();

	strncpy(inf.planet,PluginList_At(PlanetComboBox->Items,PlanetComboBox->ItemIndex).c_str(),DCMapColoringPluginNameLen);
    strncpy(inf.background,PluginList_At(BackgroundComboBox->Items,BackgroundComboBox->ItemIndex).c_str(),DCMapColoringPluginNameLen);
    strncpy(inf.cellborder,PluginList_At(BorderComboBox->Items,BorderComboBox->ItemIndex).c_str(),DCMapColoringPluginNameLen);
    strncpy(inf.planetborder,PluginList_At(PlanetBorderComboBox->Items,PlanetBorderComboBox->ItemIndex).c_str(),DCMapColoringPluginNameLen);
    strncpy(inf.marker,PluginList_At(MarkerComboBox->Items,MarkerComboBox->ItemIndex).c_str(),DCMapColoringPluginNameLen);

    inf.planet_color = PlanetColor->ColorValue ;
    inf.background_color = BackgroundColor->ColorValue;
    inf.cellborder_color = BorderColor->ColorValue ;
    inf.planetborder_color = PlanetBorderColor->ColorValue ;
	inf.marker_color = MarkerColor->ColorValue ;

	m_pColoring->Update();
	m_pColoring->SetData(&inf);
	m_pColoring->SetWideName(NameEdit->Text.w_str());
	m_pColoring->Post();
}

void TColoringPropframe::UpdateData()
{
    if(!m_pColoring->SelectID(m_Ref)) return;

    DCMapColoringInfo const* pinf = m_pColoring->GetData();

    NameEdit->OnChange = 0;
    NameEdit->Text = pinf->name;
    NameEdit->OnChange = NameEditChange;

    PlanetComboBox->ItemIndex = PluginList_IndexOf(PlanetComboBox->Items,pinf->planet,0);
    BackgroundComboBox->ItemIndex = PluginList_IndexOf(BackgroundComboBox->Items,pinf->background,0);
    BorderComboBox->ItemIndex = PluginList_IndexOf(BorderComboBox->Items,pinf->cellborder,0);
    PlanetBorderComboBox->ItemIndex = PluginList_IndexOf(PlanetBorderComboBox->Items,pinf->planetborder,0);
    MarkerComboBox->ItemIndex = PluginList_IndexOf(MarkerComboBox->Items,pinf->marker,0);

    PlanetColor->ColorValue = pinf->planet_color;
    BackgroundColor->ColorValue = pinf->background_color;
    BorderColor->ColorValue = pinf->cellborder_color;
    PlanetBorderColor->ColorValue = pinf->planetborder_color;
    MarkerColor->ColorValue = pinf->marker_color;

    ComboBoxChange(this);
    Changed = false;
}

void __fastcall TColoringPropframe::NameEditChange(TObject *Sender)
{
    SetNewKeyValue(NameEdit->Text);
    Changed=true;
}
//---------------------------------------------------------------------------

bool TColoringPropframe::OnDataTest()
{
    if(!Changed)return true;
	if(m_pColoring->SelectWideName(NameEdit->Text.w_str()))
    {
        if(*m_pColoring->GetID() != m_Ref)
        {
            Dialogs::TMsgDlgButtons Buttons;Buttons << mbOK;
			sMessageDlg(L"Ошибка",L"Существует другая схема расцветки с таким именем!",mtError,Buttons,0);
            NameEdit->SetFocus();
            return false;
        }
    }
    return true;
}
void __fastcall TColoringPropframe::DataChange(TObject *Sender)
{
    Changed=true;
}
//---------------------------------------------------------------------------
void __fastcall TColoringPropframe::ComboBoxChange(TObject *Sender)
{
	PlanetColor->Visible = (PluginList_At(PlanetComboBox->Items,PlanetComboBox->ItemIndex) == "Color");
	BackgroundColor->Visible = (PluginList_At(BackgroundComboBox->Items,BackgroundComboBox->ItemIndex) == "Color");
	BorderColor->Visible = (PluginList_At(BorderComboBox->Items,BorderComboBox->ItemIndex) == "Color");
	PlanetBorderColor->Visible = (PluginList_At(PlanetBorderComboBox->Items,PlanetBorderComboBox->ItemIndex) == "Color");
	MarkerColor->Visible = (PluginList_At(MarkerComboBox->Items,MarkerComboBox->ItemIndex) == "Color");
    Changed=true;
}
//---------------------------------------------------------------------------


