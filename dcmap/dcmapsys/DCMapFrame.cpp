#include "StdAfx.h"
#include "DCMapFrame.h"
#include "DCMapCore.h"
#include "DCMapSystem.h"
#include <g_file_util.h>
#include "../api/dcmap_script_helper.h"

DCMAP_SCRIPT_REFLECTION(CDCMapFrame)
{
	DCMAP_SCRIPT_FUNC(CreateModule);
	DCMAP_SCRIPT_FUNC(DestroyModule);
	DCMAP_SCRIPT_FUNC(GetService);

	DCMAP_SCRIPT_PROPERTY_R(UI);
	DCMAP_SCRIPT_PROPERTY_R(Core);
}

CDCMapFrame::CDCMapFrame(void):
	m_pUI(0)
{

}

CDCMapFrame::~CDCMapFrame(void)
{

}

void* DCMAPAPI CDCMapFrame::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(CDCMapFrame);
	DCMAP_MODULE_SUPPORTS(IDCMapFrameEx);
	DCMAP_MODULE_SUPPORTS(IDCMapFrame);
	DCMAP_MODULE_SUPPORTS(IDCMapScriptable);
	DCMAP_MODULE_SUPPORTS(IDCMapObject);
	return 0;
}