//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CorruptionColorPref.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sColorSelect"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TCorruptionPrefFrame *CorruptionPrefFrame;

static CFramePanelWrapper<TCorruptionPrefFrame> sWrap("PPrefs_colors","CorruptionColorPrefs");

//---------------------------------------------------------------------------
__fastcall TCorruptionPrefFrame::TCorruptionPrefFrame(TComponent* Owner)
	: TBasePrefsFrame(Owner)
{
	AddConfigTarget("CorruptionColor",1);
}
//---------------------------------------------------------------------------
