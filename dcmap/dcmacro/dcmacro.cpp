// dcmacro.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream> 
#include <string> 
using namespace std;

#include "../api/dcmapsys.h"
#include "../api/dcmapsys_util.h"

struct CConsoleFidder : public IDCMapProcessRecordReseiver
{
	bool DCMAPAPI AddMessage(dcmapWCSTR Text,int text_line)
	{
		wcout << Text << endl;
		return true;
	}
};
static CConsoleFidder s_Fidder;

int testdb()
{
	IDCMapDatabaseEx* pDB = dcmapsysCreateDatabase();
	if(!pDB)return 1;

	if(!pDB->Load(L"data/",0,0,DCMAP_DSLF_LAZY))return 1;

	IDCMapDataStorage* pPlanets = pDB->GetDataStorage("planets");
	if(!pPlanets)return 1;
	
	if(pPlanets->SelectAll())
	do
	{
		if(pPlanets->GetInteger(DCMDB_PLANET_S) > 90)
		{
			wcout << pPlanets->GetWideString(DCMDB_PLANET_CAPTION) << endl;
		}
	}
	while (pPlanets->Next());

	return 0;
}



int wmain(int argc, wchar_t* argv[])
{
	setlocale( LC_ALL, ".866" ); 

	if(argc <= 1)return 1;

	dcmapWCSTR szScriptName = argv[1];

	IDCMapSys* pSys = dcmapsysInit();
	if(!pSys)return 1;

	dcmapsysLocalize();
	dcmapsysInitModules();

	IDCMapCoreEx* pCore = dcmapsysCreateCore();
	if(!pCore)return 1;

	pCore->Configure();
	pCore->InitModules();
	pCore->LoadDatabase();
	pCore->PostInit();

	IDCMapConsole* pConsole= pCore->GetConsole();
	if(!pConsole)return 1;

	wstring ScriptText = dcmapLoadText(szScriptName);

	if(!ScriptText.empty()) pConsole->Execute(ScriptText.c_str(),&s_Fidder);

	pCore->SaveDatabase();

	dcmapsysTerminate();
	return 0;
}

