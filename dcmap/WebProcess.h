//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef WebProcessH
#define WebProcessH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <dcmapsyslib.h>

//#include <InWinSock2.hpp>
//#include <IdWinSock2.hpp>

#include "sSkinProvider.hpp"
#include "sGauge.hpp"
#include "sLabel.hpp"
#include "sButton.hpp"
//#include "TntForms.hpp"
#include <ExtCtrls.hpp>
#include "IdCompressorZLib.hpp"
#include "IdZLibCompressorBase.hpp"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdCookieManager.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerStream.hpp>
#include <IdThreadComponent.hpp>
#include <IdAntiFreeze.hpp>
#include <IdAntiFreezeBase.hpp>


//---------------------------------------------------------------------------
class TWebProcessForm : public TForm
{
__published:	// IDE-managed Components
	TIdHTTP *IdHTTP;
	TIdCookieManager *IdCookieManager;
	TsSkinProvider *sSkinProvider;
	TsGauge *Gauge;
	TsLabel *StatusLabel;
	TsLabel *OperationLabel;
	TsButton *CanselBtn;
	TIdCompressorZLib *IdCompressorZLib1;
	TIdAntiFreeze *IdAntiFreeze1;
	void __fastcall IdHTTPWorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax);
	void __fastcall IdHTTPWorkEnd(TObject *ASender, TWorkMode AWorkMode);
	void __fastcall IdHTTPStatus(TObject *ASender, const TIdStatus AStatus, const AnsiString AStatusText);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall IdHTTPWork(TObject *ASender, TWorkMode AWorkMode,
          __int64 AWorkCount);
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
//---------------------------------------------------------------------------
extern PACKAGE TWebProcessForm *WebProcessForm;
//---------------------------------------------------------------------------
#endif
