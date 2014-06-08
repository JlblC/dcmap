//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "GridDataEdit.h"

#include "api/dcmap_script_helper.h"
#include "sGraphUtils.hpp"
#include "SkinUtils.hpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseViewPopup"
#pragma link "sSkinProvider"
#pragma link "sGraphUtils"

#pragma resource "*.dfm"
TGridDataEditForm *GridDataEditForm;

class CGridDataEdit : public CBasicFormService<TGridDataEditForm>
{
public:
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);

	bool EditTable(String table)
	{
		 TGridDataEditForm* pForm = GetInstance();
		 if(!pForm->SetupTable(table))return false;
		 ShowModal();
		 return true;
	}
};

DCMAP_SCRIPT_REFLECTION(CGridDataEdit)
{
	DCMAP_SCRIPT_FUNC(EditTable);
}


static CAutoReg<CGridDataEdit> sWrap("PCoreService","GridDataEdit");

//---------------------------------------------------------------------------
__fastcall TGridDataEditForm::TGridDataEditForm(TComponent* Owner)
	: TBaseViewPopupForm(Owner)
{
}
//---------------------------------------------------------------------------

bool TGridDataEditForm::LoadColumnSetup(String SectionName)
{
	DCMapConfigWrapper cfg(m_pCore);
	cfg.section(SectionName);

	int NumColumns = 0;
	cfg["NumColumns"] >> NumColumns;

	if(!NumColumns) return false;

	ColumnInfo inf;
	for(int i=0;i<NumColumns;i++)
	{
		String base = L"Col"+IntToStr(i);
		cfg[base+L"Name"] >> inf.Name;
		cfg[base+L"Width"] >> inf.Width;

		inf.id = m_pStorage->ColumnID(inf.Name.c_str());

		if(inf.id <= 0)continue;

		inf.datatype = m_pStorage->ColumnDataType(inf.id);
		inf.datasize = m_pStorage->ColumnDataSize(inf.id);

		Columns.push_back(inf);
	}
	return true;
}

bool TGridDataEditForm::SaveColumnSetup(String SectionName)
{
	DCMapConfigWrapper cfg(m_pCore);
	cfg.section(SectionName);

	int NumColumns = Columns.size();
	cfg["NumColumns"] << NumColumns;


	for(int i=0;i<NumColumns;i++)
	{
		ColumnInfo& inf = Columns[i];
		String base = L"Col"+IntToStr(i);
		inf.Width = Grid->ColWidths[i+1];

		cfg[base+L"Name"] << inf.Name;
		cfg[base+L"Width"] << inf.Width;
	}
	return true;
}

bool TGridDataEditForm::SetupTable(String strTableName)
{
	DataRefs.clear();
	Columns.clear();

	TableName = strTableName;
	ColumnsSction = L"GridEditColumns_"+strTableName;


	if(!m_pStorage.init(m_pCore,AnsiString(TableName).c_str()))return false;

	if(!LoadColumnSetup(ColumnsSction))
	{
		ColumnInfo inf;
		int n = m_pStorage->NumColumns();
		for(int i=1;i<n;i++)
		{
			inf.Name = m_pStorage->ColumnName(i);
			inf.id = i;
			inf.datatype = m_pStorage->ColumnDataType(i);
			inf.datasize = m_pStorage->ColumnDataSize(i);
			inf.Width = 50;

			Columns.push_back(inf);
		}
	}
	Grid->ColCount = Columns.size()+1;

	Grid->ColWidths[0] = 20;

	for(int i=1;i<Grid->ColCount;i++)
	{
		Grid->ColWidths[i] = Columns[i-1].Width;
	}

	if(m_pStorage->SelectAll())
	do
	{
		 DataRefs.push_back(*m_pStorage->GetID());
	}
	while(m_pStorage->Next());

	Grid->RowCount = DataRefs.size()+2;

	LastDataRow = -1;

	return true;
}

dcmapWCSTR TGridDataEditForm::CellText(int Col,int Row)
{
	if(Row >= DataRefs.size())return L"";

	if(LastDataRow != Row)
	{
		m_pStorage->SelectID(DataRefs[Row]);
	}
	LastDataRow = Row;
	int id = Columns[Col].id;
	return m_pStorage->GetWideString(id);
}

void __fastcall TGridDataEditForm::GridDrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State)
{
	int Row = ARow-1;
	int Col = ACol-1;

	if(ARow == 0 && ACol == 0)
	{
		TRect ItemRect(Rect);
		ItemRect.right += 1;
		ItemRect.bottom += 1;

		SkinPaintItem(Grid,ItemRect,Grid->Canvas,"COLHEADER",SkinProvider->SkinData->SkinManager);
	}
        else if(ACol == 0)
	{
        	TRect ItemRect(Rect);
		ItemRect.right += 1;
		ItemRect.bottom += 1;

		SkinPaintItem(Grid,ItemRect,Grid->Canvas,"COLHEADER",SkinProvider->SkinData->SkinManager);
	}
	else if(ARow == 0 && ACol > 0)
	{
		TRect TextRect(Rect);
		TextRect.left += 5;
		TextRect.right -= 2;

		TRect ItemRect(Rect);
		ItemRect.right += 1;
		//ItemRect.left -= 1;
		ItemRect.bottom += 1;

		String Caption =  CutText(Grid->Canvas,Columns[Col].Name,TextRect.Width());

		SkinPaintItem(Grid,ItemRect,Grid->Canvas,"COLHEADER",SkinProvider->SkinData->SkinManager);

		WriteText(Grid->Canvas,Caption.w_str(),True,TextRect,DT_LEFT|DT_VCENTER);
	}
	else if(ARow > 0)
	{
		dcmapWCSTR str = CellText(Col,Row);
		String Caption =  CutText(Grid->Canvas,str,Rect.Width()-3);
		Grid->Canvas->TextRect(Rect,Rect.left+2,Rect.top+2,Caption);
	}
}
//---------------------------------------------------------------------------

void __fastcall TGridDataEditForm::GridGetEditText(TObject *Sender, int ACol, int ARow,
		  UnicodeString &Value)
{
	if(ARow > 0 && ACol > 0)
	{
	   Value = CellText(ACol-1,ARow-1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TGridDataEditForm::GridSetEditText(TObject *Sender, int ACol, int ARow,
		  const UnicodeString Value)
{
	if(ARow > 0 && ACol > 0)
	{
        	if(ARow > DataRefs.size())return;

		String PreValue = CellText(ACol-1,ARow-1);
		if(PreValue == Value)return;
		m_pStorage->Update();
		int id = Columns[ACol-1].id;
		m_pStorage->SetWideString(id,Value.w_str());
		m_pStorage->Post(&DataRefs[ARow-1]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TGridDataEditForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	SaveColumnSetup(ColumnsSction);
	Inherited::FormClose(Sender,Action);
}
//---------------------------------------------------------------------------


void __fastcall TGridDataEditForm::GridColumnMoved(TObject *Sender, int FromIndex,
		  int ToIndex)
{
	ColumnInfo inf = Columns[FromIndex-1];
	Columns.erase(Columns.begin()+(FromIndex-1));
	Columns.insert(Columns.begin()+(ToIndex-1),inf);

	Grid->Refresh();
}
//---------------------------------------------------------------------------


