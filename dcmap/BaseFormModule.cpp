//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BaseFormModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma resource "*.dfm"
TBaseModuleForm *BaseModuleForm;
//---------------------------------------------------------------------------
__fastcall TBaseModuleForm::TBaseModuleForm(TComponent* Owner)
	: TForm(Owner),m_pSystem(0),m_pCore(0),m_pWorkspace(0)
{

}
//---------------------------------------------------------------------------

void __fastcall TBaseModuleForm::CreateParams(TCreateParams &Params)
{
	 Inherited::CreateParams(Params);
	 TForm* ParentForm = dynamic_cast<TForm*>(Owner);
	 if(ParentForm)
	 {
		Params.WndParent = ParentForm->Handle;
	 }
}

bool TBaseModuleForm::OnInit()
{
	return true;
}

dcmapLRESULT TBaseModuleForm::OnMessage(dcmapMSG MSG,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender)
{
   return 0;
}

int TBaseModuleForm::OnScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
{
    return DCMEXEC_FAIL;
}
