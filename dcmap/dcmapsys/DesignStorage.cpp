#include "StdAfx.h"
#include "DesignStorage.h"

DCMAP_REGISTER_STORAGEX(CComponetsStorage,"components",":Hidden:");

CComponetsStorage::CComponetsStorage()
{
	m_BR.RegisterWideString("name",&value_type::name,DCMDB_COMPONENT_NAME);
	m_BR.RegisterWideString("description",&value_type::description,DCMDB_COMPONENT_DESCRIPTION);

	m_BR.RegisterWideString("icon_name",&value_type::icon_name,DCMDB_COMPONENT_ICON_NAME);

	m_BR.RegisterInt("build_allow",&value_type::build_allow,DCMDB_COMPONENT_BUILD_ALLOW);

	m_BR.RegisterInt("build_req",&value_type::build_req,DCMDB_COMPONENT_BUILD_REQ);
	m_BR.RegisterInt("build_unique",&value_type::build_unique,DCMDB_COMPONENT_BUILD_UNIQUE);

	m_BR.RegisterInt("component_id",&value_type::component_id,DCMDB_COMPONENT_COMPID);
}

void DCMAPAPI CComponetsStorage::FillDefaults( value_type* pinf )
{
	Inherited::FillDefaults(pinf);
}

void* DCMAPAPI CComponetsStorage::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(CComponetsStorage);
	return Inherited::CastTo(idInterface);
}

