
#define DCMapNumPlanets 2000
#define DCMapCodePage   1251


struct IDCMapView;
struct IDCMapLayeredMapView;
struct IDCMapPlanetDataProcessor;
struct IDCMapPlanetDataFilter;
struct IDCMapPlanetColorProvider;
struct IDCMapLayer;
struct IDCMapGraphicLayer;
struct IDCMapProcessRecordReseiver;
struct IDCMapImportProcessor;


enum DCMapDragState { dcmap_dsDragEnter, dcmap_dsDragLeave, dcmap_dsDragMove };

struct IDCMapDragObject
{
	virtual dcmapCSTR DCMAPAPI Name()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapDragObject);

struct tagWNDCLASSW;

struct IDCMapView : public IDCMapModule
{
	virtual  dcmapHWND DCMAPAPI Create(dcmapHWND hwndParent,dcmapHWND hwndMain,dcmapRECT const* rcClient)DCMAP_IMPL_HELPER_RET(0);

	///	Вызывается при изменеии резмеров родительского окна.
	/// rcClient Клиентская область род. окна. 
	virtual void DCMAPAPI OnResize(dcmapRECT const* rcClient)DCMAP_IMPL_HELPER;
	virtual bool DCMAPAPI OnMouseWheel(int delta, int x,int y)DCMAP_IMPL_HELPER_RET(false);
	virtual void DCMAPAPI Activate(bool Active)DCMAP_IMPL_HELPER;

	virtual tagWNDCLASSW* DCMAPAPI RegisterWndClass()DCMAP_IMPL_HELPER_RET(0);
	virtual void* DCMAPAPI BeforeCreate(dcmapHWND hwndParent,dcmapHWND hwndMain)DCMAP_IMPL_HELPER_RET(0);

	virtual void DCMAPAPI AfterCreate(dcmapHWND wnd,dcmapHWND hwndParent,dcmapHWND hwndMain)DCMAP_IMPL_HELPER;

	virtual bool DCMAPAPI DragOver(int x,int y,int State,IDCMapDragObject* pDragInfo,IDCMapModule* pModule)DCMAP_IMPL_HELPER_RET(false);
	virtual bool DCMAPAPI DragDrop(int x,int y,IDCMapDragObject* pDragInfo,IDCMapModule* pModule)DCMAP_IMPL_HELPER_RET(false);
};
DCMAP_SPEC_INTERFACE(IDCMapView);


struct IDCMapLayeredMapView : public IDCMapView
{	
	virtual int DCMAPAPI NumLayers()=0;

	virtual dcmapCSTR   DCMAPAPI LayerModuleName(dcmapUINT n)=0;
	virtual dcmapWCSTR   DCMAPAPI LayerName(dcmapUINT n)=0;
	virtual dcmapDCSTR DCMAPAPI LayerCaption(dcmapUINT n)=0;
	virtual bool DCMAPAPI LayerVisible(dcmapUINT n)=0;
	virtual bool DCMAPAPI LayerListed(dcmapUINT n)=0;
	virtual void DCMAPAPI SetLayerVisiblity(dcmapUINT n,bool Visible)=0;
	virtual dcmapDBREF const* DCMAPAPI LayerDataID(dcmapUINT n)=0;

	virtual bool DCMAPAPI EditLayer(dcmapUINT n)=0;

	virtual int DCMAPAPI ActiveLayer()=0;
	virtual IDCMapLayer* DCMAPAPI LayerModule(dcmapUINT n)=0;

	virtual bool DCMAPAPI SetActiveLayer(dcmapUINT n)=0;

	virtual void DCMAPAPI UpdateLayers()=0;

	virtual IDCMapLayer* DCMAPAPI LayerInterface(dcmapUINT n)=0;

	virtual void DCMAPAPI UpdateMapRect(const dcmapRECT* rc)=0;

	virtual bool DCMAPAPI StartAnim(dcmapRECT const* rcPos,dcmapHDC* pDC,void* pGDIP)=0;
	virtual void DCMAPAPI PresentAnim()=0;
	virtual void DCMAPAPI EndAnim()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapLayeredMapView);


struct DCMapPlanetDisplayInfo
{
	DCMapPlanetParamInfo planet;

	dcmapCOLORREF clBackground;
	dcmapCOLORREF clPlanetBody;
	dcmapCOLORREF clPlanetBorder;
	dcmapCOLORREF clCellBorder;
	dcmapCOLORREF clMarker;

	short    Icon1;
	short    Icon2;
	short    Icon3;

	dcmapBYTE     iBackground;
	dcmapBYTE     iPlanetBody;
	dcmapBYTE     iPlanetBorder;
	dcmapBYTE	 iCellBorder;
	dcmapBYTE	 iMarker;
	dcmapBYTE	 iHidden;
};

struct IDCMapPlanetDataProcessor : public IDCMapDataSource
{
	virtual bool DCMAPAPI SelectLine(int LineNum,int From,int To)=0;
	virtual DCMapPlanetDisplayInfo* DCMAPAPI GetPlanet(int x, int y)=0;
	virtual DCMapPlanetDisplayInfo* DCMAPAPI GetPlanetInfo()=0;
	virtual bool DCMAPAPI ActualizeData()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPlanetDataProcessor);

struct IDCMapPlanetDataFilter : public IDCMapModule
{
	virtual bool DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* data,DCMapPlanetInfo const* pinf)=0;
	virtual bool DCMAPAPI ActualizeData()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPlanetDataFilter);

struct IDCMapGeoDataFilter : public IDCMapModule
{
	virtual bool DCMAPAPI ProcessGeoData(DCMapPlanetParamInfo const* pGeo,DCMapPlanetValueInfo const* pValue)=0;
	virtual bool DCMAPAPI ActualizeData()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapGeoDataFilter);

//////////////////////////////////////////////////////////////////////////

struct IDCMapPlanetColorProvider : public IDCMapModule
{
	virtual dcmapBYTE DCMAPAPI ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf,dcmapCOLOR base, dcmapCOLOR& res)=0;
	virtual bool DCMAPAPI ActualizeData()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPlanetColorProvider);

//////////////////////////////////////////////////////////////////////////

enum EDCMapLayerRenderFlag
{
	DCMAP_LRF_HIDDEN = (1<<0),
};

enum EDCMapLayerRenderOptions
{
	DCMAP_LRO_RENDER			= (1<<0),
	DCMAP_LRO_RENDER_HIDDEN		= (1<<1),
	DCMAP_LRO_PRERENDER			= (1<<2),
	DCMAP_LRO_PRERENDER_HIDDEN	= (1<<3),
	DCMAP_LRO_PLANET_RENDER		= (1<<4),
	DCMAP_LRO_PLANET_RENDER_HIDDEN = (1<<5),
};


struct IDCMapLayer : public IDCMapModule
{
	virtual bool DCMAPAPI InitLayer(IDCMapLayeredMapView* pView)DCMAP_IMPL_HELPER_RET(true);
	virtual bool DCMAPAPI InitUserLayer(dcmapDBREF const& ref)DCMAP_IMPL_HELPER_RET(false);

	virtual int DCMAPAPI PrepareRender(dcmapRECT const& rcBound, dcmapRECT const& rcWnd,int CellSize,int iFlags)DCMAP_IMPL_HELPER_RET(DCMAP_LRO_RENDER);

	virtual void DCMAPAPI Render(dcmapHDC dc,dcmapRECT const& rcBound, dcmapRECT const& rcMapCoord,int CellSize,int iFlags=0)=0;
	virtual void DCMAPAPI PreRender(dcmapHDC dc,dcmapRECT const& rcBound, dcmapRECT const& rcMapCoord,int CellSize,int iFlags)DCMAP_IMPL_HELPER;
	virtual bool DCMAPAPI ActualizeData()=0;
	virtual bool DCMAPAPI EditLayer()=0;

	virtual bool DCMAPAPI DragOver(int x,int y,int State,IDCMapDragObject* pDragInfo,IDCMapModule* pModule)=0;
	virtual bool DCMAPAPI DragDrop(int x,int y,IDCMapDragObject* pDragInfo,IDCMapModule* pModule)=0;

	virtual dcmapCSTR DCMAPAPI GetLayerName()=0;
	virtual dcmapDCSTR DCMAPAPI GetLayerCaption()DCMAP_IMPL_HELPER_RET(0);
};
DCMAP_SPEC_INTERFACE(IDCMapLayer);

struct IDCMapGraphicLayer : public IDCMapLayer
{
	virtual void DCMAPAPI BeginDraw(EDCMapGraphicsType type,IDCMapModule* pManager=0)=0;
	virtual void DCMAPAPI EndDraw()=0;
	virtual void DCMAPAPI CancelDraw()=0;
	virtual bool DCMAPAPI TestPoint(float x,float y,IDCMapVarValue* pResult)=0;
	virtual DCMapGraphicsInfo* DCMAPAPI GetGraphicsInfo()=0;
	virtual void DCMAPAPI UpdateGraphicsInfo(DCMapGraphicsInfo* pinf)=0;
	virtual void DCMAPAPI Undo()=0;
	virtual void DCMAPAPI PlaceText(bool place)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapGraphicLayer);

struct IDCMapFleetDragObject : public IDCMapDragObject
{
	virtual int DCMAPAPI NumFleets()=0;
	virtual dcmapDBREF const* DCMAPAPI Fleet(int n)=0;
};

struct IDCMapFleetLayer : public IDCMapLayer
{

};
DCMAP_SPEC_INTERFACE(IDCMapFleetLayer);

//////////////////////////////////////////////////////////////////////////
enum EDCMapPauseOprions
{
	DCM_PAUSE_QUIT  = (1<<0),
	DCM_PAUSE_CANCEL = (1<<1),
	DCM_PAUSE_CONTINUE = (1<<2),
	DCM_PAUSE_RETRY = (1<<3),
};
struct IDCMapProcessRecordReseiver
{
	virtual void DCMAPAPI SetErrorCode(int code, dcmapDCSTR ErrorText=0)DCMAP_IMPL_HELPER;

	virtual bool DCMAPAPI AddRecord(dcmapDCSTR Text,int id=-1,int text_line=-1)DCMAP_IMPL_HELPER_RET(true);
	virtual bool DCMAPAPI AddError(dcmapDCSTR Text,int text_line=-1)DCMAP_IMPL_HELPER_RET(true);
	virtual bool DCMAPAPI AddMessage(dcmapDCSTR Text,int text_line=-1)DCMAP_IMPL_HELPER_RET(true);
	virtual void DCMAPAPI ClearMessages()DCMAP_IMPL_HELPER;

	virtual void DCMAPAPI SetupProgress(float max,float cur=0,dcmapDCSTR header=0)DCMAP_IMPL_HELPER;
	virtual void DCMAPAPI UpdateProgress(float pos,dcmapDCSTR header=0)DCMAP_IMPL_HELPER;
	virtual void DCMAPAPI AdvanceProgress(float adv,dcmapDCSTR header=0)DCMAP_IMPL_HELPER;

	virtual void DCMAPAPI Show()DCMAP_IMPL_HELPER;
	virtual void DCMAPAPI Hide()DCMAP_IMPL_HELPER;
	virtual void DCMAPAPI SetHeader(dcmapDCSTR header)DCMAP_IMPL_HELPER;
	virtual void DCMAPAPI SetStatus(dcmapDCSTR status)DCMAP_IMPL_HELPER;
	virtual bool DCMAPAPI TestCancel()DCMAP_IMPL_HELPER_RET(false);

	virtual bool DCMAPAPI PeekCancel()DCMAP_IMPL_HELPER_RET(false);
	virtual dcmapINT DCMAPAPI Pause(dcmapDCSTR status=0,dcmapINT Options=0)DCMAP_IMPL_HELPER_RET(true);
};

#define DCMAP_IMPORT_REPLACE 0x00000001

#define DCMAP_IMPORT_UNIVERSAL 0x00010000
#define DCMAP_IMPORT_ALLOW_REPLACE 0x00020000
#define DCMAP_IMPORT_SORT_RECORDS 0x00040000

#define DCMAP_EXPORT_UNIVERSAL 0x00010000

struct IDCMapImportProcessor : public IDCMapModule
{
	// param get
	virtual bool DCMAPAPI SetTextParam(dcmapDCSTR szParam)DCMAP_IMPL_HELPER_RET(false);
	virtual bool DCMAPAPI SetDataSourceName(const char* szName)DCMAP_IMPL_HELPER_RET(false);

	virtual dcmapDWORD DCMAPAPI GetImportParams()DCMAP_IMPL_HELPER_RET(0);

	virtual dcmapWCSTR DCMAPAPI GetFileFilters()=0;
	virtual dcmapWCSTR DCMAPAPI GetDisplayName()=0;
	virtual char const* DCMAPAPI GetConfigSectionName()=0;
																					   
	virtual bool DCMAPAPI PreProcessData(dcmapDCSTR szInputData,int iLines,
		                                IDCMapProcessRecordReseiver* pRsvr,
										dcmapDWORD options)=0;

	virtual void DCMAPAPI ClearProcessData()=0;

	virtual bool DCMAPAPI BeginExecute()=0;
	virtual bool DCMAPAPI ExecuteRecord(dcmapDCSTR Text,int id)=0;
	virtual void DCMAPAPI EndExecute()=0;

	virtual dcmapWCSTR DCMAPAPI ReformatInput(dcmapWCSTR input)DCMAP_IMPL_HELPER_RET(0);
};
DCMAP_SPEC_INTERFACE(IDCMapImportProcessor);

struct IDCMapFilteredDataSource : public IDCMapDataSource
{
	virtual bool DCMAPAPI SetTextParam(const char* szParam)=0;
	virtual bool DCMAPAPI SetDataSourceName(const char* szName)=0;
	virtual bool DCMAPAPI SetDataSource(IDCMapDataSource* pSource)=0;
	virtual bool DCMAPAPI Process(IDCMapProcessRecordReseiver* pRsvr,dcmapDWORD options)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapFilteredDataSource);

struct IDCMapExportProcessor : public IDCMapModule
{
	virtual dcmapDWORD DCMAPAPI GetExportParams()=0;
	// param get
	virtual bool DCMAPAPI SetTextParam(dcmapDCSTR szParam)=0;
	virtual bool DCMAPAPI SetDataSourceName(const char* szName)=0;
	virtual bool DCMAPAPI SetDataSource(IDCMapDataSource* pSource)=0;
	virtual bool DCMAPAPI SetFilterExpr(dcmapDCSTR szExpr)=0;

	virtual dcmapWCSTR DCMAPAPI GetDisplayName()=0;
	virtual char const* DCMAPAPI GetConfigSectionName()=0;

	virtual bool DCMAPAPI Export(IDCMapProcessRecordReseiver* pRsvr,dcmapDWORD options)=0;

	virtual dcmapWCSTR DCMAPAPI GetFileFilters()=0;
	virtual dcmapWCSTR DCMAPAPI GetDefaultExt()=0;

};
DCMAP_SPEC_INTERFACE(IDCMapExportProcessor);

struct IDCMapFilterItem : public IDCMapModule
{
	virtual dcmapCSTR DCMAPAPI UIModuleName()=0;
	virtual dcmapLOCSTR DCMAPAPI ItemCaption()=0;

	virtual bool DCMAPAPI Process(dcmapDBREF const& ref)=0;

	virtual void DCMAPAPI SetStrParam(int id, dcmapCSTR val){};
	virtual void DCMAPAPI SetRealParam(int id,dcmapREAL val){};
	virtual void DCMAPAPI SetIntParam(int id, dcmapINT val){};
	virtual bool DCMAPAPI ActualizeData(){return false;};

	virtual bool DCMAPAPI Enabled()=0;
	virtual void DCMAPAPI SetEnabled(bool e)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapFilterItem);

struct IDCMapPlanetHintSource : public IDCMapModule
{
	virtual dcmapWCSTR DCMAPAPI GetHint(dcmapDBREF const& ref)=0;
	virtual dcmapWCSTR DCMAPAPI GetCellHint(int x,int y)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPlanetHintSource);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct IDCMapPlanetContextManager : public IDCMapModule
{
	virtual dcmapWCSTR DCMAPAPI GetPlanetHint(dcmapDBREF const& ref)=0;
	virtual dcmapWCSTR DCMAPAPI GetCellHint(int x,int y)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPlanetContextManager);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct IDCMapExpressionTableFilterProcessor : public IDCMapModule
{
	virtual bool DCMAPAPI Setup(dcmapCSTR strTable,dcmapDCSTR wstrExpression)=0;
	virtual bool DCMAPAPI Process(dcmapDBREF const* Record)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapExpressionTableFilterProcessor);


struct IDCMapScriptModule : public IDCMapModule
{
	virtual bool DCMAPAPI CallProc(dcmapWCSTR Proc,IDCMapVarValue* Params,IDCMapVarValue* Result)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapScriptModule);


struct IDCMapSimpleDataFilter : public IDCMapModule
{
	virtual bool Process(IDCMapDataSource* pSource,dcmapDBREF const& dbref)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapSimpleDataFilter);
