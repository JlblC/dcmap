#include "StdAfx.h"
#include "Keywords.h"
#include "utils.h"
#include <boost/regex.hpp>

struct CWebGetKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){

	int cp = param[1].to_real();
	if(!cp) cp = CP_UTF8;
	return (CVarValue)GetDCMapCore()->GetCoreUI()->WebGet(param[0].to_string().c_str(),cnt->pRsrv,cp);
}};static KeywordAutoReg<CWebGetKW> sCWebGetKW(L"webget");


struct CWebPostKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){

	int cp = param[2].to_real();
	if(!cp) cp = CP_UTF8;

	return (CVarValue)GetDCMapCore()->GetCoreUI()->WebPost(param[0].to_string().c_str(),param[1].to_string().c_str(),cnt->pRsrv,cp);

}};static KeywordAutoReg<CWebPostKW> sCWebPostKW(L"webpost");

struct CGameGetKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,res);
	else cnt->ParseExpression(active,is,res);
	if(active)
	{
		res = GetDCMapCore()->GetCoreUI()->GameGet(res.to_string().c_str(),cnt->pRsrv);
	}
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}};static KeywordAutoReg<CGameGetKW> sCGameGetKW(L"dcget");

struct CGamePostKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,res);
	else cnt->ParseExpression(active,is,res);
	if(active)
	{
		if(res.size()<2)throw parse_exception(is,PERR_CUSTOM,L"webpost needs at least 2 parameters");
		res = GetDCMapCore()->GetCoreUI()->GamePost(res[0].to_string().c_str(),res[1].to_string().c_str(),cnt->pRsrv);
	}
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}};static KeywordAutoReg<CGamePostKW> sCGamePostKW(L"dcpost");

struct CGameLoginKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,res);
	else cnt->ParseExpression(active,is,res);

	if(active)
	{
		if(res.size()<2)throw parse_exception(is,PERR_CUSTOM,L"game_login needs at least 2 parameters");
		bool free = false;
		if(res.size()>2)free = res[2].to_bool();
		res = GetDCMapCore()->GetCoreUI()->GameLogin(res[0].to_string().c_str(),res[1].to_string().c_str(),free,cnt->pRsrv);
	}
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}};static KeywordAutoReg<CGameLoginKW> sCGameLoginKW(L"dclogin");

struct CGameLogoutKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt){
	is >> wsp_nl_ucmt_scip;
	if(is.peek() == L'(')cnt->ParseBasicBlock(active,is,res);
	else cnt->ParseExpression(active,is,res);
	if(active)res = GetDCMapCore()->GetCoreUI()->GameLogout(cnt->pRsrv);
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}};static KeywordAutoReg<CGameLogoutKW> sCGameLogoutKW(L"dclogout");

struct CXMLUnqouteKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	static boost::wregex re(L"(&amp;)|(&lt;)|(&gt;)");
	return (CVarValue)boost::regex_replace(param.to_string(),re,L"(?1&)(?2<)(?3>)",boost::match_default | boost::format_all);
}};
static KeywordAutoReg<CXMLUnqouteKW> sCXMLUnqouteKW(L"xmlunqoute");
static KeywordAutoReg<CXMLUnqouteKW> sCXMLUnqouteKW2(L"xmlunquote");

struct CXMLQuoteKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	static boost::wregex re(L"(&)|(<)|(>)");
	return (CVarValue)boost::regex_replace(param.to_string(),re,L"(?1&amp;)(?2&lt;)(?3&gt;)",boost::match_default | boost::format_all);
}};
static KeywordAutoReg<CXMLQuoteKW> sCXMLQuoteKW(L"xmlquote");

struct CURLEncodeKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	if(param.type() == VT_HASH)
	{
		wstring output;
		CVarValueMap const& hash = param.get_hash();
		bool start=true;
		_const_foreach(CVarValueMap,hash,it)
		{
			if(start) start=false;
			else output += L"&";

			output += it->first.to_string()+L"="+ToWide(dcmapURLEncodeW(it->second.to_string().c_str()));
		}
		return (CVarValue)output;
	}
	return (CVarValue)ToWide(dcmapURLEncodeW(param.to_string().c_str()));
}};
static KeywordAutoReg<CURLEncodeKW> sCURLEncodeKW(L"urlencode");

