//---------------------------------------------------------------------------

#include "dcmapsyslib.h"

#ifndef UIFrameFormH
#define UIFrameFormH

class CBaseFrameFormModule;
class TBaseFrameForm;
class TCoreUIManager;

class TFrameUI : public IDCMapVCLFrameUI
{
	IDCMapCore* m_pCore;
	IDCMapFrame* m_pFrame;
	TCoreUIManager* m_pCoreUI;

	CBaseFrameFormModule* m_pFrameModule;
	TBaseFrameForm* m_pFrameForm;
public:
	TFrameUI(IDCMapFrame* pFrame,IDCMapCore* pCore);

	bool DCMAPAPI CreateFrameForm();
	TBaseFrameForm* GetFrameForm(){return m_pFrameForm;}
	TForm* DCMAPAPI GetForm();

	void DCMAPAPI ShowHint(dcmapRECT const& area,dcmapDCSTR string);
	void DCMAPAPI HideHint();
	void DCMAPAPI Destroy(){};
	bool OnNewWorkspace(IDCMapWorkspaceUI* pWorkspace);

};

//---------------------------------------------------------------------------
#endif
