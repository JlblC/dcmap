#pragma once
#include "../dcmapsys.h"
#include "BaseDataStorage.h"

class CComponetsStorage : public CBaseGameIndexedDataStorage<DCMapComponentInfo,IDCMapComponentsDataStorage>
{
	typedef CBaseGameIndexedDataStorage<DCMapComponentInfo,IDCMapComponentsDataStorage> Inherited;
public:
	CComponetsStorage();

	dcmapCSTR DCMAPAPI Name(){return "components";}
	dcmapCSTR DCMAPAPI RecordName(){return "component";}
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
	void DCMAPAPI FillDefaults(value_type* pinf);
	int DCMAPAPI TableParams(){return DCMAP_TABLE_INTERNAL | Inherited::TableParams();}
};
DCMAP_SPEC_INTERFACE(CComponetsStorage);