#pragma once
#include "../dcmapsyslib.h"
#include "Parser.h"

class CScriptModule : public CModuleParser
{
	wstring buffer;
	wiparsestream stream;
	void Execute();
	void Execute(CVarValue val);
public:
	CVarValue AddMenu(CVarValue const& params);
protected:
	bool ParseCustomKeyword(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt);
public:
    CScriptModule();
	bool LoadSource(dcmapWCSTR szBuffer,dcmapFCSTR szFileName=0);
	bool LoadFromFile(dcmapFCSTR Filename);

	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	void AddContextMenu(CVarValue const& params);

	bool RegisterModule(CVarValue const& mod,string mclass,string mname);

	CScriptModule* ScriptModule() { return this; }

};