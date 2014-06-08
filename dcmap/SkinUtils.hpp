// CodeGear C++Builder
// Copyright (c) 1995, 2008 by CodeGear
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Skinutils.pas' rev: 20.00

#ifndef SkinutilsHPP
#define SkinutilsHPP

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
#include <Sskinmanager.hpp>	// Pascal unit
#include <Stdctrls.hpp>	// Pascal unit
#include <Sskinprovider.hpp>	// Pascal unit
#include <Extctrls.hpp>	// Pascal unit
#include <Sgraphutils.hpp>	// Pascal unit
#include <Sconst.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Skinutils
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall SkinPaintItem(Controls::TControl* Control, const Types::TRect &Rect, Graphics::TCanvas* Canvas, System::UnicodeString SkinSection, Sskinmanager::TsSkinManager* Manager);

}	/* namespace Skinutils */
using namespace Skinutils;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// SkinutilsHPP
