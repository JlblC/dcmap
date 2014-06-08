//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SystemPrefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BasePrefs"
#pragma link "sFrameAdapter"
#pragma link "sCheckBox"
#pragma resource "*.dfm"
TSystemPrefsFrame *SystemPrefsFrame;

static CFramePanelWrapper<TSystemPrefsFrame> sWrap("PPrefs_base","01_SystemPrefs");

//---------------------------------------------------------------------------
__fastcall TSystemPrefsFrame::TSystemPrefsFrame(TComponent* Owner)
	: TBasePrefsFrame(Owner)
{
	AddConfigTarget("Core",1);
	AddConfigTarget("System",2);


}
//---------------------------------------------------------------------------
