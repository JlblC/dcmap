//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GraphicToolbar.h"
#include "GraphHelper.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sToolBar"
#pragma link "sSpeedButton"
#pragma link "sComboBox"
#pragma resource "*.dfm"
TGraphicToolbarFrame *GraphicToolbarFrame;

static CFramePanelWrapper<TGraphicToolbarFrame> sWrap("PToolbarItem","GraphicToolbarItem");

//---------------------------------------------------------------------------
__fastcall TGraphicToolbarFrame::TGraphicToolbarFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
	m_pView=0;
	m_DrawForm=0;
}
//---------------------------------------------------------------------------
int TGraphicToolbarFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
	case DCMM_LAYERS_CHANGED:
	  m_pView = pSender->Cast<IDCMapLayeredMapView>();
      UpdateList();
	  break;
	case DCMM_GRAPHICS_DRAW_BEGIN:
	  break;
	case DCMM_GRAPHICS_DRAW_END:
	  UpdateTools(true);
	  break;
	case DCMM_GRAPHICS_POINT_PLACED:
	  break;
	case DCMM_GRAPHICS_TEXT_PLACED:
	  if(m_DrawForm)
	  {
		m_DrawForm->TextPlaced(lParam);
	  }
	  break;
	}
    return 0;
}

void TGraphicToolbarFrame::UpdateList()
{
	if(!m_pView)return;

	LayerCombo->Items->Clear();
    int n = m_pView->NumLayers();
    for(int i=0;i<n;i++)
    {
        if(SameStr(m_pView->LayerModuleName(i),"GraphicsLayer"))
        {
        	LayerCombo->Items->AddObject(m_pView->LayerCaption(i),(TObject*)i);
        }
    }
    if(LayerCombo->Items->Count)
    {
    	LayerCombo->ItemIndex = LayerCombo->Items->IndexOf(LastLayer);
        if(LayerCombo->ItemIndex < 0) LayerCombo->ItemIndex = 0;
    }
    LastLayer = LayerCombo->Text;

    m_pCurrentLayer=0;
    UpdateTools(true);
}

void TGraphicToolbarFrame::UpdateTools(bool d)
{
	bool use=false;

	use |= AddPolygonBtn->Down;
	use |= AddPolylineBtn->Down;
	use |= AddZonesBtn->Down;

	if(d || !use)
	{
		AddPolygonBtn->Down = false;
		AddPolylineBtn->Down = false;
		AddZonesBtn->Down = false;

		if(m_pCurrentLayer)m_pCurrentLayer->EndDraw();
		if(m_DrawForm)m_DrawForm->Close();
		m_pCurrentLayer = 0;
		if(m_pView) m_pView->SetActiveLayer(0);
		return;
	}

	if(!m_pView || !LayerCombo->Items->Count)
	{
		UpdateTools(1);
		return;
	}

	int n = (int)LayerCombo->Items->Objects[LayerCombo->ItemIndex];

	m_pView->SetLayerVisiblity(n,true);
    m_pView->UpdateLayers();
	m_pView->SetActiveLayer(n);


    IDCMapLayer* pLayer = m_pView->LayerInterface(n);
    if(pLayer)
        m_pCurrentLayer = pLayer->Cast<IDCMapGraphicLayer>();
    else m_pCurrentLayer = 0;

	if(!m_pCurrentLayer)
    {
	   AddPolygonBtn->Down = false;
	   AddPolylineBtn->Down = false;
	   AddZonesBtn->Down = false;

	   m_pView->SetActiveLayer(0);
	   return;
	}
	else
	{
		m_pCurrentLayer->EndDraw();
		if(AddPolygonBtn->Down)
		{
			m_pCurrentLayer->BeginDraw(DCMAP_GRAPHICS_POLYGON,m_pPanelInterface);
		}
		else if(AddPolylineBtn->Down)
		{
			m_pCurrentLayer->BeginDraw(DCMAP_GRAPHICS_POLYLINE,m_pPanelInterface);
		}
		else if(AddZonesBtn->Down)
		{
			m_pCurrentLayer->BeginDraw(DCMAP_GRAPHICS_ZONES,m_pPanelInterface);
		}
		m_pView->UpdateLayers();

		if(!m_DrawForm)
		{
			m_DrawForm = TGraphHelperForm::CreateInstance(m_pWorkspace);
		}
		m_DrawForm->Setup(this,m_pCurrentLayer);
		m_DrawForm->Show();
	}
}

void __fastcall TGraphicToolbarFrame::AddPolygonBtnClick(TObject *Sender)
{
	if(!m_pView) return;
	UpdateTools(0);
}
//---------------------------------------------------------------------------

void __fastcall TGraphicToolbarFrame::EditButtonClick(TObject *Sender)
{
	LayerComboChange(Sender);
    if(LayerCombo->ItemIndex < 0)return;
	m_pCore->GetCoreUI()->OpenListTableEdit("graphics",0,&m_LayerRef);
}
//---------------------------------------------------------------------------

void __fastcall TGraphicToolbarFrame::LayerComboChange(TObject *Sender)
{
    if(!m_pView)return;

    if(LayerCombo->ItemIndex < 0)return;

    int n = (int)LayerCombo->Items->Objects[LayerCombo->ItemIndex];

    IDCMapLayer* pLayer = m_pView->LayerInterface(n);
    if(pLayer)
        m_pCurrentLayer = pLayer->Cast<IDCMapGraphicLayer>();
    else m_pCurrentLayer = 0;

    DCMapDataReference const* pDef = m_pView->LayerDataID(n);
    if(pDef)m_LayerRef = *pDef;

    LastLayer = LayerCombo->Text;

    UpdateTools(true);
}

void TGraphicToolbarFrame::LoadConfig(DCMapConfigWrapper cfg)
{
	cfg.section("GraphicToolbar");
    cfg["ActiveLayer"] >> LastLayer;
}

void TGraphicToolbarFrame::SaveConfig(DCMapConfigWrapper cfg)
{
	cfg.section("GraphicToolbar");
	cfg["ActiveLayer"] << LastLayer;
}

void TGraphicToolbarFrame::DoEndDraw()
{
	m_DrawForm=0;
	UpdateTools(true);
}

//---------------------------------------------------------------------------


