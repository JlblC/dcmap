#include "StdAfx.h"
#include "platform.h"

#include "DCMapNavView.h"
#include <g_alg.h>
#include <g_rect.h>
#include <GdiPlus.h>
#include "resource.h"
#include "dcmapcore.h"
#include "BaseDataStorage.h"
#include "Parser.h";
using namespace Gdiplus;

int inline WrapCoord(int c){return (c+g_DCMapNumPlanets-1)%g_DCMapNumPlanets+1;}

dcmapHBITMAP LoadBMP(dcmapBYTE* pdata,dcmapHDC base,int &w,int &h,float scale=1.0f)
{
	BITMAPFILEHEADER* fh = (BITMAPFILEHEADER*)pdata;
	BITMAPINFOHEADER* ih = (BITMAPINFOHEADER*)(pdata+sizeof(BITMAPFILEHEADER));

	if(fh->bfType != 0x4d42)return (dcmapHBITMAP)INVALID_HANDLE_VALUE;
	if(fh->bfOffBits < (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)))
		return (dcmapHBITMAP)INVALID_HANDLE_VALUE;

	dcmapBYTE* data = pdata+fh->bfOffBits;

	w = ih->biWidth*scale;
	h = ih->biHeight*scale;

	int imgsz = 11;
	dcmapHBITMAP bmp = CreateCompatibleBitmap(base,w,h);
	//SelectBitmap(dc,bmp);
	//StretchDIBits(dc,0,0,w,h,0,0,ih->biWidth,ih->biHeight,data,
	//						(BITMAPINFO*)ih,DIB_RGB_COLORS,SRCCOPY);

	SetDIBits(base,bmp,0,h,data,(BITMAPINFO*)ih,DIB_RGB_COLORS);
	return bmp;
}

struct NavViewShared
{
	dcmapHBITMAP m_hPreBmp;
	dcmapHBITMAP m_hBmp;
	dcmapHBITMAP m_hAnimBmp;
	dcmapHBITMAP m_hAnimPreBmp;
	dcmapHDC m_hdcBmp;
	dcmapHDC m_hdcMem;
	dcmapHDC m_hdcAnimBmp;

	bool m_bIconsReady;

	int m_iBmpWidth;
	int m_iBmpHeight;

	bool created;
	bool anim_created;

	int m_iAnimBmpWidth;
	int m_iAnimBmpHeight;

	NavViewShared()
	{
		m_iBmpWidth=0;
		m_iBmpHeight=0;
		created=false;

		m_iAnimBmpWidth=0;
		m_iAnimBmpHeight=0;
		anim_created=false;

		m_hdcBmp=0;
		m_hdcMem=0;
		m_bIconsReady=false;
	}

	Graphics* m_pGraphics;
	Graphics* m_pAnimGraphics;

	HCURSOR hcHand;
	HCURSOR hcArrow;

	int CalcSize(int sz)
	{
		return (sz/4+1)*4;
	}

	bool InitAnim(dcmapHWND hWnd,int w,int h)
	{
		if(!anim_created)
		{
			m_iAnimBmpWidth = w;
			m_iAnimBmpHeight = h;

			m_hdcAnimBmp = CreateCompatibleDC(GetDC(hWnd));

			m_hAnimBmp = CreateCompatibleBitmap(GetDC(hWnd),m_iAnimBmpWidth,m_iAnimBmpHeight);
			m_hAnimPreBmp = SelectBitmap(m_hdcAnimBmp,m_hAnimBmp);
			anim_created = true;

			m_pAnimGraphics = new Graphics(m_hdcAnimBmp);
		}
		else if(w > m_iAnimBmpWidth || h > m_iAnimBmpHeight)
		{
			m_iAnimBmpWidth = max(m_iAnimBmpWidth,w);
			m_iAnimBmpHeight = max(m_iAnimBmpHeight,h);

			delete m_pAnimGraphics;

			DeleteBitmap(SelectBitmap(m_hdcAnimBmp,m_hAnimPreBmp));
			DeleteBitmap(m_hAnimBmp);
			m_hAnimBmp = CreateCompatibleBitmap(GetDC(hWnd),m_iAnimBmpWidth,m_iAnimBmpHeight);
			m_hAnimPreBmp = SelectBitmap(m_hdcAnimBmp,m_hAnimBmp);

			m_pAnimGraphics = new Graphics(m_hdcAnimBmp);
		}
		return true;
	}

	void Resize(dcmapHWND hWnd,int w,int h)
	{
		w = CalcSize(w);
		h = CalcSize(h);
		if(!created)
		{
			hcHand = LoadCursor(g_hInstance,MAKEINTRESOURCE(IDC_HAND));
			hcArrow = LoadCursor(0,IDC_ARROW);

			m_iBmpWidth = w;
			m_iBmpHeight = h;

			m_hdcBmp = CreateCompatibleDC(GetDC(hWnd));
			m_hdcMem = CreateCompatibleDC(GetDC(hWnd));

			m_hBmp = CreateCompatibleBitmap(GetDC(hWnd),m_iBmpWidth,m_iBmpHeight);
			m_hPreBmp = SelectBitmap(m_hdcBmp,m_hBmp);

			m_pGraphics = new Graphics(m_hdcBmp);

			created = true;
		}
		else if(w > m_iBmpWidth || h > m_iBmpHeight)
		{
			m_iBmpWidth = max(m_iBmpWidth,w);
			m_iBmpHeight = max(m_iBmpHeight,h);

			delete m_pGraphics;

			DeleteBitmap(SelectBitmap(m_hdcBmp,m_hPreBmp));
			DeleteBitmap(m_hBmp);
			m_hBmp = CreateCompatibleBitmap(GetDC(hWnd),m_iBmpWidth,m_iBmpHeight);
			m_hPreBmp = SelectBitmap(m_hdcBmp,m_hBmp);

			m_pGraphics = new Graphics(m_hdcBmp);
		}
	}
	dcmapQWORD IconsState;
	IDCMapIconsDataStoragePtr m_pIcons;

	// array of icons
	vector<IconData> vecIcons;


	dcmapHBITMAP CreateMask2(dcmapHBITMAP hbmColour, dcmapCOLORREF crTransparent)
	{
		dcmapHDC hdcMem, hdcMem2;
		dcmapHBITMAP hbmMask;
		BITMAP bm;
		// Create monochrome (1 bit) mask bitmap.  
		GetObject(hbmColour, sizeof(BITMAP), &bm);

		//hbmMask = CreateCompatibleBitmap(m_hdcBmp,bm.bmWidth, bm.bmHeight);
		hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

		// Get some HDCs that are compatible with the display driver
		hdcMem = CreateCompatibleDC(0);
		hdcMem2 = CreateCompatibleDC(0);
		SelectBitmap(hdcMem, hbmColour);
		SelectBitmap(hdcMem2, hbmMask);
		crTransparent = GetPixel(hdcMem,0,0);
		// Set the background colour of the colour image to the colour
		// you want to be transparent.
		SetBkColor(hdcMem, crTransparent);
		// Copy the bits from the colour image to the B+W mask... everything
		// with the background colour ends up white while everythig else ends up
		// black...Just what we wanted.
		BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
		// Take our new mask and use it to turn the transparent colour in our
		// original colour image to black so the transparency effect will
		// work right.
		BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);
		// Clean up.
		DeleteDC(hdcMem);
		DeleteDC(hdcMem2);
		return hbmMask;
	}

	dcmapHBITMAP CreateMask(dcmapHBITMAP hbmColour, dcmapCOLORREF crTransparent)
	{
		dcmapHDC hdcMem, hdcMem2;
		dcmapHBITMAP hbmMask;
		BITMAP bm;
		// Create monochrome (1 bit) mask bitmap.  
		GetObject(hbmColour, sizeof(BITMAP), &bm);

		//hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
		hdcMem = CreateCompatibleDC(m_hdcBmp);
		hdcMem2 = CreateCompatibleDC(m_hdcBmp);

		hbmMask = CreateCompatibleBitmap(m_hdcBmp,bm.bmWidth, bm.bmHeight);

		// Get some HDCs that are compatible with the display driver
		SelectBitmap(hdcMem, hbmColour);
		SelectBitmap(hdcMem2, hbmMask);

		crTransparent = GetPixel(hdcMem,0,0);

		for(int y=0;y<bm.bmHeight;y++)
		{
			for(int x=0;x<bm.bmWidth;x++)
			{
				dcmapCOLORREF cl = GetPixel(hdcMem,x,y);
				if(cl == crTransparent)
				{
					SetPixel(hdcMem2,x,y,dcmapRGB(255,255,255));
					SetPixel(hdcMem,x,y,dcmapRGB(0,0,0));
				}
				else
				{
					SetPixel(hdcMem2,x,y,dcmapRGB(0,0,0));
				}
			}
		}

		DeleteDC(hdcMem);
		DeleteDC(hdcMem2);
		return hbmMask;
	}

	bool Actualize()
	{
		if(m_pIcons.actualize_state())
		{
			m_bIconsReady = false;
			return true;
		}
		return false;
	}

	void LoadIcons()
	{
		if(m_bIconsReady)return;

		if(!m_pIcons)m_pIcons.init(&g_dcmapCore);
		if(!m_pIcons)return;

		if(!m_hdcMem)return;

		vecIcons.reserve(32);

		int imgdata = m_pIcons->ColumnID("imgdata");
		int bigimgdata = m_pIcons->ColumnID("bigimgdata");
		int transparent = m_pIcons->ColumnID("transparent");
		if(m_pIcons->SelectAll())
		do
		{
			dcmapBYTE* pdta=0;
			int sz = m_pIcons->GetBlob(imgdata,&pdta);
			dcmapBYTE* b_pdta=0;
			int b_sz = m_pIcons->GetBlob(bigimgdata,&b_pdta);

			// Load bitmaps from stream...
			int id = m_pIcons->GetID()->id;

			if(id >= vecIcons.size())
				vecIcons.resize(id+1);

			IconData& ico = vecIcons[id];

			if(ico.hbmp)DeleteBitmap(ico.hbmp);
			if(ico.b_hbmp)DeleteBitmap(ico.b_hbmp);
			if(ico.hmsk)DeleteBitmap(ico.hmsk);
			if(ico.b_hmsk)DeleteBitmap(ico.b_hmsk);

			if(!sz && !b_sz)continue;

			if(sz)	 ico.hbmp = LoadBMP(pdta,m_hdcBmp,ico.w,ico.h);
			else	 ico.hbmp = LoadBMP(b_pdta,m_hdcBmp,ico.w,ico.h);

			if(b_sz) ico.b_hbmp = LoadBMP(b_pdta,m_hdcBmp,ico.b_w,ico.b_h);
			else     ico.b_hbmp = LoadBMP(pdta,m_hdcBmp,ico.b_w,ico.b_h);

			//ico.tr = GetPixel(ico.hdc,0,0);
			ico.trns = m_pIcons->GetInteger(transparent);

			if(ico.trns)
			{
				ico.hmsk = CreateMask(ico.hbmp,ico.tr);
				ico.b_hmsk = CreateMask(ico.b_hbmp,ico.tr);
			}

		}while(m_pIcons->Next());

		m_bIconsReady = true;
	}
};

DCMAP_REGISTER(CDCMapNavView,"PView","MapNavView");



static NavViewShared s_Shared;

const int c_iNumScales = 14;
//static int CellSizeMod[c_iNumScales] = {1,2,3,5,7,11,13,15,19,23,27,33,41,51,61,71,81,101};
static int CellSizeMod[c_iNumScales] = {1,2,3,4,8,10,12,14,18,22,26,32,40,60};

const int c_iDefaultScale = 9;

const int c_iGridScale = 5;

void CDCMapNavView::SetupColors()
{
	m_clGrid1 = dcmapRGB(75,75,75);
	m_clGrid2 = dcmapRGB(20,125,20);
	m_clGrid3 = dcmapRGB(100,255,100);
	m_clGrid4 = dcmapRGB(150,255,150);
	m_clBorder = dcmapRGB(255,200,50);
	m_clInactiveBorder = dcmapRGB(60,60,60);

	m_clLineBackground = dcmapRGB(25,25,25);
	m_clMapBackground = dcmapRGB(0,0,0);

	m_clSelection = dcmapRGB(255,200,100);

	m_clText = dcmapRGB(180,180,180);
	m_clPointedText = dcmapRGB(255,255,255);

	m_clGeoGoodBorder = dcmapRGB(10,100,10);
	m_clGeoGoodFill   = dcmapRGB(0,0,0);

	m_clGeoBadBorder = dcmapRGB(50,40,40);
	m_clGeoBadFill   = dcmapRGB(0,0,0);

	DCMapConfigWrapper cfg(m_pCore->GetConfig());
	cfg.section("MapColor");

	//cfg["HighQuality"] >> m_bHighQuality;

	cfg["Grid1"] >> m_clGrid1;
	cfg["Grid2"] >> m_clGrid2;
	cfg["Grid3"] >> m_clGrid3;
	cfg["Grid4"] >> m_clGrid4;

	cfg["Border"] >> m_clBorder;
	cfg["InactiveBorder"] >> m_clInactiveBorder;

	cfg["LineBackground"] >> m_clLineBackground;
	cfg["MapBackground"] >> m_clMapBackground;

	cfg["Text"] >> m_clText;
	cfg["PointedText"] >> m_clPointedText;

	cfg["Selection"] >> m_clSelection;
	cfg["SelectionPlanet"] >> m_clSelection2;

	cfg["GeoGoodBorder"] >> m_clGeoGoodBorder;
	cfg["GeoGoodFill"] >> m_clGeoGoodFill;
	cfg["GeoBadBorder"] >> m_clGeoBadBorder;
	cfg["GeoBadFill"] >> m_clGeoBadFill;

}


void CDCMapNavView::SetupParams()
{
	if(!m_bWndCreated)return;

	m_iTextOffset=3;

	if(m_iScale<0)m_iScale=0;
	if(m_iScale>=c_iNumScales)m_iScale = c_iNumScales-1;
	m_iCellSize = CellSizeMod[m_iScale];

	m_iGrid1 = 1;
	m_iGrid2 = 25;
	m_iGrid3 = 50;
	m_iGrid4 = 500;

	m_iHGrid1 = 1;
	m_iHGrid2 = 25;
	m_iHGrid3 = 50;

	m_iVGrid1 = 1;
	m_iVGrid2 = 25;
	m_iVGrid3 = 50;

	m_iVLineSize = 40;
	m_iHLineSize = 38;

	m_iVFontSize = 20;
	m_iHFontSize = 18;

	m_iVLineMark = 1;
	m_bVRotateMark=false;

	m_iHLineMark = 1;
	m_bHRotateMark=true;

	if(m_iCellSize < 16)
	{
		if(m_bClassicHLine)
			m_iHFontSize = 16;
		else
			m_iHFontSize = 14;

		m_iVFontSize = 14;
	}

	if(m_iCellSize < 14)
	{
		m_iVFontSize = 20;
		m_iHFontSize = 18;

		m_iHLineMark = 25;
		m_iHGrid1 = 25;

		m_iVLineMark = 25;
		m_iVGrid1 = 25;

		m_iHLineMark = 5;
		m_iHGrid1 = 5;

		m_iVLineMark = 5;
		m_iVGrid1 = 5;
	}

	if(m_iCellSize < 8)
	{
		m_iGrid1 = 25;
		m_iGrid2 = 100;
		m_iGrid3 = 500;

		m_iHLineMark = 25;
		m_iHGrid1 = 25;

		m_iVLineMark = 25;
		m_iVGrid1 = 25;
	}

	if(m_bClassicHLine)
	{
		m_bHRotateMark=false;
		m_iHLineSize=25;

		if(m_iCellSize < 30)
		{
			m_iHFontSize = 16;
		}
		if(m_iCellSize < 26)
		{
			m_iHGrid1 = 2;
			m_iHLineMark = 2;
			m_iHFontSize = 18;
		}
		if(m_iCellSize < 16)
		{
			m_iHFontSize = 16;
		}
		if(m_iCellSize < 14)
		{
			m_iHFontSize = 18;
			m_iHLineMark = 5;
			m_iHGrid1 = 5;
		}

		if(m_iCellSize < 8)
		{
			m_iHLineMark = 25;
			m_iHGrid1 = 25;
		}
		if(m_iCellSize < 2)
		{
			m_iHLineMark = 50;
			m_iHGrid1 = 50;
			//m_iHFontSize = 16;
		}

	}


	m_rcMap.left = m_iVLineSize;
	m_rcMap.top =  m_iHLineSize;

	m_iScrWidth = m_iBmpWidth;
	m_iScrHeight = m_iBmpHeight;

	m_rcMap.bottom = m_iHLineSize+((m_iBmpHeight-m_iHLineSize)/m_iCellSize)*m_iCellSize;
	m_rcMap.right = m_iVLineSize+((m_iBmpWidth-m_iVLineSize)/m_iCellSize)*m_iCellSize;


	m_iWidth = (m_rcMap.right-m_rcMap.left)/m_iCellSize;
	m_iHeight = (m_rcMap.bottom-m_rcMap.top)/m_iCellSize;

	m_iDisplayHeight = m_iBmpHeight;
	m_iDisplayWidth = m_iBmpWidth;

	DeletePen(m_hpnLineGrid1);m_hpnLineGrid1 = CreatePen(PS_SOLID,0,m_clGrid1);
	DeletePen(m_hpnLineGrid2);m_hpnLineGrid2 = CreatePen(PS_SOLID,0,m_clGrid2);
	DeletePen(m_hpnLineGrid3);m_hpnLineGrid3 = CreatePen(PS_SOLID,0,m_clGrid3);
	DeletePen(m_hpnLineGrid4);m_hpnLineGrid4 = CreatePen(PS_SOLID,0,m_clGrid4);

	DeletePen(m_hpnSelection);m_hpnSelection = CreatePen(PS_SOLID,2,m_clSelection);
	DeletePen(m_hpnSelection2);m_hpnSelection2 = CreatePen(PS_SOLID,2,m_clSelection2);

	DeletePen(m_hpnLineBorder);m_hpnLineBorder = CreatePen(PS_SOLID,0,m_clBorder);
	DeletePen(m_hpnInactiveLineBorder);m_hpnInactiveLineBorder = CreatePen(PS_SOLID,0,m_clInactiveBorder);

	DeleteBrush(m_hbrLineBackground);m_hbrLineBackground = CreateSolidBrush(m_clLineBackground);
	DeleteBrush(m_hbrMapBackground);m_hbrMapBackground = CreateSolidBrush(m_clMapBackground);

	LOGFONT fnt;
	ZeroMemory(&fnt,sizeof(fnt));
	wcscpy(fnt.lfFaceName,L"Arial");

	fnt.lfHeight = m_iHFontSize;
	fnt.lfEscapement = m_bHRotateMark?900:0;
	DeleteFont(m_hfntHLineText);m_hfntHLineText = CreateFontIndirect(&fnt);

	fnt.lfHeight = m_iVFontSize;
	fnt.lfEscapement = m_bVRotateMark?900:0;
	DeleteFont(m_hfntVLineText);m_hfntVLineText = CreateFontIndirect(&fnt);


	DeletePen(m_hpnGeoGood);m_hpnGeoGood = CreatePen(PS_SOLID,0,m_clGeoGoodBorder);
	DeletePen(m_hpnGeoBad);m_hpnGeoBad = CreatePen(PS_SOLID,0,m_clGeoBadBorder);

	DeleteBrush(m_hbrGeoGood);m_hbrGeoGood = CreateSolidBrush(m_clGeoGoodFill);
	DeleteBrush(m_hbrGeoBad);m_hbrGeoBad = CreateSolidBrush(m_clGeoBadFill);
}

void DCMAPAPI CDCMapNavView::LoadConfig(IDCMapConfig* pConfig)
{
    DCMapConfigWrapper cfg(pConfig);
    LoadPrefs();

	cfg.section("MapNavView");

	cfg["Left"] >> m_iLeft;  
	cfg["Top"] >> m_iTop;  
	cfg["Scale"] >> m_iScale;  

	if(m_iScale >= c_iNumScales)m_iScale = c_iNumScales-1;

    SetupColors();
	SetupParams();
}

void DCMAPAPI CDCMapNavView::SaveConfig(IDCMapConfig* pConfig)
{
    DCMapConfigWrapper cfg(pConfig);

	cfg.section("MapNavView");

	cfg["Left"] << m_iLeft;  
	cfg["Top"]  << m_iTop;  
	cfg["Scale"] << m_iScale;  

	SaveLayersCfg();
}

void CDCMapNavView::LoadPrefs(IDCMapConfig* pConfig)
{
    DCMapConfigWrapper cfg(pConfig?pConfig:m_pCore->GetConfig());
	cfg.section("MapNavView");
	cfg["HighQuality"] >> m_bHighQuality;
	cfg["ClassicHLine"] >> m_bClassicHLine;
	cfg["SwapButtons"] >> m_bSwapButtons;
	cfg["SelectEmptySpace"] >> m_bSelectEmptySpace;
	cfg["MinIconsCellSize"] >> m_iMinIconsCellSize;

	cfg["RightClickSelect"] >> m_bRightClickSelect;
    cfg["LeftMouseDrag"] >> m_bLeftDragMode;

    cfg["ReadOnly"] >> m_bReadOnly;

	cfg["ToolTipDelay"] >> m_iToolTipDelay;
	cfg["EnableToolTip"] >> m_bEnableToolTip;

    if(!pConfig)
        LoadPrefs(m_pWorkspace->GetConfig());
}

CDCMapNavView::CDCMapNavView(void)
{
	m_bAnimating=false;

	m_bHighQuality=false;
	m_bClassicHLine = false;
	m_bSwapButtons = 0;
	m_bSelectEmptySpace=true;
	m_bRightClickSelect=false;
    m_bLeftDragMode=false;
    m_bReadOnly=false;

	m_iScale=c_iDefaultScale;

	m_iLeft=490;
	m_iTop=490;
	// temp

	m_bHasMouse=false;
	m_iActivelayer=PlanetLayer;
	m_pActiveLayer=0;

	m_bLayerVisiblityChanged=false;

	m_iSelX=-1;
	m_iSelY=-1;

	m_bDragMode=false;
	m_bSpaceDragMode=false;

	m_bActive=false;

	m_iMinIconsCellSize=true;


	m_iToolTipDelay=800;
	m_bEnableToolTip=true;

	m_hWnd=0;

	m_bRenderReady=false;
	m_bWndCreated=false;
}

CDCMapNavView::~CDCMapNavView(void)
{
}

void InvertBtns(WPARAM &wParam)
{
	if((wParam & MK_LBUTTON) && !(wParam & MK_RBUTTON))
	{
		wParam &= ~MK_LBUTTON;
		wParam |= MK_RBUTTON;
	}
	else if((wParam & MK_RBUTTON) && !(wParam & MK_LBUTTON))
	{
		wParam &= ~MK_RBUTTON;
		wParam |= MK_LBUTTON;
	}
}

LRESULT CALLBACK CDCMapNavView::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM  lParam)
{
	CDCMapNavView* pView = (CDCMapNavView*)GetWindowLongPtr(hwnd,GWLP_USERDATA);

    dcmapHDC hdc;
    PAINTSTRUCT ps;
    switch (message)
    {
	case WM_MOUSEHOVER:
		pView->OnMouseEnter();
		return 0;
	case WM_MOUSELEAVE:
		pView->OnMouseExit();
		return 0;
	case WM_ERASEBKGND:
		return 1;
	case WM_GETDLGCODE:
		return DLGC_WANTARROWS|DLGC_WANTCHARS;
		break;
	case WM_TIMER:
		pView->OnHintTimer();
		break;
	case WM_ACTIVATE:
		if(wParam == WA_INACTIVE)pView->OnDeactivate();
		else pView->OnActivate();
		break;
	case WM_KILLFOCUS:
		pView->m_bActive = false;
		pView->OnDeactivate();
		break;
	case WM_SETFOCUS:
		pView->m_bActive = true;
		pView->OnActivate();
		break;
	case WM_KEYDOWN:
		pView->OnKeyDown(wParam,lParam);
		return 0;
		break;
	case WM_KEYUP:
		pView->OnKeyUp(wParam,lParam);
		return 0;
		break;
	case WM_SETCURSOR:
		pView->OnSetCursor();
		return TRUE;
		break;
	case WM_CAPTURECHANGED:
		pView->m_bHasMouse=false;
		pView->OnMouseExit();
		break;
	case WM_MBUTTONDOWN:
		pView->DoSetFocus();
		pView->OnMButtonDown(GET_KEYSTATE_WPARAM(wParam),GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;
	case WM_MBUTTONUP:
		pView->OnMButtonUp(GET_KEYSTATE_WPARAM(wParam),GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONDOWN:
		pView->DoSetFocus();
		if(pView->m_bSwapButtons)InvertBtns(wParam);
		if(pView->m_bSwapButtons)
			pView->OnRButtonDown(GET_KEYSTATE_WPARAM(wParam),GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		else
			pView->OnLButtonDown(GET_KEYSTATE_WPARAM(wParam),GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;
	case WM_RBUTTONDOWN:
		pView->DoSetFocus();
		if(pView->m_bSwapButtons)InvertBtns(wParam);
		if(pView->m_bSwapButtons)
			pView->OnLButtonDown(GET_KEYSTATE_WPARAM(wParam),GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		else
			pView->OnRButtonDown(GET_KEYSTATE_WPARAM(wParam),GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONUP:
		if(pView->m_bSwapButtons)InvertBtns(wParam);
		if(pView->m_bSwapButtons)
			pView->OnRButtonUp(GET_KEYSTATE_WPARAM(wParam),GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		else
			pView->OnLButtonUp(GET_KEYSTATE_WPARAM(wParam),GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;
	case WM_RBUTTONUP:
		if(pView->m_bSwapButtons)InvertBtns(wParam);
		if(pView->m_bSwapButtons)
			pView->OnLButtonUp(GET_KEYSTATE_WPARAM(wParam),GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		else
			pView->OnRButtonUp(GET_KEYSTATE_WPARAM(wParam),GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONDBLCLK:
		if(pView->m_bSwapButtons)
			pView->OnRButtonDoubleClick(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		else
			pView->OnLButtonDoubleClick(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;
	case WM_RBUTTONDBLCLK:
		if(pView->m_bSwapButtons)
			pView->OnLButtonDoubleClick(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		else
			pView->OnRButtonDoubleClick(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;	
	case WM_MOUSEMOVE:
		if(pView->m_bSwapButtons)InvertBtns(wParam);
		pView->OnMouseMove(GET_KEYSTATE_WPARAM(wParam),GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;
	case WM_MOUSEWHEEL:
		if(pView->m_bSwapButtons)InvertBtns(wParam);
		pView->OnWndMouseWheel(GET_KEYSTATE_WPARAM(wParam),GET_WHEEL_DELTA_WPARAM(wParam),
			                GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
        return (0);
	case WM_CREATE:
		SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);
		((CDCMapNavView*)((CREATESTRUCT*)lParam)->lpCreateParams)->m_hWnd = hwnd;
        return (0);
    case WM_PAINT:
        hdc = BeginPaint (hwnd, &ps);
        pView->OnPaint(hdc);
        EndPaint (hwnd, &ps);
        return (0);
    case WM_DESTROY:
        return (0);
		pView->OnMouseExit();
		break;
	case WM_SIZE:
		pView->OnResize(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}

tagWNDCLASSW* DCMAPAPI CDCMapNavView::RegisterWndClass()
{
	HINSTANCE hInst = GetModuleHandle(0);
	static wstring cln; 
	cln = ToWide(typeid(*this).name());

	static tagWNDCLASSW cl;
	ZeroMemory(&cl,sizeof(cl));
	//cl.cbSize = sizeof(cl);
	cl.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS|CS_CLASSDC;
	cl.lpszClassName = cln.c_str();
	//cl.hbrBackground = (dcmapHBRUSH)GetStockObject(BLACK_BRUSH);
	cl.hInstance = hInst;
	cl.lpfnWndProc = WndProc;
	cl.hCursor		= LoadCursor(NULL, IDC_ARROW);
	RegisterClassW(&cl);

	/*static tagWNDCLASSA cla;
	ZeroMemory(&cla,sizeof(cla));
	cla.cbSize = sizeof(cl);
	cla.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS|CS_CLASSDC;
	cla.lpszClassName = cln;
	//cl.hbrBackground = (dcmapHBRUSH)GetStockObject(BLACK_BRUSH);
	cla.hInstance = hInst;
	cla.lpfnWndProc = WndProc;
	cla.hCursor		= LoadCursor(NULL, IDC_ARROW);*/

	return &cl;
}

void* DCMAPAPI CDCMapNavView::BeforeCreate(dcmapHWND hwndParent,dcmapHWND hwndMain)
{
	return this;
}

void  DCMAPAPI CDCMapNavView::AfterCreate(dcmapHWND wnd,dcmapHWND hwndParent,dcmapHWND hwndMain)
{
	m_hWnd = wnd;

	//s_Shared.LoadIcons();

	m_bActive = false;
	m_bWndCreated = true;

	SetupParams();
	LayersMessage(DCMM_MAP_PAN,m_iLeft,m_iTop);
	LayersMessage(DCMM_MAP_SCALE_CHANGED,m_iCellSize,0);
	LayersMessage(DCMM_MAP_VIEW_RESIZE,m_iWidth,m_iHeight);
	Render();
}

dcmapHWND DCMAPAPI CDCMapNavView::Create(dcmapHWND hwndMain,dcmapHWND hwndParent,dcmapRECT const* rcClient)
{
	HINSTANCE hInst = GetModuleHandle(0);

	m_hwndParent = hwndParent;
	m_hwndMain = hwndMain;

	wstring cln = ToWide(typeid(*this).name());

	WNDCLASSEXW cl;
	ZeroMemory(&cl,sizeof(cl));
	cl.cbSize = sizeof(cl);
	cl.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS|CS_CLASSDC;
	cl.lpszClassName = cln.c_str();;
	//cl.hbrBackground = (dcmapHBRUSH)GetStockObject(BLACK_BRUSH);
	cl.hInstance = hInst;
	cl.lpfnWndProc = WndProc;
	cl.hCursor		= LoadCursor(NULL, IDC_ARROW);
	RegisterClassExW(&cl);

	m_iBmpWidth = rcClient->right-rcClient->left;
	m_iBmpHeight = rcClient->bottom-rcClient->top;

	//cln = "EDIT";

	m_hWnd = CreateWindowExW(0,cln.c_str(),L"Wind",WS_CHILD|WS_VISIBLE|WS_TABSTOP,
		rcClient->left,rcClient->top,m_iBmpWidth,m_iBmpHeight,hwndParent,0,hInst,this);

	if (!m_hWnd)return false;

	s_Shared.Resize(m_hWnd,m_iBmpWidth,m_iBmpHeight);
	s_Shared.LoadIcons();

//	m_hdcBmp = CreateCompatibleDC(GetDC(m_hWnd));
//	m_hBmp = CreateCompatibleBitmap(GetDC(m_hWnd),m_iBmpWidth,m_iBmpHeight);
//	m_hPreBmp = SelectBitmap(m_hdcBmp,m_hBmp);

	UpdateWindow(m_hWnd);

	SetupParams();
	Render();

	return m_hWnd;
}

void DCMAPAPI CDCMapNavView::Destroy()
{
	DestroyWindow(m_hWnd);
	//UnregisterClass(typeid(*this).name(),GetModuleHandle(0));
	delete this;
}

void DCMAPAPI CDCMapNavView::OnResize(int w,int h) 
{
	m_iBmpWidth = w;
	m_iBmpHeight = h;
	s_Shared.Resize(m_hWnd,m_iBmpWidth,m_iBmpHeight);

	SetupParams();
	if(m_bWndCreated)
	{
		SetupParams();
		Render();
	}
}

void DCMAPAPI CDCMapNavView::OnResize(dcmapRECT const* rcClient)
{
	if(!m_hWnd)return;

	m_iBmpWidth = rcClient->right-rcClient->left;
	m_iBmpHeight = rcClient->bottom-rcClient->top;

	MoveWindow(m_hWnd,rcClient->left,rcClient->top,m_iBmpWidth,m_iBmpHeight,true);

	s_Shared.Resize(m_hWnd,m_iBmpWidth,m_iBmpHeight);

	SetupParams();
	LayersMessage(DCMM_MAP_VIEW_RESIZE,m_iWidth,m_iHeight);
	Render();
}

void CDCMapNavView::OnPaint(dcmapHDC dc)
{
	BitBlt(dc,0,0,m_iBmpWidth,m_iBmpHeight,s_Shared.m_hdcBmp,0,0,SRCCOPY);
}

void CDCMapNavView::Zoom(int rel,int sile,dcmapSHORT x ,dcmapSHORT y)
{
	bool keeppos = x >=0 && y >= 0;

	int cx = m_iLeft+(x-m_rcMap.left)/m_iCellSize;
	int cy = m_iTop+(y-m_rcMap.top)/m_iCellSize;

	m_iScale+=rel;

	if(m_iScale<0)m_iScale=0;
	if(m_iScale>=c_iNumScales)m_iScale = c_iNumScales-1;

	int ctx = m_iLeft + m_iWidth/2;
	int cty = m_iTop + m_iHeight/2;

	SetupParams();

	if(keeppos)
	{
		int ncx = m_iLeft+(x-m_rcMap.left)/m_iCellSize;
		int ncy = m_iTop+(y-m_rcMap.top)/m_iCellSize;

    	m_iLeft += cx-ncx;
		m_iTop += cy-ncy;
	}
	else
	{
		m_iLeft = ctx-m_iWidth/2;
		m_iTop = cty-m_iHeight/2;
	}

	WrapCoords();
	LayersMessage(DCMM_MAP_PAN,m_iLeft,m_iTop);
	LayersMessage(DCMM_MAP_VIEW_RESIZE,m_iWidth,m_iHeight);
	LayersMessage(DCMM_MAP_SCALE_CHANGED,m_iCellSize,0);
	Render();
}

void CDCMapNavView::OnWndMouseWheel(dcmapWORD fwKeys,dcmapSHORT zDelta, dcmapSHORT x ,dcmapSHORT y)
{
	ResetHint();

	dcmapPOINT mpt={x,y};

	ScreenToClient(m_hWnd,&mpt);
	if(!PtInRect(&m_rcMap,mpt))return;

	DoSetFocus();
	int upd = zDelta > 0?1:-1;
	Zoom(upd,0,mpt.x,mpt.y);

}
bool DCMAPAPI CDCMapNavView::OnMouseWheel(int zDelta, int x,int y)
{
	ResetHint();

	dcmapPOINT mpt={x,y};
	if(!PtInRect(&m_rcMap,mpt))return false;

	DoSetFocus();
	int upd = zDelta > 0?1:-1;
	Zoom(upd,0,mpt.x,mpt.y);

	return true;
}

void CDCMapNavView::OnRButtonDown(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y)
{
	ResetHint();
	SetCapture(m_hWnd);
	m_ptLastMouse.x = x;
	m_ptLastMouse.y = y;
}

void CDCMapNavView::OnLButtonDown(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y)
{
	ResetHint();
	SetCapture(m_hWnd);

	m_ptLastMouse.x = x;
	m_ptLastMouse.y = y;

	if(m_bDragMode || m_bSpaceDragMode)return;

	dcmapPOINT pt = {x,y};
	if(!PtInRect(&m_rcMap,pt))return;

	int cx = (m_iLeft+(x-m_rcMap.left)/m_iCellSize)%g_DCMapNumPlanets+1;
	int cy = (m_iTop+(y-m_rcMap.top)/m_iCellSize)%g_DCMapNumPlanets+1;

	if(!m_pActiveLayer)
	{
		DCMapPlanetDisplayInfo* pinf = m_pDataProcessor->GetPlanet(cx,cy);
		if(pinf)
		{
			m_pPlanetMan->SetCurrentPlanet(&pinf->planet.id,cx,cy);
		}
		else if(m_bSelectEmptySpace)
		{
			m_pPlanetMan->SetCurrentPlanet(0,cx,cy);
		}
	}
}

void CDCMapNavView::OnLButtonUp(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y)
{
	ReleaseCapture();
	ResetHint();

	if(m_bDragMode && !(fwKeys&MK_RBUTTON))
	{
		m_bDragMode = false;
		SetCursor(s_Shared.hcArrow);
		if(m_bHasMouse)
		{
			ReleaseCapture();
			m_bHasMouse=false;
		}
	}
	else
	{
		dcmapPOINT pt = {x,y};
		if(!PtInRect(&m_rcMap,pt))return;

		int cx = (m_iLeft+(x-m_rcMap.left)/m_iCellSize)%g_DCMapNumPlanets+1;
		int cy = (m_iTop+(y-m_rcMap.top)/m_iCellSize)%g_DCMapNumPlanets+1;

		if(m_pActiveLayer)
		{
			m_pActiveLayer->OnMessage(DCMM_MAP_CLICK,cx,cy,this);
		}
		else
		{
			DCMapPlanetDisplayInfo* pinf = m_pDataProcessor->GetPlanet(cx,cy);
			if(pinf)
			{
				m_pPlanetMan->SetCurrentPlanet(&pinf->planet.id,cx,cy);
			}
			else if(m_bSelectEmptySpace)
			{
				m_pPlanetMan->SetCurrentPlanet(0,cx,cy);
			}
		}
	}
}


void CDCMapNavView::OnRButtonUp(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y)
{
	ReleaseCapture();
	ResetHint();

	if(m_bDragMode) 
	{
		if(!fwKeys&MK_LBUTTON)
		{
			m_bDragMode = false;
			SetCursor(s_Shared.hcArrow);
			if(m_bHasMouse)
			{
				m_bHasMouse=false;
			}
		}
	}
	else if(!m_bSpaceDragMode)
	{
		dcmapPOINT pt = {x,y};
		if(!PtInRect(&m_rcMap,pt))return;

		int cx = (m_iLeft+(x-m_rcMap.left)/m_iCellSize)%g_DCMapNumPlanets+1;
		int cy = (m_iTop+(y-m_rcMap.top)/m_iCellSize)%g_DCMapNumPlanets+1;


		if(m_pActiveLayer)
		{
			m_pActiveLayer->OnMessage(DCMM_MAP_RCLICK,cx,cy,this);
		}
		else
		{
			m_ptCurrentCell.x =cx;
			m_ptCurrentCell.y =cy;

			DCMapPlanetDisplayInfo* pinf = m_pDataProcessor->GetPlanet(cx,cy);
			if(pinf)
			{
				if(m_bRightClickSelect) m_pPlanetMan->SetCurrentPlanet(&pinf->planet.id,cx,cy);
				m_pPlanetPopup->Popup();
			}
			else
			{
				if(m_bRightClickSelect) m_pPlanetMan->SetCurrentPlanet(0,cx,cy);
				m_pCellPopup->Popup();
			}
			//m_pPlanetMan->GetCurrentPlanetCoords((int*)&m_ptCurrentCell.x,(int*)&m_ptCurrentCell.y);
		}
	}
}

void CDCMapNavView::OnSetCursor()
{
	if(m_bDragMode || m_bSpaceDragMode)
	{
		SetCursor(s_Shared.hcHand);
	}
	else
	{
		SetCursor(s_Shared.hcArrow);
	}
}


void  CDCMapNavView::LayersMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam)
{
	for(int i=LastLayer+1;i<m_Layers.size();i++)
	{
		if(m_Layers[i].m_pLayerIface)
			m_Layers[i].m_pLayerIface->OnMessage(Msg,wParam,lParam,this);
	}
}

void CDCMapNavView::OnLButtonDoubleClick(dcmapSHORT x,dcmapSHORT y)
{
	if(m_bDragMode || m_bSpaceDragMode)return;

	dcmapPOINT pt = {x,y};
	if(!PtInRect(&m_rcMap,pt))return;

	SetFocus(m_hWnd);

	int cx = (m_iLeft+(x-m_rcMap.left)/m_iCellSize)%g_DCMapNumPlanets+1;
	int cy = (m_iTop+(y-m_rcMap.top)/m_iCellSize)%g_DCMapNumPlanets+1;

	if(m_pActiveLayer)
	{
		m_pActiveLayer->OnMessage(DCMM_MAP_DCLICK,cx,cy,this);
		return;
	}

	DCMapPlanetDisplayInfo* pinf = m_pDataProcessor->GetPlanet(cx,cy);
	if(pinf)
	{
		if(!m_bReadOnly)m_pPlanetMan->EditPlanet(pinf->planet.id);
		//SetFocus(m_hWnd);
		OnMouseMove(0,x,y);
	}
	else
	{
		if(!m_bReadOnly)m_pPlanetMan->EditPlanetAt(cx,cy);
		//SetFocus(m_hWnd);
		DCMapPlanetDisplayInfo* pinf = m_pDataProcessor->GetPlanet(cx,cy);
		if(pinf)
		{
			m_pPlanetMan->SetCurrentPlanet(&pinf->planet.id,cx,cy);
		}
		else
		{
			m_pPlanetMan->SetCurrentPlanet(0,cx,cy);
		}
	}
	//m_pWorkspace->GetUI()->ActivateView();
}

void CDCMapNavView::OnRButtonDoubleClick(dcmapSHORT x,dcmapSHORT y)
{

}

void CDCMapNavView::WrapCoords()
{
	if(m_iLeft < 0)m_iLeft=g_DCMapNumPlanets+m_iLeft; 
	if(m_iTop < 0)m_iTop=g_DCMapNumPlanets+m_iTop; 

	if(m_iLeft >= g_DCMapNumPlanets)m_iLeft=m_iLeft-g_DCMapNumPlanets; 
	if(m_iTop  >= g_DCMapNumPlanets)m_iTop=m_iTop-g_DCMapNumPlanets; 
}

void CDCMapNavView::OnMouseEnter()
{
	//SetFocus(m_hWnd);
}
void CDCMapNavView::OnMouseExit()
{
	m_ptPointedCell.x = -1;
	m_ptPointedCell.y = -1;
	ResetHint();
}

void CDCMapNavView::ScrollMap(dcmapPOINT const& ptUpdate)
{
	if(!ptUpdate.x && !ptUpdate.y) return;

	m_iLeft += ptUpdate.x;
	m_iTop += ptUpdate.y;

	LayersMessage(DCMM_MAP_PAN,m_iLeft,m_iTop);

	WrapCoords();

	int dx = -ptUpdate.x*m_iCellSize;
	int dy = -ptUpdate.y*m_iCellSize;

	ScrollWindow(m_hWnd,dx,dy,&m_rcMap,&m_rcMap);
	ScrollDC(s_Shared.m_hdcBmp,dx,dy,&m_rcMap,&m_rcMap,0,0);

	// calculate update x line:

	int ys=m_rcMap.top;
	int ye=m_rcMap.bottom;

	dcmapRECT rcYUpdate;
	if(ptUpdate.y < 0) // up
	{
		rcYUpdate.left = m_rcMap.left;
		rcYUpdate.right = m_rcMap.right;
		rcYUpdate.top = m_rcMap.top;
		rcYUpdate.bottom = m_rcMap.top+dy;
		ys=rcYUpdate.bottom;
		RenderMap(&rcYUpdate);
	}
	else if(ptUpdate.y > 0) // up
	{
		rcYUpdate.left = m_rcMap.left;
		rcYUpdate.right = m_rcMap.right;
		rcYUpdate.bottom = m_rcMap.bottom;
		rcYUpdate.top = m_rcMap.bottom+dy;
		ye=rcYUpdate.top;
		RenderMap(&rcYUpdate);
	}


	dcmapRECT rcXUpdate;
	if(ptUpdate.x < 0) // up
	{
		rcXUpdate.top = ys;
		rcXUpdate.bottom = ye;

		rcXUpdate.left = m_rcMap.left;
		rcXUpdate.right = m_rcMap.left+dx;
		RenderMap(&rcXUpdate);
	}
	else if(ptUpdate.x > 0) // up
	{
		rcXUpdate.top = ys;
		rcXUpdate.bottom = ye;

		rcXUpdate.left = m_rcMap.right+dx;
		rcXUpdate.right = m_rcMap.right;
		RenderMap(&rcXUpdate);
	}

	if(ptUpdate.y)RenderVLine(m_rcMap.top,m_rcMap.bottom);
	if(ptUpdate.x)RenderHLine(m_rcMap.left,m_rcMap.right);

	UpdateWindow(m_hWnd);
}

void CDCMapNavView::OnMouseMove(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y)
{
	dcmapPOINT mpt={x,y};
	if(m_bDragMode)
	{

	}
	else if(!m_bHasMouse)
	{
		if(PtInRect(&m_rcMap,mpt))
		{
			m_bHasMouse=true;
			//SetCapture(m_hWnd);
			//m_bHasMouse=true;
			//OnMouseEnter();

			//if(!m_bActive)SetFocus(m_hWnd);
		}
		else return;
	}
	else
	{
		if(!PtInRect(&m_rcMap,mpt) && !m_bDragMode)
		{
			//ReleaseCapture();
			m_bHasMouse=false;
			ResetHint();
			m_ptPointedCell.x=-1;
			m_ptPointedCell.y=-1;
			return;
		}
	}

	int cx = (m_iLeft+(x-m_rcMap.left)/m_iCellSize)%g_DCMapNumPlanets+1;
	int cy = (m_iTop+(y-m_rcMap.top)/m_iCellSize)%g_DCMapNumPlanets+1;

	dcmapPOINT ptUpdate = {0,0};
	if(fwKeys&MK_RBUTTON || (m_bSpaceDragMode||m_bLeftDragMode && fwKeys&MK_LBUTTON) || m_bDragMode)
	{
		if(1) // wrap cursor across screen borders
		{
			dcmapPOINT pt;
			GetCursorPos(&pt);
			dcmapHDC dc = GetWindowDC(m_hWnd);
			int w = GetDeviceCaps(dc, HORZRES)-1;
			int h = GetDeviceCaps(dc, VERTRES)-1;
			if(pt.x == 0){SetCursorPos(w-1,pt.y);x+=w-1;m_ptLastMouse.x+=w-1;}
			else if(pt.x == w){SetCursorPos(1,pt.y);x-=w-1;m_ptLastMouse.x-=w-1;}
			if(pt.y == 0){SetCursorPos(pt.x,h-1);y+=h-1;m_ptLastMouse.y+=h-1;}
			else if(pt.y == h){SetCursorPos(pt.x,1);y-=h-1;m_ptLastMouse.y-=h-1;}
			ReleaseDC(m_hWnd,dc);
		}

		int ux = (m_ptLastMouse.x-x)/m_iCellSize;
		int uy = (m_ptLastMouse.y-y)/m_iCellSize;

		if(abs(ux))
		{
			ptUpdate.x = ux;
			m_ptLastMouse.x-=ux*m_iCellSize;
		}
		if(abs(uy))
		{
			ptUpdate.y = uy;
			m_ptLastMouse.y-=uy*m_iCellSize;
		}

	}
	else
	{
		m_ptLastMouse.x=x;
		m_ptLastMouse.y=y;

		// Simply draw highlighted numbers in rule lines

		bool changed = false;
		if(m_ptPointedCell.x != cx)
		{
			m_ptPointedCell.x = cx;
			RenderHLine(m_rcMap.left,m_rcMap.right);
			changed=true;
		}
		if(m_ptPointedCell.y != cy)
		{
			m_ptPointedCell.y = cy;
			RenderVLine(m_rcMap.top,m_rcMap.bottom);
			changed=true;
		}
		if(changed)// Notify of pointed planet or cell change
		{
			m_pWorkspace->BroadcastMessage(DCMM_POINT_PLANET,m_ptPointedCell.x,m_ptPointedCell.y,this);
			// setup hint showing
			SetupHint(cx,cy);
		}
	}

	if(ptUpdate.x || ptUpdate.y)
	{	
		if(!m_bDragMode)
		{
			m_bDragMode = true;
			SetCursor(s_Shared.hcHand);
			if(!m_bHasMouse)
			{
				SetCapture(m_hWnd);
				m_bHasMouse=true;
			}
		}
		ScrollMap(ptUpdate);
	}
}

void CDCMapNavView::RenderFocus(dcmapHDC dc)
{
	dcmapHPEN hpnPre;
	dcmapHBRUSH hbrPre;
	hpnPre = SelectPen(dc,m_bActive?m_hpnLineBorder:m_hpnInactiveLineBorder);
	hbrPre = SelectBrush(dc,GetStockBrush(NULL_BRUSH));
	Rectangle(dc,m_rcMap.left-1,m_rcMap.top-1,m_rcMap.right+1,m_rcMap.bottom+1);
	SelectPen(dc,hpnPre);
	SelectBrush(dc,hbrPre);
}

void CDCMapNavView::RenderVLine(int from,int to)
{
	dcmapHDC m_hdcBmp = s_Shared.m_hdcBmp;

	dcmapHPEN hpnPre;
	dcmapHFONT hfntPre;

	int start = (from-m_rcMap.top)/m_iCellSize;
	int end =   (to-m_rcMap.top)/m_iCellSize;


	dcmapRECT rc = {0,from,m_rcMap.left,to};
	FillRect(m_hdcBmp,&rc,m_hbrLineBackground);
	InvalidateRect(m_hWnd,&rc,FALSE);

	//hpnLine = GetStockPen(WHITE_PEN);
	hpnPre = SelectPen(m_hdcBmp,m_bActive?m_hpnLineBorder:m_hpnInactiveLineBorder);

	MoveToEx(m_hdcBmp,m_rcMap.left-1,from,0);
	LineTo(m_hdcBmp,m_rcMap.left-1,to);

	// hlines
	hfntPre = SelectFont(m_hdcBmp,m_hfntVLineText);

	SetBkMode(m_hdcBmp, TRANSPARENT); 

	int s = ((m_iTop+start+1)/m_iVGrid1)*m_iVGrid1-m_iTop-1;
	if(s<start)s+=m_iVGrid1;
	for(int i=s;i<end;i+=m_iVGrid1)
	{
		int pos = i*m_iCellSize+m_rcMap.top;
		int ps = (m_iTop+i)%g_DCMapNumPlanets+1;

		if(ps%m_iVGrid3 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid3);
		else if(ps%m_iVGrid2 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid2);
		else if(ps%m_iVGrid1 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid1);

		MoveToEx(m_hdcBmp,0,pos,0);
		LineTo(m_hdcBmp,m_rcMap.left-1,pos);

		if(ps%m_iVLineMark == 0)
		{
			if(ps == m_ptPointedCell.y && m_iVLineMark==1)
				SetTextColor(m_hdcBmp, m_clPointedText);
			else 
				SetTextColor(m_hdcBmp, m_clText);

			wchar_t buf[8];
			wsprintf(buf,L"%d",ps);
			//TextOut(m_hdcBmp,m_iTextOffset,pos,buf,strlen(buf));

			int Size = m_iCellSize*m_iVGrid1;

			dcmapRECT rc = {m_iTextOffset,pos,m_iTextOffset+m_iVLineSize-(m_iTextOffset+1),pos+Size};
			DrawText(m_hdcBmp,buf,wcslen(buf),&rc,DT_RIGHT);
		}
	}
	SelectFont(m_hdcBmp,hfntPre);
	SelectPen(m_hdcBmp,hpnPre);
}

void CDCMapNavView::RenderHLine(int from,int to)
{
	dcmapHDC m_hdcBmp = s_Shared.m_hdcBmp;

	dcmapHPEN hpnPre;
	dcmapHFONT hfntPre;

	int start = (from-m_rcMap.left)/m_iCellSize;
	int end =   (to-m_rcMap.left)/m_iCellSize;

	dcmapRECT rc = {from,0,to,m_rcMap.top};
	FillRect(m_hdcBmp,&rc,m_hbrLineBackground);
	InvalidateRect(m_hWnd,&rc,FALSE);

	hpnPre = SelectPen(m_hdcBmp,m_bActive?m_hpnLineBorder:m_hpnInactiveLineBorder);
	MoveToEx(m_hdcBmp,from,m_rcMap.top-1,0);
	LineTo(m_hdcBmp,to,m_rcMap.top-1);

	// hlines
	hfntPre = SelectFont(m_hdcBmp,m_hfntHLineText);

	SetBkMode(m_hdcBmp, TRANSPARENT); 

	int s = ((m_iLeft+start+1)/m_iHGrid1)*m_iHGrid1-m_iLeft-1;
	if(s<start)s+=m_iHGrid1;
	for(int i=s;i<end;i+=m_iHGrid1)
	{
		int ps = (m_iLeft+i)%g_DCMapNumPlanets+1;

		int pos = i*m_iCellSize+m_rcMap.left;

		if(ps%m_iHGrid3 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid3);
		else if(ps%m_iHGrid2 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid2);
		else if(ps%m_iHGrid1 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid1);

		MoveToEx(m_hdcBmp,pos,0,0);
		LineTo(m_hdcBmp,pos,m_rcMap.top-1);

		if(ps%m_iHLineMark == 0)
		{
			if(ps == m_ptPointedCell.x && m_iHLineMark==1)
				SetTextColor(m_hdcBmp, m_clPointedText);
			else 
				SetTextColor(m_hdcBmp, m_clText);

			wchar_t buf[8];
			wsprintf(buf,L"%d",ps);
			if(m_bHRotateMark)
			{
				dcmapRECT rc = {pos,m_iVLineSize-m_iTextOffset,pos+m_iCellSize,m_iTextOffset};
				
				DrawText(m_hdcBmp,buf,wcslen(buf),&rc,DT_NOCLIP);
				//TextOut(m_hdcBmp,pos+m_iTextOffset,m_rcMap.top-m_iTextOffset,buf,strlen(buf));
			}
			else
			{
				TextOut(m_hdcBmp,pos+m_iTextOffset,m_rcMap.top-m_iHFontSize-m_iTextOffset,buf,wcslen(buf));
			}
		}
	}
	SelectFont(m_hdcBmp,hfntPre);
	SelectPen(m_hdcBmp,hpnPre);

}

void CDCMapNavView::PreRenderMap(dcmapRECT const* rcArea)
{
	dcmapHDC m_hdcBmp = s_Shared.m_hdcBmp;

	FillRect(m_hdcBmp,rcArea,m_hbrMapBackground);
	InvalidateRect(m_hWnd,rcArea,FALSE);

	dcmapRECT rcWnd;
	rcWnd.left = (rcArea->left-m_rcMap.left)/m_iCellSize;
	rcWnd.right =   (rcArea->right-m_rcMap.left)/m_iCellSize;
	rcWnd.top = (rcArea->top-m_rcMap.top)/m_iCellSize;
	rcWnd.bottom =   (rcArea->bottom-m_rcMap.top)/m_iCellSize;

	rcWnd.left += m_iLeft;
	rcWnd.right += m_iLeft;
	rcWnd.top += m_iTop;
	rcWnd.bottom += m_iTop;


	for(int i=LastLayer+1;i<m_Layers.size();i++)
	{
		if(!m_Layers[i].m_pLayerIface)continue;
		int Flags=0;
		if(!m_Layers[i].bVisible)Flags |= DCMAP_LRF_HIDDEN;

		dcmapRECT rc = rcWnd;

		int w = rcWnd.right-rcWnd.left;
		if(rc.right > g_DCMapNumPlanets)rc.right = g_DCMapNumPlanets;

		while(w)
		{
			rc.top = rcWnd.top;
			rc.bottom = rcWnd.bottom;
			int h = rcWnd.bottom-rcWnd.top;
			if(rc.bottom > g_DCMapNumPlanets)rc.bottom = g_DCMapNumPlanets;
			while(h)
			{
				dcmapRECT area;
				area.left = rcArea->left + (rc.left-rcWnd.left)*m_iCellSize;
				area.right = rcArea->right + (rc.right-rcWnd.right)*m_iCellSize;
				area.top = rcArea->top + (rc.top-rcWnd.top)*m_iCellSize;
				area.bottom = rcArea->bottom + (rc.bottom-rcWnd.bottom)*m_iCellSize;

				dcmapRECT rrc;
				rrc.left = rc.left%g_DCMapNumPlanets;
				rrc.right = rc.right%g_DCMapNumPlanets;
				rrc.top = rc.top%g_DCMapNumPlanets;
				rrc.bottom = rc.bottom%g_DCMapNumPlanets;

				if(rrc.right == 0)rrc.right = g_DCMapNumPlanets;
				if(rrc.bottom == 0)rrc.bottom = g_DCMapNumPlanets;

				m_Layers[i].m_pLayerIface->PreRender(m_hdcBmp,area,rrc,m_iCellSize,Flags);

				h-= rc.bottom-rc.top;

				rc.top = rc.bottom;
				rc.bottom = rcWnd.bottom;
			}

			w-= rc.right-rc.left;

			rc.left = rc.right;
			rc.right = rcWnd.right;
		}
	}
	
}


void CDCMapNavView::RenderMap(dcmapRECT const* rcArea)
{
	if(!m_Layers.size()) return;
	if(!m_bRenderReady)return;

	PreRenderMap(rcArea);

	dcmapHDC m_hdcBmp = s_Shared.m_hdcBmp;

	dcmapHPEN hpnPre;
	dcmapHBRUSH hbrPre;

	dcmapRECT rcWnd;
	rcWnd.left = (rcArea->left-m_rcMap.left)/m_iCellSize;
	rcWnd.right =   (rcArea->right-m_rcMap.left)/m_iCellSize;

	rcWnd.top = (rcArea->top-m_rcMap.top)/m_iCellSize;
	rcWnd.bottom =   (rcArea->bottom-m_rcMap.top)/m_iCellSize;

	bool grid = m_Layers[GridLayer].bVisible;
	bool bgrid = m_Layers[BigGridLayer].bVisible;
	if(grid || bgrid)
	{
		// vertical grid
		hpnPre = SelectPen(m_hdcBmp,m_hpnLineGrid1);

		int start;
		
		start = ((m_iLeft+rcWnd.left+1)/m_iGrid1)*m_iGrid1-m_iLeft-1;
		for(int i=start;i<rcWnd.right;i+=m_iGrid1)
		{
			int ps = i+m_iLeft+1;
			int pos = i*m_iCellSize+m_rcMap.left;

			if(!bgrid)SelectPen(m_hdcBmp,m_hpnLineGrid1);
			else if(ps%m_iGrid4 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid4);
			else if(ps%m_iGrid3 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid3);
			else if(ps%m_iGrid2 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid2);
			else if(ps%m_iGrid1 == 0)
			{
				if(!grid)continue;
				SelectPen(m_hdcBmp,m_hpnLineGrid1);
			}

			MoveToEx(m_hdcBmp,pos,rcArea->top,0);
			LineTo(m_hdcBmp,pos,rcArea->bottom);
		}
		// horizontal grid

		start = ((m_iTop+rcWnd.top+1)/m_iGrid1)*m_iGrid1-m_iTop-1;
		for(int i=start;i<rcWnd.bottom;i+=m_iGrid1)
		{
			int ps = i+m_iTop+1;
			int pos = i*m_iCellSize+m_rcMap.top;

			if(!bgrid)SelectPen(m_hdcBmp,m_hpnLineGrid1);
			else if(ps%m_iGrid4 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid4);
			else if(ps%m_iGrid3 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid3);
			else if(ps%m_iGrid2 == 0)SelectPen(m_hdcBmp,m_hpnLineGrid2);
			else if(ps%m_iGrid1 == 0)
			{
				if(!grid)continue;
				SelectPen(m_hdcBmp,m_hpnLineGrid1);
			}

			MoveToEx(m_hdcBmp,rcArea->left,pos,0);
			LineTo(m_hdcBmp,rcArea->right,pos);
		}
		SelectPen(m_hdcBmp,hpnPre);
	}

	if(!m_pDataProcessor) return;

	if(m_Layers[GeoLayer].bVisible && m_iCellSize > 5)
	{
		for(int y=rcWnd.top;y<rcWnd.bottom;y++)
		{
			int ps = (m_iTop+y)%g_DCMapNumPlanets;

			int from = m_iLeft+rcWnd.left;
			int width   = rcWnd.right-rcWnd.left;
			int update=0;
			do 
			{
				if(from >= g_DCMapNumPlanets)from=from-g_DCMapNumPlanets;
				int to = from+width;
				if(to >= g_DCMapNumPlanets) 
					to = g_DCMapNumPlanets-1;
				int cy = y*m_iCellSize+m_rcMap.top;

				DCMapPlanetParamInfo const* inf;

				if(m_pGeo->SelectLine(ps+1,from+1,to+1))
				{
					do
					{
						inf = m_pGeo->GetData();
						int sp = (inf->x-1-from)+update;
						int cx = sp*m_iCellSize+rcArea->left;
						RenderGeo(inf,cx,cy);
					} 
					while(m_pGeo->Next());
				}
				//while(inf = m_pDataProcessor->GetNextAsDisplayInfo())
				width -= (to-from);
				update+=(to-from)+1;
				from = to+1;
			} while(width>0);
		}
	}

	// Render Cells
	if(m_Layers[PlanetLayer].bVisible)
	{
		for(int y=rcWnd.top;y<rcWnd.bottom;y++)
		{
			int ps = (m_iTop+y)%g_DCMapNumPlanets;

			int from = m_iLeft+rcWnd.left;
			int width   = rcWnd.right-rcWnd.left;
			int update=0;
			do 
			{
				if(from >= g_DCMapNumPlanets)from=from-g_DCMapNumPlanets;
				int to = from+width;
				if(to >= g_DCMapNumPlanets) 
					to = g_DCMapNumPlanets-1;
				int cy = y*m_iCellSize+m_rcMap.top;
				DCMapPlanetDisplayInfo* inf;

				if(m_pDataProcessor->SelectLine(ps+1,from+1,to+1))
				{
					do
					{
						inf = m_pDataProcessor->GetPlanetInfo();
						int sp = (inf->planet.x-1-from)+update;
						int cx = sp*m_iCellSize+rcArea->left;

						// Render planet
						//if(m_bHighQuality) RenderPlanetHQ(inf,cx,cy);
						//else  RenderPlanet(inf,cx,cy);

						RenderPlanet(inf,cx,cy);
					} 
					while(m_pDataProcessor->Next());
				}
				//while(inf = m_pDataProcessor->GetNextAsDisplayInfo())
				width -= (to-from);
				update+=(to-from)+1;
				from = to+1;
			} while(width>0);
		}
	}

	// Render Selection
	for(int y=rcWnd.top;y<rcWnd.bottom;y++)
	{
		int ps = (m_iTop+y)%g_DCMapNumPlanets;

		int from = m_iLeft+rcWnd.left;
		int width   = rcWnd.right-rcWnd.left;
		int update=0;
		do 
		{
			if(from >= g_DCMapNumPlanets)from=from-g_DCMapNumPlanets;
			int to = from+width;
			if(to >= g_DCMapNumPlanets)to = g_DCMapNumPlanets-1;
			int cy = y*m_iCellSize+m_rcMap.top;

			// render single selection rect
			if(m_iSelY == ps+1 && m_iSelX >= from+1 &&  m_iSelX <= to+1)
			{ // Render Selectiom

				if(m_iSelType==1)
					hpnPre = SelectPen(m_hdcBmp,m_hpnSelection2);
				else
					hpnPre = SelectPen(m_hdcBmp,m_hpnSelection);

				hbrPre = SelectBrush(m_hdcBmp,GetStockBrush(NULL_BRUSH));

				int sp = (m_iSelX-1-from)+update;
				int cx = sp*m_iCellSize+rcArea->left;
				Rectangle(m_hdcBmp,cx+2,cy+2,cx+m_iCellSize,cy+m_iCellSize);

				SelectPen(m_hdcBmp,hpnPre);
				SelectBrush(m_hdcBmp,hbrPre);
			}
			width -= (to-from);
			update+=(to-from)+1;
			from = to+1;
		} while(width>0);
	}

	// Render icons
	if(m_Layers[IconsLayer].bVisible)
	{
		s_Shared.LoadIcons();

		int la = 0;
		if(m_iCellSize<m_iMinIconsCellSize)return;
		if(m_iCellSize < 10)
		{
			la = 5/m_iCellSize;
		}

		for(int y=rcWnd.top-la;y<rcWnd.bottom+la;y++)
		{
			int ps = (m_iTop+y)%g_DCMapNumPlanets;

			int from = m_iLeft+rcWnd.left-la;
			int width   = rcWnd.right-rcWnd.left+2*la;
			int update=0;
			do 
			{
				if(from >= g_DCMapNumPlanets)from=from-g_DCMapNumPlanets;
				int to = from+width;
				if(to >= g_DCMapNumPlanets) 
					to = g_DCMapNumPlanets-1;
				int cy = y*m_iCellSize+m_rcMap.top;
				DCMapPlanetDisplayInfo* inf;

				if(m_pDataProcessor->SelectLine(ps+1,from+1,to+1))
				{
					do
					{
						inf = m_pDataProcessor->GetPlanetInfo();
						int sp = (inf->planet.x-1-from)+update-la;
						int cx = sp*m_iCellSize+rcArea->left;

						RenderIcons(inf,cx,cy);
					} 
					while(m_pDataProcessor->Next());
				}
				width -= (to-from);
				update+=(to-from)+1;
				from = to+1;
			} while(width>0);
		}
	}

	rcWnd.left += m_iLeft;
	rcWnd.right += m_iLeft;
	rcWnd.top += m_iTop;
	rcWnd.bottom += m_iTop;

	for(int i=LastLayer+1;i<m_Layers.size();i++)
	{
		if(!m_Layers[i].bVisible)continue;
		if(!m_Layers[i].m_pLayerIface)continue;

		int Flags=0;
		if(!m_Layers[i].bVisible)Flags |= DCMAP_LRF_HIDDEN;

		dcmapRECT rc = rcWnd;

		int w = rcWnd.right-rcWnd.left;
		if(rc.right >= g_DCMapNumPlanets)rc.right = g_DCMapNumPlanets;

		while(w)
		{
			rc.top = rcWnd.top;
			rc.bottom = rcWnd.bottom;
			int h = rcWnd.bottom-rcWnd.top;
			if(rc.bottom > g_DCMapNumPlanets)rc.bottom = g_DCMapNumPlanets;
			while(h)
			{
				dcmapRECT area;
				area.left = rcArea->left + (rc.left-rcWnd.left)*m_iCellSize;
				area.right = rcArea->right + (rc.right-rcWnd.right)*m_iCellSize;
				area.top = rcArea->top + (rc.top-rcWnd.top)*m_iCellSize;
				area.bottom = rcArea->bottom + (rc.bottom-rcWnd.bottom)*m_iCellSize;

				dcmapRECT rrc;
				rrc.left = rc.left%g_DCMapNumPlanets;
				rrc.right = rc.right%g_DCMapNumPlanets;
				rrc.top = rc.top%g_DCMapNumPlanets;
				rrc.bottom = rc.bottom%g_DCMapNumPlanets;

				if(rrc.right == 0)rrc.right = g_DCMapNumPlanets;
				if(rrc.bottom == 0)rrc.bottom = g_DCMapNumPlanets;

				m_Layers[i].m_pLayerIface->Render(m_hdcBmp,area,rrc,m_iCellSize,Flags);

				h-= rc.bottom-rc.top;

				rc.top = rc.bottom;
				rc.bottom = rcWnd.bottom;
			}

			w-= rc.right-rc.left;

			rc.left = rc.right;
			rc.right = rcWnd.right;
		}
		//if(rc.bottom > DCMapNumPlanets)rc.bottom = DCMapNumPlanets;
	}
}

void CDCMapNavView::RenderBorder()
{
	dcmapHDC m_hdcBmp = s_Shared.m_hdcBmp;
	dcmapHPEN hpnPre;
	hpnPre = SelectPen(m_hdcBmp,m_bActive?m_hpnLineBorder:m_hpnInactiveLineBorder);

	MoveToEx(m_hdcBmp,m_iVLineSize,m_rcMap.bottom,0);
	LineTo(m_hdcBmp,m_rcMap.right,m_rcMap.bottom);

	MoveToEx(m_hdcBmp,m_rcMap.right,m_iHLineSize,0);
	LineTo(m_hdcBmp,m_rcMap.right,m_rcMap.bottom);

	SelectPen(m_hdcBmp,hpnPre);
}

void CDCMapNavView::Render()
{
	if(!m_bWndCreated)return;

	dcmapHDC m_hdcBmp = s_Shared.m_hdcBmp;

	// Draw right lines
	dcmapRECT rcTop = {0,0,m_rcMap.left,m_rcMap.top};
	FillRect(m_hdcBmp,&rcTop,m_hbrLineBackground);
	InvalidateRect(m_hWnd,&rcTop,FALSE);

	// Bottom line
	dcmapRECT rcBottom = {0,m_rcMap.bottom,m_iScrWidth,m_iScrHeight};
	FillRect(m_hdcBmp,&rcBottom,m_hbrLineBackground);
	InvalidateRect(m_hWnd,&rcBottom,FALSE);

	// Left Line
	dcmapRECT rcLeft = {m_rcMap.right,0,m_iScrWidth,m_rcMap.bottom};
	FillRect(m_hdcBmp,&rcLeft,m_hbrLineBackground);
	InvalidateRect(m_hWnd,&rcLeft,FALSE);

	RenderBorder();

	RenderMap(&m_rcMap);
	RenderVLine(m_rcMap.top,m_rcMap.bottom);
	RenderHLine(m_rcMap.left,m_rcMap.right);

	//InvalidateRect(m_hWnd,0,TRUE);
}

void CDCMapNavView::DrawIcon(IconData const &ico,int x,int y,bool bounded,bool left,bool top,int& bw,int &bh)
{
	bool center = ico.w >= bw || ico.h >= bh;
	if(center && bounded)return; // do not draw bounded

	bool big = ico.b_w <= bw-1 && ico.b_h <= bh-1 && 
		m_iCellSize - ico.b_w > ico.w && m_iCellSize - ico.b_h > ico.h;

	int w,h;
	if(big){w=ico.b_w;h=ico.b_h;}
	else{w=ico.w;h=ico.h;}

	if(center)
	{
		x = (x+m_iCellSize/2)-w/2;
		y = (y+m_iCellSize/2)-h/2;
	}
	else
	{
		bw = min(bw,m_iCellSize-w); 
		bh = min(bh,m_iCellSize-h); 

		if(!left)x = x+m_iCellSize-w; 
		if(!top)y = y+m_iCellSize-h;
	}

	dcmapHBITMAP Pre;
	dcmapHDC hdc = s_Shared.m_hdcBmp;
	dcmapHDC dc = s_Shared.m_hdcMem;

	if(ico.trns)
	{

		if(big) Pre=SelectBitmap(dc,ico.b_hmsk);
		else    Pre=SelectBitmap(dc,ico.hmsk); 
		BitBlt(hdc,x,y,w,h,dc,0,0,SRCAND);
		if(big) SelectBitmap(dc,ico.b_hbmp);
		else    SelectBitmap(dc,ico.hbmp); 
		BitBlt(hdc,x,y,w,h,dc,0,0,SRCPAINT);
		SelectBitmap(dc,Pre);
	}
	else
	{
		if(big) Pre=SelectBitmap(dc,ico.b_hbmp);
		else  Pre=SelectBitmap(dc,ico.hbmp); 
		BitBlt(hdc,x,y,w,h,dc,0,0,SRCCOPY);
		SelectBitmap(dc,Pre);
	}
}

void CDCMapNavView::RenderIcons(DCMapPlanetDisplayInfo* pinf,int x,int y)
{
	dcmapHDC hdc = s_Shared.m_hdcBmp;
	dcmapHDC dc = s_Shared.m_hdcMem;

	int bw = m_iCellSize;
	int bh = m_iCellSize;
	bool bounded = false;

	//int sw=0,th=0;
	if(pinf->Icon1 >= 0&& pinf->Icon1 < s_Shared.vecIcons.size())
	{
		IconData &ico = s_Shared.vecIcons[pinf->Icon1];
		DrawIcon(ico,x,y,bounded,true,true,bw,bh);
		bounded=true;

	}
	if(pinf->Icon2 >= 0&& pinf->Icon2 < s_Shared.vecIcons.size())
	{
		IconData &ico = s_Shared.vecIcons[pinf->Icon2];
		DrawIcon(ico,x,y,true,true,false,bw,bh);
		//bounded=true;
	}
	if(pinf->Icon3 >= 0 && pinf->Icon3 < s_Shared.vecIcons.size())
	{
		IconData &ico = s_Shared.vecIcons[pinf->Icon3];
		DrawIcon(ico,x,y,bounded,false,false,bw,bh);
		bounded=true;
	}
}

void CDCMapNavView::RenderGeo(DCMapPlanetParamInfo const* pinf,int x,int y)
{
	DCMapPlanetValueInfo value;
	int r = m_pGeoValue->CalculateGeoIndirect(&pinf->geo,&value);

	for(int i=0;i<m_vecGeoFilters.size();i++)
	{
		if(!m_vecGeoFilters[i]->ProcessGeoData(pinf,&value))return;
	}

	dcmapHDC m_hdcBmp = s_Shared.m_hdcBmp;

	float fImgRad = float(m_iCellSize-1)/2;
	if(pinf->s >= 90)fImgRad *= 1.0;
	else if(pinf->s >= 80)fImgRad *= 0.9;
	else if(pinf->s >= 70)fImgRad *= 0.8;
	else if(pinf->s >= 60)fImgRad *= 0.7;
	else if(pinf->s >= 50)fImgRad *= 0.6;
	else if(pinf->s == 0)fImgRad *= 0.5;
	else fImgRad *= 0.4;

	int iImgRad = fImgRad;
	if(iImgRad<2)iImgRad=2;

	dcmapHPEN  hpnPre;
	dcmapHBRUSH hbrPre;

	if(r > 0)
	{
		hpnPre = SelectPen(m_hdcBmp,m_hpnGeoGood);
		hbrPre = SelectBrush(m_hdcBmp,m_hbrGeoGood);
	}
	else
	{
		hpnPre = SelectPen(m_hdcBmp,m_hpnGeoBad);
		hbrPre = SelectBrush(m_hdcBmp,m_hbrGeoBad);
	}

	Ellipse(m_hdcBmp,x+m_iCellSize/2-iImgRad+1,y+m_iCellSize/2-iImgRad+1,
		x+m_iCellSize/2+iImgRad,y+m_iCellSize/2+iImgRad);

	SelectPen(m_hdcBmp,hpnPre);
	SelectBrush(m_hdcBmp,hbrPre);
}

void CDCMapNavView::RenderPlanet(DCMapPlanetDisplayInfo* pinf,int x,int y)
{
	dcmapHDC m_hdcBmp = s_Shared.m_hdcBmp;

	if(m_iCellSize == 1)
	{
		SetPixel(m_hdcBmp,x,y,pinf->clPlanetBody);
	}
	else if(m_iCellSize == 2)
	{
		SetPixel(m_hdcBmp,x,y,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x+1,y,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x,y+1,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x+1,y+1,pinf->clPlanetBody);
	}
	else if(m_iCellSize == 3)
	{
		SetPixel(m_hdcBmp,x+1,y+1,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x+1,y,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x,y+1,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x+1,y+2,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x+2,y+1,pinf->clPlanetBody);

		SetPixel(m_hdcBmp,x,y,pinf->clBackground);
		SetPixel(m_hdcBmp,x+2,y+2,pinf->clBackground);
		SetPixel(m_hdcBmp,x,y+2,pinf->clBackground);
		SetPixel(m_hdcBmp,x+2,y,pinf->clBackground);
	}
	else // Big planet
	{
		float fImgRad = float(m_iCellSize-1)/2;
		if(pinf->planet.s >= 90)fImgRad *= 1;
		else if(pinf->planet.s >= 80)fImgRad *= 0.9;
		else if(pinf->planet.s >= 70)fImgRad *= 0.8;
		else if(pinf->planet.s >= 60)fImgRad *= 0.7;
		else if(pinf->planet.s >= 50)fImgRad *= 0.6;
		else if(pinf->planet.s == 0)fImgRad *= 0.5;
		else fImgRad *= 0.4;

		int iImgRad = fImgRad;

		//if(!(iImgRad%2))iImgRad++;

		if(iImgRad<2)iImgRad=2;

		dcmapHBRUSH hbrPre;
		dcmapHPEN  hpnPre;

		if(pinf->iBackground)
		{
			dcmapHBRUSH hbrBackground;
			hbrBackground = CreateSolidBrush(pinf->clBackground);

			int csz = 1;

			dcmapRECT rc = {x+1,y+1,x+m_iCellSize-1+csz,y+m_iCellSize-1+csz};
			FillRect(m_hdcBmp,&rc,hbrBackground);
			DeleteBrush(hbrBackground);
		}

		if(pinf->iMarker)
		{
			dcmapHBRUSH  hbrMarker = CreateSolidBrush(pinf->clMarker);
			dcmapHPEN	hpnMarker = CreatePen(PS_SOLID,0,pinf->clMarker);
			hbrPre = SelectBrush(m_hdcBmp,hbrMarker);
			hpnPre = SelectPen(m_hdcBmp,hpnMarker);

			int ofs = m_iCellSize/3+2;
			const int cz = m_iCellSize;
			if(ofs > cz)ofs = cz;

			dcmapPOINT pts[] = {{x+cz-ofs,y+1},{x+cz-1,y+1},{x+cz-1,y+ofs}};

			Polygon(m_hdcBmp,pts,3);

			DeleteBrush(SelectBrush(m_hdcBmp,hbrPre));
			DeletePen(SelectPen(m_hdcBmp,hpnPre));
		}

		if(pinf->iCellBorder)
		{
			dcmapHPEN   hpnBorder;

			int csz = pinf->iCellBorder;
			if(csz)hpnBorder = CreatePen(PS_SOLID,csz,pinf->clCellBorder);
			else 
			{
				hpnBorder = CreatePen(PS_SOLID,0,pinf->clBackground);
				csz=1;
			}
			hbrPre = SelectBrush(m_hdcBmp,GetStockBrush(NULL_BRUSH));
			hpnPre = SelectPen(m_hdcBmp,hpnBorder);

			Rectangle(m_hdcBmp,x+1,y+1,x+m_iCellSize-1+csz,y+m_iCellSize-1+csz);

			SelectBrush(m_hdcBmp,hbrPre);
			DeletePen(SelectPen(m_hdcBmp,hpnPre));
		}

		if(pinf->iPlanetBody || pinf->iPlanetBorder)
		{
			dcmapHPEN hpnBody;
			dcmapHBRUSH hbrBody;

			if(pinf->iPlanetBody) hbrBody = CreateSolidBrush(pinf->clPlanetBody);
			else hbrBody = GetStockBrush(NULL_BRUSH);

			int bsz = pinf->iPlanetBorder;
			if(bsz && iImgRad <= 8)bsz=1;

			if(bsz)hpnBody = CreatePen(PS_SOLID,bsz,pinf->clPlanetBorder);
			else hpnBody = CreatePen(PS_SOLID,0,pinf->clPlanetBody);

			hbrPre = SelectBrush(m_hdcBmp,hbrBody);
			hpnPre = SelectPen(m_hdcBmp,hpnBody);

			Ellipse(m_hdcBmp,x+m_iCellSize/2-iImgRad+1,y+m_iCellSize/2-iImgRad+1,
				x+m_iCellSize/2+iImgRad,y+m_iCellSize/2+iImgRad);

			DeleteBrush(SelectBrush(m_hdcBmp,hbrPre));
			DeletePen(SelectPen(m_hdcBmp,hpnPre));
		}
	}
}

void CDCMapNavView::RenderPlanetHQ(DCMapPlanetDisplayInfo* pinf,int x,int y)
{
	Graphics& gr = *s_Shared.m_pGraphics;

	dcmapHDC m_hdcBmp = s_Shared.m_hdcBmp;

	if(m_iCellSize == 1)
	{
		SetPixel(m_hdcBmp,x,y,pinf->clPlanetBody);
	}
	else if(m_iCellSize == 2)
	{
		SetPixel(m_hdcBmp,x,y,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x+1,y,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x,y+1,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x+1,y+1,pinf->clPlanetBody);
	}
	else if(m_iCellSize == 3)
	{
		SetPixel(m_hdcBmp,x+1,y+1,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x+1,y,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x,y+1,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x+1,y+2,pinf->clPlanetBody);
		SetPixel(m_hdcBmp,x+2,y+1,pinf->clPlanetBody);

		SetPixel(m_hdcBmp,x,y,pinf->clBackground);
		SetPixel(m_hdcBmp,x+2,y+2,pinf->clBackground);
		SetPixel(m_hdcBmp,x,y+2,pinf->clBackground);
		SetPixel(m_hdcBmp,x+2,y,pinf->clBackground);
	}
	else // Big planet
	{
		float fImgRad = float(m_iCellSize-1)/2;
		if(pinf->planet.s >= 90)fImgRad *= 1;
		else if(pinf->planet.s >= 80)fImgRad *= 0.9;
		else if(pinf->planet.s >= 70)fImgRad *= 0.8;
		else if(pinf->planet.s >= 60)fImgRad *= 0.7;
		else if(pinf->planet.s >= 50)fImgRad *= 0.6;
		else if(pinf->planet.s == 0)fImgRad *= 0.5;
		else fImgRad *= 0.4;

		int iImgRad = fImgRad;

		//iImgRad--;

		//if(!(iImgRad%2))iImgRad++;

		if(iImgRad<2)iImgRad=2;

		int csz = pinf->iCellBorder;

		int bsz = pinf->iPlanetBorder;
		if(bsz && iImgRad <= 8)bsz=1;

		Color cl;

		cl.SetFromCOLORREF(pinf->clPlanetBody);
		SolidBrush brBody(cl);

		cl.SetFromCOLORREF(pinf->clBackground);
		SolidBrush brBack(cl);

		gr.FillRectangle(&brBack,x+1,y+1,m_iCellSize-1+csz,m_iCellSize-1+csz);

		if(bsz)
		{
			gr.FillEllipse(&brBody,x+m_iCellSize/2-iImgRad,y+m_iCellSize/2-iImgRad,iImgRad*2,iImgRad*2);

			cl.SetFromCOLORREF(pinf->clPlanetBorder);
			Pen   penBody(cl,bsz);

			gr.SetSmoothingMode(SmoothingModeAntiAlias);
			gr.DrawEllipse(&penBody,x+m_iCellSize/2-iImgRad,y+m_iCellSize/2-iImgRad,iImgRad*2,iImgRad*2);
			gr.SetSmoothingMode(SmoothingModeNone);
		}
		else
		{
			gr.SetSmoothingMode(SmoothingModeAntiAlias);
			gr.FillEllipse(&brBody,x+m_iCellSize/2-iImgRad,y+m_iCellSize/2-iImgRad,iImgRad*2,iImgRad*2);
			gr.SetSmoothingMode(SmoothingModeNone);
		}

		if(csz)
		{
			cl.SetFromCOLORREF(pinf->clCellBorder);
			Pen   penBorder(cl,csz);

			gr.DrawRectangle(&penBorder,x+1,y+1,m_iCellSize+csz-3,m_iCellSize+csz-3);
		}

	}
}

enum UICommands
{
	UICMD_DELETE = 1,
	UICMD_EDITPLANET,
	UICMD_CENTER,
	UICMD_COPY_COORDS,
	UICMD_COPY_INFO,
	UICMD_GOTO,
	UICMD_PASTE,
};

bool DCMAPAPI CDCMapNavView::Init(IDCMapCore* core,IDCMapWorkspace* pWorkspace)
{
	m_pWorkspace = pWorkspace;
	m_pCore = core;

    LoadPrefs();

	// create feeder for test
	m_pDataProcessor = pWorkspace->GetServiceTyped<IDCMapPlanetDataProcessor>("PlanetDataProcessor");

	m_pPlanetMan = pWorkspace->GetServiceTyped<IDCMapPlanetManager>("PlanetManager");
	m_pLayers = core->GetDataSourceTyped<IDCMapLayersDataStorage>("layers");
	m_pPlanets = core->GetDataSourceTyped<IDCMapPlanetDataStorage>("planets");

	if(!m_pDataProcessor) return false;
	if(!m_pPlanetMan) return false;
	if(!m_pLayers) return false;
	if(!m_pPlanets) return false;

	//return true;
	//////////////////////////////////////////////////////////////////////////
	// Geo 
	{
		if(!m_pGeo.init(core,"geo"))return false;

		m_pGeoValue = pWorkspace->GetServiceTyped<IDCMapGeoValueCalculator>("GeoValueCalculator");
		if(!m_pGeoValue) return false;

		m_pPlanetContextMan = pWorkspace->GetServiceTyped<IDCMapPlanetContextManager>("PlanetContextManager");
		if(!m_pPlanetContextMan) return false;

		vector<string> modules;
		g_pDCMapSys->GetClassPluginNamesT("PGeoFilter",modules);

		for(int i=0;i<modules.size();i++)
		{
			IDCMapGeoDataFilter* pfilter = 
				m_pWorkspace->CreateModuleTyped<IDCMapGeoDataFilter>("PGeoFilter",modules[i].c_str());
			if(pfilter)
			{
				m_vecGeoFilters.push_back(pfilter);
			}	
		}
	}
	IDCMapUIMenuItem* Item;

	g_pLocalStrings->SelectSection("MapNavView");
	dcmapDCSTR szMenuName = g_pLocalStrings->GetSectionStringW("MenuName");
	dcmapDCSTR szCenterMenuItem = g_pLocalStrings->GetSectionStringW("CenterMenuItem");
	dcmapDCSTR szEditPlanetMenuItem = g_pLocalStrings->GetSectionStringW("EditPlanetMenuItem");
	dcmapDCSTR szDeletePlanetMenuItem = g_pLocalStrings->GetSectionStringW("DeletePlanetMenuItem");
	dcmapDCSTR szAddPlanetMenuItem = g_pLocalStrings->GetSectionStringW("AddPlanetMenuItem");
	dcmapDCSTR szClearGeoMenuItem = g_pLocalStrings->GetSectionStringW("ClearGeoMenuItem");

	dcmapWCSTR szCopyCoordMenuItem = g_pLocalStrings->GetSectionStringW("CopyCoordMenuItem");
	dcmapWCSTR szCopyCoordMenuItemSC = g_pLocalStrings->GetSectionStringW("CopyCoordMenuItem.ShortCut");

	dcmapWCSTR szGotoPlanetMenuItem = g_pLocalStrings->GetSectionStringW("GotoPlanetMenuItem");
	dcmapWCSTR szGotoPlanetMenuItemSC = g_pLocalStrings->GetSectionStringW("GotoPlanetMenuItem.ShortCut");

	dcmapWCSTR szCopyInfoMenuItem = g_pLocalStrings->GetSectionStringW("CopyInfoMenuItem");
	dcmapWCSTR szCopyInfoMenuItemSC = g_pLocalStrings->GetSectionStringW("CopyInfoMenuItem.ShortCut");

	dcmapWCSTR szPasteInfoMenuItem = g_pLocalStrings->GetSectionStringW("PasteInfoMenuItem");
	dcmapWCSTR szPasteInfoMenuItemSC = g_pLocalStrings->GetSectionStringW("PasteInfoMenuItem.ShortCut");

	//////////////////////////////////////////////////////////////////////////
	// Main menues

	pWorkspace->GetUI()->AddMenu("DCMapNavView",szMenuName,this,-1);

	Item = pWorkspace->GetUI()->AddMenuItem("DCMapNavView/",szCopyCoordMenuItem,this,UICMD_COPY_COORDS);
	Item->SetShortCut(szCopyCoordMenuItemSC);

	Item = pWorkspace->GetUI()->AddMenuItem("DCMapNavView/",szCopyInfoMenuItem,this,UICMD_COPY_INFO);
	Item->SetShortCut(szCopyInfoMenuItemSC);

    if(!m_bReadOnly)
    {
        Item = pWorkspace->GetUI()->AddMenuItem("DCMapNavView/",szPasteInfoMenuItem,this,UICMD_PASTE);
        Item->SetShortCut(szPasteInfoMenuItemSC);
    }

	Item = pWorkspace->GetUI()->AddMenuItem("DCMapNavView/",L"-",this,-1);

	Item = pWorkspace->GetUI()->AddMenuItem("DCMapNavView/",szGotoPlanetMenuItem,this,UICMD_GOTO);
	Item->SetShortCut(szGotoPlanetMenuItemSC);

	/////////////////////////////////////////////////////////////////////////
	// Popup menues

	m_pPlanetPopup = core->GetCoreUI()->CreatePopupMenu(this,0);
	if(!m_pPlanetPopup) return false;
	m_pCellPopup = core->GetCoreUI()->CreatePopupMenu(this,0);
	if(!m_pCellPopup) return false;


	Item = m_pPlanetPopup->AddItem(0,szCenterMenuItem,this,UICMD_CENTER);
	Item->SetShortCut(L"C");

	m_pPlanetPopup->AddItem(0,L"-",this,0);

	Item = m_pPlanetPopup->AddItem(0,szCopyCoordMenuItem,this,UICMD_COPY_COORDS);
	Item->SetShortCut(L"Ctrl+C");

    Item = m_pCellPopup->AddItem(0,szCenterMenuItem,this,UICMD_CENTER);
    Item->SetShortCut(L"C");

    m_pCellPopup->AddItem(0,L"-",this,0);

    Item = m_pCellPopup->AddItem(0,szCopyCoordMenuItem,this,UICMD_COPY_COORDS);
    Item->SetShortCut(L"Ctrl+C");

    if(!m_bReadOnly)
    {
        Item = m_pPlanetPopup->AddItem(0,szPasteInfoMenuItem,this,UICMD_PASTE);
        Item->SetShortCut(szPasteInfoMenuItemSC);

	    m_pPlanetPopup->AddItem(0,L"-",this,0);

	    Item = m_pPlanetPopup->AddItem(0,szEditPlanetMenuItem,this,UICMD_EDITPLANET);
	    Item->SetShortCut(L"Enter");

	    Item = m_pPlanetPopup->AddItem(0,szDeletePlanetMenuItem,this,UICMD_DELETE);
	    Item->SetShortCut(L"Del");

        Item = m_pCellPopup->AddItem(0,szPasteInfoMenuItem,this,UICMD_PASTE);
        Item->SetShortCut(L"Ctrl+V");

        m_pCellPopup->AddItem(0,L"-",this,0);

        Item = m_pCellPopup->AddItem(0,szAddPlanetMenuItem,this,UICMD_EDITPLANET);
        Item->SetShortCut(L"Enter");

        Item = m_pCellPopup->AddItem(0,szClearGeoMenuItem,this,UICMD_DELETE);
        Item->SetShortCut(L"Del");
    }
	//////////////////////////////////////////////////////////////////////////

	return true;
}

void CDCMapNavView::DeletePlanet()
{
	if(!m_pPlanetMan->GetCurrentPlanet(0))
	{
		if(m_pGeo->SelectCoord(m_ptCurrentCell.x,m_ptCurrentCell.y))
			m_pGeo->Delete();
		return;
	}
	g_pLocalStrings->SelectSection("MapNavView");
	int r = m_pCore->GetCoreUI()->MsgBox(g_pLocalStrings->GetSectionStringW("DeletePlanetMsgText"),
											 g_pLocalStrings->GetSectionStringW("DeletePlanetMSGCaption"),MB_YESNO);
	if(r == IDYES)
		m_pPlanetMan->DeleteCurrentPlanet();
}

void CDCMapNavView::PasteInfo()
{
	dcmapDCSTR dta = m_pCore->GetCoreUI()->ClipboardGetW();
	if(!dta)return;	
	
	m_pCore->LockDataUpdate();
	DCMapConfigWrapper cfg(m_pCore->GetConfig());
	cfg.section("MapNavView");
	int num=0;
	cfg["RegExConfNum"] >> num;
	int i;
	for(i=0;i<num;i++)
	{
		CDBBlockParser prs;
        prs.Rights(PRSR_READ|PRSR_DB_WRITE);
		prs.SetVariable(L"x",CVarValue(m_ptCurrentCell.x));
		prs.SetVariable(L"y",CVarValue(m_ptCurrentCell.y));
		prs.SetTable("planets");
		prs.SetDefaultValue(dta);

		wstring regexp;
		cfg["RegExConfRegExp"+gsys::to_str(i)] >> regexp;
		wiparsestream is(regexp);
		CVarValue res;
		try
		{
			prs.Parse(true,is,res);
		}
		catch (die_exception& e)
		{
			continue;
		}
		catch(std::exception& e)
		{
			m_pCore->GetCoreUI()->ErrorMsg(ToWide(e.what()).c_str());
			break;
		}
		prs.Post();
		break;
	}
	if(i == num)
	{
		dcmapPOINT pt;
		string value;
		if(StrToCoords(WideToString(dta).c_str(),pt) && gsys::at_range(pt.x,1,g_DCMapNumPlanets) && gsys::at_range(pt.y,1,g_DCMapNumPlanets))
		{
			m_pPlanetMan->SetCurrentPlanet(0,pt.x,pt.y);
		}
		else if(i == num) m_pCore->GetCoreUI()->ErrorMsg(LSTRW(DataNotRecognized));
	}
	m_pCore->UnlockDataUpdate();
}

int DCMAPAPI CDCMapNavView::OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
{
	switch(Msg)
	{
	case DCMM_UI_COMMAND:
		{
			int cx,cy;
			m_pPlanetMan->GetCurrentPlanetCoords(&cx,&cy);
			switch(wParam)
			{	
			case UICMD_EDITPLANET:
                if(!m_bReadOnly)
                {
				    m_pPlanetMan->SetCurrentPlanet(0,m_ptCurrentCell.x,m_ptCurrentCell.y);
				    m_pPlanetMan->EditCurrentPlanet();
				    m_pPlanetMan->SetCurrentPlanet(0,cx,cy);
                }
				break;
			case UICMD_DELETE:
                if(!m_bReadOnly)
                {
				    m_pPlanetMan->SetCurrentPlanet(0,m_ptCurrentCell.x,m_ptCurrentCell.y);
				    DeletePlanet();
				    m_pPlanetMan->SetCurrentPlanet(0,cx,cy);
                }
				break;
			case UICMD_PASTE:
				if(!m_bReadOnly)PasteInfo();
				break;
			case UICMD_COPY_COORDS:
				m_pCore->GetCoreUI()->ClipboardPut((gsys::to_str(m_ptCurrentCell.x)+":"+gsys::to_str(m_ptCurrentCell.y)).c_str());
				break;
			case UICMD_CENTER:
				{
					ShowPlace(m_ptCurrentCell.x,m_ptCurrentCell.y,true);
				}
				break;
			case UICMD_GOTO:
				{
                    dcmapDCSTR vcoord = m_pCore->GetCoreUI()->ClipboardGetW();
                    dcmapPOINT pt;
                    string value;

                    if(vcoord && vcoord[0])
                    {
                        if(StrToCoords(WideToString(vcoord).c_str(),pt))
					    {
						    if(gsys::at_range(pt.x,1,g_DCMapNumPlanets) && gsys::at_range(pt.y,1,g_DCMapNumPlanets))
							    value = gsys::to_str(pt.x)+":"+gsys::to_str(pt.y);
					    }
                    }	

				    g_pLocalStrings->SelectSection("MapNavView");
				    dcmapDCSTR coord = m_pCore->GetCoreUI()->InputBox(g_pLocalStrings->GetSectionStringW("GotoPlanetInpText"),
					    g_pLocalStrings->GetSectionStringW("GotoPlanetInpCaption"),ToWide(value).c_str());
				    if(coord && coord[0])
				    {
					    if(StrToCoords(WideToString(coord).c_str(),pt))
					    {
						    m_pPlanetMan->SetCurrentPlanet(0,pt.x,pt.y);
					    }
					    else  m_pCore->GetCoreUI()->ErrorMsg(g_pLocalStrings->GetStringW("BadCoords"));
				    }
				}
				break;
			};
			m_pWorkspace->GetUI()->ActivateView();
		}
		break;
	case DCMM_CONFIG_CHANGED:
		LoadPrefs();
		SetupColors();
		SetupParams();
		Render();
		break;
	case DCMM_POST_INIT:
		ActualizeLayers();
		break;
	case DCMM_DATA_CHANGED:
		ActualizeData();
		break;
	case DCMM_MAPDATA_CHANGED:
		Render();
		break;
	case DCMM_WORKSPACE_ACTIVATE:
		m_bRenderReady=true;
		Render();
		break;
	case DCMM_WORKSPACE_DEACTIVATE:
		m_bRenderReady=false;
		break;
	case DCMM_WORKSPACE_SHOW:
		m_bRenderReady=true;
		Render();
		{
			ShowWindow(m_hWnd,SW_SHOW);
			HDC dc = GetDC(m_hWnd);
			OnPaint(dc);
			ReleaseDC(m_hWnd,dc);
		}
		break;
	case DCMM_WORKSPACE_HIDE:
		m_bRenderReady=false;
		break;
	case DCMM_PLANET_MAPDATA_CHANGED:
		{
			dcmapRECT rc = {wParam,lParam,wParam+1,lParam+1};
			UpdateMapRect(&rc);
		}
		break;
	case DCMM_PLANET_CHANGED:
		UpdateSelection();
		break;
    case DCMM_SHOW_CELL:
        ShowPlace(LOWORD(wParam),HIWORD(wParam),lParam);
        break;
	}
	return 0;
}

void DCMAPAPI CDCMapNavView::UpdateLayers()
{
	if(m_bLayerVisiblityChanged)
	{
		m_pWorkspace->BroadcastMessage(DCMM_LAYERS_VISIBLITY_CHANGED,0,0,this);
		m_bLayerVisiblityChanged=false;
		Render();
	}
}

void CDCMapNavView::SaveLayersCfg()
{
	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
	cfg.section("MapNavView");

	for(int i=0;i<m_Layers.size();i++)
	{
		string Name = "Layer_"+m_Layers[i].strLyaerId; 
		cfg[Name] << m_Layers[i].bVisible;
	}	
}


void CDCMapNavView::ActualizeLayers()
{
	SaveLayersCfg();

	if(m_pActiveLayer)	m_pActiveLayer->OnMessage(DCMM_MAP_LAYER_DEACTIVATE,0,0,this);
	m_pActiveLayer=0;

	for(int i=LastLayer+1;i<m_Layers.size();i++)
	{
		if(m_Layers[i].m_pLayerIface)
		{
			m_pWorkspace->DestroyModule(m_Layers[i].m_pLayerIface);
		}
	}
	m_Layers.clear();

    LSTRSECTION(MapLayers);
	{
		LayerDesc desc;
		// first 2 layers - is grid & planets
		desc.bInternal=true;
		desc.strCaption = SLSTRW(BigGrid);
		desc.strName="big_grid";
		desc.strInternalName=L"big_grid";
		desc.bVisible=true;
		desc.strLyaerId = desc.strName;
		m_Layers.push_back(desc);
	}

	{
		LayerDesc desc;
		// first 2 layers - is grid & planets
		desc.bInternal=true;
		desc.strCaption = SLSTRW(Grid);
		desc.strName="grid";
		desc.strInternalName=L"grid";
		desc.bVisible=true;
		desc.strLyaerId = desc.strName;
		m_Layers.push_back(desc);
	}

	{
		LayerDesc desc;
		// first 2 layers - is grid & planets
		desc.bInternal=true;
		desc.bPlanetary=true;
		desc.strCaption = SLSTRW(Geology);
		desc.strName="geo";
		desc.strInternalName=L"geo";
		desc.bVisible=false;
		desc.strLyaerId = desc.strName;
		m_Layers.push_back(desc);
	}

	{
		LayerDesc desc;
		// first 2 layers - is grid & planets
		desc.bInternal=true;
		desc.bPlanetary=true;
		desc.strCaption = SLSTRW(Planets);
		desc.strName="planets";
		desc.strInternalName=L"planets";
		desc.bVisible=true;
		desc.strLyaerId = desc.strName;
		m_Layers.push_back(desc);
	}

	{
		LayerDesc desc;
		// first 2 layers - is grid & planets
		desc.bInternal=true;
		desc.bPlanetary=true;
		desc.strCaption = SLSTRW(Icons);
		desc.strName="icons";
		desc.strInternalName=L"icons";
		desc.bVisible=true;
		desc.strLyaerId = desc.strName;
		m_Layers.push_back(desc);
	}


	// get next layer plug
	// then fill from layers plugins
	vector<string> strs;
	g_pDCMapSys->GetClassPluginNamesT("PMapLayer",strs);
	_foreach(vector<string>,strs,i)
	{
		LayerDesc desc;
		// first 2 layers - is grid & planets
		desc.bVisible=false;
		desc.bUser=true;
		desc.m_pLayerIface = m_pWorkspace->CreateModuleTyped<IDCMapLayer>("PMapLayer",i->c_str());
		if(!desc.m_pLayerIface) continue;
		if(!desc.m_pLayerIface->InitLayer(this))
		{
			m_pWorkspace->DestroyModule(desc.m_pLayerIface);
		}

		desc.strName = desc.m_pLayerIface->GetLayerName();
		desc.strInternalName = ToWide(desc.m_pLayerIface->GetLayerName());

		dcmapWCSTR capstr = desc.m_pLayerIface->GetLayerCaption();
		if(capstr && capstr[0])desc.strCaption = capstr;
		else capstr = g_pLocalStrings->GetSectionStringW(desc.strName.c_str(),"MapLayers");

		if(capstr && capstr[0]) desc.strCaption = capstr;
		else desc.strCaption = ToWStr(desc.strName);

		desc.strLyaerId = *i+"_"+desc.strName;

		m_Layers.push_back(desc); 
	}

	// if user - get from layers db
	IDCMapLayersDataStorage* pLayers = m_pCore->GetDataSourceTyped<IDCMapLayersDataStorage>("layers");
	if(m_pLayers)
	{
		if(m_pLayers->SelectAll())
		do 
		{
			LayerDesc desc;
			// first 2 layers - is grid & planets
			desc.strInternalName = m_pLayers->GetWideName();
			desc.strCaption = desc.strInternalName;
			desc.bVisible=false;
			desc.bUser=true;
			desc.strName = m_pLayers->GetString(DCMDB_LAYER_MODULE);
			desc.m_pLayerIface = m_pWorkspace->CreateModuleTyped<IDCMapLayer>("PUserLayer",desc.strName.c_str());
			if(!desc.m_pLayerIface) continue;
			if(!desc.m_pLayerIface->InitLayer(this))
			{
					m_pWorkspace->DestroyModule(desc.m_pLayerIface);
			}
			desc.LayerRef = *m_pLayers->GetID();
			desc.bHideFromList = m_pLayers->GetBoolean(DCMDB_LAYER_HIDDEN);

			if(!desc.m_pLayerIface) continue;
			desc.m_pLayerIface->InitUserLayer(desc.LayerRef);

			dcmapWCSTR capstr = g_pLocalStrings->GetSectionStringW(ToStr(desc.strCaption).c_str(),"MapLayers");
			if(capstr && capstr[0])desc.strCaption = capstr;

			desc.strLyaerId = desc.strName+"_"+ToStr(desc.LayerRef.id)+"_"+ToStr(desc.LayerRef.age);

			m_Layers.push_back(desc);

		} while(m_pLayers->Next());
	}

	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
	cfg.section("MapNavView");

	for(int i=0;i<m_Layers.size();i++)
	{
		string Name = "Layer_"+m_Layers[i].strLyaerId; 
		bool v;
		if(cfg[Name] >> v) m_Layers[i].bVisible = v;
	}

	m_LayersState = m_pLayers->GetStateIndex();

	m_pWorkspace->BroadcastMessage(DCMM_LAYERS_CHANGED,0,0,this);
}

bool DCMAPAPI CDCMapNavView::EditLayer(dcmapUINT n)
{
	if(n >= m_Layers.size())return false;

	if(n>LastLayer)
	{
		if(m_Layers[n].m_pLayerIface)
			return m_Layers[n].m_pLayerIface->EditLayer();
	}
	else if(n == IconsLayer)
	{
		m_pCore->GetCoreUI()->OpenListTableEdit("icons");
		return true;
	}
	return false;
}

bool DCMAPAPI CDCMapNavView::SetActiveLayer(dcmapUINT n)
{
	if(n >= m_Layers.size())return false;

	if(m_iActivelayer == n) return true;

	if(m_pActiveLayer)	m_pActiveLayer->OnMessage(DCMM_MAP_LAYER_DEACTIVATE,0,0,this);
 	if(n<=LastLayer)
	{
		m_iActivelayer = PlanetLayer;
		m_pActiveLayer=0;
	}
	else
	{
		if(n>=m_Layers.size())return false;
		m_iActivelayer = n;
		m_pActiveLayer = m_Layers[m_iActivelayer].m_pLayerIface;
		m_pActiveLayer->OnMessage(DCMM_MAP_LAYER_ACTIVATE,0,0,this);
	}
	return true;
}

void DCMAPAPI CDCMapNavView::UpdateMapRect(const dcmapRECT* rcIn)
{
    if(!rcIn)
    {
        Render();
        return;
    }

    dcmapRECT rc = *rcIn;
	if(rc.right < m_iLeft)
	{
		rc.left += g_DCMapNumPlanets; 
		rc.right += g_DCMapNumPlanets; 
	}

	if(rc.bottom < m_iTop)
	{
		rc.top += g_DCMapNumPlanets; 
		rc.bottom += g_DCMapNumPlanets; 
	}

	dcmapRECT rcArea;
	rcArea.left = (rc.left - m_iLeft-1)*m_iCellSize+m_rcMap.left;
	rcArea.top = (rc.top - m_iTop-1)*m_iCellSize+m_rcMap.top;
	rcArea.right = (rc.right - m_iLeft)*m_iCellSize+m_rcMap.left;
	rcArea.bottom = (rc.bottom - m_iTop)*m_iCellSize+m_rcMap.top;

	rcArea.left = max(rcArea.left,m_rcMap.left);
	rcArea.right = min(rcArea.right,m_rcMap.right);
	rcArea.top = max(rcArea.top,m_rcMap.top);
	rcArea.bottom = min(rcArea.bottom,m_rcMap.bottom);

	if(rcArea.left >= rcArea.right)return;
	if(rcArea.top >= rcArea.bottom)return;

	RenderMap(&rcArea);
}

void CDCMapNavView::ActualizeData()
{
	bool update=false;

	update |= s_Shared.Actualize();
	//s_Shared.LoadIcons();

	update |= m_pDataProcessor->ActualizeData();

	if(m_LayersState != m_pLayers->GetStateIndex())
	{
		ActualizeLayers();
		update=true;
	}

	for(int i=LastLayer+1;i<m_Layers.size();i++)
	{
		if(m_Layers[i].bVisible && m_Layers[i].m_pLayerIface)
		{
			update |= m_Layers[i].m_pLayerIface->ActualizeData();
		}
	}
	if(update) Render();
}


bool CDCMapNavView::CellVisible(int x,int y)
{
    x = WrapCoord(x);
    y = WrapCoord(y);

    if(m_iLeft+m_iWidth > g_DCMapNumPlanets && x < m_iLeft) x+= g_DCMapNumPlanets;
    if(m_iTop+m_iHeight > g_DCMapNumPlanets && y < m_iTop) y+= g_DCMapNumPlanets;

    if((x > m_iLeft && x <= m_iLeft+m_iWidth &&
       y > m_iTop && y <= m_iTop+m_iHeight)) return true;

    return false;
}

bool CDCMapNavView::ShowPlace(int x,int y,bool center)
{
	x = WrapCoord(x);
	y = WrapCoord(y);

	if(m_iLeft+m_iWidth > g_DCMapNumPlanets && x < m_iLeft) x+= g_DCMapNumPlanets;
	if(m_iTop+m_iHeight > g_DCMapNumPlanets && y < m_iTop) y+= g_DCMapNumPlanets;

	if(!center && (x > m_iLeft && x <= m_iLeft+m_iWidth &&
		y > m_iTop && y <= m_iTop+m_iHeight)) return false;

    int nleft,ntop; 

	nleft = x-m_iWidth/2;
	ntop = y-m_iHeight/2;

	nleft = WrapCoord(nleft);
	ntop = WrapCoord(ntop);

    dcmapPOINT ptUpd;
    ptUpd.x = nleft-m_iLeft;
    ptUpd.y = ntop-m_iTop;

    if( abs(ptUpd.x) < m_iWidth-2 && abs(ptUpd.y) < m_iHeight-2 )
    {
        ScrollMap(ptUpd);
    }
    else
    {
        m_iLeft = nleft;
        m_iTop = ntop;
        LayersMessage(DCMM_MAP_PAN,m_iLeft,m_iTop);
        Render();
    }
	return true;
}


void CDCMapNavView::UpdateSelection()
{
	if(m_iSelX >= 0 || m_iSelY >= 0) // Clear current selection if any
	{
		dcmapRECT rc = {m_iSelX,m_iSelY,m_iSelX+1,m_iSelY+1};
		m_iSelX=-1;
		m_iSelY=-1;
		UpdateMapRect(&rc);
	}
	m_iSelType=0;
	m_iSelX=-1;
	m_iSelY=-1;

	dcmapDBREF ref;
	if(m_pPlanetMan->GetCurrentPlanet(&ref))
	{
		m_iSelType=1;
	}

	int cx,cy;
	if(m_pPlanetMan->GetCurrentPlanetCoords(&cx,&cy))
	{
		m_iSelX=cx;
		m_iSelY=cy;		
		if(CellVisible(cx,cy))
		{
			dcmapRECT rc = {cx,cy,cx+1,cy+1};
			UpdateMapRect(&rc);		
		}
		m_ptCurrentCell.x = cx;
		m_ptCurrentCell.y = cy;
	}
}

void CDCMapNavView::OnKeyDown(dcmapDWORD key,dcmapDWORD opt)
{
	if(!m_bActive)return;
	ResetHint();

	bool cntr =   GetAsyncKeyState(VK_CONTROL);
	bool shift =  GetAsyncKeyState(VK_SHIFT);

	bool selmode = !m_pActiveLayer;

	int upd = ceil(min(m_iWidth,m_iHeight)*(shift?0.08:0.02));

	int cx,cy;
	m_pPlanetMan->GetCurrentPlanetCoords(&cx,&cy);

	if(key == VK_SPACE)
	{
		if(!m_bSpaceDragMode)
		{
			m_bSpaceDragMode = true;
			SetCursor(s_Shared.hcHand);
		}
	}
	else if(key == VK_ADD || key == VK_OEM_PLUS) 
	{
		Zoom((1+shift),0,-1,-1);
	}
	else if(key == VK_SUBTRACT || key == VK_OEM_MINUS) 
	{
		Zoom(-(1+shift),0,-1,-1);
	}
	else if(key == VK_UP) 
	{
		if(cntr && selmode)
		{
			int x,y;m_pPlanetMan->GetCurrentPlanetCoords(&x,&y);y--;
			m_pPlanetMan->SetCurrentPlanet(0,x,y);
		}
		else
		{
			dcmapPOINT pt = {0,-upd};ScrollMap(pt);
		}
	}
	else if(key == VK_DOWN) 
	{
		if(cntr && selmode)
		{
			int x,y;m_pPlanetMan->GetCurrentPlanetCoords(&x,&y);y++;
			m_pPlanetMan->SetCurrentPlanet(0,x,y);
		}
		else
		{
			dcmapPOINT pt = {0,upd};ScrollMap(pt);
		}
	}
	else if(key == VK_LEFT) 
	{
		if(cntr && selmode)
		{
			int x,y;m_pPlanetMan->GetCurrentPlanetCoords(&x,&y);x--;
			m_pPlanetMan->SetCurrentPlanet(0,x,y);
		}
		else
		{
			dcmapPOINT pt = {-upd,0};ScrollMap(pt);
		}
	}
	else if(key == VK_RIGHT) 
	{
		if(cntr && selmode)
		{
			int x,y;m_pPlanetMan->GetCurrentPlanetCoords(&x,&y);x++;
			m_pPlanetMan->SetCurrentPlanet(0,x,y);
		}
		else
		{
			dcmapPOINT pt = {upd,0};ScrollMap(pt);
		}
	}
	else if(m_pActiveLayer)
	{
		//m_pActiveLayer->OnMessage(DCMM_MAP_KEYDOWN,key,opt,this);
		//return;
	}
	else if(key == VK_DELETE) 
	{
		if(!m_bReadOnly)DeletePlanet();
	}
	else if(key == VK_RETURN) 
	{
		if(!m_bReadOnly)m_pPlanetMan->EditCurrentPlanet();
	}
	else if((key == 'V' && cntr) || (key == VK_INSERT && shift))
	{
		OnMessage(DCMM_UI_COMMAND,UICMD_PASTE,0,0);
	}
	else if((key == 'C' || key == VK_INSERT) && cntr)
	{
		OnMessage(DCMM_UI_COMMAND,UICMD_COPY_COORDS,0,0);
	}
	else if(key == 'C')
	{
		OnMessage(DCMM_UI_COMMAND,UICMD_CENTER,0,0);
	}
	else if(key == 'G')
	{
		OnMessage(DCMM_UI_COMMAND,UICMD_GOTO,0,0);
	}	
}

void CDCMapNavView::OnKeyUp(dcmapDWORD key,dcmapDWORD opt)
{
	if(key == VK_SPACE && m_bSpaceDragMode)
	{
		m_bSpaceDragMode = false;
		if(!m_bDragMode)
		{
			SetCursor(s_Shared.hcArrow);
		}
	}
}

void CDCMapNavView::OnMButtonDown(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y)
{
	if(m_bDragMode || m_bSpaceDragMode)return;

	dcmapPOINT pt = {x,y};
	if(!PtInRect(&m_rcMap,pt))return;

	int cx = (m_iLeft+(x-m_rcMap.left)/m_iCellSize)%g_DCMapNumPlanets+1;
	int cy = (m_iTop+(y-m_rcMap.top)/m_iCellSize)%g_DCMapNumPlanets+1;

	ShowPlace(cx,cy,true);
}

void CDCMapNavView::OnMButtonUp(dcmapWORD fwKeys,dcmapSHORT x,dcmapSHORT y)
{
	
}

void CDCMapNavView::DoSetFocus()
{
	if(m_bActive)return;
	//m_pWorkspace->GetUI()->ActivateView();
	SetFocus(m_hWnd);
	m_bActive=true;
}

void CDCMapNavView::OnHintTimer()
{
	if(!m_TimerID)return;
	if(m_bHasMouse)
	{
		dcmapPOINT pt1 = {m_rcHintArea.left,m_rcHintArea.top};
		dcmapPOINT pt2 = {m_rcHintArea.right,m_rcHintArea.bottom};

		ClientToScreen(m_hWnd,&pt1);
		ClientToScreen(m_hWnd,&pt2);

		dcmapRECT area = {pt1.x,pt1.y,pt2.x,pt2.y};

		if(m_strHint.empty())
		{
			m_pWorkspace->GetFrameUI()->ShowHint(area,GetHint());
			//m_pCore->GetCoreUI()->ShowHint(area,GetHint());
		}
		else m_pWorkspace->GetFrameUI()->ShowHint(area,m_strHint.c_str());
		//else m_pCore->GetCoreUI()->ShowHint(area,m_strHint.c_str());
	}
	KillTimer(m_hWnd,m_TimerID);
	m_TimerID=0;
}

void CDCMapNavView::SetupHint(int x, int y)
{
	if(!m_bEnableToolTip)return;

	m_ptHintPlanet.x = x;
	m_ptHintPlanet.y = y;

	int sx = (x-m_iLeft-1);
	int sy = (y-m_iTop-1);

	if(sx<0)sx+=g_DCMapNumPlanets;
	if(sy<0)sy+=g_DCMapNumPlanets;


	int cx = m_rcMap.left+sx*m_iCellSize;
	int cy = m_rcMap.top+sy*m_iCellSize;
	dcmapRECT rc = {cx,cy,cx+m_iCellSize,cy+m_iCellSize};

	SetupHint(0,&rc);
}

dcmapWCSTR CDCMapNavView::GetHint()
{
	return m_pPlanetContextMan->GetCellHint(m_ptHintPlanet.x,m_ptHintPlanet.y);
}

void DCMAPAPI CDCMapNavView::SetupHint(wchar_t const* str,dcmapRECT const* area)
{
	ResetHint();
	m_strHint = str?str:L"";

	if(area)m_rcHintArea = *area;
	else m_rcMap.left = m_rcMap.right = m_rcMap.top = m_rcMap.bottom = 0;

	m_TimerID = SetTimer(m_hWnd,1,m_iToolTipDelay,0);


	TRACKMOUSEEVENT evt;
	evt.cbSize = sizeof(TRACKMOUSEEVENT);
	evt.dwFlags = TME_LEAVE;
	evt.hwndTrack = m_hWnd;

	TrackMouseEvent(&evt);

}

void DCMAPAPI CDCMapNavView::ResetHint()
{
	if(m_TimerID)
	{
		KillTimer(m_hWnd,m_TimerID);
		m_TimerID=0;
	}
	m_pWorkspace->GetFrameUI()->HideHint();

}

void CDCMapNavView::OnActivate()
{
	RenderFocus(s_Shared.m_hdcBmp);
	dcmapHDC dc = GetDC(m_hWnd);
	RenderFocus(dc);
	ReleaseDC(m_hWnd,dc);
}

void CDCMapNavView::OnDeactivate()
{
	RenderFocus(s_Shared.m_hdcBmp);
	dcmapHDC dc = GetDC(m_hWnd);
	RenderFocus(dc);
	ReleaseDC(m_hWnd,dc);
}

void DCMAPAPI CDCMapNavView::Activate(bool Active)
{
	if(Active) SetFocus(m_hWnd);
}

bool DCMAPAPI CDCMapNavView::DragOver(int x,int y,int State,IDCMapDragObject* pDragInfo,IDCMapModule* pModule)
{
	if(State==0)OnMouseEnter();
	else if(State==1)OnMouseExit();
	else if(State==2)OnMouseMove(0,x,y);

	int cx = x-m_rcMap.left;
	int cy = y-m_rcMap.top;

	for(int i=0;i<m_Layers.size();i++)
	{
		if(!m_Layers[i].m_pLayerIface)continue;
		if(m_Layers[i].m_pLayerIface->DragOver(cx,cy,State,pDragInfo,pModule))return true;
	}
	return false;
}

bool DCMAPAPI CDCMapNavView::DragDrop(int x,int y,IDCMapDragObject* pDragInfo,IDCMapModule* pModule)
{
	int cx = x-m_rcMap.left;
	int cy = y-m_rcMap.top;

	for(int i=0;i<m_Layers.size();i++)
	{
		if(!m_Layers[i].m_pLayerIface)continue;
		if(m_Layers[i].m_pLayerIface->DragDrop(cx,cy,pDragInfo,pModule))return true;
	}
	return false;
}

bool DCMAPAPI CDCMapNavView::StartAnim(dcmapRECT const* rcPos,dcmapHDC* pDC,void* pGDIP)
{
	_ASSERT(rcPos);
	int w = rcPos->right-rcPos->left;
	int h = rcPos->bottom-rcPos->top;
	if(!s_Shared.InitAnim(m_hWnd,w,h))return false;

	// clear bmp from base image
	BitBlt(s_Shared.m_hdcAnimBmp,0,0,w,h,s_Shared.m_hdcBmp,rcPos->left+m_rcMap.left,rcPos->top+m_rcMap.top,SRCCOPY);

	if(pDC)*pDC = s_Shared.m_hdcAnimBmp;
	if(pGDIP)*((Graphics**)pGDIP) = s_Shared.m_pAnimGraphics;
	if(!m_bAnimating)m_bAnimating = 1;
	m_rcAnim = *rcPos;
	return true;
}

void DCMAPAPI CDCMapNavView::PresentAnim()
{
	if(m_bAnimating)
	{
		if(m_bAnimating > 1) // clear old rect
		{
			HRGN hNewRgn = CreateRectRgn(m_rcAnim.left+m_rcMap.left,m_rcAnim.top+m_rcMap.top,
									m_rcAnim.right+m_rcMap.left,m_rcAnim.bottom+m_rcMap.top);
			HRGN hOldRgn = CreateRectRgn(m_rcOldAnim.left+m_rcMap.left,m_rcOldAnim.top+m_rcMap.top,
									m_rcOldAnim.right+m_rcMap.left,m_rcOldAnim.bottom+m_rcMap.top);
			
			HRGN hResRgn = CreateRectRgn(0,0,1,1);

			int r = CombineRgn(hResRgn,hOldRgn,hNewRgn,RGN_DIFF);
			if(r!=NULLREGION)
			{
				InvalidateRgn(m_hWnd,hResRgn,FALSE);
			}
			DeleteRgn(hNewRgn);
			DeleteRgn(hOldRgn);
			DeleteRgn(hResRgn);
		}
		int w = m_rcAnim.right-m_rcAnim.left;
		int h = m_rcAnim.bottom-m_rcAnim.top;
		HDC dc = GetDC(m_hWnd);
		BitBlt(dc,m_rcAnim.left+m_rcMap.left,m_rcAnim.top+m_rcMap.top,w,h,s_Shared.m_hdcAnimBmp,0,0,SRCCOPY);
		ReleaseDC(m_hWnd,dc);
		m_rcOldAnim=m_rcAnim;
		m_bAnimating = 2;
	}
}

void DCMAPAPI CDCMapNavView::EndAnim()
{
	if(m_bAnimating)
	{
		int w = m_rcAnim.right-m_rcAnim.left;
		int h = m_rcAnim.bottom-m_rcAnim.top;
		HDC dc = GetDC(m_hWnd);
		BitBlt(dc,m_rcAnim.left+m_rcMap.left,m_rcAnim.top+m_rcMap.top,w,h,s_Shared.m_hdcBmp,
			m_rcAnim.left+m_rcMap.left,m_rcAnim.top+m_rcMap.top,SRCCOPY);
		ReleaseDC(m_hWnd,dc);
		m_bAnimating = false;
	}
}

IDCMapModule* DCMAPAPI CDCMapNavView::CreateCallback( void* pUserData )
{
	return new CDCMapNavView();
}
class CGeoDataProcessorFilter : public IDCMapFilteredDataSource
{
	vector<dcmapDBREF> m_Refs;
	vector<dcmapDBREF>::iterator current;

	IDCMapCore* m_pCore;
	IDCMapGeoValueCalculator* m_pGeoValue;

	IDCMapGeoDataStoragePtr m_pGeo;

	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pCore = core;
		if(!m_pGeo.init(core,"geo"))return false;
		return true;
	}
	bool DCMAPAPI SetTextParam(const char* szParam){return false;};
	bool DCMAPAPI SetDataSourceName(const char* szName){return false;};
	bool DCMAPAPI SetDataSource(IDCMapDataSource* pSource){return false;};
	bool DCMAPAPI Process(IDCMapProcessRecordReseiver* pRsvr,dcmapDWORD options)
	{
		IDCMapWorkspace* workspace = m_pCore->GetCurrentWorkspace();
		if(!workspace)return false;

		if(!workspace->SelectModuleByID("MapView"))return false;

		CDCMapNavView* pView = dynamic_cast<CDCMapNavView*>(workspace->Module());
		if(!pView)return false;
		std::vector<IDCMapGeoDataFilter*> &m_vecGeoFilters = pView->m_vecGeoFilters;

		m_pGeoValue = workspace->GetServiceTyped<IDCMapGeoValueCalculator>("GeoValueCalculator");
		if(!m_pGeoValue) return false;

		DCMapPlanetValueInfo value;
		if(m_pGeo->SelectAll())
			do 
			{
				DCMapPlanetParamInfo const* pinf = m_pGeo->GetData();
				m_pGeoValue->CalculateGeoIndirect(&pinf->geo,&value);
				bool r=true;
				for(int i=0;i<m_vecGeoFilters.size();i++)
				{
					if(!m_vecGeoFilters[i]->ProcessGeoData(pinf,&value)){r=false;break;}
				}
				if(r)m_Refs.push_back(pinf->id);
			} while(m_pGeo->Next());

			return true;
	}
	bool DCMAPAPI SelectAll(){current = m_Refs.begin();return current != m_Refs.end();}
	bool DCMAPAPI Next()
	{
		++current;
		return current != m_Refs.end();
	}
	const dcmapDBREF* DCMAPAPI GetID(){return &(*current); }
	int DCMAPAPI NumRecords(){return m_Refs.size();}

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapFilteredDataSource);
		DCMAP_MODULE_SUPPORTS(IDCMapDataSource);
		return 0;
	}
};
DCMAP_REGISTER(CGeoDataProcessorFilter,"PTableFilter_geo","CurrentGeoDataProcessorFilter");
