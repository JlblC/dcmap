//---------------------------------------------------------------------------

#ifndef ScriptEditorH
#define ScriptEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ScriptEditCntr.h"
#include <ExtCtrls.hpp>
#include <tntForms.hpp>
//---------------------------------------------------------------------------
class TScriptEditForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
private:	// User declarations
public:		// User declarations
	__fastcall TScriptEditForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TScriptEditForm *ScriptEditForm;
//---------------------------------------------------------------------------
#endif
