//---------------------------------------------------------------------------

#ifndef PlayerPropH
#define PlayerPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sComboBox.hpp"
#include "sEdit.hpp"
#include "sGroupBox.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "sCheckBox.hpp"
#include "sLabel.hpp"
#include "sComboBoxes.hpp"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include "sColorSelect.hpp"
#include "sRadioButton.hpp"
#include "sMemo.hpp"
//#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TPlayerEditPropFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
    TsComboBox *RaceComboBox;
    TsRadioGroup *DipRadio;
    TsSpeedButton *sSpeedButton1;
    TsComboBox *OrgComboBox;
    TsLabel *sLabel1;
    TsComboBox *RelationComboBox;
    TsCheckBox *RelMultCheckBox;
    TsCheckBox *VassalCheckBox;
    TsWebLabel *SURL;
    TsSpeedButton *sSpeedButton2;
	TsColorSelect *PlayerColor1;
	TsColorSelect *PlayerColor2;
	TsComboBoxEx *IconCombo;
	TsRadioButton *OtherDipCheck;
	TsComboBox *OtherDip;
	TsMemo *NotesMemo;
	TsEdit *NameEdit;
    void __fastcall DataChange(TObject *Sender);
    void __fastcall RelMultCheckBoxClick(TObject *Sender);
    void __fastcall sSpeedButton1Click(TObject *Sender);
    void __fastcall sSpeedButton2Click(TObject *Sender);
	void __fastcall DipRadioClick(TObject *Sender);
	void __fastcall OtherDipCheckClick(TObject *Sender);
	void __fastcall OtherDipChange(TObject *Sender);
	void __fastcall NameEditChange(TObject *Sender);
private:	// User declarations
    void UpdateData();
    DataReference m_Ref;

    IDCMapDipDataStoragePtr m_pDip;

    IDCMapPlayersDataStorage* m_pPlayers;
    IDCMapOrganizationDataStorage* m_pOrg;
    IDCMapRacesDataStorage*   m_pRaces;
    IDCMapIconsDataStorage* m_pIcons;

public:		// User declarations
    __fastcall TPlayerEditPropFrame(TComponent* Owner);
    void OnShow();
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    bool OnInit();
    void OnApply();
    void OnCansel();
    bool OnDataTest();
};
//---------------------------------------------------------------------------
extern PACKAGE TPlayerEditPropFrame *PlayerEditPropFrame;
//---------------------------------------------------------------------------
#endif
