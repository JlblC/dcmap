//---------------------------------------------------------------------------

#ifndef RacePropH
#define RacePropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sEdit.hpp"
#include "sSpinEdit.hpp"
#include "sCurrEdit.hpp"
#include "sCurrencyEdit.hpp"
#include "sCustomComboEdit.hpp"
#include "sMaskEdit.hpp"
#include <Mask.hpp>
#include "sComboBox.hpp"
#include <ImgList.hpp>
#include "sComboBoxes.hpp"
//---------------------------------------------------------------------------
class TRaceEditPropFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
    TsEdit *NameEdit;
    TsDecimalSpinEdit *MinTEdit;
    TsDecimalSpinEdit *MaxTEdit;
    TsDecimalSpinEdit *GrowthEdit;
    TsDecimalSpinEdit *MningEdit;
    TsComboBoxEx *NatureEdit;
    TsComboBoxEx *BasisEdit;
    TsComboBoxEx *SecondEdit;
	TsDecimalSpinEdit *MinGrowthEdit;
    void __fastcall DataChanged(TObject *Sender);
    void __fastcall NatureEditChange(TObject *Sender);
    void __fastcall BasisEditChange(TObject *Sender);
    void __fastcall SecondEditChange(TObject *Sender);
    void __fastcall NameEditChange(TObject *Sender);
private:	// User declarations
    void UpdateData();
    DataReference m_Ref;
    IDCMapRacesDataStorage*   m_pRaces;
public:		// User declarations
    __fastcall TRaceEditPropFrame(TComponent* Owner);
    void OnShow();
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    bool OnInit();
    void OnApply();
    bool OnDataTest();
};
//---------------------------------------------------------------------------
extern PACKAGE TRaceEditPropFrame *RaceEditPropFrame;
//---------------------------------------------------------------------------
#endif
