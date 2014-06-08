
struct IDCMapPlanetManager : public IDCMapModule
{
	virtual void DCMAPAPI SetCurrentPlanet(dcmapDBREF const* ref,int x,int y,int show=1)=0;

	virtual void DCMAPAPI EditCurrentPlanet()=0;
	virtual void DCMAPAPI DeleteCurrentPlanet()=0;

	virtual void DCMAPAPI EditPlanet(dcmapDBREF ref)=0;
	virtual void DCMAPAPI EditPlanetAt(int x,int y)=0;

	virtual bool DCMAPAPI GetCurrentPlanet(dcmapDBREF* ref)=0;
	virtual bool DCMAPAPI GetCurrentPlanetCoords(int* x,int* y)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPlanetManager);

struct DCMapPlanetValueInfo
{
	int good;
	float Growth;
	float Primary;
	float PrimaryCredit;
	float Secondaty;
	float SecondatyCredit;
};

struct IDCMapGeoValueCalculator : public IDCMapModule
{
	virtual int DCMAPAPI  CalculatePlanet(dcmapDBREF const& ref, 
		float* pGrowth,float* pPrimary=0,float* fPrimaryCredit=0,
		float* pSecondaty=0,float* pSecondatyCredit=0)=0;

	virtual int DCMAPAPI  CalculateGeo(DCMapPlanetGeoInfo const* geo, 
		float* pGrowth,float* pPrimary=0,float* fPrimaryCredit=0,
		float* pSecondaty=0,float* pSecondatyCredit=0)=0;

	virtual int DCMAPAPI  CalculateGeoIndirect(DCMapPlanetGeoInfo const* geo, DCMapPlanetValueInfo* value)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapGeoValueCalculator);

struct IDCMapRecordImport : public IDCMapModule
{
	virtual bool DCMAPAPI  ImportClipboard(dcmapCSTR strTable, dcmapDBREF const* CurrentRecord)=0;
	virtual bool DCMAPAPI  ImportSequence(dcmapCSTR strTable,dcmapDCSTR format, dcmapDCSTR data)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapRecordImport);

struct IDCMapRecordExport : public IDCMapModule
{
	virtual bool DCMAPAPI  ExportClipboard(dcmapCSTR strTable,dcmapDCSTR strFormat,dcmapDBREF const* CurrentRecord)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapRecordExport);

class IDCMapPlanetCalculator : public IDCMapModule
{
public:
	struct PlanetParamInfo
	{
		int   surface;

		int   people;
		int   people_consume;
		int   people_free;
		int   people_max;
		int   people_grow;

		float production;
		float basis;
		float second;
		float credits;
		float growth;
		float corruption_mod;

		float race_t;
		float race_dt;
		float race_tp;
		float race_growth;

		dcmapDWORD BuildMask;
		dcmapDWORD BuildUnique;

		double res_bonus[3];
		double resources[3];
		double prod_bonus;

		int basis_ind;
		int second_ind;
		int nature_ind;
		int corruption;

		int planet_o;
		int planet_e;
		int planet_m;
		int planet_t;
		int planet_s;

	};

	struct PlanetProdInfo
	{
		int   people_consume;
		int   people_free;
		int   people_max;
		float production;

		float nature;
		float basis;
		float second;
		float credits;
	};

	struct PlayerProdInfo
	{
		float nature;
		float basis;
		float second;
		float credits;

		int population;
		int free_population;
		int governors;
	};

	enum EFlags
	{
		PRESIZE,WITHOUT_CORRUPTION,
	};

	virtual bool DCMAPAPI ProcessPlanetParams(dcmapDBREF const& ref, PlanetParamInfo* res,int Flags=0)=0;


	virtual bool DCMAPAPI ProcessPlanet(dcmapDBREF const& ref,PlanetProdInfo* res)=0;
	virtual bool DCMAPAPI ProcessPlanetIndirect(DCMapPlanetInfo const* pInf,PlanetProdInfo* res)=0;

	virtual bool DCMAPAPI CalculateCorruption(dcmapDBREF const& player)=0;
	virtual bool DCMAPAPI CalculatePlayerIncome(dcmapDBREF const& player,PlayerProdInfo* res)=0;

	virtual bool DCMAPAPI CalculateQueue(dcmapDBREF const& FleetRef, PlanetParamInfo const* planetparams,int Flags=0)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPlanetCalculator);

// Service FleetManager
struct IDCMapFleetManager : public IDCMapModule
{
	struct FleetJumpInfo
	{
		dcmapDBREF Fleet;
		float Range;
		float Speed;
		int MaxTurns;
		int x,y;
		bool JumpFromOpen;
	};

	virtual bool DCMAPAPI CreateFleetAt(int x,int y,dcmapDBREF const& Owner,dcmapWCSTR name,dcmapDBREF* pResult)=0;
	virtual bool DCMAPAPI SelectFleet(dcmapDBREF const* pFleet)=0;
	virtual bool DCMAPAPI UseFleet(dcmapDBREF const& Fleet)=0;

	virtual int DCMAPAPI TestJump(int x,int y, bool* PlanetJumpable =0)=0;
	virtual FleetJumpInfo const* DCMAPAPI GetJumpInfo()=0;

// PUBLISHED

	virtual bool DCMAPAPI CanMoveFleetTo(dcmapDBREF const& Fleet,int x,int y)=0;
    virtual bool DCMAPAPI MoveFleetTo(dcmapDBREF const& Fleet,int x,int y)=0;

	virtual bool DCMAPAPI CanCancelJump(dcmapDBREF const& Fleet)=0;
	virtual bool DCMAPAPI CancelJump(dcmapDBREF const& Fleet)=0;

    virtual bool DCMAPAPI MoveUnitToFleet(dcmapDBREF const& Unit,dcmapDBREF const& Fleet)=0;
	virtual bool DCMAPAPI CanMoveUnitToFleet(dcmapDBREF const& Unit,dcmapDBREF const& Fleet)=0;

	virtual bool DCMAPAPI DisbandUnit(dcmapDBREF const& Unit)=0;
	virtual bool DCMAPAPI CanDisbandUnit(dcmapDBREF const& Unit)=0;
	virtual bool DCMAPAPI DeleteFleet(dcmapDBREF const& Fleet)=0;

	virtual bool DCMAPAPI RenameFleet(dcmapDBREF const& Fleet,dcmapWCSTR name,dcmapWCSTR public_name=0)=0;

	virtual bool DCMAPAPI CalculateFleet(dcmapDBREF const& Fleet)=0;

	virtual bool DCMAPAPI AddUnitToQueue(dcmapDBREF const& Planet,dcmapDBREF const& Project )=0;

	virtual bool DCMAPAPI SetFleetIcon(dcmapDBREF const& Fleet,dcmapDBREF const& Icon )=0;

	virtual bool DCMAPAPI CanExecuteUnitAction(dcmapDBREF const& Unit,dcmapDBREF const& Action )=0;
	virtual bool DCMAPAPI ExecuteUnitAction(dcmapDBREF const& Unit,int ActionID)=0;
	virtual bool DCMAPAPI CancelUnitAction(dcmapDBREF const& Unit,int ActionID)=0;

	virtual bool DCMAPAPI CanHideFleet(dcmapDBREF const& Fleet,int iMode)=0;
	virtual bool DCMAPAPI HideFleet(dcmapDBREF const& Fleet,int iMode)=0;

	virtual bool DCMAPAPI ChangeBehavior(dcmapDBREF const& Fleet,dcmapDWORD iBehavior)=0;

	virtual bool DCMAPAPI UnpackContainer(dcmapDBREF const& Container)=0;
	virtual bool DCMAPAPI CanUnpackContainer(dcmapDBREF const& Container)=0;

	virtual bool DCMAPAPI UpdateFleet(dcmapDBREF const& Fleet)=0;

	virtual bool DCMAPAPI UpdateFleetGroups(dcmapDBREF const& Fleet)=0;
	virtual void DCMAPAPI BeginUpdate()=0;
	virtual void DCMAPAPI EndUpdate()=0;

	virtual bool DCMAPAPI CanLoadCargo(dcmapDBREF const& Container,dcmapDBREF const& Fleet)=0;
	virtual bool DCMAPAPI LoadCargo(dcmapDBREF const& Container,dcmapDBREF const& Fleet)=0;
	virtual bool DCMAPAPI CanCancelCargoLoad(dcmapDBREF const& Container)=0;
	virtual bool DCMAPAPI CancelCargoLoad(dcmapDBREF const& Container)=0;

	virtual dcmapDBREF const* DCMAPAPI GetGarrison(int x,int y)=0;
	virtual dcmapDBREF const* DCMAPAPI GetBuildings(int x,int y)=0;

};
DCMAP_SPEC_INTERFACE(IDCMapFleetManager);

struct IDCMapActionProcessor : public IDCMapModule
{
    virtual dcmapDCSTR DCMAPAPI MakeRequest(dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID=0,int ForTurn=0,dcmapINT* pActionsSent=0)=0;
    virtual int DCMAPAPI ProcessResponse(dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID,dcmapDCSTR rText)=0;
	virtual bool DCMAPAPI HaveActions(dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID=0)=0;
	virtual bool DCMAPAPI DropActions(dcmapDCSTR szPlayer, dcmapDBREF const* pPlayerID=0)=0;
	virtual void DCMAPAPI UpdateActions(bool Force=false)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapActionProcessor);

struct IDCMapFleetGrouping : public IDCMapModule
{
    virtual bool DCMAPAPI ProcessFleets()=0;
	virtual bool DCMAPAPI UpdateFleetGroups(dcmapDBREF const& ref)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapFleetGrouping);


struct IDCMapWebProcess : public IDCMapModule
{
	virtual dcmapDCSTR DCMAPAPI WebGet(dcmapDCSTR URL,IDCMapProcessRecordReseiver *pRec=0,int CodePage = DCMAP_CP_UTF8)=0;
	virtual dcmapDCSTR DCMAPAPI WebPost(dcmapDCSTR URL,dcmapDCSTR PostData,IDCMapProcessRecordReseiver *pRec=0,int CodePage = DCMAP_CP_UTF8)=0;
	virtual bool DCMAPAPI GameLogin(dcmapDCSTR Login,dcmapDCSTR Pass,bool free,IDCMapProcessRecordReseiver *pRec=0)=0;
	virtual bool DCMAPAPI GameLogout(IDCMapProcessRecordReseiver *pRec=0)=0;
	virtual dcmapDCSTR DCMAPAPI GameGet(dcmapDCSTR URL,IDCMapProcessRecordReseiver *pRec=0)=0;
	virtual dcmapDCSTR DCMAPAPI GamePost(dcmapDCSTR URL,dcmapDCSTR PostData,IDCMapProcessRecordReseiver *pRec=0)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapWebProcess);



