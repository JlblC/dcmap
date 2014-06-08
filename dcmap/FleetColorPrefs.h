//---------------------------------------------------------------------------

#ifndef FleetColorPrefsH
#define FleetColorPrefsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BasePrefs.h"
#include "sFrameAdapter.hpp"
#include "sColorSelect.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "sGroupBox.hpp"
//---------------------------------------------------------------------------
class TFleetColorPrefsFrame : public TBasePrefsFrame
{
__published:	// IDE-managed Components
	TsColorSelect *OpenPlanetBorder;
	TsGroupBox *JumpAvaibilityGroup;
	TsColorSelect *FleetStart;
	TsColorSelect *Dist1Turn;
	TsColorSelect *Dist2Turn;
	TsColorSelect *Dist3Turn;
	TsColorSelect *NormalDist;
	TsColorSelect *MaxDist;
	TsGroupBox *FleetColorsGroup;
	TsColorSelect *MyFleet;
	TsColorSelect *FleetInTransit;
	TsColorSelect *UndefinedFleet;
	TsColorSelect *NeutralFleet;
	TsColorSelect *AllyFleet;
	TsColorSelect *EnemyFleet;
	TsColorSelect *IncomingFleet;
private:	// User declarations
public:		// User declarations
	__fastcall TFleetColorPrefsFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFleetColorPrefsFrame *FleetColorPrefsFrame;
//---------------------------------------------------------------------------
#endif
