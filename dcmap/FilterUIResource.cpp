//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FilterUIResource.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sCheckBox"
#pragma link "sComboBox"
#pragma link "sEdit"
#pragma link "sFrameAdapter"
#pragma link "sSpinEdit"
#pragma resource "*.dfm"
TFilterUIResourceFrame *FilterUIResourceFrame;

static CFramePanelWrapper<TFilterUIResourceFrame> sWrap("FilterUIItem","ResourceCompare");

//---------------------------------------------------------------------------
__fastcall TFilterUIResourceFrame::TFilterUIResourceFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
	m_pFilter=0;
}
//---------------------------------------------------------------------------

int TFilterUIResourceFrame::OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	 switch(MSG)
     {
     case DCMM_FILTERUI_SET_FILTER:
        m_pFilter =  pSender->Cast<IDCMapFilterItem>();
        if(m_pFilter)
        {
            NameCheck->Caption = m_pFilter->ItemCaption();
        }
        break;
     }
}
