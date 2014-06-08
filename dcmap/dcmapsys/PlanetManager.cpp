#include "StdAfx.h"
#include "PlanetManager.h"
#include "../dcmapsys.h"
#include "../api/dcmap_script_helper.h"

int inline WrapCoord(int c){return (c+g_DCMapNumPlanets-1)%g_DCMapNumPlanets+1;}

DCMAP_REGISTER(CPlanetManager,"PService","PlanetManager")

DCMAP_SCRIPT_REFLECTION(CPlanetManager)
{
	DCMAP_SCRIPT_FUNC(SetCurrentPlanet);
	DCMAP_SCRIPT_FUNC(EditCurrentPlanet);
	DCMAP_SCRIPT_FUNC(DeleteCurrentPlanet);
	DCMAP_SCRIPT_FUNC(EditPlanet);
	DCMAP_SCRIPT_FUNC(EditPlanetAt);
}

CPlanetManager::CPlanetManager(void)
{
	m_CurrentPlanet.Invalidate();

	m_iCurrentX = -1;
	m_iCurrentY = -1;
}

CPlanetManager::~CPlanetManager(void)
{
}

bool  DCMAPAPI CPlanetManager::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	m_pUI = workspace->GetUI();
	m_pCore = core;

	m_pWorkspace = workspace;

	m_pPlanets = m_pCore->GetDataSourceTyped<IDCMapPlanetDataStorage>("planets");
	if(!m_pPlanets) return false;

	return true;
}

int DCMAPAPI CPlanetManager::OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
{
	switch(Msg)
	{
	case DCMM_DATA_CHANGED:
		if(m_iCurrentX >=0 && m_iCurrentY >=0)
			SetCurrentPlanet(0,m_iCurrentX,m_iCurrentY,0);
		break;
	}
	return 0;
}


void DCMAPAPI CPlanetManager::SetCurrentPlanet(dcmapDBREF const* pref,int x,int y,int show)
{
	x = WrapCoord(x);
	y = WrapCoord(y);

	dcmapDBREF ref;

	if((!pref || !pref->Valid()))
	{
		if(m_pPlanets->SelectCoord(x,y))
		{
			ref = *m_pPlanets->GetID();
		}		
		
		if(!ref.Valid() && (m_iCurrentX != x || m_iCurrentY != y))
		{
			m_iCurrentX = x;
			m_iCurrentY = y;

			m_CurrentPlanet.Invalidate();
			m_pWorkspace->BroadcastMessage(DCMM_PLANET_CHANGED,ref.id,ref.age,0);
            if(show)m_pWorkspace->BroadcastMessage(DCMM_SHOW_CELL,dcmapMAKELONG(x,y),show-1,0);
			return;
		}
        else if(show)
        {
            m_pWorkspace->BroadcastMessage(DCMM_SHOW_CELL,dcmapMAKELONG(x,y),show-1,0);
        }
	}
	else ref = *pref;

   	if(ref == m_CurrentPlanet) 
    {
        if(show)
        {
            m_pWorkspace->BroadcastMessage(DCMM_SHOW_CELL,dcmapMAKELONG(m_iCurrentX,m_iCurrentY),show-1,0);
        }
        return;
    }

	if(m_pPlanets->SelectID(ref))
	{
		m_CurrentPlanet = ref;
		m_iCurrentX	= m_pPlanets->GetData()->x;
		m_iCurrentY	= m_pPlanets->GetData()->y;
		m_pWorkspace->BroadcastMessage(DCMM_PLANET_CHANGED,ref.id,ref.age,0);
		if(show)m_pWorkspace->BroadcastMessage(DCMM_SHOW_CELL,dcmapMAKELONG(m_iCurrentX,m_iCurrentY),show-1,0);
	}
}

void CPlanetManager::DoEditPlanet()
{
	bool insert=false;

	if(m_CurrentPlanet.id == DCMapInvalidReferenceId)
	{
		if(m_pPlanets->SelectCoord(m_iCurrentX,m_iCurrentY))
		{
			m_CurrentPlanet = *m_pPlanets->GetID();
		}
		else
		{
			insert=true;
			m_pPlanets->BeginUpdate();

			m_pPlanets->Insert();
			m_pPlanets->SetCoords(m_iCurrentX,m_iCurrentY);
			if(!m_pPlanets->Post(&m_CurrentPlanet))
			{
				m_pPlanets->EndUpdate();
				return;
			}
		}
	}

	if(m_CurrentPlanet.id != DCMapInvalidReferenceId)
	{
		if(!m_pCore->GetCoreUI()->OpenListTableEdit("planets",&m_CurrentPlanet))
		{
			if(insert)
			{
				if(m_pPlanets->SelectID(m_CurrentPlanet))
					m_pPlanets->Delete();
			}
			//m_iCurrentX = -1;
			//m_iCurrentY = -1;
		}
	}
	if(insert)m_pPlanets->EndUpdate();
}

void DCMAPAPI CPlanetManager::EditCurrentPlanet()
{
	DoEditPlanet();
}

void DCMAPAPI CPlanetManager::DeleteCurrentPlanet()
{
	if(m_pPlanets->SelectID(m_CurrentPlanet))
	{
		m_pPlanets->Delete();
		m_CurrentPlanet.Invalidate();
		m_pWorkspace->BroadcastMessage(DCMM_PLANET_CHANGED,m_CurrentPlanet.id,m_CurrentPlanet.age,0);
	}

}

void DCMAPAPI CPlanetManager::EditPlanet(dcmapDBREF ref)
{
	SetCurrentPlanet(&ref,-1,-1);
	DoEditPlanet();
}

void DCMAPAPI CPlanetManager::EditPlanetAt(int x,int y)
{
	m_CurrentPlanet.id = DCMapInvalidReferenceId;

	m_iCurrentX = x;
	m_iCurrentY = y;
	DoEditPlanet();
}

bool DCMAPAPI CPlanetManager::GetCurrentPlanet(dcmapDBREF* ref)
{
	if(ref)*ref = m_CurrentPlanet;
	return m_CurrentPlanet.id != DCMapInvalidReferenceId;
}

bool DCMAPAPI CPlanetManager::GetCurrentPlanetCoords(int* x,int* y)
{
	if(m_iCurrentX<0 || m_iCurrentY<0) return false;
	if(x)*x = m_iCurrentX;
	if(y)*y = m_iCurrentY;
	return true;
	//m_CurrentPlanet.id == InvalidReferenceId;
}

void* DCMAPAPI CPlanetManager::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(IDCMapPlanetManager);
	return 0;
}

