//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "WebView.h"
#include "WebBase.hpp"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sComboBox"
#pragma link "sToolBar"
#pragma link "EmbeddedWB"
#pragma link "EwbCore"
#pragma link "SHDocVw_EWB"
#pragma link "Wininet.lib"
#pragma link "Urlmon.lib"
#pragma link "shlwapi.lib"

#pragma resource "*.dfm"
TWebViewFrame *WebViewFrame;

static CFramePanelWrapper<TWebViewFrame> sWrap("PView","WebView");

//---------------------------------------------------------------------------
__fastcall TWebViewFrame::TWebViewFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}

bool TWebViewFrame::OnInit()
{
	if(!m_pPlayers.init(m_pCore))return false;

	Browser = CreateBrowser(this);
	Browser->Parent = this;

	TBrowserParams* params = new TBrowserParams;
	params->UserAgend = String(L"DCMap/")+DCMAP_VERSION_FAMILY;
	Browser->SetupBrowser(params);
	delete params;


    UpdateLoginMenu();
	return TBaseModuleFrame::OnInit();
}

void TWebViewFrame::OnShow()
{
	Browser->SetUserName(L"");
	Browser->HomePageURL = "http://the-game.ru";
	Browser->Go(Browser->HomePageURL);
}

void TWebViewFrame::UpdateLoginMenu()
{
	int n = Browser->LoginMenu->Items->Count;
    for(int i=0;i<n;i++)
    {
		Browser->LoginMenu->Items->Items[0]->Free();
	}

	if(m_pPlayers->SelectAll())
	{
        do
        {
            String Login = m_pPlayers[DCMDB_PLAYER_LOGIN];
            if(Login.IsEmpty())continue;

            TMenuItem* pItem = new TMenuItem(this);
            pItem->Caption = m_pPlayers->GetWideName();
            pItem->Hint = m_pPlayers->GetWideName();
			pItem->Tag = m_pPlayers[DCMDB_PLAYER_USER_ID];
			pItem->OnClick = OnLoginMenuCLick;
			Browser->LoginMenu->Items->Add(pItem);
        }
        while(m_pPlayers->Next());
        InterfaceModule->HookMenu(Browser->LoginMenu);
	}
}

int TWebViewFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
	case DCMM_DATA_CHANGED:
	  if(m_pPlayers.actualize_state())
	  {
          UpdateLoginMenu();
      }
      break;
    }
    return 0;
}

void __fastcall TWebViewFrame::OnLoginMenuCLick(TObject *Sender)
{
	TMenuItem* pItem = dynamic_cast<TMenuItem*>(Sender);
	if(!pItem)return;

    if(!(m_pPlayers->SelectGameID(pItem->Tag) ||
         m_pPlayers->SelectWideName(pItem->Hint.w_str())))return;

	String Login = dcmapURLEncodeW(m_pPlayers[DCMDB_PLAYER_LOGIN]);
	String Pass  = dcmapURLEncodeW(m_pPlayers[DCMDB_PLAYER_PASSWORD]);

	String Url  =  L"http://www.the-game.ru/overview/";
	String Post = (String)L"action=login&login="+Login+"&pwd="+Pass;

    Browser->Go("http://www.the-game.ru/logoff/");
    Browser->Wait(2000);
	Browser->Go(Url,Post);

	Browser->SetUserName(m_pPlayers->GetWideName());
}

//---------------------------------------------------------------------------








