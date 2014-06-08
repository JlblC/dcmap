//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef DataExportH
#define DataExportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sSkinProvider.hpp"
#include "sMemo.hpp"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include "sComboBox.hpp"
#include "sButton.hpp"
#include "sGauge.hpp"
#include "dcmapsyslib.h"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "sDialogs.hpp"
#include <Dialogs.hpp>
//#include "TntForms.hpp"
//#include "TntMenus.hpp"
#include "sComboEdit.hpp"
#include "sCustomComboEdit.hpp"
#include "sMaskEdit.hpp"
#include <Mask.hpp>
#include "BaseViewPopup.h"

//---------------------------------------------------------------------------
class TDataExportForm : public TBaseViewPopupForm
{
__published:	// IDE-managed Components
	TsPanel *sPanel1;
	TsPanel *sPanel2;
	TsMemo *OutMemo;
	TsComboBox *ParamEdit;
	TsComboBox *ModuleList;
	TsComboBox *TableCombo;
	TsButton *ProcessButton;
	TsButton *SaveButton;
	TsGauge *Gauge;
	TsButton *CloseButton;
	TsSpeedButton *SaveParam;
	TsSaveDialog *SaveDialog;
	TsSpeedButton *DeleteParam;
	TsSpeedButton *CopyData;
	TsComboBox *FilterCombo;
	TsComboEdit *ExpressionEdit;
	TsComboBox *CodepageComboBox;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ModuleListChange(TObject *Sender);
	void __fastcall TableComboChange(TObject *Sender);
	void __fastcall ProcessButtonClick(TObject *Sender);
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall SaveParamClick(TObject *Sender);
	void __fastcall DeleteParamClick(TObject *Sender);
	void __fastcall CopyDataClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FilterComboChange(TObject *Sender);
	void __fastcall ExpressionEditButtonClick(TObject *Sender);
private:	// User declarations
    IDCMapExportProcessor* m_pModule;
    String ParamSection;
    String LastTable;
    bool m_bParamGood;
    bool InitModule();
    void SaveModuleConfig();
    void LoadModuleConfig();
    void SaveConfig();
	void LoadConfig();
public:		// User declarations
	UnicodeString Accum;
	__fastcall TDataExportForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDataExportForm *DataExportForm;
//---------------------------------------------------------------------------
#endif
