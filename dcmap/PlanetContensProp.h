//---------------------------------------------------------------------------

#ifndef PlanetContensPropH
#define PlanetContensPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sListView.hpp"
#include <ComCtrls.hpp>
#include "sButton.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------

class TPlanetGarrisonFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsListView *BuildingsView;
	TsListView *GarrisonView;
	TsListView *QueueView;
	TsSpeedButton *AddToQueueBtn;
	TPopupMenu *ProjectsMenu;
	TMenuItem *BuildingsMenu;
	TMenuItem *UnitsMenu;
	void __fastcall BuildingsViewDeletion(TObject *Sender, TListItem *Item);
	void __fastcall AddClick(TObject *Sender);

private:	// User declarations

	IDCMapPlanetCalculator::PlanetParamInfo m_PlanetInfo;

	IDCMapPlanetDataStoragePtr m_pPlanets;
	IDCMapPlayersDataStoragePtr m_pPlayers;
	IDCMapFleetsDataStoragePtr m_pFleets;
	IDCMapBuildingsDataStoragePtr m_pBuildings;
	IDCMapProjectDataStoragePtr m_pProjects;
	IDCMapActionsDataStoragePtr m_pActions;
	IDCMapPlanetCalculator* m_pCalc;


	dcmapDBREF m_Ref;
	dcmapDBREF m_RefGarrizon;
	dcmapDBREF m_RefBuildings;
        dcmapDBREF m_RefQueue;
        dcmapDBREF m_RefOwner;

        void UpdateList(TsListView *TargetView,dcmapDBREF refFleet,bool sort);

public:		// User declarations
	__fastcall TPlanetGarrisonFrame(TComponent* Owner);
	int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	int UpdateData();
	bool OnInit();
	void UpdateLists();
	void MakeProjectsMenu();
	void OnShow();
};
//---------------------------------------------------------------------------
extern PACKAGE TPlanetGarrisonFrame *PlanetGarrisonFrame;
//---------------------------------------------------------------------------
#endif
