//$$---- Frame HDR ----
//---------------------------------------------------------------------------


#ifndef FilterGroupH
#define FilterGroupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sFrameAdapter.hpp"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "sLabel.hpp"
#include "dcmapsyslib.h"
#include <map>
//---------------------------------------------------------------------------
class TFilterGroupFrame : public TFrame
{
	friend class TFilterFrame;
__published:	// IDE-managed Components
	TsFrameAdapter *sFrameAdapter1;
	TsPanel *ClientPanel;
	TShape *MarkerShape;
	TsPanel *HeaderPanel;
	TsSpeedButton *CollapseButton;
	TsSpeedButton *AddBtn;
	TsLabel *sLabel1;
	void __fastcall AddBtnMouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int HitTest,
          TMouseActivate &MouseActivate);
	void __fastcall CollapseButtonClick(TObject *Sender);
private:	// User declarations
	TFilterFrame* Filter;
	TFilterGroupFrame* ParentGroup;
    int ItemCounter;
    void SetClientHeight(int h);
    bool Collapsed;

    struct FilterRecord
    {
        TFilterGroupFrame* group;
        TFrame* frame;
        IDCMapFilterItem* filter;
        IDCMapVCLPanel* panel;
        FilterRecord():group(0),frame(0),filter(0),panel(0){}
    };
    typedef std::map<TFrame*,FilterRecord> MapRecords;
    MapRecords m_mapRecords;

    bool Conjunction;
public:		// User declarations
	__fastcall TFilterGroupFrame(TComponent* Owner);
    void Init();
    void AddSubGroup();
    void AddNewItem(IDCMapFilterItem* filter,IDCMapVCLPanel* panel);
    void UpdateHeight();
    int Process(const DCMapDataReference & ref);
};
//---------------------------------------------------------------------------
extern PACKAGE TFilterGroupFrame *FilterGroupFrame;
//---------------------------------------------------------------------------
#endif
