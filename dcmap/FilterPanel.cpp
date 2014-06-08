//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FilterPanel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "Filter"
#pragma resource "*.dfm"
TFilterPanelFrame *FilterPanelFrame;

static CFramePanelWrapper<TFilterPanelFrame> sWrap("PFrameBar","TestFilter");
static CAutoReg<CMapFilter> sReg("PPlanetFilter","TestFilter");

CMapFilter::CMapFilter()
{
    Changed=false;
    m_pFrame=0;
}

bool  DCMAPAPI CMapFilter::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	m_pWorkspace = workspace;
	return true;
}

void  DCMAPAPI CMapFilter::LoadConfig(IDCMapConfig* pConfig)
{
	DCMapConfigWrapper cfg(pConfig);
}

void  DCMAPAPI CMapFilter::SaveConfig(IDCMapConfig* pConfig)
{
	DCMapConfigWrapper cfg(pConfig);
}

int DCMAPAPI CMapFilter::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	switch(Msg)
	{
	case DCMM_DATA_CHANGED:
		break;
	case DCMM_POST_INIT:
		break;
	}
	return 0;
};

void* DCMAPAPI CMapFilter::CastTo(char const* szInterface)
{
	if(!stricmp(szInterface,"IDCMapPlanetDataFilter"))return (IDCMapPlanetDataFilter*)this;
	return 0;
}

bool DCMAPAPI CMapFilter::ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf)
{
	if(!m_pFrame)return true;
    m_pFrame->FilterFrame1->Process(pinfo->planet.id);
	return true;
}

bool DCMAPAPI CMapFilter::ActualizeData()
{
	bool update = Changed;
    Changed=false;

    return update;
}

//---------------------------------------------------------------------------
__fastcall TFilterPanelFrame::TFilterPanelFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------
bool TFilterPanelFrame::OnInit()
{
    Caption = "Ôèëüòð";
    IDCMapModuleManager* pCreator = m_pWorkspace?(IDCMapModuleManager*)m_pWorkspace:(IDCMapModuleManager*)m_pCore;
    FilterFrame1->OnInit(pCreator);
    return true;
}

int TFilterPanelFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	switch (Msg)
	{
	case DCMM_DATA_CHANGED:
	  break;
	case DCMM_POST_INIT:
	  if(m_pWorkspace->SelectModuleByID("CMapFilter"))
	  {
		  m_pFilter = (CMapFilter*)m_pWorkspace->Module();
          m_pFilter->m_pFrame = this;
	  }
	  break;
	}
    return 0;
}

void TFilterPanelFrame::OnShow()
{
}
void TFilterPanelFrame::LoadConfig(DCMapConfigWrapper cfg)
{
}
void TFilterPanelFrame::SaveConfig(DCMapConfigWrapper cfg)
{
}
