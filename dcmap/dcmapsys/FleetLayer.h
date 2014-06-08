#pragma once
#include "../dcmapsys.h"

enum EFleetColors
{
	EFC_START,EFC_1TURN,EFC_2TURN,EFC_3TURN,
	EFC_NDIST,EFC_MAX_DIST,
	EFC_FLEET_MY,EFC_FLEET_IN_TRANSIT,
	EFC_FLEET_UNDEF,EFC_FLEET_NEUTRAL,EFC_FLEET_ALLY,EFC_FLEET_ENEMY,
	EFC_FLEET_INCOMING,
	EFC_BRUSHES,
	EFC_OPEN_BORDER = EFC_BRUSHES,
	EFC_COUNT
};

class CFleetLayer :	public IDCMapFleetLayer
{
	IDCMapFleetManager::FleetJumpInfo m_JumpInfo;
	bool m_bNoGdiPlus;

	IDCMapFleetManager* m_pFleetMan;
	IDCMapPlanetDataStoragePtr m_pPlanets;
	IDCMapFleetsDataStoragePtr m_pFleets;
	int m_iCellSize;
	int m_iLeft;
	int m_iTop;
	int m_iWidth;
	int m_iHeight;
	IDCMapLayeredMapView* m_pView;
	bool m_bHilightJump;

	IDCMapCore* m_pCore;
	IDCMapWorkspace* m_pWorkspace;


	dcmapCOLOR m_cColors[EFC_COUNT];

	dcmapHBRUSH m_clBrushes[EFC_BRUSHES];
	dcmapHPEN m_clPens[EFC_COUNT-EFC_BRUSHES];

	dcmapHPEN m_hpenBorder;
	dcmapHPEN m_hpenBorderSmall;

public:

	CFleetLayer(void);
	virtual ~CFleetLayer(void);

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapLayer);
		DCMAP_MODULE_SUPPORTS(IDCMapFleetLayer);
		return 0;
	}

	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);

	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	bool DCMAPAPI InitLayer(IDCMapLayeredMapView* pView);

	void DCMAPAPI Render(dcmapHDC dc,dcmapRECT const& rcBound, dcmapRECT const& rcMapCoord,int CellSize,int iFlags);
	bool DCMAPAPI ActualizeData();
	bool DCMAPAPI EditLayer();

	bool DCMAPAPI DragOver(int x,int y,int State,IDCMapDragObject* pDragInfo,IDCMapModule* pModule);
	bool DCMAPAPI DragDrop(int x,int y,IDCMapDragObject* pDragInfo,IDCMapModule* pModule);

	dcmapCSTR DCMAPAPI GetLayerName();
	void DCMAPAPI PreRender(dcmapHDC dc,dcmapRECT const& rcBound, dcmapRECT const& rcWnd,int CellSize,int iFlags);
	void UpdateConfig();
	int DCMAPAPI PrepareRender(dcmapRECT const& rcBound, dcmapRECT const& rcWnd,int CellSize,int iFlags);
};
void DrawActiveTrasserEx(HDC dc,int fx,int fy,int tx,int ty,gsys::grect const& bound,float dist,int iTurns,dcmapDBREF const& ref);