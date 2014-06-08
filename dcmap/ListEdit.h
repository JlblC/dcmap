//---------------------------------------------------------------------------

#ifndef ListEditH
#define ListEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "sAdapter.hpp"
#include "sSkinProvider.hpp"
#include "sAlphaListBox.hpp"
#include "sComboBox.hpp"
#include "sEdit.hpp"
#include "sGroupBox.hpp"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include "sSpeedButton.hpp"
#include "sTabControl.hpp"
#include "sButton.hpp"
#include "PropPage.h"
#include "sCheckBox.hpp"
#include <Buttons.hpp>
#include "sBitBtn.hpp"
#include <Menus.hpp>
#include "BaseDataEdit.h"
//#include "TntMenus.hpp"
#include "sListView.hpp"
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class TListEditForm : public TBaseEditForm
{
	typedef TBaseEditForm Inherited;
__published:	// IDE-managed Components
    TsEdit *MaskEdit;
    TsButton *AddButton;
    TsButton *DeleteButton;
    TsButton *ApplyButton;
	TsButton *CloseButton;
    TsButton *CanselButton;
    TsButton *OkButton;
	TsCheckBox *SortCheck;
	TsSpeedButton *UpBtn;
	TsSpeedButton *DownBtn;
	TsListView *DataList;
	TTimer *DelayTimer;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall MaskEditChange(TObject *Sender);
    void __fastcall _PlayerListBoxClick(TObject *Sender);
    void __fastcall MaskEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall _PlayerListBoxKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ApplyButtonClick(TObject *Sender);
    void __fastcall DeleteButtonClick(TObject *Sender);
    void __fastcall AddButtonClick(TObject *Sender);
    void __fastcall KeyValueChange(String Val,TObject *Sender);
    void __fastcall CanselButtonClick(TObject *Sender);
	void __fastcall SortCheckClick(TObject *Sender);
	void __fastcall UpBtnClick(TObject *Sender);
	void __fastcall DownBtnClick(TObject *Sender);
	void __fastcall _PlayerListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall DataListSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
	void __fastcall DelayTimerTimer(TObject *Sender);
private:	// User declarations
    String SelPlayer;
    bool m_bInsertMode;
    bool m_bKeyChanged;
    bool m_bError;

	void __fastcall UpdateList(TObject *Sender=0);
    void ClearList();
    void SaveConfig();
    void LoadConfig();

	bool m_bUpdatesHolded;
	int  m_iIconColumn;

    void DoApply();
	void DoUpdateData();
	void __fastcall ShowListSelection();

    void AddItem(String MaskText,dcmapDBREF const& ref);
    bool HaveIcons;

public:
	IDCMapSimpleDataFilter* Filter;
public:		// User declarations
    __fastcall TListEditForm(TComponent* Owner);
    __fastcall ~TListEditForm();
    void Select(DataReference const& Ref);
    bool Init(IDCMapDataSource* pData,DataReference const* Parent=0);
};
//---------------------------------------------------------------------------
extern PACKAGE TListEditForm *ListEditForm;
//---------------------------------------------------------------------------
#endif
