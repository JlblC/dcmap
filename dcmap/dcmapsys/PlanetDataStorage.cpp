#include "stdafx.h"
#include "PlanetDataStorage.h"
#include "geostorage.h"
#include <g_alg.h>
#include <g_vector2.h>

#include "../dcmapsys.h"

/*
class CPlanetPlayerDataStorage : public CBaseChildTableDataStorage<DCMapPlanetPlayerInfo,IDCMapPlanetPlayerDataStorage,CPlanetPlayerDataStorage>
{
	typedef CBaseChildTableDataStorage<DCMapPlanetPlayerInfo,IDCMapPlanetPlayerDataStorage,CPlanetPlayerDataStorage> Inherited;
public:
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CPlanetPlayerDataStorage();}

	CPlanetPlayerDataStorage():Inherited("planets","planet")
	{
		m_BR.RegisterReference("player",&value_type::player,"players",DCMDB_PLANETPLAYER_PLAYER);
		m_BR.RegisterInt("turn",&value_type::turn,DCMDB_PLANETPLAYER_TURN);
		m_BR.RegisterFlag("open",&value_type::flags,DCMAP_PPF_Open,DCMDB_PLANETPLAYER_OPEN);
	}

	void DCMAPAPI FillDefaults(value_type* pinf)
	{
		memset(pinf,0,sizeof(value_type));
	}

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapDataStorage);
		DCMAP_MODULE_SUPPORTS(IDCMapPlanetPlayerDataStorage);
		DCMAP_MODULE_SUPPORTS(CPlanetPlayerDataStorage);
		return 0;
	}

	bool DCMAPAPI SelectPlanetPlayer(dcmapDBREF const& planet,
		dcmapDBREF const& player)
	{
		return false;
	}

	dcmapCSTR DCMAPAPI Name(){return "planetplayer";}
};
//DCMAP_REGISTER_STORAGE(CPlanetPlayerDataStorage,"planetplayer");
*/


DCMAP_REGISTER_STORAGE(CPlanetDataStorage,"planets");

DCMAP_REGISTER(CPlanetDataStorage,"PPlanetStorage","RawData");
DCMAP_REGISTER(CPlanetDataStorage,"PPlanetSource","RawData");


CPlanetDataStorage::CPlanetDataStorage(void)
{
	m_pGeo=0;
	m_bLineMode = false;

	m_BR.RegisterReference("owner",&value_type::owner,"players",DCMDB_PLANET_OWNER);
	m_BR.RegisterReference("loader",&value_type::loader,"players",DCMDB_PLANET_LOADER);
	m_BR.RegisterReference("icon1",&value_type::icon1,"icons",DCMDB_PLANET_ICON1);
	m_BR.RegisterReference("icon2",&value_type::icon2,"icons",DCMDB_PLANET_ICON2);
	//m_BR.RegisterReference("garrison",&value_type::garrison,"fleets",DCMDB_PLANET_GARRISON);

    m_BR.RegisterReference("garrison",&value_type::garrison,"fleets",DCMDB_PLANET_GARRISON);
	m_BR.RegisterReference("buildings",&value_type::buildings,"fleets",DCMDB_PLANET_BUILDINGS);
	m_BR.RegisterReference("queue",&value_type::queue,"fleets",DCMDB_PLANET_QUEUE);

    m_BR.RegisterReference("fleets",&value_type::fleets,"fleets",DCMDB_PLANET_FLEETS);

	m_BR.RegisterInt("x",&value_type::x,DCMDB_PLANET_X);
	m_BR.RegisterInt("y",&value_type::y,DCMDB_PLANET_Y);
	m_BR.RegisterInt("o",&value_type::o,DCMDB_PLANET_O);
	m_BR.RegisterInt("e",&value_type::e,DCMDB_PLANET_E);
	m_BR.RegisterInt("m",&value_type::m,DCMDB_PLANET_M);
	m_BR.RegisterInt("t",&value_type::t,DCMDB_PLANET_T);
	m_BR.RegisterInt("s",&value_type::s,DCMDB_PLANET_S);

	m_BR.RegisterWideString("caption",&value_type::caption,DCMDB_PLANET_CAPTION);

	m_BR.RegisterInt("color1",&value_type::color1,DCMDB_PLANET_COLOR1);
	m_BR.RegisterInt("color2",&value_type::color2,DCMDB_PLANET_COLOR2);

	m_BR.RegisterInt("turn",&value_type::turn,DCMDB_PLANET_TURN);

	m_BR.RegisterInt("mines",&value_type::mines,DCMDB_PLANET_HAS_MINES);
	m_BR.RegisterInt("biofarms",&value_type::biofarms,DCMDB_PLANET_HAS_BIOFARMS);
	m_BR.RegisterInt("powerplants",&value_type::powerplants,DCMDB_PLANET_HAS_POWERPLANTS);
	m_BR.RegisterInt("factories",&value_type::factories,DCMDB_PLANET_HAS_FACTORIES);
	m_BR.RegisterInt("compressors",&value_type::compressors,DCMDB_PLANET_HAS_COMPRESSORS);
	m_BR.RegisterInt("governors",&value_type::governors,DCMDB_PLANET_HAS_GOVERNORS);

	m_BR.RegisterFlag("barracks",&value_type::Flags,DCMAP_PF_Barracks,DCMDB_PLANET_HAS_BARRACKS);
	m_BR.RegisterFlag("shipyard",&value_type::Flags,DCMAP_PF_Shipyard,DCMDB_PLANET_HAS_SHIPYARD);
	m_BR.RegisterFlag("components",&value_type::Flags,DCMAP_PF_Components,DCMDB_PLANET_HAS_COMPONENTS);
	m_BR.RegisterFlag("customs",&value_type::Flags,DCMAP_PF_Customs,DCMDB_PLANET_HAS_CUSTOMS);
	m_BR.RegisterFlag("research",&value_type::Flags,DCMAP_PF_Research,DCMDB_PLANET_HAS_RESEARCH);
	m_BR.RegisterFlag("design",&value_type::Flags,DCMAP_PF_Design,DCMDB_PLANET_HAS_DESIGN);
	m_BR.RegisterFlag("palace",&value_type::Flags,DCMAP_PF_Palace,DCMDB_PLANET_HAS_PALACE);

	m_BR.RegisterFlag("industrial",&value_type::Flags,DCMAP_PF_Industrial,DCMDB_PLANET_INDUSTRIAL);
	m_BR.RegisterFlag("bounds",&value_type::Flags,DCMAP_PF_Bounds,DCMDB_PLANET_BOUNDS);
	m_BR.RegisterFlag("homeworld",&value_type::Flags,DCMAP_PF_Homeworld,DCMDB_PLANET_HW);
	m_BR.RegisterFlag("unsafe",&value_type::Flags,DCMAP_PF_Unsafe,DCMDB_PLANET_UNSAFE);
	m_BR.RegisterFlag("hidden",&value_type::Flags,DCMAP_PF_Hidden,DCMDB_PLANET_HIDDEN);
	m_BR.RegisterFlag("my",&value_type::Flags,DCMAP_PF_My,DCMDB_PLANET_MY);

	m_BR.RegisterFlag("planning",&value_type::Flags,DCMAP_PF_Planning,DCMDB_PLANET_BUILD_PLANNING);

	m_BR.RegisterFlag("keep_name",&value_type::Flags,DCMAP_PF_KeepName,DCMDB_PLANET_KEEP_NAME);
	m_BR.RegisterFlag("synchronize_name",&value_type::Flags,DCMAP_PF_SynchronizeName,DCMDB_PLANET_SYNCHRONIZE_NAME);

	m_BR.RegisterInt("corruption",&value_type::corruption,DCMDB_PLANET_CORRUPTION);

	m_BR.RegisterInt("population",&value_type::population,DCMDB_PLANET_POPULATION);

	m_BR.RegisterInt("infiltration",&value_type::infiltration,DCMDB_PLANET_INFILTRATION);


	m_BR.SetParam(DCMAP_COLUMN_INDEX,DCMDB_PLANET_X);
	m_BR.SetParam(DCMAP_COLUMN_INDEX,DCMDB_PLANET_Y);

	m_BR.Alias("name",DCMDB_PLANET_CAPTION);
	m_BR.Alias("PlanetName",DCMDB_PLANET_CAPTION);
	m_BR.Alias("Planet",DCMDB_PLANET_CAPTION);

	m_BR.Alias("Player",DCMDB_PLANET_OWNER);
	m_BR.Alias("User",DCMDB_PLANET_OWNER);
	m_BR.Alias("PlayerName",DCMDB_PLANET_OWNER);
	m_BR.Alias("UserName",DCMDB_PLANET_OWNER);
	m_BR.Alias("hw",DCMDB_PLANET_HW);

	m_BR.Alias("icon",DCMDB_PLANET_ICON1);
}

CPlanetDataStorage::~CPlanetDataStorage(void)
{
}

bool DCMAPAPI CPlanetDataStorage::SelectLine(int LineNum,int From,int To)
{_CFN
	if(!DataLoad())return false;

	//_ASSERT(CheckSanity());
	m_pCurrentRecord=0;
	int line = LineNum-1;
	if(line < 0 || line >= g_DCMapNumPlanets)return false;
	_ASSERTREF(m_Index[line]);

	//m_Index[line].clear();
	//return false;

	if(From <= 1)m_itCurrent = m_Index[line].begin();
	else m_itCurrent = m_Index[line].lower_bound(From);

	if(To >= g_DCMapNumPlanets) m_itEnd = m_Index[line].end();
	else m_itEnd = m_Index[line].upper_bound(To);

	if(m_itCurrent == m_itEnd) return false;

	m_pCurrentRecord = &m_lstData[m_itCurrent->second];
	++m_itCurrent;

	m_bLineMode=true;
	return true;
}

void DCMAPAPI CPlanetDataStorage::PostInit()
{
	m_pGeo = m_pDatabase->GetDataSourceTyped<CGeoStorage>("geo");

	base_storage::PostInit();
}

int DCMAPAPI CPlanetDataStorage::OnMessage(int Msg,dcmapDWORD wParam,
										   dcmapDWORD lParam,IDCMapObject* pSender)
{
	return base_storage::OnMessage(Msg,wParam,lParam,pSender);
}


bool DCMAPAPI CPlanetDataStorage::SelectCoord(int x,int y)
{
	if(!DataLoad())return false;

	int line = y-1;
	if(line < 0 || line >= g_DCMapNumPlanets)return false;
	if(x < 1 || x > g_DCMapNumPlanets)return false;

	m_pCurrentRecord = GetDataByCoords(x,y);
	return m_pCurrentRecord != 0;
}

bool DCMAPAPI CPlanetDataStorage::Next()
{
	if(!m_bLineMode) return base_storage::Next();
	m_pCurrentRecord=0;
	if(m_itCurrent == m_itEnd)
	{
		m_bLineMode = false;
		return false;
	}
	m_pCurrentRecord = &m_lstData[m_itCurrent->second];
	++m_itCurrent;
	return true;
}

bool CPlanetDataStorage::NeedReindex(value_type const* old,value_type const* newd)
{
	if(old->x != newd->x || old->y != newd->y)return true;
	return Inherited::NeedReindex(old,newd);
}

bool CPlanetDataStorage::IndexValue(const value_type* pDta)
{_CFN;
	int line = pDta->y-1;
	if(line<0 || line >= g_DCMapNumPlanets)return false;
	int x = pDta->x;

	MapXCoordIndex::iterator it = m_Index[line].find(x);
	if(it != m_Index[line].end())return false;
	m_Index[line][x] = pDta->id.id;
	return true;
}

void CPlanetDataStorage::UnIndexValue(const value_type* pDta)
{
	int line = pDta->y-1;
	if(line<0 || line >= g_DCMapNumPlanets)return;
	m_Index[line].erase(pDta->x);
}

void CPlanetDataStorage::ClearIndex()
{
	for(int i=0;i<g_DCMapNumPlanets;i++) // Clear index
	{
		m_Index[i].clear();
	}
}

void DCMAPAPI CPlanetDataStorage::FillDefaults(DCMapPlanetInfo* pinf)
{
	memset(pinf,0,sizeof(DCMapPlanetInfo));
	pinf->owner.Invalidate();
	pinf->color1 = dcmapRGB(185,185,185);
	pinf->color2 = dcmapRGB(0,0,0);
	pinf->Flags |= DCMAP_PF_SynchronizeName;
}

bool DCMAPAPI CPlanetDataStorage::SelectName(const char* szName)
{
	if(!DataLoad())return false;
	dcmapPOINT pt;
	if(StrToCoords(szName,pt)) return SelectCoord(pt.x,pt.y);
	return false;
}

bool DCMAPAPI CPlanetDataStorage::SelectWideName(dcmapDCSTR szName)
{
	if(!DataLoad())return false;
	dcmapPOINT pt;
	if(StrToCoords(StrD2C(szName).c_str(),pt)) return SelectCoord(pt.x,pt.y);
	return false;
}

bool DCMAPAPI CPlanetDataStorage::RetrieveByName(const char* szName,
												 dcmapDBREF* pRef)
{

	if(!DataLoad())return false;
	dcmapPOINT pt;
	if(StrToCoords(szName,pt)) return RetrieveByCoords(pt.x,pt.y,pRef);
	return false;
}
dcmapCSTR DCMAPAPI CPlanetDataStorage::GetName()
{
	static string ss;
	ss = StrD2C(GetWideName());
	return ss.c_str();
}


void DCMAPAPI CPlanetDataStorage::SetName(dcmapCSTR szName)
{
	if(szName)
		SetWideName(ToDStr(szName).c_str());
}

void DCMAPAPI CPlanetDataStorage::SetWideName(dcmapDCSTR szName)
{
	if(szName)
		dcsncpy(m_temp.caption,szName,DCMapPlanetCaptionLen);
	m_temp.caption[DCMapPlanetCaptionLen-1]=0;
}
dcmapUCSTR DCMAPAPI CPlanetDataStorage::GetDisplayName()
{
	static ustring DName;
	if(m_pCurrentRecord->caption[0])
		DName = dcmapUT("[")+ToUStr(m_pCurrentRecord->x)+dcmapUT(":")+ToUStr(m_pCurrentRecord->y)+dcmapUT("] ")+
				 ToUStr(m_pCurrentRecord->caption);
	else
		DName = dcmapUT("[")+ToUStr(m_pCurrentRecord->x)+dcmapUT(":")+ToUStr(m_pCurrentRecord->y)+dcmapUT("]");
	return DName.c_str();
}

dcmapDCSTR DCMAPAPI CPlanetDataStorage::GetWideName()
{
	m_strCurName = ToDStr(ToWStr(m_pCurrentRecord->x)+dcmapWT(":")+ToWStr(m_pCurrentRecord->y));
	return m_strCurName.c_str();
}

bool DCMAPAPI CPlanetDataStorage::RetrieveByCoords(int x,int y,dcmapDBREF* pRef)
{
	if(SelectCoord(x,y))
	{
		if(pRef)*pRef = GetData()->id;
	}
	else
	{
		value_type val;
		FillDefaults(&val);
		val.x = x;
		val.y = y;
		InsertData(&val,pRef);
	}
	return true;
}

void DCMAPAPI CPlanetDataStorage::SetCoords(int x,int y)
{
	m_temp.x=x;
	m_temp.y=y;
}

bool DCMAPAPI CPlanetDataStorage::SelectIndex()
{
	if(!m_bEditIndex)return false;
	m_bEditIndex = false;
	return SelectCoord(m_temp.x,m_temp.y);
}

void CPlanetDataStorage::OnChange()
{
	if(m_pCurrentRecord)
	{
		DCMapPlanetGeoInfo& geo = m_pGeo->m_data(m_pCurrentRecord->x-1,m_pCurrentRecord->y-1);
		if(m_pCurrentRecord->s > 0) // update geo information
		{
			if(geo.s == 0) m_pGeo->m_NumRecords++;
			if(!(geo == m_pCurrentRecord->geo))
			{
				geo = m_pCurrentRecord->geo;
				m_pGeo->m_modIndex++;
			}
		}
		else
		{
			m_pCurrentRecord->geo = geo;
		}
	}
}

void DCMAPAPI CPlanetDataStorage::SetBaseData( const DCMapPlanetParamInfo* data )
{
	_ASSERT(data);
	*((DCMapPlanetParamInfo*)&m_temp) = *data;
}

void DCMAPAPI CPlanetDataStorage::SetGeoData( const DCMapPlanetGeoInfo* data )
{
	_ASSERT(data);
	m_temp.geo = *data;
}
//////////////////////////////////////////////////////////////////////////

DCMAP_REGISTER_STORAGEX(CGroupsDataStorage,"groups",":Hidden:");

CGroupsDataStorage::CGroupsDataStorage()
{
	m_BR.RegisterReference("player",&value_type::player,"players",DCMDB_GROUP_PLAYER);
	m_BR.RegisterString("name",&value_type::name,DCMDB_GROUP_NAME);
	m_BR.RegisterInt("type",&value_type::type,DCMDB_GROUP_TYPE);
}

void DCMAPAPI CGroupsDataStorage::FillDefaults( value_type* pinf )
{
	memset(pinf,0,sizeof(value_type));
}

void* DCMAPAPI CGroupsDataStorage::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(IDCMapDataStorage);
	DCMAP_MODULE_SUPPORTS(IDCMapGroupsDataStorage);
	DCMAP_MODULE_SUPPORTS(CGroupsDataStorage);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
DCMAP_REGISTER_STORAGEX(CPlanetsGroupsDataStorage,"planetsgroups",":Hidden:");

CPlanetsGroupsDataStorage::CPlanetsGroupsDataStorage()
{
	m_strParent1Table = "groups";
	m_strParent2Table = "planets";

	m_BR.RegisterReference("group",&value_type::c1,"groups",DCMDB_PLANETSGROUP_GROUP);
	m_BR.RegisterReference("planet",&value_type::c2,"planets",DCMDB_PLANETSGROUP_PLANET);
	m_BR.RegisterInt("turn",&value_type::turn,DCMDB_PLANETSGROUP_TURN);
}

void DCMAPAPI CPlanetsGroupsDataStorage::FillDefaults( value_type* pinf )
{
	memset(pinf,0,sizeof(value_type));
}

void* DCMAPAPI CPlanetsGroupsDataStorage::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(IDCMapDataStorage);
	DCMAP_MODULE_SUPPORTS(IDCMapPlanetsGroupsDataStorage);
	DCMAP_MODULE_SUPPORTS(CPlanetsGroupsDataStorage);
	return 0;
}