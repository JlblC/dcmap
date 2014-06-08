#pragma once
#include "../dcmapsys.h"

#include "BaseDataStorage.h"

class CGeoStorage;

class CPlanetDataStorage : public CBaseDataStorage<DCMapPlanetInfo,IDCMapPlanetDataStorage,CPlanetDataStorage>
{
	typedef CBaseDataStorage<DCMapPlanetInfo,IDCMapPlanetDataStorage,CPlanetDataStorage> Inherited;

	friend class CGeoStorage;

	CGeoStorage* m_pGeo;

	typedef std::multimap<dcmapDBREF,DataList::size_type> MapPlanetOwner;

	typedef std::map<short,DataList::size_type> MapXCoordIndex;
	MapXCoordIndex m_Index[DCMapNumPlanets];

	MapXCoordIndex::iterator m_itCurrent;
	MapXCoordIndex::iterator m_itEnd;

	dstring m_strCurName;

protected:
	value_type* GetDataByCoords(int x,int y)
	{
		int line = y-1;
		MapXCoordIndex::iterator it = m_Index[line].find(x);
		if(it == m_Index[line].end())return 0;
		return &m_lstData[it->second];;
	}
public:
	CPlanetDataStorage(void);
	bool m_bLineMode;
public:
	void DCMAPAPI Destroy(){delete this;};
	void OnChange();
public:
	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);

	virtual ~CPlanetDataStorage(void);

	bool DCMAPAPI SelectLine(int LineNum,int From,int To);
	bool DCMAPAPI SelectCoord(int x,int y);

	bool DCMAPAPI Next();

	bool IndexValue(const value_type* pDta);
	void UnIndexValue(const value_type* pDta);
	void ClearIndex();

	void DCMAPAPI FillDefaults(DCMapPlanetInfo* pInf);

	bool DCMAPAPI RetrieveByCoords(int x,int y,dcmapDBREF* pRef);

	bool DCMAPAPI SelectName(const char* szName);
	bool DCMAPAPI SelectWideName(dcmapDCSTR szName);
	bool DCMAPAPI RetrieveByName(const char* szName,dcmapDBREF* pRef=0);

	const char* DCMAPAPI GetName();
	dcmapDCSTR DCMAPAPI GetWideName();
	dcmapUCSTR DCMAPAPI GetDisplayName();

	void DCMAPAPI SetName(dcmapCSTR szName);
	void DCMAPAPI SetWideName(dcmapDCSTR szName);

	void DCMAPAPI SetCoords(int x,int y);

	bool DCMAPAPI SelectIndex();

	char const* DCMAPAPI Name(){return "planets";}

	void DCMAPAPI SetBaseData(const DCMapPlanetParamInfo* data);
	void DCMAPAPI SetGeoData(const DCMapPlanetGeoInfo* data);
	bool NeedReindex(value_type const* old,value_type const* newd);
	void DCMAPAPI PostInit();
};
DCMAP_SPEC_INTERFACE(CPlanetDataStorage);
//////////////////////////////////////////////////////////////////////////

class CGroupsDataStorage : public CBaseDataStorage<DCMapGroupsInfo,IDCMapGroupsDataStorage,CGroupsDataStorage>
{
public:
	char const* DCMAPAPI Name(){return "groups";}
	CGroupsDataStorage();
	void DCMAPAPI FillDefaults(value_type* pinf);
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
};
DCMAP_SPEC_INTERFACE(CGroupsDataStorage);

//////////////////////////////////////////////////////////////////////////

class CPlanetsGroupsDataStorage : public CBaseConnectionTableDataStorage<DCMapPlanetsGroupsInfo,IDCMapPlanetsGroupsDataStorage,CPlanetsGroupsDataStorage>
{
public:
	char const* DCMAPAPI Name(){return "planetsgroups";}
	CPlanetsGroupsDataStorage();
	void DCMAPAPI FillDefaults(value_type* pinf);
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
};
//DCMAP_SPEC_INTERFACE(CPlanetsGroupsDataStorage);