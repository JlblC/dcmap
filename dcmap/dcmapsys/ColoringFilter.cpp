#include "stdafx.h"
#include "../dcmapsyslib.h"
#include "dcmapcore.h"

#include <g_text.h>
#include <g_std_file.h>
#include <g_file_istream.h>

using namespace std;
using namespace gsys;

#define MSG_COLORING_CHANGED DCMM_USER+0  

struct CColoringFilter : public IDCMapPlanetDataFilter
{
	void DCMAPAPI Destroy(){delete this;}
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CColoringFilter();}
	virtual char const* DCMAPAPI ModuleID(){return "CColoringFilter";};

	IDCMapColoringDataStorage* m_pColoring;
	dcmapQWORD m_ColoringState;

	DCMapColoringInfo m_ColoringInfo;
	IDCMapWorkspace* m_pWorkspace;

	IDCMapPlanetColorProvider* m_pPlanetColor;
	IDCMapPlanetColorProvider* m_pBackgroundColor;
	IDCMapPlanetColorProvider* m_pBorderColor;
	IDCMapPlanetColorProvider* m_pPlanetBorderColor;
	IDCMapPlanetColorProvider* m_pMarkerColor;

	typedef std::map<string,IDCMapPlanetColorProvider*> MapProviders;
	MapProviders m_mapProviders;

	bool Changed;
	bool HasFilter;

	CColoringFilter()
	{
		m_pPlanetColor=0;
		m_pBackgroundColor=0;
		m_pBorderColor=0;
		m_pPlanetBorderColor=0;
		HasFilter=false;
		Changed=false;
	}

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pWorkspace = workspace;
		DCMAP_GET_STORAGE(core,m_pColoring,IDCMapColoringDataStorage,coloring);

		IDCMapSys* pSys = GetDCMapSys();
		// Get aviable coloring plugins

		vector<string> modules;
		pSys->GetClassPluginNamesT("PPlanetColorProvider",modules);

		_foreach_array(modules,i)
		{
			IDCMapPlanetColorProvider* pProv = 
				m_pWorkspace->CreateModuleTyped<IDCMapPlanetColorProvider>("PPlanetColorProvider",modules[i].c_str());
			if(pProv)m_mapProviders[modules[i]] = pProv;
		}

		m_pColoring->FillDefaults(&m_ColoringInfo);

		m_ColoringState = m_pColoring->GetStateIndex();

		return true;
	}

	void  DCMAPAPI LoadConfig(IDCMapConfig* pConfig)
	{
		DCMapConfigWrapper cfg(pConfig);
		cfg.section("PlanetColoring");
		wstring id;
		cfg["Coloring"] >> id;

		if(m_pColoring->SelectWideName(id.c_str()))
		{
			m_ColoringInfo = *m_pColoring->GetData();
			UpdatePlugins();
			HasFilter=true;
		}
	}
	void  DCMAPAPI SaveConfig(IDCMapConfig* pConfig)
	{
		DCMapConfigWrapper cfg(pConfig);
		cfg.section("PlanetColoring");
		cfg["Coloring"] << (wstring)m_ColoringInfo.name;
	}

	void UpdatePlugins()
	{
		m_pPlanetColor=0;
		m_pBackgroundColor=0;
		m_pBorderColor=0;
		m_pPlanetBorderColor=0;
		m_pMarkerColor=0;

		MapProviders::iterator it;
		it = m_mapProviders.find(m_ColoringInfo.planet);
		if(it!=m_mapProviders.end())m_pPlanetColor = it->second;

		it = m_mapProviders.find(m_ColoringInfo.background);
		if(it!=m_mapProviders.end())m_pBackgroundColor = it->second;

		it = m_mapProviders.find(m_ColoringInfo.cellborder);
		if(it!=m_mapProviders.end())m_pBorderColor = it->second;

		it = m_mapProviders.find(m_ColoringInfo.planetborder);
		if(it!=m_mapProviders.end())m_pPlanetBorderColor = it->second;

		it = m_mapProviders.find(m_ColoringInfo.marker);
		if(it!=m_mapProviders.end())m_pMarkerColor = it->second;
	}

	void UpdateData()
	{
		if(m_pColoring->SelectID(m_ColoringInfo.id))
		{
			m_ColoringInfo = *m_pColoring->GetData();
			UpdatePlugins();
			//m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0,this);
		}
		m_ColoringState = m_pColoring->GetStateIndex();
	}

	void UpdateColoring(dcmapDBREF ref)
	{
		if(m_pColoring->SelectID(ref))
		{
			if(m_ColoringInfo.id != *m_pColoring->GetID())
			{
				m_ColoringInfo = *m_pColoring->GetData();
				UpdatePlugins();
				HasFilter=true;
				Changed=true;
				m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0,this);
			}
		}
		else HasFilter=false;
	}

	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
	{
		switch(Msg)
		{
		case MSG_COLORING_CHANGED:
			UpdateColoring(dcmapDBREF(wParam,lParam));
			break;
		//case DCMM_DATA_CHANGED:
		//	UpdateData();
		//	break;
		default:
			break;
		}
		return 0;
	};
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetDataFilter);
		return 0;
	}

	bool DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* data,DCMapPlanetInfo const* pinf)
	{
		if(!HasFilter)return true;

		if(m_pPlanetColor)
		{
			data->iPlanetBody = m_pPlanetColor->ProcessPlanet(data,pinf,m_ColoringInfo.planet_color,data->clPlanetBody);
		}
		else data->iPlanetBody=0;

		if(m_pBackgroundColor)
		{
			data->iBackground = m_pBackgroundColor->ProcessPlanet(data,pinf,m_ColoringInfo.background_color,data->clBackground);
		}
		else data->iBackground=0;

		if(m_pBorderColor)
		{
			data->iCellBorder = m_pBorderColor->ProcessPlanet(data,pinf,m_ColoringInfo.cellborder_color,data->clCellBorder);
		}
		else data->iCellBorder=0;

		if(m_pPlanetBorderColor)
		{
			data->iPlanetBorder = m_pPlanetBorderColor->ProcessPlanet(data,pinf,m_ColoringInfo.planetborder_color,data->clPlanetBorder);
			//if(data->iPlanetBorder)	data->iPlanetBorder = 2;
		}
		else data->iPlanetBorder=0;

		if(m_pMarkerColor)
		{
			data->iMarker = m_pMarkerColor->ProcessPlanet(data,pinf,m_ColoringInfo.marker_color,data->clMarker);
		}	
		else data->iMarker=0;

		return true;
	}
	bool DCMAPAPI ActualizeData()
	{
		bool update = Changed;
		Changed = false;

		if(m_ColoringState != m_pColoring->GetStateIndex())
		{
			UpdateData();
			update = true;
		}

		if(m_pPlanetColor)
		{
			update |= m_pPlanetColor->ActualizeData();
		}
		if(m_pBackgroundColor)
		{
			update |= m_pBackgroundColor->ActualizeData();
		}
		if(m_pBorderColor)
		{
			update |= m_pBorderColor->ActualizeData();
		}
		if(m_pPlanetBorderColor)
		{
			update |= m_pPlanetBorderColor->ActualizeData();
		}		

		return update;
	}
};

DCMAP_REGISTER(CColoringFilter,"PPlanetFilter","000_CColoringFilter")




