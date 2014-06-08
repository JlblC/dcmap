//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef MapPrefsH
#define MapPrefsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sCheckBox.hpp"
#include "sEdit.hpp"
#include "sSpinEdit.hpp"
#include "sGroupBox.hpp"
//---------------------------------------------------------------------------
class TMapPrefsFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsCheckBox *HighQuality;
	TsCheckBox *SelectEmptySpace;
	TsCheckBox *SwapButtons;
	TsCheckBox *ClassicHLine;
	TsCheckBox *RightClickSelect;
	TsCheckBox *EnableToolTip;
	TsGroupBox *sGroupBox1;
	TsSpinEdit *ToolTipDelay;
	TsCheckBox *LeftMouseDrag;
	void __fastcall OnChanged(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMapPrefsFrame(TComponent* Owner);
    void LoadConfig(DCMapConfigWrapper cfg);
	void OnApply();
};
//---------------------------------------------------------------------------
extern PACKAGE TMapPrefsFrame *MapPrefsFrame;
//---------------------------------------------------------------------------
#endif
