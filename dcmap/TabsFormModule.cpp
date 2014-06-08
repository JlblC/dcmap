//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "acntUtils.hpp"
#include "TabsFormModule.h"
#include "UIWorkspace.h"
#include "Utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseFrameFormModule"
#pragma link "sSkinProvider"
#pragma link "sTabControl"
#pragma link "sPanel"
#pragma resource "*.dfm"
TTabsFrameForm *TabsFrameForm;

static CAutoReg<CTabsFormModule> sWrap("PSystem","TabsFrameForm");

void* DCMAPAPI CTabsFormModule::CastTo(DCMapInterfaceID idInterface)
{
	DCMAP_MODULE_SUPPORTS(CBaseFrameFormModule);
	return 0;
}

void DCMAPAPI CTabsFormModule::RestoreSessions()
{
	GetForm()->RestoreSessions();
}

//---------------------------------------------------------------------------
__fastcall TTabsFrameForm::TTabsFrameForm(TComponent* Owner)
	: TBaseFrameForm(Owner)
{
	m_iFrameCounter=1;
	m_pActiveFrame=0;
	m_pHiddenTab=0;
	m_bAutoHideTabs=1;
	OperateTabIndex=-1;
	DragTabIndex=-1;
    RestoreMode=false;
    RestoreUI=0;
}
//---------------------------------------------------------------------------

bool TTabsFrameForm::OnInit()
{
	if(!Inherited::OnInit())return false;


	return true;
}



void TTabsFrameForm::CreateTab(UnicodeString DefTabName,String FrameName,String Template)
{
	UpdateAutoHideTabs(true);

	// Locate sutable tab name
	String TabName = DefTabName;
	int sz = FrameTabs->Tabs->Count;

	for(int n=1;n<100;n++)
	{
		int i=0;
		for(i=0;i<sz;i++)
		{
			if(SameText(FrameTabs->Tabs->Strings[i],TabName))
			{
				TVarRec args[] = {DefTabName.w_str(),n};
				TabName = Format(L"%s(%d)",args, 1);
				break;
			}
		}
		if(i == FrameTabs->Tabs->Count)break;
	}

    int i=0;
    while(true)
    {
        bool Found=false;
        String Name = FrameName+IntToStr(i);
    	for(int Index=0;Index<FrameTabs->Tabs->Count;Index++)
        {
            TabDesc* pDesc = (TabDesc*)FrameTabs->Tabs->Objects[Index];
            if(pDesc->FrameName == Name)Found = true;
        }
        if(!Found)break;
        i++;
    }
    FrameName = FrameName+IntToStr(i);
    AddSession(FrameName,TabName,true,Template);
    ActivateTab(FrameTabs->Tabs->Count-1);
}

void TTabsFrameForm::RestoreSessions()
{
    DCMapConfigWrapper cfg(GetDCMapCore()->GetConfig());
    cfg.section("Sessions");
    int num=0;
    cfg["Num"] >> num;

    FrameTabs->OnChange = 0;

    for(int i=0;i<num;i++)
    {
        TabDesc* pDesc = new TabDesc;
        UnicodeString FrameCaption;
        cfg["Tab"+IntToStr(i)] >> pDesc->FrameName;
        cfg["Caption"+IntToStr(i)] >> FrameCaption;
        FrameTabs->Tabs->AddObject(FrameCaption,pDesc);
    }
    //m_bInitComplete=true;

    int Index = -1;
    cfg["ActiveTab"] >> Index;
	ActivateTab(Index);

    FrameTabs->OnChange = FrameTabsChange;

    UpdateAutoHideTabs();
}

TViewLayoutFrame* TTabsFrameForm::RestoreSession(int Index)
{
	TabDesc* pDesc;
	if(m_pHiddenTab) pDesc = m_pHiddenTab;
	else pDesc = (TabDesc*)FrameTabs->Tabs->Objects[Index];

	pDesc->pFrame = AddSession(pDesc->FrameName,"",false);
	return pDesc->pFrame;
}

bool TTabsFrameForm::OnNewWorkspace(IDCMapWorkspaceUI* pWorkspace)
{
	if(!Inherited::OnNewWorkspace(pWorkspace))return false;
	TUIWorkspace* pWUI = dynamic_cast<TUIWorkspace*>(pWorkspace);
	if(!pWUI)return false;
	TViewLayoutFrame* pLayout = pWUI->GetLayout();
	if(!pLayout)return false;

	if(RestoreMode)
	{
		 RestoreUI = pWUI;
	}
	else
	{
		RestoreUI = 0;
	}
	return true;
}

TViewLayoutFrame* TTabsFrameForm::AddSession(String FrameName,UnicodeString Caption,bool NewSession, String Template)
{
	TViewLayoutFrame* pFrame = 0;
	try
	{
    	RestoreMode = true;
		IDCMapWorkspaceEx* pWorkspace = GetDCMapSys()->CreateWorkspace(m_pFrame,
										m_pCore,FrameName.w_str(),Template.w_str());

		pFrame = RestoreUI->GetLayout();

		pFrame->Align = alClient;
		pFrame->Name = FrameName;
		pFrame->Visible = false;
		pFrame->Parent = FrameTabs;
		pFrame->Width = FrameTabs->ClientWidth;
		pFrame->Height = FrameTabs->ClientHeight;

		if(NewSession)
		{
			TabDesc* pDesc = new TabDesc;
			pDesc->FrameName = FrameName;
			pDesc->pFrame = pFrame;
			if(!m_pHiddenTab && FrameTabs->Tabs->Count == 0 && m_bAutoHideTabs)
			{
				m_pHiddenTab = pDesc;
			}
			else FrameTabs->Tabs->AddObject(Caption,pDesc);
		}
	}
	catch(...)
	{
		if(pFrame)delete pFrame;
		pFrame = 0;
	}
	RestoreMode = false;
	return pFrame;
}
//---------------------------------------------------------------------------
void TTabsFrameForm::CloseTab(int Index)
{
	if(m_pHiddenTab)
    {
        delete m_pHiddenTab;
        m_pHiddenTab=0;
        m_pActiveFrame=0;
        return;
    }
	if(FrameTabs->Tabs->Count == 0)return;
    if(Index<0)return;

	TabDesc* pDesc = (TabDesc*)FrameTabs->Tabs->Objects[Index];
	TViewLayoutFrame* pPreFrame = m_pActiveFrame;

	//if(sSkinManager->Active)
	{
		if(Index == FrameTabs->TabIndex)
		{
			int NewIndex = FrameTabs->TabIndex;
			FrameTabs->Tabs->Delete(Index);
			if(NewIndex >= FrameTabs->Tabs->Count)NewIndex--;
			ActivateTab(NewIndex);
		}
		else FrameTabs->Tabs->Delete(Index);
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

void TTabsFrameForm::UpdateAutoHideTabs(bool Restore)
{
    if(FrameTabs->Tabs->Count == 1 && !Restore && m_bAutoHideTabs)
    {
    	m_pHiddenTab = (TabDesc*)FrameTabs->Tabs->Objects[0];
        m_pHiddenTab->TabName = FrameTabs->Tabs->Strings[0];
     	FrameTabs->Tabs->Clear();
		FrameTabs->SkinData->SkinSection = "CHECKBOX";//TABCONTROL
		FrameTabs->TabIndex = -1;
		OperateTabIndex=-1;
    }
	else if((Restore || FrameTabs->Tabs->Count>1) && m_pHiddenTab)
    {
		FrameTabs->Tabs->InsertObject(0,m_pHiddenTab->TabName,m_pHiddenTab);
		FrameTabs->SkinData->SkinSection = "PAGECONTROL";//TABCONTROL
		m_pHiddenTab=0;
		FrameTabs->TabIndex = 0;
    }
}

void TTabsFrameForm::ActivateTab(int Index)
{
	if(m_pHiddenTab)
    {
        m_pActiveFrame = m_pHiddenTab->pFrame;
        m_pActiveFrame->Visible=true;
        m_pActiveFrame->Activate();
        return;
    }

    if(Index<0)return;
	if(Index >= FrameTabs->Tabs->Count)return;


	TabDesc* pDesc = (TabDesc*)FrameTabs->Tabs->Objects[Index];
    FrameTabs->TabIndex = Index;

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
			//PrepareForAnimation(m_pActiveFrame);

			if(PreFrame)PreFrame->Visible=false;
			m_pActiveFrame->Visible = true;
			m_pActiveFrame->m_pWorkspace->BroadcastMessage(DCMM_WORKSPACE_SHOW,0,0);
			//AnimShowControl(m_pActiveFrame, pMan->AnimEffects->PageChange->Time);
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

void TTabsFrameForm::CloseTabs()
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
		Config["Num"] << FrameTabs->Tabs->Count;
		Config["ActiveTab"] << FrameTabs->TabIndex;
		int n = FrameTabs->Tabs->Count;
		for(int i=0;i<n;i++)
		{
			TabDesc* pDesc = (TabDesc*)FrameTabs->Tabs->Objects[i];
			Config["Tab"+IntToStr(i)] << pDesc->FrameName;
			Config["Caption"+IntToStr(i)] << FrameTabs->Tabs->Strings[i];
			delete pDesc;
			FrameTabs->Tabs->Objects[i]=0;
        }
    	FrameTabs->Tabs->Clear();
    }
}

void __fastcall TTabsFrameForm::FrameTabsChange(TObject *Sender)
{
	ActivateTab(FrameTabs->TabIndex);
}
//---------------------------------------------------------------------------

void __fastcall TTabsFrameForm::FrameTabsMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	OperateTabIndex = FrameTabs->IndexOfTabAt(X,Y);
	if(Button == mbLeft)
	{
		Mouse->Capture = FrameTabs->Handle;
		DragTabIndex = OperateTabIndex;
	}
}
//---------------------------------------------------------------------------

void __fastcall TTabsFrameForm::FrameTabsMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	int Index = FrameTabs->IndexOfTabAt(X,Y);
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

void __fastcall TTabsFrameForm::FrameTabsMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
	if(DragTabIndex < 0)return;

	int Index = FrameTabs->IndexOfTabAt(X,Y);
	THitTests hit = FrameTabs->GetHitTestInfoAt(X,Y);
	//if(Index < 0 && hit == htOnRight) Index = sTabs->Tabs->Count;

	if(Index >= 0 && DragTabIndex != Index)
	{
		String Str = FrameTabs->Tabs->Strings[DragTabIndex];
		TObject* Obj = FrameTabs->Tabs->Objects[DragTabIndex];

		FrameTabs->Tabs->Exchange(Index,DragTabIndex);

		int RelocIndex = FrameTabs->IndexOfTabAt(X,Y);
		if(RelocIndex != Index)
		{
			FrameTabs->Tabs->Exchange(Index,DragTabIndex);
		}
		else
		{
			FrameTabs->TabIndex = Index;
			DragTabIndex = Index;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TTabsFrameForm::CntCloseMenuItemClick(TObject *Sender)
{
	if(Sender == CntCloseMenuItem)
		CloseTab(OperateTabIndex);
	else
		CloseTab(FrameTabs->TabIndex);
}
//---------------------------------------------------------------------------

void __fastcall TTabsFrameForm::CntRenameMenuItemClick(TObject *Sender)
{
	int Index = OperateTabIndex;
	if(Sender != CntSaveTemplateMenuItem)
	{
		Index = FrameTabs->TabIndex;
		if(Index >= FrameTabs->Tabs->Count)Index=FrameTabs->Tabs->Count-1;
	}
	if(Index>=0)
	{
		DCMapVarValue Val;
		DCMapVarValue Res;
		Val[L"value"] = FrameTabs->Tabs->Strings[Index];
		if(m_pCore->GetCoreUI()->InputDialog("RenameFrameTab",Val,Res))
		{
			FrameTabs->Tabs->Strings[Index] = Res[L"value"].Str();
		}
	}
	else if(m_pHiddenTab)
	{
		DCMapVarValue Val;
		DCMapVarValue Res;
		Val[L"value"] = m_pHiddenTab->TabName;
		if(m_pCore->GetCoreUI()->InputDialog("RenameFrameTab",Val,Res))
		{
			m_pHiddenTab->TabName = Res[L"value"].Str();
		}
	}
/*
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
*/
}
//---------------------------------------------------------------------------

void __fastcall TTabsFrameForm::CntSaveTemplateMenuItemClick(TObject *Sender)
{
	int Index = OperateTabIndex;
	if(Sender != CntSaveTemplateMenuItem)
	{
		Index = FrameTabs->TabIndex;
		if(Index >= FrameTabs->Tabs->Count)Index=FrameTabs->Tabs->Count-1;
	}

	TabDesc* desc;
	String TemplateName;

	if(Index >=0)
	{
		desc= (TabDesc*)FrameTabs->Tabs->Objects[Index];
		TemplateName = FrameTabs->Tabs->Strings[Index];
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

	DCMapVarValue Val;
	DCMapVarValue Res;
	Val[L"value"] = TemplateName;
	if(m_pCore->GetCoreUI()->InputDialog("TabTemplateCreation",Val,Res))
	{
		desc->pFrame->SaveAsTemplate(Res[L"value"].Str());
	}
}
//---------------------------------------------------------------------------

void __fastcall TTabsFrameForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	Hide();
	CloseTabs();
	Inherited::FormClose(Sender,Action);
}
//---------------------------------------------------------------------------


void __fastcall TTabsFrameForm::CreateTabClick(TObject *Sender)
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
            pSubItem->OnClick = AddTabMenuClick;
            pMenu->Add(pSubItem);
        }
        while(!FindNext(F));
    }
    FindClose(F);

}
//---------------------------------------------------------------------------

void __fastcall TTabsFrameForm::AddTabMenuClick(TObject *Sender)
{
	TMenuItem* pMenu = (TMenuItem*)Sender;
    CreateTab(pMenu->Hint,"Session",pMenu->Hint);
}
//---------------------------------------------------------------------------

