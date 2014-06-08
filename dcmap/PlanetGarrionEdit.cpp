//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PlanetGarrionEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sListView"
#pragma resource "*.dfm"
TPlanetGarrisonFrame *PlanetGarrisonFrame;

//static CFramePanelWrapper<TPlanetGarrisonFrame> sWrap("PListEditPropPage_planets","05GarrisonInfo");

//---------------------------------------------------------------------------
__fastcall TPlanetGarrisonFrame::TPlanetGarrisonFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------

