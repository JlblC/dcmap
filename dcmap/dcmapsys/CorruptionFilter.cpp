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

struct CCorruptionFilter : public IDCMapPlanetColorProvider
{
	IDCMapWorkspace* m_pWorkspace;
	IDCMapCore* m_pCore;

	IDCMapPlanetDataStoragePtr m_pPlanets;
	IDCMapPlayersDataStoragePtr m_pPlayers;

	dcmapCOLOR m_clCorrupton[6];

	CCorruptionFilter()
	{
		m_clCorrupton[0] = 16776960;
		m_clCorrupton[1] = 112129;
		m_clCorrupton[2] = 65535;
		m_clCorrupton[3] = 4227327;
		m_clCorrupton[4] = 255;
		m_clCorrupton[5] = 128;
	}

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		if(!m_pPlanets.init(core,"planets"))return false;
		if(!m_pPlayers.init(core,"players"))return false;

		m_pWorkspace = workspace;
		m_pCore = core;

		return true;
	}

	void  DCMAPAPI LoadConfig(IDCMapConfig* pConfig=0)
	{
		DCMapConfigWrapper cfg(m_pCore->GetConfig());
		cfg.section("CorruptionColor");

		cfg["Corruption0"] >> m_clCorrupton[0];
		cfg["Corruption10"] >> m_clCorrupton[1];
		cfg["Corruption25"] >> m_clCorrupton[2];
		cfg["Corruption50"] >> m_clCorrupton[3];
		cfg["Corruption85"] >> m_clCorrupton[4];
		cfg["Corruption95"] >> m_clCorrupton[5];
	}


	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
	{
		switch(Msg)
		{
		case DCMM_CONFIG_CHANGED:
			LoadConfig();
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
		_ASSERTOBJ(pinf);
		cl = base;
		if(!m_pPlayers->SelectID(pinf->owner))return 0;
		if(!(bool)m_pPlayers[DCMDB_PLAYER_CALCULATECORRUPTION])return 0;

		switch(pinf->corruption)
		{
		case 0: cl = m_clCorrupton[0];break;
		case 10: cl = m_clCorrupton[1];break;
		case 25: cl = m_clCorrupton[2];break;
		case 50: cl = m_clCorrupton[3];break;
		case 85: cl = m_clCorrupton[4];break;
		case 95: cl = m_clCorrupton[5];break;
		default:
			return 0;
		}
		return 2;
	}

	bool DCMAPAPI ActualizeData()
	{
		return m_pPlanets.actualize_state() | m_pPlanets.actualize_state();
	}
};

DCMAP_REGISTER(CCorruptionFilter,"PPlanetColorProvider","CorruptionColor");



