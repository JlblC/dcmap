//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef PlanetGarrionEditH
#define PlanetGarrionEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sListView.hpp"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TPlanetGarrisonFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
	__fastcall TPlanetGarrisonFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPlanetGarrisonFrame *PlanetGarrisonFrame;
//---------------------------------------------------------------------------
#endif
