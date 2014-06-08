//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef PreferencesH
#define PreferencesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sSkinProvider.hpp"
#include "sPanel.hpp"
#include "sTreeView.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "sButton.hpp"
//#include "TntForms.hpp"
//#include "TntMenus.hpp"

#include "dcmapsys_vcl.h"
#include "BaseViewPopup.h"
#include <vector>
#include "BaseViewPopup.h"

//---------------------------------------------------------------------------
class TPreferencencesForm : public TBaseViewPopupForm
{
	typedef TBaseViewPopupForm Inherited;
__published:	// IDE-managed Components
	TsTreeView *TreeView;
	TsPanel *ClientPanel;
	TsButton *ApplyButton;
	TsButton *OkButton;
	TsButton *CloseButton;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ApplyButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TreeViewChange(TObject *Sender, TTreeNode *Node);
private:	// User declarations
    std::vector<IDCMapVCLPanel*> m_vecPanels;
    TFrame* m_pCurrentFrame;
public:		// User declarations
	__fastcall TPreferencencesForm(TComponent* Owner);
    __fastcall ~TPreferencencesForm();
    void __fastcall FillGroup(AnsiString Name);

};
//---------------------------------------------------------------------------
extern PACKAGE TPreferencencesForm *PreferencencesForm;
//---------------------------------------------------------------------------
#endif
