//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <string>

#include "DataImport.h"
#include "Utils.h"
#include "BaseModule.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sMemo"
#pragma link "sPanel"
#pragma link "sSkinProvider"
#pragma link "sSplitter"
#pragma link "sAlphaListBox"
#pragma link "sCheckListBox"
#pragma link "sCustomComboEdit"
#pragma link "sMaskEdit"
#pragma link "sRadioButton"
#pragma link "sTooledit"
#pragma link "sEdit"
#pragma link "sButton"
#pragma link "sComboBox"
#pragma link "sLabel"
#pragma link "sPageControl"
#pragma link "sGauge"
#pragma link "sSpeedButton"
#pragma link "acProgressBar"
#pragma link "BaseModule"
#pragma link "ScriptEditModule"
#pragma resource "*.dfm"
TDataImportForm *DataImportForm=0;
//---------------------------------------------------------------------------
static CAutoReg<CBasicFormService<TDataImportForm> > sWrap("PCoreService","DataImportForm");

struct TRecordData : public TObject
{
    int id;
    int line;
};

struct TDataFidder : public IDCMapProcessRecordReseiver
{
	bool DCMAPAPI AddRecord(dcmapWCSTR Text,int id,int text_line)
    {
        TRecordData* dta = new TRecordData;
        dta->id = id;
        dta->line = text_line;
		DataImportForm->RecordList->Items->AddObject(Text,dta);
        return true;
    }
	bool DCMAPAPI AddError(dcmapWCSTR Text,int text_line=-1)
    {
        TRecordData* dta = new TRecordData;
        dta->line = text_line;
		DataImportForm->ErrorsList->Items->AddObject(Text,dta);
		DataImportForm->Pages->ActivePage = DataImportForm->ErrorsTab;
        return true;
	}
	bool DCMAPAPI AddMessage(dcmapWCSTR Text,int text_line=-1)
    {
		DataImportForm->MessagesMemo->Lines->Add(Text);
        return true;
	}

	void DCMAPAPI SetupProgress(float max,float cur=0,dcmapWCSTR header=0)
	{
		DataImportForm->Progress->Visible = true;
		DataImportForm->Progress->MaxValue = max;
		DataImportForm->Progress->Progress = cur;

	}
	void DCMAPAPI UpdateProgress(float pos,dcmapWCSTR header=0)
	{
		DataImportForm->Progress->Progress = pos;
	}
	void DCMAPAPI AdvanceProgress(float adv,dcmapWCSTR header=0)
	{
		DataImportForm->Progress->Progress += adv;
	}
};
static TDataFidder s_Fidder;

__fastcall TDataImportForm::TDataImportForm(TComponent* Owner)
    : TBaseViewPopupForm(Owner)
{
    m_pModule=0;
	m_bParamGood=false;
	DataImportForm=this;
}
//---------------------------------------------------------------------------
void __fastcall TDataImportForm::ReadButtonClick(TObject *Sender)
{
	Screen->Cursor = crHourGlass;

	Source=L"";
	ScriptEditFrame->ClearLines();
	if(FileRadio->Checked)
    {
        FilenameEdit->Enabled=true;
        CodepageComboBox->Enabled=true;
		if( FileExists(FilenameEdit->Text))
		{
			AnsiString str;

			int iFileHandle = FileOpen(FilenameEdit->Text, fmOpenRead);
			int iFileLength = FileSeek(iFileHandle,0,2);
			FileSeek(iFileHandle,0,0);
			char* pszBuffer = new char[iFileLength+1];
			FileRead(iFileHandle, pszBuffer, iFileLength);
			FileClose(iFileHandle);
			pszBuffer[iFileLength]=0;

			int Codepage = 1251;
			if(CodepageComboBox->ItemIndex == 1)Codepage = 866;
			else if(CodepageComboBox->ItemIndex == 2)Codepage = CP_UTF8;

			Source = ToWide(pszBuffer,iFileLength,Codepage);
			delete[] pszBuffer;

			ScriptEditFrame->SetupEdit(FilenameEdit->Text);
		}
	}
	else if(ClipboardButton->Checked)
	{
		FilenameEdit->Enabled=false;
		CodepageComboBox->Enabled=false;
		Source = Clipboard()->AsText;
		ScriptEditFrame->SetupEdit(L"");
	}

	if(m_pModule)
	{
		dcmapWCSTR Formated = m_pModule->ReformatInput(Source.w_str());
		if(Formated)ScriptEditFrame->SetText(Formated);
		else ScriptEditFrame->SetText(Source);
	}
	else ScriptEditFrame->SetText(Source);

	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TDataImportForm::FormCreate(TObject *Sender)
{
	InterfaceModule->LocalizeContainer(this);


	ScriptEditFrame->Readonly = true;

	IDCMapSys* pSys = GetDCMapSys();

	PluginList_LoadDataTables(TableCombo->Items);

	pSys->GetClassPluginNames("PTextImport",ModuleList->Items);

	ModuleList->ItemIndex=0;
	TableCombo->ItemIndex=0;
	LoadConfig();
	InitModule();
}
//---------------------------------------------------------------------------
void __fastcall TDataImportForm::ProcessButtonClick(TObject *Sender)
{
	ClearMessages();
	ClearRecords();

    if(!m_pModule)InitModule();
    if(!m_pModule)return;

    Enabled = false;
    Screen->Cursor = crHourGlass;

	if(m_pModule->SetTextParam(ToWide(ParamEdit->Text).w_str()))
    {
        m_bParamGood = true;
    }
    DWORD opt=0;
    if(ReplaceRadio->Enabled && ReplaceRadio->Checked)
    	opt |= DCMAP_IMPORT_REPLACE;

	int iLines = ScriptEditFrame->LineCount();
//	RecordList->Items->Capacity = iLines;

	RecordList->Sorted = (m_ModuleParams & DCMAP_IMPORT_SORT_RECORDS);

	m_pModule->PreProcessData(Source.w_str(),iLines,&s_Fidder,opt);

	for(int i=0;i<RecordList->Items->Count;i++)
	{
		RecordList->Checked[i] = true;
	}

    if(RecordList->Items->Count)
    {
    	Pages->ActivePage = RecordsTab;
    }
    else if(MessagesMemo->Lines->Count)
    {
    	Pages->ActivePage = MessagesTab;
    }
    else if(ErrorsList->Items->Count)
    {
    	Pages->ActivePage = ErrorsTab;
    }

    Progress->Visible = false;

    Screen->Cursor = crDefault;
    Enabled = true;
}
void TDataImportForm::ClearMessages()
{
	MessagesMemo->Clear();

    for(int i=0;i<ErrorsList->Items->Count;i++)
    {
       delete ErrorsList->Items->Objects[i];
    }
    ErrorsList->Clear();
}

void TDataImportForm::ClearRecords()
{
    for(int i=0;i<RecordList->Items->Count;i++)
    {
       delete RecordList->Items->Objects[i];
    }
	RecordList->Clear();
}

//---------------------------------------------------------------------------
void __fastcall TDataImportForm::ImportButtonClick(TObject *Sender)
{
	if(!m_pModule)return;
    if(RecordList->Items->Count==0)return;

    int iCount=0;
    for(int i=0;i<RecordList->Items->Count;i++)
        if(RecordList->Checked[i])iCount++;

	Progress->MaxValue = iCount;
	Progress->Progress = 0;
    Progress->Visible = true;

    Enabled = false;
	Screen->Cursor = crHourGlass;
	Pages->ActivePage = MessagesTab;
	ClearMessages();

    if(!m_pModule->BeginExecute())return;
    try
    {
        for(int i=0;i<RecordList->Items->Count;i++)
        {
            if(!RecordList->Checked[i])continue;

            TRecordData* dta = (TRecordData*)RecordList->Items->Objects[i];
			m_pModule->ExecuteRecord(ToWide(RecordList->Items->Strings[i]).w_str(),
									 dta->id);

            Progress->Progress++;
        }
    }
    catch(...)
    {
        m_bParamGood = false;
    }
    Screen->Cursor = crDefault;
    Enabled = true;

    m_pModule->EndExecute();
	ClearRecords();

    Progress->Progress = 100;
	Progress->Visible = false;

	if(ErrorsList->Items->Count)
	{
		Pages->ActivePage = ErrorsTab;
	}
	else if(MessagesMemo->Lines->Count)
	{
		Pages->ActivePage = MessagesTab;
	}


    if(m_bParamGood)
    {
    	SaveParamClick(Sender);
    }
}
//---------------------------------------------------------------------------

void __fastcall TDataImportForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    SaveModuleConfig();
    SaveConfig();
    ClearRecords();    
}
//---------------------------------------------------------------------------


void __fastcall TDataImportForm::FilenameEditButtonClick(TObject *Sender)
{
    //    
}
void TDataImportForm::SaveConfig()
{
    DCMapConfigWrapper  cfg(GetDCMapCore()->GetConfig());
    cfg.section("DataImport");

    cfg["Module"] << ModuleList->Text;
    cfg["Table"]  << TableCombo->Text;

    cfg["Codepage"]  << CodepageComboBox->ItemIndex;

	cfg["SourceLinesH"]  << ScriptEditFrame->Height;

    SaveFormPlacement(this);
}

void TDataImportForm::LoadConfig()
{
    DCMapConfigWrapper  cfg(GetDCMapCore()->GetConfig());
    cfg.section("DataImport");
    String val;

    if(cfg["Module"] >> val && !val.IsEmpty())
    	ModuleList->ItemIndex = ModuleList->Items->IndexOf(val);
    if(cfg["Table"]  >> val && !val.IsEmpty())
    	TableCombo->ItemIndex = TableCombo->Items->IndexOf(val);

	int ival = ScriptEditFrame->Height;
    cfg["SourceLinesH"] >> ival;
	ScriptEditFrame->Height = ival;

    ival = 0;
    cfg["Codepage"] >> ival;
    CodepageComboBox->ItemIndex=ival;
}


//---------------------------------------------------------------------------
void TDataImportForm::SaveModuleConfig()
{
    if(!m_pModule)return;

    DCMapConfigWrapper  cfg(GetDCMapCore()->GetConfig());
    cfg.section(ParamSection);

    cfg["Params"] << ParamEdit->Items->Count;
    for(int i=0;i<ParamEdit->Items->Count;i++)
    {
       cfg["Param"+IntToStr(i)] << ParamEdit->Items->Strings[i];
    }
}
void TDataImportForm::LoadModuleConfig()
{
    if(!m_pModule)return;

    DCMapConfigWrapper  cfg(GetDCMapCore()->GetConfig());
    cfg.section(ParamSection);

    ParamEdit->Items->Clear();
    int NumParams=0;
    cfg["Params"] >> NumParams;
    for(int i=0;i<NumParams;i++)
    {
       String Param;
       if(cfg["Param"+IntToStr(i)] >> Param)
       {
        	Param = Trim(Param);
            if(!Param.IsEmpty())
            	ParamEdit->Items->Add(Param);
       }
    }
}

bool TDataImportForm::InitModule()
{
	if(m_pModule)
	{
		if(CurrentModule == ModuleList->Text)return true;

		SaveModuleConfig();
        GetDCMapCore()->DestroyModule(m_pModule);
        m_pModule=0;
	}

	CurrentModule = ModuleList->Text;

    m_pModule = GetDCMapCore()->CreateModuleTyped<IDCMapImportProcessor>
								("PTextImport",CurrentModule.c_str());
	if(!m_pModule)return false;

	Screen->Cursor = crHourGlass;

	m_ModuleParams = m_pModule->GetImportParams();
	if(m_ModuleParams & DCMAP_IMPORT_UNIVERSAL)
    {
    	TableCombo->Enabled = true;
        m_pModule->SetDataSourceName(WideToString(TableCombo->Text).c_str());
    }
    else  TableCombo->Enabled = false;

    if(m_ModuleParams & DCMAP_IMPORT_ALLOW_REPLACE)
    {
        AppentRadio->Enabled = true;
        ReplaceRadio->Enabled = true;
    }
    else
    {
        AppentRadio->Enabled = false;
        ReplaceRadio->Enabled = false;
    }

    String Fltr = m_pModule->GetFileFilters();

    if(!Fltr.IsEmpty())FilenameEdit->Filter = Fltr+"|Все файлы (*.*)|*.*";
    else FilenameEdit->Filter="Все файлы (*.*)|*.*";

    ParamSection = m_pModule->GetConfigSectionName();

	LoadModuleConfig();

	dcmapWCSTR Formated = m_pModule->ReformatInput(Source.w_str());
	if(Formated)ScriptEditFrame->SetText(Formated);
	else ScriptEditFrame->SetText(Source);

	Screen->Cursor = crDefault;
	return true;
}

//---------------------------------------------------------------------------

void __fastcall TDataImportForm::ModuleListChange(TObject *Sender)
{
    InitModule();
}
//---------------------------------------------------------------------------

void __fastcall TDataImportForm::RecordListClick(TObject *Sender)
{
    if(RecordList->ItemIndex<0)return;
    TRecordData* dta = (TRecordData*)RecordList->Items->Objects[RecordList->ItemIndex];
    if(dta->line >=0)
	{
		ScriptEditFrame->HighlightLine(dta->line);
		//SourceLines->Refresh();
		//SourceLines->RefreshScrolls();
    }
}
//---------------------------------------------------------------------------

void __fastcall TDataImportForm::ErrorsListClick(TObject *Sender)
{
    if(ErrorsList->ItemIndex<0)return;
    TRecordData* dta = (TRecordData*)ErrorsList->Items->Objects[ErrorsList->ItemIndex];
    if(dta->line >=0)
    {
		ScriptEditFrame->HighlightLine(dta->line);
		//SourceLines->Refresh();
        //SourceLines->RefreshScrolls();
    }
}
//---------------------------------------------------------------------------

void __fastcall TDataImportForm::TableComboChange(TObject *Sender)
{
    if(!m_pModule)return;
    SaveModuleConfig();
    m_pModule->SetDataSourceName(WideToString(TableCombo->Text).c_str());
    ParamSection = m_pModule->GetConfigSectionName();
    LoadModuleConfig();
}
//---------------------------------------------------------------------------

void __fastcall TDataImportForm::SaveParamClick(TObject *Sender)
{
    String str = Trim(ParamEdit->Text);
    if(!str.IsEmpty() &&
        ParamEdit->Items->IndexOf(str)<0)
    {
        ParamEdit->Items->Add(str);
    }
}
//---------------------------------------------------------------------------

void __fastcall TDataImportForm::DeleteParamClick(TObject *Sender)
{
    String str = (ParamEdit->Text);
    int ind = ParamEdit->Items->IndexOf(str);
    if(ind >=0)
    {
        ParamEdit->Items->Delete(ind);
    }
    ParamEdit->Text = "";	
}
//---------------------------------------------------------------------------

void __fastcall TDataImportForm::FormShow(TObject *Sender)
{
	LoadFormPlacement(this);
}
//-------------------------------------------------------------------
