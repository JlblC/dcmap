#include "stdafx.h"
#include "GeckoManager.h"

DCMAP_REGISTER(CGeckoManager,"PCoreService","GeckoManager");

void* DCMAPAPI CGeckoManager::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(IDCMapModule);
	DCMAP_MODULE_SUPPORTS(CGeckoManager);
	return 0;
}

CGeckoManager::CGeckoManager()
{

}

CGeckoManager::~CGeckoManager()
{
	NS_TermEmbedding();
}

bool  DCMAPAPI CGeckoManager::OnCreate( DCMapObjectCreationParams const* params )
{
	m_strGeckoLocation = params->szModuleDir;
	return true;
}

bool DCMAPAPI CGeckoManager::Init( IDCMapCore* core,IDCMapWorkspace* workspace )
{
	nsCOMPtr<nsILocalFile> appdir;
	NS_NewLocalFile(nsString(m_strGeckoLocation.c_str()), PR_FALSE,getter_AddRefs(appdir));

    if(NS_InitEmbedding(appdir, nsnull) != NS_OK) return false;
	return true;
}
