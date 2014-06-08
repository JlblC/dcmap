//---------------------------------------------------------------------------

#ifndef PasswordDialogH
#define PasswordDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sButton.hpp"
#include "sEdit.hpp"
#include "sLabel.hpp"
#include "sSkinProvider.hpp"
//---------------------------------------------------------------------------
class TPasswordDialogForm : public TForm
{
__published:	// IDE-managed Components
	TsSkinProvider *sSkinProvider1;
	TsButton *OkBtn;
	TsButton *CancelBtn;
	TsEdit *PasswordEdit;
	TsLabel *Label;
private:	// User declarations
public:		// User declarations
	__fastcall TPasswordDialogForm(TComponent* Owner);
	static bool Execute(String Caption,String Message,String& Value);
};
//---------------------------------------------------------------------------
extern PACKAGE TPasswordDialogForm *PasswordDialogForm;
//---------------------------------------------------------------------------
#endif
