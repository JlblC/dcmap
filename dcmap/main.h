//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>         
#include <sSkinManager.hpp>
#include <sSkinProvider.hpp>
#include "sLabel.hpp"
#include "sTabControl.hpp"
#include "sFrameBar.hpp"
#include "sScrollBox.hpp"
#include "sPanel.hpp"
#include "sPageControl.hpp"
#include <ComCtrls.hpp>
#include "sFrameAdapter.hpp"
#include "ViewLayout.h"
#include "sDialogs.hpp"
#include <Dialogs.hpp>
#include "sHintManager.hpp"
#include "TabsFormModule.h"
#include "BaseFrameFormModule.h"

#define DefCaption (L"DCMap")

//---------------------------------------------------------------------------
class TMainForm : public TBaseFrameForm
{
typedef TBaseFrameForm Inherited;
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TsTabControl *sTabs;
	TPopupMenu *TabMenu;
	TMenuItem *ViewMenu;
	TMenuItem *SetupTabMenuItem;
	TMenuItem *N22;
	TMenuItem *CreateTabMenu;
	TMenuItem *CloseTabMenuItem;
	TMenuItem *NewTabMenuItem;
	TMenuItem *CntSaveTemplateMenuItem;
	TMenuItem *CntRenameMenuItem;
	TMenuItem *CntCloseMenuItem;
	TMenuItem *RenameMenuItem;
	TMenuItem *SaveTemplateMenuItem;
	TMenuItem *StandartTabMenuItem;
	TMenuItem *N1;
	TsPanel *HintPlace;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall NewTabMenuItemClick(TObject *Sender);
    void __fastcall sTabsChange(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall CloseTabMenuItemClick(TObject *Sender);
	void __fastcall sTabsMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall CntCloseMenuItemClick(TObject *Sender);
	void __fastcall CntRenameMenuItemClick(TObject *Sender);
	void __fastcall sTabsMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall CreateTabMenuClick(TObject *Sender);
	void __fastcall TemplateCreateClick(TObject *Sender);
	void __fastcall CntSaveTemplateMenuItemClick(TObject *Sender);
	void __fastcall FormDeactivate(TObject *Sender);
	void __fastcall SetupTabMenuItemClick(TObject *Sender);
	void __fastcall DrawMenuItem(TObject *Sender, TCanvas *ACanvas, TRect &ARect, TOwnerDrawState State);
	void __fastcall sTabsStartDrag(TObject *Sender, TDragObject *&DragObject);
	void __fastcall sTabsMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall ExitMenuItemClick(TObject *Sender);
	void __fastcall SkinManagerGetMenuExtraLineData(TMenuItem *FirstItem, UnicodeString &SkinSection,
		  UnicodeString &Caption, Graphics::TBitmap *&Glyph, bool &LineVisible);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall est1Click(TObject *Sender);

private:	// User declarations
	void __fastcall WndProc(TMessage & Message);

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
    void UpdateAutoHideTabs(bool Restore=false);

    WINDOWPLACEMENT m_WndPlace;
    bool  m_bWndPlace;

    TViewLayoutFrame* m_pActiveFrame;
    int m_iFrameCounter;
    void SaveConfig();
    void LoadConfig();

    TViewLayoutFrame* AddSession(String FrameName,UnicodeString Caption,bool NewSession, String Template="");
    TViewLayoutFrame* RestoreSession(int Index);

	int OperateTabIndex;
	int DragTabIndex;

    void CloseTab(int Index);
    void ActivateTab(int Index);

    void CreateTab(UnicodeString TabName,String FrameName,String Template);

	bool m_bEnableAnimations;
	bool m_bInitComplete;

	bool  m_bWatchClipbrdForCmd;
	bool  m_bAutoCheckUpdates;

	HWND hwndNextClipboardViewer;

	void OnCommand(UnicodeString cmd);

public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
    void CloseTabs();
    void RecreateTabs();
    void RestoreSessions();
	void LoadPrefs();
};

class CMainFormService : public IDCMapModule
{
public:
	CMainFormService();
	int   DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);;
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
};

//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
