//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
 #include <windows.h>

#include "BaseFrameFormModule.h"
#include "Localize.h"
#include "Utils.h"
#include "FakeMain.h"
#include "UIWorkspace.h"
#include "Interface.h"
#include "UISystem.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma link "sPanel"
#pragma resource "*.dfm"
TBaseFrameForm *BaseFrameForm;

bool  DCMAPAPI CBaseFrameFormModule::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	m_pCore = core;
	m_pWorkspace = workspace;
	m_pSystem = g_pDCMapSys;

	m_pForm = CreateForm();
	if(!m_pForm)return false;

	m_pForm->m_pCore = m_pCore;
	m_pForm->m_pWorkspace = m_pWorkspace;
	m_pForm->m_pSystem = m_pSystem;

	m_pForm->OnInit();

	return true;
}

void DCMAPAPI CBaseFrameFormModule::SetFrameConfig(dcmapWCSTR Config)
{

}

void DCMAPAPI CBaseFrameFormModule::RestoreSessions()
{

}
//---------------------------------------------------------------------------
__fastcall TBaseFrameForm::TBaseFrameForm(TComponent* Owner)
	: Inherited(Owner)
{
	m_bHint = false;
}

void __fastcall TBaseFrameForm::CreateParams(TCreateParams &Params)
{
	 Inherited::CreateParams(Params);
	 Params.ExStyle |= WS_EX_APPWINDOW;
	 //Params.WndParent = GetDesktopWindow();
}

bool TBaseFrameForm::OnInit()
{
	LoadConfig();

	if(m_bWndPlace)
	{
		if(m_WndPlace.showCmd == 3)
		{
			Left = m_WndPlace.ptMaxPosition.x;
			Top = m_WndPlace.ptMaxPosition.y;
			Width = Screen->WorkAreaWidth;
			Height = Screen->WorkAreaHeight;
		}
		else
		{
			Left = m_WndPlace.rcNormalPosition.left;
			Top = m_WndPlace.rcNormalPosition.top;
			Width = m_WndPlace.rcNormalPosition.right-m_WndPlace.rcNormalPosition.left;
			Height = m_WndPlace.rcNormalPosition.bottom-m_WndPlace.rcNormalPosition.top;
		}
	}
	LocalizeContainer(this);

    return true;
}

void TBaseFrameForm::LoadConfig()
{
	m_bWndPlace = LoadFormPlacement(this,&m_WndPlace);
}

//---------------------------------------------------------------------------
void __fastcall TBaseFrameForm::FormActivate(TObject *Sender)
{
	SysUI.OnFrameActivate(this);
}
//---------------------------------------------------------------------------

void __fastcall TBaseFrameForm::FormShow(TObject *Sender)
{
	if(m_bWndPlace)SetWindowPlacement(Handle,&m_WndPlace);
}
//---------------------------------------------------------------------------

void __fastcall TBaseFrameForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    SaveFormPlacement(this);
	FakeMainForm->OnFrameClose(this);
	Action = caFree;
}
//---------------------------------------------------------------------------
bool TBaseFrameForm::OnNewWorkspace(IDCMapWorkspaceUI* pWorkspace)
{
	TUIWorkspace* pWUI = dynamic_cast<TUIWorkspace*>(pWorkspace);
	if(!pWUI)return false;
	pWUI->FrameForm = this;

	return true;
}

static LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	g_CoreUIManager.HideHint();

    return CallNextHookEx(0,nCode,wParam,lParam);
}

static LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
    switch(wParam)
    {
    	case WM_MOUSEMOVE:

            break;
        default:
			g_CoreUIManager.HideHint();
            break;
    }
    return CallNextHookEx(0,nCode,wParam,lParam);
}

static HHOOK s_hHook1=0;
static HHOOK s_hHook2=0;

static bool m_bHint=false;

void TBaseFrameForm::HideHint()
{
	if(m_bHint)
	{
	   InterfaceModule->Hints->HideHint();

		//MainForm->HintManager->HideHint();
		m_bHint = false;

		UnhookWindowsHookEx(s_hHook1);
		UnhookWindowsHookEx(s_hHook2);
	}
}

void TBaseFrameForm::ShowHint(RECT const& area,dcmapWCSTR string)
{
	HideHint();
	if(!string || !string[0])return;

    TPoint pt1 = Point(area.left,area.top);
    TPoint pt2 = Point(area.right,area.bottom);

	::ScreenToClient(Handle,&pt1);
	::ScreenToClient(Handle,&pt2);

	HintPanel->Left = pt1.x;
	HintPanel->Top = pt1.y;

	HintPanel->Width = pt2.x-pt1.x;
	HintPanel->Height = pt2.y-pt1.y;

	HintPanel->Top = pt1.y+=HintPanel->Height/2+12;

	HintPanel->Hint = string;

	InterfaceModule->Hints->HideHint();
	InterfaceModule->Hints->ShowHint(HintPanel,string);

    m_bHint = true;

    s_hHook1 = SetWindowsHook(WH_KEYBOARD,(HOOKPROC)KeyboardProc);
    s_hHook2 = SetWindowsHook(WH_MOUSE,(HOOKPROC)MouseProc);
}

