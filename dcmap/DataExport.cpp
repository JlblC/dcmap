//$$---- Form CPP ----
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Utils.h"
#include "DataExport.h"
#include "BaseModule.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma link "sMemo"
#pragma link "sPanel"
#pragma link "sComboBox"
#pragma link "sButton"
#pragma link "sGauge"
#pragma link "sSpeedButton"
#pragma link "sDialogs"
#pragma link "sComboEdit"
#pragma link "sCustomComboEdit"
#pragma link "sMaskEdit"
#pragma link "sMaskEdit"
#pragma resource "*.dfm"
TDataExportForm *DataExportForm=0;

static CAutoReg<CBasicFormService<TDataExportForm> > sWrap("PCoreService","DataExportForm");


struct TExpDataFidder : public IDCMapProcessRecordReseiver
{
	bool DCMAPAPI AddRecord(dcmapWCSTR Text,int id,int text_line)
	{
		UnicodeString str = Trim((UnicodeString)Text);
		if(str.IsEmpty())return false;
		DataExportForm->Accum += str + L"\r\n";
		return true;
    }

	bool DCMAPAPI AddError(dcmapWCSTR Text,int text_line=-1)
	{
		return true;
	}
	bool DCMAPAPI AddMessage(dcmapWCSTR Text,int text_line=-1)
	{
		return true;
	}

	void DCMAPAPI SetupProgress(float max,float cur=0,dcmapWCSTR header=0)
	{
		DataExportForm->Gauge->Visible = true;
		DataExportForm->Gauge->MaxValue = max;
		DataExportForm->Gauge->Progress = cur;
	}
	void DCMAPAPI UpdateProgress(float pos,dcmapWCSTR header=0)
	{
		DataExportForm->Gauge->Progress = pos;
	}
	void DCMAPAPI AdvanceProgress(float adv,dcmapWCSTR header=0)
	{
		DataExportForm->Gauge->Progress += adv;
	}
};
static TExpDataFidder s_Fidder;

//---------------------------------------------------------------------------
__fastcall TDataExportForm::TDataExportForm(TComponent* Owner)
	: TBaseViewPopupForm(Owner)
{
	m_pModule=0;
	DataExportForm=this;
}
//---------------------------------------------------------------------------
void __fastcall TDataExportForm::FormCreate(TObject *Sender)
{
    InterfaceModule->LocalizeContainer(this);

    IDCMapSys* pSys = GetDCMapSys();

    PluginList_LoadDataTables(TableCombo->Items);
    PluginList_LoadClass("PTextExport",ModuleList->Items);

    ModuleList->ItemIndex=0;
    TableCombo->ItemIndex=0;
    LoadConfig();
    InitModule();
}
//---------------------------------------------------------------------------

void TDataExportForm::SaveConfig()
{
    DCMapConfigWrapper  cfg(GetDCMapCore()->GetConfig());
    cfg.section("DataExport");

    cfg["Module"] << PluginList_At(ModuleList->Items,ModuleList->ItemIndex);
    cfg["Table"]  << PluginList_At(TableCombo->Items,TableCombo->ItemIndex);

    SaveFormPlacement(this);
}

void TDataExportForm::LoadConfig()
{
    DCMapConfigWrapper  cfg(GetDCMapCore()->GetConfig());
    cfg.section("DataExport");
    String val;

    if(cfg["Module"] >> val && !val.IsEmpty())
    	ModuleList->ItemIndex = PluginList_IndexOf(ModuleList->Items,val,0);
    if(cfg["Table"]  >> val && !val.IsEmpty())
    	TableCombo->ItemIndex = PluginList_IndexOf(TableCombo->Items,val,0);

}

void TDataExportForm::SaveModuleConfig()
{
    if(!m_pModule)return;

    DCMapConfigWrapper  cfg(GetDCMapCore()->GetConfig());
    cfg.section(ParamSection);

    cfg["Params"] << ParamEdit->Items->Count;
    for(int i=0;i<ParamEdit->Items->Count;i++)
    {
       cfg["Param"+IntToStr(i)] << ParamEdit->Items->Strings[i];
    }
    cfg["LastParam"] << ParamEdit->ItemIndex;
	cfg["ParamText"] << ParamEdit->Text;

	AnsiString FilterName = PluginList_At(FilterCombo->Items,FilterCombo->ItemIndex);
	if(FilterName.IsEmpty()) FilterName = IntToStr(FilterCombo->ItemIndex);
	cfg["Filter"] << FilterName;

	cfg["FilterExpression"] << ExpressionEdit->Text;

	cfg["Codepage"] << CodepageComboBox->Text;
}

void TDataExportForm::LoadModuleConfig()
{
    if(!m_pModule)return;

    DCMapConfigWrapper  cfg(GetDCMapCore()->GetConfig());
    cfg.section(ParamSection);

    ParamEdit->Items->Clear();
    ParamEdit->Text = "";
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
    int v=-1;
    cfg["LastParam"] >> v;
    ParamEdit->ItemIndex = v;
    if(v<0)
    {
        String str;
    	cfg["ParamText"] >> str;
        ParamEdit->Text = str;
    }

	PluginList_Clear(FilterCombo->Items);
	FilterCombo->Items->Add(g_pLocalStrings->GetStringW("NoDBItem"));
	FilterCombo->Items->Add(g_pLocalStrings->GetStringW("ExpressionItem"));

	AnsiString FilterClass = "PTableFilter_"+AnsiString(TableCombo->Text);

	PluginList_LoadClass(FilterClass,FilterCombo->Items,true);
    FilterCombo->Enabled = FilterCombo->Items->Count>1;

	String Filter;
	cfg["Filter"] >> Filter;
	int Index = PluginList_IndexOf(FilterCombo->Items,Filter,0);
	if(!Index)try{Index = StrToInt(Filter);}catch(...){}
	FilterCombo->ItemIndex = Index;
	
	ExpressionEdit->Visible = Index == 1;

	Filter= "";
	cfg["FilterExpression"] >> Filter;
	ExpressionEdit->Text = Filter;

	String Codepage;
	cfg["Codepage"] >> Codepage;
	CodepageComboBox->ItemIndex = CodepageComboBox->Items->IndexOf(Codepage);
	if(CodepageComboBox->ItemIndex<0)CodepageComboBox->ItemIndex=0;
}

bool TDataExportForm::InitModule()
{
    if(m_pModule)
    {
        SaveModuleConfig();
        GetDCMapCore()->DestroyModule(m_pModule);
        m_pModule=0;
    }

    String Module = PluginList_At(ModuleList->Items,ModuleList->ItemIndex);

    m_pModule = GetDCMapCore()->CreateModuleTyped<IDCMapExportProcessor>("PTextExport",Module);
    if(!m_pModule)return false;

    int params = m_pModule->GetExportParams();
    if(params & DCMAP_EXPORT_UNIVERSAL)
    {
    	TableCombo->Enabled = true;
        m_pModule->SetDataSourceName(PluginList_At(TableCombo->Items,TableCombo->ItemIndex).c_str());
    }
    else  TableCombo->Enabled = false;

    ParamSection = m_pModule->GetConfigSectionName();

    LoadModuleConfig();
    return true;
}
void __fastcall TDataExportForm::ModuleListChange(TObject *Sender)
{
	InitModule();
}


//---------------------------------------------------------------------------
void __fastcall TDataExportForm::TableComboChange(TObject *Sender)
{
    if(!m_pModule)return;
    SaveModuleConfig();
	m_pModule->SetDataSourceName(WideToString(TableCombo->Text).c_str());
    ParamSection = m_pModule->GetConfigSectionName();
    LoadModuleConfig();
}
//---------------------------------------------------------------------------
void __fastcall TDataExportForm::ProcessButtonClick(TObject *Sender)
{
	if(!m_pModule)InitModule();
    if(!m_pModule)return;

    String Filter = PluginList_At(FilterCombo->Items,FilterCombo->ItemIndex);
    IDCMapFilteredDataSource* pDS=0;
    if(!Filter.IsEmpty())
    {
        String FilterClass = ("PTableFilter_"+TableCombo->Text);
        pDS = GetDCMapCore()->CreateModuleTyped<IDCMapFilteredDataSource>(FilterClass,Filter);
        if(pDS)
        {
			 pDS->SetDataSourceName(WideToString(TableCombo->Text).c_str());
             if(!pDS->Process(&s_Fidder,0))
             {
                  GetDCMapCore()->DestroyModule(pDS);
                  pDS=0;
             }
        }
    }
	Accum=L"";
	OutMemo->Clear();

	m_pModule->SetDataSource(pDS);

	if(ExpressionEdit->Visible)
		m_pModule->SetFilterExpr(Trim(ExpressionEdit->Text).w_str());
	else m_pModule->SetFilterExpr(0);


    Enabled = false;
    Screen->Cursor = crHourGlass;
    if(m_pModule->SetTextParam(ToWide(ParamEdit->Text).w_str()))
    {
        m_bParamGood = true;
    }
    DWORD opt=0;

	m_pModule->Export(&s_Fidder,opt);

	OutMemo->Text = Accum;

    Gauge->Visible = false;

    Screen->Cursor = crDefault;
    Enabled = true;

    if(pDS)GetDCMapCore()->DestroyModule(pDS);
}
//---------------------------------------------------------------------------
void __fastcall TDataExportForm::SaveButtonClick(TObject *Sender)
{
	if(!m_pModule)return;
	if(!OutMemo->Lines->Count)return;

	int Codepage = 1251;
	if(CodepageComboBox->ItemIndex == 1)Codepage = 866;
	else if(CodepageComboBox->ItemIndex == 2)Codepage = CP_UTF8;


	String Fltr = m_pModule->GetFileFilters();
	if(!Fltr.IsEmpty())SaveDialog->Filter = Fltr+L"|Все файлы (*.*)|*.*";
	else SaveDialog->Filter=L"Все файлы (*.*)|*.*";

	SaveDialog->DefaultExt =  m_pModule->GetDefaultExt();

	if(!SaveDialog->Execute())return;

	AnsiString Str = WideToString(Accum,Codepage);

	int iFileHandle = FileCreate(SaveDialog->FileName);
	FileWrite(iFileHandle, Str.data(), Str.Length());
	FileClose(iFileHandle);

	SaveParamClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TDataExportForm::SaveParamClick(TObject *Sender)
{
    String str = Trim(ParamEdit->Text);
    if(!str.IsEmpty() &&
        ParamEdit->Items->IndexOf(str)<0)
    {
        ParamEdit->Items->Add(str);
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataExportForm::DeleteParamClick(TObject *Sender)
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

void __fastcall TDataExportForm::CopyDataClick(TObject *Sender)
{
	if(!m_pModule)return;
	if(!OutMemo->Lines->Count)return;
	Clipboard()->AsText = Accum;
}
//---------------------------------------------------------------------------

void __fastcall TDataExportForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	SaveModuleConfig();
    SaveConfig();
}
//---------------------------------------------------------------------------

void __fastcall TDataExportForm::FormShow(TObject *Sender)
{
    LoadFormPlacement(this);
}
//---------------------------------------------------------------------------

void __fastcall TDataExportForm::FilterComboChange(TObject *Sender)
{
	SaveModuleConfig();
	LoadModuleConfig();
}
//---------------------------------------------------------------------------

void __fastcall TDataExportForm::ExpressionEditButtonClick(TObject *Sender)
{
	ExpressionEdit->Text = ScriptEdit(ExpressionEdit->Text);
}
//-----------------------------------------------------------------