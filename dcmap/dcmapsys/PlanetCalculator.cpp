// dcmapsys.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "../dcmapsyslib.h"
#include "dcmapcore.h"
#include "dcgov.h"

using namespace std;
using namespace gsys;

struct CPlanetCalcService : public IDCMapPlanetCalculator
{
protected:
	bool DCMAPAPI ProcessUnits(dcmapDBREF const& fleet, PlanetParamInfo* res,int Flags);
	
	void DCMAPAPI Destroy(){delete this;}
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CPlanetCalcService();}

	IDCMapRacesDataStoragePtr m_pRaces;
	IDCMapPlanetDataStoragePtr m_pPlanets;
	IDCMapPlayersDataStoragePtr m_pPlayers;
	IDCMapProjectDataStoragePtr m_pProjects;
	IDCMapComponentsDataStoragePtr m_pComponents;
	IDCMapBuildingsDataStoragePtr m_pUnits;
	IDCMapFleetsDataStoragePtr m_pFleets;

	DCMapRaceInfo m_RaceInfo;
	IDCMapWorkspace* m_pWorkspace;

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pWorkspace = workspace;
		
		if(!m_pRaces.init(core))return false;
		if(!m_pPlanets.init(core))return false;
		if(!m_pPlayers.init(core))return false;
		if(!m_pProjects.init(core))return false;
		if(!m_pUnits.init(core))return false;
		if(!m_pFleets.init(core))return false;
		if(!m_pComponents.init(core))return false;
		return true;
	}

	void  DCMAPAPI LoadConfig(IDCMapConfig* pConfig)
	{

	}

	bool DCMAPAPI CalculateCorruption(dcmapDBREF const& player)
	{
		if(!m_pPlayers->SelectID(player))return false;
		if(!dcgovReadPlanets(m_pPlanets,player))return false;
		dcgovUpdateCorruption(m_pPlanets);
		dcgovClear();
		return true;
	}

	float inline round(float f)
	{
		return floor(f)-f > 0.5f?ceil(f):floor(f);
	}

	bool DCMAPAPI ProcessPlanetIndirect(DCMapPlanetInfo const* pInf,PlanetProdInfo* res)
	{
		
		return false;
	}

	bool DCMAPAPI ProcessPlanet(dcmapDBREF const& ref, PlanetProdInfo* res)
	{
		if(!res)return false;
		memset(res,0,sizeof(*res));

		if(!m_pPlanets->SelectID(ref))return false;

		dcmapDBREF refowner = m_pPlanets[DCMDB_PLANET_OWNER];
		if(!m_pPlayers->SelectID(refowner))return false;

		dcmapDBREF refrace = m_pPlayers[DCMDB_PLAYER_RACE];
		if(!m_pRaces->SelectID(refrace)) 
		{
			return false;
		}

		float mainig = 1.0+(float)m_pRaces[DCMDB_RACE_MINING]*0.01;

		// calculate building production bonuses
		float production_bonus = 1;

		float bonus[3] = {1,1,1};
		float rsr[3];

		rsr[0] = int(m_pPlanets[DCMDB_PLANET_O]);
		rsr[1] = int(m_pPlanets[DCMDB_PLANET_E]);
		rsr[2] = int(m_pPlanets[DCMDB_PLANET_M]);

		int people_consume=0;

		production_bonus += int(m_pPlanets[DCMDB_PLANET_HAS_FACTORIES])*0.25;
		production_bonus += int(m_pPlanets[DCMDB_PLANET_HAS_PALACE])*0.1;
		production_bonus += int(m_pPlanets[DCMDB_PLANET_HAS_COMPONENTS])*0.1;
		production_bonus += int(m_pPlanets[DCMDB_PLANET_HAS_COMPRESSORS])*0.05;
		production_bonus += int(m_pPlanets[DCMDB_PLANET_HAS_RESEARCH])*0.1;

		bonus[0] += (dcmapREAL)m_pPlanets[DCMDB_PLANET_HAS_BIOFARMS]*0.2;
		bonus[1] += (dcmapREAL)m_pPlanets[DCMDB_PLANET_HAS_POWERPLANTS]*0.2;
		bonus[2] += (dcmapREAL)m_pPlanets[DCMDB_PLANET_HAS_MINES]*0.2;

		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_FACTORIES])*30000;
		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_PALACE])*2000;
		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_COMPONENTS])*37000;
		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_DESIGN])*20000;
		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_COMPRESSORS])*25000;
		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_RESEARCH])*10000;
		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_SHIPYARD])*10000;
		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_BARRACKS])*5000;
		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_BIOFARMS])*5000;
		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_POWERPLANTS])*5000;
		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_MINES])*5000;
		people_consume += int(m_pPlanets[DCMDB_PLANET_HAS_CUSTOMS])*50000;

		int people_max  = floor((float(m_pPlanets[DCMDB_PLANET_S])-0.0015)*2000);

		int basis = int(m_pRaces[DCMDB_RACE_BASIS]);
		int second = int(m_pRaces[DCMDB_RACE_SECOND]);
		int nature = int(m_pRaces[DCMDB_RACE_NATURE]);

		float corruption = 1.0-int(m_pPlanets[DCMDB_PLANET_CORRUPTION])*0.01;

		res->basis = (people_max*rsr[basis]*(1.0/160000)*mainig*bonus[basis]*corruption);
		res->second = (people_max*rsr[second]*(1.0/320000)*mainig*bonus[second]*corruption);

		res->credits = (people_max/25000.0*corruption + (res->basis+res->second)/5.0);

		res->people_consume = people_consume;
		res->people_free = people_max-people_consume;
		res->people_max = people_max;
		res->production = people_max*0.01*production_bonus*corruption;

		//50000P*50М/160000
		return true;
	}

	bool DCMAPAPI CalculatePlayerIncome(dcmapDBREF const& player,PlayerProdInfo* res)
	{
		if(!res)return false;
		memset(res,0,sizeof(*res));
		
		if(m_pPlanets->SelectAll())
		do 
		{
			if(m_pPlanets[DCMDB_PLANET_OWNER] == player)
			{
				PlanetProdInfo pp;
				if(!ProcessPlanet(*m_pPlanets->GetID(),&pp))return false;

				res->basis += pp.basis;
				res->second += pp.second;
				res->credits += pp.credits;
				res->population += pp.people_max;
			}
		} 
		while(m_pPlanets->Next());

		return true;
	}

	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
	{
		switch(Msg)
		{
			break;
		default:
			break;
		}
		return 0;
	};

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetCalculator);
		return 0;
	}
public:
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);

	bool DCMAPAPI ProcessPlanetParams(dcmapDBREF const& ref, PlanetParamInfo* res,int Flags=0);

	bool DCMAPAPI CalculateQueue(dcmapDBREF const& FleetRef, PlanetParamInfo const* planetparams,int Flags=0);


protected:
	void CalcGrow(PlanetParamInfo* res);
	void AddBuildingInfuence(PlanetParamInfo* res,DCMapProjectInfo const* prj);

	typedef int  (DCMAPAPI CPlanetCalcService::*ScripFunc)(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	typedef std::map<wstring,ScripFunc> FnMap;
	FnMap m_fns;
public:
	CPlanetCalcService()
	{
		m_fns[L"calculate"] = &CPlanetCalcService::ScriptCalc;
		m_fns[L"calc"] = &CPlanetCalcService::ScriptCalc;
		m_fns[L"calc_value"] = &CPlanetCalcService::ScriptCalcValue;
	}
	int DCMAPAPI ScriptCalc(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	int DCMAPAPI ScriptCalcValue(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
};
DCMAP_REGISTER(CPlanetCalcService,"PCoreService","PlanetCalculator");



int DCMAPAPI CPlanetCalcService::ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
{
	FnMap::iterator it = m_fns.find(command);
	if(it != m_fns.end())
	{
		if(type == DCMEXEC_QUERY) return DCMEXEC_CALL;
		return CALL_MEMBER_FN(*this,it->second)(type,command,pParams,pResult);
	}
	return DCMEXEC_FAIL;
}

void CPlanetCalcService::AddBuildingInfuence(PlanetParamInfo* res,DCMapProjectInfo const* prj)
{
	res->res_bonus[0] += prj->bonus_o;		
	res->res_bonus[1] += prj->bonus_e;		
	res->res_bonus[2] += prj->bonus_m;	
	res->surface += prj->bonus_s;
	res->prod_bonus += prj->bonus_production;
	res->people_consume += prj->requires_pepl;
}

bool CPlanetCalcService::ProcessUnits(dcmapDBREF const& fleet, PlanetParamInfo* res,int Flags)
{
	if(m_pUnits->SelectByParentID(fleet))
		do
		{
			if(m_pUnits[DCMDB_BUILDING_OUTDATED])continue;
			if(m_pUnits[DCMDB_BUILDING_QUEUED])continue;
			if(m_pUnits[DCMDB_BUILDING_DISBANDED])continue;

			if(!m_pProjects->SelectID(m_pUnits[DCMDB_BUILDING_PROJECT]))continue;
			DCMapProjectInfo const* prj = m_pProjects->GetData();
			if(!m_pComponents->SelectID(prj->chassis))continue;
			DCMapComponentInfo const* chassis = m_pComponents->GetData();

			AddBuildingInfuence(res,prj);

			res->BuildMask |= chassis->build_allow;
			res->BuildUnique |= chassis->build_unique;
		}
		while(m_pUnits->Next());
	return true;
}

static float s_round(float x){return floorf(x+0.5f);}

void CPlanetCalcService::CalcGrow(PlanetParamInfo* res)
{
	res->growth = min(1.0, 2.0-double(res->people)/res->surface/1000.0) * 
		min(1, 2.0-abs(res->race_t-res->race_tp)/res->race_dt) * 
		res->resources[res->nature_ind]*res->corruption_mod*res->res_bonus[res->nature_ind]*0.05*(1.0 + res->race_growth/100);

	res->people_grow = floor(res->growth*res->people/100.0f);
}

bool DCMAPAPI CPlanetCalcService::ProcessPlanetParams(dcmapDBREF const& ref, PlanetParamInfo* res,int Flags)
{
	if(!res)return false;
	memset(res,0,sizeof(*res));

	if(!m_pPlanets->SelectID(ref))return false;

	dcmapDBREF refowner = m_pPlanets[DCMDB_PLANET_OWNER];
	if(!m_pPlayers->SelectID(refowner))return false;
	dcmapDBREF refrace = m_pPlayers[DCMDB_PLAYER_RACE];
	if(!m_pRaces->SelectID(refrace))return false; 
	dcmapDBREF refbld = m_pPlanets[DCMDB_PLANET_BUILDINGS];
	dcmapDBREF refgar = m_pPlanets[DCMDB_PLANET_GARRISON];
	if(!m_pFleets->SelectID(refbld))return false; 

	DCMapPlanetInfo const* pinf = m_pPlanets->GetData();
	DCMapRaceInfo   const* rinf = m_pRaces->GetData();

	res->planet_o = pinf->o;
	res->planet_e = pinf->e;
	res->planet_m = pinf->m;
	res->planet_t = pinf->t;
	res->planet_s = pinf->s;

	if(pinf->s==0)return false;
	if(abs(rinf->tmin-rinf->tmax) < gsys::float_cmp_epsilon)return false;

	res->surface = pinf->s;

	res->res_bonus[0] = 100;
	res->res_bonus[1] = 100;
	res->res_bonus[2] = 100;

	res->prod_bonus=100;

	res->basis_ind = int(m_pRaces[DCMDB_RACE_BASIS]);
	res->second_ind = int(m_pRaces[DCMDB_RACE_SECOND]);
	res->nature_ind = int(m_pRaces[DCMDB_RACE_NATURE]);

	res->resources[0] = pinf->o;
	res->resources[1] = pinf->e;
	res->resources[2] = pinf->m;

	ProcessUnits(refbld,res,Flags);
	ProcessUnits(refgar,res,Flags);

	res->prod_bonus /= 100.0;
	res->res_bonus[0] /= 100.0;
	res->res_bonus[1] /= 100.0;
	res->res_bonus[2] /= 100.0;

	double mainig = 1.0+(double)m_pRaces[DCMDB_RACE_MINING]*0.01;

	res->people_max  = floor((double(res->surface)-0.0015)*2000);
	res->people = m_pPlanets[DCMDB_PLANET_POPULATION];
	res->people_free = res->people-res->people_consume;

	res->corruption_mod = 1.0;

	res->corruption = m_pPlanets[DCMDB_PLANET_CORRUPTION];

	if( (Flags&WITHOUT_CORRUPTION) == 0)
		res->corruption_mod = 1.0-int(res->corruption)*0.01;

	res->basis = s_round(res->people*res->resources[res->basis_ind]*(1.0/160000)*mainig*res->res_bonus[res->basis_ind]*res->corruption_mod);
	res->second = s_round(res->people*res->resources[res->second_ind]*(1.0/320000)*mainig*res->res_bonus[res->second_ind]*res->corruption_mod);
	res->credits = s_round(res->people/25000.0*res->corruption_mod + (res->basis+res->second)/5.0);
	res->production = s_round(res->people*0.01*res->prod_bonus*res->corruption_mod);

	res->race_t  = (rinf->tmin+rinf->tmax)/2;
	res->race_dt = abs(rinf->tmin-rinf->tmax)/2;
	res->race_tp = pinf->t;

	res->race_growth =  rinf->growth;

	CalcGrow(res);

	if(!res->people_grow) res->growth = 0;
	return true;
}

int DCMAPAPI CPlanetCalcService::ScriptCalc( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
{
	int x = pParams->GetNumber(0);
	int y = pParams->GetNumber(1);
	PlanetParamInfo res;
	if(m_pPlanets->SelectCoord(x,y))
	{
		if(ProcessPlanetParams(*m_pPlanets->GetID(),&res))
		{
			pResult->SetNamedNumber(L"surface",res.surface);
			pResult->SetNamedNumber(L"people_consume",res.people_consume);
			pResult->SetNamedNumber(L"people_free",res.people_free);
			pResult->SetNamedNumber(L"people_max",res.people_max);
			pResult->SetNamedNumber(L"production",res.production);
			pResult->SetNamedNumber(L"basis",res.basis);
			pResult->SetNamedNumber(L"second",res.second);
			pResult->SetNamedNumber(L"credits",res.credits);
			pResult->SetNamedNumber(L"growth",res.growth);
		}
	}
	return DCMEXEC_SUCCESS;
}

int DCMAPAPI CPlanetCalcService::ScriptCalcValue( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
{
	int x = pParams->GetNumber(0);
	int y = pParams->GetNumber(1);
	PlanetParamInfo res;
	if(m_pPlanets->SelectCoord(x,y))
	{
		if(ProcessPlanetParams(*m_pPlanets->GetID(),&res,WITHOUT_CORRUPTION))
		{
			pResult->SetNamedNumber(L"production",res.production);
			pResult->SetNamedNumber(L"basis",res.basis);
			pResult->SetNamedNumber(L"second",res.second);
			pResult->SetNamedNumber(L"credits",res.credits);
		}
	}
	return DCMEXEC_SUCCESS;
}

bool DCMAPAPI CPlanetCalcService::CalculateQueue( dcmapDBREF const& FleetRef, PlanetParamInfo const* PPlanetParams,int Flags/*=0*/ )
{
	dcmapDBREF fref;

	PlanetParamInfo params;
	if(PPlanetParams)params = *PPlanetParams;

	if(!m_pFleets->SelectID(FleetRef))
	{
		if(m_pPlanets->SelectID(FleetRef))
		{
			if(!PPlanetParams)
			{
				if(!ProcessPlanetParams(FleetRef,&params))return false;
				PPlanetParams = &params;
			}
			fref = m_pPlanets[DCMDB_PLANET_QUEUE];
			if(!FleetRef.Valid())return false;
		}
		return false;
	}
	else 
	{
		fref = FleetRef;
		if(!PPlanetParams)
		{	
			int x = m_pFleets[DCMDB_FLEET_X];
			int y = m_pFleets[DCMDB_FLEET_Y];
			if(!m_pPlanets->SelectCoord(x,y))return false;
			if(!ProcessPlanetParams(*m_pPlanets->GetID(),&params))return false;
		}	
	}
	
	struct QueueInfo
	{
		int iCompleted;
		int iNeeded;
		int iTurn;
		dcmapDBREF RefProject;
		dcmapDBREF RefUnit;
	};

	typedef std::vector<QueueInfo> VecQueueInfo;

	VecQueueInfo vecInfo;

	if(m_pUnits->SelectByParentID(fref))
	do
	{
		if(m_pUnits[DCMDB_BUILDING_OUTDATED])continue;
		if(m_pUnits[DCMDB_BUILDING_DISBANDED])continue;
		if(!m_pUnits[DCMDB_BUILDING_QUEUED])continue;
		if(!m_pProjects->SelectID(m_pUnits[DCMDB_BUILDING_PROJECT]))continue;

		QueueInfo qi;
		qi.RefUnit     =  *m_pUnits->GetID();
		qi.RefProject  =  m_pUnits[DCMDB_BUILDING_PROJECT];
		qi.iCompleted  =  m_pUnits[DCMDB_BUILDING_COMPLETE];
		qi.iNeeded     =  m_pProjects[DCMDB_PROJECT_BUILD_SPEED];
		vecInfo.push_back(qi);
	}
	while(m_pUnits->Next());

	VecQueueInfo::iterator it = vecInfo.begin();
	VecQueueInfo::iterator end = vecInfo.end();

	int iTurn = 0;
	int iProdLeft = 0;

	while(iTurn<1024)
	{
		iTurn++;
		if(it == end)break;
		it->iCompleted += params.production + iProdLeft;
		if(it->iCompleted > it->iNeeded) // will complete
		{
			if(m_pProjects->SelectID(it->RefProject)) // influence by built
			{
				DCMapProjectInfo const* prj = m_pProjects->GetData();
				AddBuildingInfuence(&params,prj);

				params.people -= prj->cost_pepl;
			}
			it->iTurn = iTurn;
			iProdLeft = it->iCompleted - it->iNeeded;
			++it;
		}
		params.people += params.people_grow;

		CalcGrow(&params);
		params.production = s_round(params.people*0.01*params.prod_bonus*params.corruption_mod);
	}
	
	_foreach(VecQueueInfo,vecInfo,it)
	{
		if(m_pUnits->SelectID(it->RefUnit))
		{
			m_pUnits->Update();
			m_pUnits[DCMDB_BUILDING_READY_IN] = it->iTurn;
			m_pUnits->Post();
		}
	}
	return true;
}



