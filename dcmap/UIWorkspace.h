//---------------------------------------------------------------------------

#ifndef UIWorkspaceH
#define UIWorkspaceH

#include "dcmapsyslib.h"
#include <set>

class TViewLayoutFrame;
class TDCMapMenuItem;
class TBaseFrameForm;

TDCMapMenuItem* DCMAPAPI AddMenuItem(TMenuItem* pParent,const char* MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id);
TMenuItem* CreateMenuItem(TMenuItem* pParent,const char* MenuPath,dcmapWCSTR MenuCaption);

class TUIWorkspace : public IDCMapWorkspaceUI
{
	friend class TViewLayoutFrame;
	friend class TBaseFrameForm;

	TViewLayoutFrame* m_pLayout;
	TBaseFrameForm* FrameForm;

	bool Destroing;

	typedef std::set<TForm*> ToolForms;
	ToolForms m_ToolForms;

	IDCMapWorkspace* m_pWorkspace;
	IDCMapFrame*  m_pFrame;
	IDCMapCore*  m_pCore;
public:
	~TUIWorkspace();
	TUIWorkspace(IDCMapWorkspace* pWorkspace,IDCMapFrame* pFrame,IDCMapCore* pCore);

	void DCMAPAPI Destroy(){};

	void DCMAPAPI ActivateView();

	void DCMAPAPI OnActivate();
	void DCMAPAPI OnDeactivate();

	void DCMAPAPI RegisterForm(TForm* pForm);
	void DCMAPAPI UnregisterForm(TForm* pForm);
	IDCMapUIMenuItem* DCMAPAPI AddMenuItem(dcmapCSTR MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id);
	IDCMapUIMenuItem* DCMAPAPI AddMenu(dcmapCSTR MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id);

	IDCMapFrameUI* DCMAPAPI GetFrameUI(){return 0;}
	TViewLayoutFrame* GetLayout();
};
//---------------------------------------------------------------------------
#endif
 