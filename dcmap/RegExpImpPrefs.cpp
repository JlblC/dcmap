//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RegExpImpPrefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sAlphaListBox"
#pragma link "sComboBox"
#pragma link "sMemo"
#pragma link "sButton"
#pragma link "sEdit"
#pragma link "ScriptEditModule"
#pragma resource "*.dfm"
#include "dcmapsyslib.h"
TRegExpImpPrefsFrame *RegExpImpPrefsFrame;

static CFramePanelWrapper<TRegExpImpPrefsFrame> sWrap("PPrefs_base","RegExpImpPrefs");

//---------------------------------------------------------------------------
__fastcall TRegExpImpPrefsFrame::TRegExpImpPrefsFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------

class TParamInfo : public TObject
{
	public:
	UnicodeString RegExp;
	String Format;
    String Section;
};

bool TRegExpImpPrefsFrame::OnInit()
{
	{
        TParamInfo* pinf = new TParamInfo;
        pinf->Section = "MapNavView";
        pinf->Format = "RegExConf";
        pinf->RegExp = "RegExp";
        BlockComboBox->Items->AddObject(LSTRW(MapRegExpImportCaption),pinf);
    }
	{
        TParamInfo* pinf = new TParamInfo;
        pinf->Section = "MapNavView";
        pinf->RegExp = "Format";
        pinf->Format = "ClipbrdExport";
        BlockComboBox->Items->AddObject(LSTRW(MapClipbrdExportCaption),pinf);
    }

    TStringList* pTables = new TStringList;
    GetDCMapSys()->GetClassPluginNames("PDataStorage",pTables);
    for(int i=0;i<pTables->Count;i++)
    {
        TParamInfo* pinf = new TParamInfo;
        pinf->Section = "Table_"+pTables->Strings[i];
        pinf->RegExp = "RegExp";
    	pinf->Format = "RegExConf";
        BlockComboBox->Items->AddObject(LSTRW(TableRegExpImportCaption)+pTables->Strings[i],pinf);
    }
    for(int i=0;i<pTables->Count;i++)
    {
        TParamInfo* pinf = new TParamInfo;
        pinf->Section = "Table_"+pTables->Strings[i];
        pinf->RegExp = "Format";
    	pinf->Format = "ClipbrdExport";
        BlockComboBox->Items->AddObject(LSTRW(TableClipbrdExportCaption)+pTables->Strings[i],pinf);
    }
    delete pTables;

    BlockComboBox->ItemIndex=0;
    BlockComboBoxChange(this);
    return true;
}
void TRegExpImpPrefsFrame::OnApply()
{
    SaveParams();
}

void TRegExpImpPrefsFrame::SaveParams()
{
	if(m_strCurrentSection.IsEmpty())return;

	ConfListBox->ItemIndex=-1;
    ConfListBoxClick(this);

    int num=ConfListBox->Items->Count;

    DCMapConfigWrapper cfg(m_pCore->GetConfig());
    cfg.section(m_strCurrentSection);
    cfg[m_strCurrentBaseName+"Num"] << num;


    for(int i=0;i<num;i++)
    {
    	TParamInfo* inf = (TParamInfo*)ConfListBox->Items->Objects[i];

        cfg[m_strCurrentBaseName+m_strCurrentParamName+IntToStr(i)] << inf->RegExp;
    	cfg[m_strCurrentBaseName+"Name"+IntToStr(i)] << ConfListBox->Items->Strings[i];
    }
}

void TRegExpImpPrefsFrame::LoadParams(String Section,String BaseName,String ParamName)
{
	SaveParams();

    m_iCurrentID = -1;
	ConfListBox->Clear();

	m_strCurrentSection = Section;
    m_strCurrentBaseName = BaseName;
	m_strCurrentParamName = ParamName;

    DCMapConfigWrapper cfg(m_pCore->GetConfig());

    cfg.section(m_strCurrentSection);
    int num=0;
    cfg[m_strCurrentBaseName+"Num"] >> num;

    for(int i=0;i<num;i++)
    {
        TParamInfo* inf = new TParamInfo;
    	cfg[m_strCurrentBaseName+m_strCurrentParamName+IntToStr(i)] >> inf->RegExp;
    	String str;
    	cfg[m_strCurrentBaseName+"Name"+IntToStr(i)] >> str;
        ConfListBox->Items->AddObject(str,inf);
    }
    if(num)
    {
        m_iCurrentID = -1;
        ConfListBox->ItemIndex=0;
    }
    ConfListBoxClick(this);
}
void __fastcall TRegExpImpPrefsFrame::AddButtonClick(TObject *Sender)
{
    TParamInfo* inf = new TParamInfo;
    ConfListBox->Items->AddObject("",inf);
    ConfListBox->ItemIndex = ConfListBox->Items->Count-1;
    ConfListBoxClick(Sender);
	NameEdit->Text = LSTRW(NewRegExpParam);
}
//---------------------------------------------------------------------------

void __fastcall TRegExpImpPrefsFrame::DeleteButtonClick(TObject *Sender)
{
	if(ConfListBox->ItemIndex < 0)return;
    m_iCurrentID=-1;
    ConfListBox->Items->Delete(ConfListBox->ItemIndex);
    ConfListBoxClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TRegExpImpPrefsFrame::ConfListBoxClick(TObject *Sender)
{
	if(m_iCurrentID >= 0)
    {
		TParamInfo* inf = (TParamInfo*)ConfListBox->Items->Objects[m_iCurrentID];

		inf->RegExp = ScriptEditFrame->GetText();
    }
    m_iCurrentID = ConfListBox->ItemIndex;
    if(m_iCurrentID < 0)
    {
		NameEdit->Text = "";
		ScriptEditFrame->SetText(L"");
	}
    else
    {
		NameEdit->Text = ConfListBox->Items->Strings[m_iCurrentID];
        TParamInfo* inf = (TParamInfo*)ConfListBox->Items->Objects[m_iCurrentID];

		ScriptEditFrame->SetText(inf->RegExp);
	}

}
//---------------------------------------------------------------------------

void __fastcall TRegExpImpPrefsFrame::NameEditChange(TObject *Sender)
{
	if(m_iCurrentID < 0)return;
    ConfListBox->Items->Strings[m_iCurrentID] = NameEdit->Text;
}
//---------------------------------------------------------------------------

void __fastcall TRegExpImpPrefsFrame::BlockComboBoxChange(TObject *Sender)
{
        TParamInfo* pinf = (TParamInfo*)BlockComboBox->Items->Objects[BlockComboBox->ItemIndex];
        LoadParams(pinf->Section,pinf->Format,pinf->RegExp);
}
//---------------------------------------------------------------------------

