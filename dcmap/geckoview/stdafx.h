// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
#include <string>
using std::string;
using std::wstring;

#define XP_WIN

#include "geckosdk/nsStringAPI.h"
#include "geckosdk/nsCOMPtr.h"
#include "geckosdk/nsWeakReference.h"

#include "geckosdk/nsEmbedAPI.h"
#include "geckosdk/nsComponentManagerUtils.h"
#include "geckosdk/nsEmbedCID.h"

#include "geckosdk/nsIBaseWindow.h"
#include "geckosdk/nsIWebBrowser.h"
#include "geckosdk/nsIWebBrowserChrome.h"
#include "geckosdk/nsIWindowCreator.h"
#include "geckosdk/nsIWebProgressListener.h"
#include "geckosdk/nsIWebNavigation.h"
#include "geckosdk/nsIEmbeddingSiteWindow.h"
#include "geckosdk/nsIWebBrowserSetup.h"
#include "geckosdk/nsIDOMWindow.h"
#include "geckosdk/nsIWebProgress.h"
#include "geckosdk/nsISHistoryListener.h"
#include "geckosdk/nsIWebBrowserFocus.h"

//#include "geckosdk/nsISupportsWeakReference.h"

#include "../api/dcmap.h"




