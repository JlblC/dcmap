//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "dcmapsyslib.h"

#include "System.h"
#include "Interface.h"
#include "main.h"
#include "BaseModule.h"
#include "Splash.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSystemModule *SystemModule=0;

IDCMapSys* g_pDCMapSys=0;


static CAutoReg<CSystemService> sWrap("PCoreDaemon","System");

CSystemService::CSystemService()
{

}

bool  DCMAPAPI CSystemService::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{

	return true;
}

int   DCMAPAPI CSystemService::OnMessage( int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender )
{
	if(Msg == DCMM_CONFIG_CHANGED)
	{
		 SystemModule->LoadPrefs();
	}
	return 0;
}

int DCMAPAPI CSystemService::ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
{
	if(type == DCMEXEC_QUERY) return DCMEXEC_CALL;

	return DCMEXEC_FAIL;
}


//---------------------------------------------------------------------------
__fastcall TSystemModule::TSystemModule(TComponent* Owner)
	: TDataModule(Owner)
{
	HiddenToTray = false;
}

void TSystemModule::LoadPrefs()
{
	DCMapConfigWrapper  Config(m_pCore->GetConfig());

	Config.section("System");

	Config["ShowTrayIcon"] >>  ShowTrayIcon;
	Config["MinimizeToTray"] >>  MinimizeToTray;

	Config.section("Core");

	Config["AutoCheckUpdates"] >> m_bAutoCheckUpdates;

	TrayIcon->Visible = ShowTrayIcon || HiddenToTray;
}

void TSystemModule::BringToTop()
{
	if(!MainForm->Visible)
	{
		HiddenToTray = false;
		MainForm->Show();
	}
	Application->Restore();
	Application->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TSystemModule::TrayIconClick(TObject *Sender)
{
	if(m_bTerminating)return;
	if(!MainForm)return;

	if(MainForm->Visible)
	{
		Application->Minimize();
		MainForm->Hide();
		HiddenToTray = true;
	}
	else
	{
		HiddenToTray = false;
		MainForm->Show();
		Application->Restore();
		Application->BringToFront();
	}
}
//---------------------------------------------------------------------------

void __fastcall TSystemModule::ApplicationEventsMinimize(TObject *Sender)
{
	if(MinimizeToTray)
	{
		MainForm->Hide();
		TrayIcon->Visible=true;
		HiddenToTray = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TSystemModule::ApplicationEventsRestore(TObject *Sender)
{
		HiddenToTray = false;
		TrayIcon->Visible=ShowTrayIcon;
}
//---------------------------------------------------------------------------

void __fastcall TSystemModule::DataModuleCreate(TObject *Sender)
{
	HiddenToTray = false;
	m_bTerminating = false;

	m_pSys = GetDCMapSys();
}

//---------------------------------------------------------------------------
void TSystemModule::SaveDB()
{
	String Dir = GetAppPath()+"data\\";
	String Backup = GetAppPath()+"backup\\";

	m_pCore->SaveWithBackup(Dir.w_str(),Backup.w_str(),3);
}

void TSystemModule::InitSystem(IDCMapCoreUIEx* pCoreUI)
{
	dcmapsysLoadPlugins((GetAppPath()+L"plugins\\").w_str());

	m_pSys->GetScriptSystem()->LoadScriptModuleFromFile((GetAppPath()+L"dcmap.dcm").w_str());
	m_pSys->GetScriptSystem()->LoadScriptModuleFromFile((GetAppPath()+L"dcmapdev.dcm").w_str());

	m_pCore = dcmapsysCreateCore();
	m_pCore->ConfigureFromFile(L"DCMap.ini",
								 L"cfgdef.ini",
								 L"cfgupd.ini");

	m_pCore->ConfigureSystem();
	LoadPrefs();

	m_pCore->SetCoreUI(pCoreUI);
	dcmapsysInitModules();
	m_pCore->InitModules();

	m_pCore->LoadScriptModules((GetAppPath()+L"plugins\\").w_str());

	String DataPath = GetAppPath()+L"data\\";
	m_pCore->LoadDatabaseFrom(DataPath.w_str());

	GetDCMapCore()->PostInit();

   	IDCMapFrameEx* Frame = GetDCMapSys()->CreateFrame(m_pCore,0,0);
}

void TSystemModule::TerminateSystem()
{
	m_bTerminating = true;
	if(SplashForm)
	{
		SplashForm->StatusLabel->Caption = L"Saving Database...";
		Application->ProcessMessages();
	}
	SaveDB();
	if(SplashForm)
	{
		SplashForm->StatusLabel->Caption = L"Saving Config...";
		Application->ProcessMessages();
	}
	m_pCore->Destroy();
	m_pCore->SaveConfig((GetAppPath()+"DCMap.ini").w_str());
}

void __fastcall TSystemModule::ucmdExecuteMacro(TObject *Sender, TStrings *Msg)
{
	String szCmd = Msg->Text;
}
//---------------------------------------------------------------------------

void __fastcall TSystemModule::SystemTimerTimer(TObject *Sender)
{
	SystemTimer->Tag++;
	if(SystemTimer->Tag < 2)return;

	dcmapLRESULT res = m_pCore->BroadcastMessage(DCMM_SYSTEM_TIMER_TICK,
				SystemTimer->Tag-1,SystemTimer->Interval,0);
}
//---------------------------------------------------------------------------

void __fastcall TSystemModule::CheckUpdateTimerTimer(TObject *Sender)
{
	CheckUpdateTimer->Tag++;
	if(CheckUpdateTimer->Tag < 2)return;
	CheckUpdateTimer->Enabled = false;
	if(!m_bAutoCheckUpdates)return;

	DCMapConfigWrapper cfg(m_pCore->GetConfig());
	unsigned interval=60;

	cfg.section("WebUpdate");

	dcmapTIME upd=0;
	cfg["UpdateTime"] >> upd;
	cfg["UpdateTime"] << dcmapGetTime()+360;

	if(upd > dcmapGetTime())
	{
		interval = 2;
	}
	else
	{
		dcmapLRESULT res = m_pCore->BroadcastMessage(DCMM_CHECK_WEB_UPDATES,Tag-1,0,0,
								DCMAP_BRADCAST_STOP_ON_FAIL|DCMAP_BRADCAST_STOP_ON_SUCCESS);

		if(res & DCMM_FAIL) interval = 45;
		else if(res & DCMM_SUCCESS) interval = 60*24;
		else interval = 60*6;

		cfg.section("WebUpdate");
		cfg["UpdateTime"] << dcmapGetTime()+interval*60;
	}
	CheckUpdateTimer->Interval = (interval*60*1000)+20000;
	CheckUpdateTimer->Enabled = true;
}
//---------------------------------------------------------------------------

