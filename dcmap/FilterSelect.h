//---------------------------------------------------------------------------

#ifndef FilterSelectH
#define FilterSelectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseViewPopup.h"
#include "sSkinProvider.hpp"
#include "sListView.hpp"
#include <ComCtrls.hpp>
#include "sBitBtn.hpp"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFilterSelectForm : public TBaseViewPopupForm
{
__published:	// IDE-managed Components
	TsListView *ItemsView;
	TsBitBtn *OkButton;
	TsBitBtn *CancelButton;
	TsBitBtn *DeleteButton;
	void __fastcall ItemsViewDblClick(TObject *Sender);
	void __fastcall DeleteButtonClick(TObject *Sender);
private:	// User declarations
	bool Changed;
public:		// User declarations
	__fastcall TFilterSelectForm(TComponent* Owner);

        bool Execute(DCMapConfigWrapper cfg);

        String PresetName;
        String PresetSection;
};
//---------------------------------------------------------------------------
extern PACKAGE TFilterSelectForm *FilterSelectForm;
//---------------------------------------------------------------------------
#endif
