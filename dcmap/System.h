//---------------------------------------------------------------------------

#ifndef SystemH
#define SystemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <AppEvnts.hpp>
#include <ExtCtrls.hpp>

#include "dcmapbase.h"
#include <DdeMan.hpp>
//---------------------------------------------------------------------------
class TSystemModule : public TDataModule
{
	friend class CSystemService;
__published:	// IDE-managed Components
	TApplicationEvents *ApplicationEvents;
	TTrayIcon *TrayIcon;
	TDdeServerConv *ucmd;
	TTimer *CheckUpdateTimer;
	TTimer *SystemTimer;
	void __fastcall TrayIconClick(TObject *Sender);
	void __fastcall ApplicationEventsMinimize(TObject *Sender);
	void __fastcall ApplicationEventsRestore(TObject *Sender);
	void __fastcall DataModuleCreate(TObject *Sender);
	void __fastcall ucmdExecuteMacro(TObject *Sender, TStrings *Msg);
	void __fastcall SystemTimerTimer(TObject *Sender);
	void __fastcall CheckUpdateTimerTimer(TObject *Sender);
private:
	bool m_bTerminating;
public:
	bool m_bAutoCheckUpdates;
public:	// User declarations
	IDCMapSys* m_pSys;
	IDCMapCoreEx* m_pCore;

public:
	bool ShowTrayIcon;
	bool MinimizeToTray;
	bool HiddenToTray;

	void InitSystem(IDCMapCoreUIEx* pCoreUI);
	void TerminateSystem();
	void SaveDB();
	void BringToTop();

public:		// User declarations
	void LoadPrefs();
	__fastcall TSystemModule(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSystemModule *SystemModule;
//---------------------------------------------------------------------------

class CSystemService : public IDCMapModule
{
public:
	CSystemService();
	int   DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);;
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
};

#endif
