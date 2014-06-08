//---------------------------------------------------------------------------

#ifndef BasePropH
#define BasePropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
//---------------------------------------------------------------------------
class TBaseEditModuleFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
void __fastcall NameEditChange(TObject *Sender);
void __fastcall OnChange(TObject *Sender);
private:	// User declarations

protected:
    dcmapDBREF m_DataRef;
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    virtual void OnDataUpdate();
    bool NameChanged;
public:		// User declarations
	__fastcall TBaseEditModuleFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBaseEditModuleFrame *BaseEditModuleFrame;
//---------------------------------------------------------------------------
#endif
