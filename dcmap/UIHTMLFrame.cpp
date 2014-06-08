//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UIHTMLFrame.h"
#include "Utils.h"
#include "sSkinManager.hpp"
#include "sEdit.hpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sLabel"
#pragma link "Htmlview"
#pragma resource "*.dfm"

THTMLFrame *HTMLFrame;

static CFramePanelWrapper<THTMLFrame> sWrap("UI","HTMLFrame");

IDCMapModule* DCMAPAPI CUIHTMLFrame::GetFinalModule()
{
	return pFrame->Module()->GetFinalModule();
}

bool DCMAPAPI CUIHTMLFrame::InitUI(IDCMapModule* pOwner,int id)
{
	return pFrame->Module()->InitUI(pOwner,id);
};

bool DCMAPAPI CUIHTMLFrame::LoadHTML(dcmapWCSTR pHTML,dcmapWCSTR pbaseURL,IDCMapModule* pProcessModule)
{return pFrame->LoadHTML(pHTML,pbaseURL,pProcessModule);}
void DCMAPAPI CUIHTMLFrame::Reload(IDCMapModule* pProcessModule)
{pFrame->Reload(pProcessModule);}

bool DCMAPAPI CUIHTMLFrame::SetObjectAttribute(dcmapWCSTR szObjID,dcmapWCSTR szAttribute,dcmapWCSTR szValue)
{return pFrame->SetObjectAttribute(szObjID,szAttribute,szValue);}
bool DCMAPAPI CUIHTMLFrame::SetObjectVisible(dcmapWCSTR szObjID,bool bVisible)
{return pFrame->SetObjectVisible(szObjID,bVisible);}

dcmapWCSTR DCMAPAPI CUIHTMLFrame::GetObjectAttribute(dcmapWCSTR szObjID,dcmapWCSTR szAttribute)
{return pFrame->GetObjectAttribute(szObjID,szAttribute);}
bool DCMAPAPI CUIHTMLFrame::GetObjectVisible(dcmapWCSTR szObjID)
{return pFrame->GetObjectVisible(szObjID);}
int DCMAPAPI CUIHTMLFrame::OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{ return pFrame->OnMessage(MSG,wParam,lParam,pSender); }
void DCMAPAPI CUIHTMLFrame::Reformat()
{ pFrame->Html->Reformat();}


//---------------------------------------------------------------------------
__fastcall THTMLFrame::THTMLFrame(TComponent* Owner):
		m_UIFrame(this),
		TBaseModuleFrame(Owner)
{
	inCallback = false;
	inProcess = false;
	m_bInside = false;
}

void* DCMAPAPI CUIHTMLFrame::CastTo(DCMapInterfaceID idInterface)
{
	DCMAP_MODULE_SUPPORTS(IDCMapUIHTMLFrame);
	DCMAP_MODULE_SUPPORTS(IDCMapUIFrameItem);
	return pFrame->Module()->CastTo(idInterface);
}

void* DCMAPAPI THTMLFrame::GetInterface(DCMapInterfaceID idInterface)
{
	if(DCMAP_INTERFACE_CMP(idInterface,IDCMapUIHTMLFrame))
		return &m_UIFrame;
	return 0;
}

void THTMLFrame::SetupColors()
{
	if(!sFrameAdapter->SkinData->SkinManager->Active)return;

	 String BgSection = sFrameAdapter->SkinData->SkinSection;
	 if(BgSection == "CHECKBOX")
		  BgSection = "FORM";

	 DynamicArray<Smaskdata::TsGeneralData > & gd =
			sFrameAdapter->SkinData->SkinManager->gd;

	 int Index = sFrameAdapter->SkinData->SkinManager->GetSkinIndex(BgSection);

	 Html->DefBackground = gd[Index].Color;
	 Html->DefFontColor = gd[Index].FontColor[0];
	 Html->DefHotSpotColor = gd[Index].FontColor[0];

	 Index = sFrameAdapter->SkinData->SkinManager->GetSkinIndex("WEBBUTTON");

	 //Html->DefHotSpotColor = gd[Index].FontColor[0];
	 //Html->DefOverLinkColor = gd[Index].HotColor;
	 //Html->DefVisitedLinkColor = gd[Index].FontColor[0];
}
bool THTMLFrame::DoLoadHTML()
{
	if(m_bInside) return false;

	int v = Html->VScrollBarPosition;
	int h = Html->HScrollBarPosition;

	String PreMetaSection = MetaSection;
	Html->LoadFromString(CurrentHtml,BaseURL);
	Caption = Html->DocumentTitle;
	if(PreMetaSection != MetaSection)
	{
		 SetupColors();
		 Html->Reformat();
	}

	Html->VScrollBarPosition=v;
	Html->HScrollBarPosition=h;
	return true;
}

bool THTMLFrame::LoadHTML(dcmapWCSTR pHTML,dcmapWCSTR pBaseURL,IDCMapModule* pProcessModule)
{
	MetaSection="";
	pProcessor = pProcessModule;
	SetupColors();

	CurrentHtml = pHTML;
	BaseURL = pBaseURL?pBaseURL:L"";
	return DoLoadHTML();
}

void THTMLFrame::Reload(IDCMapModule* pProcessModule)
{
	pProcessor = pProcessModule;
	if(!CurrentHtml.IsEmpty())
	{
		DoLoadHTML();
	}
}

void THTMLFrame::OnShow()
{
	SetupColors();
}

//---------------------------------------------------------------------------
void __fastcall THTMLFrame::htmlMeta(TObject *Sender, const AnsiString HttpEq,
      const AnsiString Name, const AnsiString Content)
{
	if(SameText(Name,"width"))
	{
		Width = StrToInt(Content);
	}
	else if(SameText(Name,"height"))
	{
		Height = StrToInt(Content);
	}
}
//---------------------------------------------------------------------------

void __fastcall THTMLFrame::htmlFormSubmit(TObject *Sender,
      const AnsiString Action, const AnsiString Target,
	  const AnsiString EncType, const AnsiString Method, TStringList *Results)
{
	String Text = Results->Text;
	int sz = Results->Count;
	for(int i=0;i<sz;i++)
	{
	}
}
//---------------------------------------------------------------------------

void __fastcall THTMLFrame::htmlObjectAction(TObject *Sender, TObject *Obj,
	  const AnsiString Action)
{
	m_bInside = true;
	Evalate(Action);
	m_bInside = false;
}
//---------------------------------------------------------------------------

void __fastcall THTMLFrame::HtmlScript(TObject *Sender, const AnsiString Name,
	  const AnsiString Language, const AnsiString Script)
{
	int i=0;
}
//---------------------------------------------------------------------------

void __fastcall THTMLFrame::HtmlInclude(TObject *Sender,
	  const AnsiString Command, TStrings *Params, AnsiString &IString)
{
	IString = Evalate(Params->Values["eval"]);
}
//---------------------------------------------------------------------------

bool THTMLFrame::SetObjectAttribute(dcmapWCSTR szObjID,dcmapWCSTR szAttribute,dcmapWCSTR szValue)
{
	 if(WideSameText(szAttribute,L"visible"))
	 {
		bool vis = false;
		if(szValue && szValue[0] && !WideSameText(szValue,L"0"))
			vis = true;
		return SetObjectVisible(szObjID,vis);
	 }
	 if(WideSameText(szObjID,L"width")) Width = StrToInt(szValue);
	 else if(WideSameText(szObjID,L"height")) Width = StrToInt(szValue);
	 else
	 {
		 TObject* pCntr = Html->IDControl[szObjID];
		 if(!pCntr)return false;

		 TsEdit* pEdit =  dynamic_cast<TsEdit*>(pCntr);
		 if(pEdit)
		 {
			 if(WideSameText(szAttribute,L"value"))
			 {
				 pEdit->Text = szValue;
				 return true;
			 }
		 }
		 return false;
	 }
	 return true;
}
bool THTMLFrame::SetObjectVisible(dcmapWCSTR szObjID,bool bVisible)
{
	Html->IDDisplay[szObjID] = bVisible;
	return true;
}
dcmapWCSTR THTMLFrame::GetObjectAttribute(dcmapWCSTR szObjID,dcmapWCSTR szAttribute)
{
	 static UnicodeString buf;

	 if(WideSameText(szAttribute,L"visible"))
	 {
		return GetObjectVisible(szObjID)?L"1":L"0";
	 }

	 if(WideSameText(szObjID,L"width")) buf = IntToStr(Width);
	 else if(WideSameText(szObjID,L"height")) buf = IntToStr(Height);
	 else
	 {
		 TObject* pCntr = Html->IDControl[szObjID];
		 if(!pCntr)return 0;

		 TsEdit* pEdit =  dynamic_cast<TsEdit*>(pCntr);
		 if(pEdit)
		 {
			 if(WideSameText(szAttribute,L"value"))
			 {
				 buf = pEdit->Text;
				 return buf.w_str();
			 }
		 }
		 return 0;
	 }
	 return buf.w_str();
}
bool THTMLFrame::GetObjectVisible(dcmapWCSTR szObjID)
{
	return Html->IDDisplay[szObjID];
}

void __fastcall THTMLFrame::HtmlProcessing(TObject *Sender, bool ProcessingOn)
{
	inProcess = ProcessingOn;
}
//---------------------------------------------------------------------------

String THTMLFrame::Evalate(String Script)
{
	int id = m_pPanelInterface->GetID();

	if(!Script.IsEmpty() && pProcessor)
	{
		int res = pProcessor->OnMessage(
				DCMM_UI_COMMAND_STR_FUNC,id,
				reinterpret_cast<dcmapLPARAM>(ToWide(Script).w_str()),&m_UIFrame);
		if(res && res !=1)
		{
			 return WideToString((dcmapWCSTR)res,CP_UTF8);
		}
	}
	return "";
}


void __fastcall THTMLFrame::HtmlHotSpotClick(TObject *Sender,
	  const AnsiString SRC, bool &Handled)
{
	TSysCharSet Delims;
	Delims << ':';
	String Proto = ExtractWord(1,SRC,Delims);
	Handled=false;

	if(SameText(Proto,"dcm"))
	{
		 String Script = SRC.c_str()+4;
		 Evalate(Script);
		 Handled = true;
	}
	else if(SameText(Proto,"http") || SameText(Proto,"mailto"))
	{
		ShellExecuteA(Handle, NULL, SRC.c_str(), NULL, NULL, SW_SHOWNORMAL);
		Handled = true;
	}
}
//---------------------------------------------------------------------------

void THTMLFrame::OnChnageSkin()
{
	SetupColors();
}

int THTMLFrame::OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	switch(MSG)
	{
	   case DCMM_SKIN_CHANGED:
		 SetupColors();
		 //Html->Reformat();
		 Reload(pProcessor);
		 break;
	}
}




