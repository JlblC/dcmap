//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UISystem.h"
#include "UIFrameForm.h"
#include "UIWorkspace.h"
#include "CoreUIManager.h"
#include "BaseFrameFormModule.h"

TSysUI SysUI;

//---------------------------------------------------------------------------

TSysUI::TSysUI()
{
	m_pActiveFrame = 0;
}

TForm* TSysUI::ActiveFrameForm()
{
	return m_pActiveFrame;
}


void TSysUI::OnFrameActivate(TBaseFrameForm* pFrame)
{
	m_pActiveFrame = pFrame;
}

IDCMapCoreUI* TSysUI::CreateCoreUI(IDCMapCore* pCore)
{
	 TCoreUIManager* CoreUI = new TCoreUIManager;

	 return CoreUI;
}

IDCMapFrameUI* TSysUI::CreateFrameUI(IDCMapFrame* pFrame,IDCMapCore* pCore)
{
	 TFrameUI* FrameUI = new TFrameUI(pFrame,pCore);
	 FrameUI->CreateFrameForm();

	 FrameForms.insert(FrameUI->GetFrameForm());
	 // create framw window
	 return FrameUI;
}

IDCMapWorkspaceUI* TSysUI::CreateWorkspaceUI(IDCMapWorkspace* pWorkspace,IDCMapFrame* pFrame,IDCMapCore* pCore)
{
	return new TUIWorkspace(pWorkspace,pFrame,pCore);;
}



#pragma package(smart_init)
