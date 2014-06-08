//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef FixedFilterH
#define FixedFilterH
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
#include "FixedFilterDB.h"
#include <Menus.hpp>
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include "sRadioButton.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
//#include "TntMenus.hpp"


const static int NumDBFilters = 9;

enum EResourceType
{
	RT_Organics,
    RT_Energy,
    RT_Minerals,
    RT_Temperature,
    RT_Size,
    RT_X,
    RT_Y,
	RT_Growth,
	RT_Open,
    RT_Credits,
    RT_MiningBase,
    RT_CreditsBase,
    RT_MiningSecond,
    RT_CreditsSecond,
    RT_Sector,
    RT_Expression
};
enum EOperation
{
	OP_EQUAL,
    OP_NOTEQUAL,
    OP_GREATER_EQUAL,
    OP_LESS_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_CNT,
    OP_NOTCNT
};

enum EFilterMode
{
	FMODE_AND,
	FMODE_OR,
	FMODE_NONE,
};

enum EOperationTable
{
    OP_RESOURCE,
	OP_PLANETS,
    OP_PLAYERS,
    OP_ORG,
};

struct BaseCachedData
{
	virtual ~BaseCachedData(){}
};

class FilterDef
{
public:
    bool m_bResourceFilter;
    EResourceType Resource;
	EOperation Operation;
	EFilterMode Mode;
    float Value;

    EOperationTable table;
    int iField;
    bool    bStrValue;
    String  StrValue;
    DCMapDataReference* RefValue;
    IDCMapDataStorage* pStorage;

	mutable BaseCachedData* pCachedData;
    mutable IDCMapModule* pProcessModule;
	IDCMapWorkspace* pWorkspace;

    void Init(IDCMapWorkspace* p)
    {
       if(pCachedData)delete pCachedData;
       pCachedData = 0;
       if(pProcessModule && pWorkspace)
       		pWorkspace->DestroyModule(pProcessModule);
       pProcessModule = 0;
       pWorkspace = p;
    }
  
    FilterDef():table(OP_RESOURCE),m_bResourceFilter(false),pCachedData(0),pProcessModule(0),pWorkspace(0),pStorage(0){}
    ~FilterDef(){Init(pWorkspace);}
};

//---------------------------------------------------------------------------
class CMapFixedFilter : public IDCMapPlanetDataFilter
{
public:
	CMapFixedFilter();
    ~CMapFixedFilter();
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CMapFixedFilter();}
	virtual char const* DCMAPAPI ModuleID(){return "CMapFixedFilter";};
private:
	friend class TFixedFilterFrame;
	friend class TFixedFilterDBFrame;

    IDCMapGeoValueCalculator* m_pGeoCalc;
    FilterDef m_fdef[NumDBFilters];

    bool Process(FilterDef &def, DCMapPlanetInfo const* pinf,DCMapPlanetValueInfo& val);

	IDCMapWorkspace* m_pWorkspace;
	IDCMapPlayersDataStorage* m_pPlayers;
	IDCMapPlanetDataStorage* m_pPlanets;
	IDCMapPlanetPlayerDataStorage* m_pPlanetPlayer;

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace);

	void  DCMAPAPI LoadConfig(IDCMapConfig* pConfig);
	void  DCMAPAPI SaveConfig(IDCMapConfig* pConfig);

	bool __fastcall ProcessOpen(FilterDef const& pFDef,DCMapPlanetParamInfo const& pinf);

	int DCMAPAPI OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	void* DCMAPAPI CastTo(char const* szInterface);
	bool DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* data,DCMapPlanetInfo const* pinf);

    bool DCMAPAPI ActualizeData();

	bool Changed;
	int OpMode;
};

//---------------------------------------------------------------------------
class CMapGeoFixedFilter : public IDCMapGeoDataFilter
{
public:
	CMapGeoFixedFilter();
	~CMapGeoFixedFilter();
    static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CMapGeoFixedFilter();}
	virtual char const* DCMAPAPI ModuleID(){return "CMapGeoFixedFilter";};
private:
	friend class TFixedFilterFrame;
	friend class TFixedFilterDBFrame;

    FilterDef m_fdef[NumDBFilters];

	void* DCMAPAPI CastTo(char const* szInterface);

	bool DCMAPAPI ProcessGeoData(DCMapPlanetParamInfo const* pGeo,DCMapPlanetValueInfo const* pValue);
    bool DCMAPAPI ActualizeData();

	bool Changed;
	int OpMode;
};


class TFixedFilterFrame : public TBaseModuleFrame
{
	friend class TFixedFilterDBFrame;
__published:	// IDE-managed Components
	TPopupMenu *FilterMenu;
	TMenuItem *Y_num;
	TMenuItem *X_num;
	TMenuItem *planets_owner;
	TMenuItem *players_dip;
	TMenuItem *planets_caption;
	TMenuItem *planets_icon1;
	TMenuItem *planets_icon2;
	TMenuItem *planets_notes;
	TMenuItem *Credits_num;
	TMenuItem *MiningBase_num;
	TsPanel *TopPanel;
	TMenuItem *players_organization;
	TMenuItem *Growth_num;
	TMenuItem *CreditsBase_num;
	TMenuItem *MiningSecond_num;
	TMenuItem *CreditsSecond_num;
	TsSpinEdit *NumFiltersEdit;
	TMenuItem *Sector_str;
	TMenuItem *Size_num;
	TMenuItem *Temperature_num;
	TMenuItem *Minerals_num;
	TMenuItem *Energy_num;
	TMenuItem *Organics_num;
	TMenuItem *ValueMenu;
	TMenuItem *PlanetMenu;
	TMenuItem *OwnerMenu;
	TMenuItem *PositionMenu;
	TMenuItem *GeoMenu;
	TMenuItem *Expression_custom;
	TsCheckBox *NoFilterCheckBox;
	TMenuItem *open_players;
	TTimer *DelayTimer;
	TsSpeedButton *LoadButton;
	TsSpeedButton *SaveButton;
	TsSpeedButton *ShadeButton;
	TsSpeedButton *OpModeButton;
	void __fastcall UpdateFilter(TObject *Sender);
	void __fastcall NoFilterCheckBoxClick(TObject *Sender);
	void __fastcall FilterMenuClick(TObject *Sender);
	void __fastcall NumFiltersEditChange(TObject *Sender);
	void __fastcall ShadeButtonClick(TObject *Sender);
	void __fastcall DelayTimerTimer(TObject *Sender);
	void __fastcall LoadButtonClick(TObject *Sender);
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall OpModeButtonClick(TObject *Sender);
private:	// User declarations
	void UpdateData();
	CMapFixedFilter* m_pFilter;
    CMapGeoFixedFilter* m_pGeoFilter;

	String m_strOwner;
    TFixedFilterDBFrame* CurrentFrame;

	TFixedFilterDBFrame* Frames[NumDBFilters];

	void LoadParams(DCMapConfigWrapper cfg);

    void CreateFilterFrame(int n,int h);

	int m_iNumFilters;
	int m_iOpMode;

    bool m_bFilterChanged;

public:		// User declarations
	__fastcall TFixedFilterFrame(TComponent* Owner);

        void FilterChanged();
protected:
    IDCMapPlayersDataStorage* m_pPlayers;
    bool OnInit();
    int OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	void OnShow();
    void LoadConfig(DCMapConfigWrapper cfg);
    void SaveConfig(DCMapConfigWrapper cfg);
};
//---------------------------------------------------------------------------
extern PACKAGE TFixedFilterFrame *FixedFilterFrame;
//---------------------------------------------------------------------------
#endif
