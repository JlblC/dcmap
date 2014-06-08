// dcmapsys.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../dcmapsyslib.h"
#include "dcmapcore.h"

#include <g_text.h>
#include <g_std_file.h>
#include <g_file_istream.h>

using namespace std;
using namespace gsys;

#define MSG_RACE_CHANGED DCMM_USER+0  

struct CGeoValFilter : public IDCMapPlanetColorProvider
{
	virtual char const* DCMAPAPI ModuleID(){return "CGeoValFilter";};

	IDCMapGeoValueCalculator* m_pGeoCalc;

	IDCMapRacesDataStorage* m_pRaces;
	DCMapRaceInfo m_RaceInfo;
	IDCMapWorkspace* m_pWorkspace;

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pWorkspace = workspace;

		m_pGeoCalc = workspace->GetServiceTyped<IDCMapGeoValueCalculator>("GeoValueCalculator");
		if(!m_pGeoCalc)return false;

		DCMAP_GET_STORAGE(core,m_pRaces,IDCMapRacesDataStorage,races);
		return true;
	}

	void  DCMAPAPI LoadConfig(IDCMapConfig* pConfig)
	{
		DCMapConfigWrapper cfg(pConfig);

		cfg.section("GeoValueFrame");
		string Race;
		cfg["Race"] >> Race;
		if(m_pRaces->SelectName(Race.c_str()))
		{
			m_RaceInfo = *m_pRaces->GetData();
		}
	}

	void UpdateRace(dcmapDBREF ref)
	{
		if(!m_pRaces->SelectID(ref))
		{
			
		}
		else
		{
			if(m_RaceInfo.id != *m_pRaces->GetID())
			{
				m_RaceInfo = *m_pRaces->GetData();
				m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0,this);
			}
			m_RaceInfo = *m_pRaces->GetData();
			// force update list
		}
	}

	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
	{
		switch(Msg)
		{
		case MSG_RACE_CHANGED:
			UpdateRace(dcmapDBREF(wParam,lParam));
			break;
		default:
			break;
		}
		return 0;
	};
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetColorProvider);
		return 0;
	}
	dcmapBYTE DCMAPAPI  ProcessPlanet(DCMapPlanetDisplayInfo* data,DCMapPlanetInfo const* pinf,dcmapCOLOR base, dcmapCOLOR& cl)
	{
		float g;
		if(m_pGeoCalc->CalculatePlanet(data->planet.id,&g)>0)
		{
			cl = dcmapRGB(50,250,50);
			return true;
		}	
		return false;
	}
	bool DCMAPAPI ActualizeData()
	{
		return true;
	}
};

DCMAP_REGISTER(CGeoValFilter,"PPlanetColorProvider","GeoValueColor");



