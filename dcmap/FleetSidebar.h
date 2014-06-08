//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef FleetSidebarH
#define FleetSidebarH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sTreeView.hpp"
#include <ComCtrls.hpp>
#include "sListView.hpp"
#include <ImgList.hpp>
#include "sPanel.hpp"
#include "sSplitter.hpp"
#include <ExtCtrls.hpp>
#include "sLabel.hpp"
#include "sButton.hpp"
#include "sBevel.hpp"
#include "acAlphaImageList.hpp"
#include <Menus.hpp>
//---------------------------------------------------------------------------
#include <set>

class TSelectedItems;
class TItemData;
class FleetNode;
class TIconsList;
typedef std::set<dcmapDBREF> ExpandedSet;

enum NodeType
{
	NodeNone=-1,
        NodeGarrizon=0,
        NodeBuildings,
        NodeQueue,
	NodeTypeNum
};

struct RestoreState
{
        bool OpenByType[NodeTypeNum];
	ExpandedSet exp;
        dcmapDBREF top_ref;
        TItemData* top_item;
	int top;
	int topdist;
	void SaveConfig(String Name,DCMapConfigWrapper cfg);
	void LoadConfig(String Name,DCMapConfigWrapper cfg);
	void Clear();
        RestoreState():top_item(0){Clear();}
};


class TFleetSideFrame : public TBaseModuleFrame
{
	friend class TItemData;
__published:	// IDE-managed Components
	TsDragBar *DragBar;
	TsSplitter *sSplitter1;
	TsPanel *BottomPanel;
	TsLabel *sLabel1;
	TsLabel *FleetNameLabel;
	TsLabel *sLabel3;
	TsLabel *FleetOwnerLabel;
	TsLabel *sLabel5;
	TsLabel *sLabel6;
	TsLabel *FleetSpeedLabel;
	TsLabel *FleetRangeLabel;
	TsLabel *FleetInfoLabel;
	TsLabel *UnitNameLabel;
	TsBevel *sBevel1;
	TsButton *SendFleetBtn;
	TsButton *CancelJumpBtn;
	TsTreeView *FleetView;
	TsTreeView *PlanetFleetView;
	TPopupMenu *FleetPopup;
	TMenuItem *FlyToMenu;
	TMenuItem *DisbandMenu;
	TMenuItem *CreateFleetFromSelMenu;
	TMenuItem *CancelJumpMenu;
	TMenuItem *MoveToFleetMenu;
	TMenuItem *MoveToNewFleetMenu;
	TMenuItem *RenameMenu;
	TPopupMenu *QueuePopup;
	TMenuItem *BuildingsMenu;
	TMenuItem *UnitsMenu;
	TMenuItem *SetIconMenu;
	TMenuItem *SetEmptyIconMenu;
	TMenuItem *PropertiesMenu;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *HideFleetMenu;
	TMenuItem *CancelHideFleetMenu;
	TMenuItem *UnhideFleetMenu;
	TMenuItem *CancelUnhideFleetMenu;
	TMenuItem *UnpackMenu;
	TMenuItem *CancelCargoLoadMenu;
	void __fastcall FleetViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FleetViewStartDrag(TObject *Sender, TDragObject *&DragObject);
	void __fastcall FleetViewDblClick(TObject *Sender);
	void __fastcall SendFleetBtnMouseLeave(TObject *Sender);
	void __fastcall CancelJumpBtnClick(TObject *Sender);
	void __fastcall SendFleetBtnClick(TObject *Sender);
	void __fastcall FleetViewKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall DragBarMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall DragBarMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall FleetViewExpanding(TObject *Sender, TTreeNode *Node,
          bool &AllowExpansion);
	void __fastcall FleetViewChanging(TObject *Sender, TTreeNode *Node,
          bool &AllowChange);
	void __fastcall FleetViewMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FleetViewClick(TObject *Sender);
	void __fastcall FleetViewDeletion(TObject *Sender, TTreeNode *Node);
	void __fastcall FleetViewDragOver(TObject *Sender, TObject *Source, int X, int Y,
          TDragState State, bool &Accept);
	void __fastcall FleetViewDragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall FleetViewContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall FlyToMenuClick(TObject *Sender);
	void __fastcall CancelJumpMenuClick(TObject *Sender);
	void __fastcall CreateFleetFromSelMenuClick(TObject *Sender);
	void __fastcall DisbandMenuClick(TObject *Sender);
	void __fastcall MoveToFleetMenuItemClick(TObject *Sender);
	void __fastcall FleetViewEditing(TObject *Sender, TTreeNode *Node, bool &AllowEdit);
	void __fastcall FleetViewEdited(TObject *Sender, TTreeNode *Node, UnicodeString &S);
	void __fastcall RenameMenuClick(TObject *Sender);
        void __fastcall QueueAddClick(TObject *Sender);
	void __fastcall SetIconMenuClick(TObject *Sender);
	void __fastcall OnSetIcon(TObject *Sender);
	void __fastcall FleetViewAdvancedCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node,
          TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages,
          bool &DefaultDraw);
	void __fastcall PropertiesMenuClick(TObject *Sender);
	void __fastcall FleetViewCollapsing(TObject *Sender, TTreeNode *Node, bool &AllowCollapse);
	void __fastcall UnitActionMenuClick(TObject *Sender);
	void __fastcall UnitActionCancelMenuClick(TObject *Sender);
	void __fastcall HideFleetMenuClick(TObject *Sender);
	void __fastcall FleetViewChange(TObject *Sender, TTreeNode *Node);
	void __fastcall UnpackMenuClick(TObject *Sender);
	void __fastcall CancelCargoLoadMenuClick(TObject *Sender);

private:	// User declarations
	int m_planetX;
	int m_planetY;

	DCMapDataReference CurrentFleet;

	RestoreState m_RestFleets;
	RestoreState m_RestPlanet;

	void UpdateRest(TsTreeView* pView,RestoreState* rest);

	int MoverLastMouse;
	int PlanetViewHeight;

	TTreeNode* m_pLastHintNode;

	TSelectedItems* m_pSelection;
	TsTreeView* m_pCurrentView;
	bool m_bEditMode;
	bool m_bBlockUpdate;
	bool m_bNeedUpdate;

private:
	void DeleteItems(TSelectedItems* items);
	void CreateNewFleet(TSelectedItems* items);
	bool MakeProjectsMenu(TSelectedItems* Sel);
	int MakeActionMenu(TSelectedItems* Sel);
	int MakeMoveToFleetMenu(TSelectedItems* Sel);
	int SendFleetsTo(TSelectedItems* pSel,int x,int y);
	int CancelFleetsJump(TSelectedItems* pSel);
public:
        IDCMapCore* core(){return m_pCore;}
public:		// User declarations
	IDCMapFleetsDataStoragePtr Fleets;
	IDCMapBuildingsDataStoragePtr Units;
	IDCMapPlanetDataStoragePtr Planets;
	IDCMapProjectDataStoragePtr Projects;
	IDCMapPlayersDataStoragePtr Players;
	IDCMapIconsDataStoragePtr Icons;
	IDCMapComponentsDataStoragePtr Components;
	IDCMapProjectActsDataStoragePtr ProjectActs;
	IDCMapActionsRequestedDataStoragePtr ActReq;
	IDCMapUnitTagsDataStoragePtr UnitTags;

	IDCMapPlanetManager* m_pPlanetMan;
	IDCMapFleetManager*  m_pFleetMan;
	IDCMapPlanetCalculator* m_pCalc;
        IDCMapPlanetContextManager* m_pPlanetContextMan;

	TIconsList* m_pIconsList;

	TDCMapUIContext* m_pPlayerContext;
	TDCMapUIContext* m_pPlanetContext;

	bool ShowBuildings;

	void UpdateSel(TItemData* Data,TsTreeView* pView);
	void UpdateFleetInfo(DCMapDataReference const& Fleet,DCMapDataReference const* Unit=0);

	__fastcall TFleetSideFrame(TComponent* Owner);
        __fastcall ~TFleetSideFrame();
	bool OnInit();
	void UpdateFleets();
	void UpdateFleets(DCMapDataReference const& RRef,TsTreeView* pView,RestoreState* rest);
	void UpdatePlanetFleets(dcmapDBREF refPlanet,TsTreeView* pView, RestoreState* rest);
	int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);

	void LoadConfig(DCMapConfigWrapper cfg);
	void SaveConfig(DCMapConfigWrapper cfg);

	void OnShow();
	void SetupColors();
};
//---------------------------------------------------------------------------
extern PACKAGE TFleetSideFrame *FleetSideFrame;
//---------------------------------------------------------------------------
#endif
