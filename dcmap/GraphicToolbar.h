//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef GraphicToolbarH
#define GraphicToolbarH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sToolBar.hpp"
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "sComboBox.hpp"
//---------------------------------------------------------------------------
class TGraphicToolbarFrame : public TBaseModuleFrame
{
	friend class TGraphHelperForm;
__published:	// IDE-managed Components
	TsSpeedButton *AddPolygonBtn;
	TsComboBox *LayerCombo;
	TsSpeedButton *EditButton;
	TsSpeedButton *AddPolylineBtn;
	TsSpeedButton *AddZonesBtn;
	void __fastcall AddPolygonBtnClick(TObject *Sender);
	void __fastcall EditButtonClick(TObject *Sender);
	void __fastcall LayerComboChange(TObject *Sender);
private:	// User declarations
	IDCMapLayeredMapView* m_pView;
    IDCMapGraphicLayer* m_pCurrentLayer;
    DCMapDataReference  m_LayerRef;

    String LastLayer;

    void LoadConfig(DCMapConfigWrapper cfg);
	void SaveConfig(DCMapConfigWrapper cfg);

	void UpdateTools(bool d);
	TGraphHelperForm* m_DrawForm;

	void DoEndDraw();

public:		// User declarations
	__fastcall TGraphicToolbarFrame(TComponent* Owner);
    void UpdateList();
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
};
//---------------------------------------------------------------------------
extern PACKAGE TGraphicToolbarFrame *GraphicToolbarFrame;
//---------------------------------------------------------------------------
#endif
