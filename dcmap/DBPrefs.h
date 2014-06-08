//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef DBPrefsH
#define DBPrefsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sCheckBox.hpp"
#include "sEdit.hpp"
#include "sSpinEdit.hpp"
#include "BasePrefs.h"
#include "sGroupBox.hpp"
//---------------------------------------------------------------------------
class TDBFrefsFrame : public TBasePrefsFrame
{
__published:	// IDE-managed Components
	TsGroupBox *BackupsGroupBox;
	TsCheckBox *UseHardlinks;
	TsCheckBox *UseBackups;
	TsSpinEdit *NumBackups;
	TsCheckBox *CompressDatabase;
	void __fastcall UseBackupsClick(TObject *Sender);
private:	// User declarations
	void OnLoadPrefs(DCMapConfigWrapper cfg);
public:		// User declarations
	__fastcall TDBFrefsFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDBFrefsFrame *DBFrefsFrame;
//---------------------------------------------------------------------------
#endif
