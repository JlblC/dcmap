//$$---- Frame CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FilterGroup.h"
#include "Filter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sFrameAdapter"
#pragma link "sPanel"
#pragma link "sSpeedButton"
#pragma link "sLabel"
#pragma resource "*.dfm"
TFilterGroupFrame *FilterGroupFrame;
//---------------------------------------------------------------------------
__fastcall TFilterGroupFrame::TFilterGroupFrame(TComponent* Owner)
	: TFrame(Owner)
{
	ItemCounter=0;
    ParentGroup=0;
    Filter=0;
    Collapsed=false;
    Conjunction=true;
}
//---------------------------------------------------------------------------

void TFilterGroupFrame::UpdateHeight()
{
	int h=0;
    if(!Collapsed)
    {
        for(int i=0;i<ClientPanel->ControlCount;i++)
        {
            h+=ClientPanel->Controls[i]->Height;
        }
    }
    else
    {
        h = HeaderPanel->Height;
    }
    SetClientHeight(h);
}


void TFilterGroupFrame::SetClientHeight(int h)
{
    if(h+1 != ClientPanel->Height)
    {
    	MarkerShape->Visible = h > HeaderPanel->Height;
		Height += h-ClientPanel->Height+1;
        if(ParentGroup)ParentGroup->UpdateHeight();
    }
}

void TFilterGroupFrame::Init()
{
    AddBtn->DropdownMenu = Filter->AddMenu;
    UpdateHeight();
}

void TFilterGroupFrame::AddSubGroup()
{
	TFilterGroupFrame* Frame = new TFilterGroupFrame(this);
    Frame->Name = "Frame"+IntToStr(ItemCounter);
    ItemCounter++;
    Frame->Align = alTop;
    Frame->Top = ClientPanel->Height;
    Frame->Parent = ClientPanel;

    Frame->Filter = Filter;
    Frame->ParentGroup = this;
    Frame->Init();

    if(Collapsed)CollapseButtonClick(this);
    else UpdateHeight();

    FilterRecord rec;
    rec.group = Frame;
    m_mapRecords[Frame] = rec;
}
void __fastcall TFilterGroupFrame::AddBtnMouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y, int HitTest, TMouseActivate &MouseActivate)
{
	Filter->CurrentGroup = this;
}
//---------------------------------------------------------------------------
void __fastcall TFilterGroupFrame::CollapseButtonClick(TObject *Sender)
{
	if(Collapsed)
    {
        Collapsed = false;
        UpdateHeight();
        CollapseButton->Caption = "-";
    }
    else
    {
        Collapsed = true;
        UpdateHeight();
        CollapseButton->Caption = "+";
    }
}

void TFilterGroupFrame::AddNewItem(IDCMapFilterItem* filter,IDCMapVCLPanel* panel)
{
	TFrame* Frame = panel->CreateFrame();
    Frame->Name = "Frame"+IntToStr(ItemCounter);
    ItemCounter++;
    Frame->Align = alTop;
    Frame->Top = ClientPanel->Height;
    Frame->Parent = ClientPanel;
    panel->OnShow();

    if(Collapsed)CollapseButtonClick(this);
    else UpdateHeight();

    FilterRecord rec;
    rec.filter = filter;
    rec.panel = panel;
    m_mapRecords[Frame] = rec;
}
//---------------------------------------------------------------------------

int TFilterGroupFrame::Process(const DCMapDataReference & ref)
{
	bool val = false;
	for(MapRecords::iterator it = m_mapRecords.begin();it != m_mapRecords.end();i++)
    {
    	if(it->second.group)
        {
            int r = it->second.group->Process(ref);
            if(Conjunction && !r)return false;
            val |= r;
        }
        else if(it->second.filter->Enabled())
        {
            int r = it->second.filter->Process(ref);
            if(Conjunction && !r)return false;
            val |= r;
        }
    }
    return val;
}

