//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef OldFilterH
#define OldFilterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"
#include "sComboBox.hpp"
#include "sCheckBox.hpp"
#include "sEdit.hpp"
#include "sSpinEdit.hpp"
#include "sLabel.hpp"

enum EResourceType
{
	RT_Organics,
    RT_Energy,
    RT_Minerals,
    RT_Temperature,
    RT_Size,
    RT_Growth,
    RT_Credits,
    RT_MiningBase,
    RT_CreditsBase,
    RT_MiningSecond,
    RT_CreditsSecond
};
enum EOperation
{
	OP_EQUAL,
    OP_GREATER_EQUAL,
    OP_LESS_EQUAL,
    OP_GREATER,
    OP_LESS
};

enum EOperationTable
{
	OP_PLANETS,
    OP_PLAYERS,
    OP_ORG
};

struct FilterDef
{
    bool m_bResourceFilter;
    EResourceType Resource;
    EOperation Operation;
    float Value;
};

//---------------------------------------------------------------------------
class COldFixedFilter : public IDCMapPlanetDataFilter
{
public:
	COldFixedFilter();
private:
	friend class TOldFixedFilterFrame;

    IDCMapGeoValueCalculator* m_pGeoCalc;


	bool m_bPlayerFilter;
    DCMapDataReference m_refPlayer;

    bool m_bRectRegionFilter;
    TRect m_Rect;

    bool m_bCommentFilter;
    String Comment;

    FilterDef m_fdef[3];

    bool Process(EOperationTable table,int iField,EOperation op,float NumValue,const char*  StrValue,DCMapDataReference* RefValue, DCMapPlanetDisplayInfo* pinfo);


    bool Process(EResourceType rsr,EOperation op,float Value,DCMapPlanetDisplayInfo* pinfo);

	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new COldFixedFilter();}
	virtual char const* DCMAPAPI ModuleID(){return "CMapOldFixedFilter";};
	IDCMapWorkspace* m_pWorkspace;
	IDCMapPlayersDataStorage* m_pPlayers;
	IDCMapPlanetDataStorage* m_pPlanets;


	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);

	void  DCMAPAPI LoadConfig(IDCMapConfig* pConfig);
	void  DCMAPAPI SaveConfig(IDCMapConfig* pConfig);

	int DCMAPAPI OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	void* DCMAPAPI CastTo(char const* szInterface);
	bool DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* data,DCMapPlanetInfo const* pinf);

    bool DCMAPAPI ActualizeData();

    bool Changed;
};


class TOldFixedFilterFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
	TsComboBox *PlayerList;
	TsCheckBox *OwnerCheck;
	TsCheckBox *RectRegionCheck;
	TsSpinEdit *LeftEdit;
	TsSpinEdit *TopEdit;
	TsSpinEdit *RightEdit;
	TsSpinEdit *BottomEdit;
	TsLabel *sLabel1;
	TsLabel *sLabel2;
	TsComboBox *ResourceCombo1;
	TsCheckBox *ResourceCheck1;
	TsComboBox *OpCombo1;
	TsDecimalSpinEdit *ValueEdit1;
	TsComboBox *OpCombo2;
	TsCheckBox *ResourceCheck2;
	TsComboBox *ResourceCombo2;
	TsDecimalSpinEdit *ValueEdit2;
	TsCheckBox *ComentCheck;
	TsEdit *ComentEdit;
	TsCheckBox *NoFilterCheckBox;
	void __fastcall UpdateFilter(TObject *Sender);
	void __fastcall RectRegionCheckClick(TObject *Sender);
	void __fastcall ResourceCheck1Click(TObject *Sender);
	void __fastcall ResourceCheck2Click(TObject *Sender);
	void __fastcall ComentCheckClick(TObject *Sender);
	void __fastcall NoFilterCheckBoxClick(TObject *Sender);
private:	// User declarations
	void UpdateData();
	COldFixedFilter* m_pFilter;
	String m_strOwner;
public:		// User declarations
	__fastcall TOldFixedFilterFrame(TComponent* Owner);
protected:
    IDCMapPlayersDataStorage* m_pPlayers;
    bool OnInit();
    int OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	void OnShow();
    void LoadConfig(DCMapConfigWrapper cfg);
    void SaveConfig(DCMapConfigWrapper cfg);
};
//---------------------------------------------------------------------------
extern PACKAGE TOldFixedFilterFrame *OldFixedFilterFrame;
//---------------------------------------------------------------------------
#endif
