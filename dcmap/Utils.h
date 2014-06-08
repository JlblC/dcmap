//---------------------------------------------------------------------------

#ifndef UtilsH
#define UtilsH
//---------------------------------------------------------------------------
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "dcmapsyslib.h"

using boost::scoped_ptr;

#define AppCodePage 1251

void ClipboardPut(String const& str);
//void ClipboardPutW(String const& Str);
void ClipboardPutW(UnicodeString const& wstr);
void ClipboardPutW(UnicodeString & Str);

void ClipboardClear();

AnsiString ClipboardGet();
UnicodeString ClipboardGetW();

extern TStringList* g_pCustomColors;

void SaveFormPlacement(TForm* pForm,IDCMapConfig* pConfig=0);
bool LoadFormPlacement(TForm* pForm,WINDOWPLACEMENT* pWndPlace=0,IDCMapConfig* pConfig=0);

TPoint StrToPoint(String Coords);

String inline CoordToStr(int x,int y){return IntToStr(x)+":"+IntToStr(y);}

UnicodeString ToWide(char * str,int len,int CodePage=-1);
UnicodeString ToWide(AnsiString const& str,int CodePage=-1);

AnsiString WideToString(UnicodeString const& wstr,int CodePage=-1);

class TStringHolder : public TObject
{
public:
    String str;
    TStringHolder(String name):str(name){};
};

AnsiString inline ObjectToModuleName(TObject* obj)
{
	return obj?((TStringHolder*)obj)->str:String();
}

void PluginList_LoadDataTables(TStrings* List,bool Append=false);
void PluginList_LoadClass(AnsiString Class,TStrings* List,bool Append=false);
void PluginList_Clear(TStrings* List);
int PluginList_IndexOf(TStrings* List,String Module,int Default=-1);


AnsiString inline PluginList_At(TStrings* List,int id)
{
	if(id<0)return "";
	return ObjectToModuleName(List->Objects[id]);
}

void DeleteStringsObjects(TStrings* List);

UnicodeString ScriptEdit(UnicodeString Text);
String ScriptEdit(String Text);

void UpdateManifest(bool set);
void RegisterExt(String Ext,String Descr,String Command);

void ClearMenuItems(TMenuItem* MenuItem,int start=0,TNotifyEvent ByEvent=0);
void WrapMenu(TMenuItem* MenuItem,int row_size=0);

dcmapDBREF DBRefFromStr(String str);
String DBRefToStr(dcmapDBREF ref);

#define DCMAP_DETAIL_FOREACH(it,x,i) for(it i=(x).begin(),__fe_end=(x).end();i!=__fe_end;++i)
#define _foreach(t,x,i) DCMAP_DETAIL_FOREACH(t::iterator,x,i)


#endif
