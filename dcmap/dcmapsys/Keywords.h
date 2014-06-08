#pragma once
#include <hash_map>
#include <g_text.h>
#include "Parser.h"
using namespace gsys;


class CBaseKeyword
{
protected:
    wstring const* m_pKW;
	virtual CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt);
	virtual CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt,wiparsestream &is);
public:
	virtual bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt);
};
typedef stdext::hash_map<wstring,CBaseKeyword*> MapKeywords;

class CKeywordAutoRegMan;
extern CKeywordAutoRegMan* g_pKeywordAutoRegMan;
class CKeywordAutoRegMan
{
	friend struct StdKeywordAutoReg;
	friend bool ParseStaticKeywords(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt);
	MapKeywords m_mapKW;
public:
	static void RegisterKeyword(wstring const& keyw,CBaseKeyword* pKW)
	{
		if(!g_pKeywordAutoRegMan) g_pKeywordAutoRegMan = new CKeywordAutoRegMan;
		g_pKeywordAutoRegMan->m_mapKW[keyw] = pKW;
	}
};

template<class Module>
struct KeywordAutoReg
{
	KeywordAutoReg(dcmapDCSTR kw)
	{
		static Module m_sModule;
		CKeywordAutoRegMan::RegisterKeyword(kw,&m_sModule);					
	}
};

#define REG_KEYWORD(T,N)  static KeywordAutoReg<T> s##T##N(L#N);
