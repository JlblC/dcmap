//---------------------------------------------------------------------------

#ifndef TabsFormModuleH
#define TabsFormModuleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseFrameFormModule.h"
#include "sSkinProvider.hpp"
#include "sTabControl.hpp"
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include "ViewLayout.h"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TTabsFrameForm : public TBaseFrameForm
{
	typedef TBaseFrameForm Inherited;
__published:	// IDE-managed Components
	TsTabControl *FrameTabs;
	TPopupMenu *TabMenu;
	TMenuItem *CntCloseMenuItem;
	TMenuItem *CntRenameMenuItem;
	TMenuItem *CntSaveTemplateMenuItem;
	TMenuItem *ViewMenu;
	TMenuItem *NewTabMenu;
	TMenuItem *CreateTabMenu;
	TMenuItem *StandartTabMenu;
	TMenuItem *N1;
	TMenuItem *CloseTabMenu;
	TMenuItem *RenameTabMenu;
	TMenuItem *N2;
	TMenuItem *SaveTabAsTemplateMenu;
	void __fastcall FrameTabsChange(TObject *Sender);
	void __fastcall FrameTabsMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FrameTabsMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FrameTabsMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall CntCloseMenuItemClick(TObject *Sender);
	void __fastcall CntRenameMenuItemClick(TObject *Sender);
	void __fastcall CntSaveTemplateMenuItemClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CreateTabClick(TObject *Sender);
	void __fastcall AddTabMenuClick(TObject *Sender);

protected:	// User declarations
	struct TabDesc : public TObject
    {
    	__fastcall TabDesc(){pFrame=0;}
        __fastcall ~TabDesc(){if(pFrame){pFrame->Destroy();delete pFrame;}}
        TViewLayoutFrame* pFrame;
        String FrameName;
        UnicodeString TabName;
    };
    bool m_bAutoHideTabs;
    TabDesc* m_pHiddenTab;

    TViewLayoutFrame* m_pActiveFrame;
    int m_iFrameCounter;
   	int OperateTabIndex;
	int DragTabIndex;

    bool RestoreMode;
    TUIWorkspace* RestoreUI;

    void UpdateAutoHideTabs(bool Restore=false);
    TViewLayoutFrame* AddSession(String FrameName,UnicodeString Caption,bool NewSession, String Template="");
    TViewLayoutFrame* RestoreSession(int Index);

    void CloseTab(int Index);
    void ActivateTab(int Index);

    void CreateTab(UnicodeString TabName,String FrameName,String Template);

public:		// User declarations
	__fastcall TTabsFrameForm(TComponent* Owner);
    void CloseTabs();
    void RecreateTabs();
	void RestoreSessions();
	virtual bool OnInit();
	virtual bool OnNewWorkspace(IDCMapWorkspaceUI* pWorkspace);
};

class CTabsFormModule : public CBaseFrameFormModule
{
	virtual TBaseFrameForm* CreateForm(){return new TTabsFrameForm(Application);}

public:
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
	virtual void DCMAPAPI RestoreSessions();

	TTabsFrameForm* GetForm(){return (TTabsFrameForm*)m_pForm;}
};
//---------------------------------------------------------------------------
extern PACKAGE TTabsFrameForm *TabsFrameForm;
//---------------------------------------------------------------------------
#endif
