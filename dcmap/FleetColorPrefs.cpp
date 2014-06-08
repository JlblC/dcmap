//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FleetColorPrefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BasePrefs"
#pragma link "sFrameAdapter"
#pragma link "sColorSelect"
#pragma link "sSpeedButton"
#pragma link "sGroupBox"
#pragma resource "*.dfm"
TFleetColorPrefsFrame *FleetColorPrefsFrame;

static CFramePanelWrapper<TFleetColorPrefsFrame> sWrap("PPrefs_colors","FleetColorPrefs");

//---------------------------------------------------------------------------
__fastcall TFleetColorPrefsFrame::TFleetColorPrefsFrame(TComponent* Owner)
	: TBasePrefsFrame(Owner)
{
	AddConfigTarget("FleetColor",1);
}
//---------------------------------------------------------------------------


