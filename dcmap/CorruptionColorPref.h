//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef CorruptionColorPrefH
#define CorruptionColorPrefH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sColorSelect.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "BasePrefs.h"
//---------------------------------------------------------------------------
class TCorruptionPrefFrame : public TBasePrefsFrame
{
__published:	// IDE-managed Components
	TsColorSelect *Corruption0;
	TsColorSelect *Corruption10;
	TsColorSelect *Corruption25;
	TsColorSelect *Corruption50;
	TsColorSelect *Corruption85;
	TsColorSelect *Corruption95;
private:	// User declarations
public:		// User declarations
	__fastcall TCorruptionPrefFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCorruptionPrefFrame *CorruptionPrefFrame;
//---------------------------------------------------------------------------
#endif
