//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef FilterUIResourceH
#define FilterUIResourceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sCheckBox.hpp"
#include "sComboBox.hpp"
#include "sEdit.hpp"
#include "sFrameAdapter.hpp"
#include "sSpinEdit.hpp"
//---------------------------------------------------------------------------
class TFilterUIResourceFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsCheckBox *NameCheck;
	TsComboBox *OpCombo1;
	TsDecimalSpinEdit *ValueEdit1;
private:	// User declarations
	IDCMapFilterItem* m_pFilter;
public:		// User declarations
	__fastcall TFilterUIResourceFrame(TComponent* Owner);
    int OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
};
//---------------------------------------------------------------------------
extern PACKAGE TFilterUIResourceFrame *FilterUIResourceFrame;
//---------------------------------------------------------------------------
#endif
