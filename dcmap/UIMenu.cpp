//---------------------------------------------------------------------------

#include "vcl.h"
#pragma hdrstop

#include "UIMenu.h"
#include "acntUtils.hpp"
#include "Localize.h"
#include "api/dcmap_script_helper.h"
#include "Interface.h"

String GetMenuName(String MenuRef)
{
	if(!MenuRef.IsEmpty())
	{
		int delim = MenuRef.Pos(":");

		if(delim>0)
		{
			return MenuRef.SubString(1,delim-1)+"Menu";
		}
		return MenuRef+"Menu";
	}
	return MenuRef;
}

TMenuItem* CreateMenuItem(TMenuItem* pParent,const char* MenuPath,dcmapWCSTR MenuCaption)
{
	if(!MenuPath)MenuPath = "";

	// parce MenuPath to get path to menu:
    String Path = MenuPath;
    String Name;

    TSysCharSet Delims;
    Delims << '/';

	bool found=false;
	int n=1;
    while(true)
    {
	  Name = ExtractWord(n,Path,Delims);
	  if(!Name.Length())break;
	  found=false;
	  for(int i=0;i<pParent->Count;i++)
      {
		  if(_wcscmpi(pParent->Items[i]->Name.w_str(),GetMenuName(Name).w_str())==0)
          {
			  pParent = pParent->Items[i];
              found = true;
              break;
          }
      }
	  if(!found)break;
	  n++;
	}

	if(found && Path[Path.Length()]!= '/' )
	{
		return 0;
	}

	TMenuItem* NewItem = new TMenuItem(0);
	int GroupIndex=2;

	if(!Name.IsEmpty())
	{
		int delim = Name.Pos(":");

		if(delim>0)
		{
			GroupIndex = StrToInt(Name.SubString(delim+1,Name.Length()));
			Name = Name.SubString(1,delim-1);
		}
		if(!Name.IsEmpty())
			NewItem->Name = Name+"Menu";
	}

	NewItem->GroupIndex = GroupIndex;
	NewItem->Caption = MenuCaption;

	int cnt = pParent->Count;
	int pos = 0;
	for(;pos<cnt;pos++)
	{
		if(pParent->Items[pos]->GroupIndex > GroupIndex)break;
	}
	pParent->Insert(pos,NewItem);
	return NewItem;
}




TDCMapMenuItem::TDCMapMenuItem():
	m_pLister(0),
	Check(false),
	Checked(false),
	Enabled(true),
	Visible(true)
{

}


void __fastcall TDCMapMenuItem::DoClick(TObject *Sender)
{
	TMenuItem* Item = dynamic_cast<TMenuItem*>(Sender);
	if(Item && Check)
	{
		SetChecked(Item->Checked);
    }
	m_pOwner->OnMessage(DCMM_UI_COMMAND,m_iID,(dcmapLPARAM)GetValue(),this);
}
bool DCMAPAPI TDCMapMenuItem::Test()
{
	return !(m_pOwner->OnMessage(DCMM_UI_TEST,m_iID,(dcmapLPARAM)GetValue(),this) & DCMM_FAIL);
}

void TDCMapMenuItem::ClearItem()
{
	_foreach(MapItems,Items,it)delete it->second;
	Items.clear();
}

void TDCMapMenuItem::InitItem(TDCMapUIContext* pContext,dcmapCSTR Path,dcmapWCSTR Caption,IDCMapModule* pOwner,int iID)
{
	TDCMapBasicComponentItem::InitItem(pContext,pOwner,iID);
	m_Path = Path;
	m_Caption = Caption;
}

void DCMAPAPI TDCMapMenuItem::SetShortCut(dcmapWCSTR Str)
{
	ShortCut = Str?Str:L"";
	TShortCut sc = TextToShortCut(ShortCut);
	_foreach(MapItems,Items,it)it->second->ShortCut = sc;
}
void DCMAPAPI TDCMapMenuItem::SetCaption(dcmapWCSTR Str)
{
	m_Caption = Str?Str:L"";
	_foreach(MapItems,Items,it)it->second->Caption = m_Caption;
}


void TDCMapMenuItem::InitItem(AnsiString Path,String Caption,IDCMapModule* pOwner,int iID)
{
	TDCMapBasicComponentItem::InitItem(0,pOwner,iID);
	m_Path = Path;
	m_Caption = Caption;
}

void TDCMapMenuItem::AddMenuItem(TComponent* Target,TMenuItem* pMenuItem)
{
    Items[Target] = pMenuItem;
    SetupMenuItem(pMenuItem);
}

void TDCMapMenuItem::CreateMenuItem(TComponent* Target,TMenuItem* pMenuParent)
{
	TMenuItem* Item = ::CreateMenuItem(pMenuParent,m_Path.c_str(),m_Caption.w_str());
	if(Item)
	{
		Items[Target] = Item;
		SetupMenuItem(Item);
	}
}

void TDCMapMenuItem::SetupMenuItem(TMenuItem* pItem)
{
	LocalStrings->SelectSection("Menues");

	pItem->AutoCheck = Check;
	pItem->Visible = Visible;
	pItem->Checked = Checked;
	pItem->Enabled = Enabled;
	pItem->ShortCut = TextToShortCut(ShortCut);
	pItem->Caption = m_Caption;

	LocalizeMenuItem(pItem,0,LocalStrings);

	pItem->Tag = reinterpret_cast<int>(this);
    pItem->OnClick = DoClick;
}

DCMAP_SCRIPT_REFLECTION(TDCMapMenuItem)
{
	DCMAP_SCRIPT_PROPERTY(Check);
	DCMAP_SCRIPT_PROPERTY(Checked);
	DCMAP_SCRIPT_PROPERTY(Enabled);
	DCMAP_SCRIPT_PROPERTY(Visible);
	DCMAP_SCRIPT_PROPERTY(Caption);
	DCMAP_SCRIPT_PROPERTY(ShortCut);
}

//////////////////////////////////////////////////////////////////

void TDCMapPopupMenu::InitItem(TComponent* Owner,IDCMapModule* pOwner,int iID)
{
	Link(pOwner);
	m_pMenu = new TPopupMenu(Owner);
}

void DCMAPAPI TDCMapPopupMenu::Popup()
{
	if(!m_pMenu)return;
	if(Updated)
	{
		Updated = false;
		InterfaceModule->HookMenu(m_pMenu);
	}
    m_pMenu->Popup(Mouse->CursorPos.x,Mouse->CursorPos.y);
}

IDCMapUIMenuItem* DCMAPAPI TDCMapPopupMenu::AddItem(dcmapCSTR MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id)
{
	TDCMapMenuItem* pItem = new TDCMapMenuItem;
	pItem->InitItem(MenuPath?MenuPath:"",MenuCaption?MenuCaption:L"",pOwner,id);
	pItem->CreateMenuItem(m_pMenu,m_pMenu->Items);

	Updated = true;
	return pItem;
}

TDCMapUIContext::TDCMapUIContext()
{
	m_pContextValue = dcmapCreateVarValue();
}

TDCMapUIContext::~TDCMapUIContext()
{
	m_pContextValue->Destroy();
}

void  TDCMapUIContext::Link(TDCMapBasicComponentItem* pItem)
{
	Items.insert(pItem);
}

void  TDCMapUIContext::Unlink(TDCMapBasicComponentItem* pItem)
{
	Items.erase(pItem);
}
IDCMapUIMenuItem* DCMAPAPI TDCMapUIContext::AddMenuItem(const char* MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id)
{
	TDCMapMenuItem* pItem = new TDCMapMenuItem;
	pItem->InitItem(this,MenuPath,MenuCaption,pOwner,id);
	return pItem;
}

bool DCMAPAPI TDCMapUIContext::SetMenu(IDCMapUIMenu* pPopup)
{
    //m_pMenu
	return true;
}

void TDCMapUIContext::SetMenu(TMenuItem* pMenu)
{
	m_pMenu = pMenu;
}

IDCMapVarValue* DCMAPAPI TDCMapUIContext::ContextValue()
{
	return m_pContextValue;
}

int DCMAPAPI  TDCMapUIContext::Setup()
{
	int n=0;
	_foreach(ItemsList,Items,it)
	{
		TDCMapMenuItem* pItem = dynamic_cast<TDCMapMenuItem*>(*it);
		if(pItem)
		{
			if(pItem->Test())
			{
				pItem->CreateMenuItem(m_pMenu,m_pMenu);
				n++;
			}
			else pItem->ClearItem();
		}
	}
	return n;
}

bool DCMAPAPI  TDCMapUIContext::TestEmpty()
{
	_foreach(ItemsList,Items,it)
	{
		TDCMapMenuItem* pItem = dynamic_cast<TDCMapMenuItem*>(*it);
		if(pItem)
		{
			if(pItem->Test())return false;
		}
	}
	return true;
}

void DCMAPAPI  TDCMapUIContext::ClearItems()
{
	_foreach(ItemsList,Items,it)
	{
		TDCMapMenuItem* pItem = dynamic_cast<TDCMapMenuItem*>(*it);
		if(pItem)pItem->ClearItem();
	}

}


//---------------------------------------------------------------------------

#pragma package(smart_init)
