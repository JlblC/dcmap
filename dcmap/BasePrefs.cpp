//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BasePrefs.h"
#include "sColorSelect.hpp"
#include "sCheckBox.hpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma resource "*.dfm"
TBasePrefsFrame *BasePrefsFrame;
//---------------------------------------------------------------------------
__fastcall TBasePrefsFrame::TBasePrefsFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
	AutoList=0;
}

__fastcall TBasePrefsFrame::~TBasePrefsFrame()
{
	if(AutoList)delete AutoList;
}

void TBasePrefsFrame::LoadConfig(DCMapConfigWrapper cfg)
{
	 AutoLoadConfig(cfg);
	 OnLoadPrefs(cfg);
}

void TBasePrefsFrame::OnApply()
{
	if(!Changed)return;
	DCMapConfigWrapper cfg(m_pCore->GetConfig());

	AutoSaveConfig(cfg);
	OnSavePrefs(cfg);

	Changed = false;
}

void TBasePrefsFrame::OnLoadPrefs(DCMapConfigWrapper cfg)
{

}

void TBasePrefsFrame::OnSavePrefs(DCMapConfigWrapper cfg)
{

}

void TBasePrefsFrame::AddConfigTarget(String Section,int Tag)
{
	if(!AutoList)AutoList = new TStringList;
	AutoList->AddObject(Section,(TObject*)Tag);
}

void TBasePrefsFrame::AutoLoadConfig(DCMapConfigWrapper cfg)
{
	if(!AutoList)return;
	for(int i=0;i<AutoList->Count;i++)
	{
		AutoLoadConfig(cfg,AutoList->Strings[i],(int)AutoList->Objects[i]);
	}
}

void TBasePrefsFrame::AutoSaveConfig(DCMapConfigWrapper cfg)
{
	if(!AutoList)return;
	for(int i=0;i<AutoList->Count;i++)
	{
		AutoSaveConfig(cfg,AutoList->Strings[i],(int)AutoList->Objects[i]);
	}
}

void TBasePrefsFrame::AutoLoadConfig(DCMapConfigWrapper cfg,AnsiString section,int Tag)
{
	cfg.section(section.c_str());

	for(int i=0;i<ComponentCount;i++)
	{
		TComponent* Component = Components[i];
	   if(Component->Tag != Tag)continue;


	   {
		   TsColorSelect * PColor;
		   PColor = dynamic_cast<TsColorSelect*>(Component);
		   if(PColor)
		   {
			   COLORREF cl;
			   if(cfg[Component->Name] >> cl) PColor->ColorValue = cl;
			   continue;
		   }
	   }

	   {
		   TCustomEdit* PEdit;
		   PEdit = dynamic_cast<TCustomEdit*>(Component);
		   if(PEdit)
		   {
			   String str;
			   if(cfg[Component->Name] >> str) PEdit->Text = str;
				continue;
		   }
	   }

	   {
		   TsCheckBox* PCheckBox;
		   PCheckBox = dynamic_cast<TsCheckBox*>(Component);
		   if(PCheckBox)
		   {
			   bool check;
			   if(cfg[Component->Name] >> check) PCheckBox->Checked = check;
				continue;
		   }
	   }
	}
}
//---------------------------------------------------------------------------
void TBasePrefsFrame::AutoSaveConfig(DCMapConfigWrapper cfg,AnsiString section,int Tag)
{
	cfg.section(section.c_str());

	for(int i=0;i<ComponentCount;i++)
	{
	   TComponent* Component = Components[i];
	   if(Component->Tag != Tag)continue;

	   {
			TsColorSelect * PColor;
			PColor = dynamic_cast<TsColorSelect*>(Components[i]);
			if(PColor)
			{
				cfg[PColor->Name] << (COLORREF)PColor->ColorValue ;
				continue;
			}
	   }

	   {
		   TCustomEdit* PEdit;
		   PEdit = dynamic_cast<TCustomEdit*>(Component);
		   if(PEdit)
		   {
				cfg[Component->Name] << PEdit->Text;
				continue;
           }
	   }

	   {
		   TsCheckBox* PCheckBox;
		   PCheckBox = dynamic_cast<TsCheckBox*>(Component);
		   if(PCheckBox)
		   {
			   cfg[Component->Name] << PCheckBox->Checked;
			   continue;
		   }
	   }
	}
}

void __fastcall TBasePrefsFrame::OnChange(TObject *Sender)
{
	Changed = true;
}

