#pragma once
//#include "platform.h"

#include "../dcmapsys.h"

struct IconData 
{
	dcmapHBITMAP hbmp;
	dcmapHBITMAP hmsk;
	int w;
	int h;

	dcmapHBITMAP b_hbmp;
	dcmapHBITMAP b_hmsk;
	int b_w;
	int b_h;

	dcmapCOLORREF tr;
	bool trns;
	IconData()
	{
		w=0;
		h=0;
		b_w=0;
		b_h=0;
		hbmp=0;
		hmsk=0;
		b_hbmp=0;
		b_hmsk=0;
		trns=false;
		tr=0;
	}
};

class CDCMapNavView : public IDCMapLayeredMapView
{
	friend class CGeoDataProcessorFilter;
	void WrapCoords();

	char const* DCMAPAPI ModuleID(){return "MapView";}

protected:
	IDCMapWorkspace* m_pWorkspace;
	IDCMapCore* m_pCore;
	IDCMapPlanetDataStorage* m_pPlanets;
protected: 
	dcmapPOINT m_ptLastMouse;

	dcmapPOINT m_ptPointedCell;
	dcmapPOINT m_ptCurrentCell;

protected: // some settings
	bool m_bClassicHLine;
	bool m_bHighQuality;

	bool m_bSwapButtons;
	bool m_bSelectEmptySpace;
    bool m_bReadOnly;

	int m_iMinIconsCellSize;

	bool m_bRightClickSelect;

	int  m_iToolTipDelay;
	bool m_bEnableToolTip;

protected: // Master parametrs
	int  m_iScale;
	int  m_iTop;
	int  m_iLeft;


protected: // Depended params

	int m_iTextOffset;

	int m_iVFontSize;
	int m_iHFontSize;

	int m_iVLineMark;
	int m_iHLineMark;
	bool m_bHRotateMark;
	bool m_bVRotateMark;

	int m_iGrid1;
	int m_iGrid2;
	int m_iGrid3;
	int m_iGrid4;

	int m_iVGrid1;
	int m_iVGrid2;
	int m_iVGrid3;

	int m_iHGrid1;
	int m_iHGrid2;
	int m_iHGrid3;

	int  m_iCellSize;
	int  m_iWidth;
	int  m_iHeight;

	int m_iVLineSize;
	int m_iHLineSize;

	int m_iScrWidth;
	int m_iScrHeight;

	dcmapRECT m_rcMap;

	int m_iDisplayHeight;
	int m_iDisplayWidth;

	// Common GDI Objects

	dcmapHPEN m_hpnLineGrid1;
	dcmapHPEN m_hpnLineGrid2;
	dcmapHPEN m_hpnLineGrid3;
	dcmapHPEN m_hpnLineGrid4;

	dcmapHPEN m_hpnLineBorder;
	dcmapHPEN m_hpnInactiveLineBorder;

	dcmapHPEN m_hpnSelection;
	dcmapHPEN m_hpnSelection2;

	dcmapHPEN m_hpnGeoGood;
	dcmapHPEN m_hpnGeoBad;

	dcmapHBRUSH m_hbrGeoGood;
	dcmapHBRUSH m_hbrGeoBad;

	dcmapCOLORREF m_clGeoGoodBorder;
	dcmapCOLORREF m_clGeoGoodFill;

	dcmapCOLORREF m_clGeoBadBorder;
	dcmapCOLORREF m_clGeoBadFill;

	dcmapCOLORREF m_clGrid1;
	dcmapCOLORREF m_clGrid2;
	dcmapCOLORREF m_clGrid3;
	dcmapCOLORREF m_clGrid4;
	dcmapCOLORREF m_clBorder;
	dcmapCOLORREF m_clInactiveBorder;
	dcmapCOLORREF m_clSelection;
	dcmapCOLORREF m_clSelection2;

	dcmapCOLORREF m_clLineBackground;
	dcmapCOLORREF m_clMapBackground;

	dcmapCOLORREF m_clText;
	dcmapCOLORREF m_clPointedText;

	dcmapHBRUSH m_hbrLineBackground;
	dcmapHBRUSH m_hbrMapBackground;

	dcmapHFONT m_hfntHLineText;
	dcmapHFONT m_hfntVLineText;

protected: // selection

	int m_iSelX;
	int m_iSelY;
	int m_iSelType;

	void UpdateSelection();
	bool ShowPlace(int cx,int cy,bool center=false);
    bool CellVisible(int x,int y);



protected: // modes
	
	bool m_bDragMode;
	bool m_bSpaceDragMode;
    bool m_bLeftDragMode;

	bool m_bActive;
	bool m_bRenderReady;
	bool m_bWndCreated;

protected: // ui elements
	IDCMapUIMenu* m_pPlanetPopup;
	IDCMapUIMenu* m_pCellPopup;
protected: // planets

	IDCMapPlanetManager* m_pPlanetMan;
	IDCMapPlanetDataProcessor* m_pDataProcessor;
	IDCMapFleetManager* m_pFleetMan;

protected: // geo
	DCMapDataSourcePtr<IDCMapGeoDataStorage> m_pGeo;
	IDCMapGeoValueCalculator* m_pGeoValue;
	IDCMapPlanetContextManager* m_pPlanetContextMan;
	std::vector<IDCMapGeoDataFilter*> m_vecGeoFilters;

protected: // layers
	dcmapQWORD m_LayersState;
	IDCMapLayersDataStorage*   m_pLayers;

	enum InternalLayers{BigGridLayer=0,GridLayer=1,GeoLayer=2,PlanetLayer=3,IconsLayer=4,LastLayer=4};

	struct LayerDesc
	{
		IDCMapLayer* m_pLayerIface;
		bool bInternal;
		bool bPlanetary;
		bool bUser;
		dcmapDBREF LayerRef;
		string strName;
		string strLyaerId;
		wstring strInternalName;
		wstring strCaption;
		bool bVisible;
		bool bActive;
		bool bCanBeActivated;
		bool bHideFromList;

		LayerDesc():m_pLayerIface(0),bInternal(0),bPlanetary(0),bUser(0),bVisible(0),
			bActive(0),bCanBeActivated(0),bHideFromList(0){};
	};
	typedef std::vector<LayerDesc> VecLayers;
	VecLayers m_Layers;
	void ActualizeLayers();

	void ActualizeData();
	void LoadPrefs(IDCMapConfig* cfg=0);
	void PasteInfo();

	IDCMapLayer* m_pActiveLayer;
	int m_iActivelayer;
	bool m_bLayerVisiblityChanged;

	void Zoom(int rel,int sile,dcmapSHORT x ,dcmapSHORT y);

	void DoSetFocus();

public:
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData);

	tagWNDCLASSW* DCMAPAPI RegisterWndClass();
	void* DCMAPAPI BeforeCreate(dcmapHWND hwndParent,dcmapHWND hwndMain);
	void DCMAPAPI AfterCreate(dcmapHWND wnd,dcmapHWND hwndParent,dcmapHWND hwndMain);
	void DCMAPAPI OnResize(int w,int h);

	int DCMAPAPI ActiveLayer(){return m_iActivelayer;}

	IDCMapLayer* DCMAPAPI LayerModule(uint n)
		{if(n>=m_Layers.size())return 0;return m_Layers[n].m_pLayerIface;}

	dcmapDBREF const* DCMAPAPI LayerDataID(dcmapUINT n){if(n>=m_Layers.size())return 0;return &m_Layers[n].LayerRef;}

	int DCMAPAPI NumLayers(){return m_Layers.size();}
	dcmapCSTR DCMAPAPI LayerModuleName(uint n){if(n>=m_Layers.size())return "";return m_Layers[n].strName.c_str();}
	dcmapDCSTR DCMAPAPI LayerName(uint n){if(n>=m_Layers.size())return L"";return m_Layers[n].strInternalName.c_str();}
	dcmapDCSTR DCMAPAPI LayerCaption(uint n){if(n>=m_Layers.size())return L"";return m_Layers[n].strCaption.c_str();}
	bool DCMAPAPI LayerVisible(uint n){if(n>=m_Layers.size())return false;return m_Layers[n].bVisible;}
	bool DCMAPAPI LayerListed(uint n){if(n>=m_Layers.size())return false;return !m_Layers[n].bHideFromList;}

	void DCMAPAPI SetLayerVisiblity(uint n,bool Visible)
	{
		if(n>=m_Layers.size())return;
		if(m_Layers[n].bVisible != Visible)
		{
			m_Layers[n].bVisible = Visible;
			m_bLayerVisiblityChanged=true;
		}
	}
	void DCMAPAPI UpdateLayers();
	bool DCMAPAPI SetActiveLayer(dcmapUINT n);
	bool DCMAPAPI EditLayer(dcmapUINT n);

	bool DCMAPAPI DragOver(int x,int y,int State,IDCMapDragObject* pDragInfo,IDCMapModule* pModule);
	bool DCMAPAPI DragDrop(int x,int y,IDCMapDragObject* pDragInfo,IDCMapModule* pModule);

	IDCMapLayer* DCMAPAPI LayerInterface(dcmapUINT n){if(n>=m_Layers.size())return 0;return m_Layers[n].m_pLayerIface;}

protected:

	bool m_bHasMouse;

	//dcmapHBITMAP m_hPreBmp;
	//dcmapHBITMAP m_hBmp;
	//dcmapHDC m_hdcBmp;
	int m_iBmpWidth;
	int m_iBmpHeight;

	void OnPaint(dcmapHDC dc);

	void OnLButtonDoubleClick(dcmapSHORT x,dcmapSHORT y);
	void OnRButtonDoubleClick(dcmapSHORT x,dcmapSHORT y);

	void OnRButtonDown(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y);
	void OnLButtonDown(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y);
	void OnMButtonDown(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y);

	void OnRButtonUp(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y);
	void OnLButtonUp(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y);
	void OnMButtonUp(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y);

	void OnSetCursor();

	void OnActivate();
	void OnDeactivate();

	void OnMouseMove(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y);
	void OnWndMouseWheel(dcmapWORD fwKeys,dcmapSHORT zDelta, dcmapSHORT x,dcmapSHORT y);

	void OnMouseEnter();
	void OnMouseExit();

	void OnKeyDown(dcmapDWORD key,dcmapDWORD opt);
	void OnKeyUp(dcmapDWORD key,dcmapDWORD opt);

	void ScrollMap(dcmapPOINT const& ptUpdate);

	void DCMAPAPI UpdateMapRect(const dcmapRECT* rc);

	void DeletePlanet();

protected:
	dcmapHWND m_hwndParent;
	dcmapHWND m_hwndMain;
	dcmapHWND m_hWnd;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Render();
	void RenderVLine(int from,int to);
	void RenderHLine(int from,int to);
	void RenderMap(dcmapRECT const* rcArea);
	void PreRenderMap(dcmapRECT const* rcArea);
	void RenderBorder();
	void RenderFocus(dcmapHDC dc);

	void RenderGeo(DCMapPlanetParamInfo const* pinf,int x,int y);

	void DrawIcon(IconData const &ico,int x,int y,bool bounded,bool top,bool left,int& w,int &h);
	void RenderIcons(DCMapPlanetDisplayInfo* pinf,int x,int y);

	void RenderPlanet(DCMapPlanetDisplayInfo* pinf,int x,int y);
	void RenderPlanetHQ(DCMapPlanetDisplayInfo* pinf,int x,int y);
	void SetupParams();
	void SetupColors();

	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);

	void  LayersMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam);
	void SaveLayersCfg();

protected:
	UINT_PTR m_TimerID;
	dcmapRECT m_rcHintArea;
	wstring m_strHint;
	dcmapPOINT m_ptHintPlanet;
	void OnHintTimer();
	void DCMAPAPI SetupHint(wchar_t const* string,dcmapRECT const* area);
	void DCMAPAPI ResetHint();
	const wchar_t* GetHint();
	void SetupHint(int x, int y);

	std::vector<IDCMapPlanetHintSource*> m_vecHints;

public:
	CDCMapNavView(void);
public:
	virtual ~CDCMapNavView(void);
public:
	bool DCMAPAPI StartAnim(dcmapRECT const* rcPos,dcmapHDC* pDC,void* pGDIP);
	void DCMAPAPI PresentAnim();
	void DCMAPAPI EndAnim();
private:
	int m_bAnimating;
	dcmapRECT m_rcAnim;
	dcmapRECT m_rcOldAnim;
public:
	void DCMAPAPI LoadConfig(IDCMapConfig* pConfig);
	void DCMAPAPI SaveConfig(IDCMapConfig* pConfig);

	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);

	dcmapHWND DCMAPAPI Create(dcmapHWND hwndMain,dcmapHWND hwndParent,dcmapRECT const* rcClient);
	void DCMAPAPI Destroy();
	void DCMAPAPI OnResize(dcmapRECT const* rcClient);
	static void Register();

	bool DCMAPAPI OnMouseWheel(int delta, int x,int y);

	void DCMAPAPI Activate(bool Active);

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapView);
		DCMAP_MODULE_SUPPORTS(IDCMapLayeredMapView);
		return 0;
	}
};
