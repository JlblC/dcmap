#include "StdAfx.h"
#include "BaseDataStorage.h"

class CPlayersDataStorage :	public CBaseNamedGameIndexedDataStorage<DCMapPlayerInfo,IDCMapPlayersDataStorage>
{
	typedef CBaseNamedGameIndexedDataStorage<DCMapPlayerInfo,IDCMapPlayersDataStorage> Inherited;

	struct DCMapPlayerInfo_v1 : public DCMapBaseDataStruct
	{
		dcmapDBREF race;
		dcmapDBREF relation;
		dcmapDBREF organization;
		dcmapDBREF icon;
		dcmapBYTE dip;
		dcmapBYTE relation_type;
		char name[32];
		dcmapCOLORREF color1;
		dcmapCOLORREF color2;
	};

	struct DCMapPlayerInfo_v2 : public DCMapBaseDataStruct
	{
		static const int name_len = DCMapPlayerNameLen;
		static const int login_len = 32;
		static const int password_len = 32;
		dcmapDBREF race;
		dcmapDBREF relation;
		dcmapDBREF organization;
		dcmapDBREF icon;
		dcmapDBREF dip;
		dcmapCOLORREF color1;
		dcmapCOLORREF color2;
		dcmapDWORD	 game_id;
		dcmapDWORD	 scripting_fleet_id;
		dcmapDWORD Flags;
		dcmapWORD turn;
		dcmapBYTE relation_type;
		dcmapDCHAR name[name_len];
		dcmapDCHAR login[login_len];
		dcmapDCHAR password[password_len];
	};

	IDCMapSecretsDataStorage* m_pSecrets;

public:
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CPlayersDataStorage();}

	CPlayersDataStorage()
	{
		m_pSecrets=0;

		m_BR.RegisterReference("organization",&value_type::organization,"organizations",DCMDB_PLAYER_ORGANIZATION);
		m_BR.Alias("union",DCMDB_PLAYER_ORGANIZATION);
		m_BR.Alias("org",DCMDB_PLAYER_ORGANIZATION);

		m_BR.RegisterReference("race",&value_type::race,"races",DCMDB_PLAYER_RACE);
		m_BR.RegisterReference("relation",&value_type::relation,"players",DCMDB_PLAYER_RELATION);
		m_BR.RegisterReference("icon",&value_type::icon,"icons",DCMDB_PLAYER_ICON);
		m_BR.RegisterReference("dip",&value_type::dip,"dip",DCMDB_PLAYER_DIP);
		m_BR.Alias("DipStatus",DCMDB_PLAYER_DIP);

		m_BR.RegisterReference("open_planets",&value_type::open_planets,"groups",DCMDB_PLAYER_OPEN_PLANETS);
		m_BR.RegisterReference("own_planets",&value_type::own_planets,"groups",DCMDB_PLAYER_OWN_PLANETS);
		m_BR.RegisterReference("dip_relations",&value_type::dip_relations,"groups",DCMDB_PLAYER_DIP_RELATIONS);

		m_BR.RegisterInt("relation_type",&value_type::relation_type,DCMDB_PLAYER_RELATION_TYPE);
		m_BR.RegisterInt("color1",&value_type::color1,DCMDB_PLAYER_COLOR1);
		m_BR.RegisterInt("color2",&value_type::color2,DCMDB_PLAYER_COLOR2);

		m_BR.RegisterInt("user_id",&value_type::game_id,DCMDB_PLAYER_USER_ID);
		m_BR.RegisterInt("scripting_fleet_id",&value_type::scripting_fleet_id,DCMDB_PLAYER_SCRIPTING_FLEET_ID);

		m_BR.RegisterInt("turn",&value_type::turn,DCMDB_PLAYER_TURN);

		m_BR.RegisterFlag("calc_corruption",&value_type::Flags,DCMAP_PF_CalculateCorruption,DCMDB_PLAYER_CALCULATECORRUPTION);
		m_BR.RegisterFlag("web_access",&value_type::Flags,DCMAP_PF_WebAccess,DCMDB_PLAYER_WEBACCESS);

		m_BR.RegisterWideString("login",&value_type::login,DCMDB_PLAYER_LOGIN);

		m_BR.RegisterReference("password",&value_type::password,"secrets",DCMDB_PLAYER_PASSWORD);
		m_BR.Alias("pass",DCMDB_PLAYER_PASSWORD);
		m_BR.SetParam(DCMAP_COLUMN_HIDDEN,DCMDB_PLAYER_PASSWORD);


		m_BR.RegisterInt("tehnology",&value_type::tehnology,DCMDB_PLAYER_TEHNOLOGY);
		m_BR.RegisterInt("native_tehnology",&value_type::native_tehnology,DCMDB_PLAYER_NATIVE_TEHNOLOGY);

		m_BR.RegisterInt("main_gain",&value_type::main_gain,DCMDB_PLAYER_MAIN_GAIN);
		m_BR.RegisterInt("second_gain",&value_type::second_gain,DCMDB_PLAYER_SECOND_GAIN);
		m_BR.RegisterInt("money_gain",&value_type::money_gain,DCMDB_PLAYER_MONEY_GAIN);

		m_BR.RegisterInt("main_spent",&value_type::main_spent,DCMDB_PLAYER_MAIN_SPENT);
		m_BR.RegisterInt("second_spent",&value_type::second_spent,DCMDB_PLAYER_SECOND_SPENT);
		m_BR.RegisterInt("money_spent",&value_type::money_spent,DCMDB_PLAYER_MONEY_SPENT);

		m_BR.RegisterInt("main",&value_type::main,DCMDB_PLAYER_MAIN);
		m_BR.RegisterInt("money",&value_type::money,DCMDB_PLAYER_MONEY);
		m_BR.RegisterInt("second",&value_type::second,DCMDB_PLAYER_SECOND);

		m_BR.RegisterInt("food_bonus",&value_type::food_bonus,DCMDB_PLAYER_FOOD_BONUS);

		m_BR.RegisterFlag("manage",&value_type::Flags,DCMAP_PF_Manage,DCMDB_PLAYER_MANAGE);
		m_BR.RegisterFlag("import_trusted",&value_type::Flags,DCMAP_PF_ImportTrusted,DCMDB_PLAYER_IMPORT_TRUSTED);

		m_BR.RegisterFlag("import_trusted",&value_type::Flags,DCMAP_PF_ImportTrusted,DCMDB_PLAYER_IMPORT_TRUSTED);
		m_BR.RegisterFlag("add_empty",&value_type::Flags,DCMAP_PF_AddEmpty,DCMDB_PLAYER_ADD_EMPTY);
		m_BR.RegisterFlag("add_occupied",&value_type::Flags,DCMAP_PF_AddOccupied,DCMDB_PLAYER_ADD_OCCUPIED);
		m_BR.RegisterFlag("overwrite_empty",&value_type::Flags,DCMAP_PF_OverwriteEmpty,DCMDB_PLAYER_OVERWRITE_EMPTY);
		m_BR.RegisterFlag("overwrite_occupied",&value_type::Flags,DCMAP_PF_OverwriteOccupied,DCMDB_PLAYER_OVERWRITE_OCCUPIED);
		m_BR.RegisterFlag("clear_occupied",&value_type::Flags,DCMAP_PF_ClearOccupied,DCMDB_PLAYER_CLEAR_OCCUPIED);

		m_BR.RegisterFlag("import_dip",&value_type::Flags,DCMAP_PF_ImportDip,DCMDB_PLAYER_IMPORT_DIP);
		m_BR.RegisterFlag("import_contents",&value_type::Flags,DCMAP_PF_ImportContents,DCMDB_PLAYER_IMPORT_CONTENTS);
		m_BR.RegisterFlag("import_fleets",&value_type::Flags,DCMAP_PF_ImportFleets,DCMDB_PLAYER_IMPORT_FLEETS);
		m_BR.RegisterFlag("import_alien_fleets",&value_type::Flags,DCMAP_PF_ImportAlienFleets,DCMDB_PLAYER_IMPORT_ALIEN_FLEETS);

		m_BR.RegisterInt("import_region",&value_type::import_region,DCMDB_PLAYER_IMPORT_REGION);

		m_TableVer = 3;
	}

	void DCMAPAPI FillDefaults(value_type* pinf)
	{
		memset(pinf,0,sizeof(value_type));
		pinf->race.Invalidate();
		pinf->relation.Invalidate();
		pinf->organization.Invalidate();
		pinf->icon.Invalidate();
		pinf->color1 = dcmapRGB(185,185,185);
		pinf->color2 = dcmapRGB(0,0,0);

		pinf->Flags |= DCMAP_PF_Manage|DCMAP_PF_ImportTrusted;
		pinf->Flags |= DCMAP_PF_ImportDip|DCMAP_PF_ImportContents;
		pinf->Flags |= DCMAP_PF_ImportFleets|DCMAP_PF_ImportAlienFleets;

		pinf->Flags |= DCMAP_PF_AddEmpty|DCMAP_PF_AddOccupied;
		pinf->Flags |= DCMAP_PF_OverwriteEmpty|DCMAP_PF_OverwriteOccupied;

	}
	char const* DCMAPAPI Name(){return "players";}

	void DCMAPAPI PostInit()
	{
		m_pSecrets = m_pDatabase->GetDataSourceTyped<IDCMapSecretsDataStorage>("secrets");
		base_storage::PostInit();
	}


	int DCMAPAPI OnMessage(int Msg,dcmapDWORD wParam,dcmapDWORD lParam,IDCMapObject* pSender)
	{
		return base_storage::OnMessage(Msg,wParam,lParam,pSender);
	}

	void OnDelete(value_type* pDta)
	{
		if(m_pSecrets && pDta->password.Valid())
		{
			if(m_pSecrets->SelectID(pDta->password))
				m_pSecrets->Delete();
		}
		Inherited::OnDelete(pDta);
	}

	bool WStrToBlob(int field,dcmapDBREF & data,dcmapWCSTR str)
	{
		dcmapPOINT* buf;
		int n = dcmapStrToCoordArray(str,&buf,false);
		return SetBlob(field,(dcmapBYTE*)buf,n*sizeof(dcmapPOINT));
	}

	dcmapWCSTR BlobToWStr(int field,dcmapDBREF data)
	{
		if(!data.Valid())return L"";
		dcmapPOINT* dta;
		int n = m_CLOB.GetValue(data,(dcmapBYTE**)&dta);
		if(!n)return L"";
		n /= sizeof(dcmapPOINT);
		return dcmapCoordArrayToStr(dta,n);
	}

	virtual dcmapCSTR BlobToStr(int field,dcmapDBREF data)
	{
		static string convString;
		convString = ToStr(BlobToWStr(field,data));
		return convString.c_str();
	}

	virtual bool StrToBlob(int field,dcmapDBREF & data,dcmapCSTR str)
	{
		return WStrToBlob(field,data,StrC2D(str).c_str());
	}

	virtual bool OnDataConvert(void const* pData,int size,dcmapWORD ver,value_type & res)
	{

/*
		if(ver < 2 && size == sizeof(DCMapPlayerInfo_v1)) // first versions with /10 delims
		{
			DCMapPlayerInfo_v1 const* src = (DCMapPlayerInfo_v1 const*)pData;

			res.dip.Invalidate();
			DCMapDataSourcePtr<IDCMapDipDataStorage> dip;
			if(dip.init(m_pCore,"dip"))
			{
				if(dip->SelectDipID(src->dip))
				{
					res.dip = *dip->GetID();
				}
			}
		}
*/

		/*if(ver <3 && size == sizeof(DCMapPlayerInfo_v2))
		{
		DCMapPlayerInfo_v2 const* src = (DCMapPlayerInfo_v2 const*)pData;
		res.password.Invalidate();

		dcmapCSTR pass = (dcmapCSTR)src->password;
		if(pass[0])
		{
		IDCMapSecretsDataStoragePtr secrets;
		if(secrets.init(&g_dcmapCore,"secrets"))
		{
		secrets->Insert();
		secrets->SetName(pass);
		secrets->Post(&res.password);
		}
		}
		}*/
		return true;
	}
};
DCMAP_REGISTER_STORAGE(CPlayersDataStorage,"players");


class CRacesDataStorage :	public CBaseNamedGameIndexedDataStorage<DCMapRaceInfo,IDCMapRacesDataStorage>
{
public:
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CRacesDataStorage();}

	virtual bool OnDataConvert(void const* pData,int size,dcmapWORD ver,value_type & res)
	{
		if(ver < 2) // first versions with /10 delims
		{
			res.tmin =  (float(res.tmin)/10);
			res.tmax =  (float(res.tmax)/10);

			res.growth =  (float(res.growth)/10);
			res.mining =  (float(res.mining)/10);
		}
		if(ver < 3) // O,M,E -> O,E,M
		{
			if(res.nature == 2)res.nature = 1;  
			else if(res.nature == 1)res.nature = 2;  

			if(res.basis == 2)res.basis = 1;  
			else if(res.basis == 1)res.basis = 2; 

			if(res.second == 2)res.second = 1;  
			else if(res.second == 1)res.second = 2; 
		}
		return true;
	}

	CRacesDataStorage()
	{
		m_TableVer = 3;

		m_BR.RegisterInt("nature",&value_type::nature,DCMDB_RACE_NATURE);
		m_BR.RegisterInt("basis",&value_type::basis,DCMDB_RACE_BASIS);
		m_BR.RegisterInt("second",&value_type::second,DCMDB_RACE_SECOND);

		m_BR.RegisterInt("tmin",&value_type::tmin,DCMDB_RACE_TMIN);
		m_BR.RegisterInt("tmax",&value_type::tmax,DCMDB_RACE_TMAX);
		m_BR.RegisterInt("science",&value_type::science,DCMDB_RACE_SCIENCE);
		m_BR.RegisterInt("mining",&value_type::mining,DCMDB_RACE_MINING);
		m_BR.RegisterInt("growth",&value_type::growth,DCMDB_RACE_GROWTH);
		m_BR.RegisterInt("speed",&value_type::speed,DCMDB_RACE_SPEED);
		m_BR.RegisterInt("prices",&value_type::prices,DCMDB_RACE_PRICES);
		m_BR.RegisterInt("civil_production",&value_type::civil_production,DCMDB_RACE_CIVIL_PRODUCTION);
		m_BR.RegisterInt("military_production",&value_type::military_production,DCMDB_RACE_MILITARY_PRODUCTION);
		m_BR.RegisterInt("hitpoints",&value_type::hitpoints,DCMDB_RACE_HITPOINTS);
		m_BR.RegisterInt("damage",&value_type::damage,DCMDB_RACE_DAMAGE);
		m_BR.RegisterInt("detection",&value_type::detection,DCMDB_RACE_DETECTION);
		m_BR.RegisterInt("stealth",&value_type::stealth,DCMDB_RACE_STEALTH);
		m_BR.RegisterInt("defence",&value_type::defence,DCMDB_RACE_DEFENCE);

		m_BR.RegisterInt("min_growth",&value_type::min_growth,DCMDB_RACE_MIN_GROWTH);

		m_BR.Alias("mint",DCMDB_RACE_TMIN);
		m_BR.Alias("maxt",DCMDB_RACE_TMAX);

		m_BR.Alias("RaceName",DCMDB_RACE_NAME);
		m_BR.Alias("PopBonus",DCMDB_RACE_GROWTH);
		m_BR.Alias("OutputBonus",DCMDB_RACE_MINING);
		m_BR.Alias("FirstResource",DCMDB_RACE_BASIS);
		m_BR.Alias("First",DCMDB_RACE_BASIS);
		m_BR.Alias("SecondResource",DCMDB_RACE_SECOND);

		m_BR.Alias("race_nature",DCMDB_RACE_NATURE);
		m_BR.Alias("industry_nature",DCMDB_RACE_BASIS);
		m_BR.Alias("unused_resource",DCMDB_RACE_SECOND);

		m_BR.Alias("iq",DCMDB_RACE_SCIENCE);
		m_BR.Alias("multiply",DCMDB_RACE_GROWTH);
		m_BR.Alias("attack",DCMDB_RACE_DAMAGE);
		m_BR.Alias("hp",DCMDB_RACE_HITPOINTS);
		m_BR.Alias("build_peace",DCMDB_RACE_CIVIL_PRODUCTION);
		m_BR.Alias("build_war",DCMDB_RACE_MILITARY_PRODUCTION);
		m_BR.Alias("price",DCMDB_RACE_PRICES);

		m_BR.Alias("bonus_iq",DCMDB_RACE_SCIENCE);
		m_BR.Alias("bonus_multiply",DCMDB_RACE_GROWTH);
		m_BR.Alias("bonus_attack",DCMDB_RACE_DAMAGE);
		m_BR.Alias("bonus_hp",DCMDB_RACE_HITPOINTS);
		m_BR.Alias("bonus_speed",DCMDB_RACE_SPEED);
		m_BR.Alias("bonus_build_peace",DCMDB_RACE_CIVIL_PRODUCTION);
		m_BR.Alias("bonus_build_war",DCMDB_RACE_MILITARY_PRODUCTION);
		m_BR.Alias("bonus_price",DCMDB_RACE_PRICES);
		m_BR.Alias("bonus_mining",DCMDB_RACE_MINING);

		m_BR.RegisterInt("race_id",&value_type::game_id,DCMDB_RACE_GAME_ID);
	}
	void DCMAPAPI FillDefaults(value_type* pinf)
	{
		memset(pinf,0,sizeof(value_type));
		pinf->nature = 0;
		pinf->basis=1;
		pinf->second=2;
		pinf->min_growth=1.0f;
	}
	char const* DCMAPAPI Name(){return "races";}
};
DCMAP_REGISTER_STORAGE(CRacesDataStorage,"races");


class COrganizationDataStorage :	public CBaseNamedDataStorage<DCMapOrganizationInfo,IDCMapOrganizationDataStorage>
{
public:
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new COrganizationDataStorage();}

	COrganizationDataStorage()
	{
		m_BR.RegisterWideString("full_name",&value_type::full_name);
		m_BR.RegisterInt("dip",&value_type::dip);
		m_BR.RegisterInt("color1",&value_type::color1);
		m_BR.RegisterInt("color2",&value_type::color2);
	}
	void DCMAPAPI FillDefaults(value_type* pinf)
	{
		memset(pinf,0,sizeof(value_type));
		pinf->color1 = dcmapRGB(185,185,185);
		pinf->color2 = dcmapRGB(0,0,0);
	}

	char const* DCMAPAPI Name(){return "organizations";}
};
DCMAP_REGISTER_STORAGE(COrganizationDataStorage,"organizations");


class CIconsDataStorage : public CBaseNamedDataStorage<DCMapIconInfo,IDCMapIconsDataStorage>
{
public:
	CIconsDataStorage()
	{
		dcmapDBREF filename; // Clob filename
		dcmapDBREF imgdata;  // Blob image data

		m_BR.RegisterString("filename",&value_type::filename);
		m_BR.RegisterInt("imgdata",&value_type::imgdata);
		m_BR.RegisterInt("bigimgdata",&value_type::bigimgdata);
		m_BR.RegisterInt("transparent",&value_type::transparent);
	}
	void DCMAPAPI FillDefaults(value_type* pinf)
	{
		ZeroMemory(pinf,sizeof(value_type));
		pinf->filename.Invalidate();
		pinf->imgdata.Invalidate();
		pinf->transparent = true;
	}
	dcmapCSTR DCMAPAPI Name(){return "icons";}
};
DCMAP_REGISTER_STORAGEX(CIconsDataStorage,"icons",":Hidden:");


class CPlayersGroupsDataStorage : public CBaseConnectionTableDataStorage<DCMapPlayersGroupsInfo,IDCMapPlayersGroupsDataStorage,CPlayersGroupsDataStorage>
{
public:
	char const* DCMAPAPI Name(){return "playersgroups";}
	CPlayersGroupsDataStorage()
	{
		m_strParent1Table = "groups";
		m_strParent2Table = "players";

		m_BR.RegisterReference("group",&value_type::c1,"groups",DCMDB_PLAYERSGROUP_GROUP);
		m_BR.RegisterReference("player",&value_type::c2,"players",DCMDB_PLAYERSGROUP_PLAYER);
		m_BR.RegisterInt("turn",&value_type::turn,DCMDB_PLAYERSGROUP_TURN);
		m_BR.RegisterInt("dipid",&value_type::dipid,DCMDB_PLAYERSGROUP_DIPID);

	}
	void DCMAPAPI FillDefaults(value_type* pinf)
	{
		memset(pinf,0,sizeof(value_type));
	}
	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(IDCMapDataStorage);
		DCMAP_MODULE_SUPPORTS(IDCMapPlayersGroupsDataStorage);
		DCMAP_MODULE_SUPPORTS(CPlayersGroupsDataStorage);
		return 0;
	}
};
DCMAP_REGISTER_STORAGEX(CPlayersGroupsDataStorage,"playersgroups",":Hidden:");
