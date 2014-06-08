#ifndef dcmapapi_dcmap_ui_h__
#define dcmapapi_dcmap_ui_h__

struct IDCMapCoreUI;
struct IDCMapFrameUI;
struct IDCMapUIItem;
struct IDCMapUIMenuItem;
struct IDCMapUIMenu;
struct IDCMapUIContext;
struct IDCMapSimpleDataFilter;
namespace Forms{class TForm;}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct IDCMapCoreUI : public IDCMapScriptable
{
	virtual int DCMAPAPI MsgBox(dcmapDCSTR Text,dcmapDCSTR Caption,int Flags)=0;
	virtual dcmapDCSTR DCMAPAPI InputBox(dcmapUCSTR Text,dcmapUCSTR Caption=0,dcmapUCSTR Value=0)=0;

	virtual void DCMAPAPI ErrorMsg(dcmapWCSTR Text)=0;

	virtual dcmapDCSTR DCMAPAPI FileOpenDialog(dcmapDCSTR DefaultExt=0,
		dcmapDCSTR Filter=0,int FilterIndex=0,dcmapDCSTR InitialDir=0,int iOptions=0)=0;

	virtual dcmapDCSTR DCMAPAPI SelectPath(int iOptions=0,dcmapDCSTR InitialDir=0)=0;

	virtual dcmapDCSTR DCMAPAPI GetFilename()=0;

	virtual bool DCMAPAPI OpenListTableEdit(dcmapCSTR table, DataReference const* Ref=0,DataReference const* Parent=0,IDCMapSimpleDataFilter* Filter=0)=0;

	virtual IDCMapUIMenuItem* DCMAPAPI AddMenuItem(dcmapCSTR MenuPath,dcmapDCSTR MenuCaption,IDCMapModule* pOwner,int id)=0;

	virtual bool DCMAPAPI ClipboardPut(dcmapCSTR string)=0;
	virtual bool DCMAPAPI ClipboardPutW(dcmapDCSTR string)=0;
	virtual dcmapCSTR DCMAPAPI ClipboardGet()=0;
	virtual dcmapDCSTR DCMAPAPI ClipboardGetW()=0;

	virtual void DCMAPAPI ShowHint(dcmapRECT const& area,dcmapDCSTR string)=0;
	virtual void DCMAPAPI HideHint()=0;

	virtual void DCMAPAPI UpdateSkins()=0;

	virtual dcmapDCSTR DCMAPAPI WebGet(dcmapDCSTR URL,IDCMapProcessRecordReseiver *pRec=0,int CodePage = DCMAP_CP_UTF8)=0;
	virtual dcmapDCSTR DCMAPAPI WebPost(dcmapDCSTR URL,dcmapDCSTR PostData,IDCMapProcessRecordReseiver *pRec=0,int CodePage = DCMAP_CP_UTF8)=0;
	virtual bool DCMAPAPI GameLogin(dcmapDCSTR Login,dcmapDCSTR Pass,bool free,IDCMapProcessRecordReseiver *pRec=0)=0;
	virtual bool DCMAPAPI GameLogout(IDCMapProcessRecordReseiver *pRec=0)=0;
	virtual dcmapDCSTR DCMAPAPI GameGet(dcmapDCSTR URL,IDCMapProcessRecordReseiver *pRec=0)=0;
	virtual dcmapDCSTR DCMAPAPI GamePost(dcmapDCSTR URL,dcmapDCSTR PostData,IDCMapProcessRecordReseiver *pRec=0)=0;

	virtual void DCMAPAPI CloseMainWnd()=0;

	virtual IDCMapProcessRecordReseiver * DCMAPAPI GetConsoleFidder()=0;

	virtual dcmapDCSTR DCMAPAPI OpenDlg(dcmapDCSTR Dir,dcmapDCSTR Filters,int FilterIndex=0)=0;
	virtual dcmapDCSTR DCMAPAPI SaveDlg(dcmapDCSTR Ext,dcmapDCSTR Dir,dcmapDCSTR Filters,int FilterIndex=0)=0;

	virtual void DCMAPAPI BringOnTop()=0;

    virtual IDCMapUIMenu* DCMAPAPI CreatePopupMenu(IDCMapModule* pOwner,int id)=0;

    virtual IDCMapUIItem* DCMAPAPI CreateUIModule(dcmapCSTR name,IDCMapModule* pOwner, int id)=0;

    virtual int DCMAPAPI ShowModal(IDCMapModule* pModule,dcmapDCSTR Caption, int Flags)=0;
	virtual dcmapWCSTR DCMAPAPI InputPassword(dcmapWCSTR Text=0,dcmapWCSTR Caption=0)=0;

	virtual IDCMapUIContext* DCMAPAPI GetContext(dcmapCSTR name)=0;

	virtual	int DCMAPAPI InputDialog(dcmapCSTR Module,IDCMapVarValue* pParams=0,IDCMapVarValue* pResult=0)=0;

};
DCMAP_SPEC_INTERFACE(IDCMapCoreUI);

struct IDCMapFrameUI: public IDCMapScriptable
{
	virtual void DCMAPAPI ShowHint(dcmapRECT const& area,dcmapDCSTR string)=0;
	virtual void DCMAPAPI HideHint()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapFrameUI);

struct IDCMapSysUI: public IDCMapScriptable
{
	virtual IDCMapCoreUI* DCMAPAPI CreateCoreUI(IDCMapCore*)=0;
	virtual IDCMapFrameUI* DCMAPAPI CreateFrameUI(IDCMapFrame*,IDCMapCore*)=0;
	virtual IDCMapWorkspaceUI* DCMAPAPI CreateWorkspaceUI(IDCMapWorkspace*,IDCMapFrame*,IDCMapCore*)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapSysUI);


struct IDCMapWorkspaceUI: public IDCMapScriptable
{
	 virtual void DCMAPAPI ActivateView()=0;

	 virtual IDCMapUIMenuItem* DCMAPAPI AddMenuItem(dcmapCSTR MenuPath,dcmapDCSTR MenuCaption,IDCMapModule* pOwner,int id)=0;
	 virtual IDCMapUIMenuItem* DCMAPAPI AddMenu(dcmapCSTR MenuPath,dcmapDCSTR MenuCaption,IDCMapModule* pOwner,int id)=0;
	 virtual void DCMAPAPI RegisterForm(Forms::TForm* pForm)=0;
	 virtual void DCMAPAPI UnregisterForm(Forms::TForm* pForm)=0;

	 virtual IDCMapFrameUI* DCMAPAPI GetFrameUI()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapWorkspaceUI);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



struct IDCMapUIItem : public IDCMapModule
{
    virtual bool DCMAPAPI InitUI(IDCMapModule* pOwner,int id){return true;};
};
DCMAP_SPEC_INTERFACE(IDCMapUIItem);

struct IDCMapUIMenu : public IDCMapUIItem
{
	virtual void DCMAPAPI Popup()=0;
	virtual IDCMapUIMenuItem* DCMAPAPI AddItem(dcmapCSTR MenuPath,dcmapDCSTR MenuCaption,IDCMapModule* pOwner,int id)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapUIMenu);

struct IDCMapUIMenuItem : public IDCMapUIItem
{
	virtual void DCMAPAPI SetCaption(dcmapWCSTR Str)=0;
	virtual void DCMAPAPI SetShortCut(dcmapWCSTR Str)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapUIMenuItem);

struct IDCMapUIContext : public IDCMapModule
{
	virtual IDCMapUIMenuItem* DCMAPAPI AddMenuItem(const char* MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id)=0;
	virtual bool DCMAPAPI SetMenu(IDCMapUIMenu* pPopup)=0;
	virtual IDCMapVarValue* DCMAPAPI ContextValue()=0;
	virtual int DCMAPAPI Setup()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapUIContext);

struct IDCMapUIContextManager : public IDCMapModule
{
	IDCMapUIContext* GetContext(dcmapCSTR ContextName);
};
DCMAP_SPEC_INTERFACE(IDCMapUIContextManager);


struct IDCMapWidget : public IDCMapUIItem
{
	
};

struct IDCMapPopupWindow : public IDCMapUIItem
{
	virtual void DCMAPAPI SetCaption(dcmapDCSTR Str)=0;
	virtual void DCMAPAPI SetDataReference(dcmapDBREF* pRef)=0;
	virtual void DCMAPAPI Show()=0;
	virtual void DCMAPAPI Hide()=0;
	virtual bool DCMAPAPI Visible()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPopupWindow);


struct IDCMapUIFrameItem : public IDCMapUIItem
{

};
DCMAP_SPEC_INTERFACE(IDCMapUIFrameItem);


struct IDCMapUICustomFrame : public IDCMapUIFrameItem
{

};
DCMAP_SPEC_INTERFACE(IDCMapUICustomFrame);


struct IDCMapUIPanel : public IDCMapUIFrameItem
{

};
DCMAP_SPEC_INTERFACE(IDCMapUIPanel);


struct IDCMapUIHTMLFrame : public IDCMapUIFrameItem
{
    virtual bool DCMAPAPI LoadHTML(dcmapDCSTR pHTML,dcmapDCSTR pbaseURL,IDCMapModule* pProcessModule=0)=0;
	virtual void DCMAPAPI Reload(IDCMapModule* pProcessModule=0)=0;
	virtual void DCMAPAPI Reformat()=0;

    virtual bool DCMAPAPI SetObjectAttribute(dcmapDCSTR szObjID,dcmapDCSTR szAttribute,dcmapDCSTR szValue)=0;
    virtual bool DCMAPAPI SetObjectVisible(dcmapDCSTR szObjID,bool bVisible)=0;

    virtual dcmapDCSTR DCMAPAPI GetObjectAttribute(dcmapDCSTR szObjID,dcmapDCSTR szAttribute)=0;
    virtual bool DCMAPAPI GetObjectVisible(dcmapDCSTR szObjID)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapUIHTMLFrame);




#endif // dcmapapi_dcmap_ui_h__
