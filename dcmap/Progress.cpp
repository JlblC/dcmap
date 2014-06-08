//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Progress.h"
#include "Utils.h"
#include "BaseModule.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sGauge"
#pragma link "sMemo"
#pragma link "sSkinProvider"
#pragma link "sButton"
#pragma link "sLabel"
#pragma resource "*.dfm"
TProgressForm *ProgressForm=0;

#define MaxLines 128

#define DefCaption L"DCMap"



struct TProgressFidder : public IDCMapProcessRecordReseiver
{
	bool DCMAPAPI AddError(dcmapWCSTR Text,int text_line)
	{return AddRecord(Text,-1,text_line);}
	bool DCMAPAPI AddMessage(dcmapWCSTR Text,int text_line)
	{return AddRecord(Text,-1,text_line);}

	bool DCMAPAPI AddRecord(dcmapWCSTR Text,int id,int text_line)
	{
		TStringList* pList = new TStringList;
		pList->Text = Text;
		try
		{
			for(int i=0;i<pList->Count;i++)
			{
				ProgressForm->MessagesMemo->Lines->Add(pList->Strings[i]);
			}
		}
		catch(...){ProgressForm->MessagesMemo->Lines->Clear();}
		delete pList;

		while( ProgressForm->MessagesMemo->Lines->Count >= MaxLines )
		{
			 ProgressForm->MessagesMemo->Lines->Delete(0);
        }
		return true;
	}
	void DCMAPAPI ClearMessages()
	{
		ProgressForm->MessagesMemo->Lines->Clear();
	};

	void DCMAPAPI SetupProgress(float max,float cur=0,dcmapWCSTR header=0)
	{
		ProgressForm->ProgressBar->MinValue = 0;
		ProgressForm->ProgressBar->MaxValue = max;
		ProgressForm->ProgressBar->Progress = cur;

		ProgressForm->ProgressBar->Visible =
			(ProgressForm->ProgressBar->Progress > ProgressForm->ProgressBar->MinValue &&
			ProgressForm->ProgressBar->Progress < ProgressForm->ProgressBar->MaxValue);
		ProgressForm->StatusLabel->Visible = !ProgressForm->ProgressBar->Visible;

		if(header)ProgressForm->Caption = header;
	}

	void DCMAPAPI UpdateProgress(float pos,dcmapWCSTR header=0)
	{
		ProgressForm->ProgressBar->Progress = pos;
		
		ProgressForm->ProgressBar->Visible =
			(ProgressForm->ProgressBar->Progress > ProgressForm->ProgressBar->MinValue &&
			ProgressForm->ProgressBar->Progress < ProgressForm->ProgressBar->MaxValue);
		ProgressForm->StatusLabel->Visible = !ProgressForm->ProgressBar->Visible;

		if(header)ProgressForm->Caption = header;
	}

	void DCMAPAPI AdvanceProgress(float adv,dcmapWCSTR header=0)
	{
		ProgressForm->ProgressBar->Progress += adv;

		ProgressForm->ProgressBar->Visible =
			(ProgressForm->ProgressBar->Progress > ProgressForm->ProgressBar->MinValue &&
			ProgressForm->ProgressBar->Progress < ProgressForm->ProgressBar->MaxValue);
		ProgressForm->StatusLabel->Visible = !ProgressForm->ProgressBar->Visible;

		if(header)ProgressForm->Caption = header;
	}

	void DCMAPAPI Show()
	{
		if(!ProgressForm->Visible)
		{
			ClearMessages();
			SetupProgress(0);
			ProgressForm->StatusLabel->Caption="";
			ProgressForm->Caption = DefCaption;
			ProgressForm->Show();
		}
		ProgressForm->Canceling = false;
		//MainForm->Enabled =false;
	};
	void DCMAPAPI Hide()
	{
		ProgressForm->Hide();
		//MainForm->Enabled =true;
		//MainForm->FocusControl(MainForm->ClientPanel);
	};

	void DCMAPAPI SetHeader(dcmapWCSTR header)
	{
		if(header && header[0])ProgressForm->Caption = header;
		else ProgressForm->Caption = DefCaption;
	};

	void DCMAPAPI SetStatus(dcmapWCSTR status)
	{
		if(status)ProgressForm->StatusLabel->Caption = status;
		else ProgressForm->StatusLabel->Caption = "";
	};

	bool DCMAPAPI TestCancel()
	{
		Application->ProcessMessages();

		bool c = ProgressForm->Canceling;
		ProgressForm->Canceling = false;
		return c;
	};
	bool DCMAPAPI PeekCancel()
	{
		Application->ProcessMessages();
		return ProgressForm->Canceling;
	};

	dcmapINT DCMAPAPI Pause(dcmapWCSTR status, dcmapINT Options)
	{
		ProgressForm->GoOn = false;
		ProgressForm->Canceling = false;
		ProgressForm->Retry = false;

		ProgressForm->ProgressBar->Visible = false;
		ProgressForm->StatusLabel->Visible = false;
		ProgressForm->RetryBtn->Visible = false;
		ProgressForm->ContinueBtn->Visible = true;

		if(Options)
		{
			ProgressForm->ContinueBtn->Visible = Options & DCM_PAUSE_CONTINUE;
			ProgressForm->RetryBtn->Visible = Options & DCM_PAUSE_RETRY;
		}

		if(status)
		{
			ProgressForm->StatusLabel->Caption = status;
			ProgressForm->StatusLabel->Visible = true;
		}
		Application->ProcessMessages();
		int r = true;
		while(1)
		{
			Application->ProcessMessages();
			if(ProgressForm->Canceling)
			{
				if(Options & DCM_PAUSE_CANCEL) r = DCM_PAUSE_CANCEL;
				else if(Options & DCM_PAUSE_QUIT) r = DCM_PAUSE_QUIT;
				else r = false;
				break;
			}
			if(ProgressForm->GoOn)
			{
				if(Options & DCM_PAUSE_CONTINUE) r = DCM_PAUSE_CONTINUE;
				else r = true;
				break;
			}
			if(ProgressForm->Retry)
			{
				if(Options & DCM_PAUSE_RETRY) r = DCM_PAUSE_RETRY;
				else r = true;
				break;
			}
			Sleep(50);
		}
		ProgressForm->ContinueBtn->Visible = false;
		ProgressForm->StatusLabel->Visible = false;
		ProgressForm->RetryBtn->Visible = false;
		Application->ProcessMessages();
		return r;
	};
};
static TProgressFidder s_Fidder;


//---------------------------------------------------------------------------
__fastcall TProgressForm::TProgressForm(TComponent* Owner)
	: Inherited(Owner)
{
	Canceling = false;
	GoOn = false;
}

IDCMapProcessRecordReseiver *TProgressForm::Fidder()
{
	return &s_Fidder;
}
//---------------------------------------------------------------------------
void __fastcall TProgressForm::CancelBtnClick(TObject *Sender)
{
	Canceling = true;
}
//---------------------------------------------------------------------------


void __fastcall TProgressForm::ContinueBtnClick(TObject *Sender)
{
	GoOn = true;
}
//---------------------------------------------------------------------------


void __fastcall TProgressForm::RetryBtnClick(TObject *Sender)
{
	Retry = true;
}
//---------------------------------------------------------------------------

void __fastcall TProgressForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	 Action = caHide;
	 Canceling = true;
}
//---------------------------------------------------------------------------



