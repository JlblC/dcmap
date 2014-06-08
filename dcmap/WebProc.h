//---------------------------------------------------------------------------

#ifndef WebProcH
#define WebProcH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IdAntiFreeze.hpp>
#include <IdAntiFreezeBase.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdCompressorZLib.hpp>
#include <IdCookieManager.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdZLibCompressorBase.hpp>
#include "dcmapbase.h"
//---------------------------------------------------------------------------
class TWebModule : public TDataModule
{
__published:	// IDE-managed Components
	TIdCookieManager *IdCookieManager;
	TIdCompressorZLib *IdCompressorZLib1;
	TIdHTTP *IdHTTP;
	TIdAntiFreeze *IdAntiFreeze1;

	void __fastcall IdHTTPWorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax);
	void __fastcall IdHTTPWorkEnd(TObject *ASender, TWorkMode AWorkMode);
	void __fastcall IdHTTPStatus(TObject *ASender, const TIdStatus AStatus, const AnsiString AStatusText);
	void __fastcall IdHTTPWork(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCount);
public:		// User declarations
	__fastcall TWebModule(TComponent* Owner);

private:	// User declarations

	void DoCheckUpdates();
	UnicodeString GameURL;
	UnicodeString FreeGameURL;
	UnicodeString CurrentGameURL;

	void ConfigureConnection();
	DCMapDataReference m_LoginPlayer;
	int m_iLock;
public:

	int CheckUpdateURL(UnicodeString Url,UnicodeString &Src,UnicodeString &Ver);
	int CheckUpdate(UnicodeString &Url,UnicodeString &Ver);
	int DoUpdate(UnicodeString Url);
		// User declarations

	bool DoLoginPlayer(DCMapDataReference ref);
	bool DoLogin(UnicodeString szLogin,UnicodeString szPass,bool free=false);
	bool DoLogout();
	int DownloadFile(String Url,String FileName);
	int DownloadFiles(String Url,String Destination);

	__fastcall TWebProcessForm(TComponent* Owner);
    void CheckUpdates();
    void TestGetData();
    bool DoScriptExec();

	void SetState(String State);

	UnicodeString DCMAPAPI WebGet(UnicodeString const& URL,int CodePage);
	UnicodeString DCMAPAPI WebPost(UnicodeString const& URL,UnicodeString const& PostData,int CodePage);
	UnicodeString DCMAPAPI GameGet(UnicodeString const& URL);
	UnicodeString DCMAPAPI GamePost(UnicodeString const& URL,UnicodeString const& PostData);

	IDCMapProcessRecordReseiver *m_pRec;

};

class CWebProcService : public IDCMapWebProcess
{
public:
	CWebProcService();
	int   DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);;
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);

	dcmapWCSTR DCMAPAPI WebGet(dcmapWCSTR URL,IDCMapProcessRecordReseiver *pRec=0,int CodePage = CP_UTF8);
	dcmapWCSTR DCMAPAPI WebPost(dcmapWCSTR URL,dcmapWCSTR PostData,IDCMapProcessRecordReseiver *pRec=0,int CodePage = CP_UTF8);
	bool DCMAPAPI GameLogin(dcmapWCSTR Login,dcmapWCSTR Pass,bool free,IDCMapProcessRecordReseiver *pRec=0);
	bool DCMAPAPI GameLogout(IDCMapProcessRecordReseiver *pRec=0);
	dcmapWCSTR DCMAPAPI GameGet(dcmapWCSTR URL,IDCMapProcessRecordReseiver *pRec=0);
	dcmapWCSTR DCMAPAPI GamePost(dcmapWCSTR URL,dcmapWCSTR PostData,IDCMapProcessRecordReseiver *pRec=0);

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
};

//---------------------------------------------------------------------------
extern PACKAGE TWebModule *WebModule;
//---------------------------------------------------------------------------
#endif
