//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef GraphicsPropH
#define GraphicsPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sAlphaListBox.hpp"
#include "sColorSelect.hpp"
#include "sGroupBox.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "sLabel.hpp"
#include "sTrackBar.hpp"
#include <ComCtrls.hpp>
#include "sCheckBox.hpp"
#include "sEdit.hpp"
#include "sRadioButton.hpp"
#include "sComboBox.hpp"
#include "sDialogs.hpp"
#include "sMemo.hpp"
#include "sSpinEdit.hpp"
//#include "TntMenus.hpp"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TGraphicsEditFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsListBox *CoordsList;
	TsGroupBox *LineGroupBox;
	TsColorSelect *LineColor;
	TsTrackBar *LineTrns;
	TsEdit *NameEdit;
	TsGroupBox *FillGroupBox;
	TsColorSelect *FillColor;
	TsTrackBar *FillTrns;
	TsRadioButton *ThinLineRadio;
	TsRadioButton *WideLineRadio;
	TsRadioButton *XXLLineRadio;
	TsComboBox *LineStyle;
	TsRadioButton *PolyRadio;
	TsRadioButton *LineRadio;
	TsSpeedButton *RemoveCoordBtn;
	TsSpeedButton *AddCoordBtn;
	TsSpeedButton *CopyBtn;
	TsSpeedButton *PasteBtn;
	TsSpeedButton *InsertBefore;
	TsSpeedButton *InsertAfter;
	TsSpeedButton *UpBtn;
	TsSpeedButton *DownBtn;
	TsCheckBox *VisibleCheck;
	TsCheckBox *FillCheck;
	TsCheckBox *LineCheck;
	TsEdit *TextEdit;
	TsEdit *TextCoordsEdit;
	TsColorSelect *TextColor;
	TsMemo *NotesEdit;
	TsDecimalSpinEdit *TextSizeEdit;
	TsCheckBox *TextCheck;
	TsSpeedButton *CopyLayerBtn;
	TPopupMenu *LayerMenu;
	TsSpeedButton *MoveLayerBtn;
	TsRadioButton *ZonesRadio;
	TsDecimalSpinEdit *RadiusEdit;
	void __fastcall NameEditChange(TObject *Sender);
	void __fastcall OnChange(TObject *Sender);
	void __fastcall CoordsListDblClick(TObject *Sender);
	void __fastcall RemoveCoordBtnClick(TObject *Sender);
	void __fastcall AddCoordBtnClick(TObject *Sender);
	void __fastcall CopyBtnClick(TObject *Sender);
	void __fastcall PasteBtnClick(TObject *Sender);
	void __fastcall CoordsListKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall InsertAfterClick(TObject *Sender);
	void __fastcall InsertBeforeClick(TObject *Sender);
	void __fastcall CoordsListClick(TObject *Sender);
	void __fastcall UpBtnClick(TObject *Sender);
	void __fastcall DownBtnClick(TObject *Sender);
	void __fastcall TransferLayerClick(TObject *Sender);
	void __fastcall CopyLayerBtnMouseActivate(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y, int HitTest,
          TMouseActivate &MouseActivate);
	void __fastcall MoveLayerBtnMouseActivate(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y, int HitTest,
          TMouseActivate &MouseActivate);
private:	// User declarations
	bool TransferCopyMode;
    void UpdateData();
    void AddCoords(String str,int Pos=-1,int Mode=1);
    DataReference m_Ref;
    IDCMapLayersDataStoragePtr m_pLayers;
    IDCMapGraphicsDataStoragePtr m_pGraphics;
public:		// User declarations
	__fastcall TGraphicsEditFrame(TComponent* Owner);
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    bool OnInit();
    void OnApply();
    bool OnDataTest();
};
//---------------------------------------------------------------------------
extern PACKAGE TGraphicsEditFrame *GraphicsEditFrame;
//---------------------------------------------------------------------------
#endif
