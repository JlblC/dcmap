#pragma once
#include "../dcmapsys.h"

class CPlanetManager :	public IDCMapPlanetManager
{
	dcmapDBREF m_CurrentPlanet;
	int m_iCurrentX;
	int m_iCurrentY;

	IDCMapWorkspaceUI* m_pUI;

	IDCMapWorkspace* m_pWorkspace;
	IDCMapCore* m_pCore;
	void DoEditPlanet();

	IDCMapPlanetDataStorage* m_pPlanets;

	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);

public:
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CPlanetManager();}
	CPlanetManager(void);
public:
	virtual ~CPlanetManager(void);

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);


	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);

	void DCMAPAPI SetCurrentPlanet(dcmapDBREF const* ref,int x,int y,int show=1);

	void DCMAPAPI EditPlanet(dcmapDBREF ref);
	void DCMAPAPI EditPlanetAt(int x,int y);

	bool DCMAPAPI GetCurrentPlanet(dcmapDBREF* ref);
	bool DCMAPAPI GetCurrentPlanetCoords(int* x,int* y);
	void DCMAPAPI EditCurrentPlanet();
	void DCMAPAPI DeleteCurrentPlanet();


};
