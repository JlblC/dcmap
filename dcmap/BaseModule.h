//---------------------------------------------------------------------------

#ifndef BaseModuleH
#define BaseModuleH
#include <Classes.hpp>
#include "sFrameAdapter.hpp"
#include <vector>
#include "dcmapbase.h"
#include "sFrameAdapter.hpp"

#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include <TntForms.hpp>
#include "sFrameAdapter.hpp"

#include "CoreUIManager.h"

class TDataRefObject : public TObject
{
public:
	DCMapDataReference ref;
	TDataRefObject(){};
	TDataRefObject(DCMapDataReference const& _ref):ref(_ref){};
	TDataRefObject(DCMapDataReference const* pref){if(pref)ref = *pref;};
};

class TDCMapDragObject: public TDragObjectEx
{
public:
	virtual IDCMapDragObject* Interface()=0;
	virtual IDCMapModule* Module(){return 0;};
};

class TBaseModuleFramePanel;

class TBaseModuleFrame : public TFrame
{
	typedef TFrame Inherited;
friend class TBaseModuleFramePanel;
__published:	// IDE-managed Components
	TsFrameAdapter *sFrameAdapter;
    void __fastcall ComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall FrameResize(TObject *Sender);
private:
    bool m_bChanged;
    void __fastcall SetChanged(bool bChanged);
	int  __fastcall GetHeight(){return Inherited::Height;}
    void  __fastcall SetHeight(int h);
protected:
    virtual bool OnInit();
    virtual void OnCreate();
	virtual void OnShow();
    virtual void OnOk();
    virtual void OnCansel();
    virtual void OnApply();
    virtual void OnSetFocus();
	virtual bool OnDataTest();
    virtual void LoadConfig(DCMapConfigWrapper cfg);
    virtual void SaveConfig(DCMapConfigWrapper cfg);
	virtual void OnChnageSkin(){};
	virtual void SetText(UnicodeString Text){};
	virtual UnicodeString GetText(){return UnicodeString();};

	virtual int OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	virtual int OnScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);

	IDCMapCore*      m_pCore;
    IDCMapWorkspace* m_pWorkspace;
    bool OnShowCalled;
protected:	// User declarations
	void SetNewKeyValue(String val);
	TBaseModuleFramePanel* m_pPanelInterface;

	void ApplyChanges();
	void UpdateList();

	__property bool Changed = {read=m_bChanged,write=SetChanged};
	virtual void* DCMAPAPI GetInterface(DCMapInterfaceID idInterface);

//    String m_strCaption;
public:		// User declarations
	__property int Height = {read=GetHeight,write=SetHeight};
	TBaseModuleFramePanel* Module(){return m_pPanelInterface;}
	__fastcall TBaseModuleFrame(TComponent* Owner);
    UnicodeString Caption;
    int  ImageIndex;
};

class TBaseModuleFramePanel : public IDCMapVCLPanel, public TDCMapBasicComponentItem
{
	TBaseModuleFrame* m_pFrame;
protected:
	virtual TBaseModuleFrame* DoCreateFrame()=0;
 	IDCMapSys*       m_pSys;
	IDCMapCore*      m_pCore;
	IDCMapWorkspace* m_pWorkspace;
public:
	AnsiString ModuleName;
	AnsiString ModuleClass;
public:
	TBaseModuleFramePanel();
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
	void DCMAPAPI Destroy();
    TFrame* DCMAPAPI CreateFrame();
	void DCMAPAPI DestroyFrame(TFrame* frame);

	bool DCMAPAPI OnCreate(DCMapObjectCreationParams const* params);
	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);

	UnicodeString DCMAPAPI GetCaption();
	int DCMAPAPI ImageIndex();
	int DCMAPAPI OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	IDCMapModule* DCMAPAPI GetFinalModule();
	void DCMAPAPI OnShow();
	void DCMAPAPI DoApply();
	void DCMAPAPI DoOk();
	void DCMAPAPI DoCansel();
	void DCMAPAPI SetFocus();
	bool DCMAPAPI DoTestData();
	void DCMAPAPI LoadConfig(IDCMapConfig* pConfig);
	void DCMAPAPI SaveConfig(IDCMapConfig* pConfig);
	bool DCMAPAPI InitUI(IDCMapModule* pOwner,int id);
	void DCMAPAPI SetSkinSection(String Section);
	void DCMAPAPI SetText(UnicodeString Text);
	int DCMAPAPI ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);

    TMainMenu* DCMAPAPI GetFrameMenu();

	int SendUICommand(int Msg,dcmapLPARAM Param)
	{
		return m_pOwner->OnMessage(Msg,m_iID,Param,this);
	}

	int SendUICommand(dcmapWCSTR Param=0)
	{
		return m_pOwner->OnMessage(DCMM_UI_COMMAND,m_iID,(dcmapLPARAM)Param,this);
	}

};

extern IDCMapSys* g_pSys;
extern IDCMapLocalStrings* g_pLocalStrings;

#define LSTR(x) (g_pLocalStrings->GetString(#x))
#define LSTRW(x) (g_pLocalStrings->GetStringW(#x))
#define LSTRSECTION(x) (g_pLocalStrings->SelectSection(#x))
#define SLSTR(x) (g_pLocalStrings->GetSectionString(#x))
#define SLSTRW(x) (g_pLocalStrings->GetSectionStringW(#x))


class TRegReserver;
extern TRegReserver* g_pRegReserver;
class TRegReserver
{
	struct Treg
    {
    	char const* RegClass;
        char const* RegName;
        char const* Param;
        dcmapCreateCallback pCreateCallback;
        void* pUserData;
    };
    std::vector<Treg> m_vecRegs;
    void RegisterModules(IDCMapSys* pSys);
public:
	static void RegisterPlugin(char const* RegClass,char const* RegName,char const* Param,
    			dcmapCreateCallback pCreateCallback,void* pUserData);
    friend void RegisterModules(IDCMapSys* pSys);
};
extern TRegReserver* g_pRegReserver;



template<class Frame>
struct CFramePanelWrapper : public TBaseModuleFramePanel
{
  typedef CFramePanelWrapper<Frame> self;

  TBaseModuleFrame* DoCreateFrame(){return new Frame(0);}
  static IDCMapObject* DCMAPAPI CreateCallback(DCMapObjectCreationParams const* pUserData)
  {return new self();}
  CFramePanelWrapper(){}
  CFramePanelWrapper(char const* RegClass,char const* RegName,char const* Param=0)
  {
      TRegReserver::RegisterPlugin(RegClass,RegName,Param,CreateCallback,0);
  }
};

template<class Module>
struct CAutoReg
{
  static IDCMapObject* DCMAPAPI CreateCallback(DCMapObjectCreationParams const* pUserData)
  {
	Module* pModule = new Module();
	if(!pModule->OnCreate(pUserData))
	{
		delete pModule;
		return 0;
	}
	return pModule;
  }
  CAutoReg(char const* RegClass,char const* RegName,char const* Param=0)
  {
	  TRegReserver::RegisterPlugin(RegClass,RegName,Param,CreateCallback,0);
  }
};

//---------------------------------------------------------------------------
extern PACKAGE TBaseModuleFrame *BaseModuleFrame;
//---------------------------------------------------------------------------
#endif
