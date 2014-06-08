//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BaseProp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma resource "*.dfm"
TBaseEditModuleFrame *BaseEditModuleFrame;
//---------------------------------------------------------------------------
__fastcall TBaseEditModuleFrame::TBaseEditModuleFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
	NameChanged = false;
}
//---------------------------------------------------------------------------

int TBaseEditModuleFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
    case DCMM_SELECTION_DATA_CHANGED:
        m_DataRef.id = wParam;
        m_DataRef.age = lParam;
        OnDataUpdate();
        Changed=false;
    	NameChanged=false;
        break;
	}
    return 0;
}

void __fastcall TBaseEditModuleFrame::NameEditChange(TObject *Sender)
{
    TEdit* NameEdit = dynamic_cast<TEdit*>(Sender);
    if(!NameEdit)return;
    SetNewKeyValue(NameEdit->Text);
    Changed=true;
    NameChanged=true;
}

void __fastcall TBaseEditModuleFrame::OnChange(TObject *Sender)
{
    Changed=true;
}

void TBaseEditModuleFrame::OnDataUpdate()
{

}
