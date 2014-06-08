#pragma once
#include <boost/pool/object_pool.hpp>
#include "../dcmapsys.h"

class CPlanetDataProcessor :
	public IDCMapPlanetDataProcessor
{
	class data_pool : public boost::pool<>
	{
	public:
		explicit data_pool():boost::pool<>(sizeof(DCMapPlanetDisplayInfo)){}
		DCMapPlanetDisplayInfo* malloc(){return (DCMapPlanetDisplayInfo*)boost::pool<>::malloc();}
	}
	m_PlanetDataPool;
protected:
	IDCMapWorkspace* m_pWorkspace;
	int m_iShadeMode;

	DCMapPlanetDisplayInfo* m_pCurrent;

	typedef std::map<short,DCMapPlanetDisplayInfo*> MapXCoordIndex;
	typedef MapXCoordIndex::value_type MapXCoordIndexValue;

	struct Line
	{
		MapXCoordIndex	Index;
		int Min;
		int Max;
	};
	Line m_Lines[DCMapNumPlanets];

	MapXCoordIndex::iterator m_itCurrent;
	MapXCoordIndex::iterator m_itEnd;
	MapXCoordIndex::iterator m_itFinal;

	IDCMapPlanetDataStorage* m_pFeeder;
	dcmapQWORD m_PlanetsState;

	DCMapPlanetDisplayInfo* NewData();
	void DeleteData(DCMapPlanetDisplayInfo* dta);
	void IndexData(DCMapPlanetDisplayInfo* dta);
	void ResetAlloc(int prealloc);

	void UnindexData(DCMapPlanetDisplayInfo* dta);

	void ProcessLine(int ln, int From,int To);
	void Actualize(int ln,int From,int To);

	void ClearData();

	void GetData();

	bool ProcessData(DCMapPlanetDisplayInfo* dta,DCMapPlanetInfo const* pinf);

	dcmapDBREF m_refSelection;
	dcmapPOINT m_ptSelection;

	DCMapPlanetDisplayInfo* DCMAPAPI FindInfo(int x, int y);

	bool Reprocess(int x,int y);

	typedef std::vector<IDCMapPlanetDataFilter*> VecFilters;

	VecFilters m_vecFilters;

	dcmapCOLORREF m_clSelection;

	void LoadPrefs(void);

	bool DCMAPAPI SelectAll();
	int DCMAPAPI NumRecords();

public:
	CPlanetDataProcessor(void);
public:
	virtual ~CPlanetDataProcessor(void);
public:
	void* DCMAPAPI CastTo(DCMapInterfaceID szInterface);
	void DCMAPAPI Destroy(){delete this;};

	bool DCMAPAPI SelectLine(int LineNum,int From,int To);

	DCMapPlanetDisplayInfo* DCMAPAPI GetPlanet(int x, int y);

	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	int  DCMAPAPI OnMessage(int MSG,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);
	void  DCMAPAPI LoadConfig(IDCMapConfig* pConfig);
	void  DCMAPAPI SaveConfig(IDCMapConfig* pConfig);

	bool DCMAPAPI Next();
	DCMapPlanetDisplayInfo* DCMAPAPI GetPlanetInfo();
	dcmapDBREF* DCMAPAPI GetID();

	bool DCMAPAPI ActualizeData();
};
