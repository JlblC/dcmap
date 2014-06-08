//---------------------------------------------------------------------------
#ifndef PlanetPropH
#define PlanetPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sFrameAdapter.hpp"
#include "sLabel.hpp"
#include "sComboBox.hpp"
#include "dcmapsys_vcl.h"
#include "sEdit.hpp"
#include "sSpinEdit.hpp"
#include "sGauge.hpp"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include "sMemo.hpp"
#include "sFrameBar.hpp"
#include "sScrollBox.hpp"
#include "sSpeedButton.hpp"
#include "BaseModule.h"
#include <Buttons.hpp>
#include "sColorSelect.hpp"
#include "sComboBoxes.hpp"
#include "IconsListMan.h"
//#include "TntStdCtrls.hpp"

//---------------------------------------------------------------------------
class TPlanetParamFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
    TsComboBox *OwnerComboBox;
    TsSpinEdit *EditO;
    TsSpinEdit *EditM;
    TsSpinEdit *EditE;
    TsSpinEdit *EditT;
    TsSpinEdit *EditS;
    TsSpeedButton *sSpeedButton1;
	TsMemo *CommentMemo;
	TsColorSelect *PlanetColor2;
	TsColorSelect *PlanetColor1;
	TsComboBoxEx *IconCombo1;
	TsComboBoxEx *IconCombo2;
	TsSpinEdit *EditTurn;
	TsEdit *NameEdit;
	TsSpeedButton *KeepNameBtn;
	TsSpeedButton *SynchronizeBtn;
    void __fastcall DataChange(TObject *Sender);
    void __fastcall sSpeedButton1Click(TObject *Sender);
	void __fastcall NameEditChange(TObject *Sender);
private:	// User declarations
    void UpdateData();
    DataReference m_Ref;

    DCMapPlanetDataStoragePtr m_pPlanets;
    IDCMapPlayersDataStoragePtr m_pPlayers;
    IDCMapIconsDataStoragePtr m_pIcons;
    IDCMapActionsDataStoragePtr m_pActions;

    TIconsList* m_pIconsList;
    String BaseName;
    bool NameChanged;
    String OldPlanetName;

public:		// User declarations
    __fastcall TPlanetParamFrame(TComponent* Owner);
    void OnApply();
    void OnCansel();
    bool OnInit();
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
};
//---------------------------------------------------------------------------
extern PACKAGE TPlanetParamFrame *PlanetParamFrame;
//---------------------------------------------------------------------------
#endif


