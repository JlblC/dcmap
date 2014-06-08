//---------------------------------------------------------------------------

#ifndef PlanetEconomyInfoH
#define PlanetEconomyInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sLabel.hpp"
#include <ExtCtrls.hpp>
#include "acPNG.hpp"
#include "sGroupBox.hpp"
//---------------------------------------------------------------------------
class TPlanetEconomyInfoFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TImage *BasisImg;
	TImage *SecondImg;
	TImage *MoneyImg;
	TsLabel *BasisLabel;
	TsLabel *SecondLabel;
	TsLabel *MoneyLabel;
	TsLabel *ProductionLabel;
	TImage *ProductionImg;
	TsGroupBox *PeopleGroupBox;
	TsLabel *PeopleFreeLabel;
	TsLabel *GrowthLabel;
	TsLabel *PeopleFree;
	TsLabel *PeopleGrowth;
	TsLabel *PeopleTotal;
	TsLabel *PeopleTotalLabel;
	TsLabel *Corruption;
	TsLabel *CorruptionLabel;
private:	// User declarations
	IDCMapPlanetCalculator::PlanetParamInfo m_PlanetInfo;
	IDCMapPlanetDataStoragePtr m_pPlanets;
	IDCMapPlayersDataStoragePtr m_pPlayers;
	IDCMapPlanetCalculator* m_pCalc;
	IDCMapPlanetManager* m_pPlanetMan;

public:		// User declarations
	__fastcall TPlanetEconomyInfoFrame(TComponent* Owner);
	bool OnInit();
	int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	void UpdateData();
	void ShowData(bool show);
};
//---------------------------------------------------------------------------
extern PACKAGE TPlanetEconomyInfoFrame *PlanetEconomyInfoFrame;
//---------------------------------------------------------------------------
#endif
