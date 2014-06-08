//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef DipColorPrefH
#define DipColorPrefH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sDialogs.hpp"
#include <Dialogs.hpp>
#include "sButton.hpp"
#include "sColorSelect.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "sGroupBox.hpp"
//---------------------------------------------------------------------------
class TDipColorPrefFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsColorSelect *sColor_EMPTY;
	TsColorSelect *sColor_UNDEFINED;
	TsButton *OthersButton;
	void __fastcall DipColorChange(TObject *Sender);
	void __fastcall OthersButtonClick(TObject *Sender);
private:	// User declarations
	COLORREF Colors[12];
public:		// User declarations
	__fastcall TDipColorPrefFrame(TComponent* Owner);
    void LoadConfig(DCMapConfigWrapper cfg);
	void SaveConfig(DCMapConfigWrapper cfg);
    void OnApply();
    void OnShow();
};
//---------------------------------------------------------------------------
extern PACKAGE TDipColorPrefFrame *DipColorPrefFrame;
//---------------------------------------------------------------------------
#endif
