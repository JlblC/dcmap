#include "stdafx.h"
#include "../dcmapbase.h"
#include "xmlutils.h"
#include "utils.h"
#include <bitset>

using namespace std;
using namespace gsys;

static int res_to_num(dcmapWCSTR n)
{
	if(!n)return 0;
	if(n[0]=='o')return 0;
	if(n[0]=='e')return 1;
	if(n[0]=='m')return 2;
	return 0;
}

struct CXMLGameProcessor : public IDCMapImportProcessor
{
	struct process_rec_data
	{
		IDCMapDataStorage* table;

		process_rec_data():table(0)
		{}
	};

	typedef bool (CXMLGameProcessor::*GameProcessFunc)(XNode*,int,const dcmapDBREF&);
	typedef bool (CXMLGameProcessor::*ProcessFunc)(XNode* node,process_rec_data const& rec);

	struct process_rec
	{
		wstring sec;
		GameProcessFunc gfunc;
		ProcessFunc func;
		int dep[4];

		LPXNode node;
		int r;
		bool allow;

		process_rec_data data;

		process_rec():r(0),node(0),allow(0),func(0),gfunc(0)
		{
			dep[0]=-1;
			dep[1]=-1;
			dep[2]=-1;
			dep[3]=-1;
		}

		void set(dcmapWCSTR s,GameProcessFunc f,int d1=-1,int d2=-1,int d3=-1,int d4=-1)
		{
			sec=s;
			gfunc=f;
			dep[0]=d1;
			dep[1]=d2;
			dep[2]=d3;
			dep[3]=d4;
		}
		void set(dcmapWCSTR s,ProcessFunc f,int d1=-1,int d2=-1,int d3=-1,int d4=-1)
		{
			sec=s;
			func=f;
			dep[0]=d1;
			dep[1]=d2;
			dep[2]=d3;
			dep[3]=d4;
		}
	};

	enum 
	{
		EProcessGeneral,EProcessUserPlanets,EProcessDiplomacy,
		EProcessProjects,EProcessSharedProjects,EProcessGarrison,
		EProcessFleets,EProcessSharedFleets,EProcessForeighnFleets,
		EProcessKnownPlanets,EProcessActsReq,EProcessCargoMoves,
		ENumGameRecords,
		EProcessComponents,EProcessChassiss,
		ENumRecords
	};

	static const int NumRecords=ENumRecords;
	static const int GameRecords=ENumGameRecords;

	typedef vector<process_rec> VecProcessRecords;

	VecProcessRecords m_rec;

	dcmapDBREF m_PlayerRef;
	dcmapDBREF m_OprionsPlayerRef;
	int        m_iTurn;
	int m_iErrors;
	IDCMapProcessRecordReseiver* m_pRsvr;
	XDoc m_XML;

protected:

public:
	CXMLGameProcessor();
	~CXMLGameProcessor();

	dcmapWCSTR DCMAPAPI GetFileFilters(){return L"xml files|*.xml";}
	dcmapWCSTR DCMAPAPI GetDisplayName(){return L"Game XML Import";}
	char const* DCMAPAPI GetConfigSectionName(){return "GameXMLImport";}

	dcmapWCSTR DCMAPAPI ReformatInput(dcmapWCSTR input)
	{
		XDoc doc;
		if(!doc.Load(input))return 0;
		static CString frmts;

		DISP_OPT opt;
		opt.tab_chars = L"  ";
		
		frmts =  doc.GetXML(&opt);
		return frmts.c_str();
	}


	void* DCMAPAPI CastTo(char const* idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapImportProcessor);
		return 0;
	}

	virtual bool DCMAPAPI PreProcessData(dcmapDCSTR szInputData,int iLines,
		IDCMapProcessRecordReseiver* pRsvr,
		dcmapDWORD options);

	int ProcessBase(LPXNode dc);

	virtual void DCMAPAPI ClearProcessData();

	virtual bool DCMAPAPI BeginExecute();
	virtual bool DCMAPAPI ExecuteRecord(dcmapDCSTR Text,int id);
	virtual void DCMAPAPI EndExecute();

	IDCMapRacesDataStoragePtr   m_pRaces;
	IDCMapGeoDataStoragePtr     m_pGeo;
	IDCMapPlanetDataStoragePtr  m_pPlanets;
	IDCMapPlayersDataStoragePtr m_pPlayers;
	IDCMapFleetsDataStoragePtr  m_pFleets;
	IDCMapProjectDataStoragePtr    m_pProjects;
	IDCMapBuildingsDataStoragePtr  m_pUnits;
	IDCMapBuildingsDataStoragePtr  m_pGarrisons;
	IDCMapDipDataStoragePtr  m_pDip;
	IDCMapGroupsDataStoragePtr  m_pGroups;
	IDCMapPlanetsGroupsDataStoragePtr  m_pPlanetGroups;
	IDCMapPlayersGroupsDataStoragePtr  m_pPlayersGroups;
	IDCMapComponentsDataStoragePtr  m_pComponents;
	IDCMapProjectActsDataStoragePtr m_pProjectActs;
	IDCMapActionsRequestedDataStoragePtr m_pActsReq;
	IDCMapUnitTagsDataStoragePtr m_pUnitTags;

	IDCMapFleetManager* m_pFleetMan;

	bool bManage;
	bool bImportTrusted;
	bool bAddEmpty;
	bool bAddOccupied;
	bool bOverwriteEmpty;
	bool bOverwriteOccupied;
	bool bClearOccupied;
	bool bImportDip;
	bool bImportContents;
	bool bImportFleets;
	bool bImportAlienFleets;

	int  iImportLimit;
	dcmapRECT  ImportRect;
	dcmapHRGN  ImportRegion;

	IDCMapCore* m_pCore;

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	int Process(dcmapWCSTR szXML,dcmapDBREF const* pRefPlayer);
	bool ProcessPlayer(XNode* dc,int &turn,dcmapDBREF &PlayerRef,dcmapDBREF const &OtionPlayerRef);
	bool ProcessGeneral(XNode* node,int turn,dcmapDBREF const& pref);
	bool ProcessUserPlanets(LPXNode node,int turn,dcmapDBREF const& PlayerRef);
	bool ProcessKnownPlanets(LPXNode node,int turn,dcmapDBREF const& PlayerRef) /* optimization needed */;
	bool ProcessDiplomacy(LPXNode node,int turn,dcmapDBREF const& PlayerRef);
	bool ProcessProjects(LPXNode node,int turn,dcmapDBREF const& pref);
	bool ProcessGarrison(LPXNode node,int turn,dcmapDBREF const& pref);
	bool ProcessFleets(LPXNode node,int turn,dcmapDBREF const& pref);
	bool ProcessForeighnFleets(LPXNode node,int turn,dcmapDBREF const& pref);
	bool ProcessActsReq(LPXNode node,process_rec_data const& rec);
	int ProcessUnits(LPXNode node,int turn,dcmapDBREF const& player,dcmapDBREF const& parent,bool queue=false,bool planet=false);
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	bool SelectPlayerNameId(dcmapWCSTR szPlayer,int user_id,dcmapDBREF& dref);
	int ProcessForeignUnits(LPXNode node,int turn,dcmapDBREF const& player,dcmapDBREF const& parent_fleet);
	void UpdateBuildings(int turn,dcmapDBREF const& Planet,dcmapDBREF const& Garrison);
	void OutdateUnits(dcmapDBREF const& parent);
	dcmapDBREF GePlanetGroupRef(dcmapDBREF const& PlayerRef, EDCMapPlayerInfo_Records rec = DCMDB_PLAYER_OPEN_PLANETS);
	void FillGarrizonLike(dcmapWCSTR NameFormat,dcmapWCSTR CaptionFormat,int x, int y,int turn,dcmapDBREF const& pref);
	void UpdateComponentBuildInd(dcmapDBREF ref);
	void UpdateGarrizon(int turn,dcmapDBREF const& Planet,dcmapDBREF const& Garrison);
	bool ProcessComponents(LPXNode node,process_rec_data const& rec);
	bool ProcessComponentData(LPXNode node);
	bool ProcessChassiss(LPXNode node,process_rec_data const& rec);
	bool ProcessProjectActions(LPXNode node,dcmapDBREF const& refProject);
	void ClearRecords();
	bool DCMAPAPI PreProcessDCMapData(LPXNode data);
	bool ProcessDataTable(LPXNode node,process_rec_data const& rec);
	bool ProcessCargoMoves(LPXNode node,process_rec_data const& rec);
};
DCMAP_REGISTER(CXMLGameProcessor,"PCoreService","XMLProcessor");
DCMAP_REGISTER(CXMLGameProcessor,"PTextImport","XMLImport");

DCMAP_REGALIAS("PCoreService","XMLProcessor","XMLGameProcessor");
DCMAP_REGALIAS("PTextImport","XMLImport","XMLGameProcessor");


CXMLGameProcessor::CXMLGameProcessor()
{
	ImportRegion=0;
}

CXMLGameProcessor::~CXMLGameProcessor()
{
	if(ImportRegion)DeleteRgn(ImportRegion);
}


void CXMLGameProcessor::ClearRecords()
{
	m_rec.clear();
	m_rec.resize(NumRecords);
	m_rec[EProcessGeneral].set(L"general-info",&CXMLGameProcessor::ProcessGeneral);
	m_rec[EProcessUserPlanets].set(L"user-planets",&CXMLGameProcessor::ProcessUserPlanets);
	m_rec[EProcessDiplomacy].set(L"diplomacy",&CXMLGameProcessor::ProcessDiplomacy);
	m_rec[EProcessProjects].set(L"building-types",&CXMLGameProcessor::ProcessProjects);
	m_rec[EProcessSharedProjects].set(L"shared-building-types",&CXMLGameProcessor::ProcessProjects);
	m_rec[EProcessGarrison].set(L"harrisons",&CXMLGameProcessor::ProcessGarrison,EProcessProjects);
	m_rec[EProcessFleets].set(L"fleets",&CXMLGameProcessor::ProcessFleets,EProcessProjects);
	m_rec[EProcessSharedFleets].set(L"shared-fleets",&CXMLGameProcessor::ProcessFleets,EProcessSharedProjects);
	m_rec[EProcessForeighnFleets].set(L"allien-fleets",&CXMLGameProcessor::ProcessForeighnFleets);
	m_rec[EProcessKnownPlanets].set(L"known-planets",&CXMLGameProcessor::ProcessKnownPlanets);
	m_rec[EProcessActsReq].set(L"actions-requested",&CXMLGameProcessor::ProcessActsReq);
	m_rec[EProcessComponents].set(L"components",&CXMLGameProcessor::ProcessComponents);
	m_rec[EProcessChassiss].set(L"chassiss",&CXMLGameProcessor::ProcessChassiss);
	m_rec[EProcessCargoMoves].set(L"cargo-moves",&CXMLGameProcessor::ProcessCargoMoves,EProcessGarrison,EProcessForeighnFleets);
}

bool  DCMAPAPI CXMLGameProcessor::Init( IDCMapCore* core,IDCMapWorkspace* workspace )
{
	m_pCore = core;
	if(!m_pRaces.init(core))return false;
	if(!m_pPlanets.init(core))return false;
	if(!m_pPlayers.init(core))return false;
	if(!m_pFleets.init(core))return false;
	if(!m_pProjects.init(core))return false;
	if(!m_pUnits.init(core))return false;
	if(!m_pDip.init(core))return false;
	if(!m_pGeo.init(core))return false;
	if(!m_pGroups.init(core))return false;
	if(!m_pPlanetGroups.init(core))return false;
	if(!m_pPlayersGroups.init(core))return false;
	if(!m_pComponents.init(core))return false;
	if(!m_pProjectActs.init(core))return false;
	if(!m_pActsReq.init(core))return false;
	if(!m_pUnitTags.init(core))return false;

	m_pFleetMan = core->GetServiceTyped<IDCMapFleetManager>("FleetManager");
	if(!m_pFleetMan)return false;

	return true;
}

int DCMAPAPI CXMLGameProcessor::ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
{
	if(type == DCMEXEC_QUERY) return DCMEXEC_CALL;

	if(wcscmp(command,L"process")==0)
	{
		dcmapWCSTR source = pParams->GetString(0);
		dcmapDBREF const* pRefPlayer = pParams->GetDBRef(1);

		m_pCore->LockDataUpdate();
		int r = Process(source,pRefPlayer);
		m_pCore->UnlockDataUpdate();
		pResult->AddNumber(r);

		return DCMEXEC_SUCCESS;
	}
	return DCMEXEC_FAIL;
}

struct RecordRec : public IDCMapProcessRecordReseiver
{
	vector<int> records;
	int result;
	RecordRec():result(1){}
	bool DCMAPAPI AddRecord(dcmapDCSTR Text,int id,int text_line)
	{
		records.push_back(id);
		return true;
	}
	void DCMAPAPI SetErrorCode(int code, dcmapDCSTR ErrorText)
	{
		result = code;
	}
};

int CXMLGameProcessor::Process(dcmapWCSTR szInputData,dcmapDBREF const* pRefPlayer)
{
	RecordRec rec;
	m_pRsvr = &rec;

	if(pRefPlayer && pRefPlayer->Valid())
		m_OprionsPlayerRef = *pRefPlayer;
	else
		m_OprionsPlayerRef.Invalidate();
	

	PreProcessData(szInputData,0,&rec,0);

	if(rec.result == 1)
	{
		if(BeginExecute())
		{
			for(int i=0;i<rec.records.size();i++)
			{
				ExecuteRecord(0,rec.records[i]);
			}
			EndExecute();
		}
		else
		{
			return -1;
		}
	}
	m_pRsvr = 0;
	return rec.result;
}

bool CXMLGameProcessor::ProcessGeneral(LPXNode node,int turn,dcmapDBREF const& pref)
{
	if(!node)return true;

	LPXNode player = node->GetChild(L"this-player");
	LPXNode race = node->GetChild(L"this-player-race");
	if(!player || !race)return false;

	dcmapWCSTR szRaceName = race->GetAttrValue(L"race-name");
	int race_id = xml_cast<int>(race->GetAttrValue(L"race-id"));

	if(!m_pRaces->SelectGameID(race_id))
	{
		if(!m_pRaces->SelectWideName(szRaceName))
		{
			m_pRaces->Insert();
		}
	}
	else m_pRaces->Update();

	m_pRaces[DCMDB_RACE_NAME] = szRaceName;
	m_pRaces[DCMDB_RACE_GAME_ID] = race_id;

	float tOptimal = xml_cast<float>(race->GetAttrValue(L"t-optimal"));
	float tDelta = xml_cast<float>(race->GetAttrValue(L"t-delta"));

	m_pRaces[DCMDB_RACE_TMAX] = tOptimal+tDelta;
	m_pRaces[DCMDB_RACE_TMIN] = tOptimal-tDelta;

	m_pRaces[DCMDB_RACE_GROWTH] = race->GetAttrValue(L"bonus-multiply");
	m_pRaces[DCMDB_RACE_SPEED] = race->GetAttrValue(L"bonus-speed");
	m_pRaces[DCMDB_RACE_DEFENCE] = race->GetAttrValue(L"bonus-dr");
	m_pRaces[DCMDB_RACE_PRICES] = race->GetAttrValue(L"bonus-price");
	m_pRaces[DCMDB_RACE_SCIENCE] = race->GetAttrValue(L"bonus-iq");
	m_pRaces[DCMDB_RACE_CIVIL_PRODUCTION] = race->GetAttrValue(L"bonus-build-peace");
	m_pRaces[DCMDB_RACE_MILITARY_PRODUCTION] = race->GetAttrValue(L"bonus-build-war");
	m_pRaces[DCMDB_RACE_HITPOINTS] = race->GetAttrValue(L"bonus-hp");
	m_pRaces[DCMDB_RACE_DAMAGE] = race->GetAttrValue(L"bonus-attack");
	m_pRaces[DCMDB_RACE_MINING] = race->GetAttrValue(L"bonus-mining");
	m_pRaces[DCMDB_RACE_STEALTH] = race->GetAttrValue(L"bonus-stealth");
	m_pRaces[DCMDB_RACE_DETECTION] = race->GetAttrValue(L"bonus-detectors");

	m_pRaces[DCMDB_RACE_NATURE] = res_to_num(race->GetAttrValue(L"race-nature"));
	m_pRaces[DCMDB_RACE_BASIS] = res_to_num(race->GetAttrValue(L"industry-nature"));
	m_pRaces[DCMDB_RACE_SECOND] = res_to_num(race->GetAttrValue(L"unused-resource"));

	dcmapDBREF refRace;
	if(!m_pRaces->Post(&refRace))return false;

	if(m_pPlayers->SelectID(m_PlayerRef))
	{
		m_pPlayers->Update();
		m_pPlayers[DCMDB_PLAYER_RACE] = refRace;
		
		m_pPlayers[DCMDB_PLAYER_TEHNOLOGY] = player->GetAttrValue(L"tehnology");
		m_pPlayers[DCMDB_PLAYER_NATIVE_TEHNOLOGY] = player->GetAttrValue(L"native-tehnology");

		m_pPlayers[DCMDB_PLAYER_MAIN_GAIN] = player->GetAttrValue(L"main-per-turn");
		m_pPlayers[DCMDB_PLAYER_SECOND_GAIN] = player->GetAttrValue(L"second-per-turn");
		m_pPlayers[DCMDB_PLAYER_MONEY_GAIN] = player->GetAttrValue(L"money-per-turn");

		m_pPlayers[DCMDB_PLAYER_MAIN_SPENT] = player->GetAttrValue(L"main-per-turn-spent");
		m_pPlayers[DCMDB_PLAYER_SECOND_SPENT] = player->GetAttrValue(L"second-per-turn-spent");
		m_pPlayers[DCMDB_PLAYER_MONEY_SPENT] = player->GetAttrValue(L"money-per-turn-spent");

		m_pPlayers[DCMDB_PLAYER_MAIN] = player->GetAttrValue(L"main-res");
		m_pPlayers[DCMDB_PLAYER_SECOND] = player->GetAttrValue(L"second-res");
		m_pPlayers[DCMDB_PLAYER_MONEY] = player->GetAttrValue(L"m");
		m_pPlayers[DCMDB_PLAYER_FOOD_BONUS] = player->GetAttrValue(L"food-bonus");

		m_pPlayers[DCMDB_PLAYER_CALCULATECORRUPTION] = true;
		m_pPlayers->Post();
	}

	int hwx = xml_cast<int>(player->GetAttrValue(L"homeworldx"));
	int hwy = xml_cast<int>(player->GetAttrValue(L"homeworldy"));
	
	if(hwx && hwy) // set homeworld
	{
		dcmapDBREF hw;
		gsys::raw_data_buffer<dcmapDBREF> buf;

		if(m_pPlanets->SelectAll())do 
		{
			DCMapPlanetInfo const* pinf = m_pPlanets->GetData();
			if(pinf->owner == m_PlayerRef)
			{
				if(pinf->x == hwx && pinf->y == hwy)hw=pinf->id;
				else buf.push_back(pinf->id);
			}
		} while(m_pPlanets->Next());

		_foreach(gsys::raw_data_buffer<dcmapDBREF>,buf,it)
		{
			if(m_pPlanets->SelectID(hw))
			{
				m_pPlanets->Update();
				m_pPlanets[DCMDB_PLANET_HW] = false;
				m_pPlanets->Post();
			}
		}

		if(m_pPlanets->SelectID(hw))
		{
			m_pPlanets->Update();
			m_pPlanets[DCMDB_PLANET_HW] = true;
			m_pPlanets->Post();
		}
	}

	return true;
}

dcmapDBREF CXMLGameProcessor::GePlanetGroupRef(dcmapDBREF const& PlayerRef,
											  EDCMapPlayerInfo_Records rec)
{
	if(!m_pPlayers->SelectID(PlayerRef))return dcmapDBREF();

	dcmapDBREF OpenGroupRef = m_pPlayers[rec];
	if(!m_pGroups->SelectID(OpenGroupRef))
	{
		m_pGroups->Insert();
		m_pGroups[DCMDB_GROUP_PLAYER] = PlayerRef;
		m_pGroups->Post(&OpenGroupRef);

		m_pPlayers->Update();
		m_pPlayers[rec] = OpenGroupRef;
		m_pPlayers->Post();
	}
	return OpenGroupRef;
}


bool CXMLGameProcessor::ProcessUserPlanets(LPXNode node,int turn,dcmapDBREF const& PlayerRef)
{
	if(!node)return true;

	dcmapDBREF OpenGroupRef = GePlanetGroupRef(m_PlayerRef,DCMDB_PLAYER_OPEN_PLANETS);
	dcmapDBREF OwnGroupRef = GePlanetGroupRef(m_PlayerRef,DCMDB_PLAYER_OWN_PLANETS);

	int n = node->GetChildCount();

	for(int i=0;i<n;i++)
	{
		LPXNode planet = node->GetChild(i);
		if(wcscmp(planet->name.c_str(),L"planet")!=0)continue;

		int x = xml_cast<int>(planet->GetAttrValue(L"x"));
		int y = xml_cast<int>(planet->GetAttrValue(L"y"));

		bool KeepName = false;
		if(m_pPlanets->SelectCoord(x,y))
		{
			KeepName = m_pPlanets[DCMDB_PLANET_KEEP_NAME];
			m_pPlanets->Update();
		}
		else
		{
			m_pPlanets->Insert();
		}

		m_pPlanets[DCMDB_PLANET_OWNER] = m_PlayerRef;
		m_pPlanets[DCMDB_PLANET_LOADER] = m_PlayerRef;
		m_pPlanets[DCMDB_PLANET_TURN] = m_iTurn;

		dcmapWCSTR name = planet->GetAttrValue(L"name");

		if(!KeepName)
		{
			if(name)m_pPlanets[DCMDB_PLANET_CAPTION] = planet->GetAttrValue(L"name");
			else m_pPlanets[DCMDB_PLANET_CAPTION] = L"N"+ToWStr(x)+L":"+ToWStr(y);
		}
 
		m_pPlanets[DCMDB_PLANET_O] = planet->GetAttrValue(L"o");
		m_pPlanets[DCMDB_PLANET_E] = planet->GetAttrValue(L"e");
		m_pPlanets[DCMDB_PLANET_M] = planet->GetAttrValue(L"m");
		m_pPlanets[DCMDB_PLANET_T] = planet->GetAttrValue(L"temperature");
		m_pPlanets[DCMDB_PLANET_S] = planet->GetAttrValue(L"surface");
		m_pPlanets[DCMDB_PLANET_CORRUPTION] = planet->GetAttrValue(L"corruption");
		m_pPlanets[DCMDB_PLANET_POPULATION] = planet->GetAttrValue(L"population");
		m_pPlanets[DCMDB_PLANET_HIDDEN] = planet->GetAttrValue(L"hidden");
		m_pPlanets[DCMDB_PLANET_MY] = true;

		dcmapDBREF RefPlanet;
		m_pPlanets->Post(&RefPlanet);

		if(m_pPlanetGroups->SelectConnect(OpenGroupRef,RefPlanet)) m_pPlanetGroups->Update();
		else
		{
			m_pPlanetGroups->Insert();
			m_pPlanetGroups[DCMDB_PLANETSGROUP_GROUP] = OpenGroupRef;
			m_pPlanetGroups[DCMDB_PLANETSGROUP_PLANET] = RefPlanet;
		}
		m_pPlanetGroups[DCMDB_PLANETSGROUP_TURN] = m_iTurn;
		m_pPlanetGroups->Post();

		if(m_pPlanetGroups->SelectConnect(OwnGroupRef,RefPlanet)) m_pPlanetGroups->Update();
		else
		{
			m_pPlanetGroups->Insert();
			m_pPlanetGroups[DCMDB_PLANETSGROUP_GROUP] = OwnGroupRef;
			m_pPlanetGroups[DCMDB_PLANETSGROUP_PLANET] = RefPlanet;
		}
		m_pPlanetGroups[DCMDB_PLANETSGROUP_TURN] = m_iTurn;
		m_pPlanetGroups->Post();
	}

	// clean up old my planets
	vector<dcmapDBREF> ToClear;
	if(m_pPlanetGroups->SelectByParentID(OwnGroupRef))
	do
	{
		if((int)m_pPlanetGroups[DCMDB_PLANETSGROUP_TURN] < m_iTurn)
			ToClear.push_back(*m_pPlanetGroups->GetID());
	}
	while(m_pPlanetGroups->Next());
	_foreach(vector<dcmapDBREF>,ToClear,it)
	{
		if(m_pPlanetGroups->SelectID(*it))
		{
			dcmapDBREF refPlanet = m_pPlanetGroups[DCMDB_PLANETSGROUP_PLANET];
			m_pPlanetGroups->Delete();
			if(m_pPlanets->SelectID(refPlanet))
			{
				m_pPlanets->Update();
				m_pPlanets[DCMDB_PLANET_OWNER] = dcmapDBREF();
				m_pPlanets[DCMDB_PLANET_MY] = false;
				m_pPlanets->Post();
			}
		}
	}

	return true;
}

bool CXMLGameProcessor::ProcessKnownPlanets(LPXNode node,int turn,dcmapDBREF const& PlayerRef) // optimization needed
{
	if(!node)return true;

	dcmapDBREF OpenGroupRef = GePlanetGroupRef(m_PlayerRef);

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode planet = node->GetChild(i);
		if(wcscmp(planet->name.c_str(),L"planet")!=0)continue;

		DCMapPlanetParamInfo pi;

		pi.x = xml_cast<int>(planet->GetAttrValue(L"x"));
		pi.y = xml_cast<int>(planet->GetAttrValue(L"y"));

		int age = xml_cast<int>(planet->GetAttrValue(L"age"));
		int fake = xml_cast<int>(planet->GetAttrValue(L"fake-info"));
		int open = xml_cast<int>(planet->GetAttrValue(L"open"));
		dcmapWCSTR szCaption = planet->GetAttrValue(L"name");

		int iInf=0;
		dcmapWCSTR szInf = planet->GetAttrValue(L"inf");
		if(szInf && szInf[0])
		{
			iInf = xml_cast<int>(szInf)+1;
		}

		bool db_occupyed = false;
		bool db_actual = false;
		bool geo = false;
		bool db_my = false;
		int db_turn = 0;
		bool db_keep_name = false;

		bool db_exists = m_pPlanets->SelectCoord(pi.x,pi.y);

		if(db_exists)
		{
			db_turn = m_pPlanets[DCMDB_PLANET_TURN];
			db_occupyed = m_pPlayers->SelectID(m_pPlanets[DCMDB_PLANET_OWNER]);
			db_actual = (db_turn >= m_iTurn);
			db_my =m_pPlanets[DCMDB_PLANET_MY];
			db_keep_name =m_pPlanets[DCMDB_PLANET_KEEP_NAME];
		}	

		pi.s = xml_cast<int>(planet->GetAttrValue(L"s"));
		if(pi.s && !fake)
		{
			geo = true;
			pi.o = xml_cast<int>(planet->GetAttrValue(L"o"));
			pi.e = xml_cast<int>(planet->GetAttrValue(L"e"));
			pi.m = xml_cast<int>(planet->GetAttrValue(L"m"));
			pi.t = xml_cast<int>(planet->GetAttrValue(L"t"));
		}	
		int OwnerID = xml_cast<int>(planet->GetAttrValue(L"owner-id"));

		bool actual = (age || open);
		bool empty = OwnerID==0;

		bool import = actual && bImportTrusted; // import trusted planets
		
		if(!import && !db_actual)
		{

			if(db_exists)
			{
				if(db_occupyed) 
				{
					if(empty) import |=  bClearOccupied;
					else      import |= bOverwriteOccupied;
				}
				else import |= !empty && bOverwriteEmpty;
			}
			else
			{
				if(empty) import |=  bAddEmpty;
				else      import |= bAddOccupied;
			}

			if(import && iImportLimit)
			{
				if(iImportLimit==2)
				{
					import = PtInRegion(ImportRegion,pi.x,pi.y);
				}
				else
				{
					dcmapPOINT pt={pi.x,pi.y};
					import = PtInRect(&ImportRect,pt);
					//if(pi.x < ImportRect.left || pi.x > ImportRect.right)import=false;
					//if(pi.y < ImportRect.top || pi.y > ImportRect.bottom)import=false;
				}		

			}
		}

		if(!import && !db_exists)
		{
			if(geo)m_pGeo->UpdateGeo(&pi); // update geo
			continue;
		}

		if(!db_exists)
		{
			m_pPlanets->Insert();
			m_pPlanets[DCMDB_PLANET_X] = pi.x;
			m_pPlanets[DCMDB_PLANET_Y] = pi.y;
		}
		else m_pPlanets->Update();

		if(geo)m_pPlanets->SetGeoData(&pi.geo);

		if(import)
		{			
			if(!empty)
			{
				if(m_pPlayers->SelectGameID(OwnerID)) // found
				{
					m_pPlanets[DCMDB_PLANET_OWNER] = *m_pPlayers->GetID();
				}
				else // id not found :(
				{
					dcmapDBREF pref;
					m_pPlayers->Insert();
					m_pPlayers->SetWideName((L"!Unknown_"+to_wstr(OwnerID)).c_str());
					m_pPlayers[DCMDB_PLAYER_USER_ID] = OwnerID;
					if(m_pPlayers->Post(&pref))
						m_pPlanets[DCMDB_PLANET_OWNER] = pref;
				}
			}
			else
			{
				m_pPlanets[DCMDB_PLANET_OWNER] = dcmapDBREF();	
			}

			if(szCaption && !db_keep_name) 
				m_pPlanets[DCMDB_PLANET_CAPTION] = szCaption;
			else if(!db_exists) 
				m_pPlanets[DCMDB_PLANET_CAPTION] = L"N"+ToWStr(pi.x)+L":"+ToWStr(pi.y);

			m_pPlanets[DCMDB_PLANET_INFILTRATION] = iInf;

			if(actual)
			{
				m_pPlanets[DCMDB_PLANET_TURN] = m_iTurn;
				m_pPlanets[DCMDB_PLANET_LOADER] = m_PlayerRef;
				if(!db_actual)
				{
					m_pPlanets[DCMDB_PLANET_MY] = false;
				}
			}
		}
		dcmapDBREF RefPlanet;
		m_pPlanets->Post(&RefPlanet);

		if(open) // add open planet to group
		{
			if(m_pPlanetGroups->SelectConnect(OpenGroupRef,RefPlanet)) m_pPlanetGroups->Update();
			else
			{
				m_pPlanetGroups->Insert();
				m_pPlanetGroups[DCMDB_PLANETSGROUP_GROUP] = OpenGroupRef;
				m_pPlanetGroups[DCMDB_PLANETSGROUP_PLANET] = RefPlanet;
			}
			m_pPlanetGroups[DCMDB_PLANETSGROUP_TURN] = m_iTurn;
			m_pPlanetGroups->Post();
		}
		else // or remove from group if not open
		{
			if(m_pPlanetGroups->SelectConnect(OpenGroupRef,RefPlanet)) m_pPlanetGroups->Delete();
		}
	}

	// clean up old open planets
	vector<dcmapDBREF> ToClear;
	if(m_pPlanetGroups->SelectByParentID(OpenGroupRef))
	do
	{
		if((int)m_pPlanetGroups[DCMDB_PLANETSGROUP_TURN] < m_iTurn)ToClear.push_back(*m_pPlanetGroups->GetID());
	}
	while(m_pPlanetGroups->Next());
	_foreach(vector<dcmapDBREF>,ToClear,it)
	{
		if(m_pPlanetGroups->SelectID(*it))
		{
			m_pPlanetGroups->Delete();
		}
	}

	return true;
}

bool CXMLGameProcessor::SelectPlayerNameId(dcmapWCSTR szPlayer,int user_id,dcmapDBREF& dref)
{
	if(m_pPlayers->SelectGameID(user_id)) // id found
	{
		dref = *m_pPlayers->GetID();
		if(m_pPlayers->SelectWideName(szPlayer))
		{
			if(dref != *m_pPlayers->GetID()) // rare case: name conflict - resolve it deleting old player
			{
				m_pPlayers->Delete();
				if(m_pPlayers->SelectGameID(user_id))
				{
					m_pPlayers->Update();
					m_pPlayers->SetWideName(szPlayer);
					m_pPlayers->Post(&dref);
				}
			}
		}
		else
		{
			m_pPlayers->Update();
			m_pPlayers->SetWideName(szPlayer);
			m_pPlayers->Post(&dref);
		}
	}
	else if(m_pPlayers->RetrieveByWideName(szPlayer)) // id found
	{
		if((dcmapDWORD)m_pPlayers[DCMDB_PLAYER_USER_ID] != user_id)
		{
			m_pPlayers->Update();
			m_pPlayers[DCMDB_PLAYER_USER_ID] = user_id;
			m_pPlayers->Post(&dref);
		}
	}
	return m_pPlayers->SelectID(dref);
}

int GameDipToDCMap(int dip)
{
	switch(dip)
	{
		case DCMAP_DIP_GAME_ENEMIES: return DCMAP_DIP_ENEMIES;
		case DCMAP_DIP_GAME_NEUTRALS: return DCMAP_DIP_NEUTRALS;
		case DCMAP_DIP_GAME_ALLY: return DCMAP_DIP_ALLY;
		case DCMAP_DIP_GAME_VASSAL: return DCMAP_DIP_ALLY;
		case DCMAP_DIP_GAME_LORD: return DCMAP_DIP_ALLY;
		default: return DCMAP_DIP_UNDEFINED;
	}
}

int GameDipToDCMapFull(int dip)
{
	switch(dip)
	{
	case DCMAP_DIP_GAME_ENEMIES: return DCMAP_DIP_ENEMIES;
	case DCMAP_DIP_GAME_NEUTRALS: return DCMAP_DIP_NEUTRALS;
	case DCMAP_DIP_GAME_ALLY: return DCMAP_DIP_ALLY;
	case DCMAP_DIP_GAME_VASSAL: return DCMAP_DIP_VASSAL;
	case DCMAP_DIP_GAME_LORD: return DCMAP_DIP_LORD;
	default: return DCMAP_DIP_UNDEFINED;
	}
}

bool CXMLGameProcessor::ProcessDiplomacy(LPXNode node,int turn,dcmapDBREF const& PlayerRef) 
{
	if(!node)return true;

	if(!m_pPlayers->SelectID(m_PlayerRef))return false;

	dcmapDBREF PlayerGroupRef = m_pPlayers[DCMDB_PLAYER_DIP_RELATIONS];
	if(!m_pGroups->SelectID(PlayerGroupRef))
	{
		m_pGroups->Insert();
		m_pGroups[DCMDB_GROUP_PLAYER] = m_PlayerRef;
		m_pGroups->Post(&PlayerGroupRef);
		m_pPlayers->Update();
		m_pPlayers[DCMDB_PLAYER_DIP_RELATIONS] = PlayerGroupRef;
		m_pPlayers->Post();
	}

	if(!m_pPlayers->SelectID(m_PlayerRef))return false;

	dcmapDBREF dref = m_pPlayers[DCMDB_PLAYER_DIP];
	int curdip = DCMAP_DIP_UNDEFINED;
	if(m_pDip->SelectID(dref))curdip = m_pDip[DCMDB_DIP_DIPID];
	if(m_pDip->SelectDipID(DCMAP_DIP_MULT) && (bImportDip || curdip == DCMAP_DIP_UNDEFINED))
	{
		m_pPlayers->Update();
		m_pPlayers[DCMDB_PLAYER_DIP] = *m_pDip->GetID();
		m_pPlayers->Post();
	}

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode rel = node->GetChild(i);
		if(wcscmp(rel->name.c_str(),L"rel")!=0)continue;

		dcmapWCSTR szPlayer = rel->GetAttrValue(L"name");
		int user_id = xml_cast<int>(rel->GetAttrValue(L"player"));
		if(!szPlayer || !user_id)continue;
		int gamedip = xml_cast<int>(rel->GetAttrValue(L"type"));

		dcmapDBREF pref;
		if(!SelectPlayerNameId(szPlayer,user_id,pref))continue;

		if(m_pPlayersGroups->SelectConnect(PlayerGroupRef,pref)) m_pPlayersGroups->Update();
		else
		{
			m_pPlayersGroups->Insert();
			m_pPlayersGroups[DCMDB_PLAYERSGROUP_PLAYER] = pref;
			m_pPlayersGroups[DCMDB_PLAYERSGROUP_GROUP] = PlayerGroupRef;
		}
		m_pPlayersGroups[DCMDB_PLAYERSGROUP_TURN] = m_iTurn;
		m_pPlayersGroups[DCMDB_PLAYERSGROUP_DIPID] = GameDipToDCMapFull(gamedip);
		m_pPlayersGroups->Post();

		if(!bImportDip)continue;
		
		dcmapDBREF dref = m_pPlayers[DCMDB_PLAYER_DIP];

		int dip = GameDipToDCMap(gamedip);

		int curdip = DCMAP_DIP_UNDEFINED;
		if(m_pDip->SelectID(dref))curdip = m_pDip[DCMDB_DIP_DIPID];

		bool update = false;

		update |= (curdip == DCMAP_DIP_UNDEFINED);
		update |= (curdip == DCMAP_DIP_ENEMIES && dip != curdip);
		update |= (curdip == DCMAP_DIP_NEUTRALS && dip != curdip);

		update |= (dip == DCMAP_DIP_ENEMIES && dip != curdip);
		update |= (dip == DCMAP_DIP_NEUTRALS && dip != curdip);

		if(dip != DCMAP_DIP_UNDEFINED && update && curdip != dip)
		{
			if(m_pDip->SelectDipID(dip))
			{
				m_pPlayers->Update();
				m_pPlayers[DCMDB_PLAYER_DIP] = *m_pDip->GetID();
				m_pPlayers->Post();
			}
		}
	}
	return true;
}

bool CXMLGameProcessor::ProcessProjectActions(LPXNode node,dcmapDBREF const& refProject)
{
	m_pProjectActs->ClearByParent(refProject);

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode act = node->GetChild(i);
		if(wcscmp(act->name.c_str(),L"act")!=0)continue;

		m_pProjectActs->Insert();

		m_pProjectActs[DCMDB_PROJECTACT_PROJECT] = refProject;
		m_pProjectActs[DCMDB_PROJECTACT_ACTID] = act->GetAttrValue(L"action");

		m_pProjectActs[DCMDB_PROJECTACT_COST_MAIN] = act->GetAttrValue(L"cost-main");
		m_pProjectActs[DCMDB_PROJECTACT_COST_SECOND] = act->GetAttrValue(L"cost-second");
		m_pProjectActs[DCMDB_PROJECTACT_COST_MONEY] = act->GetAttrValue(L"cost-money");
		m_pProjectActs[DCMDB_PROJECTACT_COST_PEPL] = act->GetAttrValue(L"cost-pepl");
		m_pProjectActs[DCMDB_PROJECTACT_MAXCOUNT] = act->GetAttrValue(L"maxcount");
		dcmapWCSTR szPlanets = act->GetAttrValue(L"planet-can-be");
		if(szPlanets)
		{
			while(*szPlanets)
			{
				switch(*szPlanets)
				{
				case L'E':
					m_pProjectActs[DCMDB_PROJECTACT_PLANET_EMPTY] = true;
					break;
				case L'W':
					m_pProjectActs[DCMDB_PROJECTACT_PLANET_DIP_ENEMY] = true;
					break;
				case L'U':
					m_pProjectActs[DCMDB_PROJECTACT_PLANET_DIP_UNDEFINED] = true;
					break;
				case L'N':
					m_pProjectActs[DCMDB_PROJECTACT_PLANET_DIP_NEUTRALS] = true;
					break;
				case L'A':
					m_pProjectActs[DCMDB_PROJECTACT_PLANET_DIP_ALLY] = true;
					break;
				case L'L':
					m_pProjectActs[DCMDB_PROJECTACT_PLANET_DIP_LORD] = true;
					break;
				case L'V':
					m_pProjectActs[DCMDB_PROJECTACT_PLANET_DIP_VASSAL] = true;
					break;
				}
				++szPlanets;
			}
		}
		m_pProjectActs->Post();
	}
	return true;
}

bool CXMLGameProcessor::ProcessProjects(LPXNode node,int turn,dcmapDBREF const& pref)
{
	if(!bImportFleets && !bImportContents)return true;

	if(!node)return true;
	g_pLocalStrings->SelectSection("Components");

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		bool shared=false;
		LPXNode proj = node->GetChild(i);
		if(wcscmp(proj->name.c_str(),L"building_class")!=0)
		{
			if(wcscmp(proj->name.c_str(),L"bc")!=0)continue;
			shared = true;
		}

		// support for shared building types
		dcmapDBREF refOwner = pref;
		int OwnerId = xml_cast<int>(proj->GetAttrValue(L"owner"));
		if(OwnerId)
		{
			if(m_pPlayers->SelectGameID(OwnerId))
			{
				refOwner = *m_pPlayers->GetID();
			}
			else continue;
		}

		int id = xml_cast<int>(proj->GetAttrValue(L"building-id"));

		if(m_pProjects->SelectGameID(refOwner,id)) m_pProjects->Update();
		else 
		{	
			m_pProjects->Insert();
			m_pProjects[DCMDB_PROJECT_GAMEID] = id;
			m_pProjects[DCMDB_PROJECT_OWNER] = refOwner;
		}

		int carapace = xml_cast<int>(proj->GetAttrValue(L"carapace"));

		if(!carapace && id < DCMDB_COMPONENT_BUILDING_FLAG) // fake carapace id based on building id
		{
			carapace = id | DCMDB_COMPONENT_BUILDING_FLAG;
		}

		wstring szName = proj->GetAttrStr(L"name");
		wstring szDesc = proj->GetAttrStr(L"description");

		bool comp = m_pComponents->SelectGameID(carapace);

		if(szName.empty())
		{
			string sec;
			if(id<1024)sec = "b"+gsys::to_str(id);
			else sec = "c"+gsys::to_str(carapace);
			szName = g_pLocalStrings->GetSectionStringW(sec.c_str());
			if(szName.empty() && !(carapace&DCMDB_COMPONENT_BUILDING_FLAG))
			{
				sec = "c"+gsys::to_str(carapace);
				szName = g_pLocalStrings->GetSectionStringW(sec.c_str());
			}
			if(szName.empty())
			{
				if(comp)szName = m_pComponents[DCMDB_COMPONENT_NAME].wstr();
			}

			szDesc = g_pLocalStrings->GetSectionStringW((sec+".desc").c_str());
			if(szDesc.empty())
			{
				if(comp) szDesc = m_pComponents[DCMDB_COMPONENT_DESCRIPTION].wstr();
			}
		}

		if(!szName.empty())
		{
			m_pProjects[DCMDB_PROJECT_NAME] = szName;
		}
		else
		{
			if(id<1000) m_pProjects[DCMDB_PROJECT_NAME] = "bc"+gsys::to_str(id);
			else m_pProjects[DCMDB_PROJECT_NAME] = "cp"+gsys::to_str(carapace);	
		}
		m_pProjects[DCMDB_PROJECT_DESCRIPTION] = szDesc;

		m_pProjects[DCMDB_PROJECT_TURN] = m_iTurn;

		m_pProjects[DCMDB_PROJECT_CARAPACE] = carapace;

		if(m_pComponents->SelectGameID(carapace))
		{
			m_pProjects[DCMDB_PROJECT_CHASSIS] = *m_pComponents->GetID();
		}
		else // create dummy component for future use
		{
			m_pComponents->Insert();
			m_pComponents[DCMDB_COMPONENT_GAMEID] = carapace;
			m_pComponents[DCMDB_COMPONENT_NAME] = szName;
			m_pComponents[DCMDB_COMPONENT_DESCRIPTION] = szDesc;

			if(carapace & DCMDB_COMPONENT_BUILDING_FLAG)
				m_pComponents[DCMDB_COMPONENT_ICON_NAME] = L"b"+to_wstr(carapace & ~DCMDB_COMPONENT_BUILDING_FLAG);
			else
				m_pComponents[DCMDB_COMPONENT_ICON_NAME] = L"c"+to_wstr(carapace);

			dcmapDBREF cpRef;
			m_pComponents->Post(&cpRef);
			UpdateComponentBuildInd(cpRef);
			m_pProjects[DCMDB_PROJECT_CHASSIS] = cpRef;
		}

#define GET_PROJECT(x) m_pProjects[DCMDB_PROJECT_##x] = proj->GetAttrValue(L#x);
#define GET_PROJECT2(x,y) m_pProjects[DCMDB_PROJECT_##x##_##y] = proj->GetAttrValue(L#x L"-" L#y);
#define GET_PROJECT3(x,y,z) m_pProjects[DCMDB_PROJECT_##x##_##y##_##z] = proj->GetAttrValue(L#x L"-" L#y L"-" L#z);

		GET_PROJECT(MAXCOUNT);
		GET_PROJECT(WEIGHT);
		GET_PROJECT(OFFENSIVE);
		GET_PROJECT(COLOR);

		GET_PROJECT3(IS,GROUND,UNIT);
		GET_PROJECT3(IS,SPACE,SHIP);
		GET_PROJECT3(REQ,TEHN,LEVEL);

		GET_PROJECT2(COST,MAIN);
		GET_PROJECT2(COST,SECOND);
		GET_PROJECT2(COST,MONEY);
		GET_PROJECT2(COST,PEPL);

		GET_PROJECT2(REQUIRES,PEPL);

		GET_PROJECT2(SUPPORT,MAIN);
		GET_PROJECT2(SUPPORT,SECOND);

		GET_PROJECT2(HIT,POINTS);

		GET_PROJECT2(BONUS,O);
		GET_PROJECT2(BONUS,E);
		GET_PROJECT2(BONUS,M);
		GET_PROJECT2(BONUS,S);
		GET_PROJECT2(BONUS,PRODUCTION);
		GET_PROJECT2(BONUS,SURFACE);

		GET_PROJECT2(BUILD,SPEED);

		GET_PROJECT2(SCAN,STRENGTH);
		GET_PROJECT2(DETECT,RANGE);

		GET_PROJECT2(IS,WAR);

		GET_PROJECT2(LASER,DAMAGE);
		GET_PROJECT2(BOMB,DAMAGE);

		GET_PROJECT2(LASER,NUMBER);
		GET_PROJECT2(BOMB,NUMBER);

		GET_PROJECT2(LASER,DR);
		GET_PROJECT2(BOMB,DR);
		GET_PROJECT2(LASER,AR);
		GET_PROJECT2(BOMB,AR);

		GET_PROJECT2(LASER,TARGET);

		GET_PROJECT2(BOMB,TARGET);

		GET_PROJECT2(IS,BUILDING);
		GET_PROJECT2(IS,TRANSPORTABLE);

		GET_PROJECT2(CARRIER,CAPACITY);
		GET_PROJECT2(TRANSPORT,CAPACITY);

		GET_PROJECT2(FLY,RANGE);
		GET_PROJECT2(FLY,SPEED);

		GET_PROJECT2(BUILDING,CLASS);

#undef GET_PROJECT
#undef GET_PROJECT2

		dcmapDBREF pRef;
		m_pProjects->Post(&pRef);

		LPXNode actions = proj->GetChild(L"actions");
		if(actions)
		{
			ProcessProjectActions(actions,pRef);
		}
	}
	return true;
}

void CXMLGameProcessor::FillGarrizonLike(dcmapWCSTR NameFormat,dcmapWCSTR CaptionFormat,int x, int y,int turn,dcmapDBREF const& pref)
{
	m_pFleets[DCMDB_FLEET_CAPTION] = str(wformat(CaptionFormat) % x %y); 
	m_pFleets[DCMDB_FLEET_NAME] = str(wformat(NameFormat) % x %y); 
	m_pFleets[DCMDB_FLEET_EXTNAME]    = to_wstr(x)+L":"+to_wstr(y); 
	m_pFleets[DCMDB_FLEET_GARRISON] = true;
	m_pFleets[DCMDB_FLEET_X] = x;
	m_pFleets[DCMDB_FLEET_Y] = y;
	m_pFleets[DCMDB_FLEET_OWNER] = pref;
	m_pFleets[DCMDB_FLEET_RULER] = pref;
	m_pFleets[DCMDB_FLEET_LOADER] = pref;
	m_pFleets[DCMDB_FLEET_FOREIGN] = false;
	m_pFleets[DCMDB_FLEET_TURN] = m_iTurn;
	m_pFleets[DCMDB_FLEET_OUTDATED] = false;
	m_pFleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_Ungroupped;
}

bool CXMLGameProcessor::ProcessGarrison( LPXNode node,int turn,dcmapDBREF const& pref )
{
	if(!bImportContents)return true;
	if(!node)return true;

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode garr = node->GetChild(i);
		if(wcscmp(garr->name.c_str(),L"harrison")!=0)continue;

		int x = xml_cast<int>(garr->GetAttrValue(L"x"));
		int y = xml_cast<int>(garr->GetAttrValue(L"y"));

		// get garrisoned planet
		if(!m_pPlanets->RetrieveByCoords(x,y))return false;
		dcmapDBREF Garrizon = m_pPlanets[DCMDB_PLANET_GARRISON];
		dcmapDBREF Buildings = m_pPlanets[DCMDB_PLANET_BUILDINGS];
		dcmapDBREF Queue = m_pPlanets[DCMDB_PLANET_QUEUE];
		dcmapDBREF Planet = *m_pPlanets->GetID();

		if(!m_pFleets->SelectID(Buildings))m_pFleets->Insert();
		else m_pFleets->Update();
		FillGarrizonLike(LSTRW(BuildingsNameFormat),LSTRW(BuildingsCaptionFormat),x,y,m_iTurn,pref);
		m_pFleets[DCMDB_FLEET_TYPE] = DCMAP_FTP_Buildings;
		m_pFleets->Post(&Buildings);
		if(!Buildings.Valid())return false;

		if(!m_pFleets->SelectID(Queue))m_pFleets->Insert();
		else m_pFleets->Update();
		FillGarrizonLike(LSTRW(QueueNameFormat),LSTRW(QueueCaptionFormat),x,y,m_iTurn,pref);
		m_pFleets[DCMDB_FLEET_TYPE] = DCMAP_FTP_Queue;
		m_pFleets->Post(&Queue);
		if(!Queue.Valid())return false;	

		if(!m_pFleets->SelectID(Garrizon))m_pFleets->Insert();
		else m_pFleets->Update();
		FillGarrizonLike(LSTRW(GarrisonNameFormat),LSTRW(GarrisonCaptionFormat),x,y,m_iTurn,pref);
		m_pFleets[DCMDB_FLEET_TYPE] = DCMAP_FTP_Garrizon;
		m_pFleets->Post(&Garrizon);
		if(!Garrizon.Valid())return false;

		if(m_pPlanets->SelectCoord(x,y))
		{
			m_pPlanets->Update();
			m_pPlanets[DCMDB_PLANET_GARRISON] = Garrizon;
			m_pPlanets[DCMDB_PLANET_BUILDINGS] = Buildings;
			m_pPlanets[DCMDB_PLANET_QUEUE] = Queue;
			m_pPlanets->Post();	
			
			int nunits = ProcessUnits(garr,m_iTurn,pref,Garrizon,false,false);
			int nbld = ProcessUnits(garr,m_iTurn,pref,Buildings,false,true);
			int nque = ProcessUnits(garr,m_iTurn,pref,Queue,true,false);

			UpdateBuildings(m_iTurn,Planet,Buildings);
			UpdateGarrizon(m_iTurn,Planet,Garrizon);

			m_pFleets->SelectID(Garrizon);
			m_pFleets->Update();
			m_pFleets[DCMDB_FLEET_SHIP_COUNT] = nunits;
			m_pFleets->Post();

			m_pFleets->SelectID(Buildings);
			m_pFleets->Update();
			m_pFleets[DCMDB_FLEET_SHIP_COUNT] = nbld;
			m_pFleets->Post();

			m_pFleets->SelectID(Queue);
			m_pFleets->Update();
			m_pFleets[DCMDB_FLEET_SHIP_COUNT] = nque;
			m_pFleets->Post();

			// calculate queue build times
			m_pFleetMan->CalculateFleet(Queue);
		}
	}
	return true;
}

int CXMLGameProcessor::ProcessUnits(LPXNode node,int turn,dcmapDBREF const& player,dcmapDBREF const& parent,bool need_queue,bool need_building)
{
	if(!node)return true;
	OutdateUnits(parent);

	int actual=0;

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		bool queue=false;
		bool building=false;
		bool disbanded=false;
		LPXNode unit = node->GetChild(i);
		if(wcscmp(unit->name.c_str(),L"u")!=0)
		{
			if(wcscmp(unit->name.c_str(),L"c-u")!=0)continue;
			queue=true;
		}
		if(need_queue != queue)continue;

		int bc = xml_cast<int>(unit->GetAttrValue(L"bc"));
		if(!bc)continue;
		if(!m_pProjects->SelectGameID(player,bc))continue;
		building = m_pProjects[DCMDB_PROJECT_IS_BUILDING];

		if(!need_queue && need_building != building)continue;


		int id = xml_cast<int>(unit->GetAttrValue(L"id"));
		if(!id)continue;

		int lvl = xml_cast<int>(unit->GetAttrValue(L"lvl"));
		int hp = xml_cast<int>(unit->GetAttrValue(L"hp"));
		int age = xml_cast<int>(unit->GetAttrValue(L"age"));


		if(m_pUnits->SelectGameID(id)) 
		{
			if(m_pUnits[DCMDB_BUILDING_DISBANDED])
			{
				if((int)m_pUnits[DCMDB_BUILDING_TURN] == m_iTurn) 
					disbanded=true; //
			}
			m_pUnits->Update();
		}
		else m_pUnits->Insert();

		if(queue)
		{
			m_pUnits[DCMDB_BUILDING_COMPLETE] = xml_cast<int>(unit->GetAttrValue(L"done"));
		}

		m_pUnits[DCMDB_BUILDING_DISBANDED] = disbanded;

		m_pUnits[DCMDB_BUILDING_QUEUED] = queue;

		m_pUnits[DCMDB_BUILDING_PROJECT] = *m_pProjects->GetID();

		m_pUnits[DCMDB_BUILDING_CARAPACE] = (int)m_pProjects[DCMDB_PROJECT_CARAPACE];
		m_pUnits[DCMDB_BUILDING_WEIGHT] = (int)m_pProjects[DCMDB_PROJECT_WEIGHT];
		m_pUnits[DCMDB_BUILDING_COLOR] = (int)m_pProjects[DCMDB_PROJECT_COLOR];

		m_pUnits[DCMDB_BUILDING_PLANETARY] = building;

		m_pUnits[DCMDB_BUILDING_GAMEID] = id;
		m_pUnits[DCMDB_BUILDING_OWNER] = parent;
		m_pUnits[DCMDB_BUILDING_TURN] = m_iTurn;
		m_pUnits[DCMDB_BUILDING_HP] = hp;

		m_pUnits[DCMDB_BUILDING_AGE] = age;

		m_pUnits[DCMDB_BUILDING_OUTDATED] = false;
		dcmapDBREF RefUnit;
		m_pUnits->Post(&RefUnit);


		int i_tag = xml_cast<int>(unit->GetAttrValue(L"i-tag"));

		int unwrapped = 0;
		dcmapWCSTR strUnwrapped = unit->GetAttrValue(L"unwrapped");
		if(strUnwrapped && strUnwrapped[0])
				unwrapped = xml_cast<int>(strUnwrapped)+1;

		bool tagged = false;

		LPXNode tag = unit->GetChild(L"tag");
		if(tag) // read one of posible container tags
		{
			if(m_pUnitTags->SelectByParentID(RefUnit))
			{
				m_pUnitTags->Update();
			}
			else
			{
				m_pUnitTags->Insert();
				m_pUnitTags[DCMDB_UNITTAG_UNIT] = RefUnit;
			}
			m_pUnitTags[DCMDB_UNITTAG_LOADING] = false;
			m_pUnitTags[DCMDB_UNITTAG_UNWRAPPED] = unwrapped;
			m_pUnitTags[DCMDB_UNITTAG_LOAD_FLEET] = dcmapDBREF();

			tagged = true;	


			if(LPXNode resource = tag->GetChild(L"res-in-pack"))
			{
				dcmapWCSTR rt = resource->GetAttrValue(L"res");
				if(rt && rt[0])
				{
					int irt=0;
					if(rt[0] == L'o')irt=0;
					else if(rt[0] == L'e')irt=1;
					else if(rt[0] == L'm')irt=2;
					else if(rt[0] == L'c')irt=3;

					m_pUnitTags[DCMDB_UNITTAG_COUNT] = resource->GetAttrValue(L"amount");
					m_pUnitTags[DCMDB_UNITTAG_TYPE] = DCMAP_UTT_RESOURCE;
					m_pUnitTags[DCMDB_UNITTAG_ITEMID] = irt;
				}
			}
			else if(LPXNode part = tag->GetChild(L"part"))
			{
				int partid = xml_cast<int>(part->GetAttrValue(L"id"));
				int lvl = xml_cast<int>(part->GetAttrValue(L"lvn"));
				int raceid = xml_cast<int>(part->GetAttrValue(L"race"));

				m_pUnitTags[DCMDB_UNITTAG_COUNT] = i_tag;
				m_pUnitTags[DCMDB_UNITTAG_TYPE] = DCMAP_UTT_PART;
				m_pUnitTags[DCMDB_UNITTAG_ITEMID] = partid;

				if(m_pComponents->SelectGameID(partid))
					m_pUnitTags[DCMDB_UNITTAG_CHASSIS] = m_pComponents->GetID();

				if(m_pRaces->SelectGameID(raceid))
					m_pUnitTags[DCMDB_UNITTAG_RACE] = m_pRaces->GetID();
		
			}
			else if(LPXNode part = tag->GetChild(L"uppart"))
			{
				int partid = xml_cast<int>(part->GetAttrValue(L"id"));
				int lvl = xml_cast<int>(part->GetAttrValue(L"lvn"));
				int raceid = xml_cast<int>(part->GetAttrValue(L"race"));

				m_pUnitTags[DCMDB_UNITTAG_COUNT] = i_tag;
				m_pUnitTags[DCMDB_UNITTAG_TYPE] = DCMAP_UTT_PART;
				m_pUnitTags[DCMDB_UNITTAG_ITEMID] = partid;

				if(m_pComponents->SelectGameID(partid))
					m_pUnitTags[DCMDB_UNITTAG_CHASSIS] = m_pComponents->GetID();

				if(m_pRaces->SelectGameID(raceid))
					m_pUnitTags[DCMDB_UNITTAG_RACE] = m_pRaces->GetID();

			}			
			else if(LPXNode item = tag->GetChild(L"itm-in-pack"))
			{
				int itemid = xml_cast<int>(item->GetAttrValue(L"id"));
				int classid = xml_cast<int>(item->GetAttrValue(L"class-id"));
				int lvl = xml_cast<int>(item->GetAttrValue(L"lvl"));
				int carapace = xml_cast<int>(item->GetAttrValue(L"carapace"));

				m_pUnitTags[DCMDB_UNITTAG_COUNT] = 1;
				m_pUnitTags[DCMDB_UNITTAG_TYPE] = DCMAP_UTT_UNIT;
				m_pUnitTags[DCMDB_UNITTAG_ITEMID] = itemid;

				if(m_pProjects->SelectGameID(m_PlayerRef,classid))
				{
					m_pUnitTags[DCMDB_UNITTAG_PROJECT] = m_pProjects->GetID();
					m_pUnitTags[DCMDB_UNITTAG_CHASSIS] = (dcmapDBREF)m_pProjects[DCMDB_PROJECT_CHASSIS];
				}

				if(m_pComponents->SelectGameID(carapace))
					m_pUnitTags[DCMDB_UNITTAG_CHASSIS] = m_pComponents->GetID();
			}			

			m_pUnitTags->Post();
		}

		m_pUnits->Update();
		m_pUnits[DCMDB_BUILDING_TAGGED] = tagged;
		m_pUnits->Post();

		if(!disbanded)	actual++;
	}
	return actual;
}

// outdate units so old ones can be removed later
void CXMLGameProcessor::OutdateUnits(dcmapDBREF const& parent)
{
	std::vector<dcmapDBREF> vecUnits;
	if(m_pUnits->SelectByParentID(parent))do{vecUnits.push_back(*m_pUnits->GetID());}while(m_pUnits->Next());	
	for(int i=0;i<vecUnits.size();i++)
	{
		m_pUnits->SelectID(vecUnits[i]);
		m_pUnits->Update();
		m_pUnits[DCMDB_BUILDING_OUTDATED] = true;
		m_pUnits->Post();
	}
}

int CXMLGameProcessor::ProcessForeignUnits(LPXNode node,int turn,dcmapDBREF const& player,dcmapDBREF const& parent_fleet)
{
	if(!node)return true;
	OutdateUnits(parent_fleet);

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode unit = node->GetChild(i);
		if(wcscmp(unit->name.c_str(),L"allien-ship")!=0)continue;

		int id = xml_cast<int>(unit->GetAttrValue(L"id"));
		if(!id)continue;

		int class_id = xml_cast<int>(unit->GetAttrValue(L"class-id"));
		if(!class_id)continue;

		int carapace = xml_cast<int>(unit->GetAttrValue(L"carapace"));
		if(!carapace)continue;

		int weight = xml_cast<int>(unit->GetAttrValue(L"weight"));
		int color  = xml_cast<int>(unit->GetAttrValue(L"color"));

		if(m_pUnits->SelectGameID(id)) m_pUnits->Update();
		else m_pUnits->Insert();

		m_pUnits[DCMDB_BUILDING_GAMEID] = id;
		m_pUnits[DCMDB_BUILDING_OWNER] = parent_fleet;
		m_pUnits[DCMDB_BUILDING_TURN] = m_iTurn;
		m_pUnits[DCMDB_BUILDING_WEIGHT] = weight;
		m_pUnits[DCMDB_BUILDING_CARAPACE] = carapace;
		m_pUnits[DCMDB_BUILDING_COLOR] = color;

		m_pUnits[DCMDB_BUILDING_OUTDATED] = false;

		m_pUnits->Post();


	}
	return n;
}

bool CXMLGameProcessor::ProcessFleets( LPXNode node,int turn,dcmapDBREF const& pref )
{
	if(!node)return true;
	if(!bImportFleets)return true;

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode fleet = node->GetChild(i);
		if(wcscmp(fleet->name.c_str(),L"fleet")!=0)continue;

		// support for shared fleets
		dcmapDBREF refOwner = pref;
		int OwnerId = xml_cast<int>(fleet->GetAttrValue(L"owner"));
		if(OwnerId)
		{
			if(m_pPlayers->SelectGameID(OwnerId))
			{
				refOwner = *m_pPlayers->GetID();
			}
			else continue;
		}

		int db_turn=0;
		bool db_deleted=false;

		int id = xml_cast<int>(fleet->GetAttrValue(L"id"));
		if(!id)continue;
		if(m_pFleets->SelectGameID(id)) 
		{
			db_turn = m_pFleets[DCMDB_FLEET_TURN];
			db_deleted = m_pFleets[DCMDB_FLEET_DELETED];
			m_pFleets->Update();
		}
		else 
		{
			m_pFleets->Insert();
			m_pFleets[DCMDB_FLEET_GAMEID] = id;
		}

		dcmapWCSTR szName = fleet->GetAttrValue(L"name");
		dcmapWCSTR szExtname = fleet->GetAttrValue(L"ext-name");

		m_pFleets[DCMDB_FLEET_TURN] = m_iTurn;
		m_pFleets[DCMDB_FLEET_LOADER] = pref;
		m_pFleets[DCMDB_FLEET_HISTORY] = false;
		m_pFleets[DCMDB_FLEET_OUTDATED] = false;


		m_pFleets[DCMDB_FLEET_NAME] = szName;
		if(szExtname)m_pFleets[DCMDB_FLEET_EXTNAME] = szExtname;
		else m_pFleets[DCMDB_FLEET_EXTNAME] = szName;

		m_pFleets[DCMDB_FLEET_CAPTION] = szName;

		dcmapWCSTR szBehavior = fleet->GetAttrValue(L"behavior");
		if(szBehavior)
		{
			int n = wcslen(szBehavior);
			n = min(n,6);
			dcmapDWORD Behavior=0;
			for(int i=0;i<n;i++)
			{
				Behavior = (Behavior << 4)|(szBehavior[i]-L'0');
			}
			m_pFleets[DCMDB_FLEET_BEHAVIOR] = Behavior;
		}
		// HACK: Don't realy know what to do with this:
		dcmapWCSTR szFleetChanges = fleet->GetAttrValue(L"fleet-changes");
		if(szFleetChanges && szFleetChanges[0])
		{
			m_pFleets[DCMDB_FLEET_CHANGES] = szFleetChanges[0]-L'0'; // Ugly hack
		}
		else m_pFleets[DCMDB_FLEET_CHANGES] = 0;

		m_pFleets[DCMDB_FLEET_X] = fleet->GetAttrValue(L"x");
		m_pFleets[DCMDB_FLEET_Y] = fleet->GetAttrValue(L"y");
		m_pFleets[DCMDB_FLEET_FROM_X] = fleet->GetAttrValue(L"from-x");
		m_pFleets[DCMDB_FLEET_FROM_Y] = fleet->GetAttrValue(L"from-y");

		m_pFleets[DCMDB_FLEET_FLY_RANGE] = fleet->GetAttrValue(L"fly-range");
		m_pFleets[DCMDB_FLEET_FLY_SPEED] = fleet->GetAttrValue(L"fly-speed");

		m_pFleets[DCMDB_FLEET_IN_TRANSIT] = fleet->GetAttrValue(L"in-transit");

		m_pFleets[DCMDB_FLEET_TTA] = fleet->GetAttrValue(L"tta");

		m_pFleets[DCMDB_FLEET_STEALTH] = fleet->GetAttrValue(L"stealth-lvl");
		m_pFleets[DCMDB_FLEET_HIDDEN] = fleet->GetAttrValue(L"hidden");

		m_pFleets[DCMDB_FLEET_SCRIPT_ACTIVE] = fleet->GetAttrValue(L"script-active");
		m_pFleets[DCMDB_FLEET_SCRIPT_OK] = fleet->GetAttrValue(L"script-ok");
		m_pFleets[DCMDB_FLEET_SCRIPT_OK] = fleet->GetAttrValue(L"script-ok");

		m_pFleets[DCMDB_FLEET_SCRIPT_MESSAGE] = fleet->GetAttrValue(L"script-message");

		m_pFleets[DCMDB_FLEET_OWNER] = refOwner;
		m_pFleets[DCMDB_FLEET_RULER] = pref;
		m_pFleets[DCMDB_FLEET_DIP] = DCMAP_DIP_MULT;

		m_pFleets[DCMDB_FLEET_FOREIGN] = false;
		m_pFleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_Ungroupped;
		m_pFleets[DCMDB_FLEET_TYPE] = DCMAP_FTP_Fleet;

		dcmapDBREF RefFleet;
		m_pFleets->Post(&RefFleet);
		int nunits = ProcessUnits(fleet,m_iTurn,refOwner,*m_pFleets->GetID());
		bool deleted = (db_turn >= m_iTurn) && db_deleted && nunits==0;

		m_pFleets->SelectID(RefFleet);
		m_pFleets->Update();
		m_pFleets[DCMDB_FLEET_DELETED] = deleted;
		m_pFleets[DCMDB_FLEET_SHIP_COUNT] = nunits;
		m_pFleets->Post();

		if(!deleted) m_pFleetMan->CalculateFleet(RefFleet);

	}
	return true;
	// remove non-existent fleets
}

bool CXMLGameProcessor::ProcessForeighnFleets( LPXNode node,int turn,dcmapDBREF const& pref )
{
	if(!bImportAlienFleets)return true;
	if(!node)return true;

	// remove old unknown fleets

	std::vector<dcmapDBREF> vecFleets;
	if(m_pFleets->SelectAll())do
	{
		DCMapFleetInfo const* finf = m_pFleets->GetData();
		if(finf->loader != pref)continue;
		if(finf->game_id != 0)continue;
		if(finf->group > DCMAP_FGRP_Groupped )continue;
		if( (finf->flags&DCMAP_FF_Foreign) != DCMAP_FF_Foreign)continue;
		vecFleets.push_back(finf->id);
	}
	while(m_pFleets->Next());

	for(int i=0;i<vecFleets.size();i++)
	{
		m_pFleets->SelectID(vecFleets[i]);
		m_pFleets->Delete();
	}

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode fleet = node->GetChild(i);
		if(wcscmp(fleet->name.c_str(),L"allien-fleet")!=0)continue;

		dcmapWCSTR szPlayer = fleet->GetAttrValue(L"player-name");
		int user_id = xml_cast<int>(fleet->GetAttrValue(L"player-id"));

		dcmapDBREF rplayer;
		if(szPlayer && user_id)
		{
			if(!SelectPlayerNameId(szPlayer,user_id,rplayer))continue;
		}
		// update player race ???

		int fleetid = xml_cast<int>(fleet->GetAttrValue(L"fleet-id"));
		dcmapWCSTR szFleetName = fleet->GetAttrValue(L"name");

		if(fleetid) // known fleet
		{
			if(!szFleetName)continue;
			if(!rplayer.Valid())continue;

			if(m_pFleets->SelectGameID(fleetid))
			{
				if(!m_pFleets[DCMDB_FLEET_FOREIGN])continue;
				m_pFleets->Update();
			}
			else 
			{
				m_pFleets->Insert();
			}

			if(m_pDip->SelectID(m_pPlayers[DCMDB_PLAYER_DIP]))
			{
				m_pFleets[DCMDB_FLEET_DIP] = (int)m_pDip[DCMDB_DIP_DIPID];
			}
			else m_pFleets[DCMDB_FLEET_DIP] = DCMAP_DIP_UNDEFINED;

			m_pFleets[DCMDB_FLEET_TURN] = m_iTurn;
			m_pFleets[DCMDB_FLEET_LOADER] = pref;
			m_pFleets[DCMDB_FLEET_HISTORY] = false;
			m_pFleets[DCMDB_FLEET_OUTDATED] = false;

			m_pFleets[DCMDB_FLEET_X] = fleet->GetAttrValue(L"x");
			m_pFleets[DCMDB_FLEET_Y] = fleet->GetAttrValue(L"y");

			m_pFleets[DCMDB_FLEET_GAMEID] = fleetid;
			m_pFleets[DCMDB_FLEET_FOREIGN] = true;

			m_pFleets[DCMDB_FLEET_TURN] = m_iTurn;
			m_pFleets[DCMDB_FLEET_OWNER] = rplayer;

			m_pFleets[DCMDB_FLEET_HIDDEN] = fleet->GetAttrValue(L"hidden");
			m_pFleets[DCMDB_FLEET_WEIGHT] = fleet->GetAttrValue(L"sum-weight");
			m_pFleets[DCMDB_FLEET_SHIP_COUNT] = fleet->GetAttrValue(L"large-ship-count");

			m_pFleets[DCMDB_FLEET_NAME] = szFleetName;
			m_pFleets[DCMDB_FLEET_EXTNAME] = szFleetName;
			m_pFleets[DCMDB_FLEET_GROUP] = DCMAP_FGRP_Ungroupped;
			m_pFleets[DCMDB_FLEET_TYPE] = DCMAP_FTP_Fleet;

			if(!wstring(m_pFleets[DCMDB_FLEET_CAPTION]).length())
			{
				m_pFleets[DCMDB_FLEET_CAPTION] = szFleetName;
			}

			dcmapDBREF fleetref;
			if(m_pFleets->Post(&fleetref))
			{
				ProcessForeignUnits(fleet,m_iTurn,pref,fleetref);
			}
		}
		else // unknown alien fleet
		{
			m_pFleets->Insert();
			m_pFleets[DCMDB_FLEET_TURN]   = m_iTurn;
			m_pFleets[DCMDB_FLEET_LOADER] = pref;

			m_pFleets[DCMDB_FLEET_NAME]   = LSTRW(UnknownFleet);

			m_pFleets[DCMDB_FLEET_X]      = fleet->GetAttrValue(L"x");
			m_pFleets[DCMDB_FLEET_Y]      = fleet->GetAttrValue(L"y");
			m_pFleets[DCMDB_FLEET_FROM_X] = fleet->GetAttrValue(L"from-x");
			m_pFleets[DCMDB_FLEET_FROM_Y] = fleet->GetAttrValue(L"from-y");
			m_pFleets[DCMDB_FLEET_TTA]    = fleet->GetAttrValue(L"turns-till-arrival");
			m_pFleets[DCMDB_FLEET_HIDDEN] = fleet->GetAttrValue(L"hidden");;

			m_pFleets[DCMDB_FLEET_WEIGHT] = fleet->GetAttrValue(L"sum-weight");
			m_pFleets[DCMDB_FLEET_SHIP_COUNT] = fleet->GetAttrValue(L"large-ship-count");
			m_pFleets[DCMDB_FLEET_FOREIGN] = true;
			m_pFleets[DCMDB_FLEET_IN_TRANSIT] = true;
			m_pFleets[DCMDB_FLEET_TYPE] = DCMAP_FTP_Fleet;
			m_pFleets[DCMDB_FLEET_DIP] = DCMAP_DIP_UNDEFINED;
			m_pFleets->Post();
		}
	}
	return true;
}

void CXMLGameProcessor::UpdateGarrizon(int turn,dcmapDBREF const& Planet,dcmapDBREF const& Garrison)
{
	if(!m_pPlanets->SelectID(Planet))return;
	if(m_pPlanets[DCMDB_PLANET_BUILD_PLANNING])return;

	const int max_buildn = 32;
	int buildings[max_buildn+1];
	memset(buildings,0,sizeof(buildings));
	if(m_pUnits->SelectByParentID(Garrison))
		do 
		{
			if((int)m_pUnits[DCMDB_BUILDING_TURN] != m_iTurn)continue;

			if(m_pUnits[DCMDB_BUILDING_OUTDATED])continue;
			if(m_pUnits[DCMDB_BUILDING_DISBANDED])continue;
			if(m_pUnits[DCMDB_BUILDING_QUEUED])continue;

			if(m_pProjects->SelectID(m_pUnits[DCMDB_BUILDING_PROJECT]))
			{
				ulong pid=m_pProjects[DCMDB_PROJECT_GAMEID];
				if(pid <= max_buildn)buildings[pid]++;
			}
		} 
		while(m_pUnits->Next());

		if(!m_pPlanets->SelectID(Planet))return;
		m_pPlanets->Update();
		m_pPlanets[DCMDB_PLANET_HAS_GOVERNORS] = buildings[13];
		m_pPlanets->Post();
}

void CXMLGameProcessor::UpdateBuildings(int turn,dcmapDBREF const& Planet,dcmapDBREF const& Garrison)
{
	if(!m_pPlanets->SelectID(Planet))return;
	if(m_pPlanets[DCMDB_PLANET_BUILD_PLANNING])return;

	const int max_buildn = 32;
	int buildings[max_buildn+1];
	memset(buildings,0,sizeof(buildings));
	if(m_pUnits->SelectByParentID(Garrison))
		do 
		{
			if((int)m_pUnits[DCMDB_BUILDING_TURN] != m_iTurn)continue;

			if(m_pUnits[DCMDB_BUILDING_OUTDATED])continue;
			if(m_pUnits[DCMDB_BUILDING_DISBANDED])continue;
			if(m_pUnits[DCMDB_BUILDING_QUEUED])continue;

			if(m_pProjects->SelectID(m_pUnits[DCMDB_BUILDING_PROJECT]))
			{
				ulong pid=m_pProjects[DCMDB_PROJECT_GAMEID];
				if(pid <= max_buildn)buildings[pid]++;
			}
		} 
		while(m_pUnits->Next());

		if(!m_pPlanets->SelectID(Planet))return;
		m_pPlanets->Update();

		m_pPlanets[DCMDB_PLANET_HAS_MINES] = buildings[1];
		m_pPlanets[DCMDB_PLANET_HAS_BIOFARMS] = buildings[2];
		m_pPlanets[DCMDB_PLANET_HAS_POWERPLANTS] = buildings[3];

		m_pPlanets[DCMDB_PLANET_HAS_FACTORIES] = buildings[14];
		m_pPlanets[DCMDB_PLANET_HAS_COMPRESSORS] = buildings[31];

		m_pPlanets[DCMDB_PLANET_HAS_BARRACKS] = buildings[4];
		m_pPlanets[DCMDB_PLANET_HAS_SHIPYARD] = buildings[5];
		m_pPlanets[DCMDB_PLANET_HAS_COMPONENTS] = buildings[28];
		m_pPlanets[DCMDB_PLANET_HAS_CUSTOMS] = buildings[22];

		m_pPlanets[DCMDB_PLANET_HAS_RESEARCH] = buildings[32];
		m_pPlanets[DCMDB_PLANET_HAS_DESIGN] = buildings[25];
		m_pPlanets[DCMDB_PLANET_HAS_PALACE] = buildings[12];

		m_pPlanets->Post();
}

bool CXMLGameProcessor::ProcessPlayer( LPXNode dc,int &turn,dcmapDBREF &PlayerRef,dcmapDBREF const &OptionPlayerRef )
{
	wgstring username = dc->GetAttrValue(L"user");
	int user_id = xml_cast<int>(dc->GetAttrValue(L"id"));
	turn = xml_cast<int>(dc->GetAttrValue(L"turn-n"));

	if(!m_pPlayers->SelectGameID(user_id))
	{
		if(!m_pPlayers->SelectWideName(username.c_str()))
		{
			m_pPlayers->Insert();
		}
	}
	m_pPlayers->Update();
	// update current player basic info & and enshure it exists

	m_pPlayers[DCMDB_PLAYER_NAME] = username;
	m_pPlayers[DCMDB_PLAYER_USER_ID] = user_id;
	m_pPlayers[DCMDB_PLAYER_TURN] = turn;

	if(!m_pPlayers->Post(&PlayerRef))return false;

	if(!OptionPlayerRef.Valid() || !m_pPlayers->SelectID(OptionPlayerRef))
		m_pPlayers->SelectID(PlayerRef);

	bManage =  m_pPlayers[DCMDB_PLAYER_MANAGE];
	bImportTrusted =  m_pPlayers[DCMDB_PLAYER_IMPORT_TRUSTED];
	bImportTrusted =  m_pPlayers[DCMDB_PLAYER_IMPORT_TRUSTED];
	bAddEmpty =  m_pPlayers[DCMDB_PLAYER_ADD_EMPTY];
	bAddOccupied =  m_pPlayers[DCMDB_PLAYER_ADD_OCCUPIED];
	bOverwriteEmpty =  m_pPlayers[DCMDB_PLAYER_OVERWRITE_EMPTY];
	bOverwriteOccupied =  m_pPlayers[DCMDB_PLAYER_OVERWRITE_OCCUPIED];
	bClearOccupied =  m_pPlayers[DCMDB_PLAYER_CLEAR_OCCUPIED];
	bImportDip =  m_pPlayers[DCMDB_PLAYER_IMPORT_DIP];
	bImportContents =  m_pPlayers[DCMDB_PLAYER_IMPORT_CONTENTS];
	bImportFleets =  m_pPlayers[DCMDB_PLAYER_IMPORT_FLEETS];
	bImportAlienFleets =  m_pPlayers[DCMDB_PLAYER_IMPORT_ALIEN_FLEETS];

	iImportLimit = 0;
	if(ImportRegion)DeleteRgn(ImportRegion);
	ImportRegion=0;

	dcmapPOINT* pData;
	int sz = m_pPlayers->GetBlob(DCMDB_PLAYER_IMPORT_REGION,(dcmapBYTE**)&pData);
	if(sz)
	{
		sz /= sizeof(dcmapPOINT);
		if(sz == 2) // setup rect
		{
			iImportLimit = 1;
			ImportRect.left = pData[0].x;
			ImportRect.top = pData[0].y;
			ImportRect.right = pData[1].x;
			ImportRect.bottom = pData[1].y;

		}
		else if(sz >2) // setup region
		{
			ImportRegion = CreatePolygonRgn(pData,sz,WINDING);
			if(ImportRegion)iImportLimit = 2;
		}
	}
	m_pPlayers->SelectID(PlayerRef);

	return true;
}


bool DCMAPAPI CXMLGameProcessor::ExecuteRecord( dcmapDCSTR Text,int id )
{
	if(!m_rec[id].r && m_rec[id].node)
	{
		for(int i=0;i<4;i++)
		{
			if(m_rec[id].dep[i]>=0)
			{
				ExecuteRecord(0,m_rec[id].dep[i]);
			}
		}
		wstring Descr = g_pLocalStrings->GetSectionStringW(ToStr(m_rec[id].sec).c_str());
		if(Descr.empty())Descr = m_rec[id].sec;
		if(m_pRsvr)
		{
			m_pRsvr->AddMessage(c_str(wformat(SSLSTRW(ImportMessageFormat,GameXMLImport))%Descr));
		}
		bool r=false;

		if(m_rec[id].func)	
			r = CALL_MEMBER_FN(*this,m_rec[id].func)(m_rec[id].node,m_rec[id].data);
		else if(m_rec[id].gfunc)	
			r = CALL_MEMBER_FN(*this,m_rec[id].gfunc)(m_rec[id].node,m_iTurn,m_PlayerRef);

		if(!r)
		{
			if(m_pRsvr)m_pRsvr->AddError(c_str(wformat(SSLSTRW(ImportErrorFormat,GameXMLImport))%Descr));
			m_iErrors++;
		}
		m_rec[id].r = 1;
	}
	return true;
}

void DCMAPAPI CXMLGameProcessor::EndExecute()
{
	if(m_iErrors==0)
	{
		if(m_pRsvr)m_pRsvr->AddMessage(SSLSTRW(ImportCompleted,GameXMLImport));
	}
	else
	{
		if(m_pRsvr)m_pRsvr->AddMessage(SSLSTRW(ImportErrorCompleted,GameXMLImport));
	}
	m_pCore->UnlockDataUpdate();
}

bool DCMAPAPI CXMLGameProcessor::BeginExecute()
{
	m_pCore->LockDataUpdate();
	LPXNode dc = m_XML.GetChild(L"dc");
	if(dc)
	{
		ProcessPlayer(dc,m_iTurn,m_PlayerRef,m_OprionsPlayerRef);
	}
	m_iErrors=0;
	return true;
}

void DCMAPAPI CXMLGameProcessor::ClearProcessData()
{
	for(int i=0;i<GameRecords;i++)
	{
		m_rec[i].r=0;
		m_rec[i].node=0;
	}
	m_XML.Close();
}

int CXMLGameProcessor::ProcessBase( LPXNode dc )
{
	int err = CheckGameXMLErrors(dc);
	if(err != GAMEXML_OK) return err;

	dcmapWCSTR username = dc->GetAttrValue(L"user");
	int user_id = xml_cast<int>(dc->GetAttrValue(L"id"));
	int turn = xml_cast<int>(dc->GetAttrValue(L"turn-n"));

	if(!user_id || !username || !turn)return 0;

	return 1;
}

bool DCMAPAPI CXMLGameProcessor::PreProcessData( dcmapDCSTR szInputData,int iLines, IDCMapProcessRecordReseiver* pRsvr, dcmapDWORD options )
{
	ClearRecords();

	LSTRSECTION(GameXMLImport);
	m_pRsvr = pRsvr;
	m_XML.Close();
	if(!m_XML.Load(szInputData))
	{
		if(pRsvr)
		{
			pRsvr->AddError(SLSTRW(XMLParceError));				
			pRsvr->AddError(m_XML.parse_info.error_string.c_str());	
			pRsvr->SetErrorCode(-1);
		}
		return false;
	}

	LPXNode dc = m_XML.GetChild(L"dc");
	if(dc)
	{
		int r = ProcessBase(dc);
		if(r != 1)
		{
			if(pRsvr)
			{
				pRsvr->SetErrorCode(r);
				pRsvr->AddError(SLSTRW(WrongDataError));
			}
			return false;
		}
		XNode* frame = dc->GetChild(L"iframe");
		if(frame)
		{
			for(int i=0;i<GameRecords;i++)
			{
				m_rec[i].r=0;
				LPXNode node = frame->GetChild(m_rec[i].sec.c_str());
				if(node)
				{
					m_rec[i].node = node;
					dcmapWCSTR Descr = g_pLocalStrings->GetSectionStringW(ToStr(m_rec[i].sec).c_str());
					if(!Descr[0])Descr = m_rec[i].sec.c_str();
					if(pRsvr)pRsvr->AddRecord(Descr,i);
				}
			}
		}
		return true;
	}

	LPXNode components = m_XML.GetChild(L"components");
	if(components)
	{
		int i = EProcessComponents;
		m_rec[i].node = components;
		dcmapWCSTR Descr = g_pLocalStrings->GetSectionStringW(ToStr(m_rec[i].sec).c_str());
		if(!Descr[0])Descr = m_rec[i].sec.c_str();
		if(pRsvr)pRsvr->AddRecord(Descr,i);
		return true;
	}

	LPXNode chassiss = m_XML.GetChild(L"chassiss");
	if(chassiss)
	{
		int i = EProcessChassiss;
		m_rec[i].node = chassiss;
		dcmapWCSTR Descr = g_pLocalStrings->GetSectionStringW(ToStr(m_rec[i].sec).c_str());
		if(!Descr[0])Descr = m_rec[i].sec.c_str();
		if(pRsvr)pRsvr->AddRecord(Descr,i);
		return true;
	}	

	LPXNode dcmap = m_XML.GetChild(L"dcmap");
	if(dcmap)
	{
		LPXNode data = dcmap->GetChild(L"data");
		if(data)
		{
			// process 'data' style basic data 
			PreProcessDCMapData(data);
		}
		return true;
	}

	if(pRsvr)pRsvr->AddError(SLSTRW(WrongDataError));	
	pRsvr->SetErrorCode(-1);
	return false;
}

bool DCMAPAPI CXMLGameProcessor::PreProcessDCMapData(LPXNode data)
{
	int n = data->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode ntable = data->GetChild(i);
		IDCMapDataStorage* stor = m_pCore->GetDataSourceTyped<IDCMapDataStorage>(ToStr(ntable->name).c_str());
		if(!stor)continue;

		process_rec rec;
		rec.sec = ToWide(stor->Name());
		rec.func = &CXMLGameProcessor::ProcessDataTable;
		rec.node = ntable;
		rec.data.table = stor;

		if(m_pRsvr)m_pRsvr->AddRecord(ToWide(stor->Name()).c_str(),m_rec.size());
		m_rec.push_back(rec);
	}
	return true;
}

enum BuildingsIds
{
	bld_mines=1,
	bld_biofarms=2,
	bld_powerplants=3,

	bld_factories=14,
	bld_compressors=31,

	bld_governors=13,
	bld_barracks=4,
	bld_shipyard=5,
	bld_components=28,
	bld_customs=22,
	bld_stasis=31,

	bld_research=32,
	bld_design=25,
	bld_palace=12,
};


enum BldMask
{
	bmaskSpace  = (1<<0),
	bmaskGround = (1<<1),
	bmaskPalace = (1<<2),
	bmaskCustoms = (1<<3),
	bmaskComponents = (1<<4),
	bmaskDesign = (1<<5),
};

enum BldUnique
{
	uniquePalace  = (1<<0),
	uniqueBarracks  = (1<<1),
	uniqueShipyard  = (1<<2),
	uniqueComponents  = (1<<3),
	uniqueCustoms  = (1<<4),
	uniqueResearch  = (1<<5),
	uniqueDesign  = (1<<6),
};


void CXMLGameProcessor::UpdateComponentBuildInd(dcmapDBREF ref)
{
	if(!m_pComponents->SelectID(ref))return;
	int carapace = m_pComponents[DCMDB_COMPONENT_GAMEID];
	int id = carapace & ~DCMDB_COMPONENT_BUILDING_FLAG;

	int req=0;
	int unique=0;
	int allow=0;

	switch(id)
	{
	case bld_palace:
		allow = bmaskPalace;
		unique = uniquePalace;
		break;
	case bld_governors:
		req = bmaskPalace;
		break;
	case bld_shipyard:
		allow = bmaskSpace;
		unique = uniqueShipyard;
		break;
	case bld_barracks:
		allow = bmaskGround;
		unique = uniqueBarracks;
		break;
	case bld_factories:
		req = bmaskGround;
		break;
	case bld_design:
		req = bmaskSpace;
		break;
	case bld_components:
		req = bmaskDesign;
		break;	
	case bld_stasis:
		req = bmaskCustoms;
		break;	
	//default:
		//if(m_pProjects[DCMDB_PROJECT_IS_SPACE_SHIP])
		//	req = bmaskSpace;
		//else if(m_pProjects[DCMDB_PROJECT_IS_GROUND_UNIT])
		//	req = bmaskGround;
	}

	if( carapace == 108) // Self defence
		req=0;

	m_pComponents->Update();
	m_pComponents[DCMDB_COMPONENT_BUILD_UNIQUE] = unique;
	m_pComponents[DCMDB_COMPONENT_BUILD_REQ] = req;
	m_pComponents[DCMDB_COMPONENT_BUILD_ALLOW] = allow;
	m_pComponents->Post();
};

bool CXMLGameProcessor::ProcessComponentData(LPXNode comp)
{
	int id = xml_cast<int>(comp->GetChildValue(L"id"));
	dcmapWCSTR comp_name = comp->GetChildValue(L"name");

	if(!id)return false;

	if(!comp_name || !comp_name[0])
	{
		string sec = "cp"+gsys::to_str(id);
		comp_name = g_pLocalStrings->GetSectionStringW(sec.c_str());
	}

	if(!m_pComponents->SelectGameID(id))
	{
		m_pComponents->Insert();
		m_pComponents[DCMDB_COMPONENT_GAMEID] = id;
	}
	else m_pComponents->Update();

	if(comp_name[0])
		m_pComponents[DCMDB_COMPONENT_NAME] = comp_name;
	else
		m_pComponents[DCMDB_COMPONENT_NAME] = "cp"+gsys::to_str(id);
					
	m_pComponents->Post();
	return true;
}

bool CXMLGameProcessor::ProcessComponents(LPXNode node,process_rec_data const& rec)
{
	if(!node)return false;
	g_pLocalStrings->SelectSection("Buildings");

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode comp = node->GetChild(i);
		if(wcscmp(comp->name.c_str(),L"component")!=0)continue;
		ProcessComponentData(comp);
	}
	return true;
}

bool CXMLGameProcessor::ProcessChassiss(LPXNode node,process_rec_data const& rec)
{
	if(!node)return false;
	g_pLocalStrings->SelectSection("Buildings");

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode comp = node->GetChild(i);
		if(wcscmp(comp->name.c_str(),L"chassis")!=0)continue;
		ProcessComponentData(comp);
	}
	return true;
}

bool CXMLGameProcessor::ProcessActsReq(LPXNode node,process_rec_data const& rec)
{
	if(!node)return false;

	std::set<dcmapDBREF> ToDelete;
	if(m_pActsReq->SelectAll())
	do
	{
		if(m_pActsReq[DCMDB_ACTREQ_LOADER] == m_PlayerRef)
			ToDelete.insert(*m_pActsReq->GetID());
	}
	while(m_pActsReq->Next());

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode act = node->GetChild(i);
		if(wcscmp(act->name.c_str(),L"act")!=0)continue;
		int id = xml_cast<int>(act->GetAttrValue(L"id"));
		int unitid = xml_cast<int>(act->GetAttrValue(L"u"));
		int fleetid = xml_cast<int>(act->GetAttrValue(L"fleet"));
		int x = xml_cast<int>(act->GetAttrValue(L"x"));
		int y = xml_cast<int>(act->GetAttrValue(L"y"));
		int actid = xml_cast<int>(act->GetAttrValue(L"act"));
		if(!id)continue;
		if(!actid)continue;
		if(!unitid)continue;

		if(!m_pUnits->SelectGameID(unitid))continue;

		dcmapDBREF actRef;
		if(m_pProjectActs->SelectByParentID(m_pUnits[DCMDB_BUILDING_PROJECT]))
		do
		{
			DCMapProjectActInfo const* inf = m_pProjectActs->GetData();
			if(inf->actid == actid)
			{
				actRef = inf->id;
				break;
			}
		}
		while(m_pProjectActs->Next());
		if(!actRef.Valid())continue;

		if(m_pActsReq->SelectGameID(id))
		{
			m_pActsReq->Update();
		}
		else
		{
			m_pActsReq->Insert();
			m_pActsReq[DCMDB_ACTREQ_GAMEID] = id;
		}
		m_pActsReq[DCMDB_ACTREQ_UNIT] = m_pUnits->GetID();

		if(!m_pFleets->SelectGameID(fleetid))
		{
			m_pActsReq[DCMDB_ACTREQ_FLEET] = m_pFleets->GetID();
		}
		m_pActsReq[DCMDB_ACTREQ_ACTID] = actid;
		m_pActsReq[DCMDB_ACTREQ_TURN] = m_iTurn;
		m_pActsReq[DCMDB_ACTREQ_ACTION] = actRef;
		m_pActsReq[DCMDB_ACTREQ_LOADER] = m_PlayerRef;
		m_pActsReq[DCMDB_ACTREQ_CANCELED] = false;

		dcmapDBREF refAct;
		m_pActsReq->Post(&refAct);
		ToDelete.erase(refAct);
	}
	_foreach(std::set<dcmapDBREF>,ToDelete,it)
	{
		if(m_pActsReq->SelectID(*it))m_pActsReq->Delete();
	}
	return true;
}

bool CXMLGameProcessor::ProcessDataTable(LPXNode node,process_rec_data const& rec)
{
	if(!node)return false;
	if(!rec.table)return false;

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode records = node->GetChild(i);
		int nrec = records->GetChildCount();	

		rec.table->EditIndex(); 
		for(int j=0;j<nrec;j++)
		{
			LPXNode rc = records->GetChild(j);
			int id = rec.table->ColumnID(ToStr(rc->name).c_str());
			if(id < 0)continue;
			int cparam = rec.table->ColumnParams(id); 
			if(cparam & DCMAP_COLUMN_KEY)
			{
				rec.table->SetWideString(id,rc->value.c_str());
			}
		}	
		if(rec.table->SelectIndex()) rec.table->Update();
		else rec.table->Insert();

		for(int j=0;j<nrec;j++)
		{
			LPXNode rc = records->GetChild(j);
			int id = rec.table->ColumnID(ToStr(rc->name).c_str());
			if(id < 0)continue;
			rec.table->SetWideString(id,rc->value.c_str());
		}
		rec.table->Post();
	}
	return true;
}

bool CXMLGameProcessor::ProcessCargoMoves( LPXNode node,process_rec_data const& rec )
{
	if(!node)return true;

	int n = node->GetChildCount();
	for(int i=0;i<n;i++)
	{
		LPXNode rel = node->GetChild(i);
		if(wcscmp(rel->name.c_str(),L"rel")!=0)continue;

		int unit = xml_cast<int>(rel->GetAttrValue(L"u"));
		int fleet = xml_cast<int>(rel->GetAttrValue(L"fleet"));

		if(!m_pUnits->SelectGameID(unit))continue;
		if(!m_pFleets->SelectGameID(fleet))continue;

		if(!m_pUnitTags->SelectByParentID(*m_pUnits->GetID()))continue;

		m_pUnitTags->Update();
		m_pUnitTags[DCMDB_UNITTAG_LOADING] = true;
		m_pUnitTags[DCMDB_UNITTAG_LOAD_FLEET] = m_pFleets->GetID();
		m_pUnitTags->Post();

	}

	return true;
}