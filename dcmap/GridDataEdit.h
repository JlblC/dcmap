//---------------------------------------------------------------------------

#ifndef GridDataEditH
#define GridDataEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseViewPopup.h"
#include "sSkinProvider.hpp"
#include <Grids.hpp>
//---------------------------------------------------------------------------

class TGridDataEditForm : public TBaseViewPopupForm
{
	typedef TBaseViewPopupForm Inherited;

__published:	// IDE-managed Components
	TDrawGrid *Grid;
	void __fastcall GridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall GridGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
	void __fastcall GridSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall GridColumnMoved(TObject *Sender, int FromIndex, int ToIndex);





private:	// User declarations
	IDCMapDataStoragePtr m_pStorage;

	struct ColumnInfo
	{
		int id;
		AnsiString Name;
		int datatype;
		int datasize;
		int Width;
	};
	typedef std::vector<dcmapDBREF> DataRefsList;
	int LastDataRow;
	DataRefsList DataRefs;

	typedef std::vector<ColumnInfo> ListColumns;
	ListColumns Columns;

	String TableName;
	String ColumnsSction;

	dcmapWCSTR CellText(int Col,int Row);

	bool LoadColumnSetup(String SectionName);
	bool SaveColumnSetup(String SectionName);

public:		// User declarations
	__fastcall TGridDataEditForm(TComponent* Owner);
	bool SetupTable(String TableName);
};

//---------------------------------------------------------------------------
extern PACKAGE TGridDataEditForm *GridDataEditForm;
//---------------------------------------------------------------------------
#endif
