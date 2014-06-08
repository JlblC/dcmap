//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef UIDialogFormH
#define UIDialogFormH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sSkinProvider.hpp"
#include "sFrameAdapter.hpp"

//---------------------------------------------------------------------------
class TDialogForm : public TForm
{
__published:	// IDE-managed Components
	TsSkinProvider *sSkinProvider1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TntFormDestroy(TObject *Sender);
private:	// User declarations
	TFrame* Frame;
	IDCMapVCLPanel* Panel;
public:		// User declarations
	__fastcall TDialogForm(TComponent* Owner);
	bool Setup(IDCMapModule* pModule,dcmapWCSTR Caption, int Flags);
};
//---------------------------------------------------------------------------
extern PACKAGE TDialogForm *DialogForm;
//---------------------------------------------------------------------------
#endif
