//---------------------------------------------------------------------------

#ifndef BaseFrameFormModuleH
#define BaseFrameFormModuleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sSkinProvider.hpp"
#include "BaseFormModule.h"
#include <Menus.hpp>
#include "sPanel.hpp"
#include <ExtCtrls.hpp>

class CBaseFrameFormService;
class TFrameUI;

//---------------------------------------------------------------------------
class TBaseFrameForm : public TBaseModuleForm
{
	friend class CBaseFrameFormModule;
	friend class TFrameUI;

	typedef  TBaseModuleForm  Inherited;
__published:	// IDE-managed Components
	TMainMenu *FrameMenu;
	TsPanel *HintPanel;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
	bool m_bHint;
protected:
	WINDOWPLACEMENT m_WndPlace;
	bool  m_bWndPlace;

	CBaseFrameFormService* m_pService;
	TFrameUI* m_pUI;
	void __fastcall CreateParams(TCreateParams &Params);
	virtual bool OnInit();
	virtual void LoadConfig();
	virtual bool OnNewWorkspace(IDCMapWorkspaceUI* pWorkspace);
public:		// User declarations
	__fastcall TBaseFrameForm(TComponent* Owner);
	void HideHint();
	void ShowHint(RECT const& area,dcmapWCSTR string);
};

class CBaseFrameFormModule : public IDCMapModule
{
protected:
	IDCMapSys*        m_pSystem;
	IDCMapCore*       m_pCore;
	IDCMapWorkspace*  m_pWorkspace;

	TBaseFrameForm* m_pForm;
	virtual TBaseFrameForm* CreateForm()=0;
public:
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	TBaseFrameForm* GetForm(){return m_pForm;}

	virtual void DCMAPAPI SetFrameConfig(dcmapWCSTR Config);
	virtual void DCMAPAPI RestoreSessions();

};
DCMAP_SPEC_INTERFACE(CBaseFrameFormModule);

//---------------------------------------------------------------------------
extern PACKAGE TBaseFrameForm *BaseFrameForm;
//---------------------------------------------------------------------------
#endif
