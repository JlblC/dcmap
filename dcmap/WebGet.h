//$$---- Form HDR ----
//---------------------------------------------------------------------------
#ifndef WebGetH
#define WebGetH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <InWinSock2.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdHTTP.hpp>

#include "ZLib.hpp"
#include "IdCompressorZLibEx.hpp"
#include "IdZLibCompressorBase.hpp"
#include <IdCookieManager.hpp>

//---------------------------------------------------------------------------
class TWebGetModule : public TDataModule
{
__published:	// IDE-managed Components
	TIdHTTP *IdHTTP;
	TIdCompressorZLibEx *IdCompressorZLibEx1;
	TIdCookieManager *IdCookieManager1;
private:	// User declarations
public:		// User declarations
	__fastcall TWebGetModule(TComponent* Owner);
    void CheckUpdates();
};
//---------------------------------------------------------------------------
extern PACKAGE TWebGetModule *WebGetModule;
//---------------------------------------------------------------------------
#endif
