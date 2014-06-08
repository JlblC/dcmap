//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GameImportSelect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseViewPopup"
#pragma link "sBitBtn"
#pragma link "sListView"
#pragma link "sSkinProvider"
#pragma link "sCheckBox"
#pragma resource "*.dfm"
TGameImportSelectForm *GameImportSelectForm;

 static CAutoReg < CBasicFormService<TGameImportSelectForm> > sWrap("PCoreService","GameImportSelectForm");

//---------------------------------------------------------------------------
__fastcall TGameImportSelectForm::TGameImportSelectForm(TComponent* Owner)
	: TBaseViewPopupForm(Owner)
{
}
//---------------------------------------------------------------------------


void TGameImportSelectForm::FillList()
{
	PlayersList->Items->BeginUpdate();

	PlayersList->Items->Clear();

	if(Players->SelectAll())
	do
	{
		if(Players[DCMDB_PLAYER_LOGIN].Str() == L"" ||
		   Players[DCMDB_PLAYER_PASSWORD].Str() == L"") continue;

		if(!ListAllCheckBox->Checked && !Players[DCMDB_PLAYER_WEBACCESS])continue;

		TListItem* Item = PlayersList->Items->Add();

		Item->Caption = Players[DCMDB_PLAYER_NAME];
		Item->SubItems->Add(Players[DCMDB_PLAYER_TURN]);

		Item->Data = new dcmapDBREF(*Players->GetID());
	}
	while(Players->Next());

	PlayersList->Items->EndUpdate();
}

bool TGameImportSelectForm::OnInit()
{
	if(!Players.init(m_pCore))return false;
	FillList();

	return Inherited::OnInit();
}

void __fastcall TGameImportSelectForm::PlayersListDeletion(TObject *Sender, TListItem *Item)
{
	delete (dcmapDBREF*)Item->Data;
}
//---------------------------------------------------------------------------

void __fastcall TGameImportSelectForm::ListAllCheckBoxClick(TObject *Sender)
{
	FillList();
}
//---------------------------------------------------------------------------

void __fastcall TGameImportSelectForm::PlayersListDblClick(TObject *Sender)
{
	if(!PlayersList->Selected)return;

	m_pCore->GetCoreUI()->OpenListTableEdit("players",(dcmapDBREF*)PlayersList->Selected->Data);
}
//---------------------------------------------------------------------------



void __fastcall TGameImportSelectForm::CloseButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TGameImportSelectForm::ProcessButtonClick(TObject *Sender)
{
	IDCMapVarValue* pVal = dcmapCreateVarValue();

	int n = PlayersList->Items->Count;
	for(int i=0;i<n;i++)
	{
		TListItem* Item = PlayersList->Items->Item[i];
		if(Item->Checked)
		{
			pVal->AddDBRef((dcmapDBREF*)Item->Data,Players);
		}
	}
	m_pSystem->GetScriptSystem()->CallGlobalProc(L"DCXMLImportPlayers",pVal);
	pVal->Destroy();

	FillList();
}
//---------------------------------------------------------------------------

void __fastcall TGameImportSelectForm::SelectAllButtonClick(TObject *Sender)
{
	PlayersList->Items->BeginUpdate();
	int n = PlayersList->Items->Count;
	for(int i=0;i<n;i++)
	{
		PlayersList->Items->Item[i]->Checked = true;
	}
	PlayersList->Items->EndUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TGameImportSelectForm::SelectNoneButtonClick(TObject *Sender)
{
	PlayersList->Items->BeginUpdate();
	int n = PlayersList->Items->Count;
	for(int i=0;i<n;i++)
	{
		PlayersList->Items->Item[i]->Checked = false;
	}
	PlayersList->Items->EndUpdate();
}
//---------------------------------------------------------------------------





void __fastcall TGameImportSelectForm::PlayersListClick(TObject *Sender)
{
	if(!PlayersList->Selected)return;

	PlayersList->Selected->Checked = !PlayersList->Selected->Checked;

}
//---------------------------------------------------------------------------

