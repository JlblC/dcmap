//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "dcmapsyslib.h"
#include "Splash.h"
#include "Interface.h"
#include "BaseModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma link "acProgressBar"
#pragma link "sLabel"
#pragma link "sPanel"
#pragma link "sBevel"
#pragma resource "*.dfm"
TSplashForm *SplashForm=0;

static CAutoReg<CBasicFormService<TSplashForm> > sWrap("PCoreService","SplashForm");


//---------------------------------------------------------------------------
__fastcall TSplashForm::TSplashForm(TComponent* Owner)
	: Inherited(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSplashForm::FormCreate(TObject *Sender)
{
	if(!InterfaceModule->SkinManager->Active)
        	Panel->BevelKind = bkFlat;

	StrID->Caption = DCMAP_VERSION_FAMILY;

	String ver = DCMAP_VERSION;
	int rev = StrToInt(ver.SubString(ver.Length()-2,3));
	if(rev)
	{
		RevisionLabel->Caption = "revision "+IntToStr(rev);
	}
	else RevisionLabel->Caption = "";
}
//---------------------------------------------------------------------------

void __fastcall TSplashForm::FormShow(TObject *Sender)
{
	// setup region
}
//---------------------------------------------------------------------------


