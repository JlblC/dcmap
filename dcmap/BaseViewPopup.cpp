//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Utils.h"
#include "BaseViewPopup.h"
#include "main.h"
#include "Interface.h"
#include "BaseFormModule.h"
#include "Localize.h"
#include "UISystem.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma link "BaseFormModule"
#pragma resource "*.dfm"
TBaseViewPopupForm *BaseViewPopupForm;
//---------------------------------------------------------------------------
__fastcall TBaseViewPopupForm::TBaseViewPopupForm(TComponent* Owner)
	: Inherited(Owner)
{
	m_pSystem = GetDCMapSys();
	m_pCore=GetDCMapCore();
	Visible=false;
	TForm* ParentForm = dynamic_cast<TForm*>(Owner);
	if(ParentForm)
	{
		PopupParent = ParentForm;
	}
}

__fastcall TBaseViewPopupForm::~TBaseViewPopupForm()
{
	if(m_pWorkspace)
	{
		m_pWorkspace->GetUI()->UnregisterForm(this);
    }
}

bool TBaseViewPopupForm::SetupInstance(IDCMapCore* pCore,IDCMapWorkspace* pWorkspace)
{
	InterfaceModule->LocalizeContainer(this);

	m_pCore = pCore;
	if(pWorkspace)
	{
		pWorkspace->GetUI()->RegisterForm(this);
		m_pWorkspace = pWorkspace;
	}
	return OnInit();
}
//---------------------------------------------------------------------------

void __fastcall TBaseViewPopupForm::OnClose()
{
	if(m_pWorkspace) SaveFormPlacement(this,m_pWorkspace->GetConfig());
	else SaveFormPlacement(this,m_pCore->GetConfig());
}

void __fastcall TBaseViewPopupForm::FormClose(TObject *Sender,
	  TCloseAction &Action)
{
	OnClose();
}
//---------------------------------------------------------------------------


void __fastcall TBaseViewPopupForm::FormShow(TObject *Sender)
{
	if(m_pWorkspace)LoadFormPlacement(this,0,m_pWorkspace->GetConfig());
	else LoadFormPlacement(this,0,m_pCore->GetConfig());
}
//---------------------------------------------------------------------------


void CBaseFormService::Show()
{
	if(!m_pInstance)m_pInstance = CreateForm();

	if(m_pInstance->PopupMode != pmExplicit)
	{
		m_pInstance->PopupMode = pmExplicit;
	}
	m_pInstance->PopupParent = SysUI.ActiveFrame();
	m_pInstance->Show();
}

void CBaseFormService::Hide()
{
	if(!m_pInstance)m_pInstance->Hide();
}

void CBaseFormService::ShowModal()
{
	bool preexists = m_pInstance!=0;
	if(!m_pInstance)m_pInstance = CreateForm();

	m_pInstance->PopupParent = SysUI.ActiveFrame();

	if(m_pInstance->PopupMode != pmNone)
		m_pInstance->PopupMode = pmNone;


	m_pInstance->ShowModal();
	if(!preexists)
	{
		delete m_pInstance;
		m_pInstance=0;
	}
}

CBaseFormService::CBaseFormService()
{
	m_pInstance=0;
}

TBaseViewPopupForm* CBaseFormService::CreateForm(TComponent *Owner)
{
	TBaseViewPopupForm* form = CreateInstance(Owner?Owner:SysUI.ActiveFrame());
	if(form)
	{
		if(!form->SetupInstance(m_pCore,m_pWorkspace))
		{
			delete form;
			return 0;
        }
	}
	return form;
}

bool  DCMAPAPI CBaseFormService::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	m_pCore = core;
	m_pWorkspace = workspace;
	return true;
}

int   DCMAPAPI CBaseFormService::OnMessage( int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender )
{
	if(Msg == DCMM_EXECUTE)
	{
		ShowModal();
		return 1;
	}
	return 0;
}

int DCMAPAPI CBaseFormService::ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
{
	if(type == DCMEXEC_QUERY)
	{
		if(!command || _wcsicmp(command,L"modal")==0 || _wcsicmp(command,L"ShowModal")==0)
		{
			return DCMEXEC_CALL;
		}
		else if(_wcsicmp(command,L"show")==0)
		{
			return DCMEXEC_CALL;
		}
		if(m_pInstance) return m_pInstance->OnScriptExec(type,command,pParams,pResult);
	    return DCMEXEC_FAIL;
	}

	if(type == DCMEXEC_CALL)
	{
		if(!command || _wcsicmp(command,L"modal")==0 || _wcsicmp(command,L"ShowModal")==0)
		{
			ShowModal();
			return DCMEXEC_SUCCESS;
		}
		else if(_wcsicmp(command,L"show")==0)
		{
			Show();
			return DCMEXEC_SUCCESS;
		}
	}

	if(m_pInstance) return m_pInstance->OnScriptExec(type,command,pParams,pResult);

	return DCMEXEC_FAIL;
}
void __fastcall TBaseViewPopupForm::FormCreate(TObject *Sender)
{
	LocalizeContainer(this);
	SkinProvider->DrawNonClientArea =  !InterfaceModule->m_bAeroBorders;
}
//---------------------------------------------------------------------------
