//---------------------------------------------------------------------------

#ifndef ColorSchemeToolbarH
#define ColorSchemeToolbarH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sComboBox.hpp"
//---------------------------------------------------------------------------
class TColorSchemeToolbarFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsComboBox *ColorSchemeCombo;
	void __fastcall ColorSchemeComboChange(TObject *Sender);
private:	// User declarations
    IDCMapColoringDataStorage* m_pColoring;
public:		// User declarations
	__fastcall TColorSchemeToolbarFrame(TComponent* Owner);
	__fastcall ~TColorSchemeToolbarFrame();

    void OnShow();
    bool OnInit();
    void LoadConfig(DCMapConfigWrapper cfg);
    String LastColoringStr;
	int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
};
//---------------------------------------------------------------------------
extern PACKAGE TColorSchemeToolbarFrame *ColorSchemeToolbarFrame;
//---------------------------------------------------------------------------
#endif
