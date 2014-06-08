//---------------------------------------------------------------------------

#ifndef BasePrefsH
#define BasePrefsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
//---------------------------------------------------------------------------
class TBasePrefsFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	void __fastcall OnChange(TObject *Sender);
private:	// User declarations
	TStringList* AutoList;
protected:
	void AutoLoadConfig(DCMapConfigWrapper cfg);
	void AutoSaveConfig(DCMapConfigWrapper cfg);
	void AddConfigTarget(String Section,int Tag);

	void AutoLoadConfig(DCMapConfigWrapper cfg,AnsiString section,int Tag);
	void AutoSaveConfig(DCMapConfigWrapper cfg,AnsiString section,int Tag);


	virtual void OnLoadPrefs(DCMapConfigWrapper cfg);
	virtual void OnSavePrefs(DCMapConfigWrapper cfg);

public:		// User declarations
	__fastcall TBasePrefsFrame(TComponent* Owner);
	__fastcall ~TBasePrefsFrame();
	void LoadConfig(DCMapConfigWrapper cfg);
	void OnApply();
};
//---------------------------------------------------------------------------
extern PACKAGE TBasePrefsFrame *BasePrefsFrame;
//---------------------------------------------------------------------------
#endif
