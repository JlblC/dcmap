//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SizeableSideModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sPanel"
#pragma resource "*.dfm"
TSizeableSideFrame *SizeableSideFrame;
//---------------------------------------------------------------------------
__fastcall TSizeableSideFrame::TSizeableSideFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSizeableSideFrame::BottomSizeBarMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	BottomSizeBarLastMousePos = Y;
}
//---------------------------------------------------------------------------
void __fastcall TSizeableSideFrame::BottomSizeBarMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
	if(Shift.Contains(ssLeft))
	{
		int dist = Y - BottomSizeBarLastMousePos;
		int nh = Height + dist;
		if(nh > this->Constraints->MinHeight)
			Height = nh;
	}
}
//---------------------------------------------------------------------------

void TSizeableSideFrame::SaveConfig(DCMapConfigWrapper cfg)
{
	Inherited::LoadConfig(cfg);
	cfg.section(Name);
	cfg["Height"] << Height;
}

void TSizeableSideFrame::LoadConfig(DCMapConfigWrapper cfg)
{
	Inherited::SaveConfig(cfg);
	cfg.section(Name);
	int h = Height;
	if(cfg["Height"] >> h) Height = h;
}
