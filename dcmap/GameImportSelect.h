//---------------------------------------------------------------------------

#ifndef GameImportSelectH
#define GameImportSelectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseViewPopup.h"
#include "sBitBtn.hpp"
#include "sListView.hpp"
#include "sSkinProvider.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include "sCheckBox.hpp"
//---------------------------------------------------------------------------
class TGameImportSelectForm : public TBaseViewPopupForm
{
	typedef  TBaseViewPopupForm Inherited;
__published:	// IDE-managed Components
	TsListView *PlayersList;
	TsBitBtn *SelectAllButton;
	TsBitBtn *SelectNoneButton;
	TsBitBtn *ProcessButton;
	TsBitBtn *CloseButton;
	TsCheckBox *ListAllCheckBox;
	void __fastcall PlayersListDeletion(TObject *Sender, TListItem *Item);
	void __fastcall ListAllCheckBoxClick(TObject *Sender);
	void __fastcall PlayersListDblClick(TObject *Sender);
	void __fastcall CloseButtonClick(TObject *Sender);
	void __fastcall ProcessButtonClick(TObject *Sender);
	void __fastcall SelectAllButtonClick(TObject *Sender);
	void __fastcall SelectNoneButtonClick(TObject *Sender);
	void __fastcall PlayersListClick(TObject *Sender);



private:	// User declarations
	IDCMapPlayersDataStoragePtr Players;

	void ClearList();
	void FillList();
public:		// User declarations
	bool OnInit();
	__fastcall TGameImportSelectForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGameImportSelectForm *GameImportSelectForm;
//---------------------------------------------------------------------------
#endif
