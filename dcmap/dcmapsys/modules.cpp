#include "StdAfx.h"
#include "modules.h"
#include "keywords.h"
#include "ScriptModule.h"

CModuleAutoRegMan* g_pModuleRegMan=0;

REG_MODULE(CModuleParser,module);
REG_MODULE(CObjectParser,object);

class CCoreDaemonReg : public CModuleReg<CModuleParser>
{
	bool RegisterModule(wstring const& name,CVarValue const& module,CVarValue const& params,CBasicBlockParser* cnt)
	{
		if(cnt->ScriptModule())
		{
			cnt->ScriptModule()->RegisterModule(module,"PCoreDaemon",WideToString(name));
		}
		return true;
	}
};

REG_MODULEX(CCoreDaemonReg,core_daemon)
REG_MODULEX(CCoreDaemonReg,core_demon)


struct CModuleKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;

	wstring modname;
	wstring modclass;
	ModuleDef mdef;
	if(is.peek() == L'$')is.ignore();
	wchar_t ps = is.peek();

	if(wchar_classify::alpha_ext(ps))
	{
		is >> wistream_keyword_ext(modname);
	}
	else throw parse_exception(is,PERR_UNEXPECTED_SYMBOL,wstring()+ps);

	is >> wsp_nl_ucmt_scip;
	ps = is.peek();

	CVarValue RegParams;

	ModuleRegistrator* pModReg = 0;

	if(ps == ':')
	{
		is.ignore();
		is >> wsp_nl_ucmt_scip >> wistream_keyword_ext(modclass) >> wsp_nl_ucmt_scip;
		ps = is.peek(); 

		pModReg = CModuleAutoRegMan::GetModule(modclass);
	}
	else 
	{
		pModReg = CModuleAutoRegMan::GetModule(kw);;
	}

	if(!pModReg)
	{
		throw parse_exception(is,PERR_CUSTOM,L"invalid object class type");
	}

	mdef.creator = pModReg->GetCreator();


	if(ps == ';') throw parse_exception(is,PERR_UNEXPECTED_SYMBOL,wstring()+ps);

	if(active)
	{
		mdef.stream=&is;
		mdef.pos = is.tellg(); 
		mdef.context=0;
		mdef.Acquire(cnt->GetObjectParser());
	}
	CVarValue tmpv;
	cnt->ParseBlock(false,is,tmpv);

	if(active)
	{
		CVarValue rv;
		rv.set_module(mdef);
		if(pModReg->RegisterModule(modname,rv,RegParams,cnt))
		{
			cnt->SetLocalVariable(true,modname,rv,res);
		}
	}

	return true;
}};

static KeywordAutoReg<CModuleKW> sCModuleKW(L"module");
static KeywordAutoReg<CModuleKW> s2CModuleKW(L"object");




void ModuleDef::Execute(CVarValue const& param,CVarValue & res)
{
	if(!creator)return;
	CVarValue Result;
	CObjectParser* pModule = creator();

	int callerpos = stream->tellg();
	stream->seekg(pos,ios_base::beg);

	try
	{
		pModule->Parent(context);
		pModule->SetDefaultValue(param);
		if(!pModule->InitObject())
		{
			delete pModule;
			throw parse_exception(*stream,PERR_INVALID_MODULE);
		}
		pModule->Parse(true,*stream,Result,true);
	}
	catch (return_exception e){delete pModule;throw parse_exception(*stream,PERR_INVALID_BREAK);}
	catch (break_exception e){delete pModule;throw parse_exception(*stream,PERR_INVALID_BREAK);}
	catch (continue_exception e){delete pModule;throw parse_exception(*stream,PERR_INVALID_CONTINUE);}
	stream->seekg(callerpos,ios_base::beg);

	res.set_object(pModule);
}

//////////////////////////////////////////////////////////////////////////
