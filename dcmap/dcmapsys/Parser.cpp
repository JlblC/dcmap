#include "StdAfx.h"
#include "Parser.h"
#include "Utils.h"
#include <boost/regex.hpp>
#include <boost/scoped_ptr.hpp>

#include <g_text.h>
using namespace gsys;

IDCMapProcessRecordReseiver* CBasicBlockParser::pRsrv=0;

CBasicBlockParser::CBasicBlockParser():
    m_chQoute(wchar_classify::eof()),
    m_pParent(0),
    //m_pModule(0),
    opresult(0),
    ifresult(0),
    m_Rights(PRSR_NONE),
	m_ParseOptions(PRSO_DEFAULT)
{

}

void CBasicBlockParser::BeginCircleBlock(wiparsestream &is,CurcleBlockDef& def)
{
	is >> wsp_nl_ucmt_scip;
	def.first = is.tellg();
	def.second=-1;
}

CBasicBlockParser* CBasicBlockParser::PrepareCircleBlock(wiparsestream &is,CurcleBlockDef& def)
{
	CBasicBlockParser* p = new CBlockParser();
	p->Parent(this);
	return p;
}

bool CBasicBlockParser::ProcessCircleBlock(wiparsestream &is,CVarValue& res,CurcleBlockDef& def,CBasicBlockParser* proc)
{
	//_ASSERT(m_iCircleBegin>=0);
	boost::scoped_ptr<CBasicBlockParser> p(proc);
	if(!proc)p.reset(new CBlockParser());
    p->Parent(this);

	is.seekg(def.first,ios_base::beg);
	try
	{
		p->Parse(true,is,res,true);
	}
	catch(break_exception)
	{
		is.seekg(def.first,ios_base::beg);
		return false;
	}
	catch(continue_exception)
	{
		is.seekg(def.first,ios_base::beg);
		return true;
	}
	if(def.second<0)def.second=is.tellg();
	return true;
}

void CBasicBlockParser::EndCircleBlock(wiparsestream &is,CVarValue& res,CurcleBlockDef& def)
{
	if(def.first<0)return;
	if(def.second<0)
	{
		is.seekg(def.first,ios_base::beg);
		ParseBlock(false,is,res);
	}
	else is.seekg(def.second,ios_base::beg);
	def.first = -1;
	def.second=-1;
}

CVarValue&  CBasicBlockParser::GetDefaultValue()
{
	return m_pParent->GetDefaultValue();
}

CVarValue&  CBlockParser::GetDefaultValue()
{
	if(m_valDefault.is_nil() && m_pParent)return m_pParent->GetDefaultValue();
	return m_valDefault;
}

bool CBasicBlockParser::ParseCustomKeyword(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt)
{
	if(m_pParent)
	{
		return m_pParent->ParseCustomKeyword(active,kw,is,res,expr,cnt);
	}
	return false;
}

void CBasicBlockParser::ParseKeyword(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr)
{
	if(ParseStaticKeywords(active,kw,is,res,expr,this))
	{

	}
	else if(!ParseCustomKeyword(active,kw,is,res,expr,this))
	{
		GetVariable(active,kw,is,res,this);
		if(!expr)ContinueExpression(active,is,res);
	}		
}

void CBasicBlockParser::SetVariable(bool active,wstring const& kw,CVarValue const& val,CVarValue & res) 
{
	if(m_pParent)m_pParent->SetVariable(active,kw,val,res);
}

void CBasicBlockParser::SetLocalVariable(bool active,wstring const& kw,CVarValue const& val,CVarValue & res) 
{
	if(m_pParent)m_pParent->SetLocalVariable(active,kw,val,res);
}

void CBlockParser::SetVariable(bool active,wstring const& kw,CVarValue const& val,CVarValue & res)
{
	if(kw == L"_")
	{
		SetDefaultValue(val);
	}
	else
	{
		VarMap::iterator it = m_Locals.find(kw);
		if(it != m_Locals.end())
		{
			it->second = val;
			res = val;
		}
		else if(m_pParent)
		{
			m_pParent->SetVariable(active,kw,val,res);
		}
		else 
		{
			m_Locals[kw] = val;
			res = val;
		}
	}
}

bool CBasicBlockParser::GetVariableLValue(wstring const& kw,int scope,CVarValue ** result)
{
	if(m_pParent)
	{
		return m_pParent->GetVariableLValue(kw,scope,result);
	}
	return false;
}

bool CBlockParser::GetVariableLValue(wstring const& kw,int scope,CVarValue ** result)
{
	if(scope == PRSR_SCOPE_GLOBAL)
	{
		return g_GlobalModule.GetVariableLValue(kw,PRSR_SCOPE_LOCAL,result);
	}
	if(kw == L"_")
	{
		*result = &m_valDefault;
	}
	else
	{
		VarMap::iterator it = m_Locals.find(kw);
		if(it != m_Locals.end())
		{
			*result = &it->second;
			return true;
		}
		else if(!m_LocalUsings.empty())
		{
			_foreach(VarVector,m_LocalUsings,it)
			{
				CVarValue& v = *it;
				if(v.have_subobj_writable(kw))
				{
					if(v.retrieve(CVarValue(kw),result))
					{
						return true;
					}
					else
					{

					}
				}
			}
		}

		if(m_pParent && scope != PRSR_SCOPE_LOCAL)
		{
			return m_pParent->GetVariableLValue(kw,scope,result);
		}
		else 
		{
			*result = &m_Locals[kw];
		}
	}
	return true;
}

void CBlockParser::SetLocalVariable(bool active,wstring const& kw,CVarValue const& val,CVarValue & res)
{
	if(kw == L"_")
	{
		SetDefaultValue(val.to_string());
	}
	else
	{
		m_Locals[kw] = val;
		res = val;
	}
}

static bool EndOfStatsment(wchar_t ps)
{
	if(ps == L';' || ps == L',' || wchar_classify::closing_braces(ps) || wchar_classify::eof(ps))return true;
	return false;
}

void CBasicBlockParser::GetVariable(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,CBasicBlockParser* cnt)
{
	if(m_pParent)
	{
		m_pParent->GetVariable(active,kw,is,res,cnt);
	}
	else if(ParseOptions()&PRSO_TREAT_UNKNOWN_KEYWORD_AS_STRING)
	{
		if(wchar_classify::numeric(kw[0]))res.set_nil();
		else res.set_string(kw);
	}
	else 
	{
		res.set_nil();
	}
}

void CBlockParser::GetVariable(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,CBasicBlockParser* cnt)
{
	// test local variables
	if(kw == L"_")
	{
		res = GetDefaultValue();
	}
	else
	{
		VarMap::iterator it = m_Locals.find(kw);
		if(it != m_Locals.end())
		{
			res = it->second;
			return;
		}
		else if(!m_LocalUsings.empty())
		{
			_foreach(VarVector,m_LocalUsings,it)
			{
				CVarValue& v = *it;
				if(v.have_subobj(kw))
				{
					res = v.subobj(CVarValue(kw));
					return;
				}
			}
		}

		if(m_pParent)
		{
			m_pParent->GetVariable(active,kw,is,res,cnt);
		}
		else
		{
			VarMap::iterator it = g_GlobalModule.m_Locals.find(kw);
			if(it != g_GlobalModule.m_Locals.end())
			{
				res = it->second;
			}
			else if(ParseOptions()&PRSO_TREAT_UNKNOWN_KEYWORD_AS_STRING)
			{
				if(wchar_classify::numeric(kw[0]))res.set_nil();
				else res.set_string(kw);
			}
			else 
			{
				res.set_nil();
			}
		}
	}
}

void CBlockParser::Destruct()
{
	m_valDefault.set_nil();
	m_Locals.clear();
}

void CBlockParser::UseVariable( CVarValue const& val )
{
	if(val.is_nil())return;

	m_LocalUsings.push_back(val);
}

void CBasicBlockParser::ContinueExpressionOp(bool active,int stop_prio, CVarValue& v1,
                                             CBaseOperator* op,wiparsestream &is,
                                             CVarValue& res,CBaseOperator* &nextop,
                                             CBasicBlockParser* cnt)
{
	CVarValue v2;
	CVarValue v2res;

	bool ngr = active;

	if(op->is_or() && v1.to_bool())ngr=false;
	if(op->is_and() && !v1.to_bool())ngr=false;

	if(op->is_regexp())
	{
		ParseRegExp(active,is,v1,v2);
	}
	else if(op->is_keyword())
	{
        is >> wsp_nl_ucmt_scip;
        if(wchar_classify::alpha_ext(is.peek()))
        {
            wstring& keyw = v2.set_string();
            is >> wistream_keyword_ext(keyw);
        }
        else ParseValue(ngr,is,v2,cnt);
	}
    else ParseValue(ngr,is,v2,cnt);

	is >> wsp_nl_ucmt_scip;
	CBaseOperator* op2 = GetParserOperator(is);
	is >> wsp_nl_ucmt_scip;

	if(!op2 || op2->priority() <= stop_prio)
	{
		if(active)op->execute(v1,v2,res,cnt);
		nextop = op2;
	}
	else if(op->priority() < op2->priority()) // execute second operator group till same prio
	{
		while(1)
		{
			ContinueExpressionOp(ngr,op->priority(),v2,op2,is,v2res,nextop,cnt);
			if(nextop && nextop->priority() == op2->priority())
			{
                v2.eat(v2res);
				op2 = nextop;
			}
			else
			{
				if(active)op->execute(v1,v2res,res,this);
				break;
			}
		}
		// then first operator
	}
	else // execute this operator first
	{
		if(active)op->execute(v1,v2,res,cnt);
		nextop = op2;
	}
}

void CBasicBlockParser::ContinueExpression(bool active,wiparsestream &is,CVarValue& res,int stop_prio)
{
	is >> wsp_nl_ucmt_scip;

	CBaseOperator* op1 = GetParserOperator(is);
	is >> wsp_nl_ucmt_scip;

    if(op1)
    {
	    CBaseOperator* op2=0;
	    CVarValue& v1 = res;
	    CVarValue v2;// = res;
	    do
	    {
		    ContinueExpressionOp(active,stop_prio,v1,op1,is,v2,op2,this);
            v1.eat(v2);
		    op1=op2;
	    }
        while(op1);
	    is >> wsp_nl_ucmt_scip;
    }
}


void CBasicBlockParser::ParseValue(bool active,wiparsestream &is,CVarValue& res,CBasicBlockParser* cnt)
{
	wchar_t ps = is.peek();

	if(ps == '!') 
	{
		is.ignore();
		is >> wsp_nl_ucmt_scip;
		ParseValue(active,is,res,cnt);
		res = !res.to_bool();
		return;
	}

	if(ps == L'\'')
	{
		is >> wistream_quoted(res.set_string());
        if(is.fail()) throw parse_exception(is,PERR_BAD_QUOTATION);
	}
	else if(ps == L'"')
	{
		ParseString(active,is,res,true);
	}
	else if(ps == L'`')
	{
		CVarValue cmd;
		ParseString(active,is,cmd,true);
        if(active)
        {
            if(!cnt->Permited(PRSR_OSEXEC))
                throw(parse_exception(is,PERR_INSUFFICIENT_RIGHRS),L"os execute");
            wstring Output;
            if(CmdExec(cmd.to_string(),Output))
            {
                res = Output;
            }
            else res.set_nil();
        }
	}
	else if(ps == L'$') // get some variable
	{
		is.ignore();
		if(wchar_classify::whitespace_newline(is.peek()))throw parse_exception(is,PERR_CUSTOM,L"invalid variable");
		wstring str;
		if(wchar_classify::numeric(is.peek()))
		{
			is >> wistream_str_numeric(str);
		}
		else if(wchar_classify::alpha_ext(is.peek()))
		{
			is >> wistream_keyword_ext(str);
		}
		else 
		{
			str = is.peek();
			is.ignore();
		}
		GetVariable(active,str,is,res,this);
	}
	else if(wchar_classify::alpha_ext(ps))
	{
		wstring kw;
		is >> wistream_keyword_ext(kw);
		ParseKeyword(active,kw,is,res,true);
	}
	else if(ps == L'/') // beginning of reg exp located here
	{
		ParseRegExp(active,is,GetDefaultValue(),res);
	}	
	else if(ps == L'(')
	{
		ParseBasicBlock(active,is,res);
	}	
	else if(ps == L'[')
	{
		ParseBasicBlock(active,is,res);
	}
	else if(ps == L'{')
	{
		ParseBlock(active,is,res);
	}	
	else if(wchar_classify::numeric_ex(ps))
	{
		wstring str;
		is >> wistream_str_numeric(str);
		res = lex_cast<double>(str);
	}
	else if(EndOfStatsment(ps)) // no value at all
	{
		res.set_nil();
		return;
	}
	else 
	{
		throw parse_exception(is,PERR_CUSTOM,L"invalid value");
	}
	is>>wsp_nl_ucmt_scip;
	/*while(is.peek() == L'[') // parse subscript
	{
		CVarValue r;
		ParseBasicBlock(active,is,r);
		res = res[r.as_real()];
		is>>wsp_nl_ucmt_scip;
	}*/
}

void CBasicBlockParser::ParseBlock(bool active,wiparsestream &is,CVarValue& result,CVarValue const& locals)
{
	CBlockParser parser;
    parser.Parent(this);
	parser.SetDefaultValue(locals);
	parser.Parse(active,is,result,true);
}

void CBasicBlockParser::ParseBlock(bool active,wiparsestream &is,CVarValue& result)
{
	CBlockParser parser;
    parser.Parent(this);
	parser.Parse(active,is,result,true);
}

void CBasicBlockParser::ParseBasicBlock(bool active,wiparsestream &is,CVarValue& result)
{
	CBasicBlockParser parser;
	parser.Parent(this);
	parser.Parse(active,is,result,true);
}


inline wstring full_excape(std::wstring & str)
{
	size_t n=str.length();
	wstring unq;
	for(size_t i=0;i<n;i++)
	{
		wchar_t pc = str[i];
		if(wchar_classify::alpha_ext(pc) || wchar_classify::numeric(pc))
		{
			unq+=pc;
		}
		else if(pc == L'\n')unq += L"\\n";
		else if(pc == L'\r')unq += L"\\r";
		else if(pc == L'\t')unq += L"\\t";
		else if(pc == L' ')unq += L' ';
		else 
		{
			unq+=L'\\';
			unq+=pc;
		}
	}
	return unq;
}

void CBasicBlockParser::ParseStringLexGroup(bool active,wiparsestream &is,wstring& result,wchar_t qtch,bool groupping,bool decode)
{
	wchar_t ps = is.peek();
	int gopen=0;
	if(ps == L'('  && groupping)// open group
	{
		is.ignore();
		gopen=1;
	}
	int LastResult=-1;	
	while(!is.fail() && !is.eof())
	{
		ps = is.peek();
		if(wchar_classify::eof(ps))return;
		if(wchar_classify::newline(ps)) // newlines ignored
		{
			is.ignore();
			continue;
		}

		if(ps==L'$')// variable or expression
		{
			is.ignore();
			CVarValue gvar;
			if(is.peek() == qtch)
			{
				result+= L'$';
			}
			else if(is.peek() == L'{')
			{
				ParseBlock(active,is,gvar);
			}
			else if(is.peek() == L'(')
			{
				wstring gstr;
				ParseStringLexGroup(active,is,gstr,qtch,true,decode);
				result += gstr;
			}			
			else 
			{
				wstring val;
				is >> wistream_numb_keyword(val);
				GetVariable(active,val,is,gvar,this);
			}

			if(is.peek()==L'?' && groupping) // conditional block
			{
				is.ignore();
				bool first = gvar.to_bool();

				wstring res1,res2;
				ParseStringLexGroup(first?active:false,is,res1,qtch);
				if(is.peek()==L':')
				{
					is.ignore();
					ParseStringLexGroup(first?false:active,is,res2,qtch);
				}
				if(first)
				{
					result+=res1;
				}
				else
				{
					result+=res2;
				}
			}
			else 
			{
				if(!decode) result += full_excape(gvar.to_string());
				else result += gvar.to_string();
			}
		}
		/*else if(ps==L'(' && groupping)// begin new group
		{
			wstring gstr;
			ParseStringLexGroup(active,is,gstr,qtch,groupping,decode);
			result += gstr;
		}*/
		else if(ps==L'(' && gopen) // end of group
		{
			++gopen;
			result+=ps;
			is.ignore();
		}
		else if(ps==L')' && gopen) // end of group
		{
			if(gopen==1)
			{
				is.ignore();
				return;
			}
			else 
			{
				--gopen;
				result+=ps;
				is.ignore();
			}
		}
		else if(ps == L'\\')
		{
			is.ignore();
			if(decode)
			{
				result += wchar_classify::decode_cescaped_char(is.get());
			}
			else
			{
				result+= '\\';
				result+= is.get();
			}
		}
		else if(ps==qtch) // end of string
		{
			return;
		}
		else 
		{
			result+=ps;
			is.ignore();
		}
	}
}
/// @param decode causes to don't decode cestcaped chars and return value - is now mush chars to ignore after parse
int CBasicBlockParser::ParseString(bool active,wiparsestream &is,CVarValue& res,bool quoted,bool groupped,bool decode,bool keep_quote)
{
	wstring& str = res.set_string(L"");

	wchar_t qtchs=wchar_classify::eof();
	wchar_t qtch=wchar_classify::eof();

	if(quoted)
	{
		qtchs = is.peek();
		qtch = wchar_classify::quote_pair(qtchs);
		if(!qtch)throw parse_exception(is,PERR_CUSTOM,L"unexpected quote");
		is.ignore();
	}
	ParseStringLexGroup(active,is,str,qtch,groupped,decode);
	if(quoted)
	{
		if(is.peek() != qtch)throw parse_exception(is,PERR_CUSTOM,L"unexpected end");

		if(keep_quote)
		{
			if(qtch == qtchs)
			{
				return 1;
			}
		}
		is.ignore();
	}
	return 0;
}

void CBasicBlockParser::ParseRegExp(bool active,wiparsestream &is,CVarValue const& data,CVarValue& res)
{
	wstring rexp;
	wstring rexp_to;
	CVarValue rgstr;
	CVarValue rgstr_to;

	bool subst=false;

	if(wchar_classify::alpha_ext(is.peek()))
	{
		wstring keyw;
		is >> wistream_keyword_ext(keyw);
		if(keyw == L"r" || keyw == L"replace")
		{
			subst = true;
		}
		else
		{
			throw parse_exception(is,PERR_INVALID_KEYWORD,keyw);
		}
	}

	if(subst)
	{
		ParseString(active,is,rgstr,true,false,false,true);
		ParseString(active,is,rgstr_to,true,false,false);
		rexp_to = rgstr_to.to_string();
	}
	else ParseString(active,is,rgstr,true,false,false);
	rexp = rgstr.to_string();
	
	bool passtrough=false;
	bool seq=false;
	while(!is.fail())
	{
		wchar_t mod = is.peek();
		if(mod == L'p')passtrough = true;
		else if(mod == L's')seq = true;
		else break;
		is.ignore();
	}
	if(!active)return;
		
	wstring str = data.to_string();

	if(subst)
	{
		boost::wregex re(rexp);
		res = (CVarValue)boost::regex_replace(str,re,rexp_to,boost::match_default | boost::format_all);
	}
	else
	{
		boost::wsmatch rvars;
		bool r = boost::regex_search(str,rvars,boost::wregex(rexp));

		if(!r)
		{
			CVarValue v,r;
			SetLocalVariable(active,L"0",v,r);
			res.set_nil();	
		}
		else
		{
			CVarValue v,r;
			int sz = rvars.size();	
			if(sz < 2) res = true;
			for(int i=0;i<rvars.size();i++)
			{
				if(rvars[i].matched)v = rvars[i].str();
				else v.set_nil();
				SetLocalVariable(active,lex_cast<wstring>(i),v,r);

				if(i==1)res = v;
				else if(i>1)res.append(v);
			}
		}
	}
}

void CBasicBlockParser::ParseExpression(bool active,wiparsestream &is,CVarValue& res,int stop_prio)
{
	// read first variable or unary operator
	ParseValue(active,is,res,this);
	ContinueExpression(active,is,res,stop_prio);
}

void CBasicBlockParser::Parse(bool active,wiparsestream &is,CVarValue& result,bool quoted)
{
	result.set_nil();
	bool single=false;

	if(quoted)
	{
		m_chQoute = wchar_classify::quote_pair(is.peek());
		if(!m_chQoute || m_chQoute == is.peek())
		{
			single = true;
		}
		else is.ignore();
	}
	while(!is.fail())
	{
		is >> wsp_nl_ucmt_scip;
		wchar_t ps = is.peek();
		if(wchar_classify::alpha_ext(ps)) // parse one of keywords
		{
			wstring keywrd;
			is >> wistream_keyword_ext(keywrd) >> wsp_nl_ucmt_scip;
			ParseKeyword(active,keywrd,is,result);
		}	
		else if(ps == m_chQoute || wchar_classify::eof(ps)) // end of block;
		{
			is.ignore();
			return;
		}
		else if(ps == L';')
		{
			opresult=0;
			is.ignore();
		}		
		else if(EndOfStatsment(ps))
		{
			throw parse_exception(is,PERR_CUSTOM,L"unexpected symbol");
			return;
		}
		else ParseExpression(active,is,result);
		if(single)break;
	}
}
void CBasicBlockParser::ParseParams( bool active,wiparsestream &is,CVarValue& params,int pmin,int pmax )
{
    if(!pmax)pmax=pmin;

    is >> wsp_nl_ucmt_scip;
    if(is.peek() == L'(')ParseBasicBlock(active,is,params);
    else 
    {
        if(pmin) ParseExpression(active,is,params);
        else
        {
            int pos = is.tellg();
            CBaseOperator* op = GetParserOperator(is);
            if(!op) ParseExpression(active,is,params);
            else is.seekg(pos);
        }
    }
    if(active && (params.size() < pmin || 
        pmax && params.size() > pmax) ) throw parse_exception(is,PERR_INVALID_PARAMS);
}

CModuleParser::CModuleParser()
{
    m_pRootModule=0;
    m_pParentModule=0;
    m_pWorkspace=0;
}
 
bool CModuleParser::Init(IDCMapCore* pCore,IDCMapWorkspace *pWorkspace)
{
    m_pCore = pCore;
    m_pWorkspace = pWorkspace;
    return true;
}

bool CModuleParser::InitObject()
{
    if(m_pParent)
    {
        m_pParentModule = m_pParent->GetModule();
        m_pRootModule = m_pParentModule->m_pRootModule;  
    }
    else
    {
        m_pRootModule = this;
    }
    return true;
}

bool CModuleParser::SafeExecute(CVarValue val,CVarValue param,CVarValue& res)
{
    try
    {
        pRsrv = m_pCore->GetCoreUI()->GetConsoleFidder();
        val.call(param,res);
    }
    catch(parse_exception& e)
    {
        m_pCore->GetCoreUI()->ErrorMsg(e.info());	
        return false;
    }
    catch(die_exception& e)
    {
        m_pCore->GetCoreUI()->ErrorMsg(ToWide(string("Script failed ")+e.what()).c_str());	
        return false;
    }
    catch(std::exception& e)
    {
        m_pCore->GetCoreUI()->ErrorMsg(ToWide((string)"Script Error "+e.what()).c_str());	
        return false;
    }
    return true;
}

bool CModuleParser::SafeEval(dcmapDCSTR srcipt,CVarValue param,CVarValue& res )
{
    wstring msg;
    try
    {
        return Eval(srcipt,param,res);
    }
    catch(parse_exception& e)
    {
        msg = e.info()	;
    }
    catch(die_exception& e)
    {
        msg = ToWide(string("Script failed ")+e.what());
    }
    catch(std::exception& e)
    {
        msg = ToWide((string)"Script Error "+e.what());
    }
    m_pCore->GetCoreUI()->ErrorMsg(msg.c_str());	
    return false;
}

bool CModuleParser::Eval(dcmapDCSTR srcipt,CVarValue param,CVarValue& res )
{
    if(!srcipt || !srcipt[0])return false;
    wiparsestream is;
    is.setup(srcipt);

    CBlockParser prs;
    prs.Parent(this);
    prs.Rights(EParserRights(m_Rights & ~PRSR_DECLARE_OBJ));
    prs.Parse(true,is,res);

    return true;
}

void* DCMAPAPI CModuleParser::CastTo( DCMapInterfaceID idInterface )
{
    DCMAP_MODULE_SUPPORTS(IDCMapScriptModule);
	DCMAP_MODULE_SUPPORTS(IDCMapModule);
	DCMAP_MODULE_SUPPORTS(IDCMapScriptable);
	DCMAP_MODULE_SUPPORTS(IDCMapObject);
  return 0;
}

bool CModuleParser::ParseCustomKeyword( bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt )
{
    if(kw == L"handle_msg")
    {
        CVarValue params;
        cnt->ParseParams(active,is,params,2);
        if(active)
        {
            if(!cnt->Permited(PRSR_MSG_HANDLE))throw parse_exception(is,PERR_INSUFFICIENT_RIGHRS);  
            m_handlers[params[0].to_real()].action_handle = params[1];
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    } 
    return Inherited::ParseCustomKeyword(active,kw,is,res,expr,cnt);
}

bool CModuleParser::DbSafeExecute(CVarValue val,CVarValue param,CVarValue& res)
{
	m_pCore->LockDataUpdate();
	bool r = SafeExecute(val,param,res);
	m_pCore->UnlockDataUpdate();
	return r;
}

bool CModuleParser::UIExecute(CVarValue val,CVarValue param,CVarValue& res)
{
	bool r = DbSafeExecute(val,param,res);
	if(pRsrv)pRsrv->Hide();
	//stream.setup(buffer);
	return r;
}
dcmapLRESULT DCMAPAPI CModuleParser::OnMessage(int Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender)
{
	dcmapLRESULT lres = 0;
	switch(Msg)
	{
	case DCMM_PRE_DESTROY:
		Destruct();
		break;
	case DCMM_UI_COMMAND:
		{
			if(wParam <= m_vecHandlers.size())
			{
				if(!m_vecHandlers[wParam].action_handle.is_nil())
				{
					CVarValue param,res;
					if(lParam) param = *(CDCMapVarValue*)((IDCMapVarValue*)lParam);			
					UIExecute(m_vecHandlers[wParam].action_handle,param,res);
				}
			}
			break;
		}
	case DCMM_UI_TEST:
		{
			if(wParam <= m_vecHandlers.size())
			{
				if(!m_vecHandlers[wParam].test_handle.is_nil())
				{
					CVarValue param,res;
					if(lParam) param = *(CDCMapVarValue*)((IDCMapVarValue*)lParam);			
					UIExecute(m_vecHandlers[wParam].test_handle,param,res);
					lres = res.to_bool()?DCMM_SUCCESS:DCMM_FAIL;
				}
			}
			break;
		}	
	}

    MsgHandlers::iterator it = m_handlers.find(Msg);
    if(it != m_handlers.end())
    {
        // decode message
        
        // pass by default
        CVarValue var,res;
        var.set_vector();
        var.append_value(CVarValue(Msg));
        var.append_value(CVarValue(wParam));
        var.append_value(CVarValue(lParam));
        it->second.action_handle.call(var,res);

		lres = 0;
		for(int i=0;i<res.size();i++)
		{
			lres |= (dcmapLRESULT)res[i].to_real();
		}
    }
	return lres;
}

bool DCMAPAPI CModuleParser::CallProc( dcmapWCSTR Proc,IDCMapVarValue* Params,IDCMapVarValue* Result )
{
	CVarValue pval = GetLocalVariable(Proc);
	if(pval.callable())
	{
		CVarValue prm;
		if(Params)prm = *(CDCMapVarValue*)Params;
		if(Result)
		{
			pval.call(prm, *(CDCMapVarValue*)Result);
		}
		else
		{
			CVarValue res;
			pval.call(prm, res);
		}
		return true;
	}
	return false;
}

CObjectParser::~CObjectParser()
{
	Destruct();
	_ASSERT(m_iRefCount<=2);
}

void CDBBlockParser::Destruct()
{
	Clear();
	Inherited::Destruct();
}