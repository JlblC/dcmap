//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef PlayerDataLoadPropH
#define PlayerDataLoadPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sCheckBox.hpp"
#include "sEdit.hpp"
#include "sSpinEdit.hpp"
#include "sGroupBox.hpp"
#include "sLabel.hpp"
#include <ExtCtrls.hpp>
#include "sRadioButton.hpp"
//---------------------------------------------------------------------------
class TPlayerAdvPropFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsCheckBox *CorruptionCalcCheck;
	TsEdit *NameEdit;
	TsEdit *PassEdit;
	TsCheckBox *WebAccessCheck;
	TsSpinEdit *UserIDEdit;
	TsSpinEdit *FleetIDEdit;
	TsGroupBox *IncomeBox;
	TImage *BasisImg;
	TsLabel *BasisLabel;
	TImage *SecondImg;
	TsLabel *SecondLabel;
	TImage *CreditsImg;
	TsLabel *CreditsLabel;
	TsCheckBox *Manage;
	TsCheckBox *ImportDip;
	TsGroupBox *PlanetImportModes;
	TsRadioButton *UpdateMy;
	TsRadioButton *ReplaceAll;
	TsRadioButton *UpdateAll;
	TsRadioButton *UpdateActual;
	TsCheckBox *ImportContents;
	TsCheckBox *ImportFleets;
	TsGroupBox *ImportUntrusted;
	TsCheckBox *AddOccupied;
	TsCheckBox *AddEmpty;
	TsCheckBox *OverwriteEmpty;
	TsCheckBox *OverwriteOccupied;
	TsCheckBox *ClearOccupied;
	TsCheckBox *ImportTrusted;
	TsCheckBox *ImportAlienFleets;
	TsEdit *ImportRegionEdit;
	void __fastcall DataChange(TObject *Sender);
	void __fastcall CorruptionCalcCheckClick(TObject *Sender);
	void __fastcall PassEditChange(TObject *Sender);
	void __fastcall UpdateChecks(TObject *Sender);
private:	// User declarations
    void UpdateData();
	DataReference m_Ref;
	DataReference m_RaceRef;
	bool CalculateIncome();
	IDCMapPlayersDataStoragePtr m_pPlayers;
	IDCMapRacesDataStoragePtr m_pRaces;
	bool PasswordChanged;
	String PassText;

public:		// User declarations
	__fastcall TPlayerAdvPropFrame(TComponent* Owner);
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    bool OnInit();
    void OnApply();
    void OnCansel();
};
//---------------------------------------------------------------------------
extern PACKAGE TPlayerAdvPropFrame *PlayerAdvPropFrame;
//---------------------------------------------------------------------------
#endif
