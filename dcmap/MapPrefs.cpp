//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MapPrefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sCheckBox"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma link "sGroupBox"
#pragma resource "*.dfm"
TMapPrefsFrame *MapPrefsFrame;

static CFramePanelWrapper<TMapPrefsFrame> sWrap("PPrefs_base","MapPrefs");

//---------------------------------------------------------------------------
__fastcall TMapPrefsFrame::TMapPrefsFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
void TMapPrefsFrame::LoadConfig(DCMapConfigWrapper cfg)
{
	cfg.section("MapNavView");

    bool v;
    int  i;
	for(int i=0;i<ComponentCount;i++)
    {
       TsCheckBox * PCheck = dynamic_cast<TsCheckBox*>(Components[i]);
       if(PCheck)if(cfg[PCheck->Name] >> v) PCheck->Checked = v;
    }

	for(int i=0;i<ComponentCount;i++)
    {
       TsSpinEdit * PCheck = dynamic_cast<TsSpinEdit*>(Components[i]);
       if(PCheck)if(cfg[PCheck->Name] >> i) PCheck->Value = i;
    }
}

void TMapPrefsFrame::OnApply()
{
	if(!Changed)return;

	DCMapConfigWrapper cfg(m_pCore->GetConfig());
	cfg.section("MapNavView");

	for(int i=0;i<ComponentCount;i++)
    {
       TsCheckBox * PCheck = dynamic_cast<TsCheckBox*>(Components[i]);
       if(PCheck)cfg[PCheck->Name] << PCheck->Checked ;
    }

    for(int i=0;i<ComponentCount;i++)
    {
       TsSpinEdit * PEdit = dynamic_cast<TsSpinEdit*>(Components[i]);
       if(PEdit)cfg[PEdit->Name] << PEdit->Value ;
    }

    Changed = false;
}
//---------------------------------------------------------------------------
void __fastcall TMapPrefsFrame::OnChanged(TObject *Sender)
{
	Changed = true;
}
//---------------------------------------------------------------------------
