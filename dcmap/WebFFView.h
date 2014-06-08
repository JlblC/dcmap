//---------------------------------------------------------------------------

#ifndef WebFFViewH
#define WebFFViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "GeckoBrowser.hpp"
#include "sToolBar.hpp"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//#include "MOZILLACONTROLLib_OCX.h"

//---------------------------------------------------------------------------
class TWebFFViewFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TGeckoBrowser *Browser;
	TsToolBar *sToolBar1;
	TToolButton *ToolButton1;
	void __fastcall ToolButton1Click(TObject *Sender);
private:	// User declarations
	bool OnInit();
	void OnShow();

public:		// User declarations
	//TMozillaBrowser* Browser;


	__fastcall TWebFFViewFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TWebFFViewFrame *WebFFViewFrame;
//---------------------------------------------------------------------------
#endif
