//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DataEdit.h"
#include "main.h"
#include "BaseModule.h"
#include "Interface.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma link "sButton"
#pragma link "sPageControl"
#pragma link "sTabControl"
#pragma link "sMemo"
#pragma link "PropPage"
#pragma link "sBitBtn"
#pragma link "sSpeedButton"
#pragma link "BaseDataEdit"
#pragma link "PropPage"
#pragma link "sBitBtn"
#pragma link "sButton"
#pragma link "sSkinProvider"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TDataEditForm *DataEditForm;
//---------------------------------------------------------------------------
__fastcall TDataEditForm::TDataEditForm(TComponent* Owner)
    : TBaseEditForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TDataEditForm::FormShow(TObject *Sender)
{
	PropPageFrame->OnShow();
	PropPageFrame->SetControlFocus();

	/*if(m_pData->SelectID(m_SelRef))
	{
		Caption = (UnicodeString)m_pData->RecordCaption()+L" "+m_pData->GetDisplayName();
		PropPageFrame->SelectRef(m_SelRef);
		//PropPageFrame->SendMessage(DCMM_SELECTION_DATA_CHANGED,m_SelRef.id,m_SelRef.age);
		PropPageFrame->SetControlFocus();

		Width += PropPageFrame->WidthUpdate;
		Height += PropPageFrame->HeightUpdate;
	}*/
}


void __fastcall TDataEditForm::KeyValueChange(String Val,TObject *Sender)
{
	Caption = (UnicodeString)m_pData->RecordCaption()+L" "+Val;
}

//---------------------------------------------------------------------------
void TDataEditForm::DoApply()
{
	PropPageFrame->DoApply();
}

void __fastcall TDataEditForm::FormClose(TObject *Sender,TCloseAction &Action)
{
    if(ModalResult == mrOk)
    {
		PropPageFrame->DoOk();
	}
    else if(ModalResult == mrCancel)
    {
        PropPageFrame->DoCansel();
    }
    SaveConfig();
}
//---------------------------------------------------------------------------

bool TDataEditForm::Init(IDCMapDataSource* pData,DataReference const* Parent)
{
	if(!TBaseEditForm::Init(pData,Parent))return false;


	return true;
}

void TDataEditForm::Select(DataReference const& Ref)
{
	m_SelRef = Ref;
	if(m_pData->SelectID(m_SelRef))
	{
		Caption = (UnicodeString)m_pData->RecordCaption()+L" "+m_pData->GetDisplayName();
		PropPageFrame->SelectRef(m_SelRef);

		Width += PropPageFrame->WidthUpdate;
		Height += PropPageFrame->HeightUpdate;
	}
	SelChanged();
}


void TDataEditForm::DoUpdateData()
{
	 ///PropPageFrame->SendMessage(DCMM_SELECTION_DATA_CHANGED,m_SelRef.id,m_SelRef.age);
	 PropPageFrame->SelectRef(m_SelRef);
}

void __fastcall TDataEditForm::ApplyButtonClick(TObject *Sender)
{
	PropPageFrame->DoApply();
	PropPageFrame->SelectRef(m_SelRef);
	Width += PropPageFrame->WidthUpdate;
	Height += PropPageFrame->HeightUpdate;

	SelChanged();
}
//---------------------------------------------------------------------------





