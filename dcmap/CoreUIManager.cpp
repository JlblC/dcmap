//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "WebProc.h"

#include "CoreUIManager.h"
#include "main.h"

#include "acntUtils.hpp"
#include "ListEdit.h"
#include "DataEdit.h"
#include "Utils.h"
#include "BaseModule.h"
#include "Interface.h"
#include "Progress.h"
#include "UIDialogForm.h"
#include "PasswordDialog.h"
#include "UIWorkspace.h"
#include "Localize.h"

#include "api/dcmap_script_helper.h"
#include "Input.h"
#include "UISystem.h"

TCoreUIManager g_CoreUIManager;

DCMAP_SCRIPT_REFLECTION(TCoreUIManager)
{
	DCMAP_SCRIPT_FUNC(ErrorMsg);
	DCMAP_SCRIPT_FUNC(BringOnTop);
}

/*
int DCMAPAPI TDCMapMenuItem::ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
{
	if(type == DCMEXEC_QUERY)
	{
		if(String(L"Check") == command) return  DCMEXEC_SET | DCMEXEC_GET;
		if(String(L"Checked") == command) return  DCMEXEC_SET | DCMEXEC_GET;
		if(String(L"Enabled") == command) return  DCMEXEC_SET | DCMEXEC_GET;
		if(String(L"Visible") == command) return  DCMEXEC_SET | DCMEXEC_GET;

		return  DCMEXEC_FAIL;
	}
	if(!m_pMenuItem)return  DCMEXEC_FAIL;

	if(type == DCMEXEC_SET)
	{
		if(String(L"Check") == command)
		{
			m_pMenuItem->AutoCheck = pResult->GetNumber();
			return DCMEXEC_SUCCESS;
		}
		else if(String(L"Checked") == command)
		{
			m_pMenuItem->Checked = pResult->GetNumber();
			return DCMEXEC_SUCCESS;
		}
		else if(String(L"Enabled") == command)
		{
			m_pMenuItem->Enabled = pResult->GetNumber() != 0;
			return DCMEXEC_SUCCESS;
		}
		else if(String(L"Visible") == command)
		{
			m_pMenuItem->Visible = pResult->GetNumber();
			return DCMEXEC_SUCCESS;
		}
	}
	else if(type == DCMEXEC_GET)
	{
		if(String(L"Check") == command)
		{
			pResult->AddNumber(m_pMenuItem->AutoCheck);
			return DCMEXEC_SUCCESS;
		}
		else if(String(L"Checked") == command)
		{
			pResult->AddNumber(m_pMenuItem->Checked);
			return DCMEXEC_SUCCESS;
		}
		else if(String(L"Enabled") == command)
		{
			pResult->AddNumber(m_pMenuItem->Enabled);
			return DCMEXEC_SUCCESS;
		}
		else if(String(L"Visible") == command)
		{
			pResult->AddNumber(m_pMenuItem->Visible);
			return DCMEXEC_SUCCESS;
		}
	}
	return  DCMEXEC_FAIL;
}
*/

//---------------------------------------------------------------------------

TDCMapUIContext* TCoreUIManager::GetContextEx(AnsiString name)
{
	Contextes::iterator it = m_Contextes.find(name);
	if(it == m_Contextes.end())
	{
		 m_Contextes[name] = new TDCMapUIContext;
		return m_Contextes[name];
	}
	else return it->second;
}

IDCMapUIContext* DCMAPAPI TCoreUIManager::GetContext(dcmapCSTR name)
{
	if(!name || !name[0])return 0;
	return GetContextEx(name);
}

IDCMapUIMenu* DCMAPAPI TCoreUIManager::CreatePopupMenu(IDCMapModule* pOwner,int id)
{
    TDCMapPopupMenu* pmenu = new TDCMapPopupMenu;
    pmenu->InitItem(MainForm,pOwner,id);
    return pmenu;
}

dcmapWCSTR DCMAPAPI TCoreUIManager::OpenDlg(dcmapWCSTR Dir,dcmapWCSTR Filters,int FilterIndex)
{
	InterfaceModule->OpenDialog->DefaultExt = L"";
	if(Filters&&Filters[0]) InterfaceModule->OpenDialog->Filter = String(Filters)+L"|";
		else  InterfaceModule->OpenDialog->Filter = L"";
	InterfaceModule->OpenDialog->Filter += L"Все файлы|*.*";

	if(Dir) InterfaceModule->OpenDialog->InitialDir = Dir;
	else  InterfaceModule->OpenDialog->InitialDir = L"";

	if(InterfaceModule->OpenDialog->Execute())
	{
		m_strLastFileName = InterfaceModule->OpenDialog->FileName;
		return m_strLastFileName.w_str();
	}
	return 0;
}

dcmapWCSTR DCMAPAPI TCoreUIManager::SaveDlg(dcmapWCSTR Ext,dcmapWCSTR Dir,dcmapWCSTR Filters,int FilterIndex)
{
	if(Ext)InterfaceModule->SaveDialog->DefaultExt = Ext;
	if(Filters&&Filters[0]) InterfaceModule->SaveDialog->Filter = String(Filters)+L"|";
		else  InterfaceModule->SaveDialog->Filter = L"";
	InterfaceModule->SaveDialog->Filter += L"Все файлы|*.*";

	if(Dir) InterfaceModule->SaveDialog->InitialDir = Dir;
	else  InterfaceModule->SaveDialog->InitialDir = L"";

	if(InterfaceModule->SaveDialog->Execute())
	{
		m_strLastFileName = InterfaceModule->SaveDialog->FileName;
		return m_strLastFileName.w_str();
	}
	return 0;
}

//---------------------------------------------------------------------------
dcmapWCSTR DCMAPAPI TCoreUIManager::FileOpenDialog(dcmapWCSTR DefaultExt,
								dcmapWCSTR Filter,
								int FilterIndex,
								dcmapWCSTR InitialDir,int iOptions)
{
	if(DefaultExt) InterfaceModule->OpenDialog->DefaultExt = DefaultExt;
	else  InterfaceModule->OpenDialog->DefaultExt = L"";
	if(Filter) InterfaceModule->OpenDialog->Filter = Filter;
	else  InterfaceModule->OpenDialog->Filter = L"";
	InterfaceModule->OpenDialog->Filter += L"|Все файлы|*.*";

	if(InitialDir) InterfaceModule->OpenDialog->InitialDir = InitialDir;
	else  InterfaceModule->OpenDialog->InitialDir = L"";

	if(InterfaceModule->OpenDialog->Execute())
	{
		m_strLastFileName = InterfaceModule->OpenDialog->FileName;
		return m_strLastFileName.w_str();
	}
	return 0;
}

dcmapWCSTR DCMAPAPI TCoreUIManager::SelectPath(int iOptions,dcmapWCSTR InitialDir)
{
	if(InitialDir) InterfaceModule->PathDialog->Path = InitialDir;
	else InterfaceModule->PathDialog->Path = L"";

	if(InterfaceModule->PathDialog->Execute())
	{
		m_strLastFileName = InterfaceModule->PathDialog->Path;
		return m_strLastFileName.w_str();
	}
	return 0;
}

int DCMAPAPI TCoreUIManager::MsgBox(dcmapWCSTR Text,
								dcmapWCSTR Caption,int Flags)
{
    Dialogs::TMsgDlgButtons Buttons;
	if(Flags & MB_YESNO)
    {
     	Buttons << mbYes << mbNo;
    }
    else
    {
    	Buttons << mbOK;
    }

    Dialogs::TMsgDlgType DlgType    = mtInformation;

    if(Flags & MB_ICONERROR) DlgType  = mtError;

    int r =  sMessageDlg(Caption,Text,DlgType,Buttons,0);

	return r;
}

void DCMAPAPI TCoreUIManager::ErrorMsg(dcmapWCSTR Text)
{
    Dialogs::TMsgDlgButtons Buttons;
    Buttons << mbOK;
	sMessageDlg(g_pLocalStrings->GetStringW("ErrorDlgCaption"),Text,mtError,Buttons,0);
}

//IDCMapUIMenuItem* DCMAPAPI AddMenuItem(TMenuItem* pParent,const char* MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id);

IDCMapUIMenuItem* DCMAPAPI TCoreUIManager::AddMenuItem(const char* MenuPath,
                             dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id)
{
	TSysCharSet Delims;
	Delims << '/';
	String Name = ExtractWord(1,MenuPath,Delims);

	TDCMapMenuItem* pItem = new TDCMapMenuItem;
	pItem->SetLister(&m_MenuItems);

	pItem->InitItem(MenuPath,MenuCaption,pOwner,id);

	_foreach(SetFrames,FrameForms,it)
	{
		pItem->CreateMenuItem(*it,(*it)->FrameMenu->Items);
	}


    return pItem;
	/*if(SameText(Name,"Popup"))
	{

		return ::AddMenuItem(MainForm->MainPopupMenu->Items,
		MenuPath+Name.Length(),
		MenuCaption,pOwner,id);
	}
	else */
	//{
	//	return ::AddMenuItem(MainForm->MainMenu->Items,MenuPath,MenuCaption,pOwner,id);
	//}
}

bool DCMAPAPI TCoreUIManager::OpenListTableEdit(const char* table,
                                        DataReference const* Ref,
                                        DataReference const* Parent,
                                        IDCMapSimpleDataFilter* Filter)
{
	if(!table || !table[0])return false;
	bool Result = true;

    IDCMapDataStorage* Storage=0;
    IDCMapDataSource* src = GetDCMapCore()->GetDataSource(table);

    String PClass = (String)"PListEditPropPage_"+table;
    if(Ref)
    {
	  TDataEditForm* pForm = new TDataEditForm(SysUI.ActiveFrame());
      pForm->PluginClassName = PClass;
      pForm->TableName = table;
      if(pForm->Init(src))
      {
        pForm->Select(*Ref);
        TModalResult r = pForm->ShowModal();

        if(r == mrCancel) Result = false;

        //MainForm->sSkinManager->UpdateSkin();
      }
      delete pForm;
    }
    else
    {
	  TListEditForm* EditForm = new  TListEditForm(SysUI.ActiveFrame());
	  EditForm->PluginClassName = PClass;
      EditForm->TableName = table;
      EditForm->Filter = Filter;
      if(EditForm->Init(src,Parent))
      {
        if(Ref)EditForm->Select(*Ref);
        EditForm->ShowModal();

        //MainForm->sSkinManager->UpdateSkin();
      }
	  delete EditForm;
    }

    return Result;
}

dcmapWCSTR DCMAPAPI TCoreUIManager::InputBox(dcmapWCSTR Text,dcmapWCSTR Caption,dcmapWCSTR Value)
{
	static UnicodeString Result;
	String AResult = Value?Value:L"";
	if(::InputQuery(Caption?Caption:L"DCMap",Text?Text:L"",AResult))
	{
		Result = AResult;
		return Result.w_str();
	}
	return 0;
}

dcmapWCSTR DCMAPAPI TCoreUIManager::InputPassword(dcmapWCSTR Text,dcmapWCSTR Caption)
{
	static UnicodeString Result;

	if(TPasswordDialogForm::Execute(Caption?Caption:L"DCMap",Text?Text:L"Enter Password",Result))
	{
		return Result.w_str();
	}
	return 0;
}


IDCMapLocalStrings* TCoreUIManager::RegisterUIItem(TDCMapBasicComponentItem* pItem,IDCMapModule* Module)
{
	MapModuleItems::iterator it = m_ModuleItems.find(Module);
	ModuleItems* Items;
	IDCMapLocalStrings* LocalStrings;
	if(it != m_ModuleItems.end())
	{
		Items = it->second.Items;
		LocalStrings = it->second.LocalStrings;
	}
    else
	{
		Items = new ModuleItems;
		LocalStrings = g_pLocalStrings;
		
		ModuleInfo minf;
		minf.Items = Items;
		minf.LocalStrings = LocalStrings;
		m_ModuleItems[Module] = minf;
    }
	Items->insert(pItem);

	return LocalStrings;
}

void TCoreUIManager::UnregisterUIItem(TDCMapBasicComponentItem* pItem,IDCMapModule* Module)
{
	MapModuleItems::iterator it = m_ModuleItems.find(Module);
    ModuleItems* Items;
    if(it != m_ModuleItems.end()) Items = it->second.Items;
    else return;  // error - item is'nt registereed
	Items->erase(pItem);
}

void DCMAPAPI TCoreUIManager::DestroyModuleItems(IDCMapModule* Module)
{
	if(!Module)
    	return;
        
	MapModuleItems::iterator it = m_ModuleItems.find(Module);
    ModuleItems* Items;
    if(it != m_ModuleItems.end()) Items = it->second.Items;
    else return;  // ok, module isn't registered
    m_ModuleItems.erase(it);

    for(ModuleItems::iterator it = Items->begin();it != Items->end();++it)
    {
		delete *it;
	}
    delete Items;
}

bool DCMAPAPI TCoreUIManager::ClipboardPut(const char* string)
{
    ::ClipboardPutW((String)string);
    return true;
}

bool DCMAPAPI TCoreUIManager::ClipboardPutW(dcmapWCSTR string)
{
    ::ClipboardPutW((UnicodeString)string);
    return true;
}
const char* DCMAPAPI TCoreUIManager::ClipboardGet()
{
	static AnsiString sStr;
    sStr = ::ClipboardGet();
	return sStr.c_str();
}

dcmapWCSTR DCMAPAPI TCoreUIManager::ClipboardGetW()
{
	static UnicodeString sStr;
    sStr = ::ClipboardGetW();
    return sStr.w_str();
}

static LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	g_CoreUIManager.HideHint();

    return CallNextHookEx(0,nCode,wParam,lParam);
}

static LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
    switch(wParam)
    {
    	case WM_MOUSEMOVE:

            break;
        default:
			g_CoreUIManager.HideHint();
            break;
    }
    return CallNextHookEx(0,nCode,wParam,lParam);
}

static HHOOK s_hHook1=0;
static HHOOK s_hHook2=0;

static bool m_bHint=false;

void DCMAPAPI TCoreUIManager::HideHint()
{
	if(m_bHint)
	{
	   InterfaceModule->Hints->HideHint();

		//MainForm->HintManager->HideHint();
        m_bHint = false;

        UnhookWindowsHookEx(s_hHook1);
        UnhookWindowsHookEx(s_hHook2);
    }
}

void DCMAPAPI TCoreUIManager::ShowHint(RECT const& area,dcmapWCSTR string)
{
	//if(!Application->Active)return;

	HideHint();

	if(!string || !string[0])return;

    TPoint pt1 = Point(area.left,area.top);
    TPoint pt2 = Point(area.right,area.bottom);

	ScreenToClient(MainForm->sTabs->Handle,&pt1);
    ScreenToClient(MainForm->sTabs->Handle,&pt2);

    MainForm->HintPlace->Left = pt1.x;
    MainForm->HintPlace->Top = pt1.y;

    MainForm->HintPlace->Width = pt2.x-pt1.x;
    MainForm->HintPlace->Height = pt2.y-pt1.y;

    MainForm->HintPlace->Top = pt1.y+=MainForm->HintPlace->Height/2+12;

    MainForm->HintPlace->Hint = string;

    //MainForm->HintPlace->Width = 200;
    //MainForm->HintPlace->Height = 200;

	InterfaceModule->Hints->HideHint();
	InterfaceModule->Hints->ShowHint(MainForm->HintPlace,string);

    m_bHint = true;


    s_hHook1 = SetWindowsHook(WH_KEYBOARD,(HOOKPROC)KeyboardProc);
    s_hHook2 = SetWindowsHook(WH_MOUSE,(HOOKPROC)MouseProc);
}

void DCMAPAPI TCoreUIManager::UpdateSkins()
{
	//MainForm->sSkinManager->UpdateSkin();
}

bool DCMAPAPI TCoreUIManager::GameLogin(dcmapWCSTR Login,dcmapWCSTR Pass,bool free,IDCMapProcessRecordReseiver *pRec)
{
	if(!WebModule)WebModule = new TWebModule(Application);
	WebModule->m_pRec = pRec;
	bool r= WebModule->DoLogin(Login,Pass,free);
	WebModule->m_pRec = 0;
	return b;
}

bool DCMAPAPI TCoreUIManager::GameLogout(IDCMapProcessRecordReseiver *pRec)
{
	if(!WebModule)WebModule = new TWebModule(Application);
	WebModule->m_pRec = pRec;
	bool r= WebModule->DoLogout();
	WebModule->m_pRec = 0;
	return b;
}

static UnicodeString WebTempStr;

dcmapWCSTR DCMAPAPI TCoreUIManager::WebGet(dcmapWCSTR URL,IDCMapProcessRecordReseiver *pRec,int CodePage)
{
	if(!WebModule)WebModule = new TWebModule(Application);
	WebModule->m_pRec = pRec;
	WebTempStr = WebModule->WebGet(URL,CodePage);
	WebModule->m_pRec = 0;
	return WebTempStr.w_str();
}

dcmapWCSTR DCMAPAPI TCoreUIManager::WebPost(dcmapWCSTR URL,dcmapWCSTR PostData,IDCMapProcessRecordReseiver *pRec,int CodePage)
{
	if(!WebModule)WebModule = new TWebModule(Application);
	WebModule->m_pRec = pRec;
	WebTempStr = WebModule->WebPost(URL,PostData,CodePage);
	WebModule->m_pRec = 0;
	return WebTempStr.w_str();
}

dcmapWCSTR DCMAPAPI TCoreUIManager::GameGet(dcmapWCSTR URL,IDCMapProcessRecordReseiver *pRec)
{
	if(!WebModule)WebModule = new TWebModule(Application);
	WebModule->m_pRec = pRec;
	WebTempStr = WebModule->GameGet(URL);
	WebModule->m_pRec = 0;
	return WebTempStr.w_str();
}

dcmapWCSTR DCMAPAPI TCoreUIManager::GamePost(dcmapWCSTR URL,dcmapWCSTR PostData,IDCMapProcessRecordReseiver *pRec)
{
	if(!WebModule)WebModule = new TWebModule(Application);
	WebModule->m_pRec = pRec;
	WebTempStr = WebModule->GamePost(URL,PostData);
	WebModule->m_pRec = 0;
	return WebTempStr.w_str();
}

void DCMAPAPI TCoreUIManager::CloseMainWnd()
{
    MainForm->Close();
}

IDCMapProcessRecordReseiver * DCMAPAPI TCoreUIManager::GetConsoleFidder()
{
	if(!ProgressForm)ProgressForm = new TProgressForm(Application);
	return ProgressForm->Fidder();
}

void DCMAPAPI TCoreUIManager::BringOnTop()
{
	MainForm->FormStyle = fsStayOnTop;
	Application->BringToFront();
	SetForegroundWindow(MainForm->Handle);
	SetActiveWindow(MainForm->Handle);
	MainForm->FormStyle = fsNormal;
}

IDCMapUIItem* DCMAPAPI TCoreUIManager::CreateUIModule(dcmapCSTR name,IDCMapModule* pOwner, int id)
{
	IDCMapObject* pObj = GetDCMapSys()->CreateObject("UI",name);
	if(!pObj)return 0;
	IDCMapUIItem* item = pObj->Cast<IDCMapUIItem>();

	if(!item || !item->Init(GetDCMapCore(),0) || !item->InitUI(pOwner,id) )
	{
		GetDCMapSys()->DestroyObject(pObj);
		return 0;
	}
	return item;
}

int DCMAPAPI TCoreUIManager::ShowModal(IDCMapModule* pModule,dcmapWCSTR Caption, int Flags)
{
	TDialogForm* pForm = new TDialogForm(MainForm);
	int mr=-1;
	if(pForm->Setup(pModule,Caption,Flags))
	{
		mr = pForm->ShowModal();
	}
	delete pForm;
	return mr;
}

int DCMAPAPI TCoreUIManager::InputDialog(dcmapCSTR Module,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
{
	return TInputDialog::Query(Module,pParams,pResult);
}

void TCoreUIManager::AddFrameForm(TBaseFrameForm* form)
{
	FrameForms.insert(form);
	_foreach(MenuItems,m_MenuItems,it)
	{
		(*it)->CreateMenuItem(form,form->FrameMenu->Items);
	}
}

#pragma package(smart_init)
