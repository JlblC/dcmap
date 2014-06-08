//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScriptEditor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ScriptEditCntr"
#pragma resource "*.dfm"
TScriptEditForm *ScriptEditForm;
//---------------------------------------------------------------------------
__fastcall TScriptEditForm::TScriptEditForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
