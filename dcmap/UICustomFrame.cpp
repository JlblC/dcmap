//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UICustomFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma resource "*.dfm"
TPluginFrame *PluginFrame;

static CFramePanelWrapper<TPluginFrame> sWrap("UI","CustomFrame");

//---------------------------------------------------------------------------
__fastcall TPluginFrame::TPluginFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner),
	m_UIFrame(this)
{
}
//---------------------------------------------------------------------------

void* DCMAPAPI TPluginFrame::GetInterface(DCMapInterfaceID idInterface)
{
	if(DCMAP_INTERFACE_CMP(idInterface,IDCMapUICustomFrame))return &m_UIFrame;
	return 0;
}