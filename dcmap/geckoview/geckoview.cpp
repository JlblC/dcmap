// geckoview.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "geckoview.h"

DCMAP_REGISTER(CGeckoView,"PView","GeckoWebView");

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

NS_IMPL_ADDREF(CGeckoView)
NS_IMPL_RELEASE(CGeckoView)

 NS_INTERFACE_MAP_BEGIN(CGeckoView)
    NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIWebBrowserChrome)
    NS_INTERFACE_MAP_ENTRY(nsIWebBrowserChrome)
    NS_INTERFACE_MAP_ENTRY(nsIEmbeddingSiteWindow)
    NS_INTERFACE_MAP_ENTRY(nsIWebProgressListener)
	NS_INTERFACE_MAP_ENTRY(nsISHistoryListener)
	NS_INTERFACE_MAP_ENTRY(nsISupportsWeakReference)
 NS_INTERFACE_MAP_END


 /////////  nsIWebBrowserChrome
 //////////////////////////////////////////////////////////////////////////

 /* void setStatus (in unsigned long statusType, in wstring status); */
 NS_IMETHODIMP CGeckoView::SetStatus(PRUint32 statusType, const PRUnichar *status)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsIWebBrowser webBrowser; */
NS_IMETHODIMP CGeckoView::GetWebBrowser(nsIWebBrowser * *aWebBrowser)
{
	NS_ENSURE_ARG_POINTER(aWebBrowser);
	*aWebBrowser = m_WebBrowser;
	NS_IF_ADDREF(*aWebBrowser);
	return NS_OK;
}
NS_IMETHODIMP CGeckoView::SetWebBrowser(nsIWebBrowser * aWebBrowser)
{
	m_WebBrowser = aWebBrowser;
	return NS_OK;
}

/* attribute unsigned long chromeFlags; */
NS_IMETHODIMP CGeckoView::GetChromeFlags(PRUint32 *aChromeFlags)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP CGeckoView::SetChromeFlags(PRUint32 aChromeFlags)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* void destroyBrowserWindow (); */
NS_IMETHODIMP CGeckoView::DestroyBrowserWindow()
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* void sizeBrowserTo (in long aCX, in long aCY); */
NS_IMETHODIMP CGeckoView::SizeBrowserTo(PRInt32 aCX, PRInt32 aCY)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* void showAsModal (); */
NS_IMETHODIMP CGeckoView::ShowAsModal()
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isWindowModal (); */
NS_IMETHODIMP CGeckoView::IsWindowModal(PRBool *_retval)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* void exitModalEventLoop (in nsresult aStatus); */
NS_IMETHODIMP CGeckoView::ExitModalEventLoop(nsresult aStatus)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}


/////////  nsIEmbeddingSiteWindow
//////////////////////////////////////////////////////////////////////////

/* void setDimensions (in unsigned long flags, in long x, in long y, in long cx, in long cy); */
NS_IMETHODIMP CGeckoView::SetDimensions(PRUint32 flags, PRInt32 x, PRInt32 y, PRInt32 cx, PRInt32 cy)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getDimensions (in unsigned long flags, out long x, out long y, out long cx, out long cy); */
NS_IMETHODIMP CGeckoView::GetDimensions(PRUint32 flags, PRInt32 *x, PRInt32 *y, PRInt32 *cx, PRInt32 *cy)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setFocus (); */
NS_IMETHODIMP CGeckoView::SetFocus()
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean visibility; */
NS_IMETHODIMP CGeckoView::GetVisibility(PRBool *aVisibility)
{
	NS_ENSURE_ARG_POINTER(aVisibility);
	*aVisibility = PR_TRUE;
	return NS_OK;
}

NS_IMETHODIMP CGeckoView::SetVisibility(PRBool aVisibility)
{
	return NS_OK;
}

/* attribute wstring title; */
NS_IMETHODIMP CGeckoView::GetTitle(PRUnichar * *aTitle)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP CGeckoView::SetTitle(const PRUnichar * aTitle)
{
	return NS_OK;
}

/* [noscript] readonly attribute voidPtr siteWindow; */
NS_IMETHODIMP CGeckoView::GetSiteWindow(void * *aSiteWindow)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

////////////  nsIWebProgressListener
//////////////////////////////////////////////////////////////////////////

/* void onStateChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in unsigned long aStateFlags, in nsresult aStatus); */
NS_IMETHODIMP CGeckoView::OnStateChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aStateFlags, nsresult aStatus)
{
	if ((aStateFlags & STATE_START) && (aStateFlags & STATE_IS_DOCUMENT))
	{
	}

	if ((aStateFlags & STATE_STOP) && (aStateFlags & STATE_IS_DOCUMENT))
	{
	}
	return NS_OK;
}

/* void onProgressChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in long aCurSelfProgress, in long aMaxSelfProgress, in long aCurTotalProgress, in long aMaxTotalProgress); */
NS_IMETHODIMP CGeckoView::OnProgressChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRInt32 aCurSelfProgress, PRInt32 aMaxSelfProgress, PRInt32 aCurTotalProgress, PRInt32 aMaxTotalProgress)
{
	//WebBrowserChromeUI::UpdateProgress(this, curTotalProgress, maxTotalProgress);
	return NS_OK;
}

/* void onLocationChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in nsIURI aLocation); */
NS_IMETHODIMP CGeckoView::OnLocationChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsIURI *aLocation)
{
	PRBool isSubFrameLoad = PR_FALSE; // Is this a subframe load
	if (aWebProgress) {
		nsCOMPtr<nsIDOMWindow>  domWindow;
		nsCOMPtr<nsIDOMWindow>  topDomWindow;
		aWebProgress->GetDOMWindow(getter_AddRefs(domWindow));
		if (domWindow) { // Get root domWindow
			domWindow->GetTop(getter_AddRefs(topDomWindow));
		}
		if (domWindow != topDomWindow)
			isSubFrameLoad = PR_TRUE;
	}
	if (!isSubFrameLoad)
	{
		//WebBrowserChromeUI::UpdateCurrentURI(this);
	}
	return NS_OK;
}

/* void onStatusChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in nsresult aStatus, in wstring aMessage); */
NS_IMETHODIMP CGeckoView::OnStatusChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsresult aStatus, const PRUnichar *aMessage)
{
	return NS_OK;
}

/* void onSecurityChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in unsigned long aState); */
NS_IMETHODIMP CGeckoView::OnSecurityChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aState)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

////////nsISHistoryListener
//////////////////////////////////////////////////////////////////////////


/* void OnHistoryNewEntry (in nsIURI aNewURI); */
NS_IMETHODIMP CGeckoView::OnHistoryNewEntry(nsIURI *aNewURI)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean OnHistoryGoBack (in nsIURI aBackURI); */
NS_IMETHODIMP CGeckoView::OnHistoryGoBack(nsIURI *aBackURI, PRBool *_retval)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean OnHistoryGoForward (in nsIURI aForwardURI); */
NS_IMETHODIMP CGeckoView::OnHistoryGoForward(nsIURI *aForwardURI, PRBool *_retval)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean OnHistoryReload (in nsIURI aReloadURI, in unsigned long aReloadFlags); */
NS_IMETHODIMP CGeckoView::OnHistoryReload(nsIURI *aReloadURI, PRUint32 aReloadFlags, PRBool *_retval)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean OnHistoryGotoIndex (in long aIndex, in nsIURI aGotoURI); */
NS_IMETHODIMP CGeckoView::OnHistoryGotoIndex(PRInt32 aIndex, nsIURI *aGotoURI, PRBool *_retval)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean OnHistoryPurge (in long aNumEntries); */
NS_IMETHODIMP CGeckoView::OnHistoryPurge(PRInt32 aNumEntries, PRBool *_retval)
{
	return NS_ERROR_NOT_IMPLEMENTED;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void* DCMAPAPI CGeckoView::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(IDCMapView);
	DCMAP_MODULE_SUPPORTS(CGeckoView);
	return 0;
}

CGeckoView::CGeckoView()
{
	NS_ADDREF_THIS();
}

CGeckoView::~CGeckoView()
{
	
}

void DCMAPAPI CGeckoView::OnResize( dcmapRECT const* rcClient )
{

}

void DCMAPAPI CGeckoView::OnResize(int w,int h)
{
	if(m_BrowserWindow)	
	{
		m_BrowserWindow->SetSize(w,h,true);
		m_BrowserWindow->SetVisibility(PR_TRUE);
	}
}

dcmapHWND DCMAPAPI CGeckoView::Create(dcmapHWND hwndParent,dcmapHWND hwndMain,dcmapRECT const* rcClient)
{
	HINSTANCE hInst = GetModuleHandle(0);

	m_hWnd = hwndParent;
	CreateBrowser(hwndParent,rcClient);

	m_hWnd = FindWindowEx(hwndParent,NULL,L"MozillaContentWindowClass",NULL);

	return m_hWnd;
}

void* DCMAPAPI CGeckoView::BeforeCreate(dcmapHWND hwndParent,dcmapHWND hwndMain)
{
	//m_hWnd = hwndParent;
	//CreateBrowser();

	return this;
}

void  DCMAPAPI CGeckoView::AfterCreate(dcmapHWND wnd,dcmapHWND hwndParent,dcmapHWND hwndMain)
{
	m_hWnd = wnd;
	RECT rc = {0,0,300,300};
	CreateBrowser(hwndParent,&rc);
}

LRESULT CALLBACK CGeckoView::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM  lParam)
{
	CGeckoView* pView = (CGeckoView*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
	switch(message)
	{
	case WM_ACTIVATE:
		if(wParam == WA_INACTIVE)pView->OnDeactivate();
		else pView->OnActivate();
		break;
	case WM_LBUTTONUP:
		pView->OnActivate();
			break;
	case WM_CREATE:
		{
			CREATESTRUCT* cs = ((CREATESTRUCT*)lParam);

			SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)cs->lpCreateParams);
			((CGeckoView*)((CREATESTRUCT*)lParam)->lpCreateParams)->m_hWnd = hwnd;
			return (0);
		}
	case WM_SIZE:
		pView->OnResize(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		break;
	case WM_CHAR:
		{
			int i=0;
		}
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}

tagWNDCLASSW* DCMAPAPI CGeckoView::RegisterWndClass()
{
	HINSTANCE hInst = GetModuleHandle(0);
	static wstring cln; 
	cln = L"CGeckoView";

	static tagWNDCLASSW cl;
	ZeroMemory(&cl,sizeof(cl));
	//cl.cbSize = sizeof(cl);
	cl.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
	cl.style = 0;//CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
	cl.lpszClassName = cln.c_str();
	//cl.hbrBackground = (dcmapHBRUSH)GetStockObject(BLACK_BRUSH);
	cl.hInstance = hInst;
	cl.lpfnWndProc = WndProc;
	cl.hCursor		= LoadCursor(NULL, IDC_ARROW);
	RegisterClassW(&cl);

	return &cl;
}

bool DCMAPAPI CGeckoView::Init( IDCMapCore* core,IDCMapWorkspace* workspace )
{
	m_pGeckoMan = core->GetServiceTyped<CGeckoManager>("GeckoManager");
	if(!m_pGeckoMan)return false;

	return true;
}

dcmapLRESULT DCMAPAPI CGeckoView::OnMessage( dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender )
{
	switch(Msg)
	{
	case DCMM_POST_INIT:
		break;
	}
	return 0;
}

void CGeckoView::OnActivate()
{
	//::SetFocus(m_hWnd);

	nsCOMPtr<nsIWebBrowserFocus> focus = do_QueryInterface(m_WebBrowser);
	focus->Activate();
}

void CGeckoView::OnDeactivate()
{

}

void DCMAPAPI CGeckoView::Activate(bool Active)
{
	if(Active) ::SetFocus(m_hWnd);
}

void CGeckoView::CreateBrowser(dcmapHWND hwndParent,RECT const* rcWindow)
{
	nsresult nr;
	m_WebBrowser = do_CreateInstance(NS_WEBBROWSER_CONTRACTID);
	m_BrowserWindow = do_QueryInterface(m_WebBrowser);
    m_WebNav = do_QueryInterface(m_WebBrowser);

	nr = m_WebBrowser->SetContainerWindow(static_cast<nsIWebBrowserChrome*>(this));

	nsCOMPtr<nsIWebBrowserSetup> setup(do_QueryInterface(m_WebBrowser));  
	//if (setup) 
	//	setup->SetProperty(nsIWebBrowserSetup::SETUP_IS_CHROME_WRAPPER,PR_TRUE); 

	nr = m_BrowserWindow->InitWindow(hwndParent,nsnull,
		rcWindow->left,rcWindow->top,
		rcWindow->right-rcWindow->left,rcWindow->bottom-rcWindow->top);

	nr = m_BrowserWindow->Create();	

	nsCOMPtr<nsIWebProgressListener> listener(static_cast<nsIWebProgressListener*>(this));
	nsCOMPtr<nsIWeakReference> thisListener(do_GetWeakReference(listener));

	nr = m_WebBrowser->AddWebBrowserListener(thisListener,NS_GET_IID(nsIWebProgressListener));

	// The window has been created. Now register for history notifications
	nr = m_WebBrowser->AddWebBrowserListener(thisListener, NS_GET_IID(nsISHistoryListener));

	nr = m_BrowserWindow->SetVisibility(PR_TRUE);

	nsCOMPtr<nsIWebBrowserFocus> focus = do_QueryInterface(m_WebBrowser);
	focus->Activate();


	nr = m_WebNav->LoadURI(L"http://www.the-game.ru/",nsIWebNavigation::LOAD_FLAGS_NONE,nsnull,nsnull,nsnull);

	//nr = m_WebNav->LoadURI(L"http://2ip.ru/",nsIWebNavigation::LOAD_FLAGS_NONE,nsnull,nsnull,nsnull);

	
	return;
}

