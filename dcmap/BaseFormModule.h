//---------------------------------------------------------------------------

#ifndef BaseFormModuleH
#define BaseFormModuleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sSkinProvider.hpp"
#include "dcmapsyslib.h"

#include "BaseModule.h"

//---------------------------------------------------------------------------
class TBaseModuleForm : public TForm
{
	typedef TForm Inherited;
__published:	// IDE-managed Components
	TsSkinProvider *SkinProvider;
private:	// User declarations
protected:
	IDCMapSys*        m_pSystem;
	IDCMapCore*       m_pCore;
	IDCMapWorkspace*  m_pWorkspace;
	IDCMapFrame*      m_pFrame;

	void __fastcall TBaseModuleForm::CreateParams(TCreateParams &Params) ;
public:		// User declarations
	virtual bool OnInit();
	virtual dcmapLRESULT OnMessage(dcmapMSG MSG,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender);
	virtual int OnScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);

	__fastcall TBaseModuleForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBaseModuleForm *BaseModuleForm;
//---------------------------------------------------------------------------
#endif
