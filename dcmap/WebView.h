//---------------------------------------------------------------------------

#ifndef WebViewH
#define WebViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sComboBox.hpp"
#include "sToolBar.hpp"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <OleCtrls.hpp>
#include <WebBase.hpp>


//---------------------------------------------------------------------------
class TWebViewFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	void __fastcall OnLoginMenuCLick(TObject *Sender);
private:	// User declarations
	TWebBaseFrame* Browser;
	IDCMapPlayersDataStoragePtr m_pPlayers;

	void UpdateLoginMenu();
public:		// User declarations
	virtual bool OnInit();
	virtual void OnShow();
	int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	__fastcall TWebViewFrame(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TWebViewFrame *WebViewFrame;
//---------------------------------------------------------------------------
#endif
