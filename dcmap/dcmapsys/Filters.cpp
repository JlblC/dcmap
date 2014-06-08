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


static bool Compare(int op,float ref,float val)
{
	return val >= ref;
}

struct CBaseFilter : public IDCMapFilterItem
{
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapFilterItem);
		return 0;
	}

	bool m_bEnabled;
	CBaseFilter()
	{
		m_bEnabled = true;
	}
	bool DCMAPAPI Enabled(){return m_bEnabled;}
	void DCMAPAPI SetEnabled(bool e){m_bEnabled=e;}
};

struct CPlanetSizeFilter : public CBaseFilter
{
	IDCMapPlanetDataStorage* m_pPlanets;
	dcmapQWORD PlanetIndex;
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pPlanets = core->GetDataSourceTyped<IDCMapPlanetDataStorage>("planets");
		if(!m_pPlanets)return false;
		PlanetIndex = m_pPlanets->GetStateIndex();
		return true;
	}
	dcmapCSTR DCMAPAPI UIModuleName(){return "ResourceCompare";}
	virtual dcmapLOCSTR DCMAPAPI ItemCaption(){return L"Размер планеты";}

	int op;
	float refValue;

	void DCMAPAPI SetRealParam(int id,dcmapREAL val){refValue = val;};
	void DCMAPAPI SetIntParam(int id,dcmapINT val){op = val;};

	bool DCMAPAPI Process(dcmapDBREF const& ref)
	{
		if(m_pPlanets->SelectID(ref))
		{
			float val = m_pPlanets->GetData()->s;
			return Compare(op,refValue,val);
		}
		return false;
	}
};
DCMAP_REGISTER(CPlanetSizeFilter,"FilterItem","PlanetSize");





