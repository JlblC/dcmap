//---------------------------------------------------------------------------
#include "vcl.h"
#pragma hdrstop

//#include "g_gsys.h"

#include "UIWorkspace.h"
#include "ViewLayout.h"
#include "DataEdit.h"
#include "main.h"
#include "dcmapsyslib.h"
#include "CoreUIManager.h"
#include "UIFrameForm.h"

TUIWorkspace::TUIWorkspace(IDCMapWorkspace* pWorkspace,IDCMapFrame* pFrame,IDCMapCore* pCore) :
				m_pWorkspace(pWorkspace),m_pFrame(pFrame),m_pCore(pCore),
				Destroing(false),m_pLayout(0)
{
	TFrameUI* FrameUI = dynamic_cast<TFrameUI*>(pFrame->GetUI());
	FrameUI->OnNewWorkspace(this);
}

TUIWorkspace::~TUIWorkspace()
{
	Destroing = true;
	_foreach(ToolForms,m_ToolForms,it)
	{
	   (*it)->Close();
	   delete *it;
	}
}

void DCMAPAPI TUIWorkspace::ActivateView()
{
	if(Destroing)return;
	m_pLayout->ActivateView();
}

void DCMAPAPI TUIWorkspace::OnActivate()
{
	if(Destroing)return;
	_foreach(ToolForms,m_ToolForms,it)
	{
	   (*it)->Show();
	}
}

void DCMAPAPI TUIWorkspace::OnDeactivate()
{
	if(Destroing)return;
	_foreach(ToolForms,m_ToolForms,it)
	{
	   (*it)->Hide();
	}
}

void DCMAPAPI TUIWorkspace::RegisterForm(TForm* pForm)
{
	if(Destroing)return;
	m_ToolForms.insert(pForm);
}

void DCMAPAPI TUIWorkspace::UnregisterForm(TForm* pForm)
{
	if(Destroing)return;
	m_ToolForms.erase(pForm);
}


/*
TDCMapMenuItem* DCMAPAPI AddMenuItem(TMenuItem* pParent,const char* MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id)
{
	if(!MenuPath || !MenuPath[0])return 0;

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
		  if(_wcscmpi (pParent->Items[i]->Name.w_str(),GetMenuName(Name).w_str())==0)
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
		if(!pParent->Tag)return 0;
		return reinterpret_cast<TDCMapMenuItem*>(pParent->Tag);
	}

	TMenuItem* NewItem = new TMenuItem(pParent);
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

	TDCMapMenuItem* pItem = new TDCMapMenuItem;
	pItem->InitItem(NewItem,pOwner,id);
	return pItem;
}
*/

IDCMapUIMenuItem* DCMAPAPI TUIWorkspace::AddMenuItem(dcmapCSTR MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id)
{
	if(Destroing)return 0;

	TDCMapMenuItem* pItem = new TDCMapMenuItem;
	pItem->InitItem(MenuPath,MenuCaption,pOwner,id);
	pItem->CreateMenuItem(m_pLayout,m_pLayout->MainMenu->Items);
}

IDCMapUIMenuItem* DCMAPAPI TUIWorkspace::AddMenu(dcmapCSTR MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id)
{
	if(Destroing)return 0;

	TDCMapMenuItem* pItem = new TDCMapMenuItem;
	pItem->InitItem(MenuPath,MenuCaption,pOwner,id);
	pItem->CreateMenuItem(m_pLayout,m_pLayout->MainMenu->Items);
}

TViewLayoutFrame* TUIWorkspace::GetLayout()
{
	if(m_pLayout)return m_pLayout;
	m_pLayout = new TViewLayoutFrame(0);
    m_pLayout->m_pWorkspace = dcmap_cast<IDCMapWorkspaceEx*>(m_pWorkspace);
	m_pLayout->m_pUIWorkspace = this;
    m_pLayout->FrameForm = FrameForm;

	return m_pLayout;
}

//---------------------------------------------------------------------------
#pragma package(smart_init)
