//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "WebGet.h"
#include "dcmapsyslib.h"
#include "sUtils.hpp"
#include "main.h"

//---------------------------------------------------------------------------
#include "boost/scoped_ptr.hpp"
using boost::scoped_ptr;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "IdCompressorZLibEx"
#pragma link "IdZLibCompressorBase"
#pragma link "IdCompressorZLibEx"
#pragma link "IdZLibCompressorBase"
#pragma link "IdCompressorZLibEx"
#pragma link "IdZLibCompressorBase"
#pragma link "IdCompressorZLibEx"
#pragma link "IdZLibCompressorBase"
#pragma link "IdCompressorZLibEx"
#pragma link "IdZLibCompressorBase"
#pragma link "IdCompressorZLibEx"
#pragma link "IdZLibCompressorBase"
#pragma resource "*.dfm"
TWebGetModule *WebGetModule;
//---------------------------------------------------------------------------
__fastcall TWebGetModule::TWebGetModule(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------








