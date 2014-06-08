//$$---- Form CPP ----
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "WebProc.h"
#include "dcmapsyslib.h"

#include "About.h"
#include "BaseModule.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma link "sLabel"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TAboutForm *AboutForm=0;

static CAutoReg<CBasicFormService<TAboutForm> > sWrap("PCoreService","AboutForm");

//---------------------------------------------------------------------------
__fastcall TAboutForm::TAboutForm(TComponent* Owner)
	: Inherited(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::FormCreate(TObject *Sender)
{
	Inherited::FormCreate(Sender);

	StrID->Caption = DCMAP_VERSION_FAMILY;
	Version->Caption = DCMAP_VERSION_NAME;
	ApiVer->Caption = IntToHex((__int64)DCMAP_API_VERSION,8)+":"+IntToStr(DCMAP_API_MINOR);

	if(DCMAP_API_MINOR)
	{
		ApiVer->Caption+=":"+IntToStr(DCMAP_API_MINOR);
	}

	String ver = DCMAP_VERSION;
	int rev = StrToInt(ver.SubString(ver.Length()-2,3));
	if(rev)
	{
		RevisionLabel->Caption = "revision "+IntToStr(rev);
	}
	else RevisionLabel->Caption = "";

	//CopyR1Label->Caption = CopyR1Label->Caption+MainForm->sSkinManager->Version;
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::FormClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------

void __fastcall TAboutForm::CheckUpdateBtnClick(TObject *Sender)
{
	if(!WebModule)WebModule = new TWebModule(Application);
	WebModule->CheckUpdates();
}
//---------------------------------------------------------------------------


