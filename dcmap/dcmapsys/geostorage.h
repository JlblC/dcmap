#pragma once
#include "BaseDataStorage.h"
#include <g_staticarray2d.h>

class CPlanetDataStorage;

class CGeoStorage : public CCustomDataStorage<DCMapPlanetParamInfo,IDCMapGeoDataStorage,CGeoStorage>
{
	friend class CPlanetDataStorage; 
protected:

	bool DoLoadData();

	CPlanetDataStorage* m_pPlanets;
public:
	dcmapDWORD m_NumRecords;
	gsys::static_array2d<DCMapPlanetGeoInfo,DCMapNumPlanets,DCMapNumPlanets> m_data;

	int m_iEndX;
	int m_iEndY;

	void DCMAPAPI Clear();
	int DCMAPAPI NumRecords(){return m_NumRecords;}
	bool EnumMode;
	value_type m_Record;
	CGeoStorage(void);
	static const dcmapFOURCC c_DBMagic = DCMAP_MAKEFOURCC('G','E','O','B');

#pragma pack(push)
#pragma pack(1)
	struct DataFileHeader
	{
		dcmapFOURCC magic;
		dcmapDWORD NumRecords;
		dcmapQWORD modIndex;
		int RecordSize;
		dcmapWORD  DBVer;
		dcmapWORD  TableVer;
		int reserved;
	};
#pragma pack(pop)

	bool static TestCoords(int x,int y)
	{
		if(x<1 || x>g_DCMapNumPlanets)return false;
		if(y<1 || y>g_DCMapNumPlanets)return false;
		return true;
	}

	void OnChange();

	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);

	bool DCMAPAPI SaveDataToFile(dcmapFCSTR szFileName,int Flags);
	const dcmapDBREF* DCMAPAPI GetID();
	bool DCMAPAPI SelectCoord(int x,int y);
	bool DCMAPAPI SelectIndex();
	bool DCMAPAPI SelectID(const dcmapDBREF &ref);
	bool LocatePlanet();
	bool DCMAPAPI SelectAll();
	bool DCMAPAPI Next();
	bool DCMAPAPI Delete();
	bool DCMAPAPI Post(dcmapDBREF * ref);
	bool DCMAPAPI SelectLine(int LineNum,int From,int To);
	bool DCMAPAPI UpdateGeo(const DCMapPlanetParamInfo* data);

	dcmapCSTR DCMAPAPI Name(){return "geo";};
	void DCMAPAPI PostInit();
};
DCMAP_SPEC_INTERFACE(CGeoStorage);