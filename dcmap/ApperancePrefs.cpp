//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ApperancePrefs.h"
#include "main.h"
#include "Utils.h"
#include "WebProc.h"
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
TApperancePrefsFrame *ApperancePrefsFrame;

static CFramePanelWrapper<TApperancePrefsFrame> sWrap("PPrefs_base","00_ApperancePrefs");

//---------------------------------------------------------------------------
__fastcall TApperancePrefsFrame::TApperancePrefsFrame(TComponent* Owner)
	: TBasePrefsFrame(Owner)
{
	//MainForm->sSkinManager->AnimEffects->SkinChanging->Active = true;
}
__fastcall TApperancePrefsFrame::~TApperancePrefsFrame()
{
	//MainForm->sSkinManager->AnimEffects->SkinChanging->Active = false;
}
//---------------------------------------------------------------------------

void TApperancePrefsFrame::LoadConfig(DCMapConfigWrapper cfg)
{
}

void TApperancePrefsFrame::OnCansel()
{
	if(Changed && PreviewCheck->Checked)
	{
		RestoreVisual();
	}
}

bool TApperancePrefsFrame::OnInit()
{
	if(m_pCore->GetSystemOptions()->bNoSkins)return false;

	return true;
}

void TApperancePrefsFrame::OnShow()
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

	if(m_pCore->GetSystemOptions()->bAero)
	{
		AeroBordersCheck->Visible = true;
		bool bAero=false;
		cfg["AeroBorders"] >> bAero;
		AeroBordersCheck->Checked = bAero;
	}


	bool HideTabs=1;
	cfg["HideTabs"] >> HideTabs;
	AlwaysShowTabsCheck->Checked=!HideTabs;

	SkinEnableCheckClick(this);

	Changed = false;
}

void TApperancePrefsFrame::ApplyVisual()
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

void TApperancePrefsFrame::RestoreVisual()
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

void TApperancePrefsFrame::OnApply()
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

	if(AeroBordersCheck->Visible)
	{
		cfg["AeroBorders"] << AeroBordersCheck->Checked;
	}

	ApplyVisual();

	if(m_bPreSkinState != SkinEnableCheck->Checked)
	{
		if(!SkinEnableCheck->Checked)
		{
			InterfaceModule->SkinManager->Active = false;
			MainForm->CloseTabs();
			MainForm->RestoreSessions();
		}
	}

	m_bPreSkinState = SkinEnableCheck->Checked;

	Changed = false;
}
void __fastcall TApperancePrefsFrame::OnChange(TObject *Sender)
{
	Changed = true;	
}
//---------------------------------------------------------------------------

void __fastcall TApperancePrefsFrame::SkinEnableCheckClick(TObject *Sender)
{
	Changed = true;
	SkinList->Enabled = SkinEnableCheck->Checked;
	SatBar->Enabled = SkinEnableCheck->Checked;
	HueBar->Enabled = SkinEnableCheck->Checked;
	SatLabel->Enabled = SkinEnableCheck->Checked;
	HueLabel->Enabled = SkinEnableCheck->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TApperancePrefsFrame::PreviewCheckClick(TObject *Sender)
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

void __fastcall TApperancePrefsFrame::SatBarChange(TObject *Sender)
{
	if(PreviewCheck->Checked)
		InterfaceModule->SkinManager->Saturation = SatBar->Position;
	Changed = true;
}
//---------------------------------------------------------------------------

void __fastcall TApperancePrefsFrame::HueBarChange(TObject *Sender)
{
	if(PreviewCheck->Checked)
		InterfaceModule->SkinManager->HueOffset = HueBar->Position;
	Changed = true;
}
//---------------------------------------------------------------------------

void __fastcall TApperancePrefsFrame::SkinListClick(TObject *Sender)
{
	if(PreviewCheck->Checked)
   		InterfaceModule->SkinManager->SkinName = SkinList->Items->Strings[SkinList->ItemIndex];
	Changed = true;
}
//---------------------------------------------------------------------------

void __fastcall TApperancePrefsFrame::DownloadSkinsBtnClick(TObject *Sender)
{
	if(!WebModule)
       WebModule = new TWebModule(Application);

	WebModule->DownloadFiles(L"http://dcmap.googlecode.com/svn/update/skins/list.txt",
									GetAppPath()+L"Skins/");

	InterfaceModule->SkinManager->GetSkinNames(SkinList->Items);
	SkinList->ItemIndex = SkinList->Items->IndexOf(InterfaceModule->SkinManager->SkinName);

}
//---------------------------------------------------------------------------

