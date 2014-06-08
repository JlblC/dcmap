#include "StdAfx.h"
#include "Parser.h"
#include "utils.h"
#include <boost/regex.hpp>
#include <boost/scoped_array.hpp>

#include <g_std_file.h>
#include <g_file_util.h>
#include <g_text.h>
#include <hash_map>
#include "Keywords.h"
#include "Operators.h"

using namespace std;
using namespace gsys;




#define KEYWORD_RES \
	is >> wsp_nl_ucmt_scip;\
	if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,res);\
	else cnt->ParseExpression(active,is,res);\

CVarValue CBaseKeyword::Execute( CVarValue const& param,CBasicBlockParser* cnt)
{
	return CVarValue();
};
CVarValue CBaseKeyword::Execute( CVarValue const& param,CBasicBlockParser* cnt,wiparsestream &is)
{
	return Execute(param,cnt);
};
bool CBaseKeyword::Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt)
{
	is >> wsp_nl_ucmt_scip;
	bool fnt=false;
	if(is.peek() == L'('){cnt->ParseBasicBlock(active,is,res);fnt=true;} // function syntax
	else 
	{
		if(is.peek()=='='){is.ignore();is >> wsp_nl_ucmt_scip;} // assigment syntax
		cnt->ParseExpression(active,is,res);
	}
	if(active)
	{
        m_pKW = &kw;
		CVarValue param;
		res = Execute(res,cnt,is);
        m_pKW = 0;
	}
	if(!expr && fnt)cnt->ContinueExpression(active,is,res);
	return true;
}

struct CMinKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	int n = param.size();
	CVarValue mv=param[0];
	for(int i=1;i<n;i++)
	{
		if(param[i]<mv) mv=param[i];
	}
	return CVarValue(mv);
}};static KeywordAutoReg<CMinKW> sCMinKW(L"min");

struct CMaxKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	int n = param.size();
	CVarValue mv=param[0];
	for(int i=1;i<n;i++)
	{
		if(param[i]>mv) mv=param[i];
	}
	return CVarValue(mv);
}};static KeywordAutoReg<CMaxKW> sCMaxKW(L"max");


struct CGetServiceKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	IDCMapModule* mod = GetDCMapCore()->GetService(StrW2C(param.as_string()).c_str());

	return CVarValue(mod);
}};static KeywordAutoReg<CGetServiceKW> sCGetServiceKW(L"get_service");

struct CServicesKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	CVarValue cnd; 
	if(is.peek() != L'.')
	{
		throw parse_exception(is,PERR_UNEXPECTED_SYMBOL,is.peek());
	}
	is.ignore();
	wstring skw;
	is >> wsp_nl_ucmt_scip >> wistream_keyword_ext(skw) >> wsp_nl_ucmt_scip;

	if(active) 
		res = GetDCMapCore()->GetService(StrW2C(skw).c_str());

	cnt->ContinueExpression(active,is,res);
	return true;
}};static KeywordAutoReg<CServicesKW> sCServicesKW(L"services");  


struct CCoreKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;

	if(active)
	{
		IDCMapCore* pcore = cnt->GetModule()->GetCore();
		if(!pcore)pcore = GetDCMapCore();
		res = CVarValue(pcore);
	}
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}};static KeywordAutoReg<CCoreKW> sCCoreKW(L"core");  

struct CWorkspaceKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	if(active)
	{
		IDCMapWorkspace* workspace = cnt->GetModule()->GetCurrentWorkspace();
		if(!workspace)throw parse_exception(is,PERR_INVALID_CONTEXT);
		res = CVarValue(workspace);
	}
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}};static KeywordAutoReg<CWorkspaceKW> sCWorkspaceKW(L"workspace");  

struct CSysKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	if(active)
	{
		res = CVarValue(g_pDCMapSys);
	}
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}};static KeywordAutoReg<CSysKW> sCSysKW(L"sys"); 


struct CDataTablesKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	CVarValue cnd; 
	if(is.peek() != L'.')
	{
		throw parse_exception(is,PERR_UNEXPECTED_SYMBOL,is.peek());
	}
	is.ignore();
	wstring skw;
	is >> wsp_nl_ucmt_scip >> wistream_keyword_ext(skw) >> wsp_nl_ucmt_scip;

	if(active) 
		res = GetDCMapCore()->GetDataSource(StrW2C(skw).c_str());

	cnt->ContinueExpression(active,is,res);
	return true;
}};static KeywordAutoReg<CDataTablesKW> sCDataTablesKW(L"datatables");  


struct CIfKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	CVarValue cnd; 

	cnt->ParseExpression(active,is,cnd);
	bool doit = cnd.to_bool();
	cnt->ifresult = doit?1:-1;
	is >> wsp_nl_ucmt_scip;
	if(is.peek() == L'{')
	{
		cnt->ParseBlock(active&&doit,is,res);
		if(!expr)cnt->ContinueExpression(active,is,res);
	}
	else cnt->ParseExpression(active&&doit,is,res);
	if(!doit)res.set_nil();
	return true;
}}static IfKW;


struct CNumToStrFKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	dcmapCHAR buf[64];
	if(param[1].is_nil())
	{
		sprintf(buf,"%.16g",param.to_real());
	}
	else
	{
		sprintf(buf,ToStr(param[1].to_string()).c_str(),param[0].to_real());
	}
	return CVarValue(ToWStr(buf));
}};static KeywordAutoReg<CNumToStrFKW> CNumToStrFKW(L"ftoa");


struct CRoundKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	double r;
	if(param[1].is_nil())
	{
		r = floor(param[0].as_real()+0.5); 
	}
	else
	{
		double mod = pow(10.0,(int)param[1].as_real());

		r = floor(param[0].as_real()*mod+0.5)/mod; 
	}
	return CVarValue(r);
}};static KeywordAutoReg<CRoundKW> sCRoundKW(L"round");

struct CFloorKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	double r;
	if(param[1].is_nil())
	{
		r = floor(param[0].as_real()); 
	}
	else
	{
		double mod = pow(10.0,(int)param[1].as_real());
		r = floor(param[0].as_real()*mod)/mod; 
	}
	return CVarValue(r);
}};static KeywordAutoReg<CFloorKW> sCFloorKW(L"floor");

struct CCeilKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	double r;
	if(param[1].is_nil())
	{
		r = ceil(param[0].as_real()); 
	}
	else
	{
		double mod = pow(10.0,(int)param[1].as_real());
		r = ceil(param[0].as_real()*mod)/mod; 
	}
	return CVarValue(r);
}};static KeywordAutoReg<CCeilKW> sCCeilKW(L"ceil");


struct CStrLenKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
    if(param[0].type() == VT_STR)
    {
        return CVarValue(param.to_string().length());
    }
    return CVarValue();
}};static KeywordAutoReg<CStrLenKW> sCStrLenKW(L"strlen");

struct CSizeOfKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
    return CVarValue(param.size());
}};static KeywordAutoReg<CSizeOfKW> sCSizeOfKW(L"sizeof");


struct CElseKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	bool doit = cnt->ifresult < 0;
	if(is.peek() == L'{')
	{
		cnt->ParseBlock(active&&doit,is,res);
		if(!expr)cnt->ContinueExpression(active,is,res);
	}
	else cnt->ParseExpression(active&&doit,is,res);
	if(!doit)res.set_nil();
	return true;
}}static ElseKW;

struct CForeachKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;

	wchar_t ps = is.peek();
	wstring vkw;

	CVarValue dta;
	if(wchar_classify::alpha_ext(ps))
	{
		wstring mkw;
		is >> wistream_keyword_ext(mkw)>> wsp_nl_ucmt_scip;
		if(mkw != L"my")
		{
			cnt->ParseKeyword(active,mkw,is,dta);
		}
		else
		{
			is >> wsp_nl_ucmt_scip;
			ps = is.peek();
			if(ps == L'$'){is.ignore();}
			is >> wistream_keyword_ext(vkw)>> wsp_nl_ucmt_scip;
			ps = is.peek();
			if(ps == L'(')
			{
				cnt->ParseBasicBlock(active,is,dta);
			}
			else cnt->ParseExpression(active,is,dta);
		}
	}
	else if(ps == L'(')
	{
		cnt->ParseBasicBlock(active,is,dta);
	}
	else cnt->ParseExpression(active,is,dta);

	ps = is.peek();

	CurcleBlockDef cdef;
	cnt->BeginCircleBlock(is,cdef);
	if(active)
	{
		if(dta.type() == VT_HASH)
		{
			CVarValueMap const& hash = dta.get_hash();
			_const_foreach(CVarValueMap,hash,it)
			{
				CBasicBlockParser* block = cnt->PrepareCircleBlock(is,cdef);
				CVarValue tv;
				tv.set_pair(it->second,it->first);
				if(vkw.empty())block->SetDefaultValue(tv);
				else block->SetLocalVariable(active,vkw,tv,res);
				if(!cnt->ProcessCircleBlock(is,res,cdef,block))break;
			}
		}
		else
		{
			int num = dta.size();
			for(int i=0;i<num;i++)
			{
				CBasicBlockParser* block = cnt->PrepareCircleBlock(is,cdef);
				if(vkw.empty())block->SetDefaultValue(dta[i]);
				else block->SetLocalVariable(active,vkw,dta[i],res);
				if(!cnt->ProcessCircleBlock(is,res,cdef,block))break;
			}
			res = num;
		}
	}
	cnt->EndCircleBlock(is,res,cdef);
	return true;
}}static ForeachKW;

struct CWhileKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;

	wchar_t ps = is.peek();
	wstring vkw;

	int cond_pos = is.tellg();
	CVarValue cond;
	if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,cond);
	else cnt->ParseExpression(active,is,cond);

	ps = is.peek();

	CurcleBlockDef cdef;
	cnt->BeginCircleBlock(is,cdef);
	if(active && cond.to_bool())
	{
		do
		{
			CBasicBlockParser* block = cnt->PrepareCircleBlock(is,cdef);
			if(!cnt->ProcessCircleBlock(is,res,cdef,block))break;
			is.seekg(cond_pos,ios_base::beg);

			if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,cond);
			else cnt->ParseExpression(active,is,cond);
		}
		while(cond.to_bool());
	}
	res.set_nil();
	cnt->EndCircleBlock(is,res,cdef);
	return true;
}}static WhileKW;


struct CSplitKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	CVarValue bvar;
	wstring rexp;
	std::wstring::const_iterator begin, end; 

	is >> wistream_perl_quoted(rexp) >> wsp_nl_ucmt_scip;

	if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,bvar);
	else cnt->ParseExpression(active,is,bvar);

	begin = bvar.as_string().begin();
	end = bvar.as_string().end();

	boost::match_results<std::wstring::const_iterator> rvars;
	
	boost::wregex regex(rexp);
	res.set_vector();

	while(boost::regex_search(begin,end,rvars,regex))
	{
		res.append(CVarValue(rvars.prefix().str()));
		begin = rvars.suffix().first;
	}
	if(begin != end) res.append(CVarValue(wstring(begin,end)));
	
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}}static SplitKW;

struct CSearchKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	wstring rexp;
	is >> wistream_perl_quoted(rexp);
	//if(!active)return;

	CVarValue pvar = cnt->GetDefaultValue(); 

	std::wstring::const_iterator begin, end; 
	boost::match_results<std::wstring::const_iterator> rvars;

	begin = pvar.as_string().begin();
	end = pvar.as_string().end();

	boost::wregex regex(rexp);
	CurcleBlockDef cdef;
	cnt->BeginCircleBlock(is,cdef);
	int num=0;
	while(active && boost::regex_search(begin,end,rvars,regex))
	{
		CBasicBlockParser* proc = cnt->PrepareCircleBlock(is,cdef);
		begin = rvars.suffix().first;
		for(int i=0;i<rvars.size();i++)
		{
			CVarValue v,r;
			if(rvars[i].matched)v = rvars[i].str();
			proc->SetLocalVariable(active,lex_cast<wstring>(i),v,r);
		}
		active = cnt->ProcessCircleBlock(is,res,cdef,proc);
		num++;
	}
	cnt->EndCircleBlock(is,res,cdef);
	res = num;
	return true;
}}static SearchKW;


struct CQKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	wchar_t qs = is.peek();
	if(!wchar_classify::quote_pair(qs))
	{
		throw parse_exception(is,PERR_BAD_QUOTATION);
	}
	is >> wistream_quoted(res.set_string(L""),true);
    if(is.fail()) throw parse_exception(is,PERR_BAD_QUOTATION);
	
    if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}}static QKW;

struct CQQKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	wchar_t qs = is.peek();
	if(!wchar_classify::quote_pair(qs))
	{
		throw parse_exception(is,PERR_BAD_QUOTATION);
	}
	cnt->ParseString(active,is,res,true);
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}}static QQKW;

struct CEchoKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	if(cnt->pRsrv)cnt->pRsrv->AddMessage(param.to_string().c_str());
	return param;
}};
static KeywordAutoReg<CEchoKW> sEchoKW(L"echo");

struct CMessageKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	GetDCMapCore()->GetCoreUI()->MsgBox(param.to_string().c_str(),L"DCMap",0);
	return param;
}};
static KeywordAutoReg<CMessageKW> sMessageKW(L"message");

struct CShowConsoleKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	if(cnt->pRsrv)
	{
		cnt->pRsrv->Show();
		wstring str = param.to_string();
		if(!str.empty() && str != L"0")
		{
			cnt->pRsrv->SetHeader(str.c_str());
		}
	}
	return CVarValue();
}}static ShowConsoleKW;

struct CConsoleStatusKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	if(cnt->pRsrv)
	{
		cnt->pRsrv->SetStatus(param.to_string().c_str());
	}
	return CVarValue();
}}static ConsoleStatusKW;

struct CConsoleCaptionKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	if(cnt->pRsrv)
	{
		cnt->pRsrv->SetHeader(param.to_string().c_str());
	}
	return CVarValue();
}}static ConsoleCaptionKW;

struct CHideConsoleKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	if(cnt->pRsrv)
	{
		cnt->pRsrv->Hide();
	}
	return CVarValue();
}}static HideConsoleKW;

struct CTestCanselKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	if(cnt->pRsrv)
	{
		return (CVarValue)cnt->pRsrv->TestCancel();
	}
	return CVarValue();
}}static TestCanselKW;

struct CPauseKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	if(cnt->pRsrv)
	{
		int n = param.size();
		int opt=0;
		for(int i=1;i<n;i++)
		{
			opt |= param[i].to_int();
		}
		return (CVarValue)cnt->pRsrv->Pause(param[0].to_string().c_str(),opt);
	}
	return CVarValue();
}}static PauseKW;


struct CClearConsoleKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	if(cnt->pRsrv)
	{
		cnt->pRsrv->ClearMessages();
	}
	return CVarValue();
}}static ClearConsoleKW;

struct CSetupProgressKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	if(cnt->pRsrv)
	{
		cnt->pRsrv->SetupProgress(param[0].to_real(),param[1].to_real(),param[2].to_string().c_str());
	}
	return CVarValue();
}}static SetupProgressKW;

struct CAdvanceProgressKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	if(cnt->pRsrv)
	{
		float adv = param[0].to_real();
		if(adv == 0)adv=1;
		cnt->pRsrv->AdvanceProgress(adv,param[1].to_string().c_str());
	}
	return CVarValue();
}}static AdvanceProgressKW;

struct LValueDef
{
	wstring name;
	int ppos;
	bool vec;
	bool hash;
	LValueDef():vec(false),hash(false),ppos(-1){}
};

struct CPushKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){

	CVarValue var;
	CVarValue dst;
	LValueDef lvalue;

	is >> wsp_nl_ucmt_scip;
	if(is.peek() == L'$')is.ignore();
	is >> wistream_keyword_ext(lvalue.name);
	if(lvalue.name.empty())throw parse_exception(is,PERR_VARABLE_NAME_EXPECTED);
	is >> wsp_nl_ucmt_scip;
	wchar_t ps = is.peek();
	if(ps != L',') // complex lvalue so parse it later (slow case)
	{
		lvalue.ppos = is.tellg();
		// cnt->SetValue(false,lvalue.name,is,CVarValue());
	}
	cnt->GetVariable(active,lvalue.name,is,dst,cnt);
	if(ps != L',')throw parse_exception(is,PERR_UNEXPECTED_SYMBOL,wstring()+ps);
	is.ignore();
	is >> wsp_nl_ucmt_scip;

	cnt->ParseExpression(active,is,var);
	if(active)
	{
		dst.append(var);
		cnt->SetVariable(active,lvalue.name,dst,res);
	}
	return true;
}}static PushKW;

struct CPrintKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){

    CVarValue var;
    LValueDef lvalue;

    is >> wsp_nl_ucmt_scip;
    if(is.peek() == L'$')is.ignore();
    is >> wistream_keyword_ext(lvalue.name);
    if(lvalue.name.empty())throw parse_exception(is,PERR_VARABLE_NAME_EXPECTED);
    is >> wsp_nl_ucmt_scip;
    wchar_t ps = is.peek();
    if(ps != L',') // complex lvalue so parse it later (slow case)
    {
        lvalue.ppos = is.tellg();
        // cnt->SetValue(false,lvalue.name,is,CVarValue());
    }
    //cnt->GetVariable(active,lvalue.name,is,dst,cnt);
    if(ps != L',')throw parse_exception(is,PERR_UNEXPECTED_SYMBOL,wstring()+ps);
    is.ignore();
    is >> wsp_nl_ucmt_scip;

    cnt->ParseExpression(active,is,var);
    if(active)
    {
        CVarValue *pdst;
        if(!cnt->GetVariableLValue(lvalue.name,false,&pdst)){throw parse_exception(is,PERR_VARABLE_NOT_FOUND);}

        pdst->edit_string() += var.to_string();
    }
    return true;
}};
;static KeywordAutoReg<CPrintKW> sCSPrintKW(L"sprint");
;static KeywordAutoReg<CPrintKW> sCPrintKW(L"print");

struct CSubKW: CBaseKeyword
{
	int scope;
	bool local;
	CSubKW():scope(PRSR_SCOPE_LOCAL){}
	CSubKW(int sc):scope(sc){if(scope == PRSR_SCOPE_DEFAULT)scope = PRSR_SCOPE_LOCAL;}
	bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt)
	{
		is >> wsp_nl_ucmt_scip;

		wstring subname;
		if(is.peek() == L'$')is.ignore();
		wchar_t ps = is.peek();

		bool lambda=true;

		if(wchar_classify::alpha_ext(ps))
		{
			is >> wistream_keyword_ext(subname);
			lambda = false;
		}

		is >> wsp_nl_ucmt_scip;
		ps = is.peek();

		if(ps == ';') throw parse_exception(is,PERR_UNEXPECTED_SYMBOL);

		SubDef sdef;
		if(active)
		{
			sdef.stream = &is;
			sdef.pos = is.tellg(); 
			sdef.context=0;
			sdef.Acquire(cnt->GetObjectParser());
		}

		CVarValue tmpv;
		cnt->ParseBlock(false,is,tmpv);

		if(active)
		{
			if(lambda)
			{
				res.set_sub(sdef);
			}
			else
			{
				if(scope == PRSR_SCOPE_GLOBAL && !cnt->CheckRights(PRSR_DEF_GLOBAL_SUB))
				{
					throw parse_exception(is,PERR_INSUFFICIENT_RIGHRS,L"define global sub");
				}
				CVarValue *rv;
				if(cnt->GetVariableLValue(subname,scope,&rv))
				{
					rv->set_sub(sdef);
				}
			}
		}
		return true;
	}
};
static KeywordAutoReg<CSubKW> sCSubKW(L"sub");

//////////////////////////////////////////////////////////////////////////
struct CUseKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt)
{
	is >> wsp_nl_ucmt_scip;
	cnt->ParseExpression(active,is,res);
	if(active)cnt->BlockParser()->UseVariable(res);
	return true;
}};
static KeywordAutoReg<CUseKW> sCUseKW(L"use");

struct CWithKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt)
{
	is >> wsp_nl_ucmt_scip;
	CVarValue var; 
	cnt->ParseExpression(active,is,var);
	if(is.peek() == L'{')
	{
		CVarValue res; 		
		
		CBlockParser parser;
		parser.Parent(cnt);
		parser.UseVariable(var);
		parser.Parse(active,is,res,true);

		if(!expr)cnt->ContinueExpression(active,is,res);
	}
	else // error
	{

	}
	res.set_nil();

	return true;
}};
static KeywordAutoReg<CWithKW> sCWithKW(L"with");

struct CSetKW: CBaseKeyword
{
	int is_eqop(wchar_t ch)
	{
		switch(ch)
		{
		case '=':
		case ':': 
		case '+':
		case '-':
		case '/':
		case '*':
		case '|':
		case '&':
			return OP_ASSIGN;
		default: 
			return OP_NONE;
		}
	}
	
	bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt)
	{
	
	CBaseOperator* Operator=0;
	CVarValue DefaultValue;
	CVarValue tval;

	int scope =PRSR_SCOPE_DEFAULT; 
	if(kw[0]==L'm')scope = PRSR_SCOPE_LOCAL; 
	else if(kw[0]==L'g')
	{
		scope = PRSR_SCOPE_GLOBAL;
		if(!cnt->CheckRights(PRSR_DEF_GLOBAL_VAR))
		{
			throw parse_exception(is,PERR_INSUFFICIENT_RIGHRS,L"set global variable");
		}
	}
	else if(kw[0]==L'i')
	{
		Operator = GetParserOperator(OP_PLUS);
		DefaultValue = 1;
	}
	CVarValue var;

	LValueDef lvalue;
	vector<LValueDef> values;
	while(true)
	{
		lvalue.ppos = -1;
		bool vmod=false;

		is >> wsp_nl_ucmt_scip;
		if(is.peek() == L'$')
		{
			is.ignore();
			lvalue.vec = false;
			vmod=true;
		}
		else if(is.peek() == L'@')
		{
			is.ignore();
			lvalue.vec = true;
			vmod=true;
		}
		else if(is.peek() == L'%')
		{
			is.ignore();
			lvalue.hash = true;
			vmod=true;
		}

		is >> wistream_keyword_ext(lvalue.name);
		if(lvalue.name.empty())throw parse_exception(is,PERR_VARABLE_NAME_EXPECTED);

		if(!vmod && lvalue.name == L"sub") // sub definition
		{
			CSubKW sub(scope);
			return sub.Parse(active,lvalue.name,is,res,expr,cnt);
		}
		is >> wsp_nl_ucmt_scip;
		wchar_t ps = is.peek();
		int op = is_eqop(ps);
		if(!op && ps != L',' && ps != L';') // complex lvalue so parse it later (slow case)
		{
			lvalue.ppos = is.tellg();
            while(true)
            {
                ps = is.peek();
				op = is_eqop(ps);
                if(op || ps == L',' || ps == L';')break;
                else if(ps == '[') // expression parse subscript
                {
                    CVarValue tv;
                    cnt->ParseBasicBlock(false,is,tv);   
                }
                else if(ps == '.') // expression parse subscript
                {
                    is.ignore();
                    is >> wsp_nl_ucmt_scip;
                    if(wchar_classify::alpha_ext(is.peek()))
                    {
                        wstring kw;
                        is >> wistream_keyword_ext(kw);
                    }
                    else 
                    {
                        CVarValue tv;
                        cnt->ParseValue(false,is,tv,cnt);
                    }
                }
                else throw parse_exception(is,PERR_UNEXPECTED_SYMBOL,ps);
                is >> wsp_nl_ucmt_scip;
            }
		}
		if(Operator && ps == L',')ps = L'=';

		if(ps == L',')
		{
			values.push_back(lvalue);
			is.ignore();
		}
		else if(op)
		{
			CBaseOperator* pOp = GetAssignOperator(is);
			if(pOp)Operator = pOp;

			if(!values.empty()) values.push_back(lvalue);
			else if(lvalue.ppos>=0) values.push_back(lvalue); // complex values goes to vector
			is >> wsp_nl_ucmt_scip;
			if(Operator && Operator->is_unary())
			{
				break;
			}
			else
			{
				cnt->ParseExpression(active,is,var);
			}
			break;
		}
		else if(ps == L';')
		{
			if(!values.empty()) values.push_back(lvalue);
			else if(lvalue.ppos>=0) values.push_back(lvalue); // complex values goes to vector

			if(Operator)
			{
				var = DefaultValue;
			}
			else
			{
				if(scope == PRSR_SCOPE_DEFAULT)active = false; // no empty decl needed in default scope
				var.set_nil();
			}
			break;
		}
		else throw parse_exception(is,PERR_UNEXPECTED_SYMBOL);
	}
	if(active)
	{
		if(values.empty())
		{
			CVarValue* lval;
			if(!cnt->GetVariableLValue(lvalue.name,scope,&lval)){throw parse_exception(is,PERR_VARABLE_NOT_FOUND);}

            if(lvalue.vec)
            {
                if(var.is_nil())lval->set_vector();
                else if(var.type() == VT_HASH)
                {
                    lval->set_vector();
                    lval->append_value(var);
                }
                else (*lval) = var;
            }
			else if(lvalue.hash)
			{
				lval->set_hash(var);
			}
			else // scalar set
			{
				if(Operator)
				{
					CVarValue res;
					if(Operator->is_unary()) Operator->execute((*lval),res,cnt);
					else Operator->execute((*lval),var,res,cnt);
					lval->LVAssign(res);
				}
				else lval->LVAssign(var); // scalar set
			}
		}
		else
		{
			int rmax = var.size()-1;
			int lcnt = values.size();
			int cpos = is.tellg();
			for(int i=0;i<lcnt;i++)
			{
				int ind = min(i,rmax);
				CVarValue* lval;
				if(!cnt->GetVariableLValue(values[i].name,scope,&lval)){throw parse_exception(is,PERR_VARABLE_NOT_FOUND);}

				if(values[i].ppos>=0) // parse modifiers
				{
					if(values[i].vec && lval->is_nil()) (*lval).set_vector();
					else if(values[i].hash && lval->is_nil()) (*lval).set_hash();

					is.seekg(values[i].ppos,ios_base::beg);
                    while(true)
                    {
					    wchar_t ps = is.peek();
						int op = is_eqop(ps);
                        if(op || ps == L',' || ps == L';')break;
					    else if(ps == '[') // expression parse subscript
					    {
						    CVarValue tv;
						    cnt->ParseBasicBlock(true,is,tv);   
							if(!(*lval).retrieve(tv,&lval))
							{
								tval = (*lval).retrieve_var(tv);
								lval = &tval;
							}
					    }
					    else if(ps == '.') // expression parse subscript
                        {
                            is.ignore();
                            is >> wsp_nl_ucmt_scip;
                            if(wchar_classify::alpha_ext(is.peek()))
                            {
                                wstring kw;
                                is >> wistream_keyword_ext(kw);
                                if(!(*lval).retrieve(CVarValue(kw),&lval))
								{
									tval = (*lval).retrieve_var(CVarValue(kw));
									lval = &tval;
								}
                            }
                            else 
                            {
                                CVarValue tv;
                                cnt->ParseValue(true,is,tv,cnt);
                                if(!(*lval).retrieve(tv,&lval))
								{
									tval = (*lval).retrieve_var(tv);
									lval = &tval;
								}
                           }
                        }
					    else throw parse_exception(is,PERR_UNEXPECTED_SYMBOL,ps);
                        is >> wsp_nl_ucmt_scip;
                    }
				}
				else
				{
					if(values[i].vec)(*lval).set_vector();    // clear to initial
					else if(values[i].hash)(*lval).set_hash(); 
				}
				if(lcnt==1) 
				{
					if(lvalue.vec)
                    {
                        if(var.is_nil())lval->set_vector();
                        else if(var.type() == VT_HASH)
                        {
                            lval->set_vector();
                            lval->append_value(var);
                        }
                        else lval->LVAssign(var);
                    }
					else if(lvalue.hash)lval->set_hash(var);
					else // scalar set
					{
						if(Operator)
						{
							CVarValue res;
							if(Operator->is_unary()) Operator->execute((*lval),res,cnt);
							else Operator->execute((*lval),var,res,cnt);
							lval->LVAssign(res);
						}
						else lval->LVAssign(var); // scalar set
					}
				}
				else 
				{
					if(values[i].vec)
					{
						(*lval).set_vector();    // clear to initial
						lval->append(var[ind]);
					}
					else if(values[i].hash)
					{
						(*lval).set_hash(); 
					}
					else lval->LVAssign(var[ind]);     // vector set
				}
			}
			is.seekg(cpos,ios_base::beg);
		}
	}
	return true;
}};
REG_KEYWORD(CSetKW,set);
REG_KEYWORD(CSetKW,inc);
REG_KEYWORD(CSetKW,let);
REG_KEYWORD(CSetKW,my);
REG_KEYWORD(CSetKW,global);


struct CDieKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	CVarValue msg;
	is >> wsp_nl_ucmt_scip;
	if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,msg);
	else cnt->ParseExpression(active,is,msg);
	if(active)
	{
		throw die_exception(msg.to_string().c_str());
	}
	res.set_nil();
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}};
REG_KEYWORD(CDieKW,die);


struct CContinueKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	CVarValue msg;
	if(active)throw continue_exception();
	res.set_nil();
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}}static ContinueKW;

struct CBreakKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	CVarValue msg;
	if(active)throw break_exception();
	res.set_nil();
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}}static BreakKW;

struct CRenturnKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	CVarValue msg;
	is >> wsp_nl_ucmt_scip;
	if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,msg);
	else cnt->ParseExpression(active,is,msg);
	if(active)
	{
		throw return_exception(msg);
	}
	res.set_nil();
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}}static RenturnKW;

struct CClipboardKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	if(!expr && is.peek() == L'=')
	{
		is.ignore();
		is >> wsp_nl_ucmt_scip;
		cnt->ParseExpression(active,is,res);
		if(active)
		{
            if(!cnt->Permited(PRSR_CLBRD_WRITE))throw(parse_exception(is,PERR_INSUFFICIENT_RIGHRS),L"write clipboard");
			GetDCMapCore()->GetCoreUI()->ClipboardPutW(res.to_string().c_str());
		}
	}
	else if(active)
	{
        if(!cnt->Permited(PRSR_CLBRD_READ))throw(parse_exception(is,PERR_INSUFFICIENT_RIGHRS),L"read clipboard");
		dcmapDCSTR cnt = GetDCMapCore()->GetCoreUI()->ClipboardGetW();
		if(cnt && cnt[0])res = cnt;
		else res.set_nil();
	}
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}}static ClipboardKW;

struct CLoadFileKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){

	int CP = DCMapCodePage;
	if(kw == L"loadfile_utf8")CP = CP_UTF8;

	CVarValue params;
	is >> wsp_nl_ucmt_scip;
	if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,params);
	else cnt->ParseExpression(active,is,params);

	if(active)
	{
        if(!cnt->Permited(PRSR_FILE_READ))throw(parse_exception(is,PERR_INSUFFICIENT_RIGHRS),L"read file");

        if(params.size()>1) CP = params[1].to_int();

		dcmapWCSTR buf = dcmapLoadText(params[0].to_string().c_str(),CP);
		if(!buf)res.set_nil();
		else res = buf;
	}
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}}static LoadFileKW;

struct CGUnzipKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
    
    return CVarValue();
}};
//static KeywordAutoReg<CGUnzipKW> sCGUnzipKW(L"gunzip");

struct CGZipKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
    

    return CVarValue();
}};
//static KeywordAutoReg<CGZipKW> sCGZipKW(L"gzip");


struct CSaveFileKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){

	int CP = DCMapCodePage;
	if(kw == L"savefile_utf8")CP = CP_UTF8;

    CVarValue params;
    is >> wsp_nl_ucmt_scip;
    if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,params);
    else cnt->ParseExpression(active,is,params);

	if(active)
	{
        if(!cnt->Permited(PRSR_FILE_WRITE))throw(parse_exception(is,PERR_INSUFFICIENT_RIGHRS),L"write file");

        if(params.size()>2) CP = params[2].to_real();

		if(dcmapSaveText(params[1].to_string().c_str(),params[0].to_string().c_str(),CP))
			res = true;
		else res.set_nil();
	}
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}}static SaveFileKW;



struct CNilKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	res.set_nil();
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}}static NilKW;


//////////////////////////////////////////////////////////////////////////

struct CEditTableKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	dcmapDBREF const* pRef=0;
	if(param[1].is_dbref())
		pRef = &param[1].as_dbref();

	return  (CVarValue)GetDCMapCore()->GetCoreUI()->OpenListTableEdit(
			WideToString(param[0].to_string()).c_str(),
			pRef,0);

}}static EditTableKW;

struct CSaveDBKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	GetDCMapCore()->SaveWithBackup(0,0,1);
	return CVarValue();
}}static SaveDBKW;

struct CExitDCMapKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	GetDCMapCore()->GetCoreUI()->CloseMainWnd();
	return CVarValue();
}}static ExitDCMapKW;

struct CExecuteKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	if(param.size()<1)return CVarValue();
	IDCMapModule* mod = GetDCMapCore()->GetService(WideToString(param[0].to_string()).c_str());
	if(!mod)return CVarValue();
	return (CVarValue)mod->OnMessage(DCMM_EXECUTE,param[1].to_real(),(dcmapDWORD)param[2].to_string().c_str(),cnt->GetModule());
}}static ExecuteKW;

//////////////////////////////////////////////////////////////////////////

void SubDef::Execute(CVarValue const& param,CVarValue & res)
{
	int callerpos = stream->tellg();
	stream->seekg(pos,ios_base::beg);
	try
	{
		context->ParseBlock(true,*stream,res,param);
	}
	catch (return_exception e)
	{
		res = e.val;
	}
	catch (break_exception e)
	{
		throw parse_exception(*stream,PERR_INVALID_BREAK);
	}
	catch (continue_exception e)
	{
		throw parse_exception(*stream,PERR_INVALID_CONTINUE);
	}
	stream->seekg(callerpos,ios_base::beg);
}

//////////////////////////////////////////////////////////////////////////
struct CRGBAEncodeKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){

	dcmapBYTE a = 255;
	dcmapCOLORREF cl;
	if(param.type() == VT_HASH)
	{
		dcmapCOLORREF cl = dcmapRGB(	gsys::bound(param[L"r"].to_real(),0,1)*255,
			gsys::bound(param[L"g"].to_real(),0,1)*255,
			gsys::bound(param[L"b"].to_real(),0,1)*255);
		if(!param[L"a"].is_nil())a = gsys::bound(param[L"a"].to_real(),0,1)*255;
	}
	else
	{
		dcmapCOLORREF cl = dcmapRGB(	gsys::bound(param[0].to_real(),0,1)*255,
			gsys::bound(param[1].to_real(),0,1)*255,
			gsys::bound(param[2].to_real(),0,1)*255);
		if(param.size()>=4)a = gsys::bound(param[3].to_real(),0,1)*255;
	}
	cl |= a<<24;
	return CVarValue(cl);
}};static KeywordAutoReg<CRGBAEncodeKW> sCRGBAEncodeKW(L"rgba_encode");

struct CRGBADecodeKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	dcmapCOLORREF cl = param.to_real();
	CVarValue r;
	r.set_vector();
	r.append(CVarValue(dcmapGetRValue(cl)/255.0f));//r
	r.append(CVarValue(dcmapGetGValue(cl)/255.0f));//g
	r.append(CVarValue(dcmapGetBValue(cl)/255.0f));//b
	r.append(CVarValue(((cl&0xff000000)>>24)/255.0f));//a
	return r;
}};static KeywordAutoReg<CRGBADecodeKW> sCRGBADecodeKW(L"rgba_decode");
//////////////////////////////////////////////////////////////////////////
#define KW_FNWRAP(fn) struct C##fn##KW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){return CVarValue(fn(param.to_real()));}}static fn##KW;Keywords[L#fn] = &fn##KW;	

struct RandKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
    return CVarValue(gsys::frand());
}
};static KeywordAutoReg<RandKW> sRandKW(L"rand");

#define KW_CONSTN(dcmsg,name) struct C##dcmsg##KW: CBaseKeyword{ \
	bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){\
	res = dcmsg;if(!expr)cnt->ContinueExpression(active,is,res);return true;\
}}static fn##dcmsg;Keywords[name] = &fn##dcmsg;

#define KW_CONST(dcmsg) struct C##dcmsg##KW: CBaseKeyword{ \
	bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){\
	res = dcmsg;if(!expr)cnt->ContinueExpression(active,is,res);return true;\
}}static fn##dcmsg;Keywords[L#dcmsg] = &fn##dcmsg;


struct StdKeywordAutoReg
{
	StdKeywordAutoReg();
	~StdKeywordAutoReg();
} static a_reg;

StdKeywordAutoReg::StdKeywordAutoReg()
{
	if(!g_pKeywordAutoRegMan) g_pKeywordAutoRegMan = new CKeywordAutoRegMan;
	MapKeywords& Keywords = g_pKeywordAutoRegMan->m_mapKW;

	KW_CONST(DCMM_PLANET_CHANGED);
	KW_CONST(DCMM_DATA_CHANGED);
	KW_CONST(DCMM_MAPDATA_CHANGED);
	KW_CONST(DCMM_PLANET_MAPDATA_CHANGED);
	KW_CONST(DCMM_SELECTION_DATA_CHANGED);
	KW_CONST(DCMM_PLANET_FILTER_CHANGED);
	KW_CONST(DCMM_POST_INIT);
	KW_CONST(DCMM_POINT_PLANET);
	KW_CONST(DCMM_WORKSPACE_ACTIVATE);
	KW_CONST(DCMM_WORKSPACE_DEACTIVATE);
	KW_CONST(DCMM_CURRENT_RACE_CHANGED);
	KW_CONST(DCMM_CONFIG_CHANGED);
	KW_CONST(DCMM_LAYERS_CHANGED);
	KW_CONST(DCMM_LAYERS_VISIBLITY_CHANGED);
	KW_CONST(DCMM_FILTERUI_SET_FILTER);
	KW_CONST(DCMM_PRE_DESTROY);
	KW_CONST(DCMM_EXECUTE);
	KW_CONST(DCMM_SHOW_CELL);
	KW_CONST(DCMM_SKIN_CHANGED);
	KW_CONST(DCMM_CHECK_WEB_UPDATES);

	KW_CONST(DCMM_SUCCESS);
	KW_CONST(DCMM_FAIL);
	KW_CONST(DCMM_PROCESSED);

	KW_CONSTN(DCM_PAUSE_QUIT,L"pause_quit");
	KW_CONSTN(DCM_PAUSE_CANCEL,L"pause_cancel");
	KW_CONSTN(DCM_PAUSE_CONTINUE,L"pause_continue");
	KW_CONSTN(DCM_PAUSE_RETRY,L"pause_retry");

	KW_CONST(CP_UTF8);

	KW_FNWRAP(sqrt);
	KW_FNWRAP(abs);
	KW_FNWRAP(sin);
	KW_FNWRAP(tan);		
	KW_FNWRAP(atan);		
	KW_FNWRAP(cos);		
	KW_FNWRAP(asin);		
	KW_FNWRAP(acos);		
	KW_FNWRAP(exp);		

	Keywords[L"tg"] = &tanKW;
	Keywords[L"arctg"] = &atanKW;
	Keywords[L"arcsin"] = &asinKW;
	Keywords[L"arccos"] = &acosKW;

	Keywords[L"break"] = &BreakKW;
	Keywords[L"continue"] = &ContinueKW;
	Keywords[L"next"] = &ContinueKW;

	Keywords[L"search"] = &SearchKW;
	Keywords[L"for"] = &ForeachKW;
	Keywords[L"foreach"] = &ForeachKW;
	Keywords[L"while"] = &WhileKW;

	Keywords[L"split"] = &SplitKW;

	Keywords[L"if"] = &IfKW;
	Keywords[L"else"] = &ElseKW;

	Keywords[L"loadfile"] = &LoadFileKW;
	Keywords[L"loadfile_utf8"] = &LoadFileKW;

	Keywords[L"savefile"] = &SaveFileKW;
	Keywords[L"savefile_utf8"] = &SaveFileKW;

	Keywords[L"return"] = &RenturnKW;

	Keywords[L"clbrd"] = &ClipboardKW;	

	Keywords[L"clipboard"] = &ClipboardKW;	

/*
	Keywords[L"my"] = &SetKW;	
	Keywords[L"global"] = &SetKW;	
	Keywords[L"let"] = &SetKW;	
	Keywords[L"set"] = &SetKW;	
*/

	Keywords[L"push"] = &PushKW;	

	Keywords[L"q"] = &QKW;	
	Keywords[L"qq"] = &QQKW;

	Keywords[L"nil"] = &NilKW;	
	Keywords[L"none"] = &NilKW;	

	Keywords[L"edit_table"] = &EditTableKW;
	Keywords[L"execute"] = &ExecuteKW;

	Keywords[L"save_db"] = &SaveDBKW;
	Keywords[L"sysexit"] = &ExitDCMapKW;

	Keywords[L"clear_console"] = &ClearConsoleKW;
	Keywords[L"hide_console"] = &HideConsoleKW;
	Keywords[L"show_console"] = &ShowConsoleKW;
	Keywords[L"setup_progress"] = &SetupProgressKW;   
	Keywords[L"advance_progress"] = &AdvanceProgressKW;
	Keywords[L"test_cancel"] = &TestCanselKW;

	Keywords[L"console_status"] = &ConsoleStatusKW;
	Keywords[L"console_caption"] = &ConsoleCaptionKW;
	Keywords[L"pause"] = &PauseKW;
}

StdKeywordAutoReg::~StdKeywordAutoReg()
{
	if(g_pKeywordAutoRegMan) 
	{
#ifdef _DEBUG
		MapKeywords& Keywords = g_pKeywordAutoRegMan->m_mapKW;
		gsys::std_file file(ToWide(gsys::GetAppStartupDir()+"keywords.txt"),gsys::gfile::ModeCreate);
		if(file.valid())
		{
			_foreach(MapKeywords,Keywords,kw)
			{
				file << WideToString(kw->first)+"\n";
			}
		}
#endif // _DEBUG

		delete g_pKeywordAutoRegMan;
		g_pKeywordAutoRegMan=0;
	}
}


CKeywordAutoRegMan* g_pKeywordAutoRegMan=0;
bool ParseStaticKeywords(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt)
{
	MapKeywords::iterator it = g_pKeywordAutoRegMan->m_mapKW.find(kw);
	if(it == g_pKeywordAutoRegMan->m_mapKW.end())return false;
	return it->second->Parse(active,kw,is,res,expr,cnt);
}
