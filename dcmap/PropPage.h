//---------------------------------------------------------------------------


#ifndef PropPageH
#define PropPageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sFrameAdapter.hpp"
#include "sPageControl.hpp"
#include <ComCtrls.hpp>
#include "sTabControl.hpp"
#include "dcmapsyslib.h"
#include "dcmapsys_vcl.h"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TPropPageFrame : public TFrame
{
__published:	// IDE-managed Components
    TsFrameAdapter *sFrameAdapter;
	TsTabControl *Tabs;
    TsPanel *ClientPanel;
	void __fastcall TabsChange(TObject *Sender);
	void __fastcall TabsGetImageIndex(TObject *Sender, int TabIndex, int &ImageIndex);
private:	// User declarations
    IDCMapWorkspaceEx* m_pWorkspace;
    TFrame* m_pCurrentFrame;

	std::vector<IDCMapVCLPanel*> m_vecPanels;

	int m_iBaseWidth;
	int m_iBaseHeight;

	bool m_bRedused;
	bool m_bSingle;

	void ShowCurrentFrame(TFrame* Frame);
public:		// User declarations
    __fastcall TPropPageFrame(TComponent* Owner);
    virtual __fastcall ~TPropPageFrame();
    void OnCreate();
    void OnShow();
    String PluginClassName;
    void Init(IDCMapWorkspaceEx* pWorkspace=0);
    void SendMessage(int Msg,DWORD wParam,DWORD lParam);
    void DoApply();
    void DoOk();
    void DoCansel();
    bool DoTestData();
	void SetOnKeyValueChange(TKeyValueChangeChangeEvent evt);
	void SelectRef(DCMapDataReference const& ref);

	void SetOnChange(TNotifyEvent evt);
	void SetOnApplyChanges(TNotifyEvent evt);
	void SetOnUpdateList(TNotifyEvent evt);

	void Show();
	void Hide();

    void SetControlFocus();

    int WidthUpdate;
    int HeightUpdate;
};
//---------------------------------------------------------------------------
extern PACKAGE TPropPageFrame *PropPageFrame;
//---------------------------------------------------------------------------
#endif
