//-----------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "main.h"
#include "ViewLayout.h"
#include "dcmapsyslib.h"
#include "acntUtils.hpp"
#include "CoreUIManager.h"
#include "Utils.h"
#include "ViewSetup.h"
#include "BaseModule.h"
#include "Interface.h"
#include "sVCLUtils.hpp"
#include "sGraphUtils.hpp"
#include "System.h"
#include "Splash.h"
#include "Localize.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma link "sLabel"
#pragma link "sTabControl"
#pragma link "sFrameBar"
#pragma link "sScrollBox"
#pragma link "sPanel"
#pragma link "sPageControl"
#pragma link "sFrameAdapter"
#pragma link "ViewLayout"
#pragma link "BaseFrameFormModule"
#pragma resource "*.dfm"

TMainForm *MainForm=0;

static CAutoReg<CMainFormService> sWrap("PCoreDaemon","MainForm");

CMainFormService::CMainFormService()
{

}

bool  DCMAPAPI CMainFormService::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{

	return true;
}

int   DCMAPAPI CMainFormService::OnMessage( int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender )
{
	if(Msg == DCMM_CONFIG_CHANGED)
	{
		if(MainForm)MainForm->LoadPrefs();
	}
	return 0;
}

int DCMAPAPI CMainFormService::ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
{
	if(type == DCMEXEC_QUERY) return DCMEXEC_CALL;
	return DCMEXEC_FAIL;
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: Inherited(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::WndProc(TMessage & Message)
{
	if(m_bWatchClipbrdForCmd)
	{
		if(Message.Msg == WM_DRAWCLIPBOARD)
		{
			try
			{
				UnicodeString wstr = Clipboard()->AsText;
				wstr = Trim(wstr);
				if( wstr.Pos(L"!DCMapCmd!{")==1 && wstr[wstr.Length()] ==L'}' )
				{
					wstr = wstr.SubString(11,wstr.Length());
					ClipboardClear();
					OnCommand(wstr);
					return;
				}
			}
			catch(...){}
			SendMessage(hwndNextClipboardViewer,Message.Msg,Message.WParam,Message.LParam);
		}
		else if(Message.Msg == WM_CHANGECBCHAIN)
		{
			 if((HWND)Message.WParam == hwndNextClipboardViewer)
				  hwndNextClipboardViewer = (HWND)Message.LParam;
			 else SendMessage(hwndNextClipboardViewer,Message.Msg,Message.WParam,Message.LParam);
		}
		else TForm::WndProc(Message);
	}
	else TForm::WndProc(Message);
}

void TMainForm::OnCommand(UnicodeString cmd)
{
	SystemModule->BringToTop();
	Show();
	SetForegroundWindow(Handle);
	HWND fg = GetForegroundWindow();
	GetDCMapCore()->GetConsole()->Execute(cmd.w_str());
}

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	m_iFrameCounter=1;
    m_pActiveFrame=0;
    m_bInitComplete=0;
    m_pHiddenTab=0;
	m_bAutoHideTabs=1;
	OperateTabIndex=-1;
	DragTabIndex=-1;
	hwndNextClipboardViewer=0;
	m_bWatchClipbrdForCmd=false;
	m_bAutoCheckUpdates=true;

    return;

	IDCMapSys* pSys = SystemModule->m_pSys;
	IDCMapCoreEx* pCore = SystemModule->m_pCore;

	SystemModule->InitSystem(&g_CoreUIManager);
	LoadPrefs();
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

	RestoreSessions();
}
//---------------------------------------------------------------------------

void TMainForm::CloseTabs()
{
	if(m_pActiveFrame)m_pActiveFrame->Deactivate();
    m_pActiveFrame=0;
    DCMapConfigWrapper  Config(GetDCMapCore()->GetConfig());
    Config.section("Sessions");

    if(m_pHiddenTab)
    {
		Config["Num"] << 1;
		Config["ActiveTab"] << 0;

		Config["Tab0"] << m_pHiddenTab->FrameName;
		Config["Caption0"] << m_pHiddenTab->TabName;

		delete m_pHiddenTab;
		m_pHiddenTab=0;
	}
	else
	{
		Config["Num"] << sTabs->Tabs->Count;
		Config["ActiveTab"] << sTabs->TabIndex;
		int n = sTabs->Tabs->Count;
		for(int i=0;i<n;i++)
		{
			TabDesc* pDesc = (TabDesc*)sTabs->Tabs->Objects[i];
			Config["Tab"+IntToStr(i)] << pDesc->FrameName;
			Config["Caption"+IntToStr(i)] << sTabs->Tabs->Strings[i];
			delete pDesc;
            sTabs->Tabs->Objects[i]=0;
        }
    	sTabs->Tabs->Clear();
    }
}

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	SaveConfig();
	Hide();

	if(!SplashForm)SplashForm = new TSplashForm(Application);
	SplashForm->StatusLabel->Caption = L"Closing...";
	SplashForm->Show();

	CloseTabs();

	if(m_bWatchClipbrdForCmd)
	{
		ChangeClipboardChain(Handle,hwndNextClipboardViewer);
		m_bWatchClipbrdForCmd = false;
	}

	SystemModule->TerminateSystem();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::sTabsChange(TObject *Sender)
{
	ActivateTab(sTabs->TabIndex);
//	m_pActiveFrame->ClientControl->SetFocus();
}
//---------------------------------------------------------------------------

void TMainForm::SaveConfig()
{
    DCMapConfigWrapper  Config(GetDCMapCore()->GetConfig());
	SaveFormPlacement(this);
}

void TMainForm::LoadPrefs()
{
	DCMapConfigWrapper  Config(GetDCMapCore()->GetConfig());
	String Str;
	int b;

	Config.section("Core");
	{
    	b = false;
		Config["WatchClipbrdForCmd"] >> b;

		if(m_bWatchClipbrdForCmd != b)
		{
			if(b)
			{
				hwndNextClipboardViewer = SetClipboardViewer(Handle);
				m_bWatchClipbrdForCmd = true;
			}
			else
			{
				ChangeClipboardChain(Handle,hwndNextClipboardViewer);
				m_bWatchClipbrdForCmd = false;
			}
		}
	}

	Config.section("Appearance");
	{
		if(Config["SkinMenu"] >> b)SkinProvider->MakeSkinMenu = b;
		Config["HideTabs"] >> m_bAutoHideTabs;
		bool bHideMenu = false;

		if(Config["HideMainMenu"] >> bHideMenu)
		if(bHideMenu)
			Menu = 0;
		else
			Menu = MainMenu;

		UnicodeString MainFormCaption = LSTRW(DefaultFormTitle);
		Config["MainFormCaption"] >> MainFormCaption;
		Caption = MainFormCaption;

		if(m_pHiddenTab && !m_bAutoHideTabs)
		{
			UpdateAutoHideTabs(true);
		}
		else UpdateAutoHideTabs();

	}

}

void TMainForm::LoadConfig()
{
	m_bWndPlace = LoadFormPlacement(this,&m_WndPlace);
}


void __fastcall TMainForm::FormShow(TObject *Sender)
{
	if(m_bWndPlace)SetWindowPlacement(Handle,&m_WndPlace);

	if(SplashForm)
	{
		SplashForm->Close();
		delete SplashForm;
		SplashForm=0;
	}

	InterfaceModule->EnableAnimations();
}
//---------------------------------------------------------------------------


void TMainForm::RestoreSessions()
{
    DCMapConfigWrapper cfg(GetDCMapCore()->GetConfig());
    cfg.section("Sessions");
    int num=0;
    cfg["Num"] >> num;

    for(int i=0;i<num;i++)
    {
        TabDesc* pDesc = new TabDesc;
        UnicodeString FrameCaption;
        cfg["Tab"+IntToStr(i)] >> pDesc->FrameName;
        cfg["Caption"+IntToStr(i)] >> FrameCaption;
        sTabs->Tabs->AddObject(FrameCaption,pDesc);
    }
    m_bInitComplete=true;

    int Index = -1;
    cfg["ActiveTab"] >> Index;
	ActivateTab(Index);

    sTabs->OnChange = sTabsChange;

    UpdateAutoHideTabs();
}

TViewLayoutFrame* TMainForm::RestoreSession(int Index)
{
	TabDesc* pDesc;
	if(m_pHiddenTab) pDesc = m_pHiddenTab;
	else pDesc = (TabDesc*)sTabs->Tabs->Objects[Index];

	pDesc->pFrame = AddSession(pDesc->FrameName,"",false);
	return pDesc->pFrame;
}

TViewLayoutFrame* TMainForm::AddSession(String FrameName,UnicodeString Caption,bool NewSession, String Template)
{
	TViewLayoutFrame* pFrame = 0;
	try
	{
		pFrame = new TViewLayoutFrame(sTabs);
		pFrame->Align = alClient;
		pFrame->Name = FrameName;
		pFrame->Visible = false;
		pFrame->Parent = sTabs;
		pFrame->Width = sTabs->ClientWidth;
		pFrame->Height = sTabs->ClientHeight;

		pFrame->Create(this,FrameName,NewSession,Template);
		if(NewSession)
		{
			TabDesc* pDesc = new TabDesc;
			pDesc->FrameName = FrameName;
			pDesc->pFrame = pFrame;
			if(!m_pHiddenTab && sTabs->Tabs->Count == 0 && m_bAutoHideTabs)
			{
				m_pHiddenTab = pDesc;
			}
			else sTabs->Tabs->AddObject(Caption,pDesc);
		}
	}
	catch(...)
	{
		if(pFrame)delete pFrame;
		return 0;
	}
	return pFrame;
}

void __fastcall TMainForm::sTabsMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
	OperateTabIndex = sTabs->IndexOfTabAt(X,Y);
	if(Button == mbLeft)
	{
		Mouse->Capture = sTabs->Handle;
		DragTabIndex = OperateTabIndex;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sTabsMouseUp(TObject *Sender, TMouseButton Button,
	  TShiftState Shift, int X, int Y)
{
	int Index = sTabs->IndexOfTabAt(X,Y);
	if(OperateTabIndex >=0 && OperateTabIndex == Index)
	{
		if(Button == mbMiddle)
		{
			if(Index >= 0)CloseTab(Index);
		}
		else if(Button == mbRight)
		{
			if(Index >= 0)
			{
				TabMenu->Popup(Mouse->CursorPos.x,Mouse->CursorPos.y);
			}
		}
	}
	if(Button == mbLeft)
	{
		DragTabIndex = -1;
		Mouse->Capture = 0;
	}
}
//---------------------------------------------------------------------------
void TMainForm::CloseTab(int Index)
{
	if(m_pHiddenTab)
    {
        delete m_pHiddenTab;
        m_pHiddenTab=0;
        m_pActiveFrame=0;
        return;
    }
	if(sTabs->Tabs->Count == 0)return;
    if(Index<0)return;

	TabDesc* pDesc = (TabDesc*)sTabs->Tabs->Objects[Index];
	TViewLayoutFrame* pPreFrame = m_pActiveFrame;

	//if(sSkinManager->Active)
	{
		if(Index == sTabs->TabIndex)
		{
			int NewIndex = sTabs->TabIndex;
			sTabs->Tabs->Delete(Index);
			if(NewIndex >= sTabs->Tabs->Count)NewIndex--;
			ActivateTab(NewIndex);
		}
		else sTabs->Tabs->Delete(Index);
	}
	/*else if(Index == sTabs->TabIndex)
	{
		int NewIndex = sTabs->TabIndex+1;
		if(NewIndex >= sTabs->Tabs->Count)NewIndex = sTabs->TabIndex-1;
		ActivateTab(NewIndex);
		sTabs->Tabs->Delete(Index);
	}
	else sTabs->Tabs->Delete(Index);  */

	if(m_pActiveFrame == pDesc->pFrame)m_pActiveFrame=0;
	delete pDesc;

	UpdateAutoHideTabs();
}

void TMainForm::UpdateAutoHideTabs(bool Restore)
{
    if(sTabs->Tabs->Count == 1 && !Restore && m_bAutoHideTabs)
    {
    	m_pHiddenTab = (TabDesc*)sTabs->Tabs->Objects[0];
        m_pHiddenTab->TabName = sTabs->Tabs->Strings[0];
     	sTabs->Tabs->Clear();
		sTabs->SkinData->SkinSection = "CHECKBOX";//TABCONTROL
		sTabs->TabIndex = -1;
		OperateTabIndex=-1;
    }
	else if((Restore || sTabs->Tabs->Count>1) && m_pHiddenTab)
    {
		sTabs->Tabs->InsertObject(0,m_pHiddenTab->TabName,m_pHiddenTab);
		sTabs->SkinData->SkinSection = "PAGECONTROL";//TABCONTROL
		m_pHiddenTab=0;
		sTabs->TabIndex = 0;
    }
}

void TMainForm::ActivateTab(int Index)
{
	if(m_pHiddenTab)
    {
        m_pActiveFrame = m_pHiddenTab->pFrame;
        m_pActiveFrame->Visible=true;
        m_pActiveFrame->Activate();
        return;
    }

    if(Index<0)return;
	if(Index >= sTabs->Tabs->Count)return;


	TabDesc* pDesc = (TabDesc*)sTabs->Tabs->Objects[Index];
    sTabs->TabIndex = Index;

    if(m_pActiveFrame != pDesc->pFrame || !pDesc->pFrame)
	{
		TsSkinManager* pMan =  SkinProvider->SkinData->SkinManager;
		bool Animate = pMan->AnimEffects->PageChange->Active && pMan->Active;

		TViewLayoutFrame* PreFrame = m_pActiveFrame;
		if(PreFrame)
		{
			PreFrame->Deactivate();
			//PreFrame->Visible=false;
		}
        if(!pDesc->pFrame)
        {
			pDesc->pFrame = RestoreSession(Index);
			if(!pDesc->pFrame)return;

		}
		if(PreFrame)
		{
			pDesc->pFrame->Top = PreFrame->Top;
			pDesc->pFrame->Left = PreFrame->Left;
			pDesc->pFrame->Width = PreFrame->Width;
			pDesc->pFrame->Height  = PreFrame->Height;
		}
		else
		{
			m_pActiveFrame = pDesc->pFrame;
			m_pActiveFrame->Visible = true;
		}
		m_pActiveFrame = pDesc->pFrame;

		m_pActiveFrame->Activate();

		if(Animate && 0)
		{
			PrepareForAnimation(m_pActiveFrame);

			if(PreFrame)PreFrame->Visible=false;
			m_pActiveFrame->Visible = true;
			m_pActiveFrame->m_pWorkspace->BroadcastMessage(DCMM_WORKSPACE_SHOW,0,0);
			AnimShowControl(m_pActiveFrame, pMan->AnimEffects->PageChange->Time);
		}
		else
		{
			m_pActiveFrame->Visible = true;
			if(PreFrame)PreFrame->Visible=false;
		}
		//m_pActiveFrame->ClientControl->Invalidate();
		//m_pActiveFrame->ClientControl->Refresh();

	}
}

void __fastcall TMainForm::CloseTabMenuItemClick(TObject *Sender)
{
	CloseTab(sTabs->TabIndex);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CntCloseMenuItemClick(TObject *Sender)
{
	if(Sender == CntCloseMenuItem)
		CloseTab(OperateTabIndex);
	else
		CloseTab(sTabs->TabIndex);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CntRenameMenuItemClick(TObject *Sender)
{
	int Index = OperateTabIndex;
	if(Sender != CntSaveTemplateMenuItem)
	{
		Index = sTabs->TabIndex;
		if(Index >= sTabs->Tabs->Count)Index=sTabs->Tabs->Count-1;
	}

	if(Index>=0)
	{
		sTabs->Tabs->Strings[Index] =
			sInputBox(L"Перкименоване вкладки",
					L"Введите новый заголовок вкладки",
					sTabs->Tabs->Strings[Index]);
	}
	else if(m_pHiddenTab)
	{
		m_pHiddenTab->TabName =
			sInputBox(L"Перкименоване вкладки",
				L"Введите новый заголовок вкладки",
				m_pHiddenTab->TabName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CreateTabMenuClick(TObject *Sender)
{
	TMenuItem* pMenu = (TMenuItem*)Sender;
	String TemplatePath = GetAppPath()+"Templates\\*.ini";

    ClearMenuItems(pMenu,1);

    TSearchRec F;
    if(!FindFirst(TemplatePath,0,F))
    {
        do
        {
        	if(F.Name == "default.ini")continue;
			TMenuItem* pSubItem = new TMenuItem(pMenu);
			UnicodeString Name =  F.Name;
            Name = Name.SubString(1,Name.Length()-ExtractFileExt(F.Name).Length());
			pSubItem->Caption = Name;
			pSubItem->Hint = Name;
            pSubItem->OnClick = TemplateCreateClick;
            pMenu->Add(pSubItem);
        }
        while(!FindNext(F));
    }
    FindClose(F);
}
//---------------------------------------------------------------------------

void TMainForm::CreateTab(UnicodeString DefTabName,String FrameName,String Template)
{
	UpdateAutoHideTabs(true);

	// Locate sutable tab name
	String TabName = DefTabName;
	int sz = sTabs->Tabs->Count;

	for(int n=1;n<100;n++)
	{
		int i=0;
		for(i=0;i<sz;i++)
		{
			if(SameText(sTabs->Tabs->Strings[i],TabName))
			{
				TVarRec args[] = {DefTabName.w_str(),n};
				TabName = Format(L"%s(%d)",args, 1);
				break;
			}
		}
		if(i == sTabs->Tabs->Count)break;
	}

    int i=0;
    while(true)
    {
        bool Found=false;
        String Name = FrameName+IntToStr(i);
    	for(int Index=0;Index<sTabs->Tabs->Count;Index++)
        {
            TabDesc* pDesc = (TabDesc*)sTabs->Tabs->Objects[Index];
            if(pDesc->FrameName == Name)Found = true;
        }
        if(!Found)break;
        i++;
    }
    FrameName = FrameName+IntToStr(i);
    AddSession(FrameName,TabName,true,Template);
    ActivateTab(sTabs->Tabs->Count-1);
}

void __fastcall TMainForm::NewTabMenuItemClick(TObject *Sender)
{
    CreateTab(LSTRW(DefaultTabName),"Session","");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TemplateCreateClick(TObject *Sender)
{
	TMenuItem* pMenu = (TMenuItem*)Sender;
    CreateTab(pMenu->Hint,"Session",pMenu->Hint);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CntSaveTemplateMenuItemClick(TObject *Sender)
{
	int Index = OperateTabIndex;
	if(Sender != CntSaveTemplateMenuItem)
	{
		Index = sTabs->TabIndex;
		if(Index >= sTabs->Tabs->Count)Index=sTabs->Tabs->Count-1;
	}

	TabDesc* desc;
	String TemplateName;

	if(Index >=0)
	{
		desc= (TabDesc*)sTabs->Tabs->Objects[Index];
		TemplateName = sTabs->Tabs->Strings[Index];
	}
	else if (m_pHiddenTab)
	{
		desc= m_pHiddenTab;
		TemplateName = m_pHiddenTab->TabName;
	}
	else return;

	if(!desc->pFrame)
    {
		ActivateTab(Index);
    }

    if(sInputQuery("Создание шаблона",
    		     "Введите название нового шаблона",
                 TemplateName))
    {
     	desc->pFrame->SaveAsTemplate(TemplateName);

    }
}
//---------------------------------------------------------------------------
void TMainForm::RecreateTabs()
{
	CloseTabs();
    RestoreSessions();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormDeactivate(TObject *Sender)
{
   InterfaceModule->Hints->HideHint();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::SetupTabMenuItemClick(TObject *Sender)
{
    if(!m_pActiveFrame)return;
	ViewSetupForm = new TViewSetupForm(Application);

    int Index = sTabs->TabIndex;
    TabDesc* pDesc;
    if(m_pHiddenTab) pDesc = m_pHiddenTab;
    else pDesc = (TabDesc*)sTabs->Tabs->Objects[Index];

    if(!pDesc->pFrame)return;
	pDesc->pFrame->SaveConfig();
	ViewSetupForm->m_pWorkspace = pDesc->pFrame->m_pWorkspace;
	if(ViewSetupForm->ShowModal() == mrOk)
    {
		ViewSetupForm->Save();
		pDesc->pFrame->UpdatedLayout = true;
		pDesc->pFrame->Destroy();
		delete pDesc->pFrame;
		pDesc->pFrame=0;
		m_pActiveFrame = 0;

		RestoreSession(Index);
		ActivateTab(Index);
    }
    delete ViewSetupForm;
    ViewSetupForm=0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DrawMenuItem(TObject *Sender, TCanvas *ACanvas, TRect &ARect, TOwnerDrawState State)
{
	TMenuItem* Item = (TMenuItem*)Sender;
    String str = Item->Caption;
    //ACanvas->TextRect(ARect,ARect.left+2,ARect.top,str);
}
//---------------------------------------------------------------------------

class TTabDrag : public TDragObjectEx
{
public:
	String Caption;
	TObject* Object;
	int Index;
	int FromIndex;
};

void __fastcall TMainForm::sTabsStartDrag(TObject *Sender,
	  TDragObject *&DragObject)
{
	TTabDrag* Drag = new TTabDrag;
	DragObject  = Drag;
	Drag->FromIndex = OperateTabIndex;
	Drag->Caption = sTabs->Tabs->Strings[OperateTabIndex];
	Drag->Object = sTabs->Tabs->Objects[OperateTabIndex];
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sTabsMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
	if(DragTabIndex < 0)return;

	int Index = sTabs->IndexOfTabAt(X,Y);
	THitTests hit = sTabs->GetHitTestInfoAt(X,Y);
	//if(Index < 0 && hit == htOnRight) Index = sTabs->Tabs->Count;

	if(Index >= 0 && DragTabIndex != Index)
	{
		String Str = sTabs->Tabs->Strings[DragTabIndex];
		TObject* Obj = sTabs->Tabs->Objects[DragTabIndex];

		sTabs->Tabs->Exchange(Index,DragTabIndex);

		int RelocIndex = sTabs->IndexOfTabAt(X,Y);
		if(RelocIndex != Index)
		{
			sTabs->Tabs->Exchange(Index,DragTabIndex);
		}
		else
		{
			sTabs->TabIndex = Index;
			DragTabIndex = Index;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ExitMenuItemClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SkinManagerGetMenuExtraLineData(TMenuItem *FirstItem, UnicodeString &SkinSection,
		  UnicodeString &Caption, Graphics::TBitmap *&Glyph, bool &LineVisible)

{
	Caption = FirstItem->Caption;
	LineVisible = false;
}
//----------------------------------------------------------------------
void __fastcall TMainForm::FormHide(TObject *Sender)
{
	SaveFormPlacement(this);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::est1Click(TObject *Sender)
{
	TTabsFrameForm* pTabs = new TTabsFrameForm(Application);
	pTabs->RestoreSessions();
	pTabs->Show();
}
//---------------------------------------------------------------------------

