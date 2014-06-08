//---------------------------------------------------------------------------

#ifndef OrgPropH
#define OrgPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sEdit.hpp"
#include "sGroupBox.hpp"
#include "sColorSelect.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TOrgPropFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
    TsRadioGroup *DipRadio;
    TsEdit *NameEdit;
	TsColorSelect *OrgColor2;
	TsColorSelect *OrgColor1;
    void __fastcall NameEditChange(TObject *Sender);
    void __fastcall DataChange(TObject *Sender);
private:	// User declarations
    void UpdateData();
    DataReference m_Ref;

    IDCMapOrganizationDataStorage* m_pOrg;

    bool m_bChanged;

public:		// User declarations
    __fastcall TOrgPropFrame(TComponent* Owner);
    void OnShow();
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    bool OnInit();
    void OnApply();
    void OnCansel();
    bool OnDataTest();
};
//---------------------------------------------------------------------------
extern PACKAGE TOrgPropFrame *OrgPropFrame;
//---------------------------------------------------------------------------
#endif
