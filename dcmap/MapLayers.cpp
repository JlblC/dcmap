//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MapLayers.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sAlphaListBox"
#pragma link "sCheckListBox"
#pragma link "BaseModule"
#pragma link "sPanel"
#pragma resource "*.dfm"
TMapLayersFrame *MapLayersFrame;

static CFramePanelWrapper<TMapLayersFrame> sWrap("PFrameBar","MapLayers","Управление слоями");

//---------------------------------------------------------------------------
__fastcall TMapLayersFrame::TMapLayersFrame(TComponent* Owner)
	: Inherited(Owner)
{
	Caption = L"Управление слоями";
	m_pView=0;
	LastNumLayers=0;
}
//---------------------------------------------------------------------------

int TMapLayersFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
    case DCMM_LAYERS_CHANGED:
    case DCMM_LAYERS_VISIBLITY_CHANGED:
      m_pView = pSender->Cast<IDCMapLayeredMapView>();
      UpdateList();
      break;
    }
    return 0;
}

void TMapLayersFrame::UpdateList()
{
	if(!m_pView)return;

	int top = LayersListBox->TopIndex;

	int n = m_pView->NumLayers();

	int iCurPos=0;
	int iCount = LayersListBox->Count;
	for(int i=0;i<n;i++)
	{
		if(m_pView->LayerListed(i))
		{
			if(iCurPos >= iCount)
			{
				LayersListBox->Items->AddObject(m_pView->LayerCaption(i),(TObject*)i);
			}
			else
			{
				LayersListBox->Items->Objects[iCurPos] = (TObject*)i;
				LayersListBox->Items->Strings[iCurPos] = m_pView->LayerCaption(i);
			}
			LayersListBox->Checked[iCurPos] = m_pView->LayerVisible(i);
			iCurPos++;
		}
	}
	if(iCurPos < iCount)
	{
		for(int i = iCurPos;i<iCount;i++)
		{
			LayersListBox->Items->Delete(iCurPos);
		}
	}
	if(LayersListBox->ItemIndex != LayersListBox->Items->IndexOf(LastSel))
		LayersListBox->ItemIndex = LayersListBox->Items->IndexOf(LastSel);

	LayersListBox->TopIndex = top;
}

void __fastcall TMapLayersFrame::LayersListBoxClickCheck(TObject *Sender)
{
	LayersListBoxClick(Sender);
	int n = LayersListBox->Count;
    for(int i=0;i<n;i++)
    {
		m_pView->SetLayerVisiblity((int)LayersListBox->Items->Objects[i],LayersListBox->Checked[i]);
    }
    m_pView->UpdateLayers();
}
//---------------------------------------------------------------------------

void __fastcall TMapLayersFrame::LayersListBoxClick(TObject *Sender)
{
	if(LayersListBox->ItemIndex >= 0)
		LastSel = LayersListBox->Items->Strings[LayersListBox->ItemIndex];
    else LastSel = "";
}
//---------------------------------------------------------------------------

void __fastcall TMapLayersFrame::LayersListBoxDblClick(TObject *Sender)
{
	if(LayersListBox->ItemIndex >= 0)
	{
		int ind = (int)LayersListBox->Items->Objects[LayersListBox->ItemIndex];
		m_pView->EditLayer(ind);
    }
}
//---------------------------------------------------------------------------

