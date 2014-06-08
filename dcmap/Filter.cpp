//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Filter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "FilterGroup"
#pragma link "sPanel"
#pragma link "sScrollBox"
#pragma link "sButton"
#pragma resource "*.dfm"
TFilterFrame *FilterFrame;

//---------------------------------------------------------------------------
__fastcall TFilterFrame::TFilterFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
    CurrentGroup=0;
}
bool TFilterFrame::OnInit(IDCMapModuleManager* pCreator)
{
    m_pCreator = pCreator;

	FilterGroupFrame1->Filter = this;
    FilterGroupFrame1->Init();

    TStringList* plugins = new TStringList;
    g_pSys->GetClassPluginNames("FilterItem",plugins);

    for(int i=0;i<plugins->Count;i++)
    {
    	TMenuItem* Item = new TMenuItem(this);
    	Item->Caption = plugins->Strings[i];
        Item->Name = plugins->Strings[i];
        Item->OnClick = AddFilterClick;
    	AddMenu->Items->Add(Item);
    }

    delete plugins;
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TFilterFrame::N1Click(TObject *Sender)
{
	if(!CurrentGroup)return;
    CurrentGroup->AddSubGroup();
}
//---------------------------------------------------------------------------
void __fastcall TFilterFrame::AddFilterClick(TObject *Sender)
{
	if(!CurrentGroup)return;
    TMenuItem* Item = (TMenuItem*)Sender;

    //CurrentGroup->AddNewItem(Item->Name);
    IDCMapFilterItem* filter = m_pCreator->CreateModuleTyped<IDCMapFilterItem>("FilterItem",Item->Name.c_str());
    if(!filter)return;

    IDCMapVCLPanel* panel = m_pCreator->CreateModuleTyped<IDCMapVCLPanel>("FilterUIItem", filter->UIModuleName());
    if(!panel)return;

    panel->OnMessage(DCMM_FILTERUI_SET_FILTER,0,0,filter);

    CurrentGroup->AddNewItem(filter,panel);
}
//---------------------------------------------------------------------------

int TFilterFrame::Process(const DCMapDataReference & ref)
{
     return FilterGroupFrame1->Process(ref);
}

