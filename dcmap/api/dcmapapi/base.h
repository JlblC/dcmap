#ifndef dcmap_base_h__
#define dcmap_base_h__

struct IDCMapWorkspace;
class  IDCMapModule;
struct IDCMapObject;
struct IDCMapSys;
struct IDCMapModuleManager;
struct IDCMapCore;
struct IDCMapWorkspace;
struct IDCMapConfig;
struct IDCMapDataSource;
struct IDCMapCoreUI;
struct IDCMapWorkspaceUI;
struct IDCMapDataStorage;
struct IDCMapConsole; 
struct IDCMapProcessRecordReseiver; 
struct IDCMapVarValue; 
struct IDCMapScriptSystem;
struct IDCMapScriptModule;
struct IDCMapFrame;
struct IDCMapFrameUI;
struct IDCMapSysUI;

////////////////////////////////////////////////////////////////////////////////////
///   -   ,   
/// 

enum EDCMapScriptExec
{
	DCMEXEC_QUERY=0,
	DCMEXEC_CALL	= (1 << 0),
	DCMEXEC_GET		= (1 << 1),
	DCMEXEC_SET		= (1 << 2),
	DCMEXEC_GET_MEMBER= (1 << 3),
	DCMEXEC_SET_MEMBER= (1 << 4),
	DCMEXEC_GET_SUB	= (1 << 5),
	DCMEXEC_SET_SUB	= (1 << 6),

	DCMEXEC_SUCCESS=0,
	DCMEXEC_OK=DCMEXEC_SUCCESS,
	DCMEXEC_FAIL = (1 << 15),
};

////////////////////////////////////////////////////////////////////////////////////
///   -   ,   
/// 
struct DCMapObjectCreationParams
{
	dcmapLONG cbSize;
	IDCMapSys* pDCMapSys;
	dcmapCSTR szClass;
	dcmapCSTR szName;
	dcmapCSTR szParams;
	dcmapFCSTR szModuleDir;
	void* pUserData;
};

struct DCMapInterface
{

};
DCMAP_SPEC_INTERFACE(DCMapInterface);

struct IDCMapObject
{
	virtual void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)DCMAP_IMPL_HELPER_RET(0);
	virtual  void DCMAPAPI Destroy()=0;
	virtual IDCMapModule* DCMAPAPI GetFinalModule()DCMAP_IMPL_HELPER_RET(0);

#ifdef DCMAP_TEMPLATE_HELPERS
	template<class T>T  PtrCast(){return this?static_cast<T>(static_cast<IDCMapObject*>(reinterpret_cast<T>(CastTo(IDCMapGetInterfaceID<T>())))):(T)0;};
	template<class T>T* Cast(){return PtrCast<T*>();};
#endif

#ifdef DCMAP_IMPL_HELPERS
	bool OnCreate(DCMapObjectCreationParams const* params){return true;}
#endif
};
DCMAP_SPEC_INTERFACE(IDCMapObject);

#ifdef DCMAP_TEMPLATE_HELPERS
template<class Dest,class Src> Dest dcmap_cast(Src v){return v->PtrCast<Dest>();}
#endif



//////////////////////////////////////////////////////////////////////////
///     

struct IDCMapScriptable: public IDCMapObject
{
	virtual int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams, 
		IDCMapVarValue* pResult)DCMAP_IMPL_HELPER_RET(DCMEXEC_FAIL);
};
DCMAP_SPEC_INTERFACE(IDCMapScriptable);

//////////////////////////////////////////////////////////////////////////
///     

class IDCMapModule : public IDCMapScriptable
{
public:
	///  ,          -
	///  ,   ..,     ..      
	///  ,    , workspace = 0
	virtual bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)DCMAP_IMPL_HELPER_RET(true);

	///  
	/// @param Msg - ID 
	/// @param wParam  -  
	/// @param lParam  -  
	/// @param pSender - ,  
	virtual dcmapLRESULT DCMAPAPI OnMessage(dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender)DCMAP_IMPL_HELPER_RET(0);

	///    .
	///      pConfig -      .. 
	virtual void  DCMAPAPI LoadConfig(IDCMapConfig* pConfig)DCMAP_IMPL_HELPER;
	virtual void  DCMAPAPI SaveConfig(IDCMapConfig* pConfig)DCMAP_IMPL_HELPER;

	///      0   
	///       
	virtual dcmapCSTR DCMAPAPI ModuleID()DCMAP_IMPL_HELPER_RET(0);

    ///      ,    core  workspace.   
    ///       

#ifdef DCMAP_IMPL_HELPERS
	virtual  void DCMAPAPI Destroy(){delete this;};
	virtual IDCMapModule* DCMAPAPI GetFinalModule()DCMAP_IMPL_HELPER_RET(this);
protected:
	virtual ~IDCMapModule(){};
#endif
};
DCMAP_SPEC_INTERFACE(IDCMapModule);

class IDCMapService : public IDCMapModule
{

};
DCMAP_SPEC_INTERFACE(IDCMapService);

struct DCMapSystemOptions
{
	dcmapWORD cbSize;
	bool bNoGdiPlus;
	bool bNoSkins;
	bool bWine;	
	bool bVista;
	bool bAero;
	bool bDevMode;
};

typedef IDCMapObject* (DCMAPAPI *dcmapCreateCallback)(DCMapObjectCreationParams const* pCreationParams);

struct IDCMapSys : public IDCMapScriptable
{
	virtual IDCMapScriptSystem* DCMAPAPI GetScriptSystem()=0;
	virtual DCMapSystemOptions const* DCMAPAPI GetSystemOptions()=0;

	// Object registation

	virtual void DCMAPAPI RegisterPlugin(dcmapCSTR czClass, dcmapCSTR czName,
										dcmapCreateCallback pCreateCallback,
										void* pUserData,dcmapCSTR czParam=0)=0;

	virtual void DCMAPAPI RegisterAlias(dcmapCSTR czClass, dcmapCSTR czAlias,dcmapCSTR czModule)=0;

	virtual dcmapCSTR DCMAPAPI GetPluginNameByAlias(dcmapCSTR czClass, dcmapCSTR czAlias)=0;

	virtual bool DCMAPAPI SelectClass(dcmapCSTR czClass)=0;
	virtual bool DCMAPAPI SelectPlugin(dcmapCSTR czClass,dcmapCSTR czName)=0;
	virtual bool DCMAPAPI Next()=0;

	virtual dcmapCSTR DCMAPAPI GetPluginName()=0;
	virtual dcmapCSTR DCMAPAPI GetPluginClass()=0;
	virtual dcmapCSTR DCMAPAPI GetPluginParam()=0;
	virtual dcmapWCSTR DCMAPAPI GetPluginLocalName()=0;

	virtual IDCMapObject* DCMAPAPI CreateObject(char const* czClass, char const* czName)=0;
	virtual void DCMAPAPI DestroyObject(IDCMapObject* pModule)=0;

	virtual IDCMapSysUI*  DCMAPAPI GetUI()=0;

#ifdef DCMAP_TEMPLATE_HELPERS
	#ifdef INC_VCL
	void GetClassPluginNames(char const* czClass,TStrings* Strs)
	{
		Strs->Clear();
		if(SelectClass(czClass))
			do
			{
				Strs->Add(GetPluginName());
			}
			while(Next());
	}
	#endif

	template <class CNT>
	void GetClassPluginNamesT(dcmapCSTR czClass,CNT& Strs)
	{
		Strs.clear();
		if(SelectClass(czClass))
			do
			{
				Strs.push_back(GetPluginName());
			}
			while(Next());
	}
#endif

	// Utility functions
	virtual dcmapCSTR DCMAPAPI  WideToStr(dcmapWCSTR str,int CP=0)=0;
	virtual dcmapWCSTR DCMAPAPI StrToWide(dcmapCSTR str,int CP=0)=0;

	virtual IDCMapVarValue* DCMAPAPI CreateVarValue()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapSys);

struct IDCMapScriptSystem : public IDCMapObject
{
	virtual IDCMapScriptModule* DCMAPAPI LoadScriptModuleFromFile(dcmapFCSTR szFileName,IDCMapCore* pCore=0,int Falgs=0)=0;
	virtual IDCMapScriptModule* DCMAPAPI LoadScriptModuleFromText(dcmapWCSTR szText,IDCMapCore* pCore=0,int Falgs=0)=0;
	virtual IDCMapVarValue* DCMAPAPI CreateVarValue()=0;

	virtual dcmapWCSTR DCMAPAPI ProcessFormatedString(dcmapWCSTR Format,IDCMapVarValue* Values,int Flags)=0;
	virtual dcmapWCSTR DCMAPAPI ProcessFormatedStringDB(dcmapWCSTR Format,IDCMapDataSource* pDataStorage,dcmapDBREF const* pDBRef, IDCMapVarValue* Values,int Flags)=0;

	virtual bool DCMAPAPI CallGlobalProc(dcmapWCSTR Proc,IDCMapVarValue* Params=0,IDCMapVarValue* Result=0)=0;
};

struct IDCMapDataSource: public IDCMapModule
{
	virtual bool DCMAPAPI SelectAll()=0;
	virtual bool DCMAPAPI Next()=0;
	virtual const dcmapDBREF* DCMAPAPI GetID()=0;
	virtual int DCMAPAPI NumRecords()=0;
	virtual IDCMapDataStorage* DCMAPAPI GetStorage()DCMAP_IMPL_HELPER_RET(0);
};
DCMAP_SPEC_INTERFACE(IDCMapDataSource);


struct IDCMapDatabase : public IDCMapModule
{
	virtual IDCMapDataSource* DCMAPAPI GetDataSource(dcmapCSTR pName)=0;
	virtual IDCMapDataStorage* DCMAPAPI GetDataStorage(dcmapCSTR pName)=0;

	virtual bool DCMAPAPI DataChanged()=0;

	virtual void DCMAPAPI OnDataUpdate()=0;
	virtual void DCMAPAPI LockDataUpdate()=0;
	virtual void DCMAPAPI UnlockDataUpdate()=0;

#ifdef DCMAP_TEMPLATE_HELPERS
	template<class T> 
	T* GetDataSourceTyped(dcmapCSTR czName)
	{
		IDCMapDataSource* pobj = GetDataSource(czName);
		if(!pobj)return 0;
		T* typedobj = pobj->Cast<T>();
		if(!typedobj){return 0;}
		return typedobj;
	}
#endif
};
DCMAP_SPEC_INTERFACE(IDCMapDatabase);


enum EDCMAP_BRADCAST_FLAGS
{
 DCMAP_BRADCAST_STOP_ON_FAIL      = (1<<0),
 DCMAP_BRADCAST_STOP_ON_SUCCESS   = (1<<1),
 DCMAP_BRADCAST_STOP_ON_PROCESSED = (1<<2),
 DCMAP_BRADCAST_CORE_ONLY         = (1<<3),
};

struct IDCMapModuleManager: public IDCMapModule
{
	virtual IDCMapModule* DCMAPAPI CreateModule(char const* czClass, char const* czName)=0;
	virtual void DCMAPAPI DestroyModule(IDCMapModule* pObj)=0;

	virtual bool DCMAPAPI SelectModuleByID(char const* czModuleID)=0;
	virtual IDCMapModule* DCMAPAPI Module()=0;
	virtual bool DCMAPAPI Next()=0;
	virtual IDCMapModule* DCMAPAPI GetService(const char* Name)=0;

	virtual dcmapLRESULT DCMAPAPI BroadcastMessage(int MSG,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapModule* pSender=0,dcmapDWORD flags=0)=0;

#ifdef DCMAP_TEMPLATE_HELPERS
	template<class T> 
	T* CreateModuleTyped(char const* czClass, char const* czName)
	{
		IDCMapModule* pobj = CreateModule(czClass,czName);
		if(!pobj)return 0;
		T* typedobj = pobj->Cast<T>();
		if(!typedobj){DestroyModule(pobj);return 0;}
		return typedobj;
	}

	template<class T> 
	T* GetServiceTyped(char const* czName)
	{
		IDCMapModule* pobj = GetService(czName);
		if(!pobj)return 0;
		T* typedobj = pobj->Cast<T>();
		if(!typedobj){return 0;}
		return typedobj;
	}

#ifdef INC_VCL
	template<class T>
	T* CreateModuleTyped(AnsiString czClass,AnsiString czName)
	{
		IDCMapModule* pobj = CreateModule(czClass.c_str(),czName.c_str());
		if(!pobj)return 0;
		T* typedobj = pobj->Cast<T>();
		if(!typedobj){DestroyModule(pobj);return 0;}
		return typedobj;
	}
#endif
#endif

};
DCMAP_SPEC_INTERFACE(IDCMapModuleManager);


struct IDCMapCore : public IDCMapModuleManager
{
	virtual IDCMapDataSource* DCMAPAPI GetDataSource(char const* pName)=0;
	virtual IDCMapConfig* DCMAPAPI GetConfig()=0;
	virtual IDCMapCoreUI* DCMAPAPI GetUI()=0;

	virtual void DCMAPAPI OnDataUpdate()=0;
	virtual void DCMAPAPI OnConfigChange()=0;
	virtual void DCMAPAPI LockDataUpdate()=0;
	virtual void DCMAPAPI UnlockDataUpdate()=0;
    virtual void DCMAPAPI ForceDataUpdate()=0;

	virtual void DCMAPAPI LoadDatabaseFrom(dcmapFCSTR pDir)=0;
	virtual void DCMAPAPI SaveDatabaseTo(dcmapFCSTR pDir)=0;

	virtual IDCMapDatabase* DCMAPAPI GetDatabase()=0;
	virtual bool DCMAPAPI SetDatabase(IDCMapDatabase* database)=0;

	virtual IDCMapWorkspace* DCMAPAPI GetCurrentWorkspace()=0;

	virtual IDCMapConsole* DCMAPAPI GetConsole()=0;

    virtual IDCMapVarValue* DCMAPAPI CreateVarValue()=0;

	virtual DCMapSystemOptions const* DCMAPAPI GetSystemOptions()=0;


#ifdef DCMAP_IMPL_HELPERS
	IDCMapCoreUI*  GetCoreUI(){return GetUI();}
#endif

#ifdef DCMAP_TEMPLATE_HELPERS
	template<class T> 
	T* GetDataSourceTyped(char const* czName)
	{
		IDCMapDataSource* pobj = GetDataSource(czName);
		if(!pobj)return 0;
		T* typedobj = pobj->Cast<T>();
		if(!typedobj){return 0;}
		return typedobj;
	}
#endif
};
DCMAP_SPEC_INTERFACE(IDCMapCore);


struct IDCMapWorkspace : public IDCMapModuleManager
{
	virtual IDCMapConfig* DCMAPAPI GetConfig()=0;
	virtual IDCMapWorkspaceUI* DCMAPAPI GetUI()=0;
	virtual IDCMapCore* DCMAPAPI GetCore()=0;
	virtual IDCMapFrame* DCMAPAPI GetFrame()=0;
	virtual IDCMapFrameUI* DCMAPAPI GetFrameUI()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapWorkspace);


struct IDCMapFrame : public IDCMapModuleManager
{
	virtual IDCMapConfig* DCMAPAPI GetConfig()=0;
	virtual IDCMapFrameUI* DCMAPAPI GetUI()=0;
	virtual IDCMapCore* DCMAPAPI GetCore()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapFrame);



struct IDCMapConsole 
{
	virtual void DCMAPAPI Execute(dcmapDCSTR text,IDCMapProcessRecordReseiver* pRsrv=0)=0;
	virtual void DCMAPAPI ExecuteFromWebScript(dcmapDCSTR text)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapConsole);

enum DCMapDataType
{
	DCMDT_Unknown=0,
	DCMDT_Integer=1,
	DCMDT_Number=1,
	DCMDT_Unsigned=2,
	DCMDT_Reference=3,
	DCMDT_String=4,
	DCMDT_Alpha=4,
	DCMDT_CLOB=5,
	DCMDT_Memo=5,
	DCMDT_BLOB=6,
	DCMDT_Flag=7,
	DCMDT_WideString=8,
	DCMDT_WideCLOB=9,
	DCMDT_Fixed=10,
	DCMDT_Real=11,
	DCMDT_Float=11,
	DCMDT_Time=12,
	DCMDT_ForceBYTE=0xff
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define DCMAP_COLUMN_INDEX   (1 << 0)
#define DCMAP_COLUMN_KEY     (DCMAP_COLUMN_INDEX)
#define DCMAP_COLUMN_HIDDEN  (1 << 1)

#define DCMAP_TABLE_NO_TEXT_EXPORT   (2 << 0)
#define DCMAP_TABLE_HIDDEN           ((1 << 0)|DCMAP_TABLE_NO_TEXT_EXPORT)
#define DCMAP_TABLE_INTERNAL         (DCMAP_TABLE_NO_TEXT_EXPORT)

enum DCMAPDataStorageSaveLoadFlags
{
	DCMAP_DSLF_GZIPPED = (1 << 0),
	DCMAP_DSLF_LAZY    = (1 << 1),
};

struct IDCMapDataProvider: public IDCMapDataSource
{
	//////////////////////////////////////////////////////////////////////////
	//														  Meta information 
	virtual int DCMAPAPI NumColumns()=0;

	virtual int DCMAPAPI ColumnID(const char* name)=0;

	virtual int DCMAPAPI ColumnDataSize(int n)=0;
	virtual int DCMAPAPI ColumnDataSizeN(const char* name)=0;

	virtual int DCMAPAPI ColumnParams(int n)=0;
	virtual int DCMAPAPI ColumnParamsN(const char* name)=0;

	virtual DCMapDataType DCMAPAPI ColumnDataType(int n)=0;
	virtual DCMapDataType DCMAPAPI ColumnDataTypeN(const char*  name)=0;

	virtual bool DCMAPAPI ColumnNumeric(int n)=0;
	virtual bool DCMAPAPI ColumnNumericN(const char*  name)=0;

	virtual dcmapCSTR DCMAPAPI ColumnName(int n)=0;
	//virtual char const* DCMAPAPI ColumnNameN(const char*  name)=0;

	// Returns linked table name for DCMDT_Reference data type
	// and 0 for other types
	virtual dcmapCSTR DCMAPAPI ColumnDataLinkage(int n)=0;
	virtual dcmapCSTR DCMAPAPI ColumnDataLinkageN(const char*  name)=0;

	//////////////////////////////////////////////////////////////////////////
	//														general data access
	virtual int DCMAPAPI GetInteger(int n)=0;
	virtual int DCMAPAPI GetIntegerN(char const* n)=0;

	virtual dcmapDWORD DCMAPAPI GetUnsigned(int n)=0;
	virtual dcmapDWORD DCMAPAPI GetUnsignedN(char const* n)=0;

	virtual dcmapREAL DCMAPAPI GetReal(int n)=0;
	virtual dcmapREAL DCMAPAPI GetRealN(char const* n)=0;

	virtual dcmapCSTR DCMAPAPI GetString(int n)=0;
	virtual dcmapCSTR DCMAPAPI GetStringN(char const* name)=0;

	virtual dcmapDCSTR DCMAPAPI GetWideString(int n)=0;
	virtual dcmapDCSTR DCMAPAPI GetWideStringN(char const* name)=0;

	virtual const dcmapDBREF* DCMAPAPI GetReference(int n)=0;
	virtual const dcmapDBREF* DCMAPAPI GetReferenceN(char const* name)=0;

	virtual bool DCMAPAPI GetBoolean(int n)=0;
	virtual bool DCMAPAPI GetBooleanN(char const* name)=0;

	virtual int DCMAPAPI GetBlob(int n,dcmapBYTE** pData)=0;
	virtual int DCMAPAPI GetBlobN(char const* n,dcmapBYTE** pData)=0;

	//////////////////////////////////////////////////////////////////////////
	//														general data modify
	virtual bool DCMAPAPI SetString(int n,dcmapCSTR value)=0;
	virtual bool DCMAPAPI SetStringN(char const* name,dcmapCSTR value)=0;

	virtual bool DCMAPAPI SetWideString(int n,dcmapDCSTR value)=0;
	virtual bool DCMAPAPI SetWideStringN(char const* name,dcmapDCSTR value)=0;

	virtual bool DCMAPAPI SetBoolean(int n,bool v)=0;
	virtual bool DCMAPAPI SetBooleanN(char const* n,bool v)=0;

	virtual bool DCMAPAPI SetBlob(int n,dcmapBYTE const* pData,int size)=0;
	virtual bool DCMAPAPI SetBlobN(char const* n,dcmapBYTE const* pData,int size)=0;

	virtual bool DCMAPAPI SetInteger(int n,int value)=0;
	virtual bool DCMAPAPI SetIntegerN(char const* n,int value)=0;

	virtual bool DCMAPAPI SetUnsigned(int n,dcmapDWORD value)=0;
	virtual bool DCMAPAPI SetUnsignedN(char const* n,dcmapDWORD value)=0;

	virtual bool DCMAPAPI SetReal(int n,dcmapREAL value)=0;
	virtual bool DCMAPAPI SetRealN(char const* n,dcmapREAL value)=0;

	virtual bool DCMAPAPI EditIndex()=0;
	virtual bool DCMAPAPI SelectIndex()=0;

	virtual bool DCMAPAPI SetReference(int n,dcmapDBREF const& ref)=0;
	virtual bool DCMAPAPI SetReferenceN(char const* name,dcmapDBREF const& ref)=0;


	//reorder elements
	virtual bool DCMAPAPI OrderSwap(dcmapDBREF const& r1,dcmapDBREF const& r2)=0;

};


struct IDCMapDataStorage: public IDCMapDataProvider
{
	typedef void      data_type; 
	typedef dcmapINT  index_type; 

	virtual bool DCMAPAPI InitStorage(IDCMapDatabase* pDatabase)=0;
	virtual void DCMAPAPI PostInit()=0;

	// Save & Load 
	virtual bool DCMAPAPI LoadDataFromFile(dcmapFCSTR szFileName, int Flags=0)=0;
	virtual bool DCMAPAPI SaveDataToFile(dcmapFCSTR szFileName, int Flags=0)=0;
	virtual bool DCMAPAPI LoadEmpty()=0;


	virtual void DCMAPAPI Clear()=0;

	virtual bool DCMAPAPI Changed()=0;

	virtual void DCMAPAPI ReserveForNew(int num)=0;

	virtual int DCMAPAPI TableParams()=0;

	//////////////////////////////////////////////////////////////////////////
	virtual void DCMAPAPI AddChildTable(IDCMapDataStorage* pChild)=0;
	virtual void DCMAPAPI ClearByParent(dcmapDBREF const& pRef)=0;
	virtual void DCMAPAPI ClearUnparented()=0;

	virtual const dcmapDBREF* DCMAPAPI GetParent()=0;
	virtual void DCMAPAPI SetParent(dcmapDBREF const& Ref)=0;

	virtual IDCMapDataStorage* DCMAPAPI ParentDataStorage()=0;

	//////////////////////////////////////////////////////////////////////////

	// Basic info
	virtual int DCMAPAPI GetDataSize()=0;
	// General Select 
	virtual bool DCMAPAPI SelectID(dcmapDBREF const& ref)=0;
	virtual bool DCMAPAPI SelectByParentID(dcmapDBREF const& ref)=0;

	// Used for bath data update 
	virtual void DCMAPAPI BeginUpdate()=0;
	virtual void DCMAPAPI EndUpdate()=0;

	virtual dcmapQWORD DCMAPAPI GetStateIndex()=0;

	virtual bool DCMAPAPI SelectName(dcmapCSTR szName)=0;
	virtual bool DCMAPAPI SelectWideName(dcmapDCSTR szName)=0;

	virtual bool DCMAPAPI RetrieveByName(const char* szName,dcmapDBREF* pRef=0)=0;
	virtual bool DCMAPAPI RetrieveByWideName(dcmapDCSTR szName,dcmapDBREF* pRef=0)=0;

	virtual dcmapCSTR DCMAPAPI GetName()=0;
	virtual void DCMAPAPI SetName(const char* szName)=0;
	virtual dcmapCSTR DCMAPAPI GetNotes()=0;
	virtual void DCMAPAPI SetNotes(const char* szName)=0;

	virtual dcmapDCSTR DCMAPAPI FetchWideName(dcmapDBREF const& pRef)=0;
	virtual dcmapCSTR DCMAPAPI FetchName(dcmapDBREF const& pRef)=0;

	virtual dcmapUCSTR DCMAPAPI GetDisplayName()=0;

	virtual dcmapDCSTR DCMAPAPI GetWideName()=0;
	virtual void DCMAPAPI SetWideName(dcmapDCSTR szName)=0;
	virtual dcmapDCSTR DCMAPAPI GetWideNotes()=0;
	virtual void DCMAPAPI SetWideNotes(dcmapDCSTR szName)=0;

	virtual bool DCMAPAPI Update()=0;

	virtual bool DCMAPAPI Delete()=0;
	virtual void DCMAPAPI Insert()=0;
	virtual bool DCMAPAPI Post(dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI Cansel()=0;

	virtual dcmapWCSTR DCMAPAPI StorageCaption()=0;
	virtual dcmapWCSTR DCMAPAPI NewIndexText()=0;
	virtual dcmapWCSTR DCMAPAPI RecordCaption()=0;
	virtual dcmapCSTR DCMAPAPI Name()=0;
	virtual dcmapCSTR DCMAPAPI RecordName()=0;

	virtual bool DCMAPAPI Duplicate()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapDataStorage);

struct IDCMapLocalStrings
{
	virtual bool DCMAPAPI SelectSection(dcmapCSTR name)=0;

	virtual dcmapCSTR DCMAPAPI GetString(dcmapCSTR name)=0;
	virtual dcmapCSTR DCMAPAPI GetIDString(int id)=0;
	virtual dcmapCSTR DCMAPAPI GetSectionString(dcmapCSTR name,dcmapCSTR section=0)=0;

	virtual dcmapWCSTR DCMAPAPI GetStringW(dcmapCSTR name)=0;
	virtual dcmapWCSTR DCMAPAPI GetIDStringW(int id)=0;
	virtual dcmapWCSTR DCMAPAPI GetSectionStringW(dcmapCSTR name,dcmapCSTR section=0)=0;

	virtual bool DCMAPAPI UpdateFrom(dcmapFCSTR szFileName)=0;

    virtual int DCMAPAPI CopePage()=0;
    virtual int DCMAPAPI CharSet()=0;
    virtual int DCMAPAPI Translit()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapLocalStrings);

enum EDCMapVariableType
{
	DCMVARTYPE_NIL,DCMVARTYPE_NUM,DCMVARTYPE_STRING,DCMVARTYPE_ARRAY,DCMVARTYPE_HASH,DCMVARTYPE_PROC,DCMVARTYPE_OBJECT,
};


struct IDCMapVarValue
{
    virtual void DCMAPAPI Clear()=0;
    virtual int  DCMAPAPI Count()=0;
    virtual bool DCMAPAPI IsHash()=0;

    virtual double DCMAPAPI GetNumber(int num=0)=0;
    virtual dcmapWCSTR DCMAPAPI GetString(int num=0)=0;
    virtual dcmapDBREF const* DCMAPAPI GetDBRef(int num=0)=0;
	virtual IDCMapObject* DCMAPAPI GetObject(int num=0)=0;
	virtual IDCMapVarValue* DCMAPAPI GetVar(int num=0)=0;

    virtual double DCMAPAPI GetNamedNumber(dcmapWCSTR name)=0;
    virtual dcmapWCSTR DCMAPAPI GetNamedString(dcmapWCSTR name)=0;
    virtual dcmapDBREF const* DCMAPAPI GetNamedDBRef(dcmapWCSTR name)=0;
	virtual IDCMapVarValue* DCMAPAPI GetNamedVar(dcmapWCSTR name)=0;
	virtual IDCMapObject* DCMAPAPI GetNamedObject(dcmapWCSTR name)=0;

	virtual void DCMAPAPI SetNumber(int name,double num)=0;
	virtual void DCMAPAPI SetString(int name,dcmapDCSTR string)=0;
	virtual void DCMAPAPI SetDBRef(int name,dcmapDBREF const* ref,IDCMapDataStorage* pStor=0)=0;
	virtual void DCMAPAPI SetVar(int name,IDCMapVarValue* pVar)=0;
	virtual void DCMAPAPI SetObject(int name,IDCMapObject* pVar)=0;

    virtual void DCMAPAPI SetNamedNumber(dcmapWCSTR name,double num)=0;
    virtual void DCMAPAPI SetNamedString(dcmapWCSTR name,dcmapDCSTR string)=0;
    virtual void DCMAPAPI SetNamedDBRef(dcmapWCSTR name,dcmapDBREF const* ref,IDCMapDataStorage* pStor=0)=0;
	virtual void DCMAPAPI SetNamedVar(dcmapWCSTR name,IDCMapVarValue* pVar)=0;
	virtual void DCMAPAPI SetNamedObject(dcmapWCSTR name,IDCMapObject* pVar)=0;

    virtual void DCMAPAPI AddNumber(double num)=0;
    virtual void DCMAPAPI AddString(dcmapWCSTR string)=0;
    virtual void DCMAPAPI AddDBRef(dcmapDBREF const* ref,IDCMapDataStorage* pStor=0)=0;
	virtual void DCMAPAPI AddVar(IDCMapVarValue* pVar)=0;
	virtual void DCMAPAPI AddObject(IDCMapObject* pVar)=0;

    virtual void DCMAPAPI Destroy()=0;
    virtual IDCMapVarValue* DCMAPAPI MakeCopy()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapVarValue);

//////////////////////////////////////////////////////////////////////////

#endif // dcmap_base_h__