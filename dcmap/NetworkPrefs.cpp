//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NetworkPrefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sCheckBox"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma link "sGroupBox"
#pragma resource "*.dfm"
TNetSettingsFrame *NetSettingsFrame;

static CFramePanelWrapper<TNetSettingsFrame> sWrap("PPrefs_base","NetSettings");

//---------------------------------------------------------------------------
__fastcall TNetSettingsFrame::TNetSettingsFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TNetSettingsFrame::LoadConfig(DCMapConfigWrapper cfg)
{
    cfg.section("NetSettings");

    String str;
    int    val;
    bool   bval;

    if(cfg["ProxyServer"] >> str)ProxyEdit->Text = str;
    if(cfg["ProxyPort"] >> val)PortEdit->Value = val;
    if(cfg["ProxyUser"] >> str)UsernameEdit->Text = str;
    if(cfg["ProxyPass"] >> str)PasswordEdit->Text = str;
    if(cfg["GameURL"] >> str)GameURLEdit->Text = str;
    if(cfg["UseBasicAuth"] >> bval)BasicAuthCheck->Checked = bval;
    if(cfg["UseProxy"] >> bval)UseProxyCheck->Checked = bval;

    UseProxyCheckClick(this);
    Changed=false;
}

void TNetSettingsFrame::OnCansel()
{
    DCMapConfigWrapper cfg(m_pCore->GetConfig());
    LoadConfig(cfg);
}

void TNetSettingsFrame::OnApply()
{
    DCMapConfigWrapper cfg(m_pCore->GetConfig());
    cfg.section("NetSettings");

    cfg["ProxyServer"] << ProxyEdit->Text;
    cfg["ProxyPort"] << PortEdit->Value;
    cfg["ProxyUser"] << UsernameEdit->Text;
    cfg["ProxyPass"] << PasswordEdit->Text;
    cfg["GameURL"] << GameURLEdit->Text;
    cfg["UseBasicAuth"] << BasicAuthCheck->Checked;
    cfg["UseProxy"] << UseProxyCheck->Checked;

    Changed=false;
}
void __fastcall TNetSettingsFrame::OnChange(TObject *Sender)
{
	Changed = true;
}
//---------------------------------------------------------------------------

void __fastcall TNetSettingsFrame::UseProxyCheckClick(TObject *Sender)
{
    ProxyEdit->Enabled = UseProxyCheck->Checked;
    PortEdit->Enabled = UseProxyCheck->Checked;
    UsernameEdit->Enabled = UseProxyCheck->Checked;
    PasswordEdit->Enabled = UseProxyCheck->Checked;
    BasicAuthCheck->Enabled = UseProxyCheck->Checked;
    ProxyEdit->Enabled = UseProxyCheck->Checked;
    ProxyEdit->Enabled = UseProxyCheck->Checked;
}
//---------------------------------------------------------------------------

