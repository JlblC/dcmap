//---------------------------------------------------------------------------


#ifndef ViewLayoutH
#define ViewLayoutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sFrameAdapter.hpp"
#include "sFrameBar.hpp"
#include "sScrollBox.hpp"
#include "sButton.hpp"
#include "sBitBtn.hpp"
#include <Buttons.hpp>
#include "sStatusBar.hpp"
#include <ComCtrls.hpp>
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include "sFrameBar.hpp"

#include "dcmapsys_vcl.h"
#include "dcmapsyslib.h"

#include "UIWorkspace.h"
#include <Menus.hpp>
#include "sSplitter.hpp"
#include "acCoolBar.hpp"
#include <ToolWin.hpp>
//#include "TntMenus.hpp"

#include <map>
#include <vector>

class TBaseFrameForm;
class TUIWorkspace;

//---------------------------------------------------------------------------
class TViewLayoutFrame : public TFrame
{
  friend class TMainForm;
  friend class TBaseFrameForm;
  friend class TUIWorkspace;

__published:	// IDE-managed Components
    TsFrameAdapter *sFrameAdapter1;
	TsFrameBar *RightBar;
	TsFrameBar *LeftBar;
	TsSplitter *RightClickField;
	TsSplitter *LeftClickField;
	TsCoolBar *TopBar;
	TMainMenu *MainMenu;
    void __fastcall ClientPanelResize(TObject *Sender);
    void __fastcall sFrameBarItemsCreateFrame(TObject *Sender,
          TCustomFrame *&Frame);
    void __fastcall sFrameBarItemsFrameDestroy(TObject *Sender,
          TCustomFrame *&Frame, bool &CanDestroy);
	void __fastcall RightClickFieldClick(TObject *Sender);
	void __fastcall LeftClickFieldClick(TObject *Sender);
    void __fastcall FrameResize(int Width,int Height,TObject* Sender);
	void __fastcall TopBarResize(TObject *Sender);
	void __fastcall LeftClickFieldCanResize(TObject *Sender, int &NewSize,
          bool &Accept);
	void __fastcall RightClickFieldCanResize(TObject *Sender, int &NewSize,
          bool &Accept);

private:	// User declarations
	TBaseFrameForm* FrameForm;

	IDCMapVCLPanel* m_pClientViewPanel;
    IDCMapView*     m_pClientView;
    std::map<TsTitleItem*,IDCMapVCLPanel*> m_mapRightBarPanels;
    std::map<TCustomFrame*,TsTitleItem*> m_mapSideBarFrames;

    struct PanelInfo
    {
        IDCMapVCLPanel* Panel;
        String ModuleName;
        PanelInfo(){};
		PanelInfo(IDCMapVCLPanel* p,String n):Panel(p),ModuleName(n){}
    };
    std::map<TCoolBand*,PanelInfo> m_mapTopBarPanels;

	typedef std::vector<IDCMapVCLPanel*> VecPanels;
	VecPanels m_vecTopPanels;
	VecPanels m_vecBottomPanels;
	VecPanels m_vecLeftPanels;
	VecPanels m_vecRightPanels;

	void InitPanels(String Name,TAlign PanelAlign,VecPanels& panels);
	void SavePanels(String Name,VecPanels& panels);

	bool m_bInitComplete;
	bool m_bInitFailed;

    void DoCreate();
	void InitBar(TsFrameBar* Bar);
    void CreateToolBar();
	void SaveToolBar();

	void MergeMenu();
	void UnmergeMenu();

	String ConfigName;

public:
    IDCMapWorkspaceEx* m_pWorkspace;
    TUIWorkspace* m_pUIWorkspace;

public:		// User declarations
    __fastcall TViewLayoutFrame(TComponent* Owner);
    void Create(TBaseFrameForm* FrameForm,UnicodeString TabName,bool NewSession,String Template="");
    void Destroy();
	void InitView();

	void Activate();
    void Deactivate();
    void SaveConfig();

    void SaveAsTemplate(String Name);

    bool MouseWheel(TShiftState Shift, int WheelDelta, TPoint &MousePos);

    void ActivateView()
	{
		if(m_pClientView)m_pClientView->Activate(true);
	}
	bool UpdatedLayout;

	TWinControl* ClientControl;

};
//---------------------------------------------------------------------------
extern PACKAGE TViewLayoutFrame *ViewLayoutFrame;
//---------------------------------------------------------------------------
#endif
