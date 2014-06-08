//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef LayerPropH
#define LayerPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sEdit.hpp"
#include "sComboBox.hpp"
#include "sCheckBox.hpp"
//---------------------------------------------------------------------------
class TLayerEditFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsEdit *NameEdit;
	TsComboBox *ModuleCombo;
	TsCheckBox *HiddenCheck;
	void __fastcall NameEditChange(TObject *Sender);
	void __fastcall OnChange(TObject *Sender);
private:	// User declarations
    void UpdateData();
    DataReference m_Ref;
    IDCMapLayersDataStoragePtr m_pLayers;
public:		// User declarations
	__fastcall TLayerEditFrame(TComponent* Owner);
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    bool OnInit();
    void OnApply();
    bool OnDataTest();
};
//---------------------------------------------------------------------------
extern PACKAGE TLayerEditFrame *LayerEditFrame;
//---------------------------------------------------------------------------
#endif
