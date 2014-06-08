//---------------------------------------------------------------------------

#include "vcl.h"
#pragma hdrstop

#include "UIWorkspace.h"
#include "ViewLayout.h"

#include "UIFrameForm.h"
#include "BaseFrameFormModule.h"
#include "TabsFormModule.h"
#include "CoreUIManager.h"

//---------------------------------------------------------------------------

TFrameUI::TFrameUI(IDCMapFrame* pFrame,IDCMapCore* pCore)
{
	m_pCore = pCore;
	m_pFrame = pFrame;
	m_pFrameModule=0;
	m_pFrameForm=0;

	dcmap_cast<IDCMapFrameEx*>(m_pFrame)->SetUI(this);

	m_pCoreUI = dynamic_cast<TCoreUIManager*>(m_pCore->GetUI());
}

TForm* DCMAPAPI TFrameUI::GetForm(){return m_pFrameForm;}


bool DCMAPAPI TFrameUI::CreateFrameForm()
{
	if(!m_pCoreUI)return false;

	if(!m_pFrameModule)	m_pFrameModule = m_pCore->CreateModuleTyped<CBaseFrameFormModule>("PSystem","TabsFrameForm");

	if(!m_pFrameForm)
	{
		m_pFrameForm = m_pFrameModule->GetForm();
		m_pFrameForm->m_pFrame = m_pFrame;
		m_pCoreUI->AddFrameForm(m_pFrameForm);
    }
	m_pFrameModule->RestoreSessions();

	m_pFrameForm->Show();
	return true;
}

bool TFrameUI::OnNewWorkspace(IDCMapWorkspaceUI* pWorkspace)
{
	if(!m_pFrameForm)return false;
	return m_pFrameForm->OnNewWorkspace(pWorkspace);
}

void DCMAPAPI TFrameUI::ShowHint(dcmapRECT const& area,dcmapDCSTR string)
{
	if(!m_pFrameForm)return;
	m_pFrameForm->ShowHint(area,string);
};

void DCMAPAPI TFrameUI::HideHint()
{
	if(!m_pFrameForm)return;
	m_pFrameForm->HideHint();
};


#pragma package(smart_init)
