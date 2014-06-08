//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include "dcmapsyslib.h"
#include <tlhelp32.h>
#include "utils.h"
#include "basemodule.h"
#include "splash.h"
#include "UISystem.h"

//---------------------------------------------------------------------------
USEFORM("PlanetContensProp.cpp", PlanetGarrisonFrame); /* TFrame: File Type */
USEFORM("WebProc.cpp", WebModule); /* TDataModule: File Type */
USEFORM("PlanetInfo.cpp", PlanetInfoFrame); /* TFrame: File Type */
USEFORM("BaseDataEdit.cpp", BaseEditForm);
USEFORMNS("WebBrowser.pas", Webbrowser, WebBrowserFrame); /* TFrame: File Type */
USEFORM("ViewLayout.cpp", ViewLayoutFrame); /* TFrame: File Type */
USEFORM("System.cpp", SystemModule); /* TDataModule: File Type */
USEFORM("About.cpp", AboutForm);
USEFORM("FixedFilterDB.cpp", FixedFilterDBFrame); /* TFrame: File Type */
USEFORM("FixedFilter.cpp", FixedFilterFrame); /* TFrame: File Type */
USEFORM("GraphHelper.cpp", GraphHelperForm);
USEFORM("FleetSidebar.cpp", FleetSideFrame); /* TFrame: File Type */
USEFORM("PlanetProp.cpp", PlanetParamFrame); /* TFrame: File Type */
USEFORM("LayerProp.cpp", LayerEditFrame); /* TFrame: File Type */
USEFORM("SizeableSideModule.cpp", SizeableSideFrame); /* TFrame: File Type */
USEFORM("ListEdit.cpp", ListEditForm);
USEFORM("GraphicToolbar.cpp", GraphicToolbarFrame); /* TFrame: File Type */
USEFORM("PlanetBuildingsProp.cpp", PlanetBuildingsFrame); /* TFrame: File Type */
USEFORM("BaseFormModule.cpp", BaseModuleForm);
USEFORM("PlanetEconomyInfo.cpp", PlanetEconomyInfoFrame); /* TFrame: File Type */
USEFORM("WebView.cpp", WebViewFrame); /* TFrame: File Type */
USEFORM("Console.cpp", ConsoleForm);
USEFORM("RaceAdvProp.cpp", RaceAdvPropFrame); /* TFrame: File Type */
USEFORM("TabsFormModule.cpp", TabsFrameForm);
USEFORM("FakeMain.cpp", FakeMainForm);
USEFORM("Input.cpp", InputDialog);
USEFORM("GameImportSelect.cpp", GameImportSelectForm);
USEFORM("FilterSelect.cpp", FilterSelectForm);
USEFORM("RegExpImpPrefs.cpp", RegExpImpPrefsFrame); /* TFrame: File Type */
USEFORM("DataImport.cpp", DataImportForm);
USEFORM("GeoValue.cpp", GeoValueFrame); /* TFrame: File Type */
USEFORMNS("WebBase.pas", Webbase, WebBaseFrame); /* TFrame: File Type */
USEFORM("DipProp.cpp", DipEditPropFrame); /* TFrame: File Type */
USEFORM("BasePrefs.cpp", BasePrefsFrame); /* TFrame: File Type */
USEFORM("ScriptEditModule.cpp", ScriptEditFrame); /* TFrame: File Type */
USEFORM("ApperancePrefs.cpp", ApperancePrefsFrame);
USEFORM("MapLayers.cpp", MapLayersFrame); /* TFrame: File Type */
USEFORM("PlayerToolbar.cpp", PlayerToolbarFrame); /* TFrame: File Type */
USEFORM("PropPage.cpp", PropPageFrame); /* TFrame: File Type */
USEFORM("RaceProp.cpp", RaceEditPropFrame); /* TFrame: File Type */
USEFORM("DataEdit.cpp", DataEditForm);
USEFORM("ColorSchemeToolbar.cpp", ColorSchemeToolbarFrame); /* TFrame: File Type */
USEFORM("PlayerDataLoadProp.cpp", PlayerAdvPropFrame); /* TFrame: File Type */
USEFORM("Splash.cpp", SplashForm);
USEFORM("DBPrefs.cpp", DBFrefsFrame); /* TFrame: File Type */
USEFORM("Preferences.cpp", PreferencencesForm);
USEFORM("BaseViewPopup.cpp", BaseViewPopupForm);
USEFORM("ViewSetup.cpp", ViewSetupForm);
USEFORM("PasswordDialog.cpp", PasswordDialogForm);
USEFORM("Interface.cpp", InterfaceModule); /* TDataModule: File Type */
USEFORM("UIDialogForm.cpp", DialogForm);
USEFORM("CoordsInfoToolbar.cpp", PlanetInfoToolbarFrame); /* TFrame: File Type */
USEFORM("SecurityPrefs.cpp", SecurityPrefsFrame); /* TFrame: File Type */
USEFORM("SystemPrefs.cpp", SystemPrefsFrame); /* TFrame: File Type */
USEFORM("UIHTMLFrame.cpp", HTMLFrame); /* TFrame: File Type */
USEFORM("MapColorPref.cpp", MapColorPrefFrame); /* TFrame: File Type */
USEFORM("IconsProp.cpp", IconsPropFrame); /* TFrame: File Type */
USEFORM("BaseProp.cpp", BaseEditModuleFrame); /* TFrame: File Type */
USEFORM("ColoringProp.cpp", ColoringPropframe); /* TFrame: File Type */
USEFORM("FleetProp.cpp", FleetEditFrame); /* TFrame: File Type */
USEFORM("PlayerProp.cpp", PlayerEditPropFrame); /* TFrame: File Type */
USEFORM("BaseModule.cpp", BaseModuleFrame); /* TFrame: File Type */
USEFORM("FleetColorPrefs.cpp", FleetColorPrefsFrame); /* TFrame: File Type */
USEFORM("main.cpp", MainForm);
USEFORM("Progress.cpp", ProgressForm);
USEFORM("NetworkPrefs.cpp", NetSettingsFrame); /* TFrame: File Type */
USEFORM("GraphicsProp.cpp", GraphicsEditFrame); /* TFrame: File Type */
USEFORM("DipColorPref.cpp", DipColorPrefFrame); /* TFrame: File Type */
USEFORM("BaseFrameFormModule.cpp", BaseFrameForm);
USEFORM("DataExport.cpp", DataExportForm);
USEFORM("GridDataEdit.cpp", GridDataEditForm);
USEFORM("OrgProp.cpp", OrgPropFrame); /* TFrame: File Type */
USEFORM("CorruptionColorPref.cpp", CorruptionPrefFrame); /* TFrame: File Type */
//---------------------------------------------------------------------------
IDCMapSysEx* DCMapSys;

static int CountRunnings()
{
	HANDLE h;
	PROCESSENTRY32W pe;

	h=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
	pe.dwSize=sizeof(pe);
	Process32FirstW(h,&pe);

	String szModule = GetModuleName(0);

	int Count=0;
	while(true)
	{
		if(!_wcsicmp(L"dcmap.exe",pe.szExeFile))
		{
			  HANDLE h;
			  MODULEENTRY32 mdl;
			  h=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pe.th32ProcessID);
			  mdl.dwSize=sizeof(MODULEENTRY32);
			  Module32First(h,&mdl);
			  while(1)
			  {
				if(szModule == mdl.szExePath)
				{
					Count++;
					break;
				}
				if (!Module32Next(h,&mdl)) break;
			  }
			  CloseHandle(h);
		}
		if (!Process32NextW(h,&pe)) break;
	  }
	  return Count;
}

static bool Initialize()
{
	if(CountRunnings()>1)
	{
		ClipboardPutW(L"!DCMapCmd!{bring_to_top}");
		return false;
	}

	DCMapSys = dcmapsysInit();
	if(!DCMapSys)return false;
	g_pDCMapSys = DCMapSys;

	DCMapSys->SetUI(&SysUI);

	String AppPath = GetAppPath();
	// Create all system directories
	ForceDirectories(AppPath+"data");
	ForceDirectories(AppPath+"backup");
	ForceDirectories(AppPath+"plugins");
	ForceDirectories(AppPath+"sessions");
	ForceDirectories(AppPath+"templates");
	ForceDirectories(AppPath+"skins");
	ForceDirectories(AppPath+"scripts");

	String FileName = AppPath+"update.exe";;
	if(FileExists(FileName))DeleteFile(FileName);

	g_pLocalStrings = dcmapsysLocalize((AppPath+"dcmap.lng").w_str());

	g_pLocalStrings->UpdateFrom((AppPath+"dcmapdev.lng").w_str());
	g_pLocalStrings->UpdateFrom((AppPath+"dcmap.user.lng").w_str());

	RegisterModules(g_pDCMapSys);
	return true;
}

//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    if(!Initialize())return 0;

	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->ShowMainForm = false;

		Application->Title = "DCMap";
		Application->CreateForm(__classid(TSystemModule), &SystemModule);
		Application->CreateForm(__classid(TInterfaceModule), &InterfaceModule);
		Application->CreateForm(__classid(TFakeMainForm), &FakeMainForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
