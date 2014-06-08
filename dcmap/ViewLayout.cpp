//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ViewLayout.h"
#include "dcmapsyslib.h"
#include <acntUtils.hpp>
#include "Interface.h"

#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	//

//#include "Main.h"
#include "BaseModule.h"
#include "BaseFrameFormModule.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sFrameAdapter"
#pragma link "sFrameBar"
#pragma link "sScrollBox"
#pragma link "sButton"
#pragma link "sBitBtn"
#pragma link "sStatusBar"
#pragma link "sPanel"
#pragma link "sSplitter"
#pragma link "acCoolBar"
#pragma resource "*.dfm"
TViewLayoutFrame *ViewLayoutFrame;
//---------------------------------------------------------------------------
__fastcall TViewLayoutFrame::TViewLayoutFrame(TComponent* Owner)
    : TFrame(Owner)
{
    m_pClientView=0;
    m_pWorkspace=0;
	m_bInitComplete=false;
	m_bInitFailed=false;
	m_pClientViewPanel=0;
	UpdatedLayout=false;
	ClientControl=0;
}
//---------------------------------------------------------------------------

class __declspec(delphiclass) TWindowHolderControl : public TCustomControl
{
	IDCMapView* m_pView;
	TBaseFrameForm* FrameForm;

        void __fastcall  WndProc  (TMessage & Message)
	{
		switch (Message.Msg)
		{
		case WM_CHAR:
		case WM_KEYUP:
		case WM_KEYDOWN:
			break;
		default:
			TWinControl::WndProc (Message);
		}
	}

        void __fastcall CreateWnd(void)
        {
            TCustomControl::CreateWnd();

            RECT rc = {0,0,Width,Height};
            m_pView->Create(WindowHandle,FrameForm->Handle,&rc);
        }

public:
	TWindowHolderControl(Classes::TComponent* AOwner,IDCMapView* pView,TBaseFrameForm* _FrameForm):TCustomControl(AOwner),
		m_pView(pView),FrameForm(_FrameForm)
	{
			//TabStop = true;
			//ParentBackground = false;
			//BorderWidth =0;
			//FDoubleBuffered=false;
	}
};



class __declspec(delphiclass) TModuleWrapperControl : public TWinControl
{
	IDCMapView* m_pView;
	TBaseFrameForm* FrameForm;

	WNDCLASSW BaseClass;

	void __fastcall CreateParams (TCreateParams & Params)
	{
		TWinControl::CreateParams(Params);
		WNDCLASSW const* pClass = m_pView->RegisterWndClass();
		if(!pClass)return;

		BaseClass = *pClass;

		String ustr(BaseClass.lpszClassName);

		CreateSubClass(Params,ustr.w_str());

		Params.WindowClass = BaseClass;
		Params.Param = m_pView->BeforeCreate(Params.WndParent,FrameForm->Handle);

	}
	void __fastcall CreateWindowHandle(const TCreateParams & Params)
	{
                RECT rc = {Params.X,Params.Y,
                     Params.X+Params.Width,Params.Y+Params.Height};

		TWinControl::CreateWindowHandle(Params);
		m_pView->AfterCreate(WindowHandle,Params.WndParent,FrameForm->Handle);
	}
	void BaseWndProc(TMessage & Message)               
	{
    	if(BaseClass.lpfnWndProc)
			Message.Result = BaseClass.lpfnWndProc(Handle,Message.Msg,Message.WParam,Message.LParam);
	}

	DYNAMIC void __fastcall DragOver(System::TObject* Source, int X, int Y, TDragState State, bool &Accept)
	{
		TDCMapDragObject* Obj = dynamic_cast<TDCMapDragObject*>(Source);
		if(Obj/* && (State == dsDragMove || State == dsDragEnter)*/)
		{
			if(m_pView->DragOver(X,Y,State,Obj->Interface(),Obj->Module()))
			{
				//TWinControl::DragOver(Source,X,Y,State,Accept);
				Accept=true;
				return;
			}
		}
		TWinControl::DragOver(Source,X,Y,State,Accept);
	}
	DYNAMIC void __fastcall DragDrop(System::TObject* Source, int X, int Y)
	{
		TDCMapDragObject* Obj = dynamic_cast<TDCMapDragObject*>(Source);
		if(Obj)
		{
			if(m_pView->DragDrop(X,Y,Obj->Interface(),Obj->Module()))
			{
				return;
			}
		}
		TWinControl::DragDrop(Source,X,Y);
	}
	bool mFocused;
	void __fastcall  WndProc  (TMessage & Message)
	{
	//TWinControl::WndProc (Message);
	//return;
		switch (Message.Msg)
		{
		//case CM_INVALIDATE:
		//	break;
		/*case CM_WANTSPECIALKEY:
			if(Message.WParamLo == VK_UP)Message.Result = 1;
			if(Message.WParamLo == VK_DOWN)Message.Result = 1;
			if(Message.WParamLo == VK_LEFT)Message.Result = 1;
			if(Message.WParamLo == VK_RIGHT)Message.Result = 1;
			break;*/
		case WM_MOUSEWHEEL:
		case WM_ERASEBKGND:
//		case WM_PAINT:
//		case WM_NCPAINT:
			BaseWndProc(Message);
			break;
			//BaseWndProc(Message);
			//break;
		default:
			TWinControl::WndProc (Message);
		}
	}

public:
	TModuleWrapperControl(Classes::TComponent* AOwner,IDCMapView* pView,TBaseFrameForm* _FrameForm):TWinControl(AOwner),
		m_pView(pView),FrameForm(_FrameForm)
	{
			TabStop = true;
			ParentBackground = false;
			BorderWidth =0;
			mFocused=false;
			FDoubleBuffered=false;
			BaseClass.lpfnWndProc = 0;

			//ControlStyle << csOpaque;
	}
};

void TViewLayoutFrame::InitView()
{
	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
    cfg.section("Layout");
    AnsiString View;
	cfg["ViewModule"] >> View;

	IDCMapSys* pSys = GetDCMapSys();

	IDCMapModule* pObj = 0;

	pObj = m_pWorkspace->CreateModule("PView",View.c_str());
	if(!pObj)return;

    m_pClientViewPanel = pObj->Cast<IDCMapVCLPanel>();
	if(m_pClientViewPanel)
    {
		//ClientPanel->Visible = false;
		TFrame* pFrame = m_pClientViewPanel->CreateFrame();
		pFrame->Align = alClient;
		pFrame->Parent = this;
		m_pClientViewPanel->OnShow();
		ClientControl = pFrame;
	}
	else
	{
		m_pClientView = pObj->Cast<IDCMapView>();
		if(m_pClientView)
		{
                	if(1)
                        {
							TModuleWrapperControl* pView = new TModuleWrapperControl(this,m_pClientView,FrameForm);
							pView->Align = alClient;
							pView->Parent = this;
							ClientControl = pView;
						}
						else
						{
							TWindowHolderControl* pView = new TWindowHolderControl(this,m_pClientView,FrameForm);
                            pView->Align = alClient;
                            pView->Parent = this;
                            ClientControl = pView;
                        }
		}
	}
}

void TViewLayoutFrame::SaveConfig()
{
	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
    cfg.section("Layout");

	for(int i=0;i<RightBar->Items->Count;i++)
    {
        bool open = RightBar->Items->Items[i]->State == Sframebar::stOpened;
        cfg["RightBarOpenItem"+IntToStr(i)] << open;
    }
	cfg["RightBarShow"] << RightBar->Visible;
	cfg["RightBarWidth"] << RightBar->Width;

	for(int i=0;i<LeftBar->Items->Count;i++)
	{
		bool open = LeftBar->Items->Items[i]->State == Sframebar::stOpened;
		cfg["LeftBarOpenItem"+IntToStr(i)] << open;
	}
	cfg["LeftBarShow"] << LeftBar->Visible;
	cfg["LeftBarWidth"] << LeftBar->Width;

	SavePanels("RightPanels",m_vecRightPanels);
	SavePanels("LeftPanels",m_vecLeftPanels);
	SavePanels("TopPanels",m_vecTopPanels);
	SavePanels("BottomPanels",m_vecBottomPanels);

	SaveToolBar();

	m_pWorkspace->SaveConfig();
}

void __fastcall TViewLayoutFrame::FrameResize(int Width,int Height,TObject* Sender)
{
    //////////////////
    TCustomFrame* Frame = (TCustomFrame*)Sender;

    String Path = m_mapSideBarFrames[Frame]->GetNamePath();
    int delim = Path.Pos(".");
    String FrameName = Path.SubString(1,delim-1);
    TComponent* cmp = FindComponent(FrameName);
    if(!cmp)return;
    TsFrameBar* bar = dynamic_cast<TsFrameBar*>(cmp);
    if(!bar)return;
	bar->ChangeSize(m_mapSideBarFrames[Frame]->Index,0,Height);
}

void TViewLayoutFrame::InitPanels(String Name,TAlign PanelAlign,VecPanels& panels)
{
	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
	int w,h;

	cfg.section("Layout");
	int num=0;
	cfg[Name+"Num"] >> num;

	if(num)num=1; // limit to one panel

	for(int i=0;i<num;i++)
	{
		String PName;
		cfg.section("Layout");
		cfg[Name+"Mod"+IntToStr(i)] >> PName;

		IDCMapUIPanel*  UIPanel =  m_pWorkspace->CreateModuleTyped<IDCMapUIPanel>("PFormPanelItem",PName.c_str());
		IDCMapVCLPanel* Panel =  UIPanel->Cast<IDCMapVCLPanel>();
		if(!Panel)continue;
		TFrame* pFrame = Panel->CreateFrame();
		if(!pFrame)continue;
		pFrame->Align = PanelAlign;

		cfg.section("Layout");

		if(cfg[Name+"Width"+IntToStr(i)] >> w)
			pFrame->Width = w;
		if(cfg[Name+"Height"+IntToStr(i)] >> h)
			pFrame->Height = h;

		pFrame->Parent = this;

		TsSplitter* pSplitter = new TsSplitter(this);
		pSplitter->Width=5;
		pSplitter->Height=5;
		pSplitter->MinSize=50;

		if(PanelAlign == alLeft)pSplitter->Left = Width;
		else if(PanelAlign == alTop)pSplitter->Top = Height;

		pSplitter->Align = PanelAlign;
		pSplitter->Parent = this;

		panels.push_back(Panel);
	}
}

void TViewLayoutFrame::SavePanels(String Name,VecPanels& panels)
{
	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
	cfg.section("Layout");

	int num = panels.size();
	for(int i=0;i<num;i++)
    {
		TFrame* pFrame = panels[i]->CreateFrame();

		cfg[Name+"Width"+IntToStr(i)] << pFrame->Width;
		cfg[Name+"Height"+IntToStr(i)] << pFrame->Height;
	}
}

void TViewLayoutFrame::InitBar(TsFrameBar* Bar)
{
    String Name = Bar->Name;

    DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
    cfg.section("Layout");
    int num=0;
    cfg[Name+"Num"] >> num;

    if(!num)
    {
    	return;
    }

    bool Show=true;
    cfg[Name+"Show"] >> Show;
	if(Show)Bar->Visible = true;

	int BarWidth = Bar->Width;
	cfg[Name+"Width"] >> BarWidth;
	Bar->Width = BarWidth;

	if(Name[1] == 'R')
	{
		RightClickField->Left=0;
		RightClickField->Visible = true;
	}
	else
	{
		LeftClickField->Left=Width;
		LeftClickField->Visible = true;
	}

	for(int i=0;i<num;i++)
	{
		String PName;

		cfg.section("Layout");
		cfg[Name+"Mod"+IntToStr(i)] >> PName;

		IDCMapUIPanel*  UIPanel =  m_pWorkspace->CreateModuleTyped<IDCMapUIPanel>("PFrameBar",PName.c_str());
		IDCMapVCLPanel* Panel =  UIPanel->Cast<IDCMapVCLPanel>();
		if(!Panel)continue;
		Panel->SetSkinSection("BARPANEL");

		Panel->OnResize = FrameResize;

		 // Add new panel to panels
		TsTitleItem* Item = (TsTitleItem*)Bar->Items->Add();
		Item->Caption = Panel->GetCaption();
		Item->ImageIndex = Panel->ImageIndex();
		Item->OnCreateFrame = sFrameBarItemsCreateFrame;
		Item->OnFrameDestroy = sFrameBarItemsFrameDestroy;

		m_mapRightBarPanels[Item] = Panel;

		Item->Visible = true;
	}
	cfg.section("Layout");
	for(int l=0;l<num;l++)
	{
		bool Open=false;
		cfg[Name+"OpenItem"+IntToStr(l)] >> Open;
        if(Open && Bar->Items->Count > l)Bar->OpenItem(l,0);
    }
}

/*
void TViewLayoutFrame::CreateToolBar()
{
	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
    cfg.section("Layout");
    int num=0;
    cfg["ToolbarNum"] >> num;

    if(!num){ToolBarPanel->Visible = false;return;}

    ToolBarPanel->Visible = true;

    int w=0;

    for(int i=0;i<num;i++)
    {
        String PName;
        cfg.section("Layout");
        cfg["ToolbarMod"+IntToStr(i)] >> PName;
        IDCMapVCLPanel* Panel =  m_pWorkspace->CreateModuleTyped<IDCMapVCLPanel>("PToolbarItem",PName.c_str());
        if(!Panel)continue;

        TFrame* pFrame = Panel->CreateFrame();
        pFrame->Left = w;
        w+=pFrame->Width;
        pFrame->Align = alLeft;
        pFrame->Parent = ToolBarPanel;

        Panel->OnShow();
    }
}
*/
void TViewLayoutFrame::CreateToolBar()
{
	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
    cfg.section("Layout");
    int num=0;
    cfg["ToolbarNum"] >> num;
    if(!num){TopBar->Visible = false;return;}
    TopBar->Visible = true;
	int w=0;

	bool ok=true;

	for(int i=0;i<num;i++)
	{
		String PName;
		cfg.section("Layout");
		cfg["ToolbarMod"+IntToStr(i)] >> PName;

		IDCMapUIPanel*  UIPanel =  m_pWorkspace->CreateModuleTyped<IDCMapUIPanel>("PToolbarItem",PName.c_str());
		IDCMapVCLPanel* Panel =  UIPanel->Cast<IDCMapVCLPanel>();
		if(!Panel)
		{
			ok=false;
			continue;
		}
		TFrame* pFrame = Panel->CreateFrame();

		TCoolBand* pBand = TopBar->Bands->Add();
		//pBand->FixedSize = true;
		pBand->HorizontalOnly = true;
        pBand->MinWidth = pFrame->Width;
        pBand->Text = Panel->GetCaption();
        pBand->ImageIndex = Panel->ImageIndex();

		pBand->Control = pFrame;

		Panel->OnShow();
        m_mapTopBarPanels[pBand] = PanelInfo(Panel,PName);
	}

	if(ok)
	{
		cfg.section("Layout");
		for(int i=0;i<num;i++)
		{
			TCoolBand* pBand = TopBar->Bands->Items[i];
			pBand->FixedSize = false;
			int iWidth = 0;
			bool bBreak = false;
			cfg["ToolbarWidth"+IntToStr(i)] >> iWidth;
			cfg["ToolbarBreak"+IntToStr(i)] >> bBreak;
			pBand->Break = bBreak;
			pBand->Width = iWidth;
		}
	}

}
void TViewLayoutFrame::SaveToolBar()
{
	if(UpdatedLayout)return;

	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
    cfg.section("Layout");
    int num = TopBar->Bands->Count;
    cfg["ToolbarNum"] << num;
    for(int i=0;i<num;i++)
    {
        TCoolBand* pBand = TopBar->Bands->Items[i];
        String PName = m_mapTopBarPanels[pBand].ModuleName;
        IDCMapVCLPanel* Panel = m_mapTopBarPanels[pBand].Panel;
        cfg["ToolbarMod"+IntToStr(i)] << PName;
        cfg["ToolbarWidth"+IntToStr(i)] << pBand->Control->Width;
        cfg["ToolbarBreak"+IntToStr(i)] << pBand->Break;
    }
}

void TViewLayoutFrame::DoCreate()
{
	CreateToolBar();
	
	InitPanels("RightPanels",alRight,m_vecRightPanels);
	InitPanels("LeftPanels",alLeft,m_vecLeftPanels);
	InitPanels("TopPanels",alTop,m_vecTopPanels);
	InitPanels("BottomPanels",alBottom,m_vecBottomPanels);

	if(!m_vecLeftPanels.size())InitBar(LeftBar);
	if(!m_vecRightPanels.size())InitBar(RightBar);
}

void TViewLayoutFrame::SaveAsTemplate(String Template)
{
    String TemplateName = "Templates\\"+Template+".ini";
	m_pWorkspace->SetConfigFile(TemplateName.w_str());
    SaveConfig();
	m_pWorkspace->SetConfigFile(ConfigName.w_str());
}


void TViewLayoutFrame::Create(TBaseFrameForm* _FrameForm,UnicodeString TabName,bool NewSession,String Template)
{
	FrameForm = _FrameForm;

	//GetDCMapSys()->CreateWorkspace()
	//m_pWorkspace = CreateDCMapWorkspace();
	//m_pUIWorkspace = new TUIWorkspace;
	//m_pUIWorkspace->m_pWorkspace = m_pWorkspace;
    //m_pUIWorkspace->m_pLayout = this;

	//m_pWorkspace->SetUI(m_pUIWorkspace);

	if(Template.IsEmpty())Template = "default";

	//ConfigName = "Sessions\\"+Name+".ini";
	//String TemplateName = "Templates\\"+Template+".ini";

    if(NewSession)
	{
		//m_pWorkspace->SetConfigFile(TemplateName.w_str());
		//m_pWorkspace->ConfigureFromFile();
		//m_pWorkspace->SetConfigFile(ConfigName.w_str());
	}
	else
    {
		//m_pWorkspace->SetConfigFile(ConfigName.w_str());
        //m_pWorkspace->ConfigureFromFile();
    }
}


void TViewLayoutFrame::Destroy()
{
	m_pWorkspace->OnDeactivate();
	SaveConfig();
    delete m_pUIWorkspace;
	DestroyDCMapWorkspace(m_pWorkspace);
}

void __fastcall TViewLayoutFrame::ClientPanelResize(TObject *Sender)
{
	//RECT rc = ClientPanel->ClientRect;
	//if(m_pClientView)m_pClientView->OnResize(&rc);
}
//---------------------------------------------------------------------------

void __fastcall TViewLayoutFrame::sFrameBarItemsCreateFrame(
      TObject *Sender, TCustomFrame *&Frame)
{
	TsTitleItem* Item = (TsTitleItem*)Sender;
	// Create frame from plugin
	Frame = m_mapRightBarPanels[Item]->CreateFrame();
    Frame->Visible=true;
    m_mapSideBarFrames[Frame] = Item;
	//m_mapRightBarPanels[Item]->OnShow();
}
//---------------------------------------------------------------------------

void __fastcall TViewLayoutFrame::sFrameBarItemsFrameDestroy(
      TObject *Sender, TCustomFrame *&Frame, bool &CanDestroy)
{
	/*if(!InterfaceModule->SkinManager->Active)
    {
        Frame->Visible = false;
        Frame=0;
    }
    else
    {
		CanDestroy=false;
	}  */
	CanDestroy=false;
}
void MergeFrameMenu(TMainMenu* src,TMainMenu* dst,int GroupInd)
{
	int cnt = src->Items->Count;
	for(int i=0;i<cnt;i++)
	{
		TMenuItem* item = src->Items->Items[i];
        item->GroupIndex = GroupInd;
		src->Items->Remove(item);

		int pos = 0;
		int mct = dst->Items->Count;
		for(;pos<mct;pos++)
		{
			if(dst->Items->Items[pos]->GroupIndex > item->GroupIndex)break;
		}
		dst->Items->Insert(pos,item);
		i--;cnt--;
	}
}

void UnmergeFrameMenu(TMainMenu* src,TMainMenu* dst,int GroupInd)
{
	int cnt = dst->Items->Count;
	for(int i=0;i<cnt;i++)
	{
		if(dst->Items->Items[i]->GroupIndex == GroupInd)
		{
			TMenuItem* item = dst->Items->Items[i];
			dst->Items->Remove(item);
			src->Items->Add(item);
			i--;cnt--;
		}
	}
}

void TViewLayoutFrame::MergeMenu()
{
    if(m_pClientViewPanel)
    {
        TMainMenu* pMenu = m_pClientViewPanel->GetFrameMenu();
        if(pMenu)MergeFrameMenu(pMenu,FrameForm->FrameMenu,20);
    }
    MergeFrameMenu(MainMenu,FrameForm->FrameMenu,40);
	FrameForm->SkinProvider->RepaintMenu();
}

void TViewLayoutFrame::UnmergeMenu()
{
    if(m_pClientViewPanel)
    {
        TMainMenu* pMenu = m_pClientViewPanel->GetFrameMenu();
        if(pMenu)UnmergeFrameMenu(pMenu,FrameForm->FrameMenu,20);
    }
    UnmergeFrameMenu(MainMenu,FrameForm->FrameMenu,40);
}
//---------------------------------------------------------------------------
void TViewLayoutFrame::Activate()
{
	if(m_bInitFailed)return;

	if(!m_bInitComplete)
	{
		try
		{
			DoCreate();
			InitView();
			m_pWorkspace->PostInit();
		}
		catch(...)
		{
			m_bInitFailed = true;
			return;
        }
	}
	m_pWorkspace->OnActivate();
	m_pUIWorkspace->OnActivate();
	MergeMenu();

	if(!m_bInitComplete)
	{
		//Visible=true;
		m_bInitComplete=true;
	}
}
void TViewLayoutFrame::Deactivate()
{
	m_pWorkspace->OnDeactivate();
	m_pUIWorkspace->OnDeactivate();
	UnmergeMenu();
}

bool TViewLayoutFrame::MouseWheel(TShiftState Shift, int WheelDelta, TPoint &MousePos)
{
	if(m_pClientView)
    {
		//TPoint pt = ClientPanel->ScreenToClient(MousePos);
		//return m_pClientView->OnMouseWheel(WheelDelta,pt.x,pt.y);
    }
    return false;
}


void __fastcall TViewLayoutFrame::RightClickFieldClick(TObject *Sender)
{
	RightBar->Visible=true;
	RightClickField->Left = Width-(RightBar->Width+10);
}
//---------------------------------------------------------------------------

void __fastcall TViewLayoutFrame::LeftClickFieldClick(TObject *Sender)
{
	LeftBar->Visible=true;
	RightClickField->Left = LeftBar->Width+10;
}
//---------------------------------------------------------------------------

void __fastcall TViewLayoutFrame::TopBarResize(TObject *Sender)
{
	int i=0;
}
//---------------------------------------------------------------------------

void __fastcall TViewLayoutFrame::LeftClickFieldCanResize(TObject *Sender,
	  int &NewSize, bool &Accept)
{
	Accept = true;
	if(NewSize <= 60)
	{
		NewSize=0;
		LeftBar->Visible = false;
	}
	else if(NewSize < LeftBar->Constraints->MinWidth)
	{
		   NewSize = LeftBar->Constraints->MinWidth;
		   Accept = false;
	}
	else if(NewSize > LeftBar->Constraints->MaxWidth)
	{
		   NewSize = LeftBar->Constraints->MaxWidth;
		   Accept = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TViewLayoutFrame::RightClickFieldCanResize(TObject *Sender,
      int &NewSize, bool &Accept)
{
	Accept = true;
	if(NewSize <= 60)
	{
		NewSize=0;
		RightBar->Visible = false;
	}
	else if(NewSize < RightBar->Constraints->MinWidth)
	{
		   NewSize = RightBar->Constraints->MinWidth;
		   Accept = false;
	}
	else if(NewSize > RightBar->Constraints->MaxWidth)
	{
		   NewSize = RightBar->Constraints->MaxWidth;
		   Accept = false;
	}
}
//---------------------------------------------------------------------------


