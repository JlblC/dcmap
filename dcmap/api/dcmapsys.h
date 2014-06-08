#ifndef __INCLUDED_dcmapsys_h__
#define __INCLUDED_dcmapsys_h__
#pragma once

#include "dcmap.h"

struct IDCMapWorkspaceEx;
struct IDCMapCoreEx;
struct IDCMapSysEx;
struct IDCMapFrameEx;

struct IDCMapSysEx : public IDCMapSys
{
	virtual void DCMAPAPI SetUI(IDCMapSysUI* pUI)=0;

	virtual IDCMapCoreEx* DCMAPAPI CreateCore(dcmapWCSTR szName,dcmapWCSTR szTemplate)=0;
	virtual IDCMapFrameEx* DCMAPAPI CreateFrame(IDCMapCore* pCore,dcmapWCSTR szName,dcmapWCSTR szTemplate)=0;
	virtual IDCMapWorkspaceEx* DCMAPAPI CreateWorkspace(IDCMapFrame* pFrame,IDCMapCore* pCore,dcmapWCSTR szName,dcmapWCSTR szTemplate)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapSysEx);

struct  IDCMapFrameEx : public IDCMapFrame
{
	virtual void DCMAPAPI SetUI(IDCMapFrameUI* pUI)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapFrameEx);


struct  IDCMapWorkspaceEx : public IDCMapWorkspace
{
	virtual void DCMAPAPI SetUI(IDCMapWorkspaceUI* pWrsp)=0;
	virtual void DCMAPAPI SetConfigFile(dcmapFCSTR Filename)=0;
	virtual void DCMAPAPI ConfigureFromFile(dcmapFCSTR Filename=0)=0;
	virtual void DCMAPAPI SaveConfig(dcmapFCSTR Filename=0)=0;
	virtual void DCMAPAPI PostInit()=0; 

	virtual void DCMAPAPI OnActivate()=0; 
	virtual void DCMAPAPI OnDeactivate()=0; 
};
DCMAP_SPEC_INTERFACE(IDCMapWorkspaceEx);


struct  IDCMapCoreUIEx : public IDCMapCoreUI
{
	virtual void DCMAPAPI DestroyModuleItems(IDCMapModule* Module)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapCoreUIEx);

struct IDCMapLocalStringsEx : public IDCMapLocalStrings
{
	virtual bool DCMAPAPI UpdateFrom(dcmapFCSTR szFileName)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapLocalStringsEx);

enum DCMAPDatabaseeSaveLoadFlags
{
	DCMAP_DBSLF_SAVE_ONLY_MODIFIED   = (1 << 0),
	DCMAP_DBSLF_USE_HARDLINKS        = (1 << 2),
};

struct  IDCMapDatabaseEx : public IDCMapDatabase
{
	virtual bool DCMAPAPI Load(dcmapFCSTR szLocation,dcmapFCSTR szTemplateLocation=0,int Flags=0,int LoadFlags=0)=0;
	virtual bool DCMAPAPI SaveWithBackup(dcmapFCSTR pDir,dcmapFCSTR pBackupDir,int NumBackups,int Flags, int SaveFlags)=0;
	virtual bool DCMAPAPI Save(dcmapFCSTR szLocation,int Flags=0, int SaveFlags=0)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapDatabaseEx);


struct  IDCMapCoreEx : public IDCMapCore
{
	virtual void DCMAPAPI ConfigureFromFile(dcmapFCSTR szFilename,
		dcmapFCSTR szDefaultsFilename,
		dcmapFCSTR szUpdateFilename)=0;

	virtual void DCMAPAPI InitModules()=0;

	virtual void DCMAPAPI ConfigureSystem()=0;

	virtual void DCMAPAPI SaveConfig(dcmapFCSTR Filename=0)=0;
	virtual void DCMAPAPI SetCoreUI(IDCMapCoreUIEx* pCore)=0;
	virtual void DCMAPAPI PostInit()=0;

	virtual void DCMAPAPI LoadDatabase()=0;
	virtual void DCMAPAPI SaveDatabase()=0;
	virtual void DCMAPAPI Configure()=0;

	virtual IDCMapLocalStringsEx* DCMAPAPI LoadSystemLocalStrings(dcmapFCSTR Filename)=0;

	virtual void DCMAPAPI SaveWithBackup(dcmapFCSTR pDir,dcmapFCSTR pBackupDir,int NumBackups)=0;

	virtual void DCMAPAPI SetCurrentWorkspace(IDCMapWorkspace* wsp)=0;

	virtual IDCMapModule* DCMAPAPI AddModule(IDCMapModule* pObj)=0;;
	virtual void DCMAPAPI LoadScriptModules(dcmapFCSTR szDir)=0;;
	virtual bool DCMAPAPI LoadScriptModule(dcmapFCSTR szFileName)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapCoreEx);

extern "C" // Dll export functions
{
	DCMAPSYSEXPORT IDCMapSysEx* DCMAPLIBAPI dcmapsysInit(dcmapDWORD ver = DCMAP_API_VERSION);
	DCMAPSYSEXPORT void DCMAPLIBAPI dcmapsysLoadPlugins(dcmapFCSTR szDir);
	DCMAPSYSEXPORT void DCMAPLIBAPI dcmapsysInitModules();
	DCMAPSYSEXPORT void DCMAPLIBAPI dcmapsysTerminate();

	DCMAPSYSEXPORT IDCMapLocalStringsEx* DCMAPLIBAPI dcmapsysLocalize(dcmapFCSTR szDir=0);

	DCMAPSYSEXPORT IDCMapCoreEx* DCMAPLIBAPI dcmapsysCreateCore();
	DCMAPSYSEXPORT IDCMapDatabaseEx* DCMAPLIBAPI dcmapsysCreateDatabase();
};

#endif // __INCLUDED_dcmapsys_h__