//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SecurityPrefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BasePrefs"
#pragma link "sFrameAdapter"
#pragma link "sEdit"
#pragma link "sButton"
#pragma link "sSpinEdit"
#pragma link "sLabel"
#pragma resource "*.dfm"
TSecurityPrefsFrame *SecurityPrefsFrame;

static CFramePanelWrapper<TSecurityPrefsFrame> sWrap("PPrefs_base","SecurityPrefs");

//---------------------------------------------------------------------------
__fastcall TSecurityPrefsFrame::TSecurityPrefsFrame(TComponent* Owner)
	: TBasePrefsFrame(Owner)
{
	AddConfigTarget("Secrets",1);
}
//---------------------------------------------------------------------------

bool TSecurityPrefsFrame::OnInit()
{
	if(!m_pSecrets.init(m_pCore))return false;

	return TBasePrefsFrame::OnInit();
}

void TSecurityPrefsFrame::OnLoadPrefs(DCMapConfigWrapper cfg)
{

}

void TSecurityPrefsFrame::OnSavePrefs(DCMapConfigWrapper cfg)
{


}

void __fastcall TSecurityPrefsFrame::ChangePasswordClick(TObject *Sender)
{
	if(!m_pSecrets->CheckPassword(OldPassword->Text.w_str()))
	{
		sMessageDlg(LSTRW(WrongOldPassword),mtError,TMsgDlgButtons()<<mbOK,0);
		return;
	}
	if(NewPassword->Text != NewPasswordCheck->Text)
	{
		sMessageDlg(LSTRW(PasswordsMismatch),mtError,TMsgDlgButtons()<<mbOK,0);
		return;
	}

	if(!m_pSecrets->ChangePassword(OldPassword->Text.w_str(),NewPasswordCheck->Text.w_str()))
	{
		sMessageDlg(LSTRW(PasswordChangeFailed),mtError,TMsgDlgButtons()<<mbOK,0);
	}
	else
	{
		sMessageDlg(LSTRW(PasswordChanged),mtInformation,TMsgDlgButtons()<<mbOK,0);
	}
}
//---------------------------------------------------------------------------


