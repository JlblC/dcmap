//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef AboutH
#define AboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sSkinProvider.hpp"
#include "sLabel.hpp"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//#include "TntForms.hpp"
//#include "TntMenus.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "BaseViewPopup.h"

//---------------------------------------------------------------------------
class TAboutForm : public TBaseViewPopupForm
{
	typedef TBaseViewPopupForm Inherited;
__published:	// IDE-managed Components
	TsLabelFX *sLabelFX1;
	TsLabelFX *StrID;
	TsLabel *VerLabel;
	TsLabel *Version;
	TsLabel *ApiVerLabel;
	TsLabel *ApiVer;
	TsLabel *CopyR1Label;
	TsWebLabel *sWebLabel1;
	TImage *Image1;
	TsLabelFX *RevisionLabel;
	TsSpeedButton *CheckUpdateBtn;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClick(TObject *Sender);
	void __fastcall CheckUpdateBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TAboutForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutForm *AboutForm;
//---------------------------------------------------------------------------
#endif
