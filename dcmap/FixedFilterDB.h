//$$---- Frame HDR ----
//---------------------------------------------------------------------------


#ifndef FixedFilterDBH
#define FixedFilterDBH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include "BaseModule.h"
#include <Forms.hpp>
#include "sComboBox.hpp"
#include "sCheckBox.hpp"
#include "sFrameAdapter.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include <Menus.hpp>
#include "sEdit.hpp"
#include "sSpinEdit.hpp"
//#include "TntMenus.hpp"
#include "sComboEdit.hpp"
#include "sCustomComboEdit.hpp"
#include "sMaskEdit.hpp"
#include <Mask.hpp>
//---------------------------------------------------------------------------

class TFixedFilterDBFrame : public TFrame
{
	friend class TFixedFilterFrame;
__published:	// IDE-managed Components
	TsComboBox *DBValueComboBox;
	TsComboBox *OperatorComboBox;
	TsCheckBox *UseCheck;
	TsFrameAdapter *sFrameAdapter1;
	TsSpeedButton *ResourceSelBtn;
	TsDecimalSpinEdit *NumEdit;
	TsEdit *StringEdit;
	TsComboEdit *ComboEdit;
	TsSpeedButton *ModeButton;
	void __fastcall FieldMenuClick(TObject *Sender);
	void __fastcall DataChange(TObject *Sender);
	void __fastcall ResourceSelBtnMouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y,
          int HitTest, TMouseActivate &MouseActivate);
	void __fastcall UseCheckClick(TObject *Sender);
	void __fastcall DBValueComboBoxClick(TObject *Sender);
	void __fastcall DBValueComboBoxDropDown(TObject *Sender);
	void __fastcall DBValueComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall ComboEditButtonClick(TObject *Sender);
	void __fastcall ModeButtonClick(TObject *Sender);
private:	// User declarations

	IDCMapDataStoragePtr m_ListDataSrc;
    int 	m_iColID;
    bool    m_bStringData;
    bool    m_bCustom;
	int    m_iMode;

    AnsiString Table;
	AnsiString Field;

	UnicodeString LastItem;

    int m_iRefOp;
    int m_iStrOp;
    int m_iNumOp;

    int m_eResource;
    int m_eTable;

    void UpdateData();
    void SetupData();

    void SetupFilter();

    IDCMapCore*  m_pCore;
    IDCMapWorkspace* m_pWorkspace;

    int m_iFilterNo;

    TFixedFilterFrame* m_pParent;

    DCMapDataReference m_Ref;

    void LoadConfig(DCMapConfigWrapper cfg);
	void SaveConfig(DCMapConfigWrapper cfg);

	void RecordState();
public:		// User declarations
	__fastcall ~TFixedFilterDBFrame();
	__fastcall TFixedFilterDBFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFixedFilterDBFrame *FixedFilterDBFrame;
//---------------------------------------------------------------------------
#endif
