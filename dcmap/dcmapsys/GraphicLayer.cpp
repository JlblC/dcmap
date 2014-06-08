#include "StdAfx.h"
#include "platform.h"
using namespace Gdiplus;

#include "GraphicLayer.h"
#include "utils.h"

class CLayersDataStorage : public CBaseNamedDataStorage<DCMapLayerInfo,IDCMapLayersDataStorage>
{
	typedef CBaseNamedDataStorage<DCMapLayerInfo,IDCMapLayersDataStorage> parent;
public:
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CLayersDataStorage();}
	CLayersDataStorage()
	{
		m_BR.RegisterString("module",&value_type::module,DCMDB_LAYER_MODULE);
		m_BR.RegisterFlag("hidden",&value_type::flags,DCMAP_LF_Hidden,DCMDB_LAYER_HIDDEN);
	}

	char const* DCMAPAPI Name(){return "layers";}

	void DCMAPAPI FillDefaults(DCMapLayerInfo* pInf)
	{
		parent::FillDefaults(pInf);
		strcpy(pInf->module,"GraphicsLayer");
	}

};

class CGraphicsDataStorage : public CLocalyNamedChildTableDataStorage<DCMapGraphicsInfo,IDCMapGraphicsDataStorage>
{
public:
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CGraphicsDataStorage();}

	CGraphicsDataStorage():CLocalyNamedChildTableDataStorage("layers","layer")
	{
		m_BR.RegisterWideString("text",&value_type::text,DCMDB_GRAPHICS_TEXT);

		m_BR.RegisterWideString("notes",&value_type::notes,DCMDB_GRAPHICS_NOTES);
        //m_BR.RegisterWideString("exinfo",&value_type::exinfo,DCMDB_GRAPHICS_HTMLNOTE);

		m_BR.RegisterInt("x",&value_type::x,DCMDB_GRAPHICS_X);
		m_BR.RegisterInt("y",&value_type::y,DCMDB_GRAPHICS_Y);

		m_BR.RegisterInt("line_color",&value_type::line_color,DCMDB_GRAPHICS_LINE_COLOR);
		m_BR.RegisterInt("fill_color",&value_type::fill_color,DCMDB_GRAPHICS_FILL_COLOR);

		m_BR.RegisterInt("line_size",&value_type::line_size,DCMDB_GRAPHICS_LINE_SIZE);
		m_BR.RegisterInt("line_type",&value_type::line_type,DCMDB_GRAPHICS_LINE_TYPE);
		m_BR.RegisterInt("fill_mode",&value_type::fill_mode,DCMDB_GRAPHICS_FILL_MODE);
		m_BR.RegisterInt("type",&value_type::type,DCMDB_GRAPHICS_TYPE);

		m_BR.RegisterInt("coords",&value_type::coords,DCMDB_GRAPHICS_COORDS);

		m_BR.RegisterInt("visible",&value_type::visible,DCMDB_GRAPHICS_VISIBLE);

		m_BR.RegisterInt("text_color",&value_type::text_color,DCMDB_GRAPHICS_TEXT_COLOR);
		m_BR.RegisterInt("text_size",&value_type::text_size,DCMDB_GRAPHICS_TEXT_SIZE);
		m_BR.RegisterInt("text_type",&value_type::text_type,DCMDB_GRAPHICS_TEXT_TYPE);
		m_BR.RegisterInt("radius",&value_type::radius,DCMDB_GRAPHICS_RADIUS);
	}
	void DCMAPAPI FillDefaults(value_type* pinf)
	{
		ZeroMemory(pinf,sizeof(value_type));

		pinf->fill_color = Color::MakeARGB(30,0,255,0);
		pinf->line_color = Color::MakeARGB(255,255,255,255);
		pinf->text_color = Color::MakeARGB(255,255,255,255);

		pinf->text_size=2;
		pinf->line_size=2;
		pinf->fill_mode=1;

		pinf->radius=5;

		pinf->visible = true; 
	}

	/*const char* DCMAPAPI GetName()
	{
		return m_pCurrentRecord->name;
	};
	void DCMAPAPI SetName(const char* szName)
	{
		strncpy(m_temp.name,szName,value_type::name_len);
	}*/
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapDataStorage);
		DCMAP_MODULE_SUPPORTS(IDCMapGraphicsDataStorage);
		DCMAP_MODULE_SUPPORTS(CGraphicsDataStorage);
		return 0;
	}

	virtual dcmapCSTR BlobToStr(int field,dcmapDBREF data)
	{
		static string convString;
		convString = ToStr(BlobToWStr(field,data));
		return convString.c_str();
	}

	virtual bool StrToBlob(int field,dcmapDBREF & data,dcmapCSTR str)
	{
		return WStrToBlob(field,data,StrC2D(str).c_str());
	}

	bool WStrToBlob(int field,dcmapDBREF & data,dcmapWCSTR str)
	{
		dcmapPOINT* buf;
		int n = dcmapStrToCoordArray(str,&buf,false);
		return SetBlob(field,(dcmapBYTE*)buf,n*sizeof(dcmapPOINT));
	}

	dcmapWCSTR BlobToWStr(int field,dcmapDBREF data)
	{
		if(!data.Valid())return L"";
		dcmapPOINT* dta;
		int n = m_CLOB.GetValue(data,(dcmapBYTE**)&dta);
		if(!n)return L"";
		n /= sizeof(dcmapPOINT);
		return dcmapCoordArrayToStr(dta,n);
	}

	/*bool StrToBlob(int field,dcmapDBREF & data,char const* str)
	{
		gsys::raw_data_buffer<dcmapPOINT> buf;
		std::istringstream is(str);
		dcmapPOINT pt;
		// Read x
		string tmp;
		while(!is.fail())
		{
			is >> ws >> gsys::istream_numb_keyword(tmp) >> ws;
			if(!gsys::lex_convert(pt.x,tmp))return false;
			if(!gsys::char_is_numb(is.peek()))is.ignore();
			is >> ws >> gsys::istream_numb_keyword(tmp) >> ws;
			if(!gsys::lex_convert(pt.y,tmp))return false;
			buf.push_back(pt);
			if(is.fail())break;
			if(!gsys::char_is_numb(is.peek()))is.ignore();
		}
		return SetBlob(field,(dcmapBYTE*)buf.begin(),buf.raw_size());
	}

	string BlobStr;
	char const* BlobToStr(int field,dcmapDBREF data)
	{
		if(!data.Valid())return "";
		dcmapPOINT* dta;
		int n = m_CLOB.GetValue(data,(dcmapBYTE**)&dta);
		if(!n)return "";
		n /= sizeof(dcmapPOINT);
		BlobStr.clear();
		for(int i=0;i<n;i++)
		{
			BlobStr += gsys::to_str(dta[i].x)+":"+ gsys::to_str(dta[i].y);
			if(i<n-1)BlobStr += "-";
		}
		return BlobStr.c_str();
	}*/

	dcmapCSTR DCMAPAPI Name(){return "graphics";}
};


CGraphicLayer::CGraphicLayer(void)
{
	m_bDrawMode = false;
	m_pView = 0;
	m_bActive = false;
	m_bPlaceTextMode = false;
	m_pDrawMan = 0;
}

CGraphicLayer::~CGraphicLayer(void)
{
}

bool  DCMAPAPI CGraphicLayer::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	m_pCore = core;
	if(m_pCore->GetSystemOptions()->bNoGdiPlus)return false;

	m_pGraphics = core->GetDataSourceTyped<CGraphicsDataStorage>("graphics");
	if(!m_pGraphics)return false;

	m_GraphicsState = m_pGraphics->GetStateIndex();

	return true;
}

static REAL dashVals[3][6] = {
	{4.0f,1.0f, 4.0f,1.0f, 4.0f,1.0f},
	{2.0f,3.0f, 2.0f,3.0f, 2.0f,3.0f},
	{4.0f,2.0f, 1.0f,2.0f, 1.0f,2.0f}};


void CGraphicLayer::RenderElement(GraphDef& def,Graphics& graphics,float line_scale,float CellSize)
{
	if(def.fill_mode)
	{
		SolidBrush  brush(def.clFill);
		if(def.type == DCMAP_GRAPHICS_POLYGON &&  def.path)
		{
			graphics.FillPath(&brush,def.path);
		}
		else if(def.type == DCMAP_GRAPHICS_ZONES && def.reg)
		{
			graphics.FillRegion(&brush,def.reg);
		}
	}

	if(def.line_size && def.path)
	{
		Pen pen(def.clLine);

		if(def.line_type)
		{
			if(def.line_size == 1)
			{
				REAL dVals[6];
				for(int i=0;i<6;i++)dVals[i]=dashVals[def.line_type-1][i]*line_scale*2;
				pen.SetDashPattern(dVals,6);
			}
			else pen.SetDashPattern(dashVals[def.line_type-1],6);
		}
		else
			pen.SetDashStyle(DashStyleSolid);

		pen.SetWidth(def.line_size*line_scale);
		graphics.DrawPath(&pen,def.path);
	}
	if(def.text && def.fTextSize*CellSize > 4 )
	{
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);

		SolidBrush  textbrush(def.clText);
		Font myFont(L"Arial", def.fTextSize);
		graphics.DrawString(def.text,def.iTextLen,&myFont,def.ptText,&format,&textbrush);
	}
}

void CGraphicLayer::RenderGeometry(Graphics& graphics,dcmapRECT const& rcBound,Matrix &mat,float line_scale,float CellSize,bool ox,bool oy)
{
	Pen         pen(Color(255, 255, 255, 255),2);
	SolidBrush  brush(Color(30, 0, 255, 0));
	SolidBrush  textbrush(Color(30, 0, 255, 0));
	StringFormat format;

	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);

	graphics.SetTransform(&mat);

	_foreach(RegionsList,m_Regions,it)
	{
		GraphDef& def = *it;

		if(ox && !def.x_overwrap)continue; 
		if(oy && !def.y_overwrap)continue; 

		Point pt[2];
		pt[0].X = def.rcBound.left;
		pt[0].Y = def.rcBound.top;
		pt[1].X = def.rcBound.right;
		pt[1].Y = def.rcBound.bottom;
		mat.TransformPoints(pt,2);

		if(pt[0].X > rcBound.right)continue; 
		if(pt[1].X < rcBound.left)continue; 

		if(pt[0].Y > rcBound.bottom)continue; 
		if(pt[1].Y < rcBound.top)continue; 

		RenderElement(def,graphics,line_scale,CellSize);
	}

	if(m_bDrawMode) // render current drawing poly
	{
		RenderElement(m_CurrentDef,graphics,line_scale,CellSize);

		/*
		pen.SetColor(m_CurrentDef.clLine);
		pen.SetWidth(m_CurrentDef.line_size*line_scale);

		if(m_CurrentDrawInfo.line_type)
		{
			if(m_CurrentDrawInfo.line_size == 1)
			{
				REAL dVals[6];
				for(int i=0;i<6;i++)dVals[i]=dashVals[m_CurrentDrawInfo.line_type-1][i]*line_scale*2;
				pen.SetDashPattern(dVals,6);
			}
			else pen.SetDashPattern(dashVals[m_CurrentDrawInfo.line_type-1],6);
		}
		else pen.SetDashStyle(DashStyleSolid);

		if(m_CurrentPoly.size() >= 2)
		{
			Point* lines = new Point[m_CurrentPoly.size()];

			for(int i=0;i<m_CurrentPoly.size();i++)
			{
				lines[i] = m_CurrentPoly[i];
			}
			//mat.TransformPoints(lines,m_CurrentPoly.size());
			if(m_CurrentType == DCMAP_GRAPHICS_POLYGON)
			{
				graphics.DrawPolygon(&pen,lines,m_CurrentPoly.size());
			}
			else
			{
				graphics.DrawLines(&pen,lines,m_CurrentPoly.size());
			}
			delete lines;
		}
		*/
	}
}

void DCMAPAPI CGraphicLayer::Render(dcmapHDC dc,dcmapRECT const& rcBound, dcmapRECT const& rcMapCoord,int CellSize,int iFlags)
{
	if(m_GraphicsState != m_pGraphics->GetStateIndex())BuildRegions();

	Graphics graphics(dc);

	//graphics.SetSmoothingMode(SmoothingModeHighQuality);

	Pen pen(Color(255, 255, 255, 255),2);
	//graphics.DrawLine(&pen, 0, 0, 200, 100);

	Rect rc(rcBound.left,rcBound.top,rcBound.right-rcBound.left,rcBound.bottom-rcBound.top);
	graphics.SetClip(rc);

	// Render regions

	float line_scale = 1.0f/CellSize;

	Matrix mat;

	mat.Reset();
	mat.Translate(-rcMapCoord.left,-rcMapCoord.top, MatrixOrderAppend);
	mat.Scale(CellSize,CellSize, MatrixOrderAppend);
	mat.Translate(rcBound.left-CellSize/2,rcBound.top-CellSize/2, MatrixOrderAppend);

	RenderGeometry(graphics,rcBound,mat,line_scale,CellSize,false,false);

	mat.Reset();
	mat.Translate(0,-g_DCMapNumPlanets, MatrixOrderAppend);

	mat.Translate(-rcMapCoord.left,-rcMapCoord.top, MatrixOrderAppend);
	mat.Scale(CellSize,CellSize, MatrixOrderAppend);
	mat.Translate(rcBound.left-CellSize/2,rcBound.top-CellSize/2, MatrixOrderAppend);

	RenderGeometry(graphics,rcBound,mat,line_scale,CellSize,false,true);


	mat.Reset();
	mat.Translate(-g_DCMapNumPlanets,0, MatrixOrderAppend);

	mat.Translate(-rcMapCoord.left,-rcMapCoord.top, MatrixOrderAppend);
	mat.Scale(CellSize,CellSize, MatrixOrderAppend);
	mat.Translate(rcBound.left-CellSize/2,rcBound.top-CellSize/2, MatrixOrderAppend);

	RenderGeometry(graphics,rcBound,mat,line_scale,CellSize,true,false);

	mat.Reset();
	mat.Translate(-g_DCMapNumPlanets,-g_DCMapNumPlanets, MatrixOrderAppend);

	mat.Translate(-rcMapCoord.left,-rcMapCoord.top, MatrixOrderAppend);
	mat.Scale(CellSize,CellSize, MatrixOrderAppend);
	mat.Translate(rcBound.left-CellSize/2,rcBound.top-CellSize/2, MatrixOrderAppend);

	RenderGeometry(graphics,rcBound,mat,line_scale,CellSize,true,true);

}

DCMAP_REGISTER_STORAGE(CGraphicsDataStorage,"graphics")
DCMAP_REGISTER_STORAGE(CLayersDataStorage,"layers")
DCMAP_REGISTER(CGraphicLayer,"PUserLayer","GraphicsLayer")

bool DCMAPAPI CGraphicLayer::InitUserLayer(dcmapDBREF const& ref)
{
	m_refParent = ref;
	BuildRegions();
	return true;
}

bool operator == (Point const& p1,Point const& p2)
	{return p1.X == p2.X && p1.Y == p2.Y;}
bool operator != (Point const& p1,Point const& p2){return !(p1==p2);}

bool DCMAPAPI CGraphicLayer::ActualizeData()
{
	if(m_GraphicsState == m_pGraphics->GetStateIndex())return false;
	BuildRegions();
	return true;
}

bool DCMAPAPI CGraphicLayer::DragOver(int x,int y,int State,IDCMapDragObject* pDragInfo,IDCMapModule* pModule)
{
	return false;
}
bool DCMAPAPI CGraphicLayer::DragDrop(int x,int y,IDCMapDragObject* pDragInfo,IDCMapModule* pModule)
{
	return false;
}

int DCMAPAPI CGraphicLayer::OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
{
	switch(Msg)
	{
	case DCMM_MAP_CLICK:
		if(m_bDrawMode)
		{
			dcmapRECT past = m_CurrentDef.rcBound;

			int x = wParam;
			int y = lParam;
			if(m_bPlaceTextMode)
			{
				dcmapDCSTR name = m_pCore->GetCoreUI()->InputBox(L"Введите текст подписи",m_CurrentDrawInfo.text);
				if(name && name[0])
				{
					wcsncpy(m_CurrentDrawInfo.text,name,DCMapGraphicsInfo::text_len);
					m_CurrentDrawInfo.x = x;
					m_CurrentDrawInfo.y = y;
					m_CurrentDrawInfo.text_type = 1;
				}
				else m_CurrentDrawInfo.text_type = 0;
				m_bPlaceTextMode = false;
				if(m_pDrawMan)m_pDrawMan->OnMessage(DCMM_GRAPHICS_TEXT_PLACED,MAKEWPARAM(x,y),(dcmapLPARAM)m_CurrentDrawInfo.text,this);
			}
			else
			{
				if(m_CurrentPoly.size())
				{
					if(m_CurrentPoly.back() == Point(x,y))break;

					Point & pre = m_CurrentPoly.back();
					if( abs(x-pre.X) > abs((x+g_DCMapNumPlanets)-pre.X)) 
						x += g_DCMapNumPlanets;
					if( abs(y-pre.Y) > abs((y+g_DCMapNumPlanets)-pre.Y)) 
						y += g_DCMapNumPlanets;

					if( abs(x-pre.X) > abs((x-g_DCMapNumPlanets)-pre.X)) 
						x -= g_DCMapNumPlanets;
					if( abs(y-pre.Y) > abs((y-g_DCMapNumPlanets)-pre.Y)) 
						y -= g_DCMapNumPlanets;

					if(m_CurrentPoly.back() == Point(x,y))break;
				}
				m_CurrentPoly.push_back(Point(x,y));

				// update region if it moves to negative space
				bool mx = x<0;
				bool my = y<0;
				if(mx | my)	for(int i=0;i<m_CurrentPoly.size();i++)
				{
					if(mx) m_CurrentPoly[i].X+=g_DCMapNumPlanets;
					if(my) m_CurrentPoly[i].Y+=g_DCMapNumPlanets;
				}
				if(m_pDrawMan)m_pDrawMan->OnMessage(DCMM_GRAPHICS_POINT_PLACED,MAKEWPARAM(x,y),0,this);
			}

			MakeDef(m_CurrentDrawInfo,m_CurrentDef,m_CurrentPoly.get(),m_CurrentPoly.size());
			dcmapRECT upd;
			UnionRect(&upd,&past,&m_CurrentDef.rcBound);
			m_pView->UpdateMapRect(&upd);
		}
		break;
	case DCMM_MAP_LAYER_ACTIVATE:
		if(pSender)
			m_pView = pSender->Cast<IDCMapLayeredMapView>();
		m_bActive = true;
		break;
	case DCMM_MAP_LAYER_DEACTIVATE:
		m_bActive = false;
		break;	
	case DCMM_MAP_SCALE_CHANGED:
		m_iCellSize = wParam;
		break;	
	}
	return 0;
}

void DCMAPAPI CGraphicLayer::BeginDraw(EDCMapGraphicsType type,IDCMapModule* pManager)
{
	if(m_bDrawMode)
	{
		EndDraw();
	}
	m_pDrawMan = pManager;
	m_bDrawMode=true;
	m_CurrentPoly.clear();
	m_CurrentType = type;

	m_pGraphics->FillDefaults(&m_CurrentDrawInfo);
	m_CurrentDrawInfo.type = type;

	MakeDef(m_CurrentDrawInfo,m_CurrentDef,m_CurrentPoly.get(),m_CurrentPoly.size());

	if(m_pDrawMan)m_pDrawMan->OnMessage(DCMM_GRAPHICS_DRAW_BEGIN,0,0,this);
}

void CGraphicLayer::SaveGraph()
{
	int minn = m_CurrentType == DCMAP_GRAPHICS_POLYGON?3:2;

	minn = m_CurrentType == DCMAP_GRAPHICS_ZONES?1:minn;

	if(m_CurrentPoly.size() >= minn)
	{
		dcmapDCSTR name = m_pCore->GetCoreUI()->InputBox(L"Введите название элемента",L"");

		if(name && name[0])
		{
			// get all of...
			m_pGraphics->Insert();

			if(!m_CurrentDrawInfo.x || !m_CurrentDrawInfo.y)
			{
				m_CurrentDrawInfo.x = m_CurrentPoly[0].X;
				m_CurrentDrawInfo.y = m_CurrentPoly[0].Y;
			}

			m_CurrentDrawInfo.parent = m_refParent;
			m_CurrentDrawInfo.type = m_CurrentType;

			m_pGraphics->SetData(&m_CurrentDrawInfo);

			m_pGraphics->SetWideName(name);

			Point* lines = new Point[m_CurrentPoly.size()];
			for(int i=0;i<m_CurrentPoly.size();i++)lines[i] = m_CurrentPoly[i];

			m_pGraphics->SetBlobN("coords",(dcmapBYTE*)lines,sizeof(Point)*m_CurrentPoly.size());

			m_pGraphics->Post(0);

			delete lines;

			BuildRegions();
		}
	}
	m_CurrentPoly.clear();
}

void DCMAPAPI CGraphicLayer::CancelDraw()
{
	if(!m_bDrawMode)return;
	m_bDrawMode = false;
	m_pView->UpdateMapRect(&m_CurrentDef.rcBound);
	if(m_pDrawMan)m_pDrawMan->OnMessage(DCMM_GRAPHICS_DRAW_END,0,0,this);
}

void DCMAPAPI CGraphicLayer::EndDraw()
{
	if(!m_bDrawMode)return;
	SaveGraph();
	m_bDrawMode = false;
	m_pView->UpdateMapRect(&m_CurrentDef.rcBound);
	m_pDrawMan = 0;
	if(m_pDrawMan)m_pDrawMan->OnMessage(DCMM_GRAPHICS_DRAW_END,1,0,this);
}

void CGraphicLayer::MakeDef(DCMapGraphicsInfo const& gr,GraphDef &def,Point* lines,int num)
{
	if(def.path)delete def.path;
	if(def.reg)delete def.reg;
	def.path = 0;
	def.reg = 0;
	def.text=0;
	FLOAT rad = gr.radius+0.002f;

	if(num)
	{
		def.ptBase.x = lines[0].X;
		def.ptBase.y = lines[0].Y;
	}
	else def.ptBase = gr.pt;

	def.type = (EDCMapGraphicsType)gr.type;
	if(def.type == DCMAP_GRAPHICS_POLYGON && num < 3)
		def.type = DCMAP_GRAPHICS_POLYLINE;

	if(def.type != DCMAP_GRAPHICS_ZONES && num == 1)
	{
		def.type = DCMAP_GRAPHICS_ZONES;
		rad = 0.002;
	}

	if(gr.text[0] && gr.text_type>0)
	{
		def.ptText.X = gr.pt.x;
		def.ptText.Y = gr.pt.y;
		def.text = gr.text;
		def.fTextSize = gr.text_size;
		def.clText.SetFromCOLORREF(gr.text_color&0x00ffffff);
		def.clText.SetValue(Color::MakeARGB((gr.text_color&0xff000000)>>24, def.clText.GetR(), def.clText.GetG(), def.clText.GetB()));
		def.iTextLen = wcslen(gr.text);
	}

	def.clFill.SetFromCOLORREF(gr.fill_color&0x00ffffff);
	def.clFill.SetValue(Color::MakeARGB((gr.fill_color&0xff000000)>>24, def.clFill.GetR(), def.clFill.GetG(), def.clFill.GetB()));

	def.clLine.SetFromCOLORREF(gr.line_color&0x00ffffff);
	def.clLine.SetValue(Color::MakeARGB((gr.line_color&0xff000000)>>24, def.clLine.GetR(), def.clLine.GetG(), def.clLine.GetB()));

	def.line_size = gr.line_size;
	def.fill_mode = gr.fill_mode;

	def.line_type = gr.line_type;

	if(num)
	{	
		def.path = new GraphicsPath;

		if(def.type == DCMAP_GRAPHICS_POLYGON)
			def.path->AddPolygon(lines,num);
		else if(def.type == DCMAP_GRAPHICS_POLYLINE)  
			def.path->AddLines(lines,num);
		else if(def.type == DCMAP_GRAPHICS_ZONES)
		{
			def.path->AddEllipse((REAL)lines[0].X-rad,
				(REAL)lines[0].Y-rad,
				rad*2,rad*2);
			def.reg = new Region(def.path);
			for(int i=1;i<num;i++)
			{
				GraphicsPath path;
				path.AddEllipse((REAL)lines[i].X-rad,
					(REAL)lines[i].Y-rad,
					rad*2,rad*2);
				def.reg->Union(&path);
				def.path->AddPath(&path,FALSE);
			}
		}

		def.rcBound.left =  lines[0].X;
		def.rcBound.top = lines[0].Y;
		def.rcBound.right =lines[0].X;
		def.rcBound.bottom =  lines[0].Y;
		for(int i=1;i<num;i++)
		{
			def.rcBound.left = min(lines[i].X,def.rcBound.left);
			def.rcBound.top = min(lines[i].Y,def.rcBound.top);
			def.rcBound.right = max(lines[i].X,def.rcBound.right);
			def.rcBound.bottom = max(lines[i].Y,def.rcBound.bottom);
		}
		if(gr.type == DCMAP_GRAPHICS_ZONES)
		{
			def.rcBound.top -= gr.radius+1;
			def.rcBound.bottom += gr.radius+1;
			def.rcBound.left -= gr.radius+1;
			def.rcBound.right += gr.radius+1;
		}
	}
	else
	{
		def.rcBound.left = def.ptBase.x - def.iTextLen*def.fTextSize/2;
		def.rcBound.top = def.ptBase.y - def.fTextSize/2;
		def.rcBound.right = def.ptBase.x + def.iTextLen*def.fTextSize/2;
		def.rcBound.bottom = def.ptBase.y + def.fTextSize/2;
	}
	def.x_overwrap = def.rcBound.right > g_DCMapNumPlanets;
	def.y_overwrap = def.rcBound.bottom > g_DCMapNumPlanets;
}

void CGraphicLayer::BuildRegions()
{
	_foreach(RegionsList,m_Regions,it)
	{
		if(it->path)delete it->path;
		if(it->reg)delete it->reg;
	}
	m_Regions.clear();

	if(m_pGraphics->SelectByParentID(m_refParent))
	do 
	{
		DCMapGraphicsInfo  const& gr = *m_pGraphics->GetData();
		if(!gr.visible)continue;

		Point* lines;
		int num = m_pGraphics->GetBlobN("coords",(dcmapBYTE**)&lines)/sizeof(Point);

		GraphDef def;
		def.ref = gr.id;

		MakeDef(gr,def,lines,num);

		m_Regions.push_back(def);
	} while(m_pGraphics->Next());

	m_GraphicsState = m_pGraphics->GetStateIndex();
}

bool DCMAPAPI CGraphicLayer::EditLayer()
{
	m_pCore->GetCoreUI()->OpenListTableEdit("graphics",0,&m_refParent);
	return true;
}

bool DCMAPAPI CGraphicLayer::TestPoint(float x,float y,IDCMapVarValue* pResult)
{
    if(!pResult)return false;
    _foreach(RegionsList,m_Regions,it)
    {
        if(it->type != DCMAP_GRAPHICS_POLYGON && 
            it->type != DCMAP_GRAPHICS_ZONES)continue;

		if(!it->path)continue;

        if(!it->reg)it->reg = new Region(it->path);
        dcmapPOINT iPt = {x,y};

        if(PtInRect(&it->rcBound,iPt) && it->reg->IsVisible(x,y)){pResult->AddDBRef(&it->ref,m_pGraphics);continue;}
        if(it->x_overwrap)
        {
            iPt.x += g_DCMapNumPlanets;
            if(PtInRect(&it->rcBound,iPt) && it->reg->IsVisible(x+g_DCMapNumPlanets,y)){pResult->AddDBRef(&it->ref,m_pGraphics);continue;}
            iPt.x -= g_DCMapNumPlanets;
        }
        if(it->y_overwrap)
        {
            iPt.y += g_DCMapNumPlanets;
            if(PtInRect(&it->rcBound,iPt) && it->reg->IsVisible(x,y+g_DCMapNumPlanets)){pResult->AddDBRef(&it->ref,m_pGraphics);continue;}
            if(it->x_overwrap)
            {
                iPt.x += g_DCMapNumPlanets;
                if(PtInRect(&it->rcBound,iPt) && it->reg->IsVisible(x+g_DCMapNumPlanets,y+g_DCMapNumPlanets)){pResult->AddDBRef(&it->ref,m_pGraphics);continue;}
            }		
        }
    }
    return true;
}

DCMapGraphicsInfo* DCMAPAPI CGraphicLayer::GetGraphicsInfo()
{
	return &m_CurrentDrawInfo;
}

void DCMAPAPI CGraphicLayer::UpdateGraphicsInfo( DCMapGraphicsInfo* pinf )
{
	if(pinf)
	{
		dcmapRECT past = m_CurrentDef.rcBound;

		m_CurrentDrawInfo = *pinf;
		MakeDef(m_CurrentDrawInfo,m_CurrentDef,m_CurrentPoly.get(),m_CurrentPoly.size());

		dcmapRECT upd;
		UnionRect(&upd,&past,&m_CurrentDef.rcBound);
		m_pView->UpdateMapRect(&upd);
	}
}



void DCMAPAPI CGraphicLayer::Undo()
{
	dcmapRECT past = m_CurrentDef.rcBound;
	m_CurrentPoly.pop_back();
	MakeDef(m_CurrentDrawInfo,m_CurrentDef,m_CurrentPoly.get(),m_CurrentPoly.size());

	dcmapRECT upd;
	UnionRect(&upd,&past,&m_CurrentDef.rcBound);
	m_pView->UpdateMapRect(&upd);
}


struct CPlanetHint_GraphicRegions : public IDCMapPlanetHintSource
{
	IDCMapPlanetDataStoragePtr   m_pPlanets;
	IDCMapGraphicsDataStoragePtr m_pGraphics;
	IDCMapLayeredMapView* m_pView;
	IDCMapCore* m_pCore;
	IDCMapWorkspace* m_pWorkspace;

    const wchar_t* m_FormatString;

	CPlanetHint_GraphicRegions()
	{
		m_pView=0;
	}

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetHintSource);
		return 0;
	}
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pCore = core;
		m_pWorkspace=workspace;
		if(!m_pPlanets.init(core))return false;
		if(!m_pGraphics.init(core,"graphics"))return false;

        m_FormatString = g_pLocalStrings->GetStringW("PlanetHintGraphicRegionsFormat");

		return true;
	}
	dcmapWCSTR DCMAPAPI GetHint(dcmapDBREF const& ref)
	{
		if(m_pPlanets->SelectID(ref))
		{
			return GetCellHint(m_pPlanets[DCMDB_PLANET_X],m_pPlanets[DCMDB_PLANET_Y]);
		}
		return 0;
	}

	dcmapWCSTR DCMAPAPI GetCellHint(int x,int y)
	{
		if(!m_pView)
		{
			if(m_pWorkspace->SelectModuleByID("MapView"))
			{
				m_pView = m_pWorkspace->Module()->Cast<IDCMapLayeredMapView>();
			}
		}
		if(!m_pView)return 0;

		static std::wstring res;

		res.clear();

		float fx = x-0.5f;
		float fy = y-0.5f;

		//DataRefVector regions;
		int total=0;

        IDCMapVarValue* pVar = m_pCore->CreateVarValue();

		int n = m_pView->NumLayers();
		for(int i=0;i<n;i++)
		{
			if(!m_pView->LayerVisible(i))continue;
			IDCMapLayer* pLayer = m_pView->LayerInterface(i);
			if(!pLayer)continue;
			CGraphicLayer* layer = (CGraphicLayer*)pLayer->Cast<IDCMapGraphicLayer>();
			if(!layer)continue;

			wstring lName = m_pView->LayerCaption(i);

			layer->TestPoint(fx,fy,pVar);
            int sz = pVar->Count();

			for(int i=0;i<sz;i++)
			{
				dcmapDBREF const* pRef = pVar->GetDBRef(i);
				if(pRef && m_pGraphics->SelectID(*pRef))
				{
                    res += boost::str( wformat(m_FormatString) % lName % (wstring)m_pGraphics->GetWideName());
					res += L"<br>";
					total++;
				}
			}
            pVar->Clear();
			//regions.clear();
		}
		if(total)
		{
			res=L"<font size=4><b>-------------------------------------------------------------------------------------------------------------</b></font><br>"+res;
		}
        pVar->Destroy();

		return res.c_str();
	}
};
DCMAP_REGISTER(CPlanetHint_GraphicRegions,"PlanetHintSource","060_GraphicRegions");




