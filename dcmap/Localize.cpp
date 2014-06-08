//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "Localize.h"
#include "Utils.h"
#include "Interface.h"

#include "BaseModule.h"
#include "sComboBox.hpp"
#include "sRadioButton.hpp"
#include "sCheckBox.hpp"
#include "sLabel.hpp"
#include "sBevel.hpp"
#include "sButton.hpp"
#include "sEdit.hpp"
#include "sSpeedButton.hpp"
#include "sGroupBox.hpp"
#include "sAlphaListBox.hpp"
#include "sColorSelect.hpp"
#include "sComboBoxes.hpp"
#include "sSpinEdit.hpp"
#include "sMemo.hpp"
#include "sTreeView.hpp"
#include "sListView.hpp"
#include "Utils.h"
#include "sPageControl.hpp"
#include "sCheckListBox.hpp"
#include "sCustomComboEdit.hpp"
#include "sTooledit.hpp"
#include "sTrackBar.hpp"
#include "sComboEdit.hpp"
#include "sToolBar.hpp"


static bool TotalHints = false;

template <typename T>
 void FontConf(T* sControl)
{
//	if(g_pLocalStrings->CharSet())
//		sControl->Font->Charset = g_pLocalStrings->CharSet();

	//sControl->Font->Charset = DEFAULT_CHARSET;
	//sControl->Font->Name = L"Tahoma";
}

template <typename T>
 bool CaptionConf(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	wchar_t const* str;
	T* sControl = dynamic_cast<T*>(Cnt);
	if(sControl)
	{
		str = Strs->GetSectionStringW(WideToString(sControl->Name).c_str());
		if(str[0])sControl->Caption = str;
		str = Strs->GetSectionStringW(WideToString(sControl->Name+".Hint").c_str());
		if(str[0])
		{
			sControl->Hint = str;
			sControl->ShowHint=true;
		}
		else sControl->ShowHint=TotalHints;
		return true;
	}
	return false;
 }

template <typename T>
 bool CaptionConfGlyph(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	wchar_t const* str;
	T* sControl = dynamic_cast<T*>(Cnt);
	if(sControl)
	{
		str = Strs->GetSectionStringW(WideToString(sControl->Name+".Hint").c_str());
		if(str[0])
		{
			sControl->Hint = str;
			sControl->ShowHint=true;
		}
		else sControl->ShowHint=TotalHints;

		str = Strs->GetSectionStringW(WideToString(sControl->Name+".Glyph").c_str());
		if(str[0])
		{
			sControl->Images = InterfaceModule->MenuImageList;
			sControl->ImageIndex = InterfaceModule->GetMenuImage(str);
		}

		str = Strs->GetSectionStringW(WideToString(sControl->Name).c_str());

		// ac 6.20 hack
		if(str[0])
                {
					//#ifndef _DEBUG
                        try
                        {
                        	sControl->Caption = str;
                        }
                        catch(...){}
                   //#endif
                }

		return true;
	}
	return false;
 }


 template <typename T>
 bool CaptionConf_W(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	dcmapWCSTR wstr;
	T* sControl = dynamic_cast<T*>(Cnt);
	if(sControl)
	{
		wstr = Strs->GetSectionStringW(WideToString(sControl->Name).c_str());
		if(wstr[0])sControl->Caption = wstr;
		wstr = Strs->GetSectionString(WideToString(sControl->Name+".Hint").c_str());
		if(wstr[0])
		{
			sControl->Hint = wstr;
			sControl->ShowHint=true;
		}
		else sControl->ShowHint=TotalHints;

		return true;
	}
	return false;
 }

 template <typename T>
 bool HintConf(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	dcmapWCSTR str;
	T* sControl = dynamic_cast<T*>(Cnt);
	if(sControl)
	{
		str = Strs->GetSectionStringW(WideToString(sControl->Name+".Hint").c_str());
		if(str[0])
		{
			sControl->Hint = str;
			sControl->ShowHint=true;
		}
		else sControl->ShowHint=TotalHints;

		return true;
	}
	return false;
 }

  template <typename T>
 bool ImageConf(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	dcmapWCSTR str;
	T* sControl = dynamic_cast<T*>(Cnt);
	if(sControl)
	{
		str = Strs->GetSectionStringW(WideToString(sControl->Name+".Hint").c_str());
		if(str[0])
		{
			sControl->Hint = str;
			sControl->ShowHint=true;
		}
		else sControl->ShowHint=TotalHints;


		str = Strs->GetSectionStringW(WideToString(sControl->Name+".Glyph").c_str());
		if(str[0])
		{
			int id = InterfaceModule->GetMenuImage(str);
			if(id>=0)
			{
				TPNGGraphic* png = InterfaceModule->MenuImage(id);
				sControl->Picture->Assign(png);
			}
		}

		return true;
	}
	return false;
 }

 bool LocalizeMenuItem(TMenuItem* sControl,int Mode,IDCMapLocalStrings* Strs)
 {
	if(!Strs)Strs = g_pLocalStrings;

	dcmapWCSTR wstr;

	String Name = sControl->Name;

	if(Mode==0)
	{
		 Strs->SelectSection("Menues");
		 int len = Name.Length();
		 Name.Delete(len-3,len);
	}

	if(!sControl->SubMenuImages)
		sControl->SubMenuImages = InterfaceModule->MenuImageList;

	wstr = Strs->GetSectionStringW(WideToString(Name).c_str());
	if(wstr[0])sControl->Caption = wstr;

	wstr = Strs->GetSectionStringW(WideToString(Name+".Hint").c_str());
	if(wstr[0])sControl->Hint = wstr;

	wstr = Strs->GetSectionStringW(WideToString(Name+".Shortcut").c_str());
	if(wstr[0])sControl->ShortCut = TextToShortCut(wstr);
	wstr = Strs->GetSectionStringW(WideToString(Name+".Glyph").c_str());
	if(wstr[0])
	{
		sControl->ImageIndex = InterfaceModule->GetMenuImage(wstr);
	}
	return true;

 }


template <typename T>
bool CaptionConfMenuItem(TComponent* Cnt,IDCMapLocalStrings* Strs)
{
	T* sControl = dynamic_cast<T*>(Cnt);
	if(!sControl)return false;
	return LocalizeMenuItem(sControl,1,Strs);
}

 template <typename T>
 bool CaptionConfToolButton(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	dcmapWCSTR wstr;
	T* sControl = dynamic_cast<T*>(Cnt);
	if(sControl)
	{
		wstr = Strs->GetSectionStringW(WideToString(sControl->Name).c_str());
		if(wstr[0])sControl->Caption = wstr;

		wstr = Strs->GetSectionStringW(WideToString(sControl->Name+L".Hint").c_str());
		if(wstr[0])
		{
			sControl->Hint = wstr;
			sControl->ShowHint = true;
		}
		else sControl->ShowHint=TotalHints;

		wstr = Strs->GetSectionStringW(WideToString(sControl->Name+L".Glyph").c_str());
		if(wstr[0])
		{
			sControl->ImageIndex = InterfaceModule->GetMenuImage(wstr);
		}
		return true;
	}
	return false;
 }

 template <typename T>
 bool LabeledConf(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	dcmapWCSTR str;
	T* sControl = dynamic_cast<T*>(Cnt);
	if(sControl)
	{
		FontConf(sControl);
		str = Strs->GetSectionStringW(WideToString(sControl->Name).c_str());
		if(str[0])sControl->BoundLabel->Caption = str;
		str = Strs->GetSectionStringW(WideToString(sControl->Name+L".Hint").c_str());
		if(str[0])
		{
			sControl->Hint = str;
			sControl->ShowHint=true;
						if(sControl->BoundLabel)
						{
							//sControl->BoundLabel->Hint = str;
							//sControl->BoundLabel->ShowHint=true;
						}
		}
		else if(TotalHints)sControl->ShowHint=true;
		return true;
	}
	return false;
 }


 bool ListViewConf(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	dcmapWCSTR str;
	TsListView* sControl = dynamic_cast<TsListView*>(Cnt);
	if(sControl)
	{
		LabeledConf<TsListView>(Cnt,Strs);
		int n = sControl->Columns->Count;
		for(int i=0;i<n;i++)
		{
			str = Strs->GetSectionStringW(WideToString(sControl->Name+L".Column"+IntToStr(i)).c_str());
			if(str[0])
			{
				 sControl->Columns->Items[i]->Caption = str;
			}
		}

		return true;
	}
	return false;
 }


 template <typename T>
 bool MenuConf(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	char const* str;
	T* sControl = dynamic_cast<T*>(Cnt);
	if(sControl)
	{
		sControl->Images = InterfaceModule->MenuImageList;
		return true;
	}
	return false;
 }

 bool StickyLabelConf(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	TsStickyLabel* sControl = dynamic_cast<TsStickyLabel*>(Cnt);
	if(!sControl)return false;

	FontConf(sControl);
	wchar_t const* str;
	wchar_t const* hint;
	if(sControl->AttachTo)
	{
		str = Strs->GetSectionStringW(WideToString(sControl->AttachTo->Name).c_str());
		hint = Strs->GetSectionStringW(WideToString(sControl->AttachTo->Name+".Hint").c_str());
	}
	else
	{
		str = Strs->GetSectionStringW(WideToString(sControl->Name).c_str());
		hint = Strs->GetSectionStringW(WideToString(sControl->Name+".Hint").c_str());
	}
	if(str[0])sControl->Caption = str;
	if(hint[0])
	{
		sControl->Hint = hint;
		sControl->ShowHint=true;
	}
	else sControl->ShowHint=TotalHints;
	return true;
}

 template <typename T>
 bool FontHintConf(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	dcmapWCSTR str;
	T* sControl = dynamic_cast<T*>(Cnt);
	if(sControl)
	{
		FontConf(sControl);
		str = Strs->GetSectionStringW(WideToString(sControl->Name+".Hint").c_str());
		if(str[0])
		{
			sControl->Hint = str;
			sControl->ShowHint=true;
		}
		else sControl->ShowHint=TotalHints;
		return true;
	}
	return false;
 }

 template <typename T>
 bool ImgListCont(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	dcmapWCSTR str;
	T* sControl = dynamic_cast<T*>(Cnt);
	if(sControl)
	{
		FontConf(sControl);
		sControl->Images = InterfaceModule->MenuImageList;
		return true;
	}
	return false;
 }

 bool SkinProviderConf(TComponent* Cnt,IDCMapLocalStrings* Strs)
 {
	dcmapCSTR str;
	dcmapWCSTR wstr;

	TsSkinProvider
	* sSkin = dynamic_cast<TsSkinProvider*>(Cnt);
	if(!sSkin) return false;

		str = Strs->GetSectionString("Glyph");
		if(str[0])
		{
			int Index = InterfaceModule->GetMenuImage(str);
			if(Index >=0)
			{
				sSkin->ShowAppIcon = true;
				sSkin->TitleIcon->Width = 16;
				sSkin->TitleIcon->Height = 16;
				sSkin->TitleIcon->Glyph->Assign(InterfaceModule->MenuImage(Index));
			}
		}
		return true;
 }

void LocalizeComponent(TComponent* Cnt,IDCMapLocalStrings* Strs)
{
	if(!Cnt)return;
	if(!Strs)Strs = g_pLocalStrings;

	if(CaptionConfGlyph<TsSpeedButton>(Cnt,Strs))return;
	if(CaptionConfGlyph<TsBitBtn>(Cnt,Strs))return;

	if(CaptionConf<TCustomLabel>(Cnt,Strs))return;
	if(CaptionConf<TRadioButton>(Cnt,Strs))return;
	if(CaptionConf<TsCheckBox>(Cnt,Strs))return;
	if(CaptionConf<TButton>(Cnt,Strs))return;
	if(CaptionConf<TSpeedButton>(Cnt,Strs))return;
	if(CaptionConf<TBitBtn>(Cnt,Strs))return;

	if(SkinProviderConf(Cnt,Strs))return;
	if(StickyLabelConf(Cnt,Strs))return;
	if(CaptionConf<TsWebLabel>(Cnt,Strs))return;
	if(CaptionConf<TsTabSheet>(Cnt,Strs))return;
	if(CaptionConf<TGroupBox>(Cnt,Strs))return;
	if(CaptionConfToolButton<TToolButton>(Cnt,Strs))return;

	if(CaptionConfMenuItem<TMenuItem>(Cnt,Strs))return;

	if(LabeledConf<TsEdit>(Cnt,Strs))return;
	if(LabeledConf<TsFilenameEdit>(Cnt,Strs))return;
	if(LabeledConf<TsListBox>(Cnt,Strs))return;
	if(LabeledConf<TsComboBox>(Cnt,Strs))return;
	if(LabeledConf<TsComboEdit>(Cnt,Strs))return;
	if(LabeledConf<TsComboBoxEx>(Cnt,Strs))return;
	if(LabeledConf<TsMemo>(Cnt,Strs))return;
	if(LabeledConf<TsTreeView>(Cnt,Strs))return;
	if(LabeledConf<TsCheckListBox>(Cnt,Strs))return;

	if(ListViewConf(Cnt,Strs))return;

	if(HintConf<TTrackBar>(Cnt,Strs))return;
	if(HintConf<TShape>(Cnt,Strs))return;
	if(ImageConf<TImage>(Cnt,Strs))return;

	if(MenuConf<TPopupMenu>(Cnt,Strs))return;

	if(FontHintConf<TEdit>(Cnt,Strs))return;

	if(ImgListCont<TToolBar>(Cnt,Strs))return;

	if(CaptionConf<TsLabelFX>(Cnt,Strs))return;
}

void LocalizeContainer(TControl* Base,IDCMapLocalStrings* Strs)
{
	if(!Strs)Strs = g_pLocalStrings;

	unsigned n = Base->ComponentCount;

	AnsiString Section = WideToString("UI::"+Base->Name);

	g_pLocalStrings->SelectSection(Section.c_str());

	dcmapWCSTR str;
	dcmapWCSTR wstr;


	TForm* sForm = dynamic_cast<TForm*>(Base);
	if(sForm)
	{
        sForm->Scaled = false;
		FontConf(sForm);

		wstr = Strs->GetSectionStringW("Caption");
		if(wstr[0])sForm->Caption = wstr;

		str = Strs->GetSectionStringW("Transparency");
		if(str[0])
		{
			float t=StringToFloat(str);
			if(t>0)
			{
				sForm->AlphaBlend = true;
				sForm->AlphaBlendValue = (1-t)*255;
			}
		}
	}
 	TFrame* sFrame = dynamic_cast<TFrame*>(Base);
	if(sFrame)
    {
        FontConf(sFrame);
    }
	TBaseModuleFrame* sBaseModuleFrame = dynamic_cast<TBaseModuleFrame*>(Base);
	if(sBaseModuleFrame)
	{
		wstr = Strs->GetSectionStringW("Caption");
		if(wstr[0])sBaseModuleFrame->Caption = wstr;

		str = Strs->GetSectionStringW("Hint");
		if(str[0])
		{
			sBaseModuleFrame->Hint = str;
			sBaseModuleFrame->ShowHint=true;
		}
        str = Strs->GetSectionStringW("Glyph");
		if(str[0])
		{
			sBaseModuleFrame->ImageIndex = InterfaceModule->GetMenuImage(str);
		}
    }

	for(int i=0;i<n;i++)
	{
		TComponent * Cnt = Base->Components[i];
		if(!Cnt)continue;

		TFrame* SubFrame = dynamic_cast<TFrame*>(Cnt);
		if(SubFrame)
		{
			LocalizeContainer(SubFrame,Strs);
			g_pLocalStrings->SelectSection(Section.c_str());
		}
		else
		{
			LocalizeComponent(Cnt,Strs);
        }
	}
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
