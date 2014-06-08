//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main2.h"
#include "System.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma resource "*.dfm"
TMainForm2 *MainForm2;
//---------------------------------------------------------------------------
__fastcall TMainForm2::TMainForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm2::FormCreate(TObject *Sender)
{
//	SystemModule->InitSystem(&g_CoreUIManager);
}
//---------------------------------------------------------------------------
