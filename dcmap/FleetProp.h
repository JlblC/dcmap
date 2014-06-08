//---------------------------------------------------------------------------

#ifndef FleetPropH
#define FleetPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseProp.h"
#include "sFrameAdapter.hpp"
#include "sEdit.hpp"
#include <ExtCtrls.hpp>
#include "BaseModule.h"
#include "sComboBox.hpp"
#include "sGroupBox.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFleetEditFrame : public TBaseEditModuleFrame
{
typedef TBaseEditModuleFrame Inherited;
__published:	// IDE-managed Components
	TsEdit *NameEdit;
	TsEdit *ExtNameEdit;
	TsGroupBox *BehavoirGroup;
	TsComboBox *UnknownBehCombo;
	TsComboBox *NeutralsBehCombo;
	TsComboBox *VassalBehCombo;
	TsComboBox *EnemiesBehCombo;
	TsComboBox *AllyBehCombo;
	TsComboBox *LordBehCombo;
	void __fastcall ExtNameEditChange(TObject *Sender);
	void __fastcall BehComboChange(TObject *Sender);
private:	// User declarations
    void OnDataUpdate();
	bool ExtNameChanged;
	bool BehaviorChanged;
	bool m_bStealth;

	TsComboBox* BehBoxes[6];

    DCMapPlanetDataStoragePtr m_pPlanets;
    IDCMapPlayersDataStoragePtr m_pPlayers;
    IDCMapIconsDataStoragePtr m_pIcons;
    IDCMapActionsDataStoragePtr m_pActions;
    IDCMapFleetsDataStoragePtr m_pFleets;

    IDCMapFleetManager*  m_pFleetMan;
    IDCMapPlanetCalculator* m_pCalc;


public:		// User declarations
    __fastcall TFleetEditFrame(TComponent* Owner);
    void OnApply();
    void OnCansel();
	bool OnInit();
	void OnCreate();
};
//---------------------------------------------------------------------------
extern PACKAGE TFleetEditFrame *FleetEditFrame;
//---------------------------------------------------------------------------
#endif
