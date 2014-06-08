//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef BaseDataEditH
#define BaseDataEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "TntForms.hpp"
//#include "TntMenus.hpp"
#include "PropPage.h"
#include "sSkinProvider.hpp"
#include "sBitBtn.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include <Menus.hpp>
#include "IconsListMan.h"
#include "BaseViewPopup.h"

class TDCMapUIContext;

//---------------------------------------------------------------------------
class TBaseEditForm : public TBaseViewPopupForm
{
typedef TBaseViewPopupForm Inherited;
__published:	// IDE-managed Components
	TsBitBtn *DataPasteButton;
	TsSpeedButton *DataCopyButton;
	TPopupMenu *CopyMenu;
	TPropPageFrame *PropPageFrame;
	TsSpeedButton *ContextButton;
	TPopupMenu *ContextMenu;
    void __fastcall CopyMenuClick(TObject *Sender);
    void __fastcall DataCopyButtonClick(TObject *Sender);
    void __fastcall DataPasteButtonClick(TObject *Sender);
	void __fastcall ContextMenuPopup(TObject *Sender);
	void __fastcall ContextButtonClick(TObject *Sender);
protected:
    DCMapDataSourcePtr<IDCMapDataStorage> m_pData;
    IDCMapDataSource* m_pDataSource;

    TDCMapUIContext* m_pContext;

	TIconsList* m_pIconsList;

    DataReference m_SelRef;
    DataReference m_ParentRef;

    int m_iCurrentCopyMode;

    virtual void DoApply(){};
	virtual void DoUpdateData(){};
	void SelChanged();

    String NewRecordName;

	virtual void __fastcall KeyValueChange(String Val,TObject *Sender);
public:		// User declarations
    AnsiString PluginClassName;
	AnsiString TableName;
	__fastcall TBaseEditForm(TComponent* Owner);
	__fastcall ~TBaseEditForm();
    virtual void SaveConfig();
	virtual void LoadConfig();

    virtual bool Init(IDCMapDataSource* pData,DataReference const* Parent=0);
};
//---------------------------------------------------------------------------
extern PACKAGE TBaseEditForm *BaseEditForm;
//---------------------------------------------------------------------------
#endif
