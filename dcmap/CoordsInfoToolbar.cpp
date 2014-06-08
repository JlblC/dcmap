//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CoordsInfoToolbar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sLabel"
#pragma resource "*.dfm"
TPlanetInfoToolbarFrame *PlanetInfoToolbarFrame;

static CFramePanelWrapper<TPlanetInfoToolbarFrame> sWrap("PToolbarItem","PlanetDistancesToolItem");

//---------------------------------------------------------------------------
__fastcall TPlanetInfoToolbarFrame::TPlanetInfoToolbarFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
	x1=-1;
        y1=-1;
	x2=-1;
        y2=-1;
        UpdateInfo();
}
//---------------------------------------------------------------------------
int TPlanetInfoToolbarFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
    case DCMM_PLANET_CHANGED:
        {
            DataReference ref;
            /*int X,Y;
    	    if(m_pPlanetMan->GetCurrentPlanetCoords(&x1,&y1))
            {
                 if(m_pPlanets->SelectID(ref))
                 {
                     x1 = m_pPlanets->GetData()->x;
                     y1 = m_pPlanets->GetData()->y;
                     UpdateInfo();
                 }
            }
            else */

            if(m_pPlanetMan->GetCurrentPlanetCoords(&x1,&y1))
            {
                UpdateInfo();
            }
            else
            {
                x1=-1;
                y1=-1;
            }
        }
        break;
    case DCMM_POINT_PLANET:
         x2 = wParam;
         y2 = lParam;
         UpdateInfo();
    	break;
    }
    return 0;
}

void TPlanetInfoToolbarFrame::UpdateInfo()
{
	int w = Width;
	 if(x1 >=0 && y1 >= 0)
     {
        int x = x2;
        int y = y2;

        if( abs(x-x1) > abs((x+DCMapNumPlanets)-x1))
            x += DCMapNumPlanets;
        if( abs(y-y1) > abs((y+DCMapNumPlanets)-y1))
            y += DCMapNumPlanets;

        if( abs(x-x1) > abs((x-DCMapNumPlanets)-x1))
            x -= DCMapNumPlanets;
        if( abs(y-y1) > abs((y-DCMapNumPlanets)-y1))
            y -= DCMapNumPlanets;

         float dx = x1-x;
         float dy = y1-y;
         float d = Sqrt(dx*dx+dy*dy);

         InfoLabel->Caption = IntToStr(x1)+":"+IntToStr(y1)+ " <-> "+
                  IntToStr(x2)+":"+IntToStr(y2)+"  "+FloatToStrF(d,ffFixed,7,2);
     }
     else if(x2 >=0 && y2 >= 0)
     {
         InfoLabel->Caption = IntToStr(x2)+":"+IntToStr(y2);
     }
	 else InfoLabel->Caption = "";

	 Width = w;
}

bool TPlanetInfoToolbarFrame::OnInit()
{
	m_pPlanetMan = m_pWorkspace->GetServiceTyped<IDCMapPlanetManager>("PlanetManager");
	if(!m_pPlanetMan)return false;

    DCMAP_GET_STORAGE(m_pCore,m_pPlanets,IDCMapPlanetDataStorage,planets);

    return true;
}
