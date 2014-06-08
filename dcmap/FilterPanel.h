//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef FilterPanelH
#define FilterPanelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "Filter.h"

class CMapFilter : public IDCMapPlanetDataFilter
{
	friend class TFilterPanelFrame;
public:
	CMapFilter();
    TFilterPanelFrame* m_pFrame;
private:
	IDCMapWorkspace* m_pWorkspace;
	virtual char const* DCMAPAPI ModuleID(){return "CMapFilter";};
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);

	void  DCMAPAPI LoadConfig(IDCMapConfig* pConfig);
	void  DCMAPAPI SaveConfig(IDCMapConfig* pConfig);

	int DCMAPAPI OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	void* DCMAPAPI CastTo(char const* szInterface);
	bool DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* data,DCMapPlanetInfo const* pinf);
    bool DCMAPAPI ActualizeData();
    bool Changed;
};

//---------------------------------------------------------------------------
class TFilterPanelFrame : public TBaseModuleFrame
{
	friend class CMapFilter;
    
__published:	// IDE-managed Components
	TFilterFrame *FilterFrame1;
private:	// User declarations
	CMapFilter* m_pFilter;
public:		// User declarations
	__fastcall TFilterPanelFrame(TComponent* Owner);
    bool OnInit();

    int OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	void OnShow();
    void LoadConfig(DCMapConfigWrapper cfg);
    void SaveConfig(DCMapConfigWrapper cfg);
};
//---------------------------------------------------------------------------
extern PACKAGE TFilterPanelFrame *FilterPanelFrame;
//---------------------------------------------------------------------------
#endif
