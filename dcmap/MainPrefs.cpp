//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainPrefs.h"
#include "main.h"
#include "Utils.h"
#include "WebProcess.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sCheckBox"
#pragma link "sAlphaListBox"
#pragma link "sComboBox"
#pragma link "sTreeView"
#pragma link "sTrackBar"
#pragma link "sLabel"
#pragma link "sBitBtn"
#pragma resource "*.dfm"
TMainPrefsFrame *MainPrefsFrame;

static CFramePanelWrapper<TMainPrefsFrame> sWrap("PPrefs_base","00_MainPrefs");

//---------------------------------------------------------------------------
__fastcall TMainPrefsFrame::TMainPrefsFrame(TComponent* Owner)
	: TBasePrefsFrame(Owner)
{
	//MainForm->sSkinManager->AnimEffects->SkinChanging->Active = true;
}
__fastcall TMainPrefsFrame::~TMainPrefsFrame()
{
	//MainForm->sSkinManager->AnimEffects->SkinChanging->Active = false;
}
//---------------------------------------------------------------------------

void TMainPrefsFrame::LoadConfig(DCMapConfigWrapper cfg)
{
}

void TMainPrefsFrame::OnCansel()
{
	if(Changed && PreviewCheck->Checked)
	{
		RestoreVisual();
	}
}

bool TMainPrefsFrame::OnInit()
{
	if(m_pCore->GetSystemOptions()->bNoSkins)return false;

	return true;
}

void TMainPrefsFrame::OnShow()
{
	DCMapConfigWrapper cfg(m_pCore->GetConfig());

	int bEnable=true;

	cfg.section("Appearance");
	cfg["EnableAnimations"] >> bEnable;
	AnimationCheck->Checked = bEnable;
	bEnable=true;
	cfg["EnableSkin"] >> bEnable;
	SkinEnableCheck->Checked = bEnable;
	InterfaceModule->SkinManager->GetSkinNames(SkinList->Items);
	String SkinName;
	cfg["SkinName"] >> SkinName;
	SkinList->ItemIndex = SkinList->Items->IndexOf(InterfaceModule->SkinManager->SkinName);

	HueBar->Position =  InterfaceModule->SkinManager->HueOffset;
	SatBar->Position =  InterfaceModule->SkinManager->Saturation;

	m_bPreSkinState = InterfaceModule->SkinManager->Active;
	m_PreSkin = InterfaceModule->SkinManager->SkinName;
	m_PreSat  = InterfaceModule->SkinManager->Saturation;
	m_PreHue  = InterfaceModule->SkinManager->HueOffset;

	cfg["SkinName"] >> SkinName;

	if(cfg["XPStyles"] >> bEnable)XPStylesCheck->Checked = bEnable;

	bool HideTabs=1;
	cfg["HideTabs"] >> HideTabs;
	AlwaysShowTabsCheck->Checked=!HideTabs;

	SkinEnableCheckClick(this);

	Changed = false;
}

void TMainPrefsFrame::ApplyVisual()
{
	InterfaceModule->SkinManager->FHueOffset = HueBar->Position;
	InterfaceModule->SkinManager->FSaturation = SatBar->Position;

	if(InterfaceModule->SkinManager->Active)
	{
	   InterfaceModule->SkinManager->FHueOffset = HueBar->Position;
	   InterfaceModule->SkinManager->Active = SkinEnableCheck->Checked;
	   InterfaceModule->SkinManager->SkinName = SkinList->Items->Strings[SkinList->ItemIndex];
	}
	else
	{
	   InterfaceModule->SkinManager->SkinName = SkinList->Items->Strings[SkinList->ItemIndex];
	   InterfaceModule->SkinManager->Active = SkinEnableCheck->Checked;
	}
}

void TMainPrefsFrame::RestoreVisual()
{
	InterfaceModule->SkinManager->FHueOffset = m_PreHue;
	InterfaceModule->SkinManager->FSaturation = m_PreSat;

	if(InterfaceModule->SkinManager->Active)
	{
	   InterfaceModule->SkinManager->FHueOffset = m_PreHue;
	   InterfaceModule->SkinManager->Active = m_bPreSkinState;
	   InterfaceModule->SkinManager->SkinName = m_PreSkin;
	}
	else
	{
	   InterfaceModule->SkinManager->SkinName = m_PreSkin;
	   InterfaceModule->SkinManager->Active = m_bPreSkinState;
	}
}

void TMainPrefsFrame::OnApply()
{
	if(!Changed)return;

    DCMapConfigWrapper cfg(m_pCore->GetConfig());
    cfg.section("Appearance");
	bool bEnable=true;

	cfg["EnableAnimations"] << AnimationCheck->Checked;

    cfg["EnableSkin"] << SkinEnableCheck->Checked;
    cfg["SkinName"] << SkinList->Items->Strings[SkinList->ItemIndex];

    cfg["HueOffset"] << HueBar->Position;
    cfg["Saturation"] << SatBar->Position;

    cfg["HideTabs"] << !AlwaysShowTabsCheck->Checked;;

	cfg["XPStyles"] << XPStylesCheck->Checked;

	InterfaceModule->EnableAnimations(AnimationCheck->Checked);

	if(m_bPreSkinState != SkinEnableCheck->Checked)
	   MainForm->CloseTabs();

	ApplyVisual();

	if(m_bPreSkinState != SkinEnableCheck->Checked)
	   MainForm->RestoreSessions();

	m_bPreSkinState = SkinEnableCheck->Checked;

	MainForm->Configure();
    Changed = false;
}
void __fastcall TMainPrefsFrame::OnChange(TObject *Sender)
{
	Changed = true;	
}
//---------------------------------------------------------------------------

void __fastcall TMainPrefsFrame::SkinEnableCheckClick(TObject *Sender)
{
	Changed = true;
	SkinList->Enabled = SkinEnableCheck->Checked;
	SatBar->Enabled = SkinEnableCheck->Checked;
	HueBar->Enabled = SkinEnableCheck->Checked;
	SatLabel->Enabled = SkinEnableCheck->Checked;
	HueLabel->Enabled = SkinEnableCheck->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainPrefsFrame::PreviewCheckClick(TObject *Sender)
{
	if(Changed && PreviewCheck->Checked)
	{
		ApplyVisual();
	}
	else if(Changed && !PreviewCheck->Checked)
	{
		RestoreVisual();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainPrefsFrame::SatBarChange(TObject *Sender)
{
	if(PreviewCheck->Checked)
		InterfaceModule->SkinManager->Saturation = SatBar->Position;
	Changed = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainPrefsFrame::HueBarChange(TObject *Sender)
{
	if(PreviewCheck->Checked)
		InterfaceModule->SkinManager->HueOffset = HueBar->Position;
	Changed = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainPrefsFrame::SkinListClick(TObject *Sender)
{
	if(PreviewCheck->Checked)
   		InterfaceModule->SkinManager->SkinName = SkinList->Items->Strings[SkinList->ItemIndex];
	Changed = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainPrefsFrame::DownloadSkinsBtnClick(TObject *Sender)
{
	if(!WebProcessForm)
       WebProcessForm = new TWebProcessForm(Application->MainForm);

	WebProcessForm->DownloadFiles(L"http://dcmap.googlecode.com/svn/update/skins/list.txt",
									GetAppPath()+L"Skins/");

	InterfaceModule->SkinManager->GetSkinNames(SkinList->Items);
	SkinList->ItemIndex = SkinList->Items->IndexOf(InterfaceModule->SkinManager->SkinName);

}
//---------------------------------------------------------------------------

