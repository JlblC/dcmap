//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MapColorPref.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sButton"
#pragma link "sColorSelect"
#pragma link "sSpeedButton"
#pragma link "sCheckBox"
#pragma link "BaseModule"
#pragma resource "*.dfm"
TMapColorPrefFrame *MapColorPrefFrame;
//---------------------------------------------------------------------------
static CFramePanelWrapper<TMapColorPrefFrame> sWrap("PPrefs_colors","MapColorPrefs");

__fastcall TMapColorPrefFrame::TMapColorPrefFrame(TComponent* Owner)
	: TBasePrefsFrame(Owner)
{
	AddConfigTarget("MapColor",1);
}



