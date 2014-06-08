#include "StdAfx.h"
#include "DCMapConsole.h"
#include "Utils.h"
#include "Parser.h"
#include <boost/regex.hpp>
#include <g_text.h>
#include "DCMapCore.h"
using namespace gsys;


CDCMapConsole g_Console;

CDCMapConsole::CDCMapConsole(void)
{
	
}

CDCMapConsole::~CDCMapConsole(void)
{
	
}

void DCMAPAPI CDCMapConsole::Execute(dcmapDCSTR text,IDCMapProcessRecordReseiver* pRsrv)
{
	if(!text)return;
	GetDCMapCore()->LockDataUpdate();
	
	CModuleParser prs;

    prs.Rights(PRSR_ALL);

	wiparsestream is(text);         
    prs.InitObject();
    prs.Init(&g_dcmapCore,0);
    
	prs.pRsrv = pRsrv;

	bool ok=true;
	CVarValue res;
	try
	{
		prs.Parse(true,is,res);
	}
    catch(parse_exception& e)
    {
        wstring err = L"line: "+to_wstr(e.line)+L" "+e.what();
        if(pRsrv)pRsrv->AddError(err.c_str());
    }
	catch(std::exception& e)
	{
		if(pRsrv)pRsrv->AddError(ToWide(e.what()).c_str());
		ok=false;
	}
	catch(...)
	{
		pRsrv->AddError(L"Internal Error");
		ok=false;
	}
	prs.pRsrv = 0;

	if(ok && pRsrv)pRsrv->AddRecord(res.to_string().c_str());
	GetDCMapCore()->UnlockDataUpdate();
}

void DCMAPAPI CDCMapConsole::ExecuteFromWebScript(dcmapDCSTR text)
{
	wstring res;

	boost::wcmatch rvars;
	boost::wregex re(L"<div class=\"log_line1\">([^<]*)<\\/div>");
	dcmapDCSTR end = text+wcslen(text);
	while(boost::regex_search(text,end,rvars,re))
	{
		res += rvars[1].str();
		text = rvars.suffix().first;
	}
	res = boost::regex_replace(res,boost::wregex(L"(&amp;)|(&lt;)|(&gt;)"),L"(?1&)(?2<)(?3>)",boost::match_default | boost::format_all);
	Execute(res.c_str());
}
