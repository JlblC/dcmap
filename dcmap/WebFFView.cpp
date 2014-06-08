//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "WebFFView.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "GeckoBrowser"
#pragma link "sToolBar"
#pragma resource "*.dfm"
TWebFFViewFrame *WebFFViewFrame;

static CFramePanelWrapper<TWebFFViewFrame> sWrap("PView","WebFFView");

//---------------------------------------------------------------------------
__fastcall TWebFFViewFrame::TWebFFViewFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------

bool TWebFFViewFrame::OnInit()
{
	//Browser = new TMozillaBrowser(this);

	//Browser->Align = alClient;
	//((TOleControl*)Browser)->Parent = this;

	return true;
}

void TWebFFViewFrame::OnShow()
{
}

void __fastcall TWebFFViewFrame::ToolButton1Click(TObject *Sender)
{
	Browser->LoadURI(L"http://www.the-game.ru");
}
//---------------------------------------------------------------------------

