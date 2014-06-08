//---------------------------------------------------------------------------
#ifndef IconsListManH
#define IconsListManH

#include "BaseModule.h"
#include "sComboBoxes.hpp"

class TIconsList : public IDCMapModule
{
protected:
	TImageList* m_pList;

	std::vector<DCMapDataReference> Refs;

	IDCMapIconsDataStoragePtr m_pIcons;

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
	dcmapLRESULT DCMAPAPI OnMessage(dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender);
	bool m_bNeedUpdate;

public:
	TIconsList();
	~TIconsList();
	void UpdateIcons();
	void FillItems(TsComboBoxEx* pCombo);
	int  CreateMenuItems(TMenuItem* pItem,Classes::TNotifyEvent OnClick,int at=0);

	void Prepare();

	TImageList* ImageList()const{return m_pList;};
	int IconIndex(dcmapDBREF* ref);
	int IconIndex(dcmapDBREF const& ref);

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
};
DCMAP_SPEC_INTERFACE(TIconsList);

//---------------------------------------------------------------------------
#endif
