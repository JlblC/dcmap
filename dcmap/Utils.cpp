//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Utils.h"
#include "main.h"
#include <Clipbrd.hpp>
#include <Registry.hpp>
#include "BaseModule.h"

//---------------------------------------------------------------------------

extern IDCMapSys* g_pSys;
extern IDCMapLocalStrings* g_pLocalStrings;


TStringList* g_pCustomColors = new TStringList;

/*
WINBASEAPI int WINAPI
WideCharToMultiByte(
	__in UINT     CodePage,
	__in DWORD    dwFlags,
	__in_opt LPCWSTR  lpWideCharStr,
	__in int      cchWideChar,
	__out_bcount_opt(cbMultiByte) LPSTR   lpMultiByteStr,
	__in int      cbMultiByte,
	__in_opt LPCSTR   lpDefaultChar,
	__out_opt LPBOOL  lpUsedDefaultChar)
{
	if(CodePage != CP_UTF8)
	{
		CodePage = -1;
	}
	return dcmapWideCharToMultiByte(CodePage,dwFlags,lpWideCharStr,cchWideChar,
		lpMultiByteStr,cbMultiByte,lpDefaultChar,lpUsedDefaultChar);
}

WINBASEAPI int WINAPI
MultiByteToWideChar(
	__in UINT     CodePage,
	__in DWORD    dwFlags,
	__in LPCSTR   lpMultiByteStr,
	__in int      cbMultiByte,
	LPWSTR  lpWideCharStr,
	__in int      cchWideChar)
{
	if(CodePage != CP_UTF8)
	{
		CodePage = -1;
	}
	return dcmapMultiByteToWideChar(CodePage,dwFlags,
			lpMultiByteStr,cbMultiByte,lpWideCharStr,cchWideChar);
}
*/

/*
WINBASEAPI HFONT WINAPI
CreateFontIndirectA(CONST LOGFONTA* lplf)
{
	return dcmapCreateFontIndirectA(lplf);
}
*/

#include "windows.h"

AnsiString WideToString(UnicodeString const& wstr,int CodePage)
{
	if(CodePage<1) CodePage = CP_UTF8;

	int wlen = wstr.Length();
	int len = dcmapWideCharToMultiByte(CodePage,0,wstr.w_str(),wlen,0,0,0,0);
	AnsiString str;
	str.SetLength(len);
	dcmapWideCharToMultiByte(CodePage,0,wstr.w_str(),wlen,(char*)str.data(),len,0,0);
	return str;
}

UnicodeString ToWide(char * str,int len,int CodePage)
{
	if(CodePage<1) CodePage = CP_UTF8;

	int wlen = dcmapMultiByteToWideChar(
      CodePage,         // code page
      0,         // character-type options
      str, // string to map
      len,       // number of bytes in string
      0,  // wide-character buffer
	  0        // size of buffer
    );
	UnicodeString wstr;
	wstr.SetLength(wlen);

	dcmapMultiByteToWideChar(
      CodePage,         // code page
      0,         // character-type options
      str, // string to map
      len,       // number of bytes in string
      (wchar_t*) wstr.data(),  // wide-character buffer
      wlen        // size of buffer
    );
    return wstr;
}

UnicodeString ToWide(AnsiString const& str,int CodePage)
{
	return ToWide(str.c_str(),str.Length(),CodePage);
}

void ClipboardClear()
{
	OpenClipboard(MainForm->Handle);
	EmptyClipboard();
	CloseClipboard();
}

void ClipboardPutW(UnicodeString const& wcstr)
{
	if(!wcstr.data())return;

	Clipboard()->AsText  = wcstr;
	return;

	UnicodeString & wstr = (UnicodeString &)wcstr;
	if(!wstr.data())return;

	String Str = WideToString(wstr);

	HANDLE hglb;
	LPTSTR pt;

	HWND handle=0;
	if(MainForm)handle =  MainForm->Handle;

	OpenClipboard(handle);
	EmptyClipboard();

    int wsz = (wstr.Length()+1)*2;
    hglb = GlobalAlloc(GMEM_MOVEABLE,wsz);
    pt = (LPTSTR)GlobalLock(hglb);
    if(pt)memcpy(pt,wstr.data(),wsz);
    GlobalUnlock(hglb);
    SetClipboardData(CF_UNICODETEXT,hglb);

    hglb = GlobalAlloc(GMEM_MOVEABLE,Str.Length()+1);
    pt = (LPTSTR)GlobalLock(hglb);
	if(pt)wcscpy(pt,Str.w_str());
    GlobalUnlock(hglb);

    SetClipboardData(CF_TEXT,hglb);

	CloseClipboard ();
}

void ClipboardPutW(UnicodeString & Str)
{
	if(!Str.data()) return;

	Clipboard()->AsText  = Str;
	return;


    HANDLE hglb;
    LPTSTR pt;

    OpenClipboard(MainForm->Handle);
    EmptyClipboard();

    UnicodeString wstr = Str;
    int wsz = (wstr.Length()+1)*2;
    hglb = GlobalAlloc(GMEM_MOVEABLE,wsz);
    pt = (LPTSTR)GlobalLock(hglb);
    if(pt)memcpy(pt,wstr.data(),wsz);
    GlobalUnlock(hglb);
	SetClipboardData(CF_UNICODETEXT,hglb);

    hglb = GlobalAlloc(GMEM_MOVEABLE,Str.Length()+1);
    pt = (LPTSTR)GlobalLock(hglb);
	if(pt)wcscpy(pt,Str.w_str());
    GlobalUnlock(hglb);

    SetClipboardData(CF_TEXT,hglb);

	CloseClipboard ();
}

void ClipboardPut(AnsiString Str)
{
	if(!Str.data()) return;
	Clipboard()->AsText  = Str;
	return;

	HANDLE hglb = GlobalAlloc(GMEM_MOVEABLE,Str.Length()+1);
    if(!hglb)return;
	LPSTR str = (char*)GlobalLock(hglb);
    if(!str){GlobalFree(hglb);return;}
    strcpy(str,Str.c_str());
    GlobalUnlock(hglb);

    OpenClipboard(MainForm->Handle);
    EmptyClipboard();
    HANDLE r = SetClipboardData(CF_TEXT,hglb);
    CloseClipboard ();
}

UnicodeString ClipboardGetW()
{
	return Clipboard()->AsText ;

	UnicodeString result;
	OpenClipboard(MainForm->Handle);
	HANDLE hglb = GetClipboardData(CF_UNICODETEXT);
	int sz = GlobalSize(hglb);
	if(hglb)
	{
		LPWSTR str = (LPWSTR)GlobalLock(hglb);
		if(str)
		{
			result.SetLength(sz/sizeof(wchar_t));
			memcpy((wchar_t*)result.data(),str,sz);
		}
		GlobalUnlock(str);
	}
	else
	{
		hglb = GetClipboardData(CF_TEXT);
		if(hglb)
		{
			LPTSTR str = (LPTSTR)GlobalLock(hglb);
			if(str)result = ToWide(str);
			GlobalUnlock(str);
		}
	}
	CloseClipboard();
	return result;
}

AnsiString ClipboardGet()
{
	return Clipboard()->AsText ;

	String result;
	HANDLE hglb = GetClipboardData(CF_TEXT);
	if(hglb)
	{
		LPSTR str = (char*)GlobalLock(hglb);
		if(str)result = str;
		GlobalUnlock(str);
	}
	return result;
}

void SaveFormPlacement(TForm* pForm,IDCMapConfig* pConfig)
{
	DCMapConfigWrapper  Config(pConfig?pConfig:GetDCMapCore()->GetConfig());
	Config.section(pForm->Name+"Placement");
    {
      WINDOWPLACEMENT WndPlace;
      WndPlace.length = sizeof(WndPlace);

      GetWindowPlacement(pForm->Handle,&WndPlace);

      Config["Left"] << WndPlace.rcNormalPosition.left;
      Config["Top"] << WndPlace.rcNormalPosition.top;
      Config["Right"] << WndPlace.rcNormalPosition.right;
      Config["Bottom"] << WndPlace.rcNormalPosition.bottom;

      Config["showCmd"] << WndPlace.showCmd;
      Config["Flags"] <<  WndPlace.flags;

      Config["Saved"] <<  1;
    }
}

bool LoadFormPlacement(TForm* pForm,WINDOWPLACEMENT* pWndPlace,IDCMapConfig* pConfig)
{
	WINDOWPLACEMENT m_WndPlace;
	DCMapConfigWrapper  Config(pConfig?pConfig:GetDCMapCore()->GetConfig());
	Config.section(pForm->Name+"Placement");
	{
	  ZeroMemory(&m_WndPlace,sizeof(m_WndPlace));
	  m_WndPlace.length = sizeof(m_WndPlace);

	  Config["Left"] >> m_WndPlace.rcNormalPosition.left;
	  Config["Top"] >> m_WndPlace.rcNormalPosition.top;
	  Config["Right"] >> m_WndPlace.rcNormalPosition.right;
	  Config["Bottom"] >> m_WndPlace.rcNormalPosition.bottom;

	  m_WndPlace.ptMaxPosition.x = -1;
	  m_WndPlace.ptMaxPosition.y = -1;
	  m_WndPlace.ptMinPosition.x = -1;
	  m_WndPlace.ptMinPosition.y = -1;

	  Config["showCmd"] >> m_WndPlace.showCmd;
	  Config["Flags"] >>  m_WndPlace.flags;

	  int Saved=0;
	  Config["Saved"] >>  Saved;

	  if(Saved)
	  {
		 if(pWndPlace)*pWndPlace = m_WndPlace;
		 else
		 {
			pForm->Left= m_WndPlace.rcNormalPosition.left;
			pForm->Top= m_WndPlace.rcNormalPosition.top;

			if(pForm->BorderStyle == bsSizeable
				|| pForm->BorderStyle == bsSizeToolWin)
			{
				//pForm->Width= m_WndPlace.rcNormalPosition.right;
				//pForm->Height= m_WndPlace.rcNormalPosition.bottom;
				SetWindowPlacement(pForm->Handle,&m_WndPlace);
			}
		 }
		 return true;
	  }
	}
	return false;
}

TPoint StrToPoint(String Coords)
{
	POINT pt={-1,-1};
	dcmapStrToCoords(WideToString(Coords).c_str(),&pt);
	return pt;
}

void PluginList_LoadClass(AnsiString Class,TStrings* List,bool Append)
{
	if(!Append)PluginList_Clear(List);
	if(g_pSys->SelectClass(Class.c_str()))
		do
		{
			List->AddObject(g_pSys->GetPluginLocalName(),new TStringHolder(g_pSys->GetPluginName()));
		}
		while(g_pSys->Next());
}

void PluginList_LoadDataTables(TStrings* List,bool Append)
{
	String Class = "PDataStorage";
	if(!Append)PluginList_Clear(List);
	if(g_pSys->SelectClass(WideToString(Class).c_str()))
		do
		{
			AnsiString Param = g_pSys->GetPluginParam();
			#ifndef _DEBUG
			if(Param.Pos(":Hidden:") == 0)
			#endif
			{
				List->AddObject(g_pSys->GetPluginLocalName(),new TStringHolder(g_pSys->GetPluginName()));
            }
		}
		while(g_pSys->Next());
}


void PluginList_Clear(TStrings* List)
{
	int n = List->Count;
    for(int i=0;i<n;i++)
    {
        TStringHolder* hld = (TStringHolder*)List->Objects[i];
        if(hld)delete hld;
    }
    List->Clear();
}

int PluginList_IndexOf(TStrings* List,String Module,int Default)
{
	int n = List->Count;
    for(int i=0;i<n;i++)
    {
        if(ObjectToModuleName(List->Objects[i]) == Module)
        	return i;
    }
    return Default;
}

void DeleteStringsObjects(TStrings* List)
{
	 int cnt = List->Count;
	 for(int i=0;i<cnt;i++)
	 {
		TObject* pObj = List->Objects[i];
		List->Objects[i] = 0;
		if(pObj)delete pObj;
	 }
}

UnicodeString ScriptEdit(UnicodeString Text)
{
	IDCMapUIPanel* pPanel = GetDCMapCore()->CreateModuleTyped<IDCMapUIPanel>("PView","ScriptEditor");
	if(pPanel)
	{
		pPanel->OnMessage(DCMM_UI_SET_TEXT,0,
			(dcmapLPARAM)(Text.w_str()),0);
		GetDCMapCore()->GetCoreUI()->ShowModal(pPanel,L"",0);
		Text = (dcmapWCSTR)pPanel->OnMessage(DCMM_UI_GET_TEXT,0,0,0);
		GetDCMapCore()->DestroyModule(pPanel);
	}
	return Text;
}

static const char* c_strManifest =
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
"<assembly"
" xmlns=\"urn:schemas-microsoft-com:asm.v1\""
" manifestVersion=\"1.0\">"
"<assemblyIdentity"
" processorArchitecture=\"x86\""
" version=\"5.1.0.0\""
" type=\"win32\""
" name=\"DCMap.exe\"/>"
" <description>DCMap</description>"
" <trustInfo xmlns=\"urn:schemas-microsoft-com:asm.v3\">"
"    <security>"
"      <requestedPrivileges>"
"        <requestedExecutionLevel"
"          level=\"asInvoker\""
"          uiAccess=\"false\"/>"
"        </requestedPrivileges>"
"       </security>"
" </trustInfo>"
" <dependency>"
" <dependentAssembly>"
" <assemblyIdentity"
"  type=\"win32\""
"  name=\"Microsoft.Windows.Common-Controls\""
"  version=\"6.0.0.0\""
"  publicKeyToken=\"6595b64144ccf1df\""
"  language=\"*\""
"  processorArchitecture=\"x86\"/>"
" </dependentAssembly>"
"</dependency>"
"</assembly>";



void UpdateManifest(bool set)
{
	String Manifest = Application->ExeName+".MANIFEST";
	bool exists = FileExists(Manifest);
	if(set && !exists) // Credate manifest
	{
		scoped_ptr<TStringList> lst(new TStringList);
		lst->Text = c_strManifest;
		lst->SaveToFile(Manifest);
	}
	else if(!set && exists)  // delete manifest
	{
		DeleteFile(Manifest);
    }
}

void RegisterExt(String Ext,String Descr,String Command)
{
  TRegistry *Reg = new TRegistry;
  try
  {
    bool r;
	Reg->RootKey = HKEY_CLASSES_ROOT;
	String basekey = "DCMap."+Ext+"\\";

	r = Reg->OpenKey(basekey, true);
	Reg->WriteString("","DCMap "+Descr);
	Reg->CloseKey();

	r = Reg->OpenKey(basekey+"DefaultIcon", true);
	Reg->WriteString("",Application->ExeName+",1");
	Reg->CloseKey();

	r = Reg->OpenKey(basekey+"shell", true);
	Reg->WriteString("","open");
	Reg->CloseKey();


	String shellkey = basekey+"shell\\open\\";
	r = Reg->OpenKey(shellkey+"command", true);
	Reg->WriteString("",(String)"\""+Application->ExeName+"\" -nrest -ucmd=\""+Command+"('%1')\"");
	Reg->CloseKey();

	r = Reg->OpenKey(shellkey+"ddeexec", true);
	Reg->WriteString("",Command+"('%1')");
	Reg->CloseKey();

	r = Reg->OpenKey(shellkey+"ddeexec\\application", true);
	Reg->WriteString("","DCMap");
	Reg->CloseKey();

	r = Reg->OpenKey(shellkey+"ddeexec\\topic", true);
	Reg->WriteString("","ucmd");
	Reg->CloseKey();

	r = Reg->OpenKey("."+Ext, true);
	Reg->WriteString("","DCMap."+Ext);
	Reg->WriteString("Content Type","text/plain");
	Reg->WriteString("PerceivedType","text");
	Reg->CloseKey();
  }
  __finally
  {
	delete Reg;
  }
}

class TUtils : public IDCMapModule
{
	int DCMAPAPI ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
	{
		if(type == DCMEXEC_QUERY) return DCMEXEC_CALL;

		if(type == DCMEXEC_CALL)
		{
			if(!command || _wcsicmp(command,L"RegisterExt")==0)
			{
				RegisterExt(pParams->GetString(0),pParams->GetString(1),pParams->GetString(2));
				return DCMEXEC_SUCCESS;
			}
		}
		return DCMEXEC_FAIL;
	}
};
static CAutoReg<TUtils> sWrap("PCoreService","Utils");

 void ClearMenuItems(TMenuItem* MenuItem,int start,TNotifyEvent ByEvent)
 {
	int n = MenuItem->Count-start;
	for(int i=0;i<n;i++)
	{
		TMenuItem* item = MenuItem->Items[start];
		if(!ByEvent || item->OnClick == ByEvent)
		{
			MenuItem->Delete(start);
			delete item;
		}
		else return;
	}
 }

void WrapMenu(TMenuItem* MenuItem,int row_size)
{
	if(!row_size)row_size = 32;

	int count = MenuItem->Count;
	if(count > row_size)
	{
		int columns = Ceil((float)count/row_size);

		int rsz = Ceil((float)count/columns);

		for(int i=1;i<columns;i++)
			MenuItem->Items[i*rsz]->Break = mbBreak;
	}
}

String DBRefToStr(dcmapDBREF ref)
{
	return L"0x"+IntToHex((__int64)ref.data,16);
}

dcmapDBREF DBRefFromStr(String str)
{
	dcmapDBREF Ref;
	try
	{
		Ref.data = StrToInt64(str);
	}
	catch(...)
	{
		Ref.Invalidate();
	}
	return Ref;
}




#pragma package(smart_init)
