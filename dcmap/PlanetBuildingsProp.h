//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef PlanetBuildingsPropH
#define PlanetBuildingsPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sEdit.hpp"
#include "sSpinEdit.hpp"
#include "sCheckBox.hpp"
#include "sRadioButton.hpp"
#include "sBitBtn.hpp"
#include <Buttons.hpp>
#include "sGroupBox.hpp"
#include "sLabel.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPlanetBuildingsFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsSpinEdit *MinesEdit;
	TsSpinEdit *BiofarmsEdit;
	TsSpinEdit *PowerPlantsEdit;
	TsSpinEdit *FactoriesEdit;
	TsSpinEdit *CompressorsEdit;
	TsSpinEdit *GovernorsEdit;
	TsCheckBox *BarracksCheck;
	TsCheckBox *ShipyardCheck;
	TsCheckBox *ComponentsCheck;
	TsCheckBox *CustomsCheck;
	TsCheckBox *ResearchCheck;
	TsCheckBox *DesignCheck;
	TsCheckBox *PalaceCheck;
	TsRadioButton *IndustrialRadio;
	TsRadioButton *BounceRadio;
	TsRadioButton *MiningRadio;
	TsCheckBox *HWCheck;
	TsCheckBox *UnsafeCheck;
	TsEdit *CorruptionEdit;
	TsGroupBox *ParamsGroupBox;
	TImage *BasisImg;
	TImage *SecondImg;
	TsLabel *BasisLabel;
	TsLabel *SecondLabel;
	TsLabel *CreditsLabel;
	TImage *CreditsImg;
	TsLabel *MaxPopLabel;
	TsLabel *PopMaxValue;
	TsLabel *PopFreeLabel;
	TsLabel *PopFreeValue;
	TsLabel *ProductionValue;
	TsLabel *ProductionLabel;
	TsLabel *CorruptLabel;
	TsLabel *CorruptValue;
	TsLabel *PopCurLabel;
	TsLabel *PopCurValue;
    void __fastcall DataChange(TObject *Sender);
	void __fastcall HWCheckClick(TObject *Sender);
	void __fastcall CalculateCorruptionBtnClick(TObject *Sender);
private:	// User declarations
	int UpdateData();
	DCMapPlanetInfo m_Inf;
	DataReference m_Ref;
	DCMapPlanetDataStoragePtr m_pPlanets;
	IDCMapPlayersDataStoragePtr m_pPlayers;
	IDCMapRacesDataStoragePtr m_pRaces;
	IDCMapPlanetCalculator* m_pCalc;

	bool HWChanged;

public:		// User declarations
	__fastcall TPlanetBuildingsFrame(TComponent* Owner);
    void OnApply();
    void OnCansel();
    bool OnInit();
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
};
//---------------------------------------------------------------------------
extern PACKAGE TPlanetBuildingsFrame *PlanetBuildingsFrame;
//---------------------------------------------------------------------------
#endif
