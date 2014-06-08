//---------------------------------------------------------------------------

#ifndef DataImportH
#define DataImportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sMemo.hpp"
#include "sPanel.hpp"
#include "sSkinProvider.hpp"
#include "sSplitter.hpp"
#include <ExtCtrls.hpp>
#include "sAlphaListBox.hpp"
#include "sCheckListBox.hpp"
#include "sCustomComboEdit.hpp"
#include "sMaskEdit.hpp"
#include "sRadioButton.hpp"
#include "sTooledit.hpp"
#include <Mask.hpp>
#include "sEdit.hpp"
#include "sButton.hpp"
#include "sComboBox.hpp"

#include "dcmapsyslib.h"
#include "sLabel.hpp"
#include "sPageControl.hpp"
#include <ComCtrls.hpp>
#include "sGauge.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
//#include "TntForms.hpp"
//#include "TntMenus.hpp"
#include "acProgressBar.hpp"
#include "BaseViewPopup.h"
#include "BaseModule.h"
#include "ScriptEditModule.h"

//---------------------------------------------------------------------------
class TDataImportForm : public TBaseViewPopupForm
{
	typedef TBaseViewPopupForm Inherited;
__published:	// IDE-managed Components
    TsPanel *sPanel1;
    TsSplitter *sSplitter1;
    TsPanel *sPanel2;
    TsRadioButton *FileRadio;
    TsFilenameEdit *FilenameEdit;
    TsRadioButton *ClipboardButton;
    TsComboBox *ModuleList;
    TsPanel *sPanel3;
	TsButton *ImportButton;
    TsButton *ReadButton;
	TsButton *ProcessButton;
    TsComboBox *ParamEdit;
    TsLabel *MessageLabel;
	TsButton *CloseButton;
	TsPageControl *Pages;
	TsTabSheet *RecordsTab;
	TsTabSheet *ErrorsTab;
	TsTabSheet *MessagesTab;
    TsCheckListBox *RecordList;
	TsRadioButton *ModuleRadio;
	TsComboBox *sComboBox1;
	TsMemo *MessagesMemo;
	TsListBox *ErrorsList;
	TsRadioButton *ReplaceRadio;
	TsRadioButton *AppentRadio;
	TsComboBox *TableCombo;
	TsSpeedButton *SaveParam;
	TsSpeedButton *DeleteParam;
	TsComboBox *CodepageComboBox;
	TsGauge *Progress;
	TScriptEditFrame *ScriptEditFrame;
    void __fastcall ReadButtonClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ProcessButtonClick(TObject *Sender);
    void __fastcall ImportButtonClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FilenameEditButtonClick(TObject *Sender);
    void __fastcall ModuleListChange(TObject *Sender);
    void __fastcall RecordListClick(TObject *Sender);
	void __fastcall ErrorsListClick(TObject *Sender);
	void __fastcall TableComboChange(TObject *Sender);
	void __fastcall SaveParamClick(TObject *Sender);
	void __fastcall DeleteParamClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
	//UnicodeString SourceData;
	IDCMapImportProcessor* m_pModule;
	void ClearRecords();
	void ClearMessages();
    bool m_bParamGood;
    String ParamSection;
    bool InitModule();
    void SaveModuleConfig();
    void LoadModuleConfig();
    void SaveConfig();
	void LoadConfig();
	int m_ModuleParams;
	String Source;
	String CurrentModule;
public:		// User declarations
    __fastcall TDataImportForm(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TDataImportForm *DataImportForm;
//---------------------------------------------------------------------------
#endif
