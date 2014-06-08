//---------------------------------------------------------------------------

#ifndef FakeMainH
#define FakeMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <set>

#include "dcmapbase.h"

class TBaseFrameForm;
//---------------------------------------------------------------------------
class TFakeMainForm : public TForm
{
__published:	// IDE-managed Components
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
	typedef std::set<TBaseFrameForm*> SetFrames;
	SetFrames FrameForms;

	IDCMapCoreEx* m_pCore;

public:		// User declarations
	__fastcall TFakeMainForm(TComponent* Owner);
	void OnFrameClose(TBaseFrameForm* Form);
};
//---------------------------------------------------------------------------
extern PACKAGE TFakeMainForm *FakeMainForm;
//---------------------------------------------------------------------------
#endif
