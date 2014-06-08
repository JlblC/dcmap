//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef RaceAdvPropH
#define RaceAdvPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sEdit.hpp"
#include "sSpinEdit.hpp"
//---------------------------------------------------------------------------
class TRaceAdvPropFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsDecimalSpinEdit *ScienceEdit;
	TsDecimalSpinEdit *SpeedEdit;
	TsDecimalSpinEdit *PricesEdit;
	TsDecimalSpinEdit *CivProdEdit;
	TsDecimalSpinEdit *MilProdEdit;
	TsDecimalSpinEdit *HitpointsEdit;
	TsDecimalSpinEdit *DamageEdit;
	TsDecimalSpinEdit *DetectionEdit;
	TsDecimalSpinEdit *StealthEdit;
	TsDecimalSpinEdit *DefenceEdit;
	void __fastcall DataChanged(TObject *Sender);
private:	// User declarations
    void UpdateData();
    DataReference m_Ref;
    IDCMapRacesDataStorage*   m_pRaces;
public:		// User declarations
	__fastcall TRaceAdvPropFrame(TComponent* Owner);
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    bool OnInit();
    void OnApply();
};
//---------------------------------------------------------------------------
extern PACKAGE TRaceAdvPropFrame *RaceAdvPropFrame;
//---------------------------------------------------------------------------
#endif
