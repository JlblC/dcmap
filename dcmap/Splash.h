//---------------------------------------------------------------------------

#ifndef SplashH
#define SplashH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sSkinProvider.hpp"
#include "acProgressBar.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "sLabel.hpp"
#include <Graphics.hpp>
#include "sPanel.hpp"
#include "sBevel.hpp"
#include "BaseViewPopup.h"

//---------------------------------------------------------------------------
class TSplashForm : public TBaseViewPopupForm
{
	typedef TBaseViewPopupForm Inherited;

__published:	// IDE-managed Components
	TsPanel *Panel;
	TsLabelFX *sLabelFX1;
	TsLabelFX *RevisionLabel;
	TsLabelFX *StrID;
	TsLabelFX *StatusLabel;
	TsLabelFX *sLabelFX2;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TSplashForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSplashForm *SplashForm;
//---------------------------------------------------------------------------
#endif
