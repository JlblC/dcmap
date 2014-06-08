//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PropPage.h"
#include "Interface.h"
#include "sVCLUtils.hpp"
#include "Utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sFrameAdapter"
#pragma link "sPageControl"
#pragma link "sTabControl"
#pragma link "sPanel"
#pragma resource "*.dfm"
TPropPageFrame *PropPageFrame;
//---------------------------------------------------------------------------
__fastcall TPropPageFrame::TPropPageFrame(TComponent* Owner)
    : TFrame(Owner)
{
    WidthUpdate = 0;
    HeightUpdate = 0;

	m_pWorkspace=0;

	m_bRedused = false;
	m_bSingle=false;
}

__fastcall TPropPageFrame::~TPropPageFrame()
{
    for(unsigned i=0;i<m_vecPanels.size();i++)
    {
        if(m_pWorkspace)
            m_pWorkspace->DestroyModule(m_vecPanels[i]);
        else
            GetDCMapCore()->DestroyModule(m_vecPanels[i]);
    }
    m_vecPanels.clear();
}
//---------------------------------------------------------------------------
void TPropPageFrame::OnCreate()
{
    IDCMapSys* pSys = GetDCMapSys();

	m_iBaseWidth = ClientPanel->Width;
	m_iBaseHeight = ClientPanel->Height;

    if(pSys->SelectClass(WideToString(PluginClassName).c_str()))
    {
      do
      {
        IDCMapModule* pObj;
        if(m_pWorkspace)
			pObj =  m_pWorkspace->CreateModule(WideToString(PluginClassName).c_str(),pSys->GetPluginName());
		else
			pObj =  GetDCMapCore()->CreateModule(WideToString(PluginClassName).c_str(),pSys->GetPluginName());
		if(pObj)
		{
            IDCMapVCLPanel* Panel = (IDCMapVCLPanel*)pObj->CastTo("IDCMapVCLPanel");
            if(!Panel)
			{
                if(m_pWorkspace)
                    m_pWorkspace->DestroyModule(pObj);
                else
                    GetDCMapCore()->DestroyModule(pObj);
            }
            else
			{
              m_vecPanels.push_back(Panel);
			  String Name = Panel->GetCaption();
			  TFrame* pFrame = Panel->CreateFrame();
			  m_iBaseWidth = m_iBaseWidth<pFrame->Width?pFrame->Width:m_iBaseWidth;
			  m_iBaseHeight = m_iBaseHeight<pFrame->Height?pFrame->Height:m_iBaseHeight;
			  //pFrame->Align = alNone;
			  Tabs->Tabs->AddObject(Name,pFrame);
              pFrame->Visible = false;
              pFrame->Parent = ClientPanel;
            }
		}
      }while(pSys->Next());
      if(Tabs->Tabs->Count >0)
      {
        Tabs->TabIndex=0;
		m_pCurrentFrame = (TFrame*)Tabs->Tabs->Objects[Tabs->TabIndex];
		ShowCurrentFrame(m_pCurrentFrame);

        if(Tabs->Tabs->Count == 1)
        {
		  Tabs->Tabs->Delete(0);
		  Tabs->SkinData->SkinSection = "CHECKBOX";
		  Tabs->Style = tsFlatButtons;
		  m_bSingle=true;
		}
		else
		{
		  m_bSingle=false;
		}
      }
	  WidthUpdate = m_iBaseWidth-ClientPanel->ClientWidth;
	  HeightUpdate = m_iBaseHeight-ClientPanel->ClientHeight;
	}
	m_bRedused=false;
}

void TPropPageFrame::ShowCurrentFrame(TFrame* Frame)
{
	Frame->Left = (m_iBaseWidth - Frame->Width)/2;
	Frame->Visible = true;
}


void TPropPageFrame::Init(IDCMapWorkspaceEx* pWorkspace)
{
	m_pWorkspace = pWorkspace;
}

void TPropPageFrame::OnShow()
{
    for(unsigned i=0;i<m_vecPanels.size();i++)
    {
		m_vecPanels[i]->OnShow();
		//TFrame* Frame = ((TFrame*)Tabs->Tabs->Objects[i]);
		//if(Frame->Visible)Frame->Repaint();
	}
}

void TPropPageFrame::SelectRef(DCMapDataReference const& ref)
{
	//m_iBaseWidth = ClientPanel->Width;
    //m_iBaseHeight = ClientPanel->Height;

	int HiddenIndex=0;
	int TabIndex=Tabs->TabIndex;
	int tpos=0;
	for(unsigned i=0;i<m_vecPanels.size();i++)
	{
		int r = m_vecPanels[i]->OnMessage(DCMM_SELECTION_DATA_CHANGED,ref.id,ref.age,0);

		TFrame* pFrame = m_vecPanels[i]->CreateFrame();
		int ind = Tabs->Tabs->IndexOfObject(pFrame);

		if(r < 0) // Hide frame
		{
			if(ind < 0) // stay hidden
			{
			}
			else // hide tab
			{
			   Tabs->Tabs->Delete(ind);
			   if(TabIndex == ind)TabIndex = 0;
			   TabIndex--;
			   pFrame->Visible = false;
			   if(m_pCurrentFrame == pFrame)m_pCurrentFrame=0;
			}
		}
		else
		{
			m_iBaseWidth = m_iBaseWidth<pFrame->Width?pFrame->Width:m_iBaseWidth;
			m_iBaseHeight = m_iBaseHeight<pFrame->Height?pFrame->Height:m_iBaseHeight;

			if(ind < 0) // restore tab
			{
				if(tpos == 0 && m_bSingle) // keep hidden frame
				{
					if(m_pCurrentFrame != pFrame)m_pCurrentFrame->Visible = false;
					m_pCurrentFrame = pFrame;
					m_pCurrentFrame->Visible = true;
					HiddenIndex = i;
				}
				else
				{
					if(m_bSingle)
					{
						Tabs->Tabs->InsertObject(0,m_vecPanels[HiddenIndex]->GetCaption(),m_pCurrentFrame);
					}
					m_bSingle=false;
					Tabs->Tabs->InsertObject(tpos,m_vecPanels[i]->GetCaption(),pFrame);
					if(TabIndex >= tpos)TabIndex++;
				}
			}
			tpos++;
		}
	}
	if(Tabs->Tabs->Count > 1)
	{
		Tabs->SkinData->SkinSection = "PAGECONTROL";
		Tabs->Style = tsTabs;
		m_bSingle = false;
	}

	if(m_bSingle)
	{
		//m_pCurrentFrame = (TFrame*)Tabs->Tabs->Objects[Tabs->TabIndex];
		//m_pCurrentFrame->Visible = true;
	}
	else
	{
		if(TabIndex<0)TabIndex = 0;
		Tabs->TabIndex = TabIndex;
	}

	if(!m_bSingle && Tabs->Tabs->Count == 1)
	{
		Tabs->Tabs->Delete(0);
		Tabs->SkinData->SkinSection = "CHECKBOX";
		Tabs->Style = tsFlatButtons;
		m_bSingle = true;
	}

	WidthUpdate = m_iBaseWidth-ClientPanel->ClientWidth;
	HeightUpdate = m_iBaseHeight-ClientPanel->ClientHeight;

}

void TPropPageFrame::SendMessage(int Msg,DWORD wParam,DWORD lParam)
{
    for(unsigned i=0;i<m_vecPanels.size();i++)
    {
        m_vecPanels[i]->OnMessage(Msg,wParam,lParam,0);
    }
}

void TPropPageFrame::DoApply()
{
	GetDCMapCore()->LockDataUpdate();
    for(unsigned i=0;i<m_vecPanels.size();i++)
    {
        m_vecPanels[i]->DoApply();
    }
    GetDCMapCore()->UnlockDataUpdate();
}

void TPropPageFrame::DoOk()
{
	GetDCMapCore()->LockDataUpdate();
    for(unsigned i=0;i<m_vecPanels.size();i++)
    {
        m_vecPanels[i]->DoOk();
    }
    GetDCMapCore()->UnlockDataUpdate();
}

void TPropPageFrame::DoCansel()
{
    for(unsigned i=0;i<m_vecPanels.size();i++)
    {
        m_vecPanels[i]->DoCansel();
    }
}

void TPropPageFrame::SetOnKeyValueChange(TKeyValueChangeChangeEvent evt)
{
	for(unsigned i=0;i<m_vecPanels.size();i++)
	{
		m_vecPanels[i]->OnKeyValueChange = evt;
	}
}

void TPropPageFrame::SetOnChange(TNotifyEvent evt)
{
	for(unsigned i=0;i<m_vecPanels.size();i++)
	{
		m_vecPanels[i]->OnChange = evt;
	}
}

void TPropPageFrame::SetOnApplyChanges(TNotifyEvent evt)
{
	for(unsigned i=0;i<m_vecPanels.size();i++)
	{
		m_vecPanels[i]->OnApplyChanges = evt;
	}
}

void TPropPageFrame::SetOnUpdateList(TNotifyEvent evt)
{
	for(unsigned i=0;i<m_vecPanels.size();i++)
	{
		m_vecPanels[i]->OnUpdateList = evt;
	}
}



bool TPropPageFrame::DoTestData()
{
    for(unsigned i=0;i<m_vecPanels.size();i++)
    {
        if(!m_vecPanels[i]->DoTestData())return false;
    }
    return true;
}

void TPropPageFrame::SetControlFocus()
{
    int ind=0;
    if(Tabs->TabIndex >= 0) ind = Tabs->TabIndex;
    if(m_vecPanels.size() > ind)m_vecPanels[ind]->SetFocus();
}

void __fastcall TPropPageFrame::TabsChange(TObject *Sender)
{
	TFrame* pFrame = (TFrame*)Tabs->Tabs->Objects[Tabs->TabIndex];

	if(m_pCurrentFrame)
	{
		pFrame->Top = m_pCurrentFrame->Top;
		pFrame->Left = m_pCurrentFrame->Left;
		pFrame->Width = m_pCurrentFrame->Width;
		pFrame->Height  = m_pCurrentFrame->Height;
	}

	TsSkinManager* pMan =  sFrameAdapter->SkinData->SkinManager;
	if(pMan->AnimEffects->PageChange->Active && pMan->Active)
	{
		PrepareForAnimation(pFrame);
		m_pCurrentFrame->Visible = false;
		pFrame->Visible = true;
		AnimShowControl(pFrame, pMan->AnimEffects->PageChange->Time);
	}
	else
	{
		ShowCurrentFrame(pFrame);
		m_pCurrentFrame->Visible = false;
	}
	m_pCurrentFrame = pFrame;
}
//---------------------------------------------------------------------------

void __fastcall TPropPageFrame::TabsGetImageIndex(TObject *Sender, int TabIndex, int &ImageIndex)
{
    if(TabIndex < m_vecPanels.size())
		ImageIndex = m_vecPanels[TabIndex]->ImageIndex();
    else ImageIndex = -1;
}
//---------------------------------------------------------------------------

void TPropPageFrame::Show()
{
	if(Visible)return;
	Visible = true;
	Repaint();
}

void TPropPageFrame::Hide()
{
	Visible = false;
}


