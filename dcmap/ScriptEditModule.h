//---------------------------------------------------------------------------

#ifndef ScriptEditModuleH
#define ScriptEditModuleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "SynCompletionProposal.hpp"
#include "SynEdit.hpp"
#include "SynEditHighlighter.hpp"
#include "SynHighlighterGeneral.hpp"
#include "SynHighlighterPerl.hpp"
#include "SynHighlighterDcm.hpp"
#include "sToolBar.hpp"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include "sDialogs.hpp"
#include <Dialogs.hpp>
#include "SynHighlighterIni.hpp"
#include "SynHighlighterMulti.hpp"
#include "SynEditMiscClasses.hpp"
#include "SynEditOptionsDialog.hpp"
#include "SynEditRegexSearch.hpp"
#include "SynEditSearch.hpp"
#include "SynHighlighterXML.hpp"

//---------------------------------------------------------------------------
class TScriptEditFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TSynEdit *Edit;
	TSynCompletionProposal *SynCompletionProposal1;
	TSynDcmSyn *SynDcmSyn1;
	TsToolBar *sToolBar1;
	TPopupMenu *EditMenu;
	TToolButton *SaveButton;
	TToolButton *OpenButton;
	TToolButton *SaveAsButton;
	TsOpenDialog *OpenDialog;
	TsSaveDialog *SaveDialog;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolButton *CutButton;
	TToolButton *CopyButton;
	TToolButton *PasteButton;
	TMenuItem *CutMenuItem;
	TMenuItem *CopyMenuItem;
	TMenuItem *PasteMenuItem;
	TMenuItem *UndoMenuItem;
	TMenuItem *RedoMenuItem;
	TMenuItem *N1;
	TToolButton *ToolButton3;
	TToolButton *UndoButton;
	TToolButton *RedoButton;
	TSynIniSyn *SynIniSyn1;
	TSynEditSearch *Search;
	TSynEditRegexSearch *RegexSearch;
	TSynEditOptionsDialog *OptionsDialog;
	TSynXMLSyn *SynXMLSyn1;
	TMainMenu *FrameMenu;
	TMenuItem *Edit1;
	TMenuItem *Undo1;
	TMenuItem *Redo1;
	TMenuItem *i1;
	TMenuItem *New1;
	TMenuItem *Open1;
	TMenuItem *Save1;
	TMenuItem *SaveAs1;
	void __fastcall OpenButtonClick(TObject *Sender);
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall SaveAsButtonClick(TObject *Sender);
	void __fastcall CopyMenuItemClick(TObject *Sender);
	void __fastcall CutMenuItemClick(TObject *Sender);
	void __fastcall PasteMenuItemClick(TObject *Sender);
	void __fastcall UndoMenuItemClick(TObject *Sender);
	void __fastcall RedoMenuItemClick(TObject *Sender);
private:	// User declarations
	String FileName;
	bool FReadonly;
	bool FSetReadonly(bool ro);
public:		// User declarations
	__fastcall TScriptEditFrame(TComponent* Owner);
	void SetText(UnicodeString Text);
	UnicodeString GetText();
	void SetupEdit(String FileName);
	void ClearLines();
	int LineCount();
	int HighlightLine(int Line);

	__property bool Readonly = {read=FReadonly,write=FSetReadonly};

};
//---------------------------------------------------------------------------
extern PACKAGE TScriptEditFrame *ScriptEditFrame;
//---------------------------------------------------------------------------
#endif
