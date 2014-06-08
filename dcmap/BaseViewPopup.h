//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef BaseViewPopupH
#define BaseViewPopupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sSkinProvider.hpp"
#include "dcmapsyslib.h"

#include "BaseFormModule.h"

class CBaseFormService;
//---------------------------------------------------------------------------
class TBaseViewPopupForm : public TBaseModuleForm
{
	typedef  TBaseModuleForm  Inherited;

	friend class CBaseFormService;
__published:	// IDE-managed Components
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
protected:
	bool SetupInstance(IDCMapCore* pCore,IDCMapWorkspace* pWorkspace);
	void __fastcall OnClose();
public:		// User declarations
	__fastcall TBaseViewPopupForm(TComponent* Owner);
	__fastcall ~TBaseViewPopupForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TBaseViewPopupForm *BaseViewPopupForm;
//---------------------------------------------------------------------------

class CBaseFormService : public IDCMapModule
{
	TBaseViewPopupForm* m_pInstance;
protected:
	IDCMapWorkspace* m_pWorkspace;
	IDCMapCore* m_pCore;
protected:
	virtual TBaseViewPopupForm* CreateInstance(TComponent* Owner)=0;
	TBaseViewPopupForm* CreateForm(TComponent *Owner=0);

	void Show();
	void Hide();
	void ShowModal();

	TBaseViewPopupForm* GetInstance(){if(!m_pInstance)m_pInstance = CreateForm();return m_pInstance;}
public:
	CBaseFormService();
	int   DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);;
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
};

template<class FormClass>
class CBasicFormService : public CBaseFormService
{
protected:
	virtual TBaseViewPopupForm* CreateInstance(TComponent* Owner)
	{
		return new FormClass(Owner);
	}
	FormClass* GetInstance(){return (FormClass*)CBaseFormService::GetInstance();}
};






#endif
