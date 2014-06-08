//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef InterfaceH
#define InterfaceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include "acAlphaHints.hpp"
#include <AppEvnts.hpp>
#include "sHintManager.hpp"
#include "acAlphaImageList.hpp"
#include <ExtCtrls.hpp>
#include "sSkinManager.hpp"
#include "sDialogs.hpp"
#include <Dialogs.hpp>
#include <map>
#include "dcmapsyslib.h"
//---------------------------------------------------------------------------
class TInterfaceModule : public TDataModule
{
	friend class CInterfaceService;
__published:	// IDE-managed Components
	TImageList *RsrsImageList;
	TsHintManager *Hints;
	TsAlphaImageList *MenuImageList;
	TsAlphaImageList *FleetIcons;
	TsAlphaImageList *IconsImageList;
	TsAlphaImageList *RsrsImageList2;
	TsAlphaImageList *FleetStateIcons;
	TsSkinManager *SkinManager;
	TsOpenDialog *OpenDialog;
	TsSaveDialog *SaveDialog;
	TsPathDialog *PathDialog;
	void __fastcall HintsShowHint(UnicodeString &HintStr, bool &CanShow, THintInfo &HintInfo,
		  TFrame *&Frame);
	void __fastcall DataModuleCreate(TObject *Sender);
	void __fastcall SkinManagerAfterChange(TObject *Sender);
private:	// User declarations
	IDCMapSys* m_pSys;
	IDCMapCoreEx* m_pCore;
private:	// User declarations
	typedef std::map<String,int> ImgIndexes;
	typedef std::vector<TPNGGraphic*> VecPngImages;

	ImgIndexes m_menuIndexes;
	VecPngImages PngImages;

	ImgIndexes m_fleetsIndexes;

	int m_iIconUpdate;
	bool MenuIconUpdates;
	bool m_bAero;


	bool SkinProviderConf(TComponent* Cnt,IDCMapLocalStrings* Strs);

	void LoadConfig();

	bool m_bInitComplete;

public:	
	bool m_bEnableAnimations;
	bool m_bAeroBorders;
public:		// User declarations
	__fastcall TInterfaceModule(TComponent* Owner);
	void LocalizeContainer(TControl* Base,IDCMapLocalStrings* Strs=0);
	void LocalizeComponent(TComponent* Cnt,IDCMapLocalStrings* Strs=0);

	String StdFontFace;
	String ScaledFontFace;
	int GetMenuImage(String name);
	int GetFleetImage(String name);

	void BeginIconUpdate();
	void EndIconUpdate();

	void HookMenu(TPopupMenu* menu);
	void HookMenu(TMenuItem* menu);

	void EnableAnimations();

	TPNGGraphic* MenuImage(int n){return PngImages[n];}


};

class CInterfaceService : public IDCMapModule
{
public:
	CInterfaceService();
	int   DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender);;
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);
};

//---------------------------------------------------------------------------
extern PACKAGE TInterfaceModule *InterfaceModule;
//---------------------------------------------------------------------------
#endif
