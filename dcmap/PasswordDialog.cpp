//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "PasswordDialog.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sButton"
#pragma link "sEdit"
#pragma link "sLabel"
#pragma link "sSkinProvider"
#pragma resource "*.dfm"
TPasswordDialogForm *PasswordDialogForm;
//---------------------------------------------------------------------------
__fastcall TPasswordDialogForm::TPasswordDialogForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool TPasswordDialogForm::Execute(String Caption,String Message,String& Value)
{
	std::auto_ptr<TPasswordDialogForm> form(new TPasswordDialogForm(Application->MainForm));
	InterfaceModule->LocalizeContainer(form.get());
	form->Caption = Caption;
	form->Label->Caption = Message;
	if(form->ShowModal() == mrOk)
	{
		Value = form->PasswordEdit->Text;
		return true;
	}
	return false;
}
