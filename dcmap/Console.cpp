//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Console.h"
#include "Utils.h"
#include "main.h"
#include "Interface.h"
#include "BaseModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sMemo"
#pragma link "sSplitter"
#pragma link "sPanel"
#pragma link "sSkinProvider"
#pragma link "sButton"
#pragma link "SynEdit"
#pragma link "SynEditHighlighter"
#pragma link "SynHighlighterGeneral"
#pragma link "SynCompletionProposal"
#pragma link "BaseModule"
#pragma link "ScriptEditModule"
#pragma resource "*.dfm"
TConsoleForm *ConsoleForm=0;

#define MaxLines (1024*4)

static CAutoReg<CBasicFormService<TConsoleForm> > sWrap("PCoreService","ConsoleForm");


struct TConsoleFidder : public IDCMapProcessRecordReseiver
{
	bool DCMAPAPI AddError(dcmapWCSTR Text,int text_line)
	{return AddRecord(Text,-1,text_line);}
	bool DCMAPAPI AddMessage(dcmapWCSTR Text,int text_line)
	{return AddRecord(Text,-1,text_line);}

	bool DCMAPAPI AddRecord(dcmapWCSTR Text,int id,int text_line)
	{
		if(!ConsoleForm)
		{
			GetDCMapCore()->GetService("ConsoleForm")->ScriptExec(1,L"Show",0,0);
			if(!ConsoleForm)return false;
		}

		TStringList* pList = new TStringList;
		pList->Text = Text;
		try
		{
			for(int i=0;i<pList->Count;i++)
			{
				ConsoleForm->ResultMemo->Lines->Add(pList->Strings[i]);
			}
		}
		catch(...){ConsoleForm->ResultMemo->Lines->Clear();}
		delete pList;

		while( ConsoleForm->ResultMemo->Lines->Count >= MaxLines )
		{
			 ConsoleForm->ResultMemo->Lines->Delete(0);
        }
		return true;
    }
};
static TConsoleFidder s_Fidder;


//---------------------------------------------------------------------------
__fastcall TConsoleForm::TConsoleForm(TComponent* Owner)
	: TBaseViewPopupForm(Owner)
{
	ConsoleForm = this;
}
//---------------------------------------------------------------------------
void __fastcall TConsoleForm::FormCreate(TObject *Sender)
{
	m_pConsole= GetDCMapCore()->GetConsole();
	ScriptEditFrame->SetupEdit(L".dcm");
}
//---------------------------------------------------------------------------
void __fastcall TConsoleForm::ExecuteButtonClick(TObject *Sender)
{
	UnicodeString Text =  ScriptEditFrame->Edit->Text;

	m_pConsole->Execute(Text.w_str(),&s_Fidder);
}
//---------------------------------------------------------------------------
void __fastcall TConsoleForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    SaveFormPlacement(this);	
}
//---------------------------------------------------------------------------
void __fastcall TConsoleForm::FormDestroy(TObject *Sender)
{
	ConsoleForm=0;	
}
//---------------------------------------------------------------------------

void __fastcall TConsoleForm::SourceMemoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Shift.Contains(ssCtrl) && Key == '\r')
    {
        ExecuteButtonClick(Sender);
        Key=0;
    }
}
//---------------------------------------------------------------------------

void __fastcall TConsoleForm::TntFormShow(TObject *Sender)
{
    LoadFormPlacement(this);	
}
//---------------------------------------------------------------------------

















