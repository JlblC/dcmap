//$$---- Frame CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FixedFilterDB.h"
#include "FixedFilter.h"
#include "Utils.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sComboBox"
#pragma link "sCheckBox"
#pragma link "sFrameAdapter"
#pragma link "sSpeedButton"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma link "sComboEdit"
#pragma link "sCustomComboEdit"
#pragma link "sMaskEdit"
#pragma resource "*.dfm"
TFixedFilterDBFrame *FixedFilterDBFrame;
//---------------------------------------------------------------------------
__fastcall TFixedFilterDBFrame::TFixedFilterDBFrame(TComponent* Owner)
	: TFrame(Owner)
{
	m_iRefOp=OP_EQUAL;
    m_iStrOp=OP_CNT;
	m_iNumOp=OP_GREATER_EQUAL;
	m_iMode=FMODE_AND;
}

__fastcall TFixedFilterDBFrame::~TFixedFilterDBFrame()
{
	DeleteStringsObjects(DBValueComboBox->Items);
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterDBFrame::FieldMenuClick(TObject *Sender)
{
	TMenuItem* Item = dynamic_cast<TMenuItem*>(Sender);
    String Cap = Item->Hint;
    if(Cap.IsEmpty())
    	Cap = " "+Item->Caption;

    ResourceSelBtn->Caption = Cap;

    // Decode name
    int delim = Item->Name.Pos("_");
    Table = Item->Name.SubString(1,delim-1);
    Field = Item->Name.SubString(delim+1,Item->Name.Length());
    
    SetupData();

    if(DBValueComboBox->Visible)DBValueComboBox->SetFocus();
	else if(StringEdit->Visible)StringEdit->SetFocus();
	else if(NumEdit->Visible)NumEdit->SetFocus();
}

void TFixedFilterDBFrame::UpdateData()
{
	if(m_ListDataSrc)
	{
		if(m_ListDataSrc.actualize_state())
		{
            SetupData();
		}
    }
}

void TFixedFilterDBFrame::SetupData()
{
	RecordState();

	// Get table field & id
	IDCMapDataStorage* src = m_pCore->GetDataSourceTyped<IDCMapDataStorage>(WideToString(Table).c_str());
	m_ListDataSrc.reset();
	m_iColID=-1;
    m_bStringData=false;
	m_bCustom=false;

    if(src) // get field id, type, etc
    {
		m_iColID = src->ColumnID(WideToString(Field).c_str());
        DCMapDataType type = src->ColumnDataType(m_iColID);

		m_bStringData = (type == DCMDT_String) || (type == DCMDT_CLOB) ||
						(type == DCMDT_WideString) || (type == DCMDT_WideCLOB);

        if(type == DCMDT_Reference)
        {
			m_ListDataSrc.init(m_pCore,src->ColumnDataLinkage(m_iColID));
		}
    }
    else if(Field == "str")m_bStringData=true;
	else if(Field == "custom")m_bCustom=true;
	else if(m_pCore->GetDataSourceTyped<IDCMapDataStorage>(WideToString(Field).c_str()))
	{
		  m_ListDataSrc.init(m_pCore,Field.c_str());
	}

    DBValueComboBox->Visible = false;
    NumEdit->Visible = false;
    StringEdit->Visible = false;
    ComboEdit->Visible = false;
    OperatorComboBox->Visible=true;

    String PreOp = OperatorComboBox->Text;
    //OperatorComboBox->Items->Clear();

	if(m_ListDataSrc)
	{
		DeleteStringsObjects(DBValueComboBox->Items);
		DBValueComboBox->Items->Clear();
    	DBValueComboBox->Items->Add(g_pLocalStrings->GetStringW("NoDBItem"));

        if(m_ListDataSrc->SelectAll())
        do
        {
			DBValueComboBox->Items->AddObject(m_ListDataSrc->GetWideName(),
				new TDataRefObject(m_ListDataSrc->GetID()));
		}
		while(m_ListDataSrc->Next());

		if(DBValueComboBox->Items->Count > DBValueComboBox->DropDownCount)
			  DBValueComboBox->Style = csDropDown;
		else
			  DBValueComboBox->Style = csDropDownList;

		DBValueComboBox->Visible = true;

		DBValueComboBox->ItemIndex = DBValueComboBox->Items->IndexOf(LastItem);
		if(DBValueComboBox->ItemIndex < 0) DBValueComboBox->ItemIndex = 0;

        OperatorComboBox->Items->Text = "=\n<>\n";
		OperatorComboBox->ItemIndex = m_iRefOp;
    }
    else if(m_bStringData)
    {
        StringEdit->Visible = true;
		OperatorComboBox->Items->Text = "=\n<>\n>=\n<=\n>\n<\nCNT\nNCT";
        OperatorComboBox->ItemIndex = m_iStrOp;
    }
    else if(m_bCustom)
    {
        ComboEdit->Visible = true;
        OperatorComboBox->Visible=false;
    }
    else
    {
        NumEdit->Visible = true;
        OperatorComboBox->Items->Text = "=\n<>\n>=\n<=\n>\n<";
        OperatorComboBox->ItemIndex = m_iNumOp;
    }

    m_eTable = OP_RESOURCE;

    if(Table == "planets")
    	m_eTable = OP_PLANETS;
    else if(Table == "players")
    	m_eTable = OP_PLAYERS;

    if(m_eTable == OP_RESOURCE)
    {
    	if(Table == "Growth") m_eResource = RT_Growth;
    	else if(Table == "Credits") m_eResource = RT_Credits;
    	else if(Table == "MiningBase") m_eResource = RT_MiningBase;
    	else if(Table == "CreditsBase") m_eResource = RT_CreditsBase;
    	else if(Table == "MiningSecond") m_eResource = RT_MiningSecond;
    	else if(Table == "CreditsSecond") m_eResource = RT_CreditsSecond;

    	else if(Table == "Organics") m_eResource = RT_Organics;
    	else if(Table == "Energy") m_eResource = RT_Energy;
    	else if(Table == "Minerals") m_eResource = RT_Minerals;
    	else if(Table == "Temperature") m_eResource = RT_Temperature;
    	else if(Table == "Size") m_eResource = RT_Size;
    	else if(Table == "Y") m_eResource = RT_X;
    	else if(Table == "X") m_eResource = RT_Y;
    	else if(Table == "Sector") m_eResource = RT_Sector;
		else if(Table == "Expression") m_eResource = RT_Expression;
        else if(Table == "Open") m_eResource = RT_Open;
	}
    SetupFilter();
}
//---------------------------------------------------------------------------

void TFixedFilterDBFrame::SetupFilter()
{
	if(!m_pParent->m_pFilter)return;
	FilterDef& fdef = m_pParent->m_pFilter->m_fdef[m_iFilterNo];

	bool PreFilter = fdef.m_bResourceFilter;

    fdef.m_bResourceFilter = UseCheck->Checked;
    fdef.iField = m_iColID;
	fdef.Operation = OperatorComboBox->ItemIndex;
	fdef.Mode = m_iMode;

    fdef.bStrValue=0;
    fdef.RefValue=0;
    fdef.Value=0;

    if(DBValueComboBox->Visible)
	{
		TDataRefObject* pRef = 0;
		int Index = DBValueComboBox->ItemIndex;
        if(Index >=0) pRef = (TDataRefObject*)DBValueComboBox->Items->Objects[Index];

		if(pRef && m_ListDataSrc->SelectID(pRef->ref))
			m_Ref = *m_ListDataSrc->GetID();
		else  m_Ref.Invalidate();
        fdef.RefValue=&m_Ref;
        fdef.pStorage = m_ListDataSrc;
        LastItem = m_ListDataSrc->Name();
    }
    else if(StringEdit->Visible)
    {
        fdef.StrValue = StringEdit->Text.LowerCase();
        fdef.bStrValue = true;
    }
    else if(NumEdit->Visible)
    {
        fdef.Value = NumEdit->Value;
    }
    else if(ComboEdit->Visible)
    {
        fdef.StrValue = ComboEdit->Text;
        fdef.bStrValue = true;
    }

    fdef.table = m_eTable;
    fdef.Resource = m_eResource;

    fdef.Init(m_pWorkspace);

    m_pParent->m_pFilter->Changed = true;
    m_pParent->m_pGeoFilter->Changed = true;

    m_pParent->m_pGeoFilter->m_fdef[m_iFilterNo].Init(m_pWorkspace);
    m_pParent->m_pGeoFilter->m_fdef[m_iFilterNo] = fdef;

    if(PreFilter || fdef.m_bResourceFilter)
        	m_pParent->FilterChanged();
}
void __fastcall TFixedFilterDBFrame::DataChange(TObject *Sender)
{
     SetupFilter();
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterDBFrame::ResourceSelBtnMouseActivate(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y, int HitTest, TMouseActivate &MouseActivate)
{
	m_pParent->CurrentFrame = this;
}
//---------------------------------------------------------------------------
void TFixedFilterDBFrame::LoadConfig(DCMapConfigWrapper cfg)
{
	//cfg.section("FixedFilter");
	String sv;
	String sw;
	float  iv;
	String No = IntToStr(m_iFilterNo);
	cfg["Table"+No] >> Table;
	cfg["Field"+No] >> Field;
	
	if(cfg["ParamName"+No] >> sv)
		ResourceSelBtn->Caption = sv;

	cfg["DBOperator"+No] >> m_iRefOp;
	cfg["StrOperator"+No] >> m_iStrOp;
	cfg["NumOperator"+No] >> m_iNumOp;
	cfg["FilterMode"+No] >> m_iMode;

	if(cfg["Enabled"+No] >> iv) UseCheck->Checked = iv;

	if(cfg["NumParam"+No] >> iv) NumEdit->Value = iv;
	if(cfg["StrParam"+No] >> sv) StringEdit->Text = sv;

	if(cfg["DBParam"+No] >> sw) LastItem = sw;

	if(cfg["CustomParam"+No] >> sv) ComboEdit->Text = sv;


	if(No==0)
	{
		m_iMode = FMODE_AND;
		ModeButton->Visible = false;
	}
	else
	{
	    ModeButton->Visible = true;
		if(m_iMode == FMODE_AND) ModeButton->ImageIndex = InterfaceModule->GetMenuImage(L"fmode-and");
		else if(m_iMode == FMODE_OR) ModeButton->ImageIndex = InterfaceModule->GetMenuImage(L"fmode-or");
	}


	SetupData();
}

void TFixedFilterDBFrame::RecordState()
{
	if(DBValueComboBox->Visible)
	{
		m_iRefOp = OperatorComboBox->ItemIndex;
		LastItem = DBValueComboBox->Text;
	}
	else if(StringEdit->Visible)m_iStrOp = OperatorComboBox->ItemIndex;
	else if(NumEdit->Visible)m_iNumOp = OperatorComboBox->ItemIndex;
}

void TFixedFilterDBFrame::SaveConfig(DCMapConfigWrapper cfg)
{
	RecordState();
	
	//cfg.section("FixedFilter");
	String No = IntToStr(m_iFilterNo);
	cfg["Table"+No] << Table;
	cfg["Field"+No] << Field;
	cfg["ParamName"+No] << ResourceSelBtn->Caption;

	cfg["DBOperator"+No] << m_iRefOp;
	cfg["StrOperator"+No] << m_iStrOp;
	cfg["NumOperator"+No] << m_iNumOp;

	cfg["Enabled"+No] << UseCheck->Checked;

	cfg["DBParam"+No] << LastItem;
	cfg["StrParam"+No] << StringEdit->Text;
	cfg["NumParam"+No] << NumEdit->Value;
	cfg["CustomParam"+No] << ComboEdit->Text;

	cfg["FilterMode"+No] << m_iMode;
}

void __fastcall TFixedFilterDBFrame::UseCheckClick(TObject *Sender)
{
    if(UseCheck->Checked)m_pParent->NoFilterCheckBox->Checked = false;
    else m_pParent->NoFilterCheckBoxClick(Sender);
	DataChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterDBFrame::DBValueComboBoxClick(TObject *Sender)
{
	SetupFilter();
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterDBFrame::DBValueComboBoxDropDown(TObject *Sender)
{
    DBValueComboBox->Text = LastItem;
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterDBFrame::DBValueComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
      TOwnerDrawState State)
{
	TsComboBox* box = dynamic_cast<TsComboBox*>(Control);
    if(!box)return;
    String str = box->Items->Strings[Index];
	box->Canvas->TextRect(Rect,Rect.left+2,Rect.top,str);
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterDBFrame::ComboEditButtonClick(TObject *Sender)
{
	ComboEdit->Text = ScriptEdit(ComboEdit->Text);
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterDBFrame::ModeButtonClick(TObject *Sender)
{
	if(m_iMode == FMODE_AND) m_iMode = FMODE_OR;
	else m_iMode = FMODE_AND;

	if(m_iMode == FMODE_AND) ModeButton->ImageIndex = InterfaceModule->GetMenuImage(L"fmode-and");
	else if(m_iMode == FMODE_OR) ModeButton->ImageIndex = InterfaceModule->GetMenuImage(L"fmode-or");

	DataChange(Sender);
}
//---------------------------------------------------------------------------

