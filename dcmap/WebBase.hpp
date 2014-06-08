// CodeGear C++Builder
// Copyright (c) 1995, 2008 by CodeGear
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Webbase.pas' rev: 20.00

#ifndef WebbaseHPP
#define WebbaseHPP

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
#include <Sframeadapter.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Webbase
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TBrowserParams;
class PASCALIMPLEMENTATION TBrowserParams : public System::TObject
{
	typedef System::TObject inherited;
	
public:
	System::UnicodeString UserAgend;
public:
	/* TObject.Create */ inline __fastcall TBrowserParams(void) : System::TObject() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TBrowserParams(void) { }
	
};


class DELPHICLASS TWebBaseFrame;
class PASCALIMPLEMENTATION TWebBaseFrame : public Forms::TFrame
{
	typedef Forms::TFrame inherited;
	
__published:
	Sframeadapter::TsFrameAdapter* FrameAdapter;
	Menus::TPopupMenu* LoginMenu;
	
public:
	System::UnicodeString HomePageURL;
	System::UnicodeString CurrentURL;
	virtual void __fastcall SetupBrowser(const TBrowserParams* Params) = 0 ;
	virtual void __fastcall Go(System::UnicodeString URL) = 0 /* overload */;
	virtual void __fastcall Go(System::UnicodeString URL, System::UnicodeString Post) = 0 /* overload */;
	virtual void __fastcall Wait(unsigned Time = (unsigned)(0x0)) = 0 ;
	virtual void __fastcall SetUserName(System::UnicodeString Name) = 0 ;
public:
	/* TCustomFrame.Create */ inline __fastcall virtual TWebBaseFrame(Classes::TComponent* AOwner) : Forms::TFrame(AOwner) { }
	
public:
	/* TScrollingWinControl.Destroy */ inline __fastcall virtual ~TWebBaseFrame(void) { }
	
public:
	/* TWinControl.CreateParented */ inline __fastcall TWebBaseFrame(HWND ParentWindow) : Forms::TFrame(ParentWindow) { }
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TWebBaseFrame* __fastcall CreateBrowser(Classes::TComponent* Owner);

}	/* namespace Webbase */
using namespace Webbase;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// WebbaseHPP
