#pragma once


class CScriptSystem : public IDCMapScriptSystem
{
public:
	IDCMapScriptModule* DCMAPAPI LoadScriptModuleFromFile(dcmapFCSTR szFileName,IDCMapCore* pCore=0,int Falgs=0);
	IDCMapScriptModule* DCMAPAPI LoadScriptModuleFromText(dcmapWCSTR szText,IDCMapCore* pCore=0,int Falgs=0);
	IDCMapVarValue* DCMAPAPI CreateVarValue();
	void DCMAPAPI Destroy(){};

	dcmapWCSTR DCMAPAPI ProcessFormatedString(dcmapWCSTR Format,IDCMapVarValue* Values,int Flags);
	dcmapWCSTR DCMAPAPI ProcessFormatedStringDB(dcmapWCSTR Format,IDCMapDataSource* pDataStorage,dcmapDBREF const* pDBRef, IDCMapVarValue* Values,int Flags);

	bool DCMAPAPI CallGlobalProc(dcmapWCSTR Proc,IDCMapVarValue* Params,IDCMapVarValue* Result);
public:
	void Init();
	void Terminate();
};

extern CScriptSystem g_ScriptSystem;