//---------------------------------------------------------------------------

#ifndef SecurityPrefsH
#define SecurityPrefsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BasePrefs.h"
#include "sFrameAdapter.hpp"
#include "sEdit.hpp"
#include "sButton.hpp"
#include "sSpinEdit.hpp"
#include "sDialogs.hpp"
#include "sLabel.hpp"
//---------------------------------------------------------------------------
class TSecurityPrefsFrame : public TBasePrefsFrame
{
__published:	// IDE-managed Components
	TsEdit *OldPassword;
	TsEdit *NewPassword;
	TsEdit *NewPasswordCheck;
	TsButton *ChangePassword;
	TsSpinEdit *RememberPasswordTime;
	TsLabel *MinutesLabel;
	void __fastcall ChangePasswordClick(TObject *Sender);
private:	// User declarations
	IDCMapSecretsDataStoragePtr m_pSecrets;

public:		// User declarations
	__fastcall TSecurityPrefsFrame(TComponent* Owner);
	bool OnInit();
	void OnLoadPrefs(DCMapConfigWrapper cfg);
	void OnSavePrefs(DCMapConfigWrapper cfg);
};
//---------------------------------------------------------------------------
extern PACKAGE TSecurityPrefsFrame *SecurityPrefsFrame;
//---------------------------------------------------------------------------
#endif
