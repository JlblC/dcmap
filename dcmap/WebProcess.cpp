//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "WebProcess.h"           
#include "dcmapsyslib.h"
#include "acntUtils.hpp"
#include "BaseModule.h"
#include "CoreUIManager.h"
#include "Interface.h"
#include "Utils.h"
#include "ZLib.hpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma link "sGauge"
#pragma link "sLabel"
#pragma link "sButton"
#pragma link "IdZLibCompressorBase"
#pragma link "IdCompressorZLib"
#pragma link "IdZLibCompressorBase"
#pragma link "IdCompressorZLib"
#pragma link "IdZLibCompressorBase"
#pragma link "IdCompressorZLib"
#pragma link "IdZLibCompressorBase"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
#include "boost/scoped_ptr.hpp"
using boost::scoped_ptr;
//---------------------------------------------------------------------------

TWebProcessForm *WebProcessForm=0;

class CWebProcess : public IDCMapModule
{
	 int DCMAPAPI ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
	{
		if(type == DCMEXEC_QUERY) return DCMEXEC_CALL;

		if(type == DCMEXEC_CALL)
		{
			if(!command || _wcsicmp(command,L"CheckUpdates")==0)
			{
				if(!WebProcessForm)WebProcessForm = new TWebProcessForm(Application->MainForm);
                WebProcessForm->CheckUpdates();
				return DCMEXEC_SUCCESS;
			}
		}
		return DCMEXEC_FAIL;
	}

	int   DCMAPAPI OnMessage(int Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender)
	{
		switch(Msg)
		{
		case DCMM_EXECUTE:
			{
				if(!WebProcessForm)WebProcessForm = new TWebProcessForm(Application);
				dcmapWCSTR cmd = (dcmapWCSTR)lParam;
				if(cmd && cmd[0])
				{
					if(UnicodeString(cmd) == L"GameDataGet") WebProcessForm->TestGetData();
					else if(UnicodeString(cmd) == L"CheckUpdates") WebProcessForm->CheckUpdates();
				}
				break;
			}
		case DCMM_CHECK_WEB_UPDATES:
			{
				if(!WebProcessForm)WebProcessForm = new TWebProcessForm(Application);

				UnicodeString Url;
				UnicodeString Ver;

				int r = WebProcessForm->CheckUpdate(Url,Ver);

				if(r<0)
				{
					return DCMM_FAIL;
				}
				else if(r==0)
				{
					return DCMM_PROCESSED;
				}

				Dialogs::TMsgDlgButtons Buttons;
				Buttons << mbYes << mbNo ;
				if(sMessageDlg(UnicodeString(LSTRW(WebUpdateCaption))+Ver,
							LSTRW(WebUpdateConfirm),mtConfirmation,Buttons,0) != mrYes)return DCMM_SUCCESS;

				WebProcessForm->DoUpdate(Url);
				return DCMM_PROCESSED|DCMM_SUCCESS;
			}
			break;
		}
		return 0;
	};
};
static CAutoReg<CWebProcess> sWrap("PCoreUtility","WebProcess");


//---------------------------------------------------------------------------
__fastcall TWebProcessForm::TWebProcessForm(TComponent* Owner)
	: TForm(Owner)
{
	WebProcessForm = this;
	FreeGameURL = "http://free.the-game.ru";
	GameURL = "http://www.the-game.ru";
	m_pRec=0;
	m_iLock=0;
}
//---------------------------------------------------------------------------

int TWebProcessForm::CheckUpdateURL(UnicodeString Url,UnicodeString &Src,UnicodeString &Ver)
{
	scoped_ptr<TStringList> UpdateInfo(new TStringList);
	UpdateInfo->Text = WebGet(Url,1251);
	// First line is current version number of DCMap
	if(UpdateInfo->Count < 4)return -1;
	if(UpdateInfo->Strings[0] != L"DCMap Web Update") return -1;
	if(UpdateInfo->Strings[1] <= ToWide(DCMAP_VERSION))
	{
		return 0;
	}
	Src = UpdateInfo->Strings[3];
	Ver = UpdateInfo->Strings[2];
	return 1;
}

int TWebProcessForm::CheckUpdate(UnicodeString &Url,UnicodeString &Ver)
{
	UnicodeString UpdateHost = L"http://dcmap.googlecode.com/svn/update";
	UnicodeString UpdateURL;

	DCMapConfigWrapper cfg(GetDCMapCore()->GetConfig());

	cfg.section("Core");
	cfg["UpdateHost"] >> UpdateHost;

	cfg.section("WebUpdate");
	cfg["UpdateHost"] >> UpdateHost;
	cfg["UpdateURL"] >> UpdateURL;
	int r = -1;

	if(!UpdateURL.IsEmpty())
	{
		r = CheckUpdateURL(UpdateURL,Url,Ver);
	}
	else
	{
		r = CheckUpdateURL(UpdateHost+L"/current.txt",Url,Ver);
		if(r < 1 && DCMAP_BETA)
		{
			r = CheckUpdateURL(UpdateHost+L"/beta.txt",Url,Ver);
		}
	}

	return r;
}

int TWebProcessForm::DownloadFile(String Url,String FileName)
{
	if(m_iLock)return 0;
	++m_iLock;
	bool bCansel=false;
	try
	{
		scoped_ptr<TFileStream> Stream(new TFileStream(FileName,Classes::fmCreate));
		IdHTTP->Get(Url,Stream.get());
	}
	catch(...){bCansel=true;}
	if(!bCansel && m_pRec)
		bCansel = m_pRec->TestCancel();

	--m_iLock;
	if(bCansel)
	{
	   if(FileExists(FileName))DeleteFile(FileName);
	   return 0;
	}
	return 1;
}

wchar_t const* LocateFileName(wchar_t const* begin,wchar_t const* end=0)
{
	if(!end)end = begin+wcslen(begin);
	for(wchar_t const* pt = end;--pt>begin;)
	{
		if((*pt) == L'/' || (*pt) == L'\\')return pt+1;
	}
	return begin;
}

int TWebProcessForm::DownloadFiles(String Url,String Destination)
{
	m_pRec = GetDCMapCore()->GetCoreUI()->GetConsoleFidder();
	m_pRec->Show();
	m_pRec->ClearMessages();

	scoped_ptr<TStringList> FileList(new TStringList);
	FileList->Text =  WebGet(Url,1251);

	int r=0;

	for(int i=0;i<FileList->Count;i++)
	{
		 ExtractFilePath(Url);
		 String FileUrl = FileList->Strings[i];

		 String FileName = dcmapURLDecodeW(LocateFileName(FileList->Strings[i].w_str()));

		 m_pRec->AddMessage(("Downloading "+FileName).w_str());

		 if(!(r = DownloadFile(FileUrl,Destination+FileName)))break;
	}
	m_pRec->Hide();
	m_pRec=0;

	return r;
}

int TWebProcessForm::DoUpdate(UnicodeString Url)
{
	String FileName = GetAppPath()+"update.exe";;
	if(FileExists(FileName))DeleteFile(FileName);

	int r=1;

	m_pRec = GetDCMapCore()->GetCoreUI()->GetConsoleFidder();
	m_pRec->Show();
	m_pRec->ClearMessages();
	m_pRec->SetHeader(L"DCMap Update");
	m_pRec->AddMessage((L"Loading "+Url).w_str());

	r = DownloadFile(Url,FileName);

	m_pRec->Hide();
	m_pRec=0;

	if(!r)
	{
	   return 0;
	}
	// Save current database
	GetDCMapCore()->GetCoreUI()->CloseMainWnd();
	// Execute file
	ShellExecute(0,L"open",FileName.w_str(),0,GetAppPath().w_str(),SW_SHOW);
	return r;
}


void TWebProcessForm::DoCheckUpdates()
{
	UnicodeString Url;
	UnicodeString Ver;

	int r = CheckUpdate(Url,Ver);

	if(r<0)
	{
		ShowMessage(LSTRW(WebUpdateNoServerAccess));
		return;
	}
	if(!r)
	{
		ShowMessage(LSTRW(WebUpdateLatest));
		return;
	}

	Dialogs::TMsgDlgButtons Buttons;
	Buttons << mbYes << mbNo ;
	if(sMessageDlg(LSTRW(WebUpdateCaption)+Ver,
				LSTRW(WebUpdateConfirm),mtConfirmation,Buttons,0) != mrYes)return;

	DoUpdate(Url);
}

void TWebProcessForm::CheckUpdates()
{
	DoCheckUpdates();
	Hide();
}

void __fastcall TWebProcessForm::IdHTTPWorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax)
{
	if(m_pRec)
	{
		if(AWorkMode==wmRead)m_pRec->SetStatus(L"Downloading...");
		m_pRec->SetupProgress(AWorkCountMax);
	}
	Application->ProcessMessages();
	/*Gauge->MaxValue = AWorkCountMax;
	Gauge->Progress = 0;
    Gauge->Visible = true;*/
}
//---------------------------------------------------------------------------

void __fastcall TWebProcessForm::IdHTTPWorkEnd(TObject *ASender, TWorkMode AWorkMode)
{
	if(m_pRec)
	{
		m_pRec->SetupProgress(0);
		if(AWorkMode==wmRead)
		{
			m_pRec->SetStatus(0);
		}
		if(AWorkMode==wmWrite)
		{
			m_pRec->SetStatus(L"Waiting for answer...");
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TWebProcessForm::IdHTTPWork(TObject *ASender,
	  TWorkMode AWorkMode, __int64 AWorkCount)
{
	if(m_pRec)
	{
		if(AWorkMode==wmRead)m_pRec->SetStatus(ToWide("Downloaded: "+IntToStr(AWorkCount)).w_str());
		m_pRec->UpdateProgress(AWorkCount);
		if(m_pRec->PeekCancel())IdHTTP->Disconnect();
	}
}


void __fastcall TWebProcessForm::IdHTTPStatus(TObject *ASender, const TIdStatus AStatus, const AnsiString AStatusText)
{
	if(m_pRec)m_pRec->SetStatus(ToWide(AStatusText).w_str());
	StatusLabel->Caption = AStatusText;
}
//---------------------------------------------------------------------------
void TWebProcessForm::ConfigureConnection()
{
    DCMapConfigWrapper cfg(GetDCMapCore()->GetConfig());
	cfg.section("NetSettings");

    String str;
    int    val;
    bool   bval=false;

    if(cfg["GameURL"] >> str)GameURL = str;

    if(cfg["UseProxy"] >> bval);
    if(bval)
    {
    	if(cfg["ProxyServer"] >> str)IdHTTP->ProxyParams->ProxyServer = str;
        if(cfg["ProxyUser"] >> str)IdHTTP->ProxyParams->ProxyUsername = str;
        if(cfg["ProxyPass"] >> str)IdHTTP->ProxyParams->ProxyPassword = str;
    	if(cfg["ProxyPort"] >> val)IdHTTP->ProxyParams->ProxyPort = val;
        if(cfg["UseBasicAuth"] >> bval)IdHTTP->ProxyParams->BasicAuthentication =  bval;
    }
    else
    {
    	IdHTTP->ProxyParams->ProxyServer = "";
        IdHTTP->ProxyParams->ProxyUsername = "";
        IdHTTP->ProxyParams->ProxyPassword = "";
    	IdHTTP->ProxyParams->ProxyPort = 0;
		IdHTTP->ProxyParams->BasicAuthentication =  false;
	}

	//IdHTTP->Request->UserAgent = AnsiString("Mozilla/3.0 (compatible; DCMap/")+DCMAP_VERSION_FAMILY+")";

	IdHTTP->Request->UserAgent = AnsiString("DCMap/")+DCMAP_VERSION_FAMILY+"";
	IdHTTP->ConnectTimeout = 10000;
}

bool TWebProcessForm::DoLogin(UnicodeString szLogin,UnicodeString szPass,bool free)
{
	SetState("Login...");

	UnicodeString Login = dcmapURLEncodeW(szLogin.w_str(),CP_UTF8);
	UnicodeString Pass = dcmapURLEncodeW(szPass.w_str(),CP_UTF8);

	if(!CurrentGameURL.IsEmpty())DoLogout();

	if(free) CurrentGameURL = FreeGameURL;
	else CurrentGameURL = GameURL;

	UnicodeString Url = CurrentGameURL+L"/overview/";
	UnicodeString Post = (UnicodeString)L"action=login&login="+Login+"&pwd="+Pass;
	//Post = TIdURI::URLEncode(Post);

	UnicodeString Result;
	Result = WebPost(Url,Post,CP_UTF8);

	return true;
}

bool TWebProcessForm::DoLoginPlayer(DCMapDataReference ref)
{
	IdCookieManager->CookieCollection->Clear();

    IDCMapPlayersDataStoragePtr m_pPlayers;
    if(!m_pPlayers.init(GetDCMapCore(),"players"))return false;
    if(!m_pPlayers->SelectID(ref))return false;
	m_LoginPlayer = ref;

	return DoLogin(m_pPlayers[DCMDB_PLAYER_LOGIN],m_pPlayers[DCMDB_PLAYER_PASSWORD]);
}

bool TWebProcessForm::DoScriptExec()
{
	IDCMapPlayersDataStoragePtr m_pPlayers;
    if(!m_pPlayers.init(GetDCMapCore(),"players"))return false;
    if(!m_pPlayers->SelectID(m_LoginPlayer))return false;

    String  FleetID = m_pPlayers[DCMDB_PLAYER_SCRIPTING_FLEET_ID];

	String Url = CurrentGameURL+"/blackframes/script_debugger/on/"+FleetID+"/";

	scoped_ptr<TStringList> ScriptData(new TStringList);

	ScriptData->LoadFromFile(Acntutils::GetAppPath()+"Client/script.pl");

	scoped_ptr<TStringList> PostData(new TStringList);
	ScriptData->Text = dcmapURLEncodeW(ScriptData->Text.w_str(),CP_UTF8);

	PostData->Text = "add=&action=update_fleet_variable&fleet="+FleetID+
					"&storedvar=&script_text="+ScriptData->Text+"&run=Run";

    SetState("Executing script...");

    String Result;
    try
    {
		Result = IdHTTP->Post(Url,PostData.get());
    }
    catch(EIdException& e)
    {
    	return false;
	}
	scoped_ptr<TStringList> Res(new TStringList);
    Res->Text = Result;
	//Res->SaveToFile(GetAppPath()+"Client/result.html");

    IDCMapRecordImport* pImp = GetDCMapCore()->GetServiceTyped<IDCMapRecordImport>("RecordImport");
    if(!pImp)return false;

    SetState("Processing data...");
    GetDCMapCore()->GetConsole()->ExecuteFromWebScript(UTF8ToUnicodeString(Result).w_str());
	return true;
}

bool TWebProcessForm::DoLogout()
{
    SetState("Logout...");
	m_LoginPlayer.Invalidate();
	GameGet(L"/logoff/");
	CurrentGameURL="";
	return true;
}

void TWebProcessForm::TestGetData()
{
	ConfigureConnection();

    IDCMapPlayersDataStoragePtr m_pPlayers;
    if(!m_pPlayers.init(GetDCMapCore(),"players"))return;

	Show();

	try
	{
		if(m_pPlayers->SelectAll())
		do
		{
			if(m_pPlayers[DCMDB_PLAYER_WEBACCESS])
			{
				 DoLoginPlayer(*m_pPlayers->GetID());
				 DoScriptExec();
				 DoLogout();
			}
		}
		while(m_pPlayers->Next());
	}
	catch(...){}
    Hide();
}

UnicodeString DCMAPAPI TWebProcessForm::WebGet(UnicodeString const& URL,int CodePage)
{
	if(m_iLock) return L"";
	++m_iLock;
	ConfigureConnection();

	TMemoryStream* ResultData = new TMemoryStream();
	String Url = WideToString(URL,CodePage);
	UnicodeString Result;
	bool ok=true;
	try
	{
		IdHTTP->Get(Url,ResultData);
	}
	catch(...)
	{
		ok=false;
	}
	if(ok) Result = ToWide((char*)ResultData->Memory,ResultData->Size,CodePage);
	delete ResultData;

	IdHTTP->Disconnect(true);

	--m_iLock;
	return Result;
}

UnicodeString DCMAPAPI TWebProcessForm::WebPost(UnicodeString const& URL,UnicodeString const& Post,int CodePage)
{
	if(m_iLock) return L"";
	++m_iLock;
	ConfigureConnection();

	String Url = WideToString(URL,CodePage);
	UnicodeString Result;

	TStringStream* PostData = new TStringStream(WideToString(Post,CodePage));
	TMemoryStream* ResultData = new TMemoryStream();
	bool ok=true;
	try
	{
		IdHTTP->Post(Url,PostData,ResultData);
	}
	catch(...)
	{
		ok=false;
	}
	delete PostData;
	if(ok) Result = ToWide((char*)ResultData->Memory,ResultData->Size,CodePage);
	delete ResultData;

	IdHTTP->Disconnect(true);

	--m_iLock;
	return Result;
}

UnicodeString DCMAPAPI TWebProcessForm::GameGet(UnicodeString const& srcURL)
{
	UnicodeString URL = srcURL;
	if(!URL.IsEmpty() && URL[1] != L'/') URL = UnicodeString(L"/")+srcURL;
	URL = CurrentGameURL+URL;
	return WebGet(URL,CP_UTF8);
}

UnicodeString DCMAPAPI TWebProcessForm::GamePost(UnicodeString const& srcURL,UnicodeString const& Post)
{
	UnicodeString URL = srcURL;
	if(!URL.IsEmpty() && URL[1] != L'/') URL = UnicodeString(L"/")+srcURL;
	URL = CurrentGameURL+URL;
	return WebPost(URL,Post,CP_UTF8);
}

void TWebProcessForm::SetState(String State)
{
    OperationLabel->Caption = State;
	Application->ProcessMessages();
}

void __fastcall TWebProcessForm::FormCreate(TObject *Sender)
{
	InterfaceModule->LocalizeContainer(this);
}
//---------------------------------------------------------------------------

void __fastcall TWebProcessForm::Timer1Timer(TObject *Sender)
{
	Application->ProcessMessages();
}
//---------------------------------------------------------------------------


