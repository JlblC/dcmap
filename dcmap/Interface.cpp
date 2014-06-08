//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Interface.h"
#include "BaseModule.h"
#include "sComboBox.hpp"
#include "sRadioButton.hpp"
#include "sCheckBox.hpp"
#include "Localize.h"
#include "sLabel.hpp"
#include "sBevel.hpp"
#include "sButton.hpp"
#include "sEdit.hpp"
#include "sSpeedButton.hpp"
#include "sGroupBox.hpp"
#include "sAlphaListBox.hpp"
#include "sColorSelect.hpp"
#include "sComboBoxes.hpp"
#include "sSpinEdit.hpp"
#include "sMemo.hpp"
#include "sTreeView.hpp"
#include "sListView.hpp"
#include "Utils.h"
#include "sPageControl.hpp"
#include "sCheckListBox.hpp"
#include "sCustomComboEdit.hpp"
#include "sTooledit.hpp"
#include "sTrackBar.hpp"
#include "sComboEdit.hpp"
#include "sToolBar.hpp"
#include "main.h"
#include "System.h"
#include "Splash.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "acAlphaHints"
#pragma link "sHintManager"
#pragma link "acAlphaImageList"
#pragma link "sSkinManager"
#pragma link "sDialogs"
#pragma resource "*.dfm"
TInterfaceModule *InterfaceModule;

static CAutoReg<CInterfaceService> sWrap("PCoreDaemon","Interface");

CInterfaceService::CInterfaceService()
{

}

bool  DCMAPAPI CInterfaceService::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	return true;
}

int   DCMAPAPI CInterfaceService::OnMessage( int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender )
{
	if(Msg == DCMM_CONFIG_CHANGED)
	{
		InterfaceModule->LoadConfig();
	}
	return 0;
}

int DCMAPAPI CInterfaceService::ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
{
	if(type == DCMEXEC_QUERY) return DCMEXEC_CALL;

	return DCMEXEC_FAIL;
}

static bool TotalHints = true;
static bool HelperHints = false;

//---------------------------------------------------------------------------
__fastcall TInterfaceModule::TInterfaceModule(TComponent* Owner)
	: TDataModule(Owner)
{
	StdFontFace = "MS Sans Serif";
	ScaledFontFace = "Arial";

    	m_iIconUpdate=0;
	MenuIconUpdates=false;

	m_bInitComplete=false;
	m_bEnableAnimations=true;
	m_bAeroBorders=false;

	HelperHints = atoi(g_pLocalStrings->GetSectionString("HelperHints","Tools"));
	TotalHints  = HelperHints;
}

void TInterfaceModule::LoadConfig()
{
	DCMapConfigWrapper  Config(GetDCMapCore()->GetConfig());

	DCMapSystemOptions const* pOpts = m_pSys->GetSystemOptions();

	if(pOpts->bNoSkins)
	{
			SkinManager->Active = false;
	}
	else
	{
		Config.section("Appearance");
		{
			String Str;
			bool b;
			int i;
			bool Changed=false;
			bool bEnabled=true;

                        Config["EnableSkin"] >> bEnabled;
                        if(!bEnabled)SkinManager->Active = bEnabled;

			m_bEnableAnimations = true;
			Config["EnableAnimations"] >> m_bEnableAnimations;

			if(MainForm)EnableAnimations();

			if(Config["HueOffset"] >> i)
			{
				if(i != SkinManager->FHueOffset)
				{
					SkinManager->FHueOffset=i;
					Changed = true;
				}
			}

			if(Config["Saturation"] >> i)
			{
				if(i != SkinManager->FSaturation)
				{
					SkinManager->FSaturation=i;
					Changed = true;
				}
			}
                        TStringList* pSkinList = new TStringList;
                        SkinManager->GetSkinNames(pSkinList);
			if(Config["SkinName"] >> Str)
			{
				if(SkinManager->SkinName != Str)
				{
					if(pSkinList->IndexOf(Str) >=0)
					{
						SkinManager->SkinName = Str;
						Changed = false;
					}
				}
			}
                        if(pSkinList->IndexOf(SkinManager->SkinName) < 0)
                        {
                        	if(pSkinList->Count)
                                	SkinManager->SkinName = pSkinList->Strings[0];
                                else bEnabled = false;
                        }
                        delete pSkinList;

                        if(bEnabled != SkinManager->Active)
                        	SkinManager->Active = bEnabled;

			if(bEnabled && Changed) SkinManager->UpdateSkin();

			if(Config["AeroBorders"] >> b)
			{
				m_bAeroBorders = b && m_bAero;
			}

		}
	}
}

void TInterfaceModule::EnableAnimations()
{
	SkinManager->AnimEffects->Buttons->Events =
		m_bEnableAnimations?(TacBtnEvents() << beMouseEnter << beMouseLeave << beMouseDown << beMouseUp):TacBtnEvents();
	SkinManager->AnimEffects->PageChange->Active = m_bEnableAnimations;

	SkinManager->AnimEffects->DialogShow->Active = m_bEnableAnimations;
	SkinManager->AnimEffects->FormShow->Active = m_bEnableAnimations;
}

void __fastcall TInterfaceModule::DataModuleCreate(TObject *Sender)
{
	m_pSys = SystemModule->m_pSys;
	m_pCore = SystemModule->m_pCore;

	SkinManager->SkinDirectory = GetAppPath()+"Skins";

	m_bAero = m_pSys->GetSystemOptions()->bAero;

	SkinManager->AnimEffects->FormShow->Active = false;
	SkinManager->AnimEffects->PageChange->Active = false;

	LoadConfig();

	m_bInitComplete=true;

	if(!SplashForm)SplashForm = new TSplashForm(Application);
	SplashForm->Show();

}
//---------------------------------------------------------------------------


int TInterfaceModule::GetFleetImage(String name)
{
	ImgIndexes::iterator it = m_fleetsIndexes.find(name);
	if(it != m_fleetsIndexes.end())
		return it->second;
	String FileName= GetAppPath()+"Glyphs\\fleets\\"+name+".png";
	if(!FileExists(FileName))
	{
		m_fleetsIndexes[name]=-1;
		return -1;
	}

	TPNGGraphic* Png = new TPNGGraphic;
	Png->LoadFromFile(FileName);
	HICON Ico = MakeIcon32(Png);
	ImageList_AddIcon((HIMAGELIST)FleetIcons->Handle, Ico);
	DestroyIcon(Ico);
	delete Png;

	int n = FleetIcons->Count-1;
	m_fleetsIndexes[name]=n;
	return n;
}

int TInterfaceModule::GetMenuImage(String name)
{
	ImgIndexes::iterator it = m_menuIndexes.find(name);
	if(it != m_menuIndexes.end())return it->second;
	String FileName= GetAppPath()+"Glyphs\\"+name+".png";
	if(!FileExists(FileName))
	{
		m_menuIndexes[name]=-1;
		return -1;
	}

	TPNGGraphic* Png = new TPNGGraphic;
	Png->LoadFromFile(FileName);
	HICON Ico = MakeIcon32(Png);
	ImageList_AddIcon((HIMAGELIST)MenuImageList->Handle, Ico);
	DestroyIcon(Ico);
	PngImages.push_back(Png);

	int n = MenuImageList->Count-1;
	m_menuIndexes[name]=n;
	return n;
}

void TInterfaceModule::BeginIconUpdate()
{
	m_iIconUpdate++;
}

void TInterfaceModule::EndIconUpdate()
{
	m_iIconUpdate--;
	if(!m_iIconUpdate)
	{
		//if(MenuIconUpdates)
			//MenuImageList->EndUpdate();
		MenuIconUpdates=false;
	}
}
//---------------------------------------------------------------------------


void TInterfaceModule::LocalizeComponent(TComponent* Cnt,IDCMapLocalStrings* Strs)
{
	::LocalizeComponent(Cnt,Strs);
}

void TInterfaceModule::LocalizeContainer(TControl* Base,IDCMapLocalStrings* Strs)
{
	::LocalizeContainer(Base,Strs);
}


void __fastcall TInterfaceModule::HintsShowHint(UnicodeString &HintStr, bool &CanShow,
		  THintInfo &HintInfo, TFrame *&Frame)
{
	if(HelperHints && HintInfo.HintControl && HintInfo.HintControl->Owner)
	{
		HintStr += L"<br>  <br><b>[UI::"+HintInfo.HintControl->Owner->Name+L"]<br>    "+
					HintInfo.HintControl->Name+"</b>";

		Clipboard()->AsText = HintInfo.HintControl->Name;
	}
}
//---------------------------------------------------------------------------

void TInterfaceModule::HookMenu(TPopupMenu* menu)
{
	if(SkinManager->Active)
		SkinManager->SkinableMenus->HookPopupMenu(menu,true);
}

void TInterfaceModule::HookMenu(TMenuItem* menu)
{
	if(SkinManager->Active)
		SkinManager->SkinableMenus->HookItem(menu,true);
}

//---------------------------------------------------------------------------

void __fastcall TInterfaceModule::SkinManagerAfterChange(TObject *Sender)
{
	if(!m_bInitComplete)return;

	DCMapConfigWrapper cfg(GetDCMapCore()->GetConfig());
	cfg.section("Appearance");

	cfg["SkinName"] << SkinManager->SkinName;
	cfg["EnableSkin"] << SkinManager->Active;
	cfg["HueOffset"] << SkinManager->HueOffset;
	cfg["Saturation"] << SkinManager->Saturation;

}
//---------------------------------------------------------------------------

