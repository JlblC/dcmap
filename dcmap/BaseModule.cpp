//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BaseModule.h"

#include "sComboBoxes.hpp"
#include "Interface.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sFrameAdapter"
#pragma resource "*.dfm"
TBaseModuleFrame *BaseModuleFrame;

IDCMapSys* g_pSys=0;
IDCMapLocalStrings* g_pLocalStrings=0;
TRegReserver* g_pRegReserver=0;

struct TRegReserverAutoDelete
{
    ~TRegReserverAutoDelete()
    {
        if(g_pRegReserver)delete g_pRegReserver;
        g_pRegReserver=0;
    }
};
static TRegReserverAutoDelete s_AutoeDel;

void TRegReserver::RegisterPlugin(char const* RegClass,char const* RegName,char const* Param,
            dcmapCreateCallback pCreateCallback,void* pUserData)
{
     if(!g_pRegReserver) g_pRegReserver = new TRegReserver;
     Treg reg;
     reg.RegClass = RegClass;
     reg.RegName = RegName;
     reg.pCreateCallback = pCreateCallback;
     reg.pUserData = pUserData;
     reg.Param = Param;
     g_pRegReserver->m_vecRegs.push_back(reg);
};

void TRegReserver::RegisterModules(IDCMapSys* pSys)
{
    for(int i= 0;i< m_vecRegs.size();i++)
    {
        g_pSys->RegisterPlugin(m_vecRegs[i].RegClass,
            m_vecRegs[i].RegName,
            m_vecRegs[i].pCreateCallback,
            m_vecRegs[i].pUserData,
            m_vecRegs[i].Param);
    }
}

void RegisterModules(IDCMapSys* pSys)
{
   g_pSys = pSys;
   if(!g_pRegReserver) g_pRegReserver = new TRegReserver;
   g_pRegReserver->RegisterModules(pSys);
}

//---------------------------------------------------------------------------
__fastcall TBaseModuleFrame::TBaseModuleFrame(TComponent* Owner)
    : TFrame(Owner)
{
	m_pPanelInterface = 0;
	Caption = Name;
	m_bChanged=false;
    OnShowCalled=false;
    ImageIndex = -1;
}
//---------------------------------------------------------------------------
void __fastcall TBaseModuleFrame::SetChanged(bool bChanged)
{
	m_bChanged = bChanged;
}

bool TBaseModuleFrame::OnInit()
{
	return true;
}

void TBaseModuleFrame::OnCreate()
{

}

void TBaseModuleFrame::OnShow()
{

}

void TBaseModuleFrame::OnOk()
{
    OnApply();
}

void TBaseModuleFrame::OnApply()
{
    Changed = false;
}

void TBaseModuleFrame::OnCansel()
{
	Changed = false;
}

void* DCMAPAPI TBaseModuleFrame::GetInterface(char const* szInterface)
{
	return 0;
}

void TBaseModuleFrame::OnSetFocus()
{
    TList* lst = new TList;
    GetTabOrderList(lst);
    if(lst->Count)
    {
        ((TWinControl*)lst->Items[0])->SetFocus();
    }
    delete lst;
}

int TBaseModuleFrame::OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    return 0;
}

int TBaseModuleFrame::OnScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
{
    return DCMEXEC_FAIL;
}


void TBaseModuleFrame::SetNewKeyValue(String val)
{
	if(m_pPanelInterface->OnKeyValueChange)
		m_pPanelInterface->OnKeyValueChange(val,this);
}

void TBaseModuleFrame::ApplyChanges()
{
	if(m_pPanelInterface->OnApplyChanges)
		m_pPanelInterface->OnApplyChanges(this);
	else OnApply();
}

void TBaseModuleFrame::UpdateList()
{
	if(m_pPanelInterface->OnUpdateList)
		m_pPanelInterface->OnUpdateList(this);
}

bool TBaseModuleFrame::OnDataTest()
{
    return true;
}
void TBaseModuleFrame::LoadConfig(DCMapConfigWrapper cfg)
{
}

void TBaseModuleFrame::SaveConfig(DCMapConfigWrapper cfg)
{
}

void __fastcall TBaseModuleFrame::ComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
												TOwnerDrawState State)
{
	TsComboBox* box = dynamic_cast<TsComboBox*>(Control);
	if(State.Contains(odComboBoxEdit))
	{
		//box->Canvas->
	}
	if(!box)return;
    String str = box->Items->Strings[Index];
	box->Canvas->TextRect(Rect,Rect.left+2,Rect.top,str);
}

void  __fastcall TBaseModuleFrame::SetHeight(int h)
{
	if(Inherited::Height == h)return;

	if(m_pPanelInterface && m_pPanelInterface->OnResize)
    {
         m_pPanelInterface->OnResize(Width,h,this);
    }
    else Inherited::Height = h;
}

//----------------------------------------------------------------------------

TBaseModuleFramePanel::TBaseModuleFramePanel()
{
	m_pFrame=0;
	OnKeyValueChange=0;
	OnResize=0;
	OnChange=0;
	OnApplyChanges=0;
	OnUpdateList=0;
	m_pOwner=0;
}

bool DCMAPAPI TBaseModuleFramePanel::InitUI(IDCMapModule* pOwner,int id)
{
	 m_iID = id;
	 Link(pOwner);
	 return true;
}
IDCMapModule* DCMAPAPI TBaseModuleFramePanel::GetFinalModule()
{
	if(m_pOwner)return m_pOwner->GetFinalModule();
	return this;
}

void* DCMAPAPI TBaseModuleFramePanel::CastTo(DCMapInterfaceID idInterface)
{
	DCMAP_MODULE_SUPPORTS(IDCMapUIItem);
	DCMAP_MODULE_SUPPORTS(IDCMapUIFrameItem);
	DCMAP_MODULE_SUPPORTS(IDCMapUIPanel);
	DCMAP_MODULE_SUPPORTS(IDCMapVCLPanel);
	if(m_pFrame)
		return m_pFrame->GetInterface(idInterface);
	return 0;
}

void DCMAPAPI TBaseModuleFramePanel::Destroy()
{
	if(m_pFrame)delete m_pFrame;
	delete this;
}

TFrame* DCMAPAPI TBaseModuleFramePanel::CreateFrame()
{
	if(m_pFrame)	return m_pFrame;

	m_pFrame = DoCreateFrame();
	if(!m_pFrame)return false;
	m_pFrame->m_pPanelInterface = this;
	m_pFrame->m_pCore = m_pCore;
	m_pFrame->m_pWorkspace = m_pWorkspace;
	if(!m_pFrame->OnInit())return 0;
	InterfaceModule->LocalizeContainer(m_pFrame);
	m_pFrame->OnCreate();

	return m_pFrame;
}

void DCMAPAPI TBaseModuleFramePanel::DestroyFrame(TFrame* frame)
{
	if(!m_pFrame)return;
	delete m_pFrame;
    m_pFrame = 0;
}

bool DCMAPAPI TBaseModuleFramePanel::OnCreate(DCMapObjectCreationParams const* params)
{
	m_pSys = params->pDCMapSys;

	ModuleName = params->szName;
	ModuleClass = params->szClass;

	return true;
}

bool DCMAPAPI TBaseModuleFramePanel::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	m_pCore = core;
	m_pWorkspace = workspace;

	if(!CreateFrame())return false;

	return true;
}

int DCMAPAPI TBaseModuleFramePanel::ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
{
	if(!m_pFrame)return DCMEXEC_FAIL;
	return m_pFrame->OnScriptExec(type,command,pParams,pResult);
}

UnicodeString DCMAPAPI TBaseModuleFramePanel::GetCaption()
{
	if(!m_pFrame)return L"";

	return m_pFrame->Caption;
}

void DCMAPAPI TBaseModuleFramePanel::SetSkinSection(String Section)
{
	if(m_pFrame)
	{
		m_pFrame->sFrameAdapter->SkinData->SkinSection = Section;
		m_pFrame->OnChnageSkin();
	}
}

int DCMAPAPI TBaseModuleFramePanel::ImageIndex()
{
	if(!m_pFrame)return -1;
	return m_pFrame->ImageIndex;
}

int DCMAPAPI TBaseModuleFramePanel::OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	if(!m_pFrame) return 0;

	switch(MSG)
	{
	case DCMM_SKIN_CHANGED:
		  m_pFrame->OnChnageSkin();
		  break;
	case DCMM_UI_SET_TEXT:
		  SetText((dcmapWCSTR)lParam);
		  return 0;
		  break;
	case DCMM_UI_GET_TEXT:
		  {
			  static UnicodeString sStr;
			  sStr = m_pFrame->GetText();
			  return (int)(sStr.w_str());
          }
	}
	return m_pFrame->OnMessage(MSG,wParam,lParam,pSender);
}

void DCMAPAPI TBaseModuleFramePanel::OnShow()
{
    if(!m_pFrame)return;
	//m_pFrame->sFrameAdapter->DoHook();
	//ShowWindow(m_pFrame->Handle,SW_SHOW);
    if(!m_pFrame->OnShowCalled)
    {
        m_pFrame->OnShowCalled=true;
        m_pFrame->OnShow();
    }
}

void DCMAPAPI TBaseModuleFramePanel::DoApply()
{
    m_pFrame->OnApply();
}
void DCMAPAPI TBaseModuleFramePanel::DoOk()
{
    m_pFrame->OnOk();
}
void DCMAPAPI TBaseModuleFramePanel::DoCansel()
{
    m_pFrame->OnCansel();
}
void DCMAPAPI TBaseModuleFramePanel::SetFocus()
{
    m_pFrame->OnSetFocus();
}
bool DCMAPAPI TBaseModuleFramePanel::DoTestData()
{
    return m_pFrame->OnDataTest();
}

void DCMAPAPI TBaseModuleFramePanel::LoadConfig(IDCMapConfig* pConfig)
{
    DCMapConfigWrapper cfg(pConfig);
	if(m_pFrame)m_pFrame->LoadConfig(cfg);
}

void DCMAPAPI TBaseModuleFramePanel::SaveConfig(IDCMapConfig* pConfig)
{
    DCMapConfigWrapper cfg(pConfig);
	if(m_pFrame)m_pFrame->SaveConfig(cfg);
}

void DCMAPAPI TBaseModuleFramePanel::SetText(UnicodeString Text)
{
	if(m_pFrame)m_pFrame->SetText(Text);
}

TMainMenu* DCMAPAPI TBaseModuleFramePanel::GetFrameMenu()
{
    if(m_pFrame)
    {
        TComponent* cntr = m_pFrame->FindComponent(L"FrameMenu");
        if(cntr) return dynamic_cast<TMainMenu*>(cntr);
    }
    return 0;
}

void __fastcall TBaseModuleFrame::FrameResize(TObject *Sender)
{
	if(!OnShowCalled && Parent)
    {
    	OnShowCalled=true;
        OnShow();
    }
}
//---------------------------------------------------------------------------

