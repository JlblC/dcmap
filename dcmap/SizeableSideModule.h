//---------------------------------------------------------------------------

#ifndef SizeableSideModuleH
#define SizeableSideModuleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSizeableSideFrame : public TBaseModuleFrame
{
	typedef TBaseModuleFrame Inherited;
__published:	// IDE-managed Components
	TsDragBar *BottomSizeBar;
	void __fastcall BottomSizeBarMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall BottomSizeBarMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
private:	// User declarations
	int BottomSizeBarLastMousePos;
protected:
	void SaveConfig(DCMapConfigWrapper cfg);
	void LoadConfig(DCMapConfigWrapper cfg);

public:		// User declarations
	__fastcall TSizeableSideFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSizeableSideFrame *SizeableSideFrame;
//---------------------------------------------------------------------------
#endif
