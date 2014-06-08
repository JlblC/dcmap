//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DipColorPref.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sDialogs"
#pragma link "sButton"
#pragma link "sColorSelect"
#pragma link "sSpeedButton"
#pragma link "sGroupBox"
#pragma resource "*.dfm"
TDipColorPrefFrame *DipColorPrefFrame;

static CFramePanelWrapper<TDipColorPrefFrame> sWrap("PPrefs_colors","DipColorPrefs");

//---------------------------------------------------------------------------
__fastcall TDipColorPrefFrame::TDipColorPrefFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
   
    for(int i=0;i<EDCMapDiplomatyRelationNum;i++)
    {
        Colors[i] =RGB(90,60,160);
    }
    Colors[DCMAP_DIP_UNDEFINED] = RGB(90,60,160);
    Colors[DCMAP_DIP_ALLY] = RGB(50,250,50);
    Colors[DCMAP_DIP_ENEMIES] = RGB(250,50,50);
    Colors[DCMAP_DIP_VASSAL] = RGB(100,150,250);
    Colors[DCMAP_DIP_LORD] = RGB(250,250,50);
    Colors[DCMAP_DIP_MULT] = RGB(250,50,250);
    Colors[DCMAP_DIP_ALLY] = RGB(50,250,50);
    Colors[DCMAP_DIP_EMPTY] = RGB(128,128,128);
}
//---------------------------------------------------------------------------

void __fastcall TDipColorPrefFrame::DipColorChange(TObject *Sender)
{
	TsColorSelect* pSel = (TsColorSelect*)Sender;
    Colors[pSel->Tag] = pSel->ColorValue;
}
//---------------------------------------------------------------------------
void TDipColorPrefFrame::LoadConfig(DCMapConfigWrapper cfg)
{
	cfg.section("DipColor");
	for(int i=0;i<EDCMapDiplomatyRelationNum;i++)
    {
        cfg["Color"+IntToStr(i)] >> Colors[i];
    }
}

void TDipColorPrefFrame::SaveConfig(DCMapConfigWrapper cfg)
{
	cfg.section("DipColor");
	for(int i=0;i<EDCMapDiplomatyRelationNum;i++)
    {
        cfg["Color"+IntToStr(i)] << Colors[i];
    }
}

void TDipColorPrefFrame::OnShow()
{
	sColor_UNDEFINED->ColorValue = Colors[DCMAP_DIP_UNDEFINED];
	sColor_EMPTY->ColorValue = Colors[DCMAP_DIP_EMPTY];

	/*sColor_NEUTRALS->ColorValue = Colors[DCMAP_DIP_NEUTRALS];
	sColor_ALLY->ColorValue = Colors[DCMAP_DIP_ALLY];
	sColor_ENEMIES->ColorValue = Colors[DCMAP_DIP_ENEMIES];
	sColor_LORD->ColorValue = Colors[DCMAP_DIP_LORD];
	sColor_VASSAL->ColorValue = Colors[DCMAP_DIP_VASSAL];
	sColor_MULT->ColorValue = Colors[DCMAP_DIP_MULT];
	sColor_POTENTIAL_ALLY->ColorValue = Colors[DCMAP_DIP_POTENTIAL_ALLY];
	sColor_POTENTIAL_ENEMIES->ColorValue = Colors[DCMAP_DIP_POTENTIAL_ENEMIES];
	sColor_POTENTIAL_LORD->ColorValue = Colors[DCMAP_DIP_POTENTIAL_LORD];
	sColor_POTENTIAL_VASSAL->ColorValue = Colors[DCMAP_DIP_POTENTIAL_VASSAL]; */
}

void TDipColorPrefFrame::OnApply()
{
   	SaveConfig(DCMapConfigWrapper(m_pCore->GetConfig()));
}
void __fastcall TDipColorPrefFrame::OthersButtonClick(TObject *Sender)
{
	m_pCore->GetCoreUI()->OpenListTableEdit("dip");
}
//---------------------------------------------------------------------------

