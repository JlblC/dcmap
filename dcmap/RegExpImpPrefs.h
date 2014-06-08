//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef RegExpImpPrefsH
#define RegExpImpPrefsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sAlphaListBox.hpp"
#include "sComboBox.hpp"
#include "sMemo.hpp"
#include "sButton.hpp"
#include "sEdit.hpp"
#include "ScriptEditModule.h"
//---------------------------------------------------------------------------
class TRegExpImpPrefsFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsComboBox *BlockComboBox;
	TsListBox *ConfListBox;
	TsEdit *NameEdit;
	TsButton *AddButton;
	TsButton *DeleteButton;
	TScriptEditFrame *ScriptEditFrame;
	void __fastcall AddButtonClick(TObject *Sender);
	void __fastcall DeleteButtonClick(TObject *Sender);
	void __fastcall ConfListBoxClick(TObject *Sender);
	void __fastcall NameEditChange(TObject *Sender);
	void __fastcall BlockComboBoxChange(TObject *Sender);
private:	// User declarations
	String m_strCurrentSection;
	String m_strCurrentBaseName;
	String m_strCurrentParamName;

    int m_iCurrentID;
	void LoadParams(String Section,String BaseName,String ParamName);
    void SaveParams();
public:		// User declarations
	__fastcall TRegExpImpPrefsFrame(TComponent* Owner);
    bool OnInit();
    void OnApply();
};
//---------------------------------------------------------------------------
extern PACKAGE TRegExpImpPrefsFrame *RegExpImpPrefsFrame;
//---------------------------------------------------------------------------
#endif
