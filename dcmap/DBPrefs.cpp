//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBPrefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sCheckBox"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma link "sGroupBox"
#pragma resource "*.dfm"
TDBFrefsFrame *DBFrefsFrame;

static CFramePanelWrapper<TDBFrefsFrame> sWrap("PPrefs_base","BDPrefs");

//---------------------------------------------------------------------------
__fastcall TDBFrefsFrame::TDBFrefsFrame(TComponent* Owner)
	: TBasePrefsFrame(Owner)
{
	AddConfigTarget("DataBase",1);
}
//---------------------------------------------------------------------------
void TDBFrefsFrame::OnLoadPrefs(DCMapConfigWrapper cfg)
{
	 UseBackupsClick(this);
}

void __fastcall TDBFrefsFrame::UseBackupsClick(TObject *Sender)
{
	NumBackups->Enabled = UseBackups->Checked;
	UseHardlinks->Enabled = UseBackups->Checked;
	OnChange(Sender);
}
//---------------------------------------------------------------------------


