#pragma once
#include "platform.h"
#include "../dcmapsys.h"
#include "BaseDataStorage.h"

class CGraphicsDataStorage;

typedef vector<dcmapDBREF> DataRefVector;

class CGraphicLayer : public IDCMapGraphicLayer
{
	IDCMapCore * m_pCore;

	CGraphicsDataStorage* m_pGraphics;
	typedef gsys::raw_data_buffer<Gdiplus::Point> VecPoints;

	VecPoints m_CurrentPoly;
	EDCMapGraphicsType m_CurrentType;

	IDCMapModule* m_pDrawMan;

	bool m_bDrawMode;
	bool m_bPlaceTextMode;
	bool m_bActive;
	IDCMapLayeredMapView* m_pView;

	dcmapDBREF m_refParent;

	int m_iCellSize;

	dcmapQWORD m_GraphicsState;

	void BuildRegions();

	struct GraphDef
	{
		dcmapDBREF ref;
		GraphDef():path(0),text(0),reg(0){}
		GraphicsPath* path;
		Region* reg;
		dcmapPOINT ptBase;
		dcmapRECT  rcBound;
		Color clLine;
		Color clFill;
		Color clText;
		dcmapBYTE  line_size;
		dcmapBYTE  line_type;
		dcmapBYTE  fill_mode;
		dcmapDCSTR text;
		PointF ptText;
		int iTextLen;
		float fTextSize;
		EDCMapGraphicsType type;
		bool x_overwrap;
		bool y_overwrap;
	};

	typedef std::list<GraphDef> RegionsList;
	RegionsList m_Regions;

	void RenderElement(GraphDef& def,Graphics& graphics,float line_scale,float CellSize);
	void RenderGeometry(Graphics& graphics,dcmapRECT const& rcBound,Matrix &mat,float line_scale,float CellSize,bool ox,bool oy);
	void SaveGraph();

	DCMapGraphicsInfo m_CurrentDrawInfo;
	GraphDef m_CurrentDef;
	void MakeDef(DCMapGraphicsInfo const& gr,GraphDef &def,Point* lines,int num);

public:
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CGraphicLayer();}
	CGraphicLayer(void);
	virtual ~CGraphicLayer(void);
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	bool DCMAPAPI InitUserLayer(dcmapDBREF const& ref);
	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapLayer);
		DCMAP_MODULE_SUPPORTS(IDCMapGraphicLayer);
		return 0;
	}
	void DCMAPAPI BeginDraw(EDCMapGraphicsType type,IDCMapModule* pManager);
	void DCMAPAPI EndDraw();
	void DCMAPAPI Undo();

	void DCMAPAPI PlaceText(bool place)
	{
		if(m_bDrawMode)
		{
			m_bPlaceTextMode=place;
		}
	}

	void DCMAPAPI CancelDraw();
	DCMapGraphicsInfo* DCMAPAPI GetGraphicsInfo();
	void DCMAPAPI UpdateGraphicsInfo(DCMapGraphicsInfo* pinf);

	void DCMAPAPI Render(dcmapHDC dc,dcmapRECT const& rcBound, dcmapRECT const& rcMapCoord,int CellSize,int iFlags);

	bool DCMAPAPI ActualizeData();
	bool DCMAPAPI EditLayer();

    bool DCMAPAPI TestPoint(float x,float y,IDCMapVarValue* pResult);

	bool DCMAPAPI DragOver(int x,int y,int State,IDCMapDragObject* pDragInfo,IDCMapModule* pModule);
	bool DCMAPAPI DragDrop(int x,int y,IDCMapDragObject* pDragInfo,IDCMapModule* pModule);

	dcmapCSTR DCMAPAPI GetLayerName(){return "graphics";}
	dcmapDCSTR DCMAPAPI GetLayerCaption(){return L"";}

};
