#pragma once
#include <hash_map>
#include "varvalue.h"


struct ModuleRegistrator
{
	virtual ParseObjectCreatorCallback GetCreator()=0;
	virtual bool RegisterModule(wstring const& name,CVarValue const& module,CVarValue const& params,CBasicBlockParser* cnt){return true;}
};

template<class Module>
class CModuleReg : public ModuleRegistrator
{
	static CObjectParser* Creator()
	{
		return new Module();
	}
	ParseObjectCreatorCallback GetCreator(){return Creator;}
};

class CModuleAutoRegMan;
extern CModuleAutoRegMan* g_pModuleRegMan;
class CModuleAutoRegMan
{
	typedef std::map<wstring,ModuleRegistrator*> MapModules;
	MapModules m_mapMod;

	ModuleRegistrator* DoGetModule(wstring const& keyw)
	{
		MapModules::iterator it = m_mapMod.find(keyw);
		if(it !=  m_mapMod.end())
		{
			return it->second;
		}
		return 0;
	}
public:
	static void Register(wstring const& keyw,ModuleRegistrator* pMod)
	{
		if(!g_pModuleRegMan) g_pModuleRegMan = new CModuleAutoRegMan;
		g_pModuleRegMan->m_mapMod[keyw] = pMod;
	}

	static ModuleRegistrator* GetModule(wstring const& keyw)
	{
		if(!g_pModuleRegMan) g_pModuleRegMan = new CModuleAutoRegMan;
		return g_pModuleRegMan->DoGetModule(keyw);
	}
};

template<class Module>
struct ModuleAutoReg
{
	ModuleAutoReg(dcmapDCSTR kw)
	{
		static CModuleReg<Module> Reg;
		CModuleAutoRegMan::Register(kw,&Reg);					
	}
};

template<class Module>
struct ModuleAutoRegEx
{
	ModuleAutoRegEx(dcmapDCSTR kw)
	{
		static Module Reg;
		CModuleAutoRegMan::Register(kw,&Reg);					
	}
};


#define REG_MODULE(T,N)  static ModuleAutoReg<T> sModuleAutoReg##T##N(L#N);

#define REG_MODULEX(T,N)  static ModuleAutoRegEx<T> sModuleAutoReg##T##N(L#N);
