//---------------------------------------------------------------------------

#ifndef UISystemH
#define UISystemH
//---------------------------------------------------------------------------
#include "dcmapsyslib.h"
#include <set>

class TBaseFrameForm;

class TSysUI : public IDCMapSysUI
{
	TBaseFrameForm* m_pActiveFrame;
	typedef std::set<TBaseFrameForm*> SetFrames;
    SetFrames FrameForms;
public:
	TSysUI();
	void DCMAPAPI Destroy(){};
	IDCMapCoreUI* DCMAPAPI CreateCoreUI(IDCMapCore*);
	IDCMapFrameUI* DCMAPAPI CreateFrameUI(IDCMapFrame*,IDCMapCore*);
	IDCMapWorkspaceUI* DCMAPAPI CreateWorkspaceUI(IDCMapWorkspace*,IDCMapFrame*,IDCMapCore*);

	TBaseFrameForm* ActiveFrame(){return m_pActiveFrame;}
	TForm* ActiveFrameForm();

	void OnFrameActivate(TBaseFrameForm* pFrame);
};

extern TSysUI SysUI;

#endif
