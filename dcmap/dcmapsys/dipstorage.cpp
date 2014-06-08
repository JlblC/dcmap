#include "StdAfx.h"
#include "BaseDataStorage.h"

class CDipDataStorage :	public CBaseNamedDataStorage<DCMapDipInfo,IDCMapDipDataStorage>
{
	typedef CBaseNamedDataStorage<DCMapDipInfo,IDCMapDipDataStorage> parent;
public:
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CDipDataStorage();}

	int m_iNullInitMode;

	CDipDataStorage()
	{
		m_BR.RegisterInt("dipid",&value_type::dipid,DCMDB_DIP_DIPID);
		m_BR.RegisterInt("color1",&value_type::color1,DCMDB_DIP_COLOR1);
		m_BR.RegisterInt("color2",&value_type::color2,DCMDB_DIP_COLOR2);
		m_BR.RegisterInt("native",&value_type::color2,DCMDB_DIP_NATIVE);
		m_BR.RegisterInt("gameid",&value_type::gameid,DCMDB_DIP_GAMEID);
		m_iNullInitMode=0;
	}
	void DCMAPAPI FillDefaults(value_type* pinf)
	{
		memset(pinf,0,sizeof(value_type));
		pinf->dipid = DCMAP_DIP_CUSTOM;

		//pinf->race.Invalidate();
		//pinf->relation.Invalidate();
		//pinf->organization.Invalidate();
		//pinf->icon.Invalidate();
		//pinf->color1 = dcmapRGB(185,185,185);
		//pinf->color2 = dcmapRGB(0,0,0);
	}
	dcmapCSTR DCMAPAPI Name(){return "dip";}

	bool DCMAPAPI SelectDipID(dcmapBYTE id)
	{
		if(!DataLoad())return false;

		if(id == 0 || id > 10)return false;
		_foreach(DataList,m_lstData,it)
		{
			if(it->dipid == id && it->native)
			{
				return SelectID(it->id);
			}
		}
		// if no nature dip found - select any
		_foreach(DataList,m_lstData,it)
		{
			if(it->dipid == id)
			{
				return SelectID(it->id);
			}
		}
		return false;
	}

	virtual dcmapDWORD GenerateMagic()
	{
		if(m_iNullInitMode)
		{
			return ++m_iNullInitMode;
		}
		return parent::GenerateMagic();
	}

	bool DCMAPAPI LoadEmpty()
	{
		if(!base_named_storage::LoadEmpty())return false;
		// Fill with default dip relations

		m_bBlockDataUpdate=true;
		m_iNullInitMode=1;

		Insert();
		SetName("Нейтральный");
		m_temp.dipid = DCMAP_DIP_NEUTRALS;
		m_temp.color1 = 16748711;
		m_temp.native = true;
		m_temp.gameid = DCMAP_DIP_GAME_NEUTRALS;
		Post();

		Insert();
		SetName("Союзник");
		m_temp.dipid = DCMAP_DIP_ALLY;
		m_temp.color1 = 3340850;
		m_temp.native = true;
		m_temp.gameid = DCMAP_DIP_GAME_ALLY;
		Post();

		Insert();
		SetName("Враг");
		m_temp.dipid = DCMAP_DIP_ENEMIES;
		m_temp.color1 = 2171391;
		m_temp.native = true;
		m_temp.gameid = DCMAP_DIP_GAME_ENEMIES;
		Post();		

		Insert();
		SetName("Лорд");
		m_temp.dipid = DCMAP_DIP_LORD;
		m_temp.color1 = 3341050;
		m_temp.native = true;
		m_temp.gameid = DCMAP_DIP_GAME_LORD;
		Post();	

		Insert();
		SetName("Вассал");
		m_temp.dipid = DCMAP_DIP_VASSAL;
		m_temp.color1 = 16777088;
		m_temp.native = true;
		m_temp.gameid = DCMAP_DIP_GAME_VASSAL;
		Post();	

		Insert();
		SetName("Мульт");
		m_temp.dipid = DCMAP_DIP_MULT;
		m_temp.color1 = 16397050;
		m_temp.native = true;
		m_temp.gameid = DCMAP_DIP_GAME_ALLY;
		Post();	

		Insert();
		SetName("Потенциальный союзник");
		m_temp.dipid = DCMAP_DIP_POTENTIAL_ALLY;
		m_temp.color1 = 11796416;
		Post();	

		Insert();
		SetName("Потенциальный враг");
		m_temp.dipid = DCMAP_DIP_POTENTIAL_ENEMIES;
		m_temp.color1 = 10722559;
		m_temp.gameid = DCMAP_DIP_GAME_NEUTRALS;
		Post();	

		Insert();
		SetName("Потенциальный лорд");
		m_temp.dipid = DCMAP_DIP_POTENTIAL_LORD;
		m_temp.color1 = 13434877;
		m_temp.gameid = DCMAP_DIP_GAME_NEUTRALS;
		Post();	

		Insert();
		SetName("Потенциальный вассал");
		m_temp.dipid = DCMAP_DIP_POTENTIAL_VASSAL;
		m_temp.color1 = 16646095;
		m_temp.gameid = DCMAP_DIP_GAME_NEUTRALS;
		Post();	

		m_iNullInitMode=0;
		m_bBlockDataUpdate=false;
		return true;
	}
};

DCMAP_REGISTER_STORAGE(CDipDataStorage,"dip");