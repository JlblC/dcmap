//---------------------------------------------------------------------------

#ifndef PlanetInfoH
#define PlanetInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sLabel.hpp"
#include "sFrameAdapter.hpp"
#include "sBevel.hpp"
#include <ExtCtrls.hpp>
#include "sButton.hpp"
#include "sEdit.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include <Menus.hpp>
#include "sPanel.hpp"
//#include "TntMenus.hpp"
//---------------------------------------------------------------------------
class TPlanetInfoFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsLabel *NameL;
    TsLabel *NameLabel;
	TsLabel *OwnerL;
    TsLabel *OwnerLabel;
	TsSpeedButton *CopyButton;
	TPopupMenu *CopyMenu;
	TsPanel *ExPanel;
	TShape *BoxShape;
	TsLabel *sLabel3;
	TsLabel *sLabel4;
	TsLabel *sLabel5;
	TsLabel *sLabel6;
	TsLabel *sLabel7;
	TsLabel *OLabel;
	TsLabel *ELabel;
	TsLabel *MLabel;
	TsLabel *TLabel;
	TsLabel *SLabel;
	TShape *OBox;
	TShape *EBox;
	TShape *MBox;
	TShape *TBox;
	TShape *SBox;
	TsWebLabel *WebGoStandart;
	TsWebLabel *WebGoPaid;
	TsSpeedButton *CollapseButton;
	TsSpeedButton *ExpandButton;
	TsLabel *sLabel1;
	TsLabel *OLabel2;
	TsLabel *ELabel2;
	TsLabel *sLabel9;
	TsLabel *sLabel10;
	TsLabel *MLabel2;
	TsLabel *sLabel12;
	TsLabel *TLabel2;
	TsLabel *SLabel2;
	TsLabel *sLabel15;
	TsEdit *CoordEdit;
	TShape *CorruptionBox;
	TShape *CorruptionOpBox;
	TsWebLabel *WebGoUser;
	void __fastcall CopyButtonClick(TObject *Sender);
	void __fastcall CopyMenuClick(TObject *Sender);
	void __fastcall OwnerLabelDblClick(TObject *Sender);
	void __fastcall NameLabelDblClick(TObject *Sender);
	void __fastcall CollapseButtonClick(TObject *Sender);
	void __fastcall CoordEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// User declarations
    int m_iPreHeight;
    int m_iCurrentCopyMode;
	bool Collapsed;
	void SetBoxHeight(TShape* sh,int h);
public:		// User declarations
    __fastcall TPlanetInfoFrame(TComponent* Owner);
protected:

	IDCMapGeoDataStoragePtr m_pGeo;
	IDCMapPlanetDataStoragePtr m_pPlanets;
	IDCMapPlayersDataStoragePtr m_pPlayers;

    IDCMapPlanetManager* m_pPlanetMan;
    IDCMapRecordExport*  m_pRecordExport;

    DCMapDataReference m_refOwner;
    DCMapDataReference m_refPlanet;

    bool OnInit();
    int OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    void UpdateData();
    void OnShow();
    void LoadConfig(DCMapConfigWrapper cfg);
	void SaveConfig(DCMapConfigWrapper cfg);
};
//---------------------------------------------------------------------------
extern PACKAGE TPlanetInfoFrame *PlanetInfoFrame;
//---------------------------------------------------------------------------
#endif
