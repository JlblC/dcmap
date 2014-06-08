// dcmapsys.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "../dcmapsyslib.h"
#include "dcmapcore.h"

using namespace std;
using namespace gsys;


struct CGeoValueService : public IDCMapGeoValueCalculator
{
	void DCMAPAPI Destroy(){delete this;}
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CGeoValueService();}

	IDCMapRacesDataStorage* m_pRaces;
	IDCMapPlanetDataStorage* m_pPlanets;

	DCMapRaceInfo m_RaceInfo;
	IDCMapWorkspace* m_pWorkspace;

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pWorkspace = workspace;
		DCMAP_GET_STORAGE(core,m_pRaces,IDCMapRacesDataStorage,races);
		DCMAP_GET_STORAGE(core,m_pPlanets,IDCMapPlanetDataStorage,planets);

        growth=0;
        mining=0;
        tmin=0;
        tmax=0;

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

	double growth;
	double mining;
	double tmin;
	double tmax;
	double min_growth;

	void UpdateRace(dcmapDBREF ref)
	{
		if(!m_pRaces->SelectID(ref))
		{
			growth=0;
			mining=0;
			tmin=0;
			tmax=0;
		}
		else
		{
			//if(m_RaceInfo.id != *m_pRaces->GetID())
			{
				m_RaceInfo = *m_pRaces->GetData();

				growth = m_pRaces->GetReal(DCMDB_RACE_GROWTH);
				mining = m_pRaces->GetReal(DCMDB_RACE_MINING);
				tmin = m_pRaces->GetReal(DCMDB_RACE_TMIN);
				tmax = m_pRaces->GetReal(DCMDB_RACE_TMAX);
				min_growth = m_pRaces->GetReal(DCMDB_RACE_MIN_GROWTH);

				//m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0,this);
			}
			m_RaceInfo = *m_pRaces->GetData();
			// force update list
		}
	}

	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
	{
		switch(Msg)
		{
		case DCMM_CURRENT_RACE_CHANGED:
			UpdateRace(dcmapDBREF(wParam,lParam));
			break;
		default:
			break;
		}
		return 0;
	};
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapGeoValueCalculator);
		return 0;
	}

	int DCMAPAPI  CalculateGeo(DCMapPlanetGeoInfo const* pinf, 
		float* pGrowth,float* pPrimary,float* fPrimaryCredit,
		float* pSecondaty,float* pSecondatyCredit)
	{
		double t = (tmin+tmax)/2;
		double dt = fabs(tmin-tmax)/2;

		double tp = pinf->t;
		double psz = pinf->s;
		double pn=0,pb=0,ps=0;

		float gr=0,cb=0,cs=0,mb=0,ms=0;

		switch(m_RaceInfo.nature)
		{
		case DCMAP_ORGANICS:pn = pinf->o;break;
		case DCMAP_ENEGRY:pn = pinf->e;break;
		case DCMAP_MINERALS:pn = pinf->m;break;
		}
		switch(m_RaceInfo.basis)
		{
		case DCMAP_ORGANICS:pb = pinf->o;break;
		case DCMAP_ENEGRY:pb = pinf->e;break;
		case DCMAP_MINERALS:pb = pinf->m;break;
		}
		switch(m_RaceInfo.second)
		{
		case DCMAP_ORGANICS:ps = pinf->o;break;
		case DCMAP_ENEGRY:ps = pinf->e;break;
		case DCMAP_MINERALS:ps = pinf->m;break;
		}

		if(dt<0.1)dt=0.1;
		double TempFunc = 1;
		if(fabs(t - tp) > dt)  TempFunc = 2 - (fabs(tp - t)/dt);
		gr = 0.05*pn*(1+growth/100)*TempFunc;
		if(gr < 0) gr/=2;
		//gr = round(gr,-2);

		mb = floor(((psz*2*pb/160)*(1+mining/100))*(1+floor((psz-0.001)*0.4)*0.2));
		ms = floor(((psz*2*ps/320)*(1+mining/100))*(1+floor((psz-0.001)*0.4)*0.2));

		cb = floor(psz/12.5+(1+mining/100)*psz*(ps/2+pb*(1+floor((psz-0.001)/2.5)/5))/400);
		cs = floor(psz/12.5+(1+mining/100)*psz*(pb*2+ps*(1+floor((psz-0.001)/2.5)/5))/800);

		if(pGrowth)*pGrowth = gr;

		if(pPrimary)*pPrimary = mb;
		if(pSecondaty)*pSecondaty = ms;
		if(fPrimaryCredit)*fPrimaryCredit = cb;
		if(pSecondatyCredit)*pSecondatyCredit = cs;

		return gr >= min_growth?1:-1;
	}

	int DCMAPAPI  CalculateGeoIndirect(DCMapPlanetGeoInfo const* geo, DCMapPlanetValueInfo* val)
	{
		val->good = CalculateGeo(geo,&val->Growth,&val->Primary,&val->PrimaryCredit,&val->Secondaty,&val->SecondatyCredit);
		return val->good;
	}

	int DCMAPAPI  CalculatePlanet(dcmapDBREF const& ref, 
		float* pGrowth,float* pPrimary,float* fPrimaryCredit,
		float* pSecondaty,float* pSecondatyCredit)
	{
		if(!m_pPlanets->SelectID(ref))return 0;
		DCMapPlanetInfo const* pinf = m_pPlanets->GetData();

		return CalculateGeo(&pinf->geo,pGrowth,pPrimary,fPrimaryCredit,pSecondaty,pSecondatyCredit);
	}
};
DCMAP_REGISTER(CGeoValueService,"PService","GeoValueCalculator");

