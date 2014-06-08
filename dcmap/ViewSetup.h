//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef ViewSetupH
#define ViewSetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "dcmapsys_vcl.h"
#include "dcmapsyslib.h"
#include "sSkinProvider.hpp"
#include "sAlphaListBox.hpp"
#include "sCheckListBox.hpp"
#include "sButton.hpp"
#include "sPageControl.hpp"
#include <ComCtrls.hpp>
#include "sComboBox.hpp"
#include "BaseViewPopup.h"
#include "sLabel.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>

//---------------------------------------------------------------------------
class TViewSetupForm : public TBaseViewPopupForm
{
__published:	// IDE-managed Components
	TsButton *OkButton;
	TsPageControl *Pages;
	TsTabSheet *SideBarTab;
	TsListBox *RightSidebar;
	TsListBox *LeftSidebar;
	TsListBox *SidebarModules;
	TsTabSheet *ToolbarTab;
	TsListBox *Toolbar;
	TsListBox *ToolbarModules;
	TsButton *CloseButton;
	TsTabSheet *PanelTab;
	TsListBox *PanelModules;
	TsListBox *TopPanels;
	TsListBox *BottomPanels;
	TsListBox *LeftPanels;
	TsListBox *RightPanels;
	TsTabSheet *MainTab;
	TsComboBox *ViewCombo;
	TsLabel *InfoLabel1;
	TsLabel *InfoLabel2;
	TsLabel *InfoLabel3;
	TsTabSheet *ModulesTab;
	TsCheckListBox *ModulesCheckList;
	void __fastcall ListStartDrag(TObject *Sender, TDragObject *&DragObject);
	void __fastcall ListDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ListDragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall OnChange(TObject *Sender);

private:	// User declarations
	void LoadItemsList(String Name,TsListBox* list,TsListBox* ref);
    void SaveItemsList(String Name,TsListBox* list);
    bool Changed;
public:		// User declarations
    IDCMapWorkspace* m_pWorkspace;
	__fastcall TViewSetupForm(TComponent* Owner);
	void Save();
};
//---------------------------------------------------------------------------
extern PACKAGE TViewSetupForm *ViewSetupForm;
//---------------------------------------------------------------------------
#endif
