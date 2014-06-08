#include "StdAfx.h"
#include "platform.h"
#include "g_rect.h"
#include "utils.h"
using namespace Gdiplus;

Color MakeGdipColorA(dcmapCOLOR dcmcl)
{
	Color out;
	out.SetFromCOLORREF(dcmcl&0x00ffffff);
	out.SetValue(Color::MakeARGB((dcmcl&0xff000000)>>24, out.GetR(), out.GetG(), out.GetB()));
	return out;
}

Color MakeGdipColorA(dcmapCOLOR dcmcl,float a)
{
	Color out;
	out.SetFromCOLORREF(dcmcl&0x00ffffff);
	out.SetValue(Color::MakeARGB(a*255, out.GetR(), out.GetG(), out.GetB()));
	return out;
}

Color MakeGdipColor(dcmapCOLOR dcmcl)
{
	Color out;
	out.SetFromCOLORREF(dcmcl&0x00ffffff);
	return out;
}


#include "FleetLayer.h"

static void DefaultColors(dcmapCOLOR* m_clDist)
{
	m_clDist[EFC_START] = 16776960;
	m_clDist[1] = 112129;
	m_clDist[2] = 65535;
	m_clDist[3] = 4227327;
	m_clDist[EFC_NDIST] = 255;
	m_clDist[EFC_MAX_DIST] = 255;
	m_clDist[EFC_OPEN_BORDER] = 16776960;
}


static void LoadColors(IDCMapCore* pCore,dcmapCOLOR* m_clDist)
{
	DCMapConfigWrapper cfg(pCore->GetConfig());
	cfg.section("FleetColor");

	cfg["FleetStart"]  >> m_clDist[EFC_START];
	cfg["Dist1Turn"]  >> m_clDist[1];
	cfg["Dist2Turn"] >> m_clDist[2];
	cfg["Dist3Turn"] >> m_clDist[3];
	cfg["NormalDist"] >> m_clDist[EFC_NDIST];
	cfg["MaxDist"] >> m_clDist[EFC_MAX_DIST];
	cfg["OpenPlanetBorder"] >> m_clDist[EFC_OPEN_BORDER];

	cfg["MyFleet"] >> m_clDist[EFC_FLEET_MY];
	cfg["FleetInTransit"] >> m_clDist[EFC_FLEET_IN_TRANSIT];

	cfg["UndefinedFleet"] >> m_clDist[EFC_FLEET_UNDEF];
	cfg["NeutralFleet"] >> m_clDist[EFC_FLEET_NEUTRAL];
	cfg["AllyFleet"] >> m_clDist[EFC_FLEET_ALLY];
	cfg["EnemyFleet"] >> m_clDist[EFC_FLEET_ENEMY];

	cfg["IncomingFleet"] >> m_clDist[EFC_FLEET_INCOMING];
}


struct CFleetColorsProcessor : public IDCMapPlanetDataFilter
{
	IDCMapFleetManager::FleetJumpInfo m_JumpInfo;
	IDCMapIconsDataStoragePtr Icons;
	IDCMapFleetsDataStoragePtr Fleets;
	IDCMapFleetManager* m_pFleetMan;
	dcmapDBREF m_refCurrentFleet;
	bool m_bHilightMode;
	IDCMapCore* m_pCore;
	IDCMapWorkspace* m_pWorkspace;

	dcmapCOLOR m_clDist[EFC_COUNT];

	CFleetColorsProcessor():
		m_bHilightMode(false)
	{
		DefaultColors(m_clDist);
	}

	dcmapLRESULT DCMAPAPI OnMessage(dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender)
	{
		switch (Msg)
		{
		case DCMM_CONFIG_CHANGED:
			LoadColors(m_pCore,m_clDist);
			break;
		case DCMM_FLEET_SELECT:
			{
				dcmapDBREF* pref = (dcmapDBREF*)wParam;
				if(pref && pref->Valid())
				{
					m_refCurrentFleet = *pref;
					m_bHilightMode=true;
					if(m_pFleetMan->GetJumpInfo())
						m_JumpInfo = *m_pFleetMan->GetJumpInfo();
				}
				else
				{
					m_bHilightMode=false;
					m_refCurrentFleet.Invalidate();
				}
			}
			break;
		}
		return 0;
	}

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pCore = core;
		m_pWorkspace = workspace;

		if(!Icons.init(core))return false;
		if(!Fleets.init(core))return false;

		m_pFleetMan = workspace->GetServiceTyped<IDCMapFleetManager>("FleetManager");
		if(!m_pFleetMan)return false;

		LoadColors(m_pCore,m_clDist);
		return true;
	}
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetDataFilter);
		return 0;
	}

	static dcmapCOLORREF FadeBorder(dcmapCOLORREF cl)
	{
		int r = dcmapGetRValue(cl);
		int g = dcmapGetGValue(cl);
		int b = dcmapGetBValue(cl);
		return dcmapRGB(r*0.55,g*0.55,b*0.55);
	}

	bool DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* data,DCMapPlanetInfo const* pinf)
	{
		if(pinf->infiltration==2)
		{
			data->iCellBorder = max(data->iCellBorder,2);
			data->clCellBorder = dcmapRGB(240,240,0);
		}
		else if(pinf->infiltration==1)
		{
			data->iCellBorder = max(data->iCellBorder,2);
			data->clCellBorder = dcmapRGB(240,0,0);
		}
		if(pinf->fleets.Valid() && Fleets->SelectGroup(pinf->fleets))
		{
			dcmapDBREF RefShowIcon;
			do
			{
				DCMapFleetInfo const* finf = Fleets->GetData();
				if(finf->type != DCMAP_FTP_Fleet)continue;
				if(finf->flags & DCMAP_FF_Deleted)continue;
				if(finf->flags & DCMAP_FF_Outdated)continue;
				if(finf->icon.Valid() && Icons->SelectID(finf->icon))
				{
					RefShowIcon = finf->icon;
				}
			}
			while(Fleets->Next());

			if(RefShowIcon.ValidID())
			{
				data->Icon3 = RefShowIcon.id;
			}
		}
		if(m_bHilightMode)
		{
			bool open=true;
			int turns = m_pFleetMan->TestJump(pinf->x,pinf->y,&open);
			int ind = EFC_NDIST;
			if(turns || pinf->x == m_JumpInfo.x && pinf->y == m_JumpInfo.y)
			{
				if(turns < 4)ind = turns;	
				else if(turns >= m_JumpInfo.MaxTurns)ind = EFC_MAX_DIST;	

				data->clBackground = m_clDist[ind];

				if(open)
				{
					data->iCellBorder = max(data->iCellBorder,1);
					data->clCellBorder = m_clDist[EFC_OPEN_BORDER];
				}
				else if(data->iCellBorder) data->clCellBorder = FadeBorder(data->clCellBorder);
			}
		}
		return true;
	}

	bool DCMAPAPI ActualizeData()
	{
		return Fleets.actualize_state() || Icons.actualize_state();
	}
};
DCMAP_REGISTER(CFleetColorsProcessor,"PPlanetFilter","050_FleetProcessor");


DCMAP_REGISTER(CFleetLayer,"PMapLayer","050_Fleets");

CFleetLayer::CFleetLayer(void):m_bHilightJump(false)
{
	DefaultColors(m_cColors);
}

CFleetLayer::~CFleetLayer(void)
{

}

int DCMAPAPI CFleetLayer::PrepareRender(dcmapRECT const& rcBound, dcmapRECT const& rcWnd,int CellSize,int iFlags)
{
	return DCMAP_LRO_RENDER | DCMAP_LRO_PRERENDER | DCMAP_LRO_PRERENDER_HIDDEN;
}

void DCMAPAPI CFleetLayer::PreRender(dcmapHDC dc,dcmapRECT const& rcBound, dcmapRECT const& rcWnd,int CellSize,int iFlags)
{
	if(!m_bHilightJump)return;
	if(CellSize < 5) // simplified
	{
		return;
	}

	for(int y=rcWnd.top;y<=rcWnd.bottom;y++)
	{
		for(int x=rcWnd.left;x<=rcWnd.right;++x)
		{
			int turns = m_pFleetMan->TestJump(x,y);
			int ind = EFC_NDIST;
			if(turns || x == m_JumpInfo.x && y == m_JumpInfo.y)
			{
				if(turns < 4)ind = turns;	
				else if(turns >= m_JumpInfo.MaxTurns)ind = EFC_MAX_DIST;	

				int cx = x-1-rcWnd.left;
				int cy = y-1-rcWnd.top;
				int px = rcBound.left+cx*CellSize;
				int py = rcBound.top+cy*CellSize;

				RECT rc = {px,py,px+CellSize,py+CellSize};
				FillRect(dc,&rc,m_clBrushes[ind]);
			}
		}
	}
}

void CFleetLayer::UpdateConfig()
{
	LoadColors(m_pCore,m_cColors);

	for(int i=0;i<EFC_BRUSHES;i++)
	{
		DeleteBrush(m_clBrushes[i]);m_clBrushes[i] = CreateSolidBrush(m_cColors[i]);
	}

	DeletePen(m_hpenBorder);m_hpenBorder = CreatePen(PS_SOLID,2,m_cColors[EFC_OPEN_BORDER]);
	DeletePen(m_hpenBorderSmall);m_hpenBorderSmall = CreatePen(PS_SOLID,0,m_cColors[EFC_OPEN_BORDER]);
}
enum EAlienType
{
	AT_INCOMING,
	AT_ENEMIES,
	AT_UNDEFINED,
	AT_NEUTRALS,
	AT_ALLY,
};

void DCMAPAPI CFleetLayer::Render(dcmapHDC dc,dcmapRECT const& rcBound, dcmapRECT const& rcWnd,int CellSize,int iFlags)
{
	if(CellSize < 8)return;

	int BorderSize = CellSize < 16?1:2;

	dcmapHBRUSH hbrmMyFleet,hbrmForeinFleet,hbrmMyFleetInTransit;
	hbrmMyFleet = m_clBrushes[EFC_FLEET_MY];
	hbrmMyFleetInTransit = m_clBrushes[EFC_FLEET_IN_TRANSIT];
	hbrmForeinFleet = m_clBrushes[EFC_FLEET_UNDEF];

	dcmapHBRUSH hbrmForeignFleetDip[5];

	hbrmForeignFleetDip[AT_UNDEFINED] = m_clBrushes[EFC_FLEET_UNDEF];
	hbrmForeignFleetDip[AT_NEUTRALS]  = m_clBrushes[EFC_FLEET_NEUTRAL];
	hbrmForeignFleetDip[AT_ALLY]      = m_clBrushes[EFC_FLEET_ALLY];;
	hbrmForeignFleetDip[AT_ENEMIES]   = m_clBrushes[EFC_FLEET_ENEMY];

	hbrmForeignFleetDip[AT_INCOMING] = m_clBrushes[EFC_FLEET_INCOMING];

	int FleetAreaV = CellSize; 
	int FleetAreaH = CellSize;

	int BarMarginH  = 1;
	int BarMarginV  = 2;


	int BarWidth  = 2;
	int BarHeight = 2;
	int BarSpaceH  = 0;
	int BarSpaceV  = 0;

	if(CellSize>=22)
	{
		BarWidth  = 2;
		BarHeight = 3;
		BarSpaceV  = 1;
		BarSpaceH  = 1;

		FleetAreaV = CellSize - 6; 
		FleetAreaH = CellSize - 6;
	}

	if(CellSize>=26)
	{
		BarWidth  = 2;
		BarHeight = 3;
		BarSpaceV  = 1;

		FleetAreaV = CellSize - 10; 
		FleetAreaH = CellSize - 10;
	}

	if(CellSize>=32)
	{
		BarWidth  = 3;
		BarHeight = 5;		

		BarMarginH  = 2;
		BarMarginV  = 3;
	}

	if(CellSize>=40)
	{
		FleetAreaH = CellSize - 18;
		FleetAreaV = CellSize - 10;
	}

	FleetAreaV -= BarMarginV*2;
	FleetAreaH -= BarMarginH*2;



	int LineWidth  = FleetAreaH;

	int BarCount  = FleetAreaH/(BarWidth+BarSpaceH);
	int LineCount = FleetAreaV/(BarHeight+BarSpaceV);

	for(int y=rcWnd.top;y<=rcWnd.bottom;y++)
	{
		if(m_pPlanets->SelectLine(y,rcWnd.left,rcWnd.right))
		do
		{
			DCMapPlanetInfo const* pinf = m_pPlanets->GetData();

			int cx = pinf->x-1-rcWnd.left;
			int cy = pinf->y-1-rcWnd.top;
			int px = rcBound.left+cx*CellSize;
			int py = rcBound.top+cy*CellSize;

			if(pinf->fleets.Valid() && m_pFleets->SelectGroup(pinf->fleets))
			{
				int MyFleets=0;
				int MyFleetState[2]={0,0};
				int ForeinFleets=0;
				int ForeinFleetsByDip[5]={0,0,0,0,0};

				do
				{
					DCMapFleetInfo const* finf = m_pFleets->GetData();
					if(finf->type !=  DCMAP_FTP_Fleet)continue;
					if(finf->flags & DCMAP_FF_Outdated)continue;
					if(finf->flags & DCMAP_FF_Deleted)continue;

					if(finf->flags & DCMAP_FF_Foreign)
					{
						if(finf->tta) ForeinFleetsByDip[AT_INCOMING]++;
						else 
						{
							switch(finf->dip)
							{
							case DCMAP_DIP_ALLY:ForeinFleetsByDip[AT_ALLY]++;break;
							case DCMAP_DIP_UNDEFINED:ForeinFleetsByDip[AT_UNDEFINED]++;break;
							case DCMAP_DIP_ENEMIES:ForeinFleetsByDip[AT_ENEMIES]++;break;
							case DCMAP_DIP_NEUTRALS:ForeinFleetsByDip[AT_NEUTRALS]++;break;
							}
						}
						ForeinFleets++;
					}
					else 
					{
						if(finf->tta) MyFleetState[0]++;
						else MyFleetState[1]++;
						MyFleets++;
					}
				}
				while(m_pFleets->Next());

				int top = py+BarMarginV;
				int right = px+CellSize-BarMarginH;

				if(MyFleets)
				{
					if(MyFleets > BarCount)
					{
						dcmapRECT rc = {right-LineWidth,top,right,top+BarHeight};
						FillRect(dc,&rc,hbrmMyFleet);
					}
					else
					{
						for(int i=0;i<MyFleetState[0];i++)
						{
							dcmapRECT rc = {right-BarWidth,top,right,top+BarHeight};
							FillRect(dc,&rc,hbrmMyFleetInTransit);
							right-=(BarWidth+BarSpaceH);
						}
						for(int i=0;i<MyFleetState[1];i++)
						{
							dcmapRECT rc = {right-BarWidth,top,right,top+BarHeight};
							FillRect(dc,&rc,hbrmMyFleet);
							right-=(BarWidth+BarSpaceH);
						}
					}
					top += BarHeight+BarSpaceV;
				}
				right = px+CellSize-BarMarginH;

				if(ForeinFleets)
				{
					if(ForeinFleets > BarCount)
					{
						dcmapRECT rc = {right-LineWidth,top,right,top+BarHeight};
						FillRect(dc,&rc,hbrmForeinFleet);
					}
					else
					{
						for(int n=0;n<5;n++)
						{
							for(int i=0;i<ForeinFleetsByDip[n];i++)
							{
								dcmapRECT rc = {right-BarWidth,top,right,top+BarHeight};
								FillRect(dc,&rc,hbrmForeignFleetDip[n]);
								right-=(BarWidth+BarSpaceH);
							}
						}
					}
					dcmapRECT rc = {px+CellSize-9,py+5,px+CellSize-2,py+9};
				}
			}

			if(m_bHilightJump)
			{
				bool open;
				if(m_pFleetMan->TestJump(pinf->x,pinf->y,&open) && open)
				{
					dcmapHBRUSH oldBrush;
					dcmapHPEN oldPen;

					oldBrush = SelectBrush(dc,GetStockBrush(HOLLOW_BRUSH));
					if(BorderSize==1) 
					{
						oldPen = SelectPen(dc,m_hpenBorderSmall);
						Rectangle(dc,px+1,py+1,px+CellSize,py+CellSize);
					}
					else 
					{
						oldPen = SelectPen(dc,m_hpenBorder);
						Rectangle(dc,px+2,py+2,px+CellSize,py+CellSize);
					}
					SelectPen(dc,oldPen);
					SelectBrush(dc,oldBrush);
				}
			}
		}
		while(m_pPlanets->Next());
	}
}

bool DCMAPAPI CFleetLayer::ActualizeData()
{
	return false;

}

bool DCMAPAPI CFleetLayer::EditLayer()
{
	return false;

}

void DrawActiveTrasserEx(HDC dc,int fx,int fy,int tx,int ty,gsys::grect const& bound,float dist,int iTurns,dcmapDBREF const& ref)
{
	Graphics graphics(dc);

	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	Pen outer_pen(MakeGdipColorA(dcmapRGB(240,240,140),0.3),5);

	outer_pen.SetStartCap(LineCapDiamondAnchor);
	outer_pen.SetEndCap(LineCapArrowAnchor);

	Pen inner_pen(MakeGdipColor(dcmapRGB(240,240,140)),2);
	inner_pen.SetEndCap(LineCapArrowAnchor);

	graphics.DrawLine(&outer_pen,fx,fy,tx,ty);
	graphics.DrawLine(&inner_pen,fx,fy,tx,ty);

	wstring strn = ToWStr(iTurns);

	SolidBrush  BackBrush(Color(200, 50, 50, 50));
	Pen  BackPen(Color(255, 0, 255, 0));

	// Initialize arguments.
	Font myFont(L"Arial", 10);
	PointF origin(tx-6,ty-6);
	SolidBrush blackBrush(Color(255, 0, 255, 0));
	StringFormat format;
	format.SetAlignment(StringAlignmentFar);
	format.SetLineAlignment(StringAlignmentFar);

	//SizeF sz(0,0);
	//SizeF size;

	RectF rect;
	graphics.MeasureString(strn.c_str(),strn.size(),&myFont,origin,&format,&rect);
	rect.Inflate(2,2);
	graphics.FillRectangle(&BackBrush,rect);
	graphics.DrawRectangle(&BackPen,rect);

	// Draw string.
	graphics.DrawString(strn.c_str(),strn.size(),&myFont,origin,&format,&blackBrush);
}

void DrawActiveTrasser(HDC dc,int fx,int fy,int tx,int ty,gsys::grect const& bound,float dist,int iTurns,dcmapDBREF const& ref)
{
	dcmapHPEN hpenLine;
	hpenLine = CreatePen(PS_SOLID,2,dcmapRGB(240,240,140));

	hpenLine = SelectPen(dc,hpenLine);
	MoveToEx(dc,fx,fy,0);
	LineTo(dc,tx,ty);

	DeletePen(SelectPen(dc,hpenLine));

	wstring strn = ToWStr(iTurns);

	UINT iAlign=TA_RIGHT|TA_BOTTOM;

	/*if(tx<fx)iAlign |= TA_RIGHT;
	else iAlign |= TA_LEFT;

	if(ty<fy)iAlign |= TA_BOTTOM;
	else iAlign |= TA_TOP;*/

	SetTextAlign(dc,iAlign);

	TextOut(dc,tx-5,ty-5,strn.c_str(),strn.size());

}

bool DCMAPAPI CFleetLayer::DragOver(int x,int y,int State,IDCMapDragObject* pDragInfo,IDCMapModule* pModule)
{
	if(!pDragInfo)return false;
	if(strcmp(pDragInfo->Name(),"Fleet")!=0)return false;
	IDCMapFleetDragObject* pFleetDrag = (IDCMapFleetDragObject*)pDragInfo;
	int n = pFleetDrag->NumFleets();
	if(n<=0)return false;
	dcmapDBREF FleetRef = *pFleetDrag->Fleet(0);
	
	if(State == dcmap_dsDragEnter)
	{
		m_bHilightJump=true;
		if(!m_pFleetMan->SelectFleet(&FleetRef))return false;
		if(m_pFleetMan->GetJumpInfo())
			m_JumpInfo = *m_pFleetMan->GetJumpInfo();
		m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0);
		//m_pView->UpdateMapRect(0);
	}
	else if(State == dcmap_dsDragLeave)
	{
		m_pView->EndAnim();
		m_bHilightJump=false;
		m_pFleetMan->SelectFleet(0);
		m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0);
		//m_pView->UpdateMapRect(0);
		return true;
	}

	int cx = (m_iLeft+x/m_iCellSize)%g_DCMapNumPlanets+1;
	int cy = (m_iTop+y/m_iCellSize)%g_DCMapNumPlanets+1;

	int n_ok=0;

	int turns = m_pFleetMan->TestJump(cx,cy);

	for(int i=0;i<n;i++)
	{
		dcmapDBREF const* pref = pFleetDrag->Fleet(i);
		if(!pref)continue;
		FleetRef = *pref;
		bool CanMove = m_pFleetMan->CanMoveFleetTo(FleetRef,cx,cy);
		if(!CanMove)continue;


		if(m_pFleets->SelectID(FleetRef))
		{
			DCMapFleetInfo const* finf = m_pFleets->GetData();
			
			// calculate anim rect
			gsys::grect rc(finf->x,finf->y,cx,cy);
			rc.Normalise();
			rc.offset(-1,-1);
			rc.grow(1);
			gsys::grect rcb;
			rcb.init_by_size(m_iLeft,m_iTop,m_iWidth,m_iHeight);
			gsys::grect drect = rc.Bound(rcb);
			drect.offset(-m_iLeft,-m_iTop);
			drect *= m_iCellSize;
			drect.left -= 28;
			drect.top -= 24;

			++n_ok;

			HDC dc;
			if(m_pView->StartAnim((dcmapRECT*)&drect,&dc,0))
			{
				// draw line
				int fx = ((finf->x-1-m_iLeft)*m_iCellSize+m_iCellSize/2)-drect.left;
				int fy = ((finf->y-1-m_iTop)*m_iCellSize+m_iCellSize/2)-drect.top;
				int tx = ((cx-1-m_iLeft)*m_iCellSize+m_iCellSize/2)-drect.left;
				int ty = ((cy-1-m_iTop)*m_iCellSize+m_iCellSize/2)-drect.top;

				float dist = gsys::fpoint(finf->x-cx,finf->y-cy).len();
	
				if(m_bNoGdiPlus)
					DrawActiveTrasser(dc,fx,fy,tx,ty,drect,dist,turns,*m_pFleets->GetID());
				else
					DrawActiveTrasserEx(dc,fx,fy,tx,ty,drect,dist,turns,*m_pFleets->GetID());

				m_pView->PresentAnim();
			}
			break;
		}
	}
	if(!n_ok)
	{
		m_pView->EndAnim();
		return false;
	}
	return true;
}

bool DCMAPAPI CFleetLayer::DragDrop(int x,int y,IDCMapDragObject* pDragInfo,IDCMapModule* pModule)
{
	if(!pDragInfo)return false;
	if(strcmp(pDragInfo->Name(),"Fleet")!=0)return false;
	IDCMapFleetDragObject* pFleets = (IDCMapFleetDragObject*)pDragInfo;
	int n = pFleets->NumFleets();
	if(n<=0)return false;

	int cx = (m_iLeft+x/m_iCellSize)%g_DCMapNumPlanets+1;
	int cy = (m_iTop+y/m_iCellSize)%g_DCMapNumPlanets+1;

	for(int i=0;i<n;i++)
	{
		dcmapDBREF const* pref = pFleets->Fleet(i);
		if(!pref)continue;
		m_pFleetMan->MoveFleetTo(*pref,cx,cy);
	}
	m_pView->EndAnim();
	return true;
}


dcmapCSTR DCMAPAPI CFleetLayer::GetLayerName()
{
	return "fleets";
}

bool DCMAPAPI CFleetLayer::Init( IDCMapCore* core,IDCMapWorkspace* workspace )
{
	m_pCore = core;
	m_pWorkspace = workspace;

	m_pFleetMan = m_pWorkspace->GetServiceTyped<IDCMapFleetManager>("FleetManager");
	if(!m_pFleetMan)return false;

	m_pPlanets.init(core,"planets");
	m_pFleets.init(core,"fleets");

	m_bNoGdiPlus = m_pCore->GetSystemOptions()->bNoGdiPlus;

	UpdateConfig();
	return true;
}

int DCMAPAPI CFleetLayer::OnMessage( int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender )
{
	switch(Msg)
	{
	case DCMM_CONFIG_CHANGED:
		UpdateConfig();
		break;
	case DCMM_MAP_SCALE_CHANGED:
		m_iCellSize = wParam;
		break;	
	case DCMM_MAP_VIEW_RESIZE:
		m_iWidth = wParam;
		m_iHeight = lParam;
		break;	
	case DCMM_MAP_PAN:
		m_iLeft = wParam;
		m_iTop = lParam;
		break;	
	}
	return 0;
}

bool DCMAPAPI CFleetLayer::InitLayer( IDCMapLayeredMapView* pView )
{
	m_pView = pView;
	return true;
}