#pragma once
#include "../dcmapbase.h"
#include "DCMapConfig.h"
#include "Utils.h"


bool ProcessMsgResult(dcmapLRESULT ret,dcmapLRESULT& result,dcmapDWORD Options);

template <class Target>
class ModuleManagerImpl : public Target
{
protected:
	IDCMapCoreEx* m_pCore;
	IDCMapFrameEx* m_pFrame;
	IDCMapWorkspaceEx* m_pWorkspace;
	CDCMapConfig m_Config;
	std::wstring m_strConfigFile;
protected:

	typedef std::set<IDCMapModule*> ObjectsSet;
	ObjectsSet m_Objects;

	typedef std::multimap<std::string,IDCMapModule*> ModulesIDMap;
	typedef std::pair<std::string,IDCMapModule*> StrModPair;
	ModulesIDMap m_mapModules;

	ModulesIDMap::iterator m_itCurrentMod;
	ModulesIDMap::iterator m_itEndMod;

	typedef std::map<std::string,IDCMapModule*> MapModules;
	MapModules m_Services;
	MapModules m_Daemons;

protected:
	void UnregisterMod(IDCMapModule* pObj)
	{
		m_Objects.erase(pObj);
		ModulesIDMap::iterator from = m_mapModules.begin();
		ModulesIDMap::iterator to = m_mapModules.end();
		while(from != to)
		{
			if(from->second == pObj)
			{
				from = m_mapModules.erase(from);
			}
			else from++;
		}
	}
public:
	IDCMapModule* DCMAPAPI CreateModule(char const* czClass, char const* czName)
	{
		if(!czClass || !czClass[0])return 0;
		if(!czName || !czName[0])return 0;

		IDCMapObject* pObj = g_pDCMapSys->CreateObject(czClass,czName);
		if(!pObj)return 0;

		IDCMapModule* pModule = pObj->GetFinalModule(); // problem here
		if(!pModule)
		{
			g_pDCMapSys->DestroyObject(pObj);
			return 0;
		}

		if(!pModule->Init(m_pCore,m_pWorkspace))
		{
			g_pDCMapSys->DestroyObject(pObj);
			return 0;
		}
		char const* szID = pModule->ModuleID();
		if(szID && szID[0])
		{
			m_mapModules.insert(StrModPair(szID,pModule));
		}
		m_mapModules.insert(StrModPair(std::string(czClass)+"/"+czName,pModule));

		m_itCurrentMod = m_mapModules.end();
		m_itEndMod = m_mapModules.end();

		m_Objects.insert(pModule);
		pModule->LoadConfig(&m_Config);

		return pModule;
	}

	void DCMAPAPI DestroyModule(IDCMapModule* pObj)
	{
		pObj->SaveConfig(&m_Config);
		UnregisterMod(pObj);
		//g_dcmapCore.DestroyWorkspaceModule(pObj);
	}

	bool DCMAPAPI SelectModuleByID(char const* czModuleID)
	{
		m_itCurrentMod = m_mapModules.lower_bound(czModuleID);
		m_itEndMod = m_mapModules.upper_bound(czModuleID);
		return m_itCurrentMod!=m_itEndMod;
	}

	IDCMapModule* DCMAPAPI Module()
	{
		if(m_itCurrentMod==m_itEndMod)return 0;
		return m_itCurrentMod->second;
	}

	bool DCMAPAPI Next()
	{
		if(m_itCurrentMod==m_itEndMod)return false;
		++m_itCurrentMod;
		if(m_itCurrentMod==m_itEndMod)return false;
		return true;
	}

	dcmapLRESULT DCMAPAPI BroadcastMessage(int Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapModule* pSender,dcmapDWORD Options)
	{
		dcmapLRESULT result = 0;
		dcmapLRESULT ret;

		ret = OnMessage(Msg,wParam,lParam,pSender);
		if(!ProcessMsgResult(ret,result,Options))return result;

		_foreach(ObjectsSet,m_Objects,it)
		{
			ret = (*it)->OnMessage(Msg,wParam,lParam,pSender);
			if(!ProcessMsgResult(ret,result,Options))return result;
		}
		return result;
	}

	void DCMAPAPI SetConfigFile(dcmapDCSTR Filename)
	{
		m_strConfigFile = Filename;
	}

	void DCMAPAPI ConfigureFromFile(dcmapDCSTR szFilename)
	{
		wstring BaseFilename;
		if(szFilename && szFilename[0])BaseFilename = szFilename;
		else BaseFilename = m_strConfigFile;

		wstring Filename = gsys::GetAppStartupDirW()+BaseFilename;
		wstring TplFilename = gsys::GetAppStartupDirW()+L"tpl\\"+BaseFilename;

		if(gsys::FileExists(Filename))
		{
			m_Config.Load(Filename.c_str());
		}
		else
		{
			m_Config.Load(TplFilename.c_str());
		}
	}

	void DCMAPAPI SaveConfig(dcmapDCSTR szFilename)
	{
		wstring cfg = m_strConfigFile;
		if(szFilename && szFilename[0])cfg = szFilename;

		_foreach(ObjectsSet,m_Objects,it)
		{
			(*it)->SaveConfig(&m_Config);
		}

		if(!cfg.empty())m_Config.Save((gsys::GetAppStartupDirW()+cfg).c_str());
	}

	IDCMapModule* DCMAPAPI GetService(const char* Name)
	{
		MapModules::iterator it = m_Services.find(Name);
		if(it != m_Services.end())return it->second;
		IDCMapModule* mod = CreateModule("PService",Name);
		if(mod)m_Services[Name] = mod;
		return mod;	
	}

	IDCMapConfig* DCMAPAPI GetConfig()
	{
		return &m_Config;	
	}

	IDCMapCore* DCMAPAPI GetCore()
	{
		return m_pCore;	
	}

	IDCMapWorkspace* DCMAPAPI GetWorkspace()
	{
		return m_pWorkspace;	
	}

	IDCMapFrame* DCMAPAPI GetFrame()
	{
		return m_pFrame;	
	}
};

