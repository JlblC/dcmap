//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef MainPrefsH
#define MainPrefsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sCheckBox.hpp"
#include "sAlphaListBox.hpp"
#include "sComboBox.hpp"
#include "sTreeView.hpp"
#include <ComCtrls.hpp>
#include "sTrackBar.hpp"
#include "sLabel.hpp"
#include "BasePrefs.h"
#include "sBitBtn.hpp"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TMainPrefsFrame : public TBasePrefsFrame
{
__published:	// IDE-managed Components
	TsCheckBox *SkinEnableCheck;
	TsListBox *SkinList;
	TsTrackBar *HueBar;
	TsLabel *HueLabel;
	TsTrackBar *SatBar;
	TsLabel *SatLabel;
	TsCheckBox *AlwaysShowTabsCheck;
	TsCheckBox *XPStylesCheck;
	TsCheckBox *AnimationCheck;
	TsCheckBox *PreviewCheck;
	TsBitBtn *DownloadSkinsBtn;
	void __fastcall OnChange(TObject *Sender);
	void __fastcall SkinEnableCheckClick(TObject *Sender);
	void __fastcall PreviewCheckClick(TObject *Sender);
	void __fastcall SatBarChange(TObject *Sender);
	void __fastcall HueBarChange(TObject *Sender);
	void __fastcall SkinListClick(TObject *Sender);
	void __fastcall DownloadSkinsBtnClick(TObject *Sender);
private:	// User declarations
	bool m_bPreSkinState;
	String m_PreSkin;
	int m_PreSat;
	int m_PreHue;

	void ApplyVisual();
	void RestoreVisual();

public:		// User declarations
	__fastcall TMainPrefsFrame(TComponent* Owner);
	__fastcall ~TMainPrefsFrame();
        void LoadConfig(DCMapConfigWrapper cfg);
        void OnCansel();
        void OnApply();
		void OnShow();
		bool OnInit();
};
//---------------------------------------------------------------------------
extern PACKAGE TMainPrefsFrame *MainPrefsFrame;
//---------------------------------------------------------------------------
#endif
