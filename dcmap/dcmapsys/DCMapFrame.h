#pragma once
#include "../dcmapbase.h"
#include "ModuleManager.h"


class CDCMapFrame :	public ModuleManagerImpl<IDCMapFrameEx>
{
	IDCMapFrameUI* m_pUI;
public:
	CDCMapFrame(void);
	~CDCMapFrame(void);
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);

	IDCMapFrameUI* DCMAPAPI GetUI() { return m_pUI; }
	void DCMAPAPI SetUI(IDCMapFrameUI* val) { m_pUI = val; }

	virtual void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
};

