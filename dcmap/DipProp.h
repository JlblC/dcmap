//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef DipPropH
#define DipPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sEdit.hpp"
#include "sColorSelect.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "sGroupBox.hpp"
//---------------------------------------------------------------------------
class TDipEditPropFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsEdit *NameEdit;
	TsColorSelect *DipColor;
	TsRadioGroup *DipRadio;
	void __fastcall NameEditChange(TObject *Sender);
	void __fastcall OnChange(TObject *Sender);
private:	// User declarations
    void UpdateData();
    DataReference m_Ref;
    IDCMapDipDataStoragePtr m_pDip;
public:		// User declarations
	__fastcall TDipEditPropFrame(TComponent* Owner);
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    bool OnInit();
    void OnApply();
    void OnCansel();
    bool OnDataTest();
};
//---------------------------------------------------------------------------
extern PACKAGE TDipEditPropFrame *DipEditPropFrame;
//---------------------------------------------------------------------------
#endif
