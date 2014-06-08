//---------------------------------------------------------------------------

#ifndef InputH
#define InputH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseFormModule.h"
#include "sSkinProvider.hpp"
#include "sBitBtn.hpp"
#include <Buttons.hpp>
#include "sLabel.hpp"
#include "sComboBox.hpp"
#include "sEdit.hpp"
#include <vector>
//---------------------------------------------------------------------------
class TInputDialog : public TBaseModuleForm
{
__published:	// IDE-managed Components
	void __fastcall ButtonClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
private:	// User declarations
	TsComboBox* InputCombo;
	TsEdit* InputEdit;

	int ButtonsWidth;

	static const int ButtonsHeight = 50;

	int WorkHeight;

    String InputStyle;

	void AddButtons();
	void AlignButtons();

	int m_iResult;

	std::vector<TsBitBtn*> VecButtons;

	IDCMapVarValuePtr m_Params;
	IDCMapVarValuePtr m_Result;

	void AddButton(String Cation,int Value,int type);

	void AddLabel();
	void AddCombo();
	void AddEdit();

	int DoQuery(AnsiString Module,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
public:		// User declarations
	__fastcall TInputDialog(TComponent* Owner);

	static int Query(AnsiString Module,IDCMapVarValue* pParams,IDCMapVarValue* pResult);


};
//---------------------------------------------------------------------------
extern PACKAGE TInputDialog *InputDialog;
//---------------------------------------------------------------------------
#endif
