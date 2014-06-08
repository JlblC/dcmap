
///////////////////////////////////////////////////////////////////////////
// Helper macro - obsolete

#define DCMAP_GET_STORAGE(core,var,class,name)\
	{IDCMapDataSource* var##pre = core->GetDataSource(#name);\
	if(!var##pre)return false;\
	var = (class*)var##pre->CastTo(#class);\
	if(!var)return false;}

#define DCMAP_GET_STORAGE_NR(core,var,class,name)\
	{IDCMapDataSource* var##pre = core->GetDataSource(#name);\
	var = 0;\
	if(var##pre){var = (class*)var##pre->CastTo(#class);}\
	}

template<class T>
class DCMapDataSourcePtr
{
	T* m_prt;
	dcmapQWORD index;
public:
	typedef typename T::index_type index_type;
	typedef typename T::data_type  data_type;

	DCMapDataSourcePtr():m_prt(0),index(0){}
	explicit DCMapDataSourcePtr(T* prt):m_prt(prt),index(prt?prt->GetStateIndex():0){}
	bool init(IDCMapCore* core)
	{
		m_prt = core->GetDataSourceTyped<T>(IDCMapGetInterfaceID<T*>());
		if(!m_prt)return false;
		index = m_prt->GetStateIndex();
		return true;
	}
	bool init(IDCMapCore* core,const char* czTable)
	{
		m_prt = core->GetDataSourceTyped<T>(czTable);
		if(!m_prt)return false;
		index = m_prt->GetStateIndex();
		return true;
	}
	bool init(T* ptr)
	{
		m_prt = ptr;
		if(!m_prt)return false;
		index = m_prt->GetStateIndex();
		return true;
	}

	void reset()
	{
		m_prt = 0;
		index = 0;
	}

	void reset_state()
	{
		index = 0;
	}

	T* operator ->(){return m_prt;}
	T& operator *(){return *m_prt;}

	operator T*(){return m_prt;}

	bool actualize_state()
	{
		if(!m_prt)return false;
		dcmapQWORD cind = m_prt->GetStateIndex();	
		if(index != cind)
		{
			index = cind;
			return true;
		}
		return false;
	}

	class ColumnReference
	{
		IDCMapDataStorage* m_prt;
		int m_id;

	public:
		ColumnReference(IDCMapDataStorage* prt,int id):m_prt(prt),m_id(id){}

	#ifdef INC_VCL
		operator AnsiString(){return m_prt->GetString(m_id);}
		//operator WideString(){return m_prt->GetWideString(m_id);}
		operator UnicodeString(){return m_prt->GetWideString(m_id);}

		String Str(){return m_prt->GetWideString(m_id);}
		AnsiString AnsiStr(){return m_prt->GetString(m_id);}

		ColumnReference& operator =(AnsiString const& v){m_prt->SetString(m_id,v.c_str());return *this;}
		ColumnReference& operator =(WideString const& v){m_prt->SetWideString(m_id,v.c_bstr());return *this;}
		ColumnReference& operator =(UnicodeString const& v){m_prt->SetWideString(m_id,v.w_str());return *this;}

		operator System::Extended(){return m_prt->GetReal(m_id);}
		ColumnReference& operator =(System::Extended const& v){m_prt->SetRealString(m_id,v);return *this;}

	#endif

	#ifdef DCMAP_STD_STRING

		operator std::string(){return m_prt->GetString(m_id);}
		operator std::wstring(){return m_prt->GetWideString(m_id);}

		ColumnReference& operator =(std::string const& v){m_prt->SetString(m_id,v.c_str());return *this;}
		ColumnReference& operator =(dcmap::dstring const& v){m_prt->SetWideString(m_id,v.c_str());return *this;}

	#endif
		operator dcmapDBREF const&(){return *m_prt->GetReference(m_id);}
		ColumnReference& operator =(dcmapDBREF const& v){m_prt->SetReference(m_id,v);return *this;}
		ColumnReference& operator =(dcmapDBREF const* v)
		{
			if(v)m_prt->SetReference(m_id,*v);
			else m_prt->SetReference(m_id,dcmapDBREF());
			return *this;
		}

		operator bool(){return m_prt->GetBoolean(m_id);}
		operator dcmapINT(){return m_prt->GetInteger(m_id);}
		operator dcmapUINT(){return m_prt->GetUnsigned(m_id);}
		operator dcmapLONG(){return m_prt->GetInteger(m_id);}
		operator dcmapULONG(){return m_prt->GetUnsigned(m_id);}
		operator double(){return m_prt->GetReal(m_id);}
		operator float(){return m_prt->GetReal(m_id);}

		operator dcmapCSTR(){return m_prt->GetString(m_id);}
		operator dcmapDCSTR(){return m_prt->GetWideString(m_id);}

		dcmapWCSTR wstr(){return m_prt->GetWideString(m_id);}
		dcmapCSTR str(){return m_prt->GetString(m_id);}


		ColumnReference& operator =(bool v){m_prt->SetBoolean(m_id,v);return *this;}
		ColumnReference& operator =(dcmapCSTR v){m_prt->SetString(m_id,v);return *this;}
		ColumnReference& operator =(dcmapDCSTR v){m_prt->SetWideString(m_id,v);return *this;}

		ColumnReference& operator =(int v){m_prt->SetInteger(m_id,v);return *this;}
		ColumnReference& operator =(float v){m_prt->SetReal(m_id,v);return *this;}
		ColumnReference& operator =(double v){m_prt->SetReal(m_id,v);return *this;}
		ColumnReference& operator =(dcmapDWORD v){m_prt->SetUnsigned(m_id,v);return *this;}

	};

    ColumnReference operator [](index_type n){return ColumnReference(m_prt,n);}
    ColumnReference operator [](dcmapCSTR str){return ColumnReference(m_prt,m_prt->ColumnID(str));}
};
typedef DCMapDataSourcePtr<IDCMapDataStorage> IDCMapDataStoragePtr;
typedef DCMapDataSourcePtr<IDCMapDataStorage> IDCMapDataStoragePtr;

enum EDCMapBaseTableRecords
{
	DCMDB_BASE_ID=0,

	DCMDB_BASE_NOTES=1,
	DCMDB_BASE_NAME=2,

	DCMDB_BASE_PARENT=1,
	DCMDB_LNCT_NAME=2,
	DCMDB_BASE_GAMEID=2,
};

struct DCMapBaseDataStruct
{
	dcmapDBREF id;
	dcmapDBREF notes;
};

enum EDCMapPlanetFlags
{
	// Buildings 
	DCMAP_PF_Barracks	= 1 << 0,
	DCMAP_PF_Shipyard	= 1 << 1,
	DCMAP_PF_Components = 1 << 2,
	DCMAP_PF_Customs	= 1 << 3,
	DCMAP_PF_Research	= 1 << 4,
	DCMAP_PF_Design		= 1 << 5,
	DCMAP_PF_Palace		= 1 << 6,

	// Planet Type
	DCMAP_PF_Industrial = 1 << 7,
	DCMAP_PF_Bounds		= 1 << 8,

	DCMAP_PF_Homeworld	= 1 << 9,
	DCMAP_PF_Unsafe		= 1 << 10,
	DCMAP_PF_Planning	= 1 << 11,
	DCMAP_PF_Hidden		= 1 << 12,
	DCMAP_PF_My		    = 1 << 13,

	DCMAP_PF_KeepName		    = 1 << 14,
	DCMAP_PF_SynchronizeName    = 1 << 15,

};

enum EDCMapPlanetInfo_Records
{
	DCMDB_PLANET_ID=DCMDB_BASE_ID,
	DCMDB_PLANET_NOTES=DCMDB_BASE_NOTES,

	DCMDB_PLANET_X,
	DCMDB_PLANET_Y,
	DCMDB_PLANET_O,
	DCMDB_PLANET_E,
	DCMDB_PLANET_M,
	DCMDB_PLANET_T,
	DCMDB_PLANET_S,

	DCMDB_PLANET_CAPTION,

	DCMDB_PLANET_OWNER,
	DCMDB_PLANET_COLOR1,
	DCMDB_PLANET_COLOR2,

	DCMDB_PLANET_ICON1,
	DCMDB_PLANET_ICON2,

	DCMDB_PLANET_LOADER,
	DCMDB_PLANET_TURN,

	DCMDB_PLANET_HAS_MINES,
	DCMDB_PLANET_HAS_BIOFARMS,
	DCMDB_PLANET_HAS_POWERPLANTS,
	DCMDB_PLANET_HAS_FACTORIES,
	DCMDB_PLANET_HAS_COMPRESSORS,
	DCMDB_PLANET_HAS_GOVERNORS,

	DCMDB_PLANET_HAS_BARRACKS,
	DCMDB_PLANET_HAS_SHIPYARD,
	DCMDB_PLANET_HAS_COMPONENTS,
	DCMDB_PLANET_HAS_CUSTOMS,
	DCMDB_PLANET_HAS_RESEARCH,
	DCMDB_PLANET_HAS_DESIGN,
	DCMDB_PLANET_HAS_PALACE,

	DCMDB_PLANET_INDUSTRIAL,
	DCMDB_PLANET_BOUNDS,
	DCMDB_PLANET_HW,
	DCMDB_PLANET_UNSAFE,
	DCMDB_PLANET_CORRUPTION,

    DCMDB_PLANET_GARRISON,
    DCMDB_PLANET_BUILDINGS,
	DCMDB_PLANET_QUEUE,
	DCMDB_PLANET_FLEETS,

	DCMDB_PLANET_POPULATION,

	DCMDB_PLANET_BUILD_PLANNING,
	DCMDB_PLANET_HIDDEN,
	DCMDB_PLANET_MY,

	DCMDB_PLANET_KEEP_NAME,
	DCMDB_PLANET_SYNCHRONIZE_NAME,

	DCMDB_PLANET_INFILTRATION,
};

struct DCMapPlanetGeoInfo
{
	dcmapBYTE o;
	dcmapBYTE e;
	dcmapBYTE m;
	dcmapBYTE t;
	dcmapBYTE s;
	friend bool operator == (DCMapPlanetGeoInfo const& g1,DCMapPlanetGeoInfo const& g2)
	{
		return *(dcmapDWORD const*)(&g1) == *(dcmapDWORD const*)(&g2) && g1.s == g2.s;
	}
};

struct DCMapPlanetParamInfo : public DCMapBaseDataStruct
{
	dcmapWORD x;
	dcmapWORD y;
	union
	{
		struct  
		{
			dcmapBYTE o;
			dcmapBYTE e;
			dcmapBYTE m;
			dcmapBYTE t;
			dcmapBYTE s;
		};
		DCMapPlanetGeoInfo geo;
	};
};

const int DCMapPlanetCaptionLen = 64;
struct DCMapPlanetInfo : public DCMapPlanetParamInfo
{
	dcmapDBREF owner;
	dcmapDBREF loader;
    dcmapDBREF garrison;
	dcmapDBREF buildings;
	dcmapDBREF queue;
    dcmapDBREF fleets;
	dcmapCOLORREF color1;
	dcmapCOLORREF color2;
	dcmapDCHAR caption[DCMapPlanetCaptionLen];
	//dcmapDBREF garrison;
	dcmapDBREF icon1;
	dcmapDBREF icon2;

	dcmapDWORD         population;

	dcmapDWORD         Flags;
	dcmapWORD		   turn;

	// buildings info
	dcmapBYTE		   mines;
	dcmapBYTE		   biofarms;
	dcmapBYTE		   powerplants;
	dcmapBYTE		   factories;
	dcmapBYTE		   compressors;
	dcmapBYTE		   governors;

	dcmapBYTE		   corruption;
	dcmapBYTE		   infiltration;
};

struct IDCMapPlanetDataStorage : public IDCMapDataStorage
{
	typedef DCMapPlanetInfo           data_type; 
	typedef EDCMapPlanetInfo_Records  index_type; 

	virtual bool DCMAPAPI SelectCoord(int x,int y)=0;
	virtual bool DCMAPAPI SelectLine(int LineNum,int From,int To)=0;
	virtual const data_type* DCMAPAPI GetData()=0;
	virtual bool DCMAPAPI UpdateData(const DCMapPlanetInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapPlanetInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapPlanetInfo* pInf)=0;
	virtual bool DCMAPAPI RetrieveByCoords(int x,int y,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI SetCoords(int x,int y)=0;
	virtual void DCMAPAPI SetData(const DCMapPlanetInfo* data)=0;
	virtual void DCMAPAPI SetBaseData(const DCMapPlanetParamInfo* data)=0;
	virtual void DCMAPAPI SetGeoData(const DCMapPlanetGeoInfo* data)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPlanetDataStorage);

typedef DCMapDataSourcePtr<IDCMapPlanetDataStorage> DCMapPlanetDataStoragePtr;
typedef DCMapDataSourcePtr<IDCMapPlanetDataStorage> IDCMapPlanetDataStoragePtr;

struct IDCMapGeoDataStorage : public IDCMapDataStorage
{
	typedef DCMapPlanetParamInfo      data_type; 
	typedef EDCMapPlanetInfo_Records  index_type; 

	virtual bool DCMAPAPI SelectCoord(int x,int y)=0;
	virtual const DCMapPlanetParamInfo* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI FillDefaults(DCMapPlanetParamInfo* pInf)=0;
	virtual void DCMAPAPI SetData(const DCMapPlanetParamInfo* data)=0;
	virtual bool DCMAPAPI SelectLine(int LineNum,int From,int To)=0;
	virtual bool DCMAPAPI UpdateGeo(const DCMapPlanetParamInfo* data)=0;
};

DCMAP_SPEC_INTERFACE(IDCMapGeoDataStorage);
typedef DCMapDataSourcePtr<IDCMapGeoDataStorage> IDCMapGeoDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapPlanetPlayerFlags
{
	// Buildings 
	DCMAP_PPF_Open	= 1 << 0,
};

enum EDCMapPlanetPlayerInfo_Records
{
	DCMDB_PLANETPLAYER_ID=DCMDB_BASE_ID,
	DCMDB_PLANETPLAYER_PLANET=DCMDB_BASE_PARENT,
	DCMDB_PLANETPLAYER_PLAYER,
	DCMDB_PLANETPLAYER_TURN,
	DCMDB_PLANETPLAYER_OPEN,
};

struct DCMapPlanetPlayerInfo
{
	dcmapDBREF id;
	dcmapDBREF parent;
	dcmapDBREF player;
	dcmapWORD  turn;
	dcmapWORD  flags;
};

struct IDCMapPlanetPlayerDataStorage : public IDCMapDataStorage
{
	typedef DCMapPlanetPlayerInfo      data_type; 
	typedef EDCMapPlanetPlayerInfo_Records  index_type; 

	virtual const DCMapPlanetPlayerInfo* DCMAPAPI GetData()=0;
	virtual bool DCMAPAPI UpdateData(const DCMapPlanetPlayerInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapPlanetPlayerInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapPlanetPlayerInfo* pInf)=0;
	virtual void DCMAPAPI SetData(const DCMapPlanetPlayerInfo* data)=0;

	virtual bool DCMAPAPI SelectPlanetPlayer(dcmapDBREF const& planet,
											dcmapDBREF const& player)=0;

};
DCMAP_SPEC_INTERFACE(IDCMapPlanetPlayerDataStorage);
typedef DCMapDataSourcePtr<IDCMapPlanetPlayerDataStorage> IDCMapPlanetPlayerDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapDiplomatyRelationType
{
	DCMAP_DIP_UNDEFINED=0,
	DCMAP_DIP_NEUTRALS=1,
	DCMAP_DIP_ALLY=2,
	DCMAP_DIP_ENEMIES=3,
	DCMAP_DIP_LORD=4,
	DCMAP_DIP_VASSAL=5,
	DCMAP_DIP_MULT=6,
	DCMAP_DIP_POTENTIAL_ALLY=7,
	DCMAP_DIP_POTENTIAL_ENEMIES=8,
	DCMAP_DIP_POTENTIAL_LORD=9,
	DCMAP_DIP_POTENTIAL_VASSAL=10,
	DCMAP_DIP_EMPTY=11,
	DCMAP_DIP_CUSTOM=12
};
const int EDCMapDiplomatyRelationNum = 12;

enum EDCMapGameDiplomatyRelationType
{
	DCMAP_DIP_GAME_UNDEFINED=-1,
	DCMAP_DIP_GAME_ENEMIES=0,
	DCMAP_DIP_GAME_NEUTRALS=1,
	DCMAP_DIP_GAME_ALLY=2,
	DCMAP_DIP_GAME_VASSAL=3,
	DCMAP_DIP_GAME_LORD=4,
};

enum EDCMapPlayerFlags
{
	// Buildings 
	DCMAP_PF_CalculateCorruption = (1 << 0),
	DCMAP_PF_WebAccess = (1 << 1),

	DCMAP_PF_Manage = (1 << 2),

	DCMAP_PF_ImportTrusted = (1 << 3),

	DCMAP_PF_AddEmpty       = (1 << 4),
	DCMAP_PF_AddOccupied    = (1 << 5),

	DCMAP_PF_OverwriteEmpty     = (1 << 6),
	DCMAP_PF_OverwriteOccupied  = (1 << 7),
	DCMAP_PF_ClearOccupied      = (1 << 8),

	DCMAP_PF_ImportDip = (1 << 9),
	DCMAP_PF_ImportContents = (1 << 10),
	DCMAP_PF_ImportFleets = (1 << 11),
	DCMAP_PF_ImportAlienFleets = (1 << 12),
};

enum EDCMapPlayerInfo_Records
{
	DCMDB_PLAYER_ID=DCMDB_BASE_ID,
	DCMDB_PLAYER_NOTES=DCMDB_BASE_NOTES,
	DCMDB_PLAYER_NAME=DCMDB_BASE_NAME,

	DCMDB_PLAYER_RACE,
	DCMDB_PLAYER_ORGANIZATION,
	DCMDB_PLAYER_ICON,
	DCMDB_PLAYER_DIP,
	DCMDB_PLAYER_COLOR1,
	DCMDB_PLAYER_COLOR2,
	DCMDB_PLAYER_USER_ID,
	DCMDB_PLAYER_SCRIPTING_FLEET_ID,

	DCMDB_PLAYER_RELATION,
	DCMDB_PLAYER_RELATION_TYPE,

	DCMDB_PLAYER_LOGIN,
	DCMDB_PLAYER_PASSWORD,

	DCMDB_PLAYER_CALCULATECORRUPTION,
	DCMDB_PLAYER_WEBACCESS,
	DCMDB_PLAYER_TURN,

	DCMDB_PLAYER_TEHNOLOGY,
	DCMDB_PLAYER_NATIVE_TEHNOLOGY,

	DCMDB_PLAYER_MAIN_GAIN,
	DCMDB_PLAYER_SECOND_GAIN,
	DCMDB_PLAYER_MONEY_GAIN,

	DCMDB_PLAYER_MAIN_SPENT,
	DCMDB_PLAYER_SECOND_SPENT,
	DCMDB_PLAYER_MONEY_SPENT,

	DCMDB_PLAYER_MAIN,
	DCMDB_PLAYER_MONEY,
	DCMDB_PLAYER_SECOND,
	DCMDB_PLAYER_FOOD_BONUS,

	DCMDB_PLAYER_MANAGE,

	DCMDB_PLAYER_IMPORT_TRUSTED,
	DCMDB_PLAYER_ADD_EMPTY,
	DCMDB_PLAYER_ADD_OCCUPIED,
	DCMDB_PLAYER_OVERWRITE_EMPTY,
	DCMDB_PLAYER_OVERWRITE_OCCUPIED,
	DCMDB_PLAYER_CLEAR_OCCUPIED,

	DCMDB_PLAYER_IMPORT_DIP,
	DCMDB_PLAYER_IMPORT_CONTENTS,
	DCMDB_PLAYER_IMPORT_FLEETS,	
	DCMDB_PLAYER_IMPORT_ALIEN_FLEETS,	

	DCMDB_PLAYER_OPEN_PLANETS,	
	DCMDB_PLAYER_OWN_PLANETS,
	DCMDB_PLAYER_DIP_RELATIONS,	
	DCMDB_PLAYER_IMPORT_REGION,
};

const int DCMapPlayerNameLen = 32;
struct DCMapPlayerInfo : public DCMapBaseDataStruct
{
	static const int name_len = DCMapPlayerNameLen;
	static const int login_len = 32;
	//static const int password_len = 32;
	dcmapDBREF race;
	dcmapDBREF relation;
	dcmapDBREF organization;
	dcmapDBREF icon;
	dcmapDBREF dip;
	dcmapDBREF open_planets;
	dcmapDBREF own_planets;
	dcmapDBREF dip_relations;
	dcmapDBREF import_region;

	dcmapCOLORREF color1;
	dcmapCOLORREF color2;
	dcmapDWORD	 game_id;
	dcmapDWORD	 scripting_fleet_id;
	dcmapDWORD Flags;
	dcmapWORD turn;
	dcmapBYTE relation_type;

	dcmapBYTE tehnology;
	dcmapBYTE native_tehnology;

	dcmapWORD main_gain;
	dcmapWORD second_gain;
	dcmapWORD money_gain;

	dcmapWORD main_spent;
	dcmapWORD second_spent;
	dcmapWORD money_spent;

	dcmapDWORD main;
	dcmapDWORD money;
	dcmapDWORD second;

	dcmapDWORD food_bonus;

	dcmapDCHAR name[name_len];
	dcmapDCHAR login[login_len];
	dcmapDBREF password;
	//dcmapDCHAR password[password_len];
};

struct IDCMapPlayersDataStorage: public IDCMapDataStorage
{
	typedef DCMapPlayerInfo      data_type; 
	typedef EDCMapPlayerInfo_Records  index_type; 

	virtual const DCMapPlayerInfo* DCMAPAPI GetData()=0;
	virtual bool DCMAPAPI UpdateData(const DCMapPlayerInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapPlayerInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapPlayerInfo* pInf)=0;
	virtual void DCMAPAPI SetData(const DCMapPlayerInfo* data)=0;
	virtual bool DCMAPAPI SelectGameID(dcmapDWORD id)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPlayersDataStorage);
typedef DCMapDataSourcePtr<IDCMapPlayersDataStorage>  IDCMapPlayersDataStoragePtr;

///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapDipInfo_Records
{
	DCMDB_DIP_ID=DCMDB_BASE_ID,
	DCMDB_DIP_NOTES=DCMDB_BASE_NOTES,
	DCMDB_DIP_NAME=DCMDB_BASE_NAME,
	DCMDB_DIP_COLOR1,
	DCMDB_DIP_COLOR2,
	DCMDB_DIP_DIPID,
	DCMDB_DIP_GAMEID,
	DCMDB_DIP_NATIVE,
};

const int DCMapDipNameLen = 32;
struct DCMapDipInfo : public DCMapBaseDataStruct
{
	static const int name_len = DCMapDipNameLen;
	dcmapCOLORREF color1;
	dcmapCOLORREF color2;
	dcmapBYTE dipid;	
	dcmapBYTE gameid;	
	dcmapBYTE native;
	dcmapDCHAR name[name_len];
};

struct IDCMapDipDataStorage: public IDCMapDataStorage
{
	typedef DCMapDipInfo      data_type; 
	typedef EDCMapDipInfo_Records  index_type; 

	virtual const DCMapDipInfo* DCMAPAPI GetData()=0;
	virtual bool DCMAPAPI UpdateData(const DCMapDipInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapDipInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapDipInfo* pInf)=0;
	virtual void DCMAPAPI SetData(const DCMapDipInfo* data)=0;
	virtual bool DCMAPAPI SelectDipID(dcmapBYTE id)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapDipDataStorage);
typedef DCMapDataSourcePtr<IDCMapDipDataStorage>  IDCMapDipDataStoragePtr;
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapResourceType
{
	DCMAP_ORGANICS,DCMAP_ENEGRY,DCMAP_MINERALS
};

enum EDCMapRaceInfo_Records
{
	DCMDB_RACE_ID=0,
	DCMDB_RACE_NOTES,

	DCMDB_RACE_NAME,

	DCMDB_RACE_TMIN,
	DCMDB_RACE_TMAX,

	DCMDB_RACE_NATURE,
	DCMDB_RACE_BASIS,
	DCMDB_RACE_SECOND,

	DCMDB_RACE_MIN_GROWTH,

	DCMDB_RACE_MINING,
	DCMDB_RACE_GROWTH,
	DCMDB_RACE_SCIENCE,
	DCMDB_RACE_SPEED,
	DCMDB_RACE_DEFENCE,
	DCMDB_RACE_PRICES,
	DCMDB_RACE_CIVIL_PRODUCTION,
	DCMDB_RACE_MILITARY_PRODUCTION,
	DCMDB_RACE_HITPOINTS,
	DCMDB_RACE_DAMAGE,
	DCMDB_RACE_DETECTION,
	DCMDB_RACE_STEALTH,

	DCMDB_RACE_GAME_ID,
};

const int DCMapRaceNameLen = 32;
struct DCMapRaceInfo : public DCMapBaseDataStruct
{
	static const int name_len = DCMapRaceNameLen;
	dcmapDWORD game_id;
	float  tmin;
	float  tmax;

	float  mining;
	float  growth;

	float  science;
	float  speed;
	float  defence;
	float  prices;
	float  civil_production;
	float  military_production;
	float  hitpoints;
	float  damage;
	float  detection;
	float  stealth;

	float  min_growth;

	dcmapBYTE nature;
	dcmapBYTE basis;
	dcmapBYTE second;

	dcmapDCHAR   name[name_len];
};

struct IDCMapRacesDataStorage: public IDCMapDataStorage
{
	typedef DCMapRaceInfo      data_type; 
	typedef EDCMapRaceInfo_Records  index_type; 


	virtual const DCMapRaceInfo* DCMAPAPI GetData()=0;
	virtual bool DCMAPAPI UpdateData(const DCMapRaceInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapRaceInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapRaceInfo* pInf)=0;
	virtual void DCMAPAPI SetData(const DCMapRaceInfo* data)=0;
	virtual bool DCMAPAPI SelectGameID(dcmapDWORD id)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapRacesDataStorage);
typedef DCMapDataSourcePtr<IDCMapRacesDataStorage> IDCMapRacesDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapOrganizationInfo_Records
{
	DCMDB_ORG_ID=0,
	DCMDB_ORG_NOTES,

	DCMDB_ORG_FULLNAME,
	DCMDB_ORG_DIP,

	DCMDB_ORG_COLOR1,
	DCMDB_ORG_COLOR2,
};


const int DCMapOrganizationNameLen = 32;
const int DCMapOrganizationFullNameLen = 64;
struct DCMapOrganizationInfo : public DCMapBaseDataStruct
{
	static const int name_len = DCMapOrganizationFullNameLen;
	dcmapBYTE dip;
	dcmapDCHAR   name[name_len];
	dcmapDCHAR   full_name[DCMapOrganizationFullNameLen];
	dcmapCOLOR color1;
	dcmapCOLOR color2;
};

struct IDCMapOrganizationDataStorage: public IDCMapDataStorage
{
	typedef DCMapOrganizationInfo      data_type; 
	typedef EDCMapOrganizationInfo_Records  index_type; 

	virtual const DCMapOrganizationInfo* DCMAPAPI GetData()=0;
	virtual bool DCMAPAPI UpdateData(const DCMapOrganizationInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapOrganizationInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapOrganizationInfo* pInf)=0;
	virtual void DCMAPAPI SetData(const DCMapOrganizationInfo* data)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapOrganizationDataStorage);
typedef DCMapDataSourcePtr<IDCMapOrganizationDataStorage> IDCMapOrganizationDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

const int DCMapColoringNameLen = 32;
const int DCMapColoringPluginNameLen = 32;
struct DCMapColoringInfo : public DCMapBaseDataStruct
{
	dcmapCOLOR  planet_color;
	dcmapCOLOR  background_color;
	dcmapCOLOR  cellborder_color;
	dcmapCOLOR  planetborder_color;
	dcmapCOLOR  marker_color;

	static const int name_len = DCMapColoringNameLen;
	dcmapDCHAR   name[name_len];
	char   planet[DCMapColoringPluginNameLen];
	char   background[DCMapColoringPluginNameLen];
	char   cellborder[DCMapColoringPluginNameLen];
	char   planetborder[DCMapColoringPluginNameLen];
	char   marker[DCMapColoringPluginNameLen];
};

struct IDCMapColoringDataStorage: public IDCMapDataStorage
{
	virtual const DCMapColoringInfo* DCMAPAPI GetData()=0;
	virtual bool DCMAPAPI UpdateData(const DCMapColoringInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapColoringInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapColoringInfo* pInf)=0;
	virtual void DCMAPAPI SetData(const DCMapColoringInfo* data)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapColoringDataStorage);
typedef DCMapDataSourcePtr<IDCMapColoringDataStorage> IDCMapColoringDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapLayerFlags
{
	// Buildings 
	DCMAP_LF_Hidden	= 1 << 0,
};

enum EDCMapLayerInfo_Records
{
	DCMDB_LAYER_ID=DCMDB_BASE_ID,
	DCMDB_LAYER_NOTES=DCMDB_BASE_NOTES,
	DCMDB_LAYER_NAME=DCMDB_BASE_NAME,
	DCMDB_LAYER_MODULE,

	//  flags
	DCMDB_LAYER_HIDDEN,
};

struct DCMapLayerInfo : public DCMapBaseDataStruct
{
	static const int name_len = 64;
	dcmapDCHAR name[name_len];
	char module[name_len];
	dcmapDWORD flags;
};

struct IDCMapLayersDataStorage : public IDCMapDataStorage
{
	typedef DCMapLayerInfo      data_type; 
	typedef EDCMapLayerInfo_Records  index_type; 

	virtual const DCMapLayerInfo* DCMAPAPI GetData()=0;
	virtual bool DCMAPAPI UpdateData(const DCMapLayerInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapLayerInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapLayerInfo* pInf)=0;
	virtual void DCMAPAPI SetData(const DCMapLayerInfo* data)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapLayersDataStorage);
typedef DCMapDataSourcePtr<IDCMapLayersDataStorage> IDCMapLayersDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


enum EDCMapGraphicsType
{
	DCMAP_GRAPHICS_POLYGON,
	DCMAP_GRAPHICS_POLYLINE,
	DCMAP_GRAPHICS_ZONES,
};

enum EDCMapGraphicsInfo_Records
{
	DCMDB_GRAPHICS_ID=DCMDB_BASE_ID,
	DCMDB_GRAPHICS_LAYER=DCMDB_BASE_PARENT,
	DCMDB_GRAPHICS_NAME=DCMDB_LNCT_NAME,
	DCMDB_GRAPHICS_NOTES,
	DCMDB_GRAPHICS_TEXT,
	DCMDB_GRAPHICS_X,
	DCMDB_GRAPHICS_Y,
	DCMDB_GRAPHICS_COORDS,
	DCMDB_GRAPHICS_LINE_COLOR,
	DCMDB_GRAPHICS_FILL_COLOR,
	DCMDB_GRAPHICS_TEXT_COLOR,
	DCMDB_GRAPHICS_TEXT_SIZE,
	DCMDB_GRAPHICS_TEXT_TYPE,

	DCMDB_GRAPHICS_LINE_SIZE,
	DCMDB_GRAPHICS_LINE_TYPE,
	DCMDB_GRAPHICS_FILL_MODE,
	DCMDB_GRAPHICS_TYPE,
	DCMDB_GRAPHICS_VISIBLE,
	DCMDB_GRAPHICS_RADIUS,
    DCMDB_GRAPHICS_HTMLNOTE,
};

struct DCMapGraphicsInfo : public DCMapBaseDataStruct
{
	dcmapDBREF parent;
    dcmapDBREF exinfo;
	static const int name_len = 64;
	static const int text_len = 64;
	dcmapDCHAR name[name_len];
	dcmapDCHAR text[text_len];
	union
	{
		dcmapPOINT pt;
		struct
		{
			int x,y;
		};
	};
	dcmapDBREF coords;
	dcmapDWORD line_color;
	dcmapDWORD fill_color;
	dcmapDWORD text_color;
	float text_size;
	float radius;

	dcmapBYTE text_type;
	dcmapBYTE line_size;
	dcmapBYTE line_type;
	dcmapBYTE fill_mode;
	dcmapBYTE type;

	bool visible;
};

struct IDCMapGraphicsDataStorage : public IDCMapDataStorage
{
	typedef DCMapGraphicsInfo      data_type; 
	typedef EDCMapGraphicsInfo_Records  index_type; 

	virtual const DCMapGraphicsInfo* DCMAPAPI GetData()=0;
	virtual bool DCMAPAPI UpdateData(const DCMapGraphicsInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapGraphicsInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapGraphicsInfo* pInf)=0;
	virtual void DCMAPAPI SetData(const DCMapGraphicsInfo* data)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapGraphicsDataStorage);
typedef DCMapDataSourcePtr<IDCMapGraphicsDataStorage> IDCMapGraphicsDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct DCMapIconInfo : public DCMapBaseDataStruct
{
	static const int name_len = 32;
	dcmapDCHAR name[name_len];
	dcmapDBREF filename; // Clob filename
	dcmapDBREF imgdata;  // Blob image data
	dcmapDBREF bigimgdata;  // Blob image data
	dcmapBYTE transparent;
};

struct IDCMapIconsDataStorage : public IDCMapDataStorage
{
	virtual const DCMapIconInfo* DCMAPAPI GetData()=0;
	virtual bool DCMAPAPI UpdateData(const DCMapIconInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapIconInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapIconInfo* pInf)=0;
	virtual void DCMAPAPI SetData(const DCMapIconInfo* data)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapIconsDataStorage);
typedef DCMapDataSourcePtr<IDCMapIconsDataStorage> IDCMapIconsDataStoragePtr;

enum EDCMapFleetFlags
{
	// Buildings 
	DCMAP_FF_InTransit		= (1 << 0),
	DCMAP_FF_Hidden			= (1 << 1),
    DCMAP_FF_Foreign		= (1 << 2),
    DCMAP_FF_Garrison		= (1 << 3),
	DCMAP_FF_History		= (1 << 4),
	DCMAP_FF_Outdated       = (1 << 5),
	DCMAP_FF_Deleted        = (1 << 6),
	DCMAP_FF_ScriptActive   = (1 << 7),
	DCMAP_FF_ScriptOk       = (1 << 8),
};

enum DCMapFleet_GroupType
{
    DCMAP_FGRP_Ungroupped=0,
    DCMAP_FGRP_Groupped,
    DCMAP_FGRP_RootGroup,
    DCMAP_FGRP_PlanetGroup,
    DCMAP_FGRP_UserGroup,
	DCMAP_FGRP_AutoGroup,
    DCMAP_FGRP_NameGroup,
	DCMAP_FGRP_BlockNameGroup,
	DCMAP_FGRP_PlayerGroup,
};

enum DCMapFleet_Type
{
	DCMAP_FTP_Unknown=0,
	DCMAP_FTP_Group,
	DCMAP_FTP_Fleet,
	DCMAP_FTP_Garrizon,
	DCMAP_FTP_Buildings,
	DCMAP_FTP_Queue,
};

enum EFleetSortIndex
{
	FGI_None = 0,
	FGI_FirstGroup = 10,
	FGI_SystemGroups = 20,
	FGI_Planets,
	FGI_Buildings,
	FGI_Queue,
	FGI_Garrison,
	FGI_Regular = 30,
	FGI_Transit = 40,
	FGI_Alien = 50,
	FGI_Alien_Enemy,
	FGI_Alien_Undefined,
	FGI_Alien_Neutrals,
	FGI_Alien_Ally,
	FGI_LastGroup = 60,
};

enum EFleetStatusIcon
{
	FICO_STATUS_None=-1,
	FICO_STATUS_Normal=0,
	FICO_STATUS_Hidden=1,
	FICO_STATUS_Loading,
	FICO_STATUS_ScriptOk,
	FICO_STATUS_ScriptFail,
};

enum EFleetIcon
{
	FICO_None=-1,

	FICO_O,
	FICO_E,
	FICO_M,
	FICO_C,

	FICO_My,
	FICO_My_InTransit,
	FICO_Hidden,
	FICO_Hidden_InTransit,

	FICO_Alien_InTransit,	
	FICO_Alien_Enemy,
	FICO_Alien_Undefined,
	FICO_Alien_Neutrals,
	FICO_Alien_Ally,

	FICO_Garrison,
	FICO_Buildings,
	FICO_Queue,

	FICO_NewFleet,

	FICO_MyUnit,
	FICO_AlienUnit,

	FICO_Part,

	FICO_MyPlanet,
	FICO_AlienPlanet,

	FICO_Group_My,
	FICO_Group_Enemy,
	FICO_Group_Undefined,
	FICO_Group_Neutrals,
	FICO_Group_Ally,

	FICO_Group,
	FICO_AutoGroup,
	FICO_UserGroup,
};

enum EDCMapFleetInfo_Records
{
	DCMDB_FLEET_ID=DCMDB_BASE_ID,
	DCMDB_FLEET_OWNER=DCMDB_BASE_PARENT,
	DCMDB_FLEET_GAMEID=DCMDB_BASE_GAMEID,

    DCMDB_FLEET_PARENTGROUP1,
    DCMDB_FLEET_PARENTGROUP2,
    DCMDB_FLEET_PARENTGROUP3,

	DCMDB_FLEET_LOADER,
	DCMDB_FLEET_RULER,
	DCMDB_FLEET_CAPTION,
	DCMDB_FLEET_NAME,
	DCMDB_FLEET_PUBLIC_NAME,
	DCMDB_FLEET_FROM_X,
	DCMDB_FLEET_FROM_Y,
	DCMDB_FLEET_X,
	DCMDB_FLEET_Y,
	DCMDB_FLEET_TTA,

	DCMDB_FLEET_FLY_RANGE,
	DCMDB_FLEET_FLY_SPEED,
	DCMDB_FLEET_STEALTH,
	DCMDB_FLEET_BEHAVIOR,

	DCMDB_FLEET_TURN,

	DCMDB_FLEET_WEIGHT,
	DCMDB_FLEET_SHIP_COUNT,

	DCMDB_FLEET_FOREIGN,
	DCMDB_FLEET_IN_TRANSIT,
	DCMDB_FLEET_HIDDEN,
    DCMDB_FLEET_GARRISON,
    DCMDB_FLEET_GROUP,
	DCMDB_FLEET_SORTINDEX,
	DCMDB_FLEET_LIST_IMAGE,
	DCMDB_FLEET_LIST_STATUS_IMAGE,
	DCMDB_FLEET_TYPE,
	DCMDB_FLEET_ICON,

	DCMDB_FLEET_HISTORY,
	DCMDB_FLEET_OUTDATED,
	DCMDB_FLEET_DELETED,

	DCMDB_FLEET_TRANSPORT_CAPACITY,
	DCMDB_FLEET_CARGO_COUNT,
	DCMDB_FLEET_DIP,
	DCMDB_FLEET_CHANGES,

	DCMDB_FLEET_SCRIPT_ACTIVE,
	DCMDB_FLEET_SCRIPT_OK,
	DCMDB_FLEET_SCRIPT_MESSAGE,

	DCMDB_FLEET_PARENTGROUP = DCMDB_FLEET_PARENTGROUP1,
	DCMDB_FLEET_PLANETGROUP = DCMDB_FLEET_PARENTGROUP2,
	DCMDB_FLEET_USERGROUP   = DCMDB_FLEET_PARENTGROUP3,
	DCMDB_FLEET_EXTNAME = DCMDB_FLEET_PUBLIC_NAME,
};
//////////////////////////////////////////////////////////////////////////
struct DCMapFleetInfo : public DCMapBaseDataStruct
{
	dcmapDBREF parent; // fleet owner
	dcmapDBREF ruler; //  effective fleet ruler (when fleet shared)
	dcmapDBREF loader; //  player - loader fleet information
    dcmapDBREF parentgroup; //  parent group
    dcmapDBREF parentgroup2; //  parent group (used as planet group)
    dcmapDBREF parentgroup3; //  parent group
    dcmapDBREF icon; 
	dcmapDBREF script_message; 

	static const int name_len = 64;
	static const int caption_len = 32;
	dcmapDCHAR caption[caption_len];
	dcmapDCHAR name[name_len];
    dcmapDCHAR public_name[name_len];

	dcmapDWORD game_id; // game fleet id
    dcmapDWORD flags;
	dcmapDWORD behavior;

    float fly_range;
    float fly_speed;
	float stealth;

    dcmapWORD weight; 
    dcmapWORD ship_count; 
	dcmapWORD transport_capacity; 
	dcmapWORD cargo_count; 

	dcmapWORD tta;    // turn to arrival 
	dcmapWORD turn;    // actualization turn 

	dcmapWORD x,y; 
	dcmapWORD from_x,from_y; 

    dcmapBYTE group; 
	dcmapBYTE sortindex; 
	dcmapBYTE list_image; 
	dcmapBYTE list_status_image; 
	dcmapBYTE type; 
	dcmapBYTE dip; 
	dcmapBYTE changes; 
};

struct IDCMapFleetsDataStorage : public IDCMapDataStorage
{
	typedef DCMapFleetInfo      data_type; 
	typedef EDCMapFleetInfo_Records  index_type; 

	virtual const DCMapFleetInfo* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI SetData(const DCMapFleetInfo* data)=0;
	virtual bool DCMAPAPI UpdateData(const DCMapFleetInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapFleetInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapFleetInfo* pInf)=0;
	virtual bool DCMAPAPI SelectGameID(dcmapDWORD id)=0;
    virtual bool DCMAPAPI SelectGroup(dcmapDBREF const& ref)=0;
	virtual bool DCMAPAPI SelectGroupNames(dcmapDBREF const& ref,int sind,dcmapDCSTR name)=0;
	virtual int DCMAPAPI GroupSize(dcmapDBREF const& ref)=0;
	virtual dcmapDBREF* DCMAPAPI GetRootGroup()=0;
	virtual void DCMAPAPI PushGroupSelectState()=0;
	virtual void DCMAPAPI PopGroupSelectState()=0;
};
DCMAP_SPEC_INTERFACE(IDCMapFleetsDataStorage);
typedef DCMapDataSourcePtr<IDCMapFleetsDataStorage> IDCMapFleetsDataStoragePtr;

enum EDCMapBuildingFlags
{
	// Buildings 
	DCMAP_BF_Planetary		= (1 << 0),
	DCMAP_BF_Outdated		= (1 << 1),
	DCMAP_BF_Queued			= (1 << 2),
	DCMAP_BF_Disbanded		= (1 << 3),
	DCMAP_BF_Tagged			= (1 << 4),
};

enum EDCMapBuildingInfo_Records
{
	DCMDB_BUILDING_ID=DCMDB_BASE_ID,
	DCMDB_BUILDING_OWNER=DCMDB_BASE_PARENT,
	DCMDB_BUILDING_GAMEID,
	DCMDB_BUILDING_PROJECT,
	DCMDB_BUILDING_HP,
	DCMDB_BUILDING_TURN,
	DCMDB_BUILDING_WEIGHT,
	DCMDB_BUILDING_CARAPACE,
	DCMDB_BUILDING_COLOR,
	DCMDB_BUILDING_PLANETARY,
	DCMDB_BUILDING_OUTDATED,
	DCMDB_BUILDING_QUEUED,
	DCMDB_BUILDING_COMPLETE,
	DCMDB_BUILDING_READY_IN,
	DCMDB_BUILDING_DISBANDED,
	DCMDB_BUILDING_TAGGED,
	DCMDB_BUILDING_AGE,
	DCMDB_BUILDING_PARENT = DCMDB_BUILDING_OWNER,
};

struct DCMapBuildingInfo
{
	dcmapDBREF id;		
	dcmapDBREF parent;  // parent fleet or group 
	dcmapDBREF project; // unit project
	dcmapDWORD game_id;
	dcmapDWORD hp;
	dcmapWORD  turn;
	dcmapWORD  weight;		   // unit weidth
	dcmapWORD  complete;	  
	dcmapWORD  ready_in;
	dcmapWORD  flags;
	dcmapWORD  age;
	dcmapBYTE  carapace;       // carapace type
	dcmapBYTE  color;		   // carapace color
};
//////////////////////////////////////////////////////////////////////////

struct IDCMapBuildingsDataStorage : public IDCMapDataStorage
{
	typedef DCMapBuildingInfo      data_type; 
	typedef EDCMapBuildingInfo_Records  index_type; 

	virtual const DCMapBuildingInfo* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI SetData(const DCMapBuildingInfo* data)=0;
	virtual bool DCMAPAPI UpdateData(const DCMapBuildingInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapBuildingInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapBuildingInfo* pInf)=0;

	virtual bool DCMAPAPI SelectGameID(dcmapDWORD id)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapBuildingsDataStorage);
typedef DCMapDataSourcePtr<IDCMapBuildingsDataStorage> IDCMapBuildingsDataStoragePtr;

typedef IDCMapBuildingsDataStorage IDCMapUnitsDataStorage;
typedef DCMapDataSourcePtr<IDCMapBuildingsDataStorage> IDCMapUnitsDataStoragePtr;


//////////////////////////////////////////////////////////////////////////

enum EDCMapUnitTag_Flags
{
	// Buildings 
	DCMAP_UTF_Unwrapped		= (1 << 0),
	DCMAP_UTF_Loading		= (1 << 1),
};


enum EDCMapUnitTagType
{
	DCMAP_UTT_NONE,
	DCMAP_UTT_RESOURCE,
	DCMAP_UTT_PART,
	DCMAP_UTT_UNIT,
};


enum EDCMapUnitTagInfo_Records
{
	DCMDB_UNITTAG_ID=DCMDB_BASE_ID,
	DCMDB_UNITTAG_UNIT=DCMDB_BASE_PARENT,

	DCMDB_UNITTAG_PROJECT,
	DCMDB_UNITTAG_CHASSIS,
	DCMDB_UNITTAG_RACE,

	DCMDB_UNITTAG_AMOUNT,
	DCMDB_UNITTAG_ITEMID,

	DCMDB_UNITTAG_TYPE,	
	DCMDB_UNITTAG_LEVEL,	

	DCMDB_UNITTAG_UNWRAPPED,	

	DCMDB_UNITTAG_LOADING,	
	DCMDB_UNITTAG_LOAD_FLEET,	

	DCMDB_UNITTAG_COUNT = DCMDB_UNITTAG_AMOUNT,
};

struct IDCMapUnitTagInfo
{
	dcmapDBREF id;		
	dcmapDBREF parent;  // parent fleet or group 

	dcmapDBREF project;
	dcmapDBREF chassis;
	dcmapDBREF race;
	dcmapDBREF load_fleet;

	dcmapDWORD itemid;
	dcmapDWORD amount;

	dcmapBYTE  flags;

	dcmapBYTE  level;
	dcmapBYTE  type;
};

struct IDCMapUnitTagsDataStorage : public IDCMapDataStorage
{
	typedef IDCMapUnitTagInfo      data_type; 
	typedef EDCMapUnitTagInfo_Records  index_type; 

	virtual const data_type* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI SetData(const data_type* data)=0;
	virtual bool DCMAPAPI UpdateData(const data_type* data)=0;
	virtual bool DCMAPAPI InsertData(const data_type* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(data_type* pInf)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapUnitTagsDataStorage);
typedef DCMapDataSourcePtr<IDCMapUnitTagsDataStorage> IDCMapUnitTagsDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapProjectFlags
{
	// Buildings 
	DCMAP_PRF_War		    = 1 << 0,
	DCMAP_PRF_Building	    = 1 << 1,
	DCMAP_PRF_GroundUnit    = 1 << 2,
	DCMAP_PRF_SpaceShip	    = 1 << 3,
	DCMAP_PRF_Offensive		= 1 << 4,
};

enum EDCMapProjectInfo_Records
{
	DCMDB_PROJECT_ID= DCMDB_BASE_ID,
	DCMDB_PROJECT_OWNER= DCMDB_BASE_PARENT,
	DCMDB_PROJECT_GAMEID,
	DCMDB_PROJECT_NAME,
	DCMDB_PROJECT_DESCRIPTION,
	DCMDB_PROJECT_TURN,

	DCMDB_PROJECT_COST_MAIN,
	DCMDB_PROJECT_COST_SECOND,
	DCMDB_PROJECT_COST_MONEY,
	DCMDB_PROJECT_COST_PEPL,

	DCMDB_PROJECT_SUPPORT_MAIN,
	DCMDB_PROJECT_SUPPORT_SECOND,

	DCMDB_PROJECT_HIT_POINTS,

	DCMDB_PROJECT_BONUS_O,
	DCMDB_PROJECT_BONUS_E,
	DCMDB_PROJECT_BONUS_M,
	DCMDB_PROJECT_BONUS_S,
	DCMDB_PROJECT_BONUS_PRODUCTION,

	DCMDB_PROJECT_REQ_TEHN_LEVEL,

	DCMDB_PROJECT_SCAN_STRENGTH,
	DCMDB_PROJECT_DETECT_RANGE,

	DCMDB_PROJECT_IS_WAR,

	DCMDB_PROJECT_LASER_DAMAGE,
	DCMDB_PROJECT_BOMB_DAMAGE,

	DCMDB_PROJECT_LASER_NUMBER,
	DCMDB_PROJECT_BOMB_NUMBER,

	DCMDB_PROJECT_LASER_DR,
	DCMDB_PROJECT_BOMB_DR,
	DCMDB_PROJECT_LASER_AR,
	DCMDB_PROJECT_BOMB_AR,

	DCMDB_PROJECT_LASER_TARGET,

	DCMDB_PROJECT_BOMB_TARGET,

	DCMDB_PROJECT_IS_BUILDING,
	DCMDB_PROJECT_IS_GROUND_UNIT,
	DCMDB_PROJECT_IS_SPACE_SHIP,
	DCMDB_PROJECT_IS_TRANSPORTABLE,

	DCMDB_PROJECT_CARRIER_CAPACITY,
	DCMDB_PROJECT_TRANSPORT_CAPACITY,

	DCMDB_PROJECT_FLY_RANGE,
	DCMDB_PROJECT_FLY_SPEED,

	DCMDB_PROJECT_MAXCOUNT,

	DCMDB_PROJECT_WEIGHT,
	DCMDB_PROJECT_BUILDING_CLASS,

	DCMDB_PROJECT_OFFENSIVE,

	DCMDB_PROJECT_CARAPACE,
	DCMDB_PROJECT_COLOR,
	DCMDB_PROJECT_BUILD_SPEED,

	DCMDB_PROJECT_REQUIRES_PEPL,

	DCMDB_PROJECT_CHASSIS,

	DCMDB_PROJECT_BONUS_ORGANICS = DCMDB_PROJECT_BONUS_O,
	DCMDB_PROJECT_BONUS_MINERALS = DCMDB_PROJECT_BONUS_M,
	DCMDB_PROJECT_BONUS_ENERGY   = DCMDB_PROJECT_BONUS_E,
	DCMDB_PROJECT_BONUS_SURFACE  = DCMDB_PROJECT_BONUS_S,
	DCMDB_PROJECT_CAPTION = DCMDB_PROJECT_NAME,
};

struct DCMapProjectInfo : public DCMapBaseDataStruct
{
	static const int name_len = 64;
	dcmapDCHAR name[name_len];
	dcmapDCHAR descriptin[name_len];

	dcmapDBREF parent; // user id
	dcmapDBREF produser; // race id
	dcmapDBREF chassis; // component id

	dcmapDWORD  game_id;
	dcmapDWORD flags;

	union
	{
		dcmapBYTE	laser_targets[4];
		dcmapDWORD  laser_target;
	};
	union
	{
		dcmapBYTE	bomb_targets[4];
		dcmapDWORD  bomb_target;
	};

	float cost_main;
	float cost_second;
	float cost_money;
	float support_main;
	float support_second;
	float scan_strength;
	float detect_range;
	float laser_damage;
	float bomb_damage;
	float laser_dr;
	float bomb_dr;
	float laser_ar;
	float bomb_ar;
	float fly_range;
	float fly_speed;
	float weight;

	dcmapWORD   turn;

	dcmapWORD   build_speed;
	dcmapWORD	cost_pepl;
	dcmapWORD	hit_points;
	dcmapWORD	bonus_o;
	dcmapWORD	bonus_e;
	dcmapWORD	bonus_m;
	dcmapWORD	bonus_s;
	dcmapWORD	bonus_production;
	dcmapWORD   carrier_capacity;
	dcmapWORD   transport_capacity;
	dcmapWORD   carapace;
	dcmapWORD   requires_pepl;

	dcmapBYTE	req_tehn_level;
	dcmapBYTE	laser_number;
	dcmapBYTE	bomb_number;
	dcmapBYTE   maxcount;
	dcmapBYTE	building_class;
	dcmapBYTE   color;
	dcmapBYTE   transportable;

};

struct IDCMapProjectDataStorage : public IDCMapDataStorage
{
	typedef DCMapProjectInfo      data_type; 
	typedef EDCMapProjectInfo_Records  index_type; 

	virtual const DCMapProjectInfo* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI SetData(const DCMapProjectInfo* data)=0;
	virtual bool DCMAPAPI UpdateData(const DCMapProjectInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapProjectInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapProjectInfo* pInf)=0;
	virtual bool DCMAPAPI SelectGameID(dcmapDBREF const& ParentRef,dcmapDWORD id)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapProjectDataStorage);
typedef DCMapDataSourcePtr<IDCMapProjectDataStorage> IDCMapProjectDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
enum EDCMapProjectActInfoFlags
{
	// Buildings 
	DCMAP_PAF_PlanetEmpty         = 1 << 0,
	DCMAP_PAF_PlanetDipEnemy      = 1 << 1,
	DCMAP_PAF_PlanetDipUndefined  = 1 << 2,
	DCMAP_PAF_PlanetDipNeutrals   = 1 << 3,
	DCMAP_PAF_PlanetDipAlly       = 1 << 4,
	DCMAP_PAF_PlanetDipLord       = 1 << 5,
	DCMAP_PAF_PlanetDipVassal     = 1 << 6,
};

enum EDCMapProjectActInfo_Records
{
	DCMDB_PROJECTACT_ID = DCMDB_BASE_ID,
	DCMDB_PROJECTACT_PROJECT = DCMDB_BASE_PARENT,
	DCMDB_PROJECTACT_ACTID,
	DCMDB_PROJECTACT_COST_MAIN,
	DCMDB_PROJECTACT_COST_SECOND,
	DCMDB_PROJECTACT_COST_MONEY,
	DCMDB_PROJECTACT_COST_PEPL,
	DCMDB_PROJECTACT_MAXCOUNT,

	DCMDB_PROJECTACT_PLANET_EMPTY,
	DCMDB_PROJECTACT_PLANET_DIP_ENEMY,
	DCMDB_PROJECTACT_PLANET_DIP_UNDEFINED,
	DCMDB_PROJECTACT_PLANET_DIP_NEUTRALS,
	DCMDB_PROJECTACT_PLANET_DIP_ALLY,
	DCMDB_PROJECTACT_PLANET_DIP_LORD,
	DCMDB_PROJECTACT_PLANET_DIP_VASSAL,
};

struct DCMapProjectActInfo : public DCMapBaseDataStruct
{
	dcmapDBREF  parent; 
	dcmapDWORD  flags;
	dcmapWORD  actid;
	dcmapWORD  cost_main;
	dcmapWORD  cost_second;
	dcmapWORD  cost_money;
	dcmapWORD  cost_pepl;
	dcmapWORD  maxcount;
};

struct IDCMapProjectActsDataStorage : public IDCMapDataStorage
{
	typedef DCMapProjectActInfo      data_type; 
	typedef EDCMapProjectActInfo_Records  index_type; 

	virtual const data_type* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI SetData(const data_type* data)=0;
	virtual bool DCMAPAPI UpdateData(const data_type* data)=0;
	virtual bool DCMAPAPI InsertData(const data_type* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(data_type* pInf)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapProjectActsDataStorage);
typedef DCMapDataSourcePtr<IDCMapProjectActsDataStorage> IDCMapProjectActsDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapActionsRequestedInfoFlags
{
	// Buildings 
	DCMAP_ARF_Canceled         = 1 << 0,
};

enum EDCMapActionsRequestedInfo_Records
{
	DCMDB_ACTREQ_ID = DCMDB_BASE_ID,
	DCMDB_ACTREQ_UNIT = DCMDB_BASE_PARENT,
	DCMDB_ACTREQ_GAMEID,
	DCMDB_ACTREQ_FLEET,
	DCMDB_ACTREQ_ACTID,
	DCMDB_ACTREQ_ACTION,
	DCMDB_ACTREQ_TURN,
	DCMDB_ACTREQ_LOADER,
	DCMDB_ACTREQ_CANCELED,
};

struct DCMapActionsRequestedInfo : public DCMapBaseDataStruct
{
	dcmapDBREF  parent; // unit
	dcmapDBREF  fleet; // unit
	dcmapDBREF  action; // unit
	dcmapDBREF  loader; // unit
	dcmapDWORD  game_id;
	dcmapWORD   actid;
	dcmapWORD   turn;
	dcmapWORD   flags;
};

struct IDCMapActionsRequestedDataStorage : public IDCMapDataStorage
{
	typedef DCMapActionsRequestedInfo      data_type; 
	typedef EDCMapActionsRequestedInfo_Records  index_type; 

	virtual const data_type* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI SetData(const data_type* data)=0;
	virtual bool DCMAPAPI UpdateData(const data_type* data)=0;
	virtual bool DCMAPAPI InsertData(const data_type* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(data_type* pInf)=0;

	virtual bool DCMAPAPI SelectGameID(dcmapDWORD id)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapActionsRequestedDataStorage);
typedef DCMapDataSourcePtr<IDCMapActionsRequestedDataStorage> IDCMapActionsRequestedDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum DCMapHidefleetMode
{ 
	DCMAP_HIDEFM_HIDE=1,
	DCMAP_HIDEFM_CANCEL_HIDE,
	DCMAP_HIDEFM_UNHIDE,
	DCMAP_HIDEFM_CANCEL_UNHIDE,
};

enum DCMapActions
{ 
    DCMACT_NO_ACTION,
	DCMACT_MOVE_FLEET,
    DCMACT_CANCEL_JUMP,
	DCMACT_CHANGE_FLEET_NAME,
	DCMACT_CREATE_NEW_FLEET,
	DCMACT_ADD_BUILDING_TO_QUEUE,
	DCMACT_HIDEFLEET,
	DCMACT_DROP_BUILDING_FROM_QUEUE,
	DCMACT_MOVE_UNIT_TO_FLEET,
	DCMACT_CHANGE_PLANET_NAME,
	DCMACT_BUILDING_TO_TOP,
	DCMACT_DELETE_FLEET,
	DCMACT_DEMOLISH_BUILDING,
	DCMACT_CHANGE_BEHAVIOR,
	DCMACT_STORE_ACTION,
	DCMACT_CANCEL_ACTION,
	DCMACT_UNPACK_CONAINER,
	DCMACT_LOAD_CARGO,
	DCMACT_CANCEL_CARGO_LOAD,

	DCMACT_NUMBER,

	DCMACT_DISBAND_UNIT = DCMACT_DEMOLISH_BUILDING,
};

enum EDCMapActionInfo_Flags
{
	DCMAP_ACTIF_Sended = (1<<0)
};

enum EDCMapActionInfo_Records
{
	DCMDB_ACTION_ID= DCMDB_BASE_ID,
	DCMDB_ACTION_OWNER = DCMDB_BASE_PARENT,
	DCMDB_ACTION_TYPE,
	DCMDB_ACTION_SEND_ID,
	DCMDB_ACTION_TURN,
	DCMDB_ACTION_FLEET,
	DCMDB_ACTION_SOURCE_FLEET,
	DCMDB_ACTION_BUILDING,
	DCMDB_ACTION_ACTREQ,
	DCMDB_ACTION_PROJECT,
	DCMDB_ACTION_PLANET,
	DCMDB_ACTION_FROM_X,
	DCMDB_ACTION_FROM_Y,
	DCMDB_ACTION_X,
	DCMDB_ACTION_Y,
	DCMDB_ACTION_NEW_NAME,
	DCMDB_ACTION_OLD_NAME,
	DCMDB_ACTION_NEW_PUBLIC_NAME,
	DCMDB_ACTION_OLD_PUBLIC_NAME,
	DCMDB_ACTION_MODE,
	DCMDB_ACTION_OLD_MODE,

	DCMDB_ACTION_SENDED,
};

struct DCMapActionInfo : public DCMapBaseDataStruct
{
	dcmapDBREF parent; // user id

	dcmapDBREF fleet;
	dcmapDBREF source_fleet;
	dcmapDBREF building;
	dcmapDBREF project;
	dcmapDBREF planet;
	dcmapDBREF actreq;

	static const int name_len = 64;
	dcmapDCHAR new_name[name_len];
	dcmapDCHAR old_name[name_len];
	dcmapDCHAR new_public_name[name_len];
	dcmapDCHAR old_public_name[name_len];

	union
	{
		dcmapDWORD  send_id; // 0 - action unsent
		dcmapDWORD  game_id; // 0 - action unsent
	};
	dcmapDWORD mode;
	dcmapDWORD old_mode;
	dcmapDWORD Flags;

	dcmapWORD turn;
	dcmapWORD type;
	dcmapWORD x,y; 
	dcmapWORD from_x,from_y; 
};

struct IDCMapActionsDataStorage : public IDCMapDataStorage
{
	typedef DCMapActionInfo      data_type; 
	typedef EDCMapActionInfo_Records  index_type; 

	virtual const DCMapActionInfo* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI SetData(const DCMapActionInfo* data)=0;
	virtual bool DCMAPAPI UpdateData(const DCMapActionInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapActionInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapActionInfo* pInf)=0;
	virtual bool DCMAPAPI SelectSendID(int id)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapActionsDataStorage);
typedef DCMapDataSourcePtr<IDCMapActionsDataStorage> IDCMapActionsDataStoragePtr;

struct DCMapSecretsInfo
{
	dcmapDBREF id; // user id
	static const int data_len = 64;
	dcmapBYTE enc;
	dcmapDCHAR data[data_len];
};

struct IDCMapSecretsDataStorage : public IDCMapDataStorage
{
	virtual const DCMapSecretsInfo* DCMAPAPI GetData()=0;
	virtual bool DCMAPAPI UpdateData(const DCMapSecretsInfo* data)=0;
	virtual bool DCMAPAPI InsertData(const DCMapSecretsInfo* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(DCMapSecretsInfo* pInf)=0;
	virtual void DCMAPAPI SetData(const DCMapSecretsInfo* data)=0;

	virtual bool DCMAPAPI OpenSession(dcmapWCSTR password=0)=0;
	virtual void DCMAPAPI CloseSession()=0;
	virtual int DCMAPAPI SessionStatus()=0;
	virtual bool DCMAPAPI ChangePassword(dcmapWCSTR oldp,dcmapWCSTR newp)=0;
	virtual bool DCMAPAPI CheckPassword(dcmapWCSTR pass)=0;

};
DCMAP_SPEC_INTERFACE(IDCMapSecretsDataStorage);
typedef DCMapDataSourcePtr<IDCMapSecretsDataStorage> IDCMapSecretsDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapGroupsInfo_Records
{
	DCMDB_GROUP_ID= DCMDB_BASE_ID,
	DCMDB_GROUP_PLAYER,
	DCMDB_GROUP_NAME,
	DCMDB_GROUP_TYPE,
};

struct DCMapGroupsInfo : public DCMapBaseDataStruct
{
	static const int name_len = 64;
	dcmapDCHAR name[name_len];
	dcmapDBREF player;	
	dcmapWORD type;
};

struct IDCMapGroupsDataStorage : public IDCMapDataStorage
{
	typedef DCMapGroupsInfo      data_type; 
	typedef EDCMapGroupsInfo_Records  index_type; 

	virtual const data_type* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI SetData(const data_type* data)=0;
	virtual bool DCMAPAPI UpdateData(const data_type* data)=0;
	virtual bool DCMAPAPI InsertData(const data_type* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(data_type* pInf)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapGroupsDataStorage);
typedef DCMapDataSourcePtr<IDCMapGroupsDataStorage> IDCMapGroupsDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapPlanetsGroupsInfo_Records
{
	DCMDB_PLANETSGROUP_ID= DCMDB_BASE_ID,
	DCMDB_PLANETSGROUP_GROUP,
	DCMDB_PLANETSGROUP_PLANET,
	DCMDB_PLANETSGROUP_TURN,
};

struct DCMapPlanetsGroupsInfo : public DCMapBaseDataStruct
{
	dcmapDBREF c1;	
	dcmapDBREF c2;	
	dcmapWORD turn;
};

struct IDCMapPlanetsGroupsDataStorage : public IDCMapDataStorage
{
	typedef DCMapPlanetsGroupsInfo      data_type; 
	typedef EDCMapPlanetsGroupsInfo_Records  index_type; 

	virtual const data_type* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI SetData(const data_type* data)=0;
	virtual bool DCMAPAPI UpdateData(const data_type* data)=0;
	virtual bool DCMAPAPI InsertData(const data_type* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(data_type* pInf)=0;

	virtual bool DCMAPAPI SelectConnect(dcmapDBREF const& c1,dcmapDBREF const& c2)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPlanetsGroupsDataStorage);
typedef DCMapDataSourcePtr<IDCMapPlanetsGroupsDataStorage> IDCMapPlanetsGroupsDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapPlayersGroupsInfo_Records
{
	DCMDB_PLAYERSGROUP_ID= DCMDB_BASE_ID,
	DCMDB_PLAYERSGROUP_GROUP,
	DCMDB_PLAYERSGROUP_PLAYER,
	DCMDB_PLAYERSGROUP_TURN,
	DCMDB_PLAYERSGROUP_DIPID,
};

struct DCMapPlayersGroupsInfo : public DCMapBaseDataStruct
{
	dcmapDBREF c1;	
	dcmapDBREF c2;	
	dcmapWORD turn;
	dcmapBYTE dipid;
};

struct IDCMapPlayersGroupsDataStorage : public IDCMapDataStorage
{
	typedef DCMapPlayersGroupsInfo      data_type; 
	typedef EDCMapPlayersGroupsInfo_Records  index_type; 

	virtual const data_type* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI SetData(const data_type* data)=0;
	virtual bool DCMAPAPI UpdateData(const data_type* data)=0;
	virtual bool DCMAPAPI InsertData(const data_type* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(data_type* pInf)=0;

	virtual bool DCMAPAPI SelectConnect(dcmapDBREF const& c1,dcmapDBREF const& c2)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapPlayersGroupsDataStorage);
typedef DCMapDataSourcePtr<IDCMapPlayersGroupsDataStorage> IDCMapPlayersGroupsDataStoragePtr;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum EDCMapComponetInfo_Records
{
	DCMDB_COMPONENT_ID=DCMDB_BASE_ID,
	DCMDB_COMPONENT_NOTES=DCMDB_BASE_NOTES,
	DCMDB_COMPONENT_GAMEID=DCMDB_BASE_GAMEID,

	DCMDB_COMPONENT_COMPID,

	DCMDB_COMPONENT_NAME,
	DCMDB_COMPONENT_DESCRIPTION,
	DCMDB_COMPONENT_ICON_NAME,

	DCMDB_COMPONENT_BUILD_ALLOW,
	DCMDB_COMPONENT_BUILD_REQ,
	DCMDB_COMPONENT_BUILD_UNIQUE,

};
#define DCMDB_COMPONENT_BUILDING_FLAG (0x8000)

//////////////////////////////////////////////////////////////////////////
struct DCMapComponentInfo : public DCMapBaseDataStruct
{
	dcmapDWORD game_id; // game component id
	static const int name_len = 64;
	static const int icon_name_len = 16;

	dcmapDCHAR name[name_len];
	dcmapDCHAR icon_name[icon_name_len];

	dcmapDBREF description;

	dcmapDWORD   build_allow;
	dcmapDWORD   build_req;
	dcmapDWORD   build_unique;

	dcmapWORD    component_id; // game component id
};

struct IDCMapComponentsDataStorage : public IDCMapDataStorage
{
	typedef DCMapComponentInfo           data_type; 
	typedef EDCMapComponetInfo_Records  index_type; 

	virtual const data_type* DCMAPAPI GetData()=0;
	virtual void DCMAPAPI SetData(const data_type* data)=0;
	virtual bool DCMAPAPI UpdateData(const data_type* data)=0;
	virtual bool DCMAPAPI InsertData(const data_type* data,dcmapDBREF* pRef=0)=0;
	virtual void DCMAPAPI FillDefaults(data_type* pInf)=0;
	virtual bool DCMAPAPI SelectGameID(dcmapDWORD id)=0;
};
DCMAP_SPEC_INTERFACE(IDCMapComponentsDataStorage);
typedef DCMapDataSourcePtr<IDCMapComponentsDataStorage> IDCMapComponentsDataStoragePtr;