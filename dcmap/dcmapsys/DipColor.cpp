// dcmapsys.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../dcmapsyslib.h"

#include <g_text.h>
#include <g_std_file.h>
#include <g_file_istream.h>

using namespace std;
using namespace gsys;

struct CDipColorPlug : public IDCMapPlanetColorProvider
{
	void DCMAPAPI Destroy(){delete this;}
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CDipColorPlug();}

	//dcmapQWORD PlayersState;

	dcmapCOLORREF Colors[EDCMapDiplomatyRelationNum];
	IDCMapCore* m_pCore;
	IDCMapWorkspace* m_pWorkspace; 

	DCMapDataSourcePtr<IDCMapDipDataStorage>  m_pDip;
	DCMapDataSourcePtr<IDCMapPlayersDataStorage>  m_pPlayers;

	CDipColorPlug()
	{
		for(int i=0;i<EDCMapDiplomatyRelationNum;i++)
		{
			Colors[i] =dcmapRGB(90,60,160);
		}
		Colors[DCMAP_DIP_UNDEFINED] = dcmapRGB(90,60,160);
		Colors[DCMAP_DIP_ALLY] = dcmapRGB(50,250,50);
		Colors[DCMAP_DIP_ENEMIES] = dcmapRGB(250,50,50);
		Colors[DCMAP_DIP_VASSAL] = dcmapRGB(100,150,250);
		Colors[DCMAP_DIP_LORD] = dcmapRGB(250,250,50);
		Colors[DCMAP_DIP_MULT] = dcmapRGB(250,50,250);
		Colors[DCMAP_DIP_ALLY] = dcmapRGB(50,250,50);
		Colors[DCMAP_DIP_EMPTY] = dcmapRGB(128,128,128);
	}

	void LoadColorConfig()
	{
		DCMapConfigWrapper cfg(m_pCore->GetConfig());
		cfg.section("DipColor");
		for(int i=0;i<EDCMapDiplomatyRelationNum;i++)
		{
			cfg["Color"+to_str(i)] >> Colors[i];
		}		
	}

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		if(!m_pDip.init(core,"dip"))return false;
		if(!m_pPlayers.init(core,"players"))return false;

		m_pCore = core;
		m_pWorkspace = workspace;
		//DCMAP_GET_STORAGE(core,m_pPlayers,IDCMapPlayersDataStorage,players);
		//PlayersState = m_pPlayers->GetStateIndex();
		LoadColorConfig();
		return true;
	}

	bool DCMAPAPI ActualizeData()
	{
		return m_pPlayers.actualize_state() || m_pDip.actualize_state();
	}

	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
	{
		switch(Msg)
		{
		case DCMM_CONFIG_CHANGED:
			LoadColorConfig();
			m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0,this);
			break;
		}
		return 0;
	};
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetColorProvider);
		return 0;
	}
	dcmapBYTE DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf,dcmapCOLOR base,dcmapCOLOR& cl)
	{
		cl = dcmapRGB(150,150,150);
		if(m_pPlayers->SelectID(pinf->owner))
		{
			dcmapDBREF dip = m_pPlayers->GetData()->dip;
			if(m_pDip->SelectID(dip))
			{
				cl = m_pDip->GetData()->color1;
			}
			else cl = Colors[DCMAP_DIP_UNDEFINED];
		}
		else
		{
			cl = Colors[DCMAP_DIP_EMPTY];
			//cl = dcmapRGB(80,50,150);
		}
		return true;
	}
};

struct CPlayerColor1Plug : public IDCMapPlanetColorProvider
{
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CPlayerColor1Plug();}
	IDCMapPlayersDataStorage* m_pPlayers;
	dcmapQWORD PlayersState;
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		DCMAP_GET_STORAGE(core,m_pPlayers,IDCMapPlayersDataStorage,players);
		PlayersState = m_pPlayers->GetStateIndex();
		return true;
	}
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetColorProvider);
		return 0;
	}
	dcmapBYTE DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf,dcmapCOLOR base,dcmapCOLORREF& cl)
	{
		if(m_pPlayers->SelectID(pinf->owner))
		{
			cl = m_pPlayers->GetData()->color1;
			return true;
		}
		return false;
	}

	bool DCMAPAPI ActualizeData()
	{
		if(PlayersState != m_pPlayers->GetStateIndex())
		{
			PlayersState = m_pPlayers->GetStateIndex();
			return true;
		}
		return false;
	}
};

struct CPlayerColor2Plug : public IDCMapPlanetColorProvider
{
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CPlayerColor2Plug();}
	IDCMapPlayersDataStorage* m_pPlayers;
	dcmapQWORD PlayersState;
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		DCMAP_GET_STORAGE(core,m_pPlayers,IDCMapPlayersDataStorage,players);
		PlayersState = m_pPlayers->GetStateIndex();
		return true;
	}
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetColorProvider);
		return 0;
	}
	dcmapBYTE DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf,dcmapCOLOR base,dcmapCOLORREF& cl)
	{
		if(m_pPlayers->SelectID(pinf->owner))
		{
			cl = m_pPlayers->GetData()->color2;
			return true;
		}
		return false;
	}

	bool DCMAPAPI ActualizeData()
	{
		if(PlayersState != m_pPlayers->GetStateIndex())
		{
			PlayersState = m_pPlayers->GetStateIndex();
			return true;
		}
		return false;
	}
};

struct COrgColor1Plug : public IDCMapPlanetColorProvider
{
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new COrgColor1Plug();}
	IDCMapPlayersDataStorage* m_pPlayers;
	IDCMapOrganizationDataStorage* m_pOrg;
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		DCMAP_GET_STORAGE(core,m_pPlayers,IDCMapPlayersDataStorage,players);
		DCMAP_GET_STORAGE(core,m_pOrg,IDCMapOrganizationDataStorage,organizations);
		return true;
	}
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetColorProvider);
		return 0;
	}
	dcmapBYTE DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf,dcmapCOLOR base,dcmapCOLORREF& cl)
	{
		if(m_pPlayers->SelectID(pinf->owner))
		{
			if(m_pOrg->SelectID(m_pPlayers->GetData()->organization))
			{
				cl = m_pOrg->GetData()->color1;
				return true;
			}
		}
		return false;
	}

	bool DCMAPAPI ActualizeData()
	{
		return true;
	}
};

struct COrgColor2Plug : public IDCMapPlanetColorProvider
{
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new COrgColor2Plug();}
	IDCMapPlayersDataStorage* m_pPlayers;
	IDCMapOrganizationDataStorage* m_pOrg;
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		DCMAP_GET_STORAGE(core,m_pPlayers,IDCMapPlayersDataStorage,players);
		DCMAP_GET_STORAGE(core,m_pOrg,IDCMapOrganizationDataStorage,organizations);
		return true;
	}
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetColorProvider);
		return 0;
	}
	dcmapBYTE DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf,dcmapCOLOR base,dcmapCOLORREF& cl)
	{
		if(m_pPlayers->SelectID(pinf->owner))
		{
			if(m_pOrg->SelectID(m_pPlayers->GetData()->organization))
			{
				cl = m_pOrg->GetData()->color2;
				return true;
			}
		}
		return false;
	}

	bool DCMAPAPI ActualizeData()
	{
		return true;
	}
};

struct CPlanetColor1Plug : public IDCMapPlanetColorProvider
{
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CPlanetColor1Plug();}
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		return true;
	}
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetColorProvider);
		return 0;
	}
	dcmapBYTE DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf,dcmapCOLOR base,dcmapCOLORREF& cl)
	{
		cl = pinf->color1;
		return true;
	}
	bool DCMAPAPI ActualizeData(){return true;}
};

struct CPlanetColor2Plug : public IDCMapPlanetColorProvider
{
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CPlanetColor2Plug();}
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		return true;
	}
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetColorProvider);
		return 0;
	}
	dcmapBYTE DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf,dcmapCOLOR base,dcmapCOLORREF& cl)
	{
		cl = pinf->color2;
		return true;
	}

	bool DCMAPAPI ActualizeData()
	{
		return true;
	}
};

struct CColorPlug : public IDCMapPlanetColorProvider
{
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface){DCMAP_MODULE_SUPPORTS(IDCMapPlanetColorProvider);return 0;}
	bool DCMAPAPI ActualizeData(){return true;}
	dcmapBYTE DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf,dcmapCOLOR base,dcmapCOLORREF& cl)
	{
		cl = base;
		return true;
	}
};

DCMAP_REGISTER(CColorPlug,"PPlanetColorProvider","Color");
DCMAP_REGISTER(CDipColorPlug,"PPlanetColorProvider","DipColor");
DCMAP_REGISTER(CPlayerColor1Plug,"PPlanetColorProvider","PlayerColor1");
DCMAP_REGISTER(CPlayerColor2Plug,"PPlanetColorProvider","PlayerColor2");
DCMAP_REGISTER(COrgColor1Plug,"PPlanetColorProvider","OrgColor1");
DCMAP_REGISTER(COrgColor2Plug,"PPlanetColorProvider","OrgColor2");
DCMAP_REGISTER(CPlanetColor1Plug,"PPlanetColorProvider","PlanetColor1");
DCMAP_REGISTER(CPlanetColor2Plug,"PPlanetColorProvider","PlanetColor2");

struct CPlanetIconsProcessor : public IDCMapPlanetDataFilter
{
	IDCMapIconsDataStoragePtr m_pIcons;
	IDCMapPlayersDataStoragePtr m_pPlayers;
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CPlanetIconsProcessor();}
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		if(!m_pIcons.init(core))return false;
		if(!m_pPlayers.init(core))return false;
		return true;
	}
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetDataFilter);
		return 0;
	}
	bool DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* data,DCMapPlanetInfo const* pinf)
	{
		if(m_pIcons->SelectID(pinf->icon1))
		{
			data->Icon1 = pinf->icon1.id;
		}

		if(m_pPlayers->SelectID(pinf->owner))
		{
			if(m_pIcons->SelectID(m_pPlayers[DCMDB_PLAYER_ICON]))
			{
				data->Icon3 = m_pIcons->GetID()->id;
			}
		}

		if(m_pIcons->SelectID(pinf->icon2))
		{
			data->Icon2 = pinf->icon2.id;
		}
		return true;
	}

	bool DCMAPAPI ActualizeData()
	{
		return m_pIcons.actualize_state() || m_pPlayers.actualize_state();
	}
};
DCMAP_REGISTER(CPlanetIconsProcessor,"PPlanetFilter","000_IconsProcessor");







