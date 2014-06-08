//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef ConsoleH
#define ConsoleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sMemo.hpp"
#include "sSplitter.hpp"
#include <ExtCtrls.hpp>
#include "sPanel.hpp"
#include "sSkinProvider.hpp"
#include "dcmapsyslib.h"
#include "sButton.hpp"
//#include "TntForms.hpp"
#include "SynEdit.hpp"
#include "SynEditHighlighter.hpp"
#include "SynHighlighterGeneral.hpp"
#include "SynCompletionProposal.hpp"
#include "BaseModule.h"
#include "ScriptEditModule.h"
#include "BaseViewPopup.h"

//---------------------------------------------------------------------------
class TConsoleForm : public TBaseViewPopupForm
{
__published:	// IDE-managed Components
	TsMemo *ResultMemo;
	TsSplitter *sSplitter1;
	TsPanel *sPanel1;
	TsButton *ExecuteButton;
	TScriptEditFrame *ScriptEditFrame;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ExecuteButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall SourceMemoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TntFormShow(TObject *Sender);
private:	// User declarations
	IDCMapConsole* m_pConsole;
public:		// User declarations
	__fastcall TConsoleForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TConsoleForm *ConsoleForm;
//---------------------------------------------------------------------------
#endif
