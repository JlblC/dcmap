//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef PlayerToolbarH
#define PlayerToolbarH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sComboBox.hpp"
#include <ExtCtrls.hpp>
#include "sLabel.hpp"
//---------------------------------------------------------------------------
class TPlayerToolbarFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsComboBox *PlayerCombo;
	TImage *BasisImg;
	TImage *SecondImg;
	TsLabel *BasisLabel;
	TsLabel *SecondLabel;
	TsLabel *CreditsLabel;
	TImage *CreditsImg;
	TsLabel *BasisSpent;
	TsLabel *BasisGain;
	TsLabel *SecondGain;
	TsLabel *SecondSpent;
	TsLabel *CreditsGain;
	TsLabel *CreditsSpent;
	void __fastcall PlayerComboChange(TObject *Sender);
private:	// User declarations
	IDCMapPlanetDataStoragePtr m_pPlanets;
	IDCMapPlayersDataStoragePtr m_pPlayers;
	IDCMapRacesDataStoragePtr m_pRaces;
	IDCMapPlanetCalculator* m_pCalc;

	String LastPlayer;

	void LoadConfig(DCMapConfigWrapper cfg);
	void SaveConfig(DCMapConfigWrapper cfg);

public:		// User declarations
	__fastcall TPlayerToolbarFrame(TComponent* Owner);
	void UpdateList();
	int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	bool OnInit();
};
//---------------------------------------------------------------------------
extern PACKAGE TPlayerToolbarFrame *PlayerToolbarFrame;
//---------------------------------------------------------------------------
#endif
