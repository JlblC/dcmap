#include "stdafx.h"
#include "platform.h"
#include "dcmapsystem.h"
#include "dcmapcore.h"
#include "time.h"
#include "LocalStrings.h"
#include <boost/format.hpp>
#include "utils.h"
#include "g_file_util.h"

#pragma comment(lib, "GdiPlus.lib")

HINSTANCE g_hInstance;


static Gdiplus::GdiplusStartupInput gdiplusStartupInput; 
static ULONG_PTR gdiplusToken; 

bool PlatformInit()
{
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
#ifdef _DEBUG
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
	_CrtSetDbgFlag( tmpFlag );
#endif 
	return true;
}

void PlatformTerminate()
{
	Gdiplus::GdiplusShutdown(gdiplusToken);
}


BOOL APIENTRY DllMain( HMODULE hModule,
					  dcmapDWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	g_hInstance = hModule;
	return TRUE;
}

typedef dcmapINT (DCMAPDLLAPI *InitPluginProc)(IDCMapSys* pSys,IDCMapLocalStrings* pLocalStrings,dcmapDWORD iApiVer);

bool InitalizePlugin(dcmapFCSTR szFileName,dcmapFCSTR szDir,dcmapFCSTR szName,InitPluginProc proc,bool module)
{
	sysstring ModuleName = gsys::StripEXT(fstring(szName));

	g_dcmapSys.StartPluginLoad(szFileName,ModuleName.c_str(),szDir,module);
	int r = true;
	try
	{
		if(!proc(g_pDCMapSys,g_pLocalStrings,DCMAP_API_VERSION))r = false;
	}
	catch (...)
	{
		r = false;
	}
	g_dcmapSys.EndPluginLoad();

	if(!r) return false;
	g_pLocalStrings->UpdateFrom((szDir+ModuleName+L".lng").c_str());
	return true;
}

bool IsDirectory(dcmapFCSTR File)
{
	return GetFileAttributes(File) & FILE_ATTRIBUTE_DIRECTORY;
}

bool LoadPluginModule(dcmapFCSTR szFile,dcmapFCSTR szDir,dcmapFCSTR szName,bool bModule)
{
	bool r = true;
	HMODULE lib = 0;
	try
	{
		if(bModule)SetCurrentDirectory(szDir);
		lib = LoadLibraryEx(szFile,NULL,bModule?LOAD_WITH_ALTERED_SEARCH_PATH:0);
		if(!lib)return false;
		InitPluginProc pInit = (InitPluginProc)GetProcAddress(lib,"InitPluginProc");
		if(!pInit)pInit = (InitPluginProc)GetProcAddress(lib,"_InitPlugin");
		if(!pInit)
		{
			r = false;
		}
		else if(!InitalizePlugin(szFile,szDir,szName,pInit,bModule))
		{
			r = false;
		}	
	}
	catch (...)
	{
		r = false;
	}
	if(!r)
	{
		if(lib) FreeLibrary(lib);
	}
	return r;
}


void LoadPlugins(dcmapFCSTR szDir)
{
	VecFiles vecFiles;
	sysstring loc = sysstring(szDir)+L"*.dll";
	ListDirectory(loc.c_str(),vecFiles);

	for(int i=0;i<vecFiles.size();i++)
	{
		sysstring str = szDir+vecFiles[i];
		LoadPluginModule(str.c_str(),szDir,gsys::StripEXT(vecFiles[i]).c_str(),false);
	}
}


void LoadModules(dcmapFCSTR szDir)
{
	VecFiles vecFiles;
	sysstring loc = sysstring(szDir)+L"*";
	ListDirectory(loc.c_str(),vecFiles);

	for(int i=0;i<vecFiles.size();i++)
	{
		sysstring& Filename = vecFiles[i];
		if(Filename.empty())continue;
		if(Filename[0]=='.')continue;
		sysstring Path = szDir+Filename;

		if(IsDirectory(Path.c_str())) // load module from directory
		{
			sysstring ModuleDir = Path+L"\\";
			sysstring ModuleName;
#ifdef _DEBUG
			ModuleName = ModuleDir+Filename+L".ddll";
			if(!gsys::FileExists(ModuleName))
#endif // _DEBUG
			{
				ModuleName = ModuleDir+Filename+L".dll";
				if(!gsys::FileExists(ModuleName))continue; // no module file
			}

			LoadPluginModule(ModuleName.c_str(),ModuleDir.c_str(),Filename.c_str(),true);
		}
	}
	SetCurrentDirectory(GetAppStartupDir().c_str());
}

bool DetectWine()
{
	HKEY hkResult;
	if(RegOpenKey(HKEY_CURRENT_USER,L"Software\\Wine",&hkResult)==ERROR_SUCCESS)
	{
		RegCloseKey(hkResult);
		return true;
	}
	if(RegOpenKey(HKEY_LOCAL_MACHINE,L"Software\\Wine",&hkResult)==ERROR_SUCCESS)
	{
		RegCloseKey(hkResult);
		return true;
	}
	return false;
}

void GetSystemParams(DCMapSystemOptions* sysparams)
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	OSVERSIONINFOEX osver;	
	osver.dwOSVersionInfoSize = sizeof(osver);
	GetVersionEx((OSVERSIONINFO*)&osver);

	sysparams->bVista = (osver.dwMajorVersion >= 6);
	sysparams->bAero = false;

	if(sysparams->bVista)
	{
		HMODULE hmod = LoadLibrary(L"dwmapi.dll");
		if(hmod)
		{
			typedef HRESULT  (STDAPICALLTYPE *DwmIsCompositionEnabledType)(BOOL *pfEnabled);

			DwmIsCompositionEnabledType DwmIsCompositionEnabled = 
				(DwmIsCompositionEnabledType)GetProcAddress(hmod,"DwmIsCompositionEnabled");

			if(DwmIsCompositionEnabled)
			{
				BOOL fEnabled=false;
				if(DwmIsCompositionEnabled(&fEnabled) == S_OK)
				{
					sysparams->bAero = fEnabled;
				}
			}
		}

	}
};




#ifdef _MANAGED
#pragma managed(pop)
#endif