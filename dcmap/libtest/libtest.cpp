
#include "../api/dcmapsys.h"

IDCMapSys* g_pSys;
IDCMapCoreEx* g_pCore;

int main()
{
	g_pSys = dcmapsysInit(DCMAP_API_VERSION);
	if(!g_pSys)return 2;
	g_pCore = dcmapsysCreateCore();
	if(!g_pCore)return 2;
	dcmapsysInitModules();

	return 0;
}