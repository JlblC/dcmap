//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef CoordsInfoToolbarH
#define CoordsInfoToolbarH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sLabel.hpp"
//---------------------------------------------------------------------------
class TPlanetInfoToolbarFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsLabel *InfoLabel;
private:	// User declarations
    IDCMapPlanetDataStorage* m_pPlanets;
    IDCMapPlanetManager* m_pPlanetMan;

    int x1,y1,x2,y2;

    void UpdateInfo();
public:		// User declarations
	__fastcall TPlanetInfoToolbarFrame(TComponent* Owner);
	int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	bool OnInit();
 };
//---------------------------------------------------------------------------
extern PACKAGE TPlanetInfoToolbarFrame *PlanetInfoToolbarFrame;
//---------------------------------------------------------------------------
#endif
