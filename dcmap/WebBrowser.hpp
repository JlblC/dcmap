// CodeGear C++Builder
// Copyright (c) 1995, 2008 by CodeGear
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Webbrowser.pas' rev: 20.00

#ifndef WebbrowserHPP
#define WebbrowserHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Variants.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Webbase.hpp>	// Pascal unit
#include <Sframeadapter.hpp>	// Pascal unit
#include <Olectrls.hpp>	// Pascal unit
#include <Shdocvw_ewb.hpp>	// Pascal unit
#include <Ewbcore.hpp>	// Pascal unit
#include <Embeddedwb.hpp>	// Pascal unit
#include <Comctrls.hpp>	// Pascal unit
#include <Sstatusbar.hpp>	// Pascal unit
#include <Toolwin.hpp>	// Pascal unit
#include <Stoolbar.hpp>	// Pascal unit
#include <Stdctrls.hpp>	// Pascal unit
#include <Scombobox.hpp>	// Pascal unit
#include <Ieaddress.hpp>	// Pascal unit
#include <Scomboboxes.hpp>	// Pascal unit
#include <Sgauge.hpp>	// Pascal unit
#include <Extctrls.hpp>	// Pascal unit
#include <Spanel.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <Slabel.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Webbrowser
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TWebBrowserFrame;
class PASCALIMPLEMENTATION TWebBrowserFrame : public Webbase::TWebBaseFrame
{
	typedef Webbase::TWebBaseFrame inherited;
	
__published:
	Sstatusbar::TsStatusBar* StatusBar;
	Stoolbar::TsToolBar* sToolBar1;
	Comctrls::TToolButton* GoBtn;
	Comctrls::TToolButton* BackBtn;
	Comctrls::TToolButton* ForwardBtn;
	Comctrls::TToolButton* ReloadBtn;
	Scombobox::TsComboBox* UrlBox;
	Sgauge::TsGauge* ProgressGauge;
	Spanel::TsPanel* sPanel1;
	Embeddedwb::TEmbeddedWB* WB;
	Comctrls::TToolButton* LoginBtn;
	Slabel::TsLabel* UserNameLabel;
	void __fastcall GoBtnClick(System::TObject* Sender);
	void __fastcall UrlBoxKeyDown(System::TObject* Sender, System::Word &Key, Classes::TShiftState Shift);
	void __fastcall WBBeforeNavigate2(System::TObject* ASender, const _di_IDispatch pDisp, System::OleVariant &URL, System::OleVariant &Flags, System::OleVariant &TargetFrameName, System::OleVariant &PostData, System::OleVariant &Headers, System::WordBool &Cancel);
	void __fastcall WBProgressChange(System::TObject* ASender, int Progress, int ProgressMax);
	void __fastcall WBStatusTextChange(System::TObject* ASender, const System::WideString Text);
	void __fastcall BackBtnClick(System::TObject* Sender);
	void __fastcall ForwardBtnClick(System::TObject* Sender);
	void __fastcall ReloadBtnClick(System::TObject* Sender);
	void __fastcall WBNavigateComplete2(System::TObject* ASender, const _di_IDispatch pDisp, System::OleVariant &URL);
	void __fastcall WBEvaluateNewWindow(Ewbcore::TCustomEmbeddedWB* Sender, System::WideChar * pszUrl, System::WideChar * pszName, System::WideChar * pszUrlContext, System::WideChar * pszFeatures, BOOL fReplace, unsigned dwFlags, unsigned dwUserActionTime, HRESULT &Rezult);
	
private:
	System::UnicodeString UserName;
	
public:
	virtual void __fastcall Go(System::UnicodeString URL)/* overload */;
	virtual void __fastcall Go(System::UnicodeString URL, System::UnicodeString Post)/* overload */;
	virtual void __fastcall Wait(unsigned Time);
	virtual void __fastcall SetUserName(System::UnicodeString Name);
	virtual void __fastcall SetupBrowser(const Webbase::TBrowserParams* Params);
public:
	/* TCustomFrame.Create */ inline __fastcall virtual TWebBrowserFrame(Classes::TComponent* AOwner) : Webbase::TWebBaseFrame(AOwner) { }
	
public:
	/* TScrollingWinControl.Destroy */ inline __fastcall virtual ~TWebBrowserFrame(void) { }
	
public:
	/* TWinControl.CreateParented */ inline __fastcall TWebBrowserFrame(HWND ParentWindow) : Webbase::TWebBaseFrame(ParentWindow) { }
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Webbrowser */
using namespace Webbrowser;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// WebbrowserHPP
