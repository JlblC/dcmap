//---------------------------------------------------------------------------

#ifndef GeoValueH
#define GeoValueH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sComboBox.hpp"
#include "sRadioButton.hpp"
#include "sLabel.hpp"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TGeoValueFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
    TsComboBox *PlayerList;
    TsComboBox *RaceList;
    TsRadioButton *PlayerRadio;
    TsRadioButton *RaceRadio;
    TsLabel *GrowthLabel;
	TsLabel *MBL;
	TsLabel *CBL;
	TsLabel *MSL;
	TsLabel *CSL;
    TsLabel *MBLabel;
    TsLabel *CBLabel;
    TsLabel *MSLabel;
    TsLabel *CSLabel;
	TImage *UpImage;
	TImage *DownImage;
	TImage *MiddleImage;
	TsSpeedButton *CollapseButton;
	TsSpeedButton *ExpandButton;
	TImage *BasisImg;
	TImage *SecondImg;
    void __fastcall PlayerListChange(TObject *Sender);
    void __fastcall RaceRadioClick(TObject *Sender);
    void __fastcall RaceListChange(TObject *Sender);
	void __fastcall CollapseButtonClick(TObject *Sender);
private:	// User declarations

    DCMapDataSourcePtr<IDCMapGeoDataStorage> m_pGeo;
    DCMapDataSourcePtr<IDCMapPlanetDataStorage> m_pPlanets;
    DCMapDataSourcePtr<IDCMapPlayersDataStorage> m_pPlayers;
    DCMapDataSourcePtr<IDCMapRacesDataStorage> m_pRaces;

    IDCMapGeoValueCalculator* m_pGeoCalc;
    IDCMapPlanetManager* m_pPlanetMan;

    DataReference m_PlanetRef;

    void UpdateListData();

    void UpdateData();
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);

    DCMapRaceInfo m_RaceInfo;

    String LastRace;
    String LastPlayer;

    IDCMapPlanetColorProvider* m_pFilter;

    void RaceChanged();
    void ClearView();

    int m_iPreHeight;
    bool Collapsed;

public:		// User declarations
    __fastcall TGeoValueFrame(TComponent* Owner);
    bool OnInit();
    void LoadConfig(DCMapConfigWrapper cfg);
    void SaveConfig(DCMapConfigWrapper cfg);
};
//---------------------------------------------------------------------------
extern PACKAGE TGeoValueFrame *GeoValueFrame;
//---------------------------------------------------------------------------
#endif
