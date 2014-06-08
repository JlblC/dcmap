//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "dcmapsyslib.h"

#include "UIDialogForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma resource "*.dfm"
TDialogForm *DialogForm;
//---------------------------------------------------------------------------
__fastcall TDialogForm::TDialogForm(TComponent* Owner)
	: TForm(Owner)
{
	Frame=0;
	Panel=0;
}
//---------------------------------------------------------------------------
bool TDialogForm::Setup(IDCMapModule* pModule,dcmapWCSTR sCaption, int Flags)
{
	Panel = pModule->Cast<IDCMapVCLPanel>();
	if(!Panel)return false;
	Frame = Panel->CreateFrame();
	if(!Frame)return false;

	ClientWidth = Frame->Width;
	ClientHeight = Frame->Height;

	TComponent* pComp = Frame->FindComponent("sFrameAdapter");
	if(pComp)
	{
		TsFrameAdapter* sFrameAdapter = dynamic_cast<TsFrameAdapter*>(pComp);
		sFrameAdapter->SkinData->SkinSection = "CHECKBOX";
	}

	pComp = Frame->FindComponent("MainMenu");
	if(pComp)
	{
		TMainMenu* pMainMenu = dynamic_cast<TMainMenu*>(pComp);
        Menu = pMainMenu;
	}

	Frame->Parent = this;
	Caption=L"";

	if(sCaption && sCaption[0])
		Caption = sCaption;

	return true;
}

void __fastcall TDialogForm::FormShow(TObject *Sender)
{
	if(!Frame || !Panel)return;

	Panel->OnShow();
	if(!Caption.Length())
		Caption = Panel->GetCaption();

	ClientWidth = Frame->Width;
	ClientHeight = Frame->Height;
	Frame->Align = alClient;
}
//---------------------------------------------------------------------------

void __fastcall TDialogForm::TntFormDestroy(TObject *Sender)
{
	 if(Frame)Frame->Parent = 0;
}
//---------------------------------------------------------------------------

