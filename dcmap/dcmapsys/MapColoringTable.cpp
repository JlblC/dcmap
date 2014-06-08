#include "stdafx.h"
#include "../dcmapsys.h"
#include "BaseDataStorage.h"

class CColoringDataStorage : public CBaseNamedDataStorage<DCMapColoringInfo,IDCMapColoringDataStorage>
{
public:
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CColoringDataStorage();}
	CColoringDataStorage()
	{
		m_BR.RegisterString("planet",&value_type::planet);
		m_BR.RegisterString("background",&value_type::background);
		m_BR.RegisterString("cellborder",&value_type::cellborder);
		m_BR.RegisterString("planetborder",&value_type::planetborder);
		m_BR.RegisterString("planetborder",&value_type::planetborder);
		m_BR.RegisterString("marker",&value_type::marker);

		m_BR.RegisterInt("planet_color",&value_type::planet_color);
		m_BR.RegisterInt("background_color",&value_type::background_color);
		m_BR.RegisterInt("cellborder_color",&value_type::cellborder_color);
		m_BR.RegisterInt("planetborder_color",&value_type::planetborder_color);
		m_BR.RegisterInt("marker_color",&value_type::marker_color);

	}
	void DCMAPAPI FillDefaults(value_type* pinf)
	{
		memset(pinf,0,sizeof(value_type));
	}
	dcmapCSTR DCMAPAPI Name(){return "coloring";}
};

DCMAP_REGISTER_STORAGE(CColoringDataStorage,"coloring")
