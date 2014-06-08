//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ColorSchemeToolbar.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sComboBox"
#pragma resource "*.dfm"
TColorSchemeToolbarFrame *ColorSchemeToolbarFrame;

static CFramePanelWrapper<TColorSchemeToolbarFrame> sWrap("PToolbarItem","ColoringSelectToolItem");

//---------------------------------------------------------------------------
__fastcall TColorSchemeToolbarFrame::TColorSchemeToolbarFrame(TComponent* Owner)
    : TBaseModuleFrame(Owner)
{
    
}
 __fastcall TColorSchemeToolbarFrame::~TColorSchemeToolbarFrame()
{
   DeleteStringsObjects(ColorSchemeCombo->Items);
}

bool TColorSchemeToolbarFrame::OnInit()
{
   m_pColoring = m_pCore->GetDataSourceTyped<IDCMapColoringDataStorage>("coloring");
   if(!m_pColoring)return false;
   return true;
}

void TColorSchemeToolbarFrame::OnShow()
{
   DeleteStringsObjects(ColorSchemeCombo->Items);
   ColorSchemeCombo->Clear();

   if(m_pColoring->SelectAll())
   {
	   do
	   {
		   ColorSchemeCombo->Items->AddObject(m_pColoring->GetWideName(),
				new TDataRefObject(m_pColoring->GetID()));
	   }
	   while(m_pColoring->Next());
   }
   ColorSchemeCombo->ItemIndex = ColorSchemeCombo->Items->IndexOf(LastColoringStr);
   ColorSchemeCombo->Items->Add(LSTRW(SetupDBItem));
}

int TColorSchemeToolbarFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
    case DCMM_DATA_CHANGED:
        OnShow();
        break;
    }
    return 0;
}

#define MSG_COLORING_CHANGED DCMM_USER+0

//---------------------------------------------------------------------------
void __fastcall TColorSchemeToolbarFrame::ColorSchemeComboChange(TObject *Sender)
{
   if(ColorSchemeCombo->ItemIndex == ColorSchemeCombo->Items->Count-1)
   {
		ColorSchemeCombo->ItemIndex = ColorSchemeCombo->Items->IndexOf(LastColoringStr);
		m_pCore->GetCoreUI()->OpenListTableEdit("coloring");
		return;
   }
   TDataRefObject* pObj = (TDataRefObject*)ColorSchemeCombo->Items->Objects[ColorSchemeCombo->ItemIndex];

   if(!m_pColoring->SelectID(pObj->ref))return;

    if(m_pWorkspace->SelectModuleByID("CColoringFilter"))
    {
        IDCMapPlanetDataFilter* pFilter =  m_pWorkspace->Module()->Cast<IDCMapPlanetDataFilter>();
		pFilter->OnMessage(MSG_COLORING_CHANGED,pObj->ref.id,pObj->ref.age,0);
    }
}

void TColorSchemeToolbarFrame::LoadConfig(DCMapConfigWrapper cfg)
{
    cfg.section("PlanetColoring");
    cfg["Coloring"] >> LastColoringStr;
}

//---------------------------------------------------------------------------


