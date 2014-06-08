//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifndef CoreUIManagerH
#define CoreUIManagerH
//---------------------------------------------------------------------------
#include "dcmapsyslib.h"
#include "UIMenu.h"



class TCoreUIManager : public IDCMapCoreUIEx
{
	friend class TDCMapBasicComponentItem;

	// keep assitiation from ui items to modules
	typedef std::set<TDCMapBasicComponentItem*> ModuleItems;
	struct ModuleInfo
	{
		ModuleItems* Items;
		IDCMapLocalStrings* LocalStrings;
	};
	typedef std::map<IDCMapModule*,ModuleInfo> MapModuleItems;

	typedef std::map<AnsiString,TDCMapUIContext*> Contextes;

	typedef std::list<TDCMapMenuItem*> MenuItems;
	MenuItems m_MenuItems;

	Contextes m_Contextes;
        MapModuleItems m_ModuleItems;

	IDCMapLocalStrings* RegisterUIItem(TDCMapBasicComponentItem* pItems,IDCMapModule* Module);
    void UnregisterUIItem(TDCMapBasicComponentItem* pItems,IDCMapModule* Module);

    String m_strLastFileName;

    typedef std::set<TBaseFrameForm*> SetFrames;
    SetFrames FrameForms;
public:
	void AddFrameForm(TBaseFrameForm* form);

	void DCMAPAPI Destroy(){};
        int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);


        void DCMAPAPI DestroyModuleItems(IDCMapModule* Module);
	// General create commands
	IDCMapUIMenu* DCMAPAPI CreatePopupMenu(IDCMapModule* pOwner,int id);

	IDCMapUIMenuItem* DCMAPAPI AddMenuItem(const char* MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id);

	dcmapWCSTR DCMAPAPI FileOpenDialog(dcmapWCSTR DefaultExt,
								dcmapWCSTR Filter,
								int FilterIndex,
								dcmapWCSTR InitialDir,int iOptions);

	dcmapWCSTR DCMAPAPI SelectPath(int iOptions=0,dcmapWCSTR InitialDir=0);

	dcmapWCSTR DCMAPAPI GetFilename(){return m_strLastFileName.w_str();}
	void DCMAPAPI ErrorMsg(dcmapWCSTR Text);

	dcmapWCSTR DCMAPAPI OpenDlg(dcmapWCSTR Dir,dcmapWCSTR Filters,int FilterIndex);
	dcmapWCSTR DCMAPAPI SaveDlg(dcmapWCSTR Ext,dcmapWCSTR Dir,dcmapWCSTR Filters,int FilterIndex);

	dcmapWCSTR DCMAPAPI InputBox(dcmapWCSTR Text,dcmapWCSTR Caption=0,dcmapWCSTR Value=0);

	bool DCMAPAPI OpenListTableEdit(const char* table, DataReference const* Ref=0,DataReference const* Parent=0,IDCMapSimpleDataFilter* Filter=0);

    bool DCMAPAPI ClipboardPut(const char* string);
    bool DCMAPAPI ClipboardPutW(dcmapWCSTR string);

	void DCMAPAPI UpdateSkins();

	int DCMAPAPI MsgBox(dcmapWCSTR Text,dcmapWCSTR Caption,int Flags);

	dcmapCSTR DCMAPAPI ClipboardGet();
	dcmapWCSTR DCMAPAPI ClipboardGetW();

	void DCMAPAPI ShowHint(RECT const& area,dcmapWCSTR string);
	void DCMAPAPI HideHint();

	dcmapWCSTR DCMAPAPI WebGet(dcmapWCSTR URL,IDCMapProcessRecordReseiver *pRec=0,int CodePage = CP_UTF8);
	dcmapWCSTR DCMAPAPI WebPost(dcmapWCSTR URL,dcmapWCSTR PostData,IDCMapProcessRecordReseiver *pRec=0,int CodePage = CP_UTF8);
	bool DCMAPAPI GameLogin(dcmapWCSTR Login,dcmapWCSTR Pass,bool free,IDCMapProcessRecordReseiver *pRec=0);
	bool DCMAPAPI GameLogout(IDCMapProcessRecordReseiver *pRec=0);
	dcmapWCSTR DCMAPAPI GameGet(dcmapWCSTR URL,IDCMapProcessRecordReseiver *pRec=0);
	dcmapWCSTR DCMAPAPI GamePost(dcmapWCSTR URL,dcmapWCSTR PostData,IDCMapProcessRecordReseiver *pRec=0);

	int DCMAPAPI InputDialog(dcmapCSTR Module,IDCMapVarValue* pParams=0,IDCMapVarValue* pResult=0);

	void DCMAPAPI CloseMainWnd();

	void DCMAPAPI BringOnTop();


	IDCMapProcessRecordReseiver * DCMAPAPI GetConsoleFidder();

	IDCMapUIItem* DCMAPAPI CreateUIModule(dcmapCSTR name,IDCMapModule* pOwner, int id);

	int DCMAPAPI ShowModal(IDCMapModule* pModule,dcmapWCSTR Caption, int Flags);
	dcmapWCSTR DCMAPAPI InputPassword(dcmapWCSTR Text,dcmapWCSTR Caption);

	TDCMapUIContext* GetContextEx(AnsiString name);
	IDCMapUIContext* DCMAPAPI GetContext(dcmapCSTR name);

};
extern TCoreUIManager g_CoreUIManager;
#endif
