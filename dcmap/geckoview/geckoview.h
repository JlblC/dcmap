#pragma once

#include "GeckoManager.h"

class CGeckoView: public IDCMapView,
				  public nsIWebBrowserChrome,
				  public nsIWebProgressListener,
				  public nsISHistoryListener,
				  public nsIEmbeddingSiteWindow,
				  public nsSupportsWeakReference
{
public:
	NS_DECL_ISUPPORTS
	NS_DECL_NSIWEBBROWSERCHROME
	NS_DECL_NSIWEBPROGRESSLISTENER
	NS_DECL_NSIEMBEDDINGSITEWINDOW
	NS_DECL_NSISHISTORYLISTENER
protected:
	CGeckoManager* m_pGeckoMan;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM  lParam);

	HWND m_hWnd;

	void CreateBrowser(dcmapHWND Parent,RECT const* rcWindow);
protected:
	nsCOMPtr<nsIWebBrowser> m_WebBrowser;
	nsCOMPtr<nsIBaseWindow> m_BrowserWindow;
	nsCOMPtr<nsIWebNavigation> m_WebNav;

public: // base interface
	CGeckoView();
	virtual ~CGeckoView();
	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	void* DCMAPAPI CastTo( DCMapInterfaceID idInterface );

	dcmapLRESULT DCMAPAPI OnMessage(dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender);

	tagWNDCLASSW* DCMAPAPI RegisterWndClass();
	void  DCMAPAPI AfterCreate(dcmapHWND wnd,dcmapHWND hwndParent,dcmapHWND hwndMain);
	void* DCMAPAPI BeforeCreate(dcmapHWND hwndParent,dcmapHWND hwndMain);
	void DCMAPAPI OnResize(dcmapRECT const* rcClient);
	void DCMAPAPI OnResize(int w,int h);
	void DCMAPAPI Activate(bool Active);
	void OnActivate();
	void OnDeactivate();
	dcmapHWND DCMAPAPI Create(dcmapHWND hwndMain,dcmapHWND hwndParent,dcmapRECT const* rcClient);
public:
};
DCMAP_SPEC_INTERFACE(CGeckoView);
