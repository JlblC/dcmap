//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "dcmapsyslib.h"
#include "Preferences.h"
#include "BaseModule.h"
#include "Interface.h"
#include "main.h"

#include "sVCLUtils.hpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma link "sPanel"
#pragma link "sTreeView"
#pragma link "sButton"
#pragma link "BaseViewPopup"
#pragma resource "*.dfm"
TPreferencencesForm *PreferencencesForm=0;

static CAutoReg<CBasicFormService<TPreferencencesForm> > sWrap("PCoreService","PreferencencesForm");


//---------------------------------------------------------------------------
__fastcall TPreferencencesForm::TPreferencencesForm(TComponent* Owner)
	: TBaseViewPopupForm(Owner)
{
	m_pCurrentFrame = 0;
}
__fastcall TPreferencencesForm::~TPreferencencesForm()
{
    for(unsigned i=0;i<m_vecPanels.size();i++)
    {
        GetDCMapCore()->DestroyModule(m_vecPanels[i]);
    }
	m_vecPanels.clear();

}

void __fastcall TPreferencencesForm::FillGroup(AnsiString Name)
{
	g_pLocalStrings->SelectSection("PreferencencesForm");
	String Caption = g_pLocalStrings->GetSectionStringW(Name.c_str());

	AnsiString PluginClassName = "PPrefs_"+Name;
    IDCMapSys* pSys = GetDCMapSys();

    TStringList* List = new TStringList;

    if(pSys->SelectClass(PluginClassName.c_str()))
    {
      TTreeNode* pNode = TreeView->Items->Add(0,Caption);
      do
      {
         List->Add(pSys->GetPluginName());
      }
      while(pSys->Next());

      List->Sort();

      for(int i=0;i<List->Count;i++)
      {
		 IDCMapVCLPanel* pPanel;
		 pPanel =  GetDCMapCore()->CreateModuleTyped<IDCMapVCLPanel>(PluginClassName.c_str(),List->Strings[i].c_str());
         if(!pPanel)continue;
         TFrame* pFrame = pPanel->CreateFrame();
         pFrame->Align = alClient;
         pFrame->Visible = false;
         pFrame->Parent = ClientPanel;
         m_vecPanels.push_back(pPanel);
		 TreeView->Items->AddChildObject(pNode,pPanel->GetCaption(),pFrame);
	 }
     delete List;
    }
}

//---------------------------------------------------------------------------
void __fastcall TPreferencencesForm::FormCreate(TObject *Sender)
{
	Inherited::FormCreate(Sender);

	FillGroup("base");
	FillGroup("colors");
	TreeView->FullExpand();
}
//---------------------------------------------------------------------------
void __fastcall TPreferencencesForm::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
    if(ModalResult == mrOk)
    {
    	Visible = false;
        for(unsigned i=0;i<m_vecPanels.size();i++)
        {
            m_vecPanels[i]->DoOk();
		}
		GetDCMapCore()->OnConfigChange();
	}
	else
	{
		for(unsigned i=0;i<m_vecPanels.size();i++)
		{
			m_vecPanels[i]->DoCansel();
		}
	}
	CanClose = true;
}
//---------------------------------------------------------------------------

void __fastcall TPreferencencesForm::ApplyButtonClick(TObject *Sender)
{
    for(unsigned i=0;i<m_vecPanels.size();i++)
    {
        m_vecPanels[i]->DoApply();
    }
    GetDCMapCore()->OnConfigChange();
}
//---------------------------------------------------------------------------

void __fastcall TPreferencencesForm::FormShow(TObject *Sender)
{
	Inherited::FormShow(Sender);
	//FillGroup("base");
	//FillGroup("colors");
	//TreeView->FullExpand();

	for(unsigned i=0;i<m_vecPanels.size();i++)
    {
        m_vecPanels[i]->OnShow();
    }
    TreeView->Selected = TreeView->Items->Item[0]->Item[0];
    //TreeViewClick(Sender);

	TreeViewChange(Sender,0);
}
//---------------------------------------------------------------------------

void __fastcall TPreferencencesForm::TreeViewChange(TObject *Sender,
      TTreeNode *Node)
{
	TTreeNode* sel = TreeView->Selected;
    if(sel)
	{
		TFrame* pFrame = (TFrame*)sel->Data;
		if(pFrame && pFrame != m_pCurrentFrame)
		{
			if(m_pCurrentFrame)
			{
				pFrame->Top = m_pCurrentFrame->Top;
				pFrame->Left = m_pCurrentFrame->Left;
				pFrame->Width = m_pCurrentFrame->Width;
				pFrame->Height  = m_pCurrentFrame->Height;
			}

			TsSkinManager* pMan =  SkinProvider->SkinData->SkinManager;
			if(m_pCurrentFrame && pMan->AnimEffects->PageChange->Active && pMan->Active)
			{
				PrepareForAnimation(pFrame);
				m_pCurrentFrame->Visible = false;
				pFrame->Visible = true;
				AnimShowControl(pFrame, pMan->AnimEffects->PageChange->Time);
			}
			else
			{
				pFrame->Visible = true;
				if(m_pCurrentFrame)m_pCurrentFrame->Visible = false;
			}
			m_pCurrentFrame = pFrame;
		}
    }
}
//---------------------------------------------------------------------------


