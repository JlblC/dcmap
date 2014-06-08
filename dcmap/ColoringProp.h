//---------------------------------------------------------------------------

#ifndef ColoringPropH
#define ColoringPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sEdit.hpp"
#include "sComboBox.hpp"
#include "sColorSelect.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TColoringPropframe : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsComboBox *BorderComboBox;
	TsComboBox *BackgroundComboBox;
	TsComboBox *PlanetComboBox;
	TsEdit *NameEdit;
	TsComboBox *PlanetBorderComboBox;
	TsComboBox *MarkerComboBox;
	TsColorSelect *PlanetColor;
	TsColorSelect *PlanetBorderColor;
	TsColorSelect *BackgroundColor;
	TsColorSelect *BorderColor;
	TsColorSelect *MarkerColor;
    void __fastcall NameEditChange(TObject *Sender);
    void __fastcall DataChange(TObject *Sender);
	void __fastcall ComboBoxChange(TObject *Sender);
private:	// User declarations
    void UpdateData();
    DataReference m_Ref;
    IDCMapColoringDataStorage* m_pColoring;

public:		// User declarations
    __fastcall TColoringPropframe(TComponent* Owner);
    void OnShow();
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    bool OnInit();
    void OnApply();
    void OnCansel();
    bool OnDataTest();
};
//---------------------------------------------------------------------------
extern PACKAGE TColoringPropframe *ColoringPropframe;
//---------------------------------------------------------------------------
#endif
