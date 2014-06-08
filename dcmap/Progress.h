//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef ProgressH
#define ProgressH
//---------------------------------------------------------------------------
#include "dcmapsyslib.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sGauge.hpp"
#include "sMemo.hpp"
#include "sSkinProvider.hpp"
#include "sButton.hpp"
#include "sLabel.hpp"
#include "BaseViewPopup.h"
//---------------------------------------------------------------------------
class TProgressForm : public TBaseViewPopupForm
{
	typedef TBaseViewPopupForm Inherited;
__published:	// IDE-managed Components
	TsGauge *ProgressBar;
	TsMemo *MessagesMemo;
	TsButton *CancelBtn;
	TsLabel *StatusLabel;
	TsButton *ContinueBtn;
	TsButton *RetryBtn;
	void __fastcall CancelBtnClick(TObject *Sender);
	void __fastcall ContinueBtnClick(TObject *Sender);
	void __fastcall RetryBtnClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TProgressForm(TComponent* Owner);
	IDCMapProcessRecordReseiver *Fidder();
	bool Canceling;
	bool GoOn;
	bool Retry;
};
//---------------------------------------------------------------------------
extern PACKAGE TProgressForm *ProgressForm;
//---------------------------------------------------------------------------
#endif
