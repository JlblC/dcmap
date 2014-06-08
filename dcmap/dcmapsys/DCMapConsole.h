#pragma once
#include "../dcmapsyslib.h"

class CDCMapConsole : public IDCMapConsole
{
public:
	CDCMapConsole(void);
public:
	~CDCMapConsole(void);
	void DCMAPAPI Execute(dcmapDCSTR text,IDCMapProcessRecordReseiver* pRsrv=0);
	void DCMAPAPI ExecuteFromWebScript(dcmapDCSTR text);
};

extern CDCMapConsole g_Console;
