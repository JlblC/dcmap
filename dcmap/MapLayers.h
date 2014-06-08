//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef MapLayersH
#define MapLayersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SizeableSideModule.h"
#include "sFrameAdapter.hpp"
#include "sAlphaListBox.hpp"
#include "sCheckListBox.hpp"
#include "BaseModule.h"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMapLayersFrame : public TSizeableSideFrame
{
	typedef TSizeableSideFrame Inherited;
__published:	// IDE-managed Components
	TsCheckListBox *LayersListBox;
	void __fastcall LayersListBoxClickCheck(TObject *Sender);
	void __fastcall LayersListBoxClick(TObject *Sender);
	void __fastcall LayersListBoxDblClick(TObject *Sender);
private:	// User declarations
	IDCMapLayeredMapView* m_pView;
	String  LastSel;
	int LastNumLayers;
public:		// User declarations
	__fastcall TMapLayersFrame(TComponent* Owner);
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    void UpdateList();
};
//---------------------------------------------------------------------------
extern PACKAGE TMapLayersFrame *MapLayersFrame;
//---------------------------------------------------------------------------
#endif
