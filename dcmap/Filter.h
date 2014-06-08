//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef FilterH
#define FilterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "FilterGroup.h"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "sScrollBox.hpp"
#include "sButton.hpp"
//---------------------------------------------------------------------------
class TFilterFrame : public TBaseModuleFrame
{
	friend class  TFilterGroupFrame;
__published:	// IDE-managed Components
	TsPanel *sPanel1;
	TPopupMenu *AddMenu;
	TMenuItem *N1;
	TsScrollBox *sScrollBox1;
	TFilterGroupFrame *FilterGroupFrame1;
	void __fastcall N1Click(TObject *Sender);
	void __fastcall AddFilterClick(TObject *Sender);
private:	// User declarations
	TFilterGroupFrame* CurrentGroup;
    IDCMapModuleManager* m_pCreator;
public:		// User declarations
    bool OnInit(IDCMapModuleManager* pCreator);
	__fastcall TFilterFrame(TComponent* Owner);
    int Process(const DCMapDataReference & ref);
};
//---------------------------------------------------------------------------
extern PACKAGE TFilterFrame *FilterFrame;
//---------------------------------------------------------------------------
#endif
