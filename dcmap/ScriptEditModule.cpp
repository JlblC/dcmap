//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScriptEditModule.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "SynCompletionProposal"
#pragma link "SynEdit"
#pragma link "SynEditHighlighter"
#pragma link "SynHighlighterGeneral"
#pragma link "SynHighlighterPerl"
#pragma link "SynHighlighterDcm"
#pragma link "sToolBar"
#pragma link "sDialogs"
#pragma link "SynHighlighterIni"
#pragma link "SynHighlighterMulti"
#pragma link "SynEditMiscClasses"
#pragma link "SynEditOptionsDialog"
#pragma link "SynEditRegexSearch"
#pragma link "SynEditSearch"
#pragma link "SynHighlighterXML"
#pragma resource "*.dfm"

static CFramePanelWrapper<TScriptEditFrame> sWrap("PView","ScriptEditor");


TScriptEditFrame *ScriptEditFrame;
//---------------------------------------------------------------------------
__fastcall TScriptEditFrame::TScriptEditFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TScriptEditFrame::SetText(UnicodeString Text)
{
	Edit->Text = Text;
	FileName = L"";
}

UnicodeString TScriptEditFrame::GetText()
{
	return Edit->Text;
}
void TScriptEditFrame::ClearLines()
{
	Edit->ClearAll();
}
int TScriptEditFrame::LineCount()
{
	return Edit->Lines->Count;
}

int TScriptEditFrame::HighlightLine(int Line)
{

}

bool TScriptEditFrame::FSetReadonly(bool ro)
{
	FReadonly = ro;
	Edit->ReadOnly = ro;

	Edit->WantReturns = !ro;
	Edit->WantTabs = !ro;

	CutMenuItem->Visible = !ro;
	PasteMenuItem->Visible = !ro;
	UndoMenuItem->Visible = !ro;
	RedoMenuItem->Visible = !ro;
}


void __fastcall TScriptEditFrame::OpenButtonClick(TObject *Sender)
{
	if(OpenDialog->InitialDir.IsEmpty()) OpenDialog->InitialDir = GetAppPath()+"scripts";

	if(OpenDialog->Execute())
	{
		FileName = OpenDialog->FileName;

		dcmapWCSTR text = dcmapLoadText(FileName.w_str());
		if(text) Edit->Lines->Text = text;
		else Edit->Lines->Clear();

		SetupEdit(FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TScriptEditFrame::SaveButtonClick(TObject *Sender)
{
	if(FileName.IsEmpty())SaveAsButtonClick(Sender);
	else
	{
		dcmapSaveText(Edit->Lines->Text.w_str(),FileName.w_str());
	}
}
//---------------------------------------------------------------------------

void __fastcall TScriptEditFrame::SaveAsButtonClick(TObject *Sender)
{
	if(SaveDialog->InitialDir.IsEmpty()) SaveDialog->InitialDir = GetAppPath()+"scripts";

	if(SaveDialog->Execute())
	{
		FileName = SaveDialog->FileName;
		Edit->Lines->SaveToFile(FileName);
		SetupEdit(FileName);
	}

}
//---------------------------------------------------------------------------

void __fastcall TScriptEditFrame::CopyMenuItemClick(TObject *Sender)
{
	Edit->CopyToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TScriptEditFrame::CutMenuItemClick(TObject *Sender)
{
	Edit->CopyToClipboard();
	Edit->ClearSelection();
}
//---------------------------------------------------------------------------

void __fastcall TScriptEditFrame::PasteMenuItemClick(TObject *Sender)
{
	Edit->PasteFromClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TScriptEditFrame::UndoMenuItemClick(TObject *Sender)
{
	Edit->Undo();
}
//---------------------------------------------------------------------------

void __fastcall TScriptEditFrame::RedoMenuItemClick(TObject *Sender)
{
	Edit->Redo();
}
//---------------------------------------------------------------------------
void TScriptEditFrame::SetupEdit(String FileName)
{
	String Ext = ExtractFileExt(FileName);

	if(SameStr(Ext,".dcm"))Edit->Highlighter = SynDcmSyn1;
	else if(SameStr(Ext,".ini"))Edit->Highlighter = SynIniSyn1;
	else if(SameStr(Ext,".lng"))Edit->Highlighter = SynIniSyn1;
	else if(SameStr(Ext,".xml"))Edit->Highlighter = SynXMLSyn1;
	else
	{
		Edit->Highlighter = 0;
    }

}




