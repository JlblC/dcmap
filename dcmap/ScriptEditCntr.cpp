//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScriptEditCntr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SynCompletionProposal"
#pragma link "SynEdit"
#pragma link "SynEditHighlighter"
#pragma link "SynHighlighterGeneral"
#pragma resource "*.dfm"
TScriptEditFrame *ScriptEditFrame;
//---------------------------------------------------------------------------
__fastcall TScriptEditFrame::TScriptEditFrame(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
