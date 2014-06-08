//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef GraphHelperH
#define GraphHelperH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sAlphaListBox.hpp"
#include "sButton.hpp"
#include "sCheckListBox.hpp"
#include "sSkinProvider.hpp"
#include "BaseViewPopup.h"
#include "sColorSelect.hpp"
#include "sComboBoxes.hpp"
#include "sEdit.hpp"
#include "sGroupBox.hpp"
#include "sSpeedButton.hpp"
#include "sSpinEdit.hpp"
#include <Buttons.hpp>
#include "sToolBar.hpp"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

class TGraphicToolbarFrame;

class TGraphHelperForm : public TBaseViewPopupForm
{
__published:	// IDE-managed Components
	TsGroupBox *LinesGroupBox;
	TsColorSelect *LineColor;
	TsComboBoxEx *LineStyle;
	TsComboBoxEx *LineArrows;
	TsSpinEdit *LineTrns;
	TsSpinEdit *LineWidth;
	TsGroupBox *FillGroupBox;
	TsColorSelect *FillColor;
	TsSpinEdit *FillTrns;
	TsGroupBox *TextGroupBox;
	TsColorSelect *TextColor;
	TsSpinEdit *TextTrns;
	TsSpinEdit *TextSize;
	TsSpinEdit *FillRadius;
	TImageList *ImageList1;
	TsPanel *BottomPanel;
	TsButton *OkBtn;
	TsButton *CancelBtn;
	TsSpeedButton *UndoBtn;
	TsSpeedButton *TextBtn;
	void __fastcall ParamsChnage(TObject *Sender);
	void __fastcall OkbtnClick(TObject *Sender);
	void __fastcall CancelBtnClick(TObject *Sender);
	void __fastcall TntFormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall UndoBtnClick(TObject *Sender);
	void __fastcall TextBtnClick(TObject *Sender);
private:	// User declarations
	void SetupSize();

	IDCMapGraphicLayer* m_pLayer;
	DCMapGraphicsInfo m_GInfo;
	void UpdateData(DCMapGraphicsInfo* pinf);
	TGraphicToolbarFrame* m_pToolbar;
	bool m_bUpdating;
public:		// User declarations
	__fastcall TGraphHelperForm(TComponent* Owner);
	static TGraphHelperForm* CreateInstance(IDCMapWorkspace* pWorkspace);
	void Setup(	TGraphicToolbarFrame* pToolbar,IDCMapGraphicLayer* layer);
	void TextPlaced(bool ok);
};
//---------------------------------------------------------------------------
extern PACKAGE TGraphHelperForm *GraphHelperForm;
//---------------------------------------------------------------------------
#endif
