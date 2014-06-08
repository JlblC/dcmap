//---------------------------------------------------------------------------

#ifndef DataEditH
#define DataEditH
#include "BaseDataEdit.h"
#include "PropPage.h"
#include "sBitBtn.hpp"
#include "sButton.hpp"
#include "sSkinProvider.hpp"
#include "sSpeedButton.hpp"
//#include "TntMenus.hpp"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <vector>
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sSkinProvider.hpp"
#include "sButton.hpp"
#include "sPageControl.hpp"
#include <ComCtrls.hpp>
#include "sTabControl.hpp"

#include "dcmapsyslib.h"
#include "dcmapsys_vcl.h"
#include "sMemo.hpp"
#include "PropPage.h"
//#include "TntForms.hpp"
//#include "TntMenus.hpp"
#include "BaseDataEdit.h"
#include "sBitBtn.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include <Menus.hpp>

//---------------------------------------------------------------------------
class TDataEditForm : public TBaseEditForm
{
	typedef TBaseEditForm Inherited;
__published:	// IDE-managed Components
	TsButton *OkButton;
	TsButton *CloseButton;
	TsBitBtn *ApplyButton;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall KeyValueChange(String Val,TObject *Sender);
	void __fastcall ApplyButtonClick(TObject *Sender);
private:	// User declarations
    void DoApply();
	void DoUpdateData();
public:		// User declarations
    __fastcall TDataEditForm(TComponent* Owner);
    void Select(DataReference const& Ref);
	bool Init(IDCMapDataSource* pData,DataReference const* Parent=0);
};
//---------------------------------------------------------------------------
extern PACKAGE TDataEditForm *DataEditForm;
//---------------------------------------------------------------------------
#endif
