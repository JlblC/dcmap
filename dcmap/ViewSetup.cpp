//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ViewSetup.h"
#include "dcmapbase.h"
#include "BaseModule.h"
#include "Utils.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma link "sAlphaListBox"
#pragma link "sCheckListBox"
#pragma link "sButton"
#pragma link "sPageControl"
#pragma link "sComboBox"
#pragma link "sLabel"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"

TViewSetupForm *ViewSetupForm;
//---------------------------------------------------------------------------
__fastcall TViewSetupForm::TViewSetupForm(TComponent* Owner)
	: TBaseViewPopupForm(Owner)
{
}

class TItemDrag : public TDragObjectEx
{
public:
	String Item;
    TObject* Obj;
    int Index;
    TsListBox* From;
    int FromIndex;
    TsListBox* Current;

    bool Changed()
    {
        return From != Current || Index != FromIndex;
    }
};

//---------------------------------------------------------------------------
void __fastcall TViewSetupForm::ListStartDrag(TObject *Sender, TDragObject *&DragObject)
{
     TsListBox* lst = dynamic_cast<TsListBox*>(Sender);
	 TItemDrag* obj = new TItemDrag();
     obj->Item = "test1";

     if(lst->ItemIndex>=0)
     {
     	obj->Item = lst->Items->Strings[lst->ItemIndex];
     	obj->Obj = lst->Items->Objects[lst->ItemIndex];
        //lst->Items->Delete(lst->ItemIndex);
        obj->From = lst;
        obj->FromIndex = lst->ItemIndex;

        obj->Current = lst;
        obj->Index = lst->ItemIndex;
     }
     DragObject = obj;
}
//---------------------------------------------------------------------------
void __fastcall TViewSetupForm::ListDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State,
      bool &Accept)
{
	TItemDrag* obj = dynamic_cast<TItemDrag*>(Source);
    TsListBox* lst = dynamic_cast<TsListBox*>(Sender);
	if(obj)
    {
    	if(!obj->Current)return;
        
    	int item = lst->ItemAtPos(Point(X,Y),true);
        if(obj->Current == lst)
        {
        	if(item<0)
            {

            }
            else if(item != obj->Index)
            {
               lst->Items->Exchange(item,obj->Index);
               obj->Index = item;
               lst->ItemIndex = obj->Index;
            }
        }
        else 
        {
            obj->Current->Items->Delete(obj->Index);
            obj->Current = lst;
            if(item >=0)
            {
                lst->Items->InsertObject(item,obj->Item,obj->Obj);
            	obj->Index = item;
            }
            else
            {
                lst->Items->AddObject(obj->Item,obj->Obj);
                obj->Index = lst->Items->Count-1;
            }
            lst->ItemIndex = obj->Index;
        }
		Accept = true;
    }
}

void TViewSetupForm::LoadItemsList(String Name,TsListBox* list,TsListBox* ref)
{
	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
    cfg.section("Layout");
    int num=0;
    cfg[Name+"Num"] >> num;

    for(int i=0;i<num;i++)
    {
        String PName;
        cfg[Name+"Mod"+IntToStr(i)] >> PName;

        int Index = PluginList_IndexOf(ref->Items,PName);
        if(Index>=0)
        {
            list->Items->AddObject(ref->Items->Strings[Index],ref->Items->Objects[Index]);
            ref->Items->Delete(Index);
        }
    }
}

void TViewSetupForm::SaveItemsList(String Name,TsListBox* list)
{
	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
	cfg.section("Layout");
	int num=list->Items->Count;
	cfg[Name+"Num"] << num;

	for(int i=0;i<num;i++)
	{
		cfg[Name+"Mod"+IntToStr(i)] << ObjectToModuleName(list->Items->Objects[i]);
	}
}

//---------------------------------------------------------------------------
void __fastcall TViewSetupForm::FormShow(TObject *Sender)
{
	PluginList_LoadClass("PView",ViewCombo->Items);

	PluginList_LoadClass("PFrameBar",SidebarModules->Items);
	PluginList_LoadClass("PToolbarItem",ToolbarModules->Items);
	PluginList_LoadClass("PFormPanelItem",PanelModules->Items);

	LoadItemsList("RightBar",RightSidebar,SidebarModules);
 	LoadItemsList("LeftBar",LeftSidebar,SidebarModules);

 	LoadItemsList("Toolbar",Toolbar,ToolbarModules);

	LoadItemsList("TopPanels",TopPanels,PanelModules);
	LoadItemsList("BottomPanels",BottomPanels,PanelModules);
	LoadItemsList("LeftPanels",LeftPanels,PanelModules);
	LoadItemsList("RightPanels",RightPanels,PanelModules);

	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
	cfg.section("Layout");
	String Module;
	cfg["ViewModule"] >> Module;
	ViewCombo->ItemIndex = PluginList_IndexOf(ViewCombo->Items,Module,0);

    Changed = false;
}
//---------------------------------------------------------------------------
void TViewSetupForm::Save()
{
	if(ModalResult == mrOk)
    {
        SaveItemsList("RightBar",RightSidebar);
        SaveItemsList("LeftBar",LeftSidebar);
		SaveItemsList("Toolbar",Toolbar);

		SaveItemsList("TopPanels",TopPanels);
		SaveItemsList("BottomPanels",BottomPanels);
		SaveItemsList("LeftPanels",LeftPanels);
		SaveItemsList("RightPanels",RightPanels);

		DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
		cfg.section("Layout");

		cfg["ViewModule"] << PluginList_At(ViewCombo->Items,ViewCombo->ItemIndex);
	}
}
void __fastcall TViewSetupForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	if(!Changed)ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TViewSetupForm::ListDragDrop(TObject *Sender, TObject *Source, int X, int Y)
{
	TItemDrag* obj = dynamic_cast<TItemDrag*>(Source);
    TsListBox* lst = dynamic_cast<TsListBox*>(Sender);

	Changed |= obj->Changed();
}
//---------------------------------------------------------------------------


void __fastcall TViewSetupForm::FormCreate(TObject *Sender)
{
    InterfaceModule->LocalizeContainer(this);
}
//---------------------------------------------------------------------------

void __fastcall TViewSetupForm::FormDestroy(TObject *Sender)
{
	PluginList_Clear(ViewCombo->Items);
	PluginList_Clear(SidebarModules->Items);
	PluginList_Clear(ToolbarModules->Items);
	PluginList_Clear(PanelModules->Items);
}
//---------------------------------------------------------------------------


void __fastcall TViewSetupForm::OnChange(TObject *Sender)
{
	Changed = true;
}
//---------------------------------------------------------------------------


