//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FilterSelect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseViewPopup"
#pragma link "sSkinProvider"
#pragma link "sListView"
#pragma link "sBitBtn"
#pragma resource "*.dfm"
TFilterSelectForm *FilterSelectForm;
//---------------------------------------------------------------------------
__fastcall TFilterSelectForm::TFilterSelectForm(TComponent* Owner)
	: TBaseViewPopupForm(Owner)
{
}

bool TFilterSelectForm::Execute(DCMapConfigWrapper cfg)
{
	cfg.section("FixedFilter");

    int NumPresets=0;
	cfg["NumPresets"] >> NumPresets;

	ItemsView->Items->BeginUpdate();

    for(int i=0;i<NumPresets;i++)
    {
		String PresetName;
		String PresetSection;
		cfg["PresetName"+IntToStr(i)] >> PresetName;
		cfg["PresetSection"+IntToStr(i)] >> PresetSection;

		TListItem* Item = ItemsView->Items->Add();

		Item->Caption = PresetName;
		Item->Data = (void*)i;
		Item->SubItems->Add(PresetSection);
	}
	ItemsView->Items->EndUpdate();

	if(NumPresets)ItemsView->ItemIndex = 0;
	Changed = false;

	if(ShowModal() != mrOk)return false;
	cfg.section("FixedFilter");

	if(Changed)
	{
		int n = ItemsView->Items->Count;
		for(int i=0;i<n;i++)
		{
			cfg["PresetName"+IntToStr(i)] << ItemsView->Items->Item[i]->Caption;
			cfg["PresetSection"+IntToStr(i)] << ItemsView->Items->Item[i]->SubItems->Strings[0];
		}
		cfg["NumPresets"] << n;
	}

	int Index = ItemsView->ItemIndex;
	if(Index < 0)return false;

	PresetName = ItemsView->Items->Item[Index]->Caption;
	PresetSection = ItemsView->Items->Item[Index]->SubItems->Strings[0];

    return true;
}
//---------------------------------------------------------------------------
void __fastcall TFilterSelectForm::ItemsViewDblClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TFilterSelectForm::DeleteButtonClick(TObject *Sender)
{
	int ItemIndex = ItemsView->ItemIndex;
	if(ItemIndex < 0)return;
	int Index = (int)ItemsView->Items->Item[ItemIndex]->Data;

	DCMapConfigWrapper cfg(GetDCMapCore());

	AnsiString PresetSection = ItemsView->Items->Item[ItemIndex]->SubItems->Strings[0];

	if(ItemsView->ItemIndex>=0)
	{
		ItemsView->Items->Delete(ItemsView->ItemIndex);
		cfg->RemoveSection(PresetSection.c_str());
		Changed = true;
	}
}
//---------------------------------------------------------------------------

