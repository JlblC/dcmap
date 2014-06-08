//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FakeMain.h"
#include "TabsFormModule.h"
#include "Splash.h"
#include "System.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFakeMainForm *FakeMainForm;
//---------------------------------------------------------------------------
__fastcall TFakeMainForm::TFakeMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFakeMainForm::FormCreate(TObject *Sender)
{
	SystemModule->InitSystem(&g_CoreUIManager);
	m_pCore = SystemModule->m_pCore;
	if(SplashForm)
	{
		SplashForm->Close();
		delete SplashForm;
		SplashForm=0;
	}
}
//---------------------------------------------------------------------------
void TFakeMainForm::OnFrameClose(TBaseFrameForm* Form)
{
	FrameForms.erase(Form);
	if(FrameForms.empty()) Close();
}
void __fastcall TFakeMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	SystemModule->TerminateSystem();
}
//---------------------------------------------------------------------------

