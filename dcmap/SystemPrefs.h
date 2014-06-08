//---------------------------------------------------------------------------

#ifndef SystemPrefsH
#define SystemPrefsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BasePrefs.h"
#include "sFrameAdapter.hpp"
#include "sCheckBox.hpp"
//---------------------------------------------------------------------------
class TSystemPrefsFrame : public TBasePrefsFrame
{
__published:	// IDE-managed Components
	TsCheckBox *ShowTrayIcon;
	TsCheckBox *MinimizeToTray;
	TsCheckBox *AutoCheckUpdates;
	TsCheckBox *WatchClipbrdForCmd;
private:	// User declarations
public:		// User declarations
	__fastcall TSystemPrefsFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSystemPrefsFrame *SystemPrefsFrame;
//---------------------------------------------------------------------------
#endif
