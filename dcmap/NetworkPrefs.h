//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef NetworkPrefsH
#define NetworkPrefsH
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
class TNetSettingsFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsCheckBox *UseProxyCheck;
	TsEdit *GameURLEdit;
	TsGroupBox *sGroupBox1;
	TsEdit *ProxyEdit;
	TsSpinEdit *PortEdit;
	TsEdit *UsernameEdit;
	TsEdit *PasswordEdit;
	TsCheckBox *BasicAuthCheck;
	void __fastcall OnChange(TObject *Sender);
	void __fastcall UseProxyCheckClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TNetSettingsFrame(TComponent* Owner);
    void LoadConfig(DCMapConfigWrapper cfg);
    void OnCansel();
    void OnApply();
};
//---------------------------------------------------------------------------
extern PACKAGE TNetSettingsFrame *NetSettingsFrame;
//---------------------------------------------------------------------------
#endif
