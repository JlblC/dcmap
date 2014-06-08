#include "stdafx.h"
#include "../dcmapsyslib.h"
#include "dcmapcore.h"

#include <g_text.h>
#include <g_std_file.h>
#include <g_file_istream.h>

#include "utils.h"

using namespace std;
using namespace gsys;

class CPlanetContextManager : public IDCMapPlanetContextManager
{
	IDCMapPlanetDataStoragePtr m_pPlanets;
	std::vector<IDCMapPlanetHintSource*> m_vecHints;

	IDCMapCore* m_pCore;
	IDCMapWorkspace* m_pWorkspace;

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetContextManager);
		return 0;
	}

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pCore = core;
		m_pWorkspace = workspace;

		if(!m_pPlanets.init(core))return false;

		LoadHintModules();

		return true;
	}

	void LoadHintModules()
	{
		for(int i=0;i<m_vecHints.size();i++)
		{
			m_pWorkspace->DestroyModule(m_vecHints[i]);
		}
		m_vecHints.clear();

		std::list<string> lst;
		if(g_pDCMapSys->SelectClass("PlanetHintSource"))do{
			lst.push_back(g_pDCMapSys->GetPluginName());
		}while(g_pDCMapSys->Next());
		lst.sort();

		_foreach(std::list<string>,lst,it)
		{
			IDCMapPlanetHintSource* hint = m_pWorkspace->CreateModuleTyped<IDCMapPlanetHintSource>("PlanetHintSource",it->c_str());
			if(hint)m_vecHints.push_back(hint);
		}
	}

	dcmapWCSTR DCMAPAPI GetPlanetHint(dcmapDBREF const& ref)
	{
		if(!m_pPlanets->SelectID(ref))return L"";

		static wstring hint;
		hint.clear();
		for(int i=0;i<m_vecHints.size();i++)
		{
			dcmapWCSTR str = m_vecHints[i]->GetHint(ref);
			if(str && str[0])
			{
				hint += (wstring)L"<p>"+str+L"</p>";
			}
		}
		return hint.c_str();
	}

	dcmapWCSTR DCMAPAPI GetCellHint(int x,int y)
	{
		if(m_pPlanets->SelectCoord(x,y)) 
			return GetPlanetHint(*m_pPlanets->GetID());

		static wstring hint;
		hint.clear();
		for(int i=0;i<m_vecHints.size();i++)
		{
			dcmapWCSTR str = m_vecHints[i]->GetCellHint(x,y);
			if(str && str[0])
			{
				hint += (wstring)L"<p>"+str+L"</p>";
			}
		}
		return hint.c_str();
	}
};
DCMAP_REGISTER(CPlanetContextManager,"PService","PlanetContextManager")


struct CPlanetHint_Base : public IDCMapPlanetHintSource
{
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetHintSource);
		return 0;
	}
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		if(!m_pPlanets.init(core))return false;
		return true;
	}
	dcmapWCSTR DCMAPAPI GetCellHint(int x,int y)
	{
		return 0;
	}
	DCMapDataSourcePtr<IDCMapPlanetDataStorage> m_pPlanets;
};

struct CPlanetHint_Name : public CPlanetHint_Base
{
	dcmapDCSTR m_FormatString;
	dcmapDCSTR m_FormatString2;

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_FormatString = g_pLocalStrings->GetStringW("PlanetHintNameFormat");
		m_FormatString2 = g_pLocalStrings->GetStringW("PlanetHintCoordFormat");

		if(!m_pPlanets.init(core))return false;
		return true;
	}
	dcmapWCSTR PrintOut()
	{
		static std::wstring res;
		DCMapPlanetInfo const& pinf = *m_pPlanets->GetData();

		res = boost::str( wformat(m_FormatString) % pinf.x % pinf.y % pinf.caption);

		//res = gsys::to_str(pinf.x)+":"+gsys::to_str(pinf.y)+" - <font align=right color=aa0000><b>"+pinf.caption+"</b><font>";
		return res.c_str();
	}
	dcmapWCSTR DCMAPAPI GetHint(dcmapDBREF const& ref)
	{
		if(m_pPlanets->SelectID(ref))
		{
			return PrintOut();
		}
		return 0;
	}
	dcmapWCSTR DCMAPAPI GetCellHint(int x,int y)
	{
		if(m_pPlanets->SelectCoord(x,y))
		{
			return PrintOut();
		}
		else
		{
			static std::wstring res;

			res = (boost::str( wformat(m_FormatString2) % x % y ));

			//res = string("<b>")+gsys::to_str(x)+":"+gsys::to_str(y)+"</b>";
			return res.c_str();
		}
	}
};

DCMAP_REGISTER(CPlanetHint_Name,"PlanetHintSource","000_Name");

struct CPlanetHint_Owner : public CPlanetHint_Base
{
	dcmapDCSTR m_FormatString;

	DCMapDataSourcePtr<IDCMapPlayersDataStorage> m_pPlayers;

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_FormatString = g_pLocalStrings->GetStringW("PlanetHintOwnerFormat");

		if(!m_pPlanets.init(core))return false;
		if(!m_pPlayers.init(core))return false;

		return true;
	}
	dcmapWCSTR DCMAPAPI GetHint(dcmapDBREF const& ref)
	{
		static std::wstring res;
		if(m_pPlanets->SelectID(ref))
		{
			if(m_pPlayers->SelectID(m_pPlanets->GetData()->owner))
			{
				res = boost::str( wformat(m_FormatString) % m_pPlayers->GetWideName());
				//res = string("<font color=660000><b>")+m_pPlayers->GetName()+"</b></font>";
				return res.c_str();
			}
		}
		return 0;
	}
};
DCMAP_REGISTER(CPlanetHint_Owner,"PlanetHintSource","010_Owner");

struct CPlanetHint_Geo : public CPlanetHint_Base
{
	const wchar_t* m_FormatString;

	DCMapDataSourcePtr<IDCMapGeoDataStorage> m_pGeo;
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_FormatString = g_pLocalStrings->GetStringW("PlanetHintGeoFormat");
		if(!m_pGeo.init(core,"geo"))return false;
		return CPlanetHint_Base::Init(core,workspace);
	}

	dcmapWCSTR DCMAPAPI GetHint(dcmapDBREF const& ref)
	{
		if(m_pPlanets->SelectID(ref))
		{
			const DCMapPlanetInfo* pinf = m_pPlanets->GetData();
			static wstring str;
			str = boost::str( wformat(m_FormatString) %(int)pinf->o%(int)pinf->e%(int)pinf->m%(int)pinf->t%(int)pinf->s);
			return str.c_str();
		}
		return 0;
	}
	dcmapWCSTR DCMAPAPI GetCellHint(int x,int y)
	{
		if(m_pGeo->SelectCoord(x,y))
		{
			const DCMapPlanetParamInfo* pinf = m_pGeo->GetData();
			static wstring str;
			str = boost::str( wformat(m_FormatString) %(int)pinf->o%(int)pinf->e%(int)pinf->m%(int)pinf->t%(int)pinf->s);
			return str.c_str();
		}
		return 0;
	}
};
DCMAP_REGISTER(CPlanetHint_Geo,"PlanetHintSource","020_Geo");

struct CPlanetHint_Comment : public CPlanetHint_Base
{
	dcmapWCSTR DCMAPAPI GetHint(dcmapDBREF const& ref)
	{
		static std::wstring res;
		res.clear();
		if(m_pPlanets->SelectID(ref))
		{
			std::wstring note = m_pPlanets->GetWideNotes();
			bool lines = note.length()>10;
			if(lines)res+=L"<font size=4><b>-------------------------------------------------------------------------------------------------------------</b></font><br>";
			int from=0;
			while(true)
			{
				int pos = note.find_first_of(L'\n',from);
				if(pos!=std::string::npos)
				{
					res += note.substr(from,pos-from)+L"<br>";
					from=pos+1;
				}
				else break;
			}
			res += note.substr(from);
			if(lines)res+=L"<br><font size=4><b>-------------------------------------------------------------------------------------------------------------</</b></font>";			
		}
		return res.c_str();
	}

};
DCMAP_REGISTER(CPlanetHint_Comment,"PlanetHintSource","030_Comment");

struct CPlanetHint_Dist : public CPlanetHint_Base
{
	int x1,y1,x2,y2;

	const wchar_t* m_FormatString;
	DCMapDataSourcePtr<IDCMapGeoDataStorage> m_pGeo;

	CPlanetHint_Dist()
	{
		x1=y1=x2=y2=-1;
	}

	IDCMapPlanetManager* m_pPlanetMan;

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_FormatString = g_pLocalStrings->GetStringW("PlanetHintDistFormat");

		m_pPlanetMan = workspace->GetServiceTyped<IDCMapPlanetManager>("PlanetManager");
		if(!m_pPlanetMan)return false;
		if(!m_pPlanets.init(core))return false;
		return true;
	}

	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
	{
		switch (Msg)
		{
		case DCMM_PLANET_CHANGED:
			{
				if(!m_pPlanetMan->GetCurrentPlanetCoords(&x1,&y1))
				{
					x1=-1;
					y1=-1;
				}
			}
			break;
		case DCMM_POINT_PLANET:
			x2 = wParam;
			y2 = lParam;
			break;
		}
		return 0;
	}
	dcmapWCSTR DCMAPAPI GetHint(dcmapDBREF const& ref)
	{
		if(m_pPlanets->SelectID(ref))
		{
			return GetCellHint(m_pPlanets[DCMDB_PLANET_X],m_pPlanets[DCMDB_PLANET_Y]);
		}
		return 0;
	}
	dcmapWCSTR DCMAPAPI GetCellHint(int x,int y)
	{
		static wstring str;
		if(x1 >=0 && y1 >= 0 && (x1 != x2 || y1 != y2))
		{
			int x = x2;
			int y = y2;

			if( abs(x-x1) > abs((x+g_DCMapNumPlanets)-x1))
				x += g_DCMapNumPlanets;
			if( abs(y-y1) > abs((y+g_DCMapNumPlanets)-y1))
				y += g_DCMapNumPlanets;

			if( abs(x-x1) > abs((x-g_DCMapNumPlanets)-x1))
				x -= g_DCMapNumPlanets;
			if( abs(y-y1) > abs((y-g_DCMapNumPlanets)-y1))
				y -= g_DCMapNumPlanets;

			float dx = x1-x;
			float dy = y1-y;
			float d = sqrt(dx*dx+dy*dy);

			str = boost::str( wformat(m_FormatString) % x1 % y1 % x2 % y2 % d);
			return str.c_str();
		}
		return 0;
	}
};
DCMAP_REGISTER(CPlanetHint_Dist,"PlanetHintSource","030_Dist");
