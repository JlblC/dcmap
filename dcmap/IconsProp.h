//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef IconsPropH
#define IconsPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sEdit.hpp"
#include "sLabel.hpp"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include "sButton.hpp"
#include "sDialogs.hpp"
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include "sCheckBox.hpp"
//---------------------------------------------------------------------------
class TIconsPropFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsEdit *NameEdit;
	TsPanel *sPanel1;
	TsOpenPictureDialog *OpenDlg;
	TsButton *LoadImageButton;
	TImage *Image;
	TsCheckBox *TrnsCheck;
	TsPanel *sPanel2;
	TImage *BigImage;
	TsButton *LoadBigImageButton;
	TsLabel *IconLabel;
	TsLabel *BigIconLabel;
	TsButton *ClearImageButton;
	TsButton *ClearBigImageButton;
	TsSavePictureDialog *SaveDlg;
	TsButton *SaveImageButton;
	TsButton *SaveBigImageButton;
	void __fastcall NameEditChange(TObject *Sender);
	void __fastcall LoadImageButtonClick(TObject *Sender);
	void __fastcall TrnsCheckClick(TObject *Sender);
	void __fastcall LoadBigImageButtonClick(TObject *Sender);
	void __fastcall ClearImageButtonClick(TObject *Sender);
	void __fastcall ClearBigImageButtonClick(TObject *Sender);
	void __fastcall SaveImageButtonClick(TObject *Sender);
	void __fastcall SaveBigImageButtonClick(TObject *Sender);
private:	// User declarations
    void UpdateData();
    DataReference m_Ref;
    IDCMapIconsDataStorage* m_pIcons;
public:		// User declarations
	__fastcall TIconsPropFrame(TComponent* Owner);
    int OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    bool OnInit();
    void OnApply();
    bool OnDataTest();
};
//---------------------------------------------------------------------------
extern PACKAGE TIconsPropFrame *IconsPropFrame;
//---------------------------------------------------------------------------
#endif
