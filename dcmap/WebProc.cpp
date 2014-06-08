//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "WebProc.h"
#include "BaseModule.h"
#include "Utils.h"
#include "sDialogs.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWebModule *WebModule = 0;
//---------------------------------------------------------------------------

#include "boost/scoped_ptr.hpp"
using boost::scoped_ptr;

static CAutoReg<CWebProcService> sWrap("PCoreService","WebProcess");


CWebProcService::CWebProcService()
{

}

void* DCMAPAPI CWebProcService::CastTo(DCMapInterfaceID idInterface)
{
	DCMAP_MODULE_SUPPORTS(IDCMapWebProcess);
	return 0;
}

bool  DCMAPAPI CWebProcService::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	if(WebModule == 0)WebModule = new TWebModule(Application);
	return true;
}

int   DCMAPAPI CWebProcService::OnMessage( int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender )
{
		switch(Msg)
		{
		case DCMM_EXECUTE:
			{
				dcmapWCSTR cmd = (dcmapWCSTR)lParam;
				if(cmd && cmd[0])
				{
					if(UnicodeString(cmd) == L"GameDataGet") WebModule->TestGetData();
					else if(UnicodeString(cmd) == L"CheckUpdates") WebModule->CheckUpdates();
				}
				break;
			}
		case DCMM_CHECK_WEB_UPDATES:
			{
				UnicodeString Url;
				UnicodeString Ver;

				int r = WebModule->CheckUpdate(Url,Ver);

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

				WebModule->DoUpdate(Url);
				return DCMM_PROCESSED|DCMM_SUCCESS;
			}
			break;
		}
	return 0;
}

bool DCMAPAPI CWebProcService::GameLogin(dcmapWCSTR Login,dcmapWCSTR Pass,bool free,IDCMapProcessRecordReseiver *pRec)
{
	WebModule->m_pRec = pRec;
	bool r= WebModule->DoLogin(Login,Pass,free);
	WebModule->m_pRec = 0;
	return b;
}

bool DCMAPAPI CWebProcService::GameLogout(IDCMapProcessRecordReseiver *pRec)
{
	WebModule->m_pRec = pRec;
	bool r= WebModule->DoLogout();
	WebModule->m_pRec = 0;
	return b;
}

static UnicodeString WebTempStr;

dcmapWCSTR DCMAPAPI CWebProcService::WebGet(dcmapWCSTR URL,IDCMapProcessRecordReseiver *pRec,int CodePage)
{
	WebModule->m_pRec = pRec;
	WebTempStr = WebModule->WebGet(URL,CodePage);
	WebModule->m_pRec = 0;
	return WebTempStr.w_str();
}

dcmapWCSTR DCMAPAPI CWebProcService::WebPost(dcmapWCSTR URL,dcmapWCSTR PostData,IDCMapProcessRecordReseiver *pRec,int CodePage)
{
	WebModule->m_pRec = pRec;
	WebTempStr = WebModule->WebPost(URL,PostData,CodePage);
	WebModule->m_pRec = 0;
	return WebTempStr.w_str();
}

dcmapWCSTR DCMAPAPI CWebProcService::GameGet(dcmapWCSTR URL,IDCMapProcessRecordReseiver *pRec)
{
	WebModule->m_pRec = pRec;
	WebTempStr = WebModule->GameGet(URL);
	WebModule->m_pRec = 0;
	return WebTempStr.w_str();
}

dcmapWCSTR DCMAPAPI CWebProcService::GamePost(dcmapWCSTR URL,dcmapWCSTR PostData,IDCMapProcessRecordReseiver *pRec)
{
	WebModule->m_pRec = pRec;
	WebTempStr = WebModule->GamePost(URL,PostData);
	WebModule->m_pRec = 0;
	return WebTempStr.w_str();
}


int DCMAPAPI CWebProcService::ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
{
	if(type == DCMEXEC_QUERY) return DCMEXEC_CALL;

	if(type == DCMEXEC_CALL)
	{
		if(!command || _wcsicmp(command,L"CheckUpdates")==0)
		{
			WebModule->CheckUpdates();
			return DCMEXEC_SUCCESS;
		}
	}

	return DCMEXEC_FAIL;
}

__fastcall TWebModule::TWebModule(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------


int TWebModule::CheckUpdateURL(String Url,String &Src,String &Ver)
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

int TWebModule::CheckUpdate(String &Url,String &Ver)
{
	String UpdateHost = L"http://dcmap.googlecode.com/svn/update";
	String UpdateURL;

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

int TWebModule::DownloadFile(String Url,String FileName)
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

int TWebModule::DownloadFiles(String Url,String Destination)
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

int TWebModule::DoUpdate(String Url)
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


void TWebModule::DoCheckUpdates()
{
	String Url;
	String Ver;

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

void TWebModule::CheckUpdates()
{
	DoCheckUpdates();
}

void __fastcall TWebModule::IdHTTPWorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax)
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

void __fastcall TWebModule::IdHTTPWorkEnd(TObject *ASender, TWorkMode AWorkMode)
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
void __fastcall TWebModule::IdHTTPWork(TObject *ASender,
	  TWorkMode AWorkMode, __int64 AWorkCount)
{
	if(m_pRec)
	{
		if(AWorkMode==wmRead)m_pRec->SetStatus(ToWide("Downloaded: "+IntToStr(AWorkCount)).w_str());
		m_pRec->UpdateProgress(AWorkCount);
		if(m_pRec->PeekCancel())IdHTTP->Disconnect();
	}
}


void __fastcall TWebModule::IdHTTPStatus(TObject *ASender, const TIdStatus AStatus, const AnsiString AStatusText)
{
	if(m_pRec)m_pRec->SetStatus(ToWide(AStatusText).w_str());
}
//---------------------------------------------------------------------------
void TWebModule::ConfigureConnection()
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

bool TWebModule::DoLogin(String szLogin,String szPass,bool free)
{
	SetState("Login...");

	String Login = dcmapURLEncodeW(szLogin.w_str(),CP_UTF8);
	String Pass = dcmapURLEncodeW(szPass.w_str(),CP_UTF8);

	if(!CurrentGameURL.IsEmpty())DoLogout();

	if(free) CurrentGameURL = FreeGameURL;
	else CurrentGameURL = GameURL;

	String Url = CurrentGameURL+L"/overview/";
	String Post = (String)L"action=login&login="+Login+"&pwd="+Pass;
	//Post = TIdURI::URLEncode(Post);

	String Result;
	Result = WebPost(Url,Post,CP_UTF8);

	return true;
}

bool TWebModule::DoLoginPlayer(DCMapDataReference ref)
{
	IdCookieManager->CookieCollection->Clear();

    IDCMapPlayersDataStoragePtr m_pPlayers;
    if(!m_pPlayers.init(GetDCMapCore(),"players"))return false;
    if(!m_pPlayers->SelectID(ref))return false;
	m_LoginPlayer = ref;

	return DoLogin(m_pPlayers[DCMDB_PLAYER_LOGIN],m_pPlayers[DCMDB_PLAYER_PASSWORD]);
}

bool TWebModule::DoScriptExec()
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

bool TWebModule::DoLogout()
{
    SetState("Logout...");
	m_LoginPlayer.Invalidate();
	GameGet(L"/logoff/");
	CurrentGameURL="";
	return true;
}

void TWebModule::TestGetData()
{

}

String DCMAPAPI TWebModule::WebGet(String const& URL,int CodePage)
{
	if(m_iLock) return L"";
	++m_iLock;
	ConfigureConnection();

	TMemoryStream* ResultData = new TMemoryStream();
	String Url = WideToString(URL,CodePage);
	String Result;
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

String DCMAPAPI TWebModule::WebPost(String const& URL,String const& Post,int CodePage)
{
	if(m_iLock) return L"";
	++m_iLock;
	ConfigureConnection();

	String Url = WideToString(URL,CodePage);
	String Result;

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

String DCMAPAPI TWebModule::GameGet(String const& srcURL)
{
	String URL = srcURL;
	if(!URL.IsEmpty() && URL[1] != L'/') URL = String(L"/")+srcURL;
	URL = CurrentGameURL+URL;
	return WebGet(URL,CP_UTF8);
}

String DCMAPAPI TWebModule::GamePost(String const& srcURL,String const& Post)
{
	String URL = srcURL;
	if(!URL.IsEmpty() && URL[1] != L'/') URL = String(L"/")+srcURL;
	URL = CurrentGameURL+URL;
	return WebPost(URL,Post,CP_UTF8);
}

void TWebModule::SetState(String State)
{

}







