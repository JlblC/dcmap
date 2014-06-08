//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef MapColorPrefH
#define MapColorPrefH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BasePrefs.h"
#include "sFrameAdapter.hpp"
#include "sButton.hpp"
#include "sColorSelect.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "sCheckBox.hpp"
#include "BaseModule.h"
//---------------------------------------------------------------------------
class TMapColorPrefFrame : public TBasePrefsFrame
{
__published:	// IDE-managed Components
	TsColorSelect *MapBackground;
	TsColorSelect *Grid4;
	TsColorSelect *Grid3;
	TsColorSelect *Grid1;
	TsColorSelect *Border;
	TsColorSelect *LineBackground;
	TsColorSelect *Text;
	TsColorSelect *Grid2;
	TsColorSelect *PointedText;
	TsColorSelect *Selection;
	TsColorSelect *SelectionPlanet;
	TsColorSelect *GeoGoodBorder;
	TsColorSelect *GeoGoodFill;
	TsColorSelect *GeoBadBorder;
	TsColorSelect *GeoBadFill;
	TsColorSelect *InactiveBorder;
private:	// User declarations
public:		// User declarations
	__fastcall TMapColorPrefFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMapColorPrefFrame *MapColorPrefFrame;
//---------------------------------------------------------------------------
#endif
