//---------------------------------------------------------------------------

#ifndef ScriptEditCntrH
#define ScriptEditCntrH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SynCompletionProposal.hpp"
#include "SynEdit.hpp"
#include "SynEditHighlighter.hpp"
#include "SynHighlighterGeneral.hpp"
//---------------------------------------------------------------------------
class TScriptEditFrame : public TFrame
{
__published:	// IDE-managed Components
	TSynEdit *Edit;
	TSynCompletionProposal *SynCompletionProposal1;
	TSynGeneralSyn *SynGeneralSyn1;
private:	// User declarations
public:		// User declarations
	__fastcall TScriptEditFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TScriptEditFrame *ScriptEditFrame;
//---------------------------------------------------------------------------
#endif
