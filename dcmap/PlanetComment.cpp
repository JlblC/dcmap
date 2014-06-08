//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PlanetComment.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sMemo"
#pragma link "BaseModule"
#pragma resource "*.dfm"
TPlanetCommentFrame *PlanetCommentFrame;

static CFramePanelWrapper<TPlanetCommentFrame> sWrap("PFrameBar","PlanetComment","Комментарий к планете");

//---------------------------------------------------------------------------
__fastcall TPlanetCommentFrame::TPlanetCommentFrame(TComponent* Owner)
	: TSizeableSideFrame(Owner)
{
    m_bChanging=false;
}

bool TPlanetCommentFrame::OnInit()
{
	m_pPlanetMan = m_pWorkspace->GetServiceTyped<IDCMapPlanetManager>("PlanetManager");
	if(!m_pPlanetMan)return false;
    m_pPlanets= m_pCore->GetDataSourceTyped<IDCMapPlanetDataStorage>("planets");
    if(!m_pPlanets)return false;
    UpdateData();
    return true;
}

int TPlanetCommentFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
    case DCMM_PLANET_CHANGED:
    case DCMM_DATA_CHANGED:
      UpdateData();
      break;
    }
    return 0;
}

void TPlanetCommentFrame::SaveConfig(DCMapConfigWrapper cfg)
{
	Inherited::SaveConfig(cfg);
   //	UpdateData();
}

void TPlanetCommentFrame::UpdateData()
{
    m_bChanging=true;

    DCMapDataReference ref;
    if(m_pPlanetMan->GetCurrentPlanet(&ref))
    {
    	if(!m_pPlanets->SelectID(ref))
        {
        	ref.Invalidate();
        }
    }
    if(Changed)
    {
        Changed = false;
        if(m_pPlanets->SelectID(m_CurrentRef))
        {
            m_CurrentRef = ref;
            m_pPlanets->Update();
            m_pPlanets->SetWideNotes(CommentMemo->Text.w_str());
            m_pPlanets->Post();
        }
    }
    m_CurrentRef = ref;

    if(m_CurrentRef.ValidID())
	{
		if(m_pPlanets->SelectID(m_CurrentRef))
        {
			wchar_t const* notes =  m_pPlanets->GetWideNotes();
			wchar_t const* ch = notes&&notes[0]?notes:L"";
			CommentMemo->Text = notes&&notes[0]?notes:L"";
        	CommentMemo->Enabled = true;
        }
    }
    else
    {
        CommentMemo->Enabled = false;
		CommentMemo->Text = L"Нет планеты - нет комментариев";
    }
  	if(CommentMemo->Lines->Count > 7)      CommentMemo->ScrollBars = ssVertical;
    else       CommentMemo->ScrollBars = ssNone;

    Changed = false;
    m_bChanging=false;
}

//---------------------------------------------------------------------------
void __fastcall TPlanetCommentFrame::CommentMemoChange(TObject *Sender)
{
    if(m_bChanging)return;

    m_bChanging=true;
	if(CommentMemo->Lines->Count > 7)      CommentMemo->ScrollBars = ssVertical;
    else       CommentMemo->ScrollBars = ssNone;
    Changed = true;
    m_bChanging=false;
}
//---------------------------------------------------------------------------

void __fastcall TPlanetCommentFrame::CommentMemoExit(TObject *Sender)
{
    if(m_bChanging)return;
	UpdateData();
}
//---------------------------------------------------------------------------

