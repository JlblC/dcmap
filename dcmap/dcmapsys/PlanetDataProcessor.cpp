#include "StdAfx.h"
#include "PlanetDataProcessor.h"

class CPlanetDataProcessorFilter : public IDCMapFilteredDataSource
{
	vector<dcmapDBREF> m_Refs;
	vector<dcmapDBREF>::iterator current;

	IDCMapCore* m_pCore;

	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pCore = core;
		return true;
	}
	bool DCMAPAPI SetTextParam(const char* szParam){return false;};
	bool DCMAPAPI SetDataSourceName(const char* szName){return false;};
	bool DCMAPAPI SetDataSource(IDCMapDataSource* pSource){return false;};
	bool DCMAPAPI Process(IDCMapProcessRecordReseiver* pRsvr,dcmapDWORD options)
	{
		IDCMapWorkspace* workspace = m_pCore->GetCurrentWorkspace();
		if(!workspace)return false;
		IDCMapPlanetDataProcessor* proc = workspace->GetServiceTyped<IDCMapPlanetDataProcessor>("PlanetDataProcessor");
		if(!proc)return false;

		m_Refs.clear();
		for(int i=1;i<=g_DCMapNumPlanets;i++)
		{
			if(proc->SelectLine(i,1,g_DCMapNumPlanets))
			do
			{
				DCMapPlanetDisplayInfo* const pinf = proc->GetPlanetInfo();
				if(!pinf->iHidden) m_Refs.push_back(pinf->planet.id);
			}while(proc->Next());
		}
		return true;
	}
	bool DCMAPAPI SelectAll(){current = m_Refs.begin();return current != m_Refs.end();}
	bool DCMAPAPI Next()
	{
		++current;
		return current != m_Refs.end();
	}
	const dcmapDBREF* DCMAPAPI GetID(){return &(*current); }
	int DCMAPAPI NumRecords(){return m_Refs.size();}

	void* DCMAPAPI CastTo(char const* idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapFilteredDataSource);
		DCMAP_MODULE_SUPPORTS(IDCMapDataSource);
		return 0;
	}
};
DCMAP_REGISTER(CPlanetDataProcessorFilter,"PTableFilter_planets","CurrentPlanetDataProcessorFilter");
DCMAP_REGISTER(CPlanetDataProcessor,"PService","PlanetDataProcessor");



void* DCMAPAPI CPlanetDataProcessor::CastTo(DCMapInterfaceID idInterface)
{
	DCMAP_MODULE_SUPPORTS(IDCMapPlanetDataProcessor);
	return 0;
}

bool DCMAPAPI CPlanetDataProcessor::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	DCMAP_GET_STORAGE(core,m_pFeeder,IDCMapPlanetDataStorage,planets);
	m_pWorkspace = workspace;
	LoadPrefs();

	vector<string> modules;
	g_pDCMapSys->GetClassPluginNamesT("PPlanetFilter",modules);

	for(uint i=0;i<modules.size();i++)
	{
		IDCMapPlanetDataFilter* pfilter = 
			workspace->CreateModuleTyped<IDCMapPlanetDataFilter>("PPlanetFilter",modules[i].c_str());
		if(pfilter)
		{
			m_vecFilters.push_back(pfilter);
		}	
	}

	GetData();
	return true;
}

CPlanetDataProcessor::CPlanetDataProcessor(void)
{
	m_pFeeder=0;
	m_ptSelection.x=-1;
	m_ptSelection.y=-1;
	m_pCurrent=0;
	m_iShadeMode=0;

	m_clSelection = dcmapRGB(255,200,100);
}

CPlanetDataProcessor::~CPlanetDataProcessor(void)
{
}

DCMapPlanetDisplayInfo* DCMAPAPI CPlanetDataProcessor::GetPlanet(int x, int y)
{
	if(x<1)return 0;
	int ln = y-1;
	if(ln < 0 || ln >= g_DCMapNumPlanets)return 0;
	Line& line = m_Lines[ln];

	if(x > line.Max || x < line.Min)
	{
		Actualize(ln,x,x);
	}

	MapXCoordIndex::iterator it = line.Index.find(x);
	if(it == line.Index.end())
	{
		return 0;
	}
	return it->second;
}

void CPlanetDataProcessor::ProcessLine(int ln,int From,int To)
{
	Line& line = m_Lines[ln];

	if(m_pFeeder->SelectLine(ln+1,From,To))
	{
		MapXCoordIndex::iterator it = line.Index.begin();
		do 
		{
			DCMapPlanetDisplayInfo* dta = NewData();
			DCMapPlanetInfo const* pinf = m_pFeeder->GetData();
			dta->planet = *(pinf);
			if(ProcessData(dta,pinf))
			{
				it = line.Index.insert(it,MapXCoordIndexValue(dta->planet.x,dta));
			}
			else
			{
				DeleteData(dta);
			}
		}
		while(m_pFeeder->Next());
	}
}

void CPlanetDataProcessor::Actualize(int ln,int From,int To)
{
	Line& line = m_Lines[ln];
	// Process requested space now	
	if(line.Min == line.Max)
	{
		ProcessLine(ln,From,To);
		line.Min = From;
		line.Max = To;
	}
	else
	{
		if(From < line.Min)
		{
			ProcessLine(ln,From,line.Min);
			line.Min = From;
		}
		if(To > line.Max)
		{
			ProcessLine(ln,line.Max,To);
			line.Max = To;
		}
	}
}

bool DCMAPAPI CPlanetDataProcessor::SelectLine(int LineNum,int From,int To)
{
	m_pCurrent=0;

	int ln = LineNum-1;
	if(ln < 0 || ln >= g_DCMapNumPlanets)return false;
	if(To < From)return false;

	Line& line = m_Lines[ln];
	Actualize(ln,From,To);

	m_itFinal = line.Index.end();

	if(From == 1) m_itCurrent = line.Index.begin();
	else m_itCurrent = line.Index.lower_bound(From);

	if(To == g_DCMapNumPlanets) m_itEnd = m_itFinal;
	else m_itEnd = line.Index.upper_bound(To-1);

	if(m_itCurrent == m_itEnd)return false;

	m_pCurrent = m_itCurrent->second;
	++m_itCurrent;
	return true;

	return false;
}

bool DCMAPAPI CPlanetDataProcessor::Next()
{
	m_pCurrent=0;
	if(m_itCurrent == m_itEnd || m_itCurrent == m_itFinal)
	{
		return false;
	}
	m_pCurrent = m_itCurrent->second;
	++m_itCurrent;
	return true;
}

DCMapPlanetDisplayInfo* DCMAPAPI CPlanetDataProcessor::GetPlanetInfo()
{
	return m_pCurrent;
}

dcmapDBREF* DCMAPAPI CPlanetDataProcessor::GetID()
{
	return &m_pCurrent->planet.id;
}

void CPlanetDataProcessor::GetData()
{
	if(!m_pFeeder) return;
	ClearData();

	// for test now
	return;

	for(int i=1;i<=g_DCMapNumPlanets;i++)
	{
		if(m_pFeeder->SelectLine(i,1,g_DCMapNumPlanets))
		{
			do 
			{
				DCMapPlanetDisplayInfo* dta = NewData();
				DCMapPlanetInfo const* pinf = m_pFeeder->GetData();
				dta->planet = *(pinf);
				if(ProcessData(dta,pinf))
				{
					IndexData(dta);
				}
				else
				{
					DeleteData(dta);
				}
			}
			while(m_pFeeder->Next());
		}
		
	}
	m_PlanetsState = m_pFeeder->GetStateIndex();

	m_itCurrent = m_itEnd = m_Lines[0].Index.end();
}

void CPlanetDataProcessor::ResetAlloc(int prealloc)
{
	m_PlanetDataPool.purge_memory();
	m_PlanetDataPool.set_next_size(prealloc+256);
}

DCMapPlanetDisplayInfo* CPlanetDataProcessor::NewData()
{
	//DCMapPlanetDisplayInfo* pinf = new DCMapPlanetDisplayInfo;
	DCMapPlanetDisplayInfo* pinf = m_PlanetDataPool.malloc();

	ZeroMemory(pinf,sizeof(DCMapPlanetDisplayInfo));
	return pinf;
}

void CPlanetDataProcessor::DeleteData(DCMapPlanetDisplayInfo* dta)
{
	//delete dta;
	m_PlanetDataPool.free(dta);
}

void CPlanetDataProcessor::IndexData(DCMapPlanetDisplayInfo* dta)
{
	int line = dta->planet.y-1;
	m_Lines[line].Index[dta->planet.x] = dta;
}

void CPlanetDataProcessor::UnindexData(DCMapPlanetDisplayInfo* dta)
{
	int line = dta->planet.y-1;
	m_Lines[line].Index.erase(dta->planet.x);
}

void CPlanetDataProcessor::ClearData()
{
	for(int i=0;i<g_DCMapNumPlanets;i++) // Clear index
	{
		Line& line = m_Lines[i];
		/*_foreach(MapXCoordIndex,line.Index,it)
		{
			DeleteData(it->second);
		}*/
		line.Index.clear();
		line.Min = -1;
		line.Max = -1;
	}	
	ResetAlloc(m_pFeeder->NumRecords());
	m_itCurrent = m_itEnd = m_Lines[0].Index.end();
}

int DCMAPAPI CPlanetDataProcessor::OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
{
	switch(Msg)
	{
	case DCMM_MAP_SET_FILTER_SHADE_MODE:
		if(m_iShadeMode != wParam)
		{
			m_iShadeMode = wParam;
			ClearData();
			m_pWorkspace->BroadcastMessage(DCMM_MAPDATA_CHANGED,0,0);
		}
		break;
	case DCMM_CONFIG_CHANGED:
		LoadPrefs();
		break;
	case DCMM_PLANET_FILTER_CHANGED:
		if(ActualizeData())	m_pWorkspace->BroadcastMessage(DCMM_MAPDATA_CHANGED,0,0);
		break;
	case DCMM_PLANET_CHANGED: // update selected planet
		m_refSelection.id = wParam;
		m_refSelection.age = lParam;
		Reprocess(m_ptSelection.x,m_ptSelection.y);
		if(m_pFeeder->SelectID(m_refSelection))
		{
			DCMapPlanetInfo const* dta = m_pFeeder->GetData();
			m_ptSelection.x = dta->x;
			m_ptSelection.y = dta->y;
			Reprocess(m_ptSelection.x,m_ptSelection.y);
		}
		break;
	}
	return 0;
}

bool CPlanetDataProcessor::Reprocess(int x,int y)
{
	if(x<0||y<0)return false;

	bool indexed=true;
	DCMapPlanetDisplayInfo* dta = GetPlanet(x, y);
	if(!dta)
	{
		dta = NewData();
		indexed = false;
	}
	else
	{
		ZeroMemory(dta,sizeof(DCMapPlanetDisplayInfo));
	}
	if(m_pFeeder->SelectCoord(x,y))
	{
		DCMapPlanetInfo const* pinf = m_pFeeder->GetData();
		dta->planet = *(pinf);
		if(ProcessData(dta,pinf))
		{
			if(!indexed)IndexData(dta);
		}
		else
		{
			if(indexed)UnindexData(dta);
			DeleteData(dta);
		}
	}
	m_pWorkspace->BroadcastMessage(DCMM_PLANET_MAPDATA_CHANGED,x,y);
	return true;
}

dcmapCOLORREF FadeFill(dcmapCOLORREF cl)
{
	int r = dcmapGetRValue(cl);
	int g = dcmapGetGValue(cl);
	int b = dcmapGetBValue(cl);

	return dcmapRGB(r*0.15,g*0.14,b*0.16);
}

dcmapCOLORREF FadeBorder(dcmapCOLORREF cl)
{
	int r = dcmapGetRValue(cl);
	int g = dcmapGetGValue(cl);
	int b = dcmapGetBValue(cl);

	return dcmapRGB(r*0.25,g*0.24,b*0.26);
}

bool CPlanetDataProcessor::ProcessData(DCMapPlanetDisplayInfo* dta,DCMapPlanetInfo const* pinf)
{
	dta->clPlanetBody = dcmapRGB(150,150,150);
	dta->clPlanetBorder = dcmapRGB(0,0,0);
	dta->clBackground = dcmapRGB(0,0,0);

	dta->iBackground = 0;
	dta->iPlanetBody = 1;
	dta->iPlanetBorder = 0;
	dta->iCellBorder = 0;
	dta->iMarker = 0;

	dta->Icon1 = -1;
	dta->Icon2 = -1;
	dta->Icon3 = -1;

	bool pass = true;

	_foreach(VecFilters,m_vecFilters,it)
	{
		if(!(*it)->ProcessPlanet(dta,pinf))
		{
			if(!m_iShadeMode)return false;
			pass = false;
		}
	}

	if(!pass)
	{
		dta->clPlanetBody = FadeFill(dta->clPlanetBody);
		dta->clCellBorder = FadeBorder(dta->clCellBorder);
		dta->clMarker = FadeFill(dta->clMarker);
		dta->clPlanetBorder = FadeBorder(dta->clPlanetBorder);
		dta->clBackground = FadeFill(dta->clBackground);
		dta->iHidden = 1;

		if(m_refSelection == dta->planet.id)
		{
			dta->clCellBorder = m_clSelection;
			dta->iCellBorder = 1;
		}	
	}
	else
	{
		dta->iHidden = 0;
		if(m_refSelection == dta->planet.id)
		{
			dta->clCellBorder = m_clSelection;
			dta->iCellBorder = 2;
		}
	}

	return true;
}

bool DCMAPAPI CPlanetDataProcessor::ActualizeData()
{
	bool update=false;

	update |= (m_PlanetsState != m_pFeeder->GetStateIndex());

	_foreach(VecFilters,m_vecFilters,it)
	{
		update |= (*it)->ActualizeData();
	}
	if(update)
	{
		ClearData();
	}
	return update;
}

void CPlanetDataProcessor::LoadPrefs(void)
{
	m_clSelection = dcmapRGB(255,200,100);

	DCMapConfigWrapper cfg(m_pWorkspace->GetCore()->GetConfig());

	cfg.section("MapColor");

	cfg["SelectionPlanet"] >> m_clSelection;
}

bool DCMAPAPI CPlanetDataProcessor::SelectAll()
{
	return false;
}

int DCMAPAPI CPlanetDataProcessor::NumRecords()
{
	return 0;
}

void  DCMAPAPI CPlanetDataProcessor::LoadConfig( IDCMapConfig* pConfig )
{
	DCMapConfigWrapper cfg(pConfig);

	cfg.section("PlanetFilter");
	cfg["ShadeFiltered"] >> m_iShadeMode ;
}

void  DCMAPAPI CPlanetDataProcessor::SaveConfig( IDCMapConfig* pConfig )
{
	DCMapConfigWrapper cfg(pConfig);

	cfg.section("PlanetFilter");
	cfg["ShadeFiltered"] << m_iShadeMode ;

}