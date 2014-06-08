#include "StdAfx.h"
#include "fleetstorage.h"

DCMAP_REGISTER_STORAGEX(CActionsStorage,"actions",":Hidden:");
DCMAP_REGISTER_STORAGEX(CFleetsStorage,"fleets",":Hidden:");
DCMAP_REGISTER_STORAGEX(CProjectsStorage,"projects",":Hidden:");
DCMAP_REGISTER_STORAGEX(CBuildingsStorage,"buildings",":Hidden:");
DCMAP_REGISTER_STORAGEX(CProjectActsStorage,"projectacts",":Hidden:");
DCMAP_REGISTER_STORAGEX(CActionsRequestedStorage,"actionsrequested",":Hidden:");
DCMAP_REGISTER_STORAGEX(CUnitTagsStorage,"unittags",":Hidden:");

bool CFleetsStorage::OnTestDataConvert(void const* pData,int size,dcmapWORD ver,value_type & res)
{
	if(ver<m_TableVer)return 0;
	return 1;
}

CFleetsStorage::CFleetsStorage()
{
	m_TableVer = 3;

	m_BR.RegisterReference("owner",&value_type::parent,"players",DCMDB_FLEET_OWNER);
	m_BR.RegisterReference("ruler",&value_type::ruler,"players",DCMDB_FLEET_RULER);
	m_BR.RegisterReference("loader",&value_type::loader,"players",DCMDB_FLEET_LOADER);
    m_BR.RegisterReference("parentgroup",&value_type::parentgroup,"fleets",DCMDB_FLEET_PARENTGROUP);
    m_BR.RegisterReference("parentgroup2",&value_type::parentgroup2,"fleets",DCMDB_FLEET_PARENTGROUP2);
    m_BR.RegisterReference("parentgroup3",&value_type::parentgroup3,"fleets",DCMDB_FLEET_PARENTGROUP3);
	m_BR.RegisterReference("icon",&value_type::icon,"icons",DCMDB_FLEET_ICON);

	m_BR.RegisterWideString("name",&value_type::name,DCMDB_FLEET_NAME);
	m_BR.RegisterWideString("caption",&value_type::caption,DCMDB_FLEET_CAPTION);
	m_BR.RegisterWideString("public_name",&value_type::public_name,DCMDB_FLEET_PUBLIC_NAME);

	m_BR.Alias("extname",DCMDB_FLEET_PUBLIC_NAME);

	m_BR.RegisterInt("from_x",&value_type::from_x,DCMDB_FLEET_FROM_X);
	m_BR.RegisterInt("from_y",&value_type::from_y,DCMDB_FLEET_FROM_Y);
	m_BR.RegisterInt("x",&value_type::x,DCMDB_FLEET_X);
	m_BR.RegisterInt("y",&value_type::y,DCMDB_FLEET_Y);

	m_BR.RegisterInt("tta",&value_type::tta,DCMDB_FLEET_TTA);
	m_BR.RegisterInt("turn",&value_type::turn,DCMDB_FLEET_TURN);

	m_BR.RegisterInt("fly_range",&value_type::fly_range,DCMDB_FLEET_FLY_RANGE);
	m_BR.RegisterInt("fly_speed",&value_type::fly_speed,DCMDB_FLEET_FLY_SPEED);
	m_BR.RegisterInt("stealth",&value_type::stealth,DCMDB_FLEET_STEALTH);
	m_BR.RegisterInt("behavior",&value_type::behavior,DCMDB_FLEET_BEHAVIOR);

	m_BR.RegisterInt("ship_count",&value_type::ship_count,DCMDB_FLEET_SHIP_COUNT);

	m_BR.RegisterInt("transport_capacity",&value_type::transport_capacity,DCMDB_FLEET_TRANSPORT_CAPACITY);
	m_BR.RegisterInt("cargo_count",&value_type::cargo_count,DCMDB_FLEET_CARGO_COUNT);
	m_BR.RegisterInt("weight",&value_type::weight,DCMDB_FLEET_WEIGHT);

    m_BR.RegisterInt("group",&value_type::group,DCMDB_FLEET_GROUP);
	m_BR.RegisterInt("sortindex",&value_type::sortindex,DCMDB_FLEET_SORTINDEX);
	m_BR.RegisterInt("list_image",&value_type::list_image,DCMDB_FLEET_LIST_IMAGE);
	m_BR.RegisterInt("list_status_image",&value_type::list_status_image,DCMDB_FLEET_LIST_STATUS_IMAGE);
	m_BR.RegisterInt("type",&value_type::type,DCMDB_FLEET_TYPE);
	m_BR.RegisterInt("dip",&value_type::dip,DCMDB_FLEET_DIP);
	m_BR.RegisterInt("changes",&value_type::changes,DCMDB_FLEET_CHANGES);

	m_BR.RegisterFlag("in_transit",&value_type::flags,DCMAP_FF_InTransit,DCMDB_FLEET_IN_TRANSIT);
	m_BR.RegisterFlag("hidden",&value_type::flags,DCMAP_FF_Hidden,DCMDB_FLEET_HIDDEN);
	m_BR.RegisterFlag("foreign",&value_type::flags,DCMAP_FF_Foreign,DCMDB_FLEET_FOREIGN);
    m_BR.RegisterFlag("garrison",&value_type::flags,DCMAP_FF_Garrison,DCMDB_FLEET_GARRISON);

	m_BR.RegisterFlag("history",&value_type::flags,DCMAP_FF_History,DCMDB_FLEET_HISTORY);
	m_BR.RegisterFlag("outdated",&value_type::flags,DCMAP_FF_Outdated,DCMDB_FLEET_OUTDATED);
	m_BR.RegisterFlag("deleted",&value_type::flags,DCMAP_FF_Deleted,DCMDB_FLEET_DELETED);

	m_BR.RegisterFlag("script_active",&value_type::flags,DCMAP_FF_ScriptActive,DCMDB_FLEET_SCRIPT_ACTIVE);
	m_BR.RegisterFlag("script_ok",&value_type::flags,DCMAP_FF_ScriptOk,DCMDB_FLEET_SCRIPT_OK);
	m_BR.RegisterString("script_message",&value_type::script_message,DCMDB_FLEET_SCRIPT_MESSAGE);

	m_GroupStart = m_GroupDummie.end();
	m_GroupEnd  = m_GroupDummie.end();
}

void CFleetsStorage::OnDelete(value_type* pDta)
{
	if(pDta->group != DCMAP_FGRP_Ungroupped && pDta->group != DCMAP_FGRP_Groupped)
	{
		m_GroupIndex.erase(pDta->id);
	}
	Inherited::OnDelete(pDta);
}

void CFleetsStorage::UnIndexValue( const value_type* pDta )
{
	m_GroupStart = m_GroupDummie.end();
	m_GroupEnd  = m_GroupDummie.end();

    if(pDta->group == DCMAP_FGRP_RootGroup)
    {
        m_RootGroup.Invalidate();
    }
	NameIndex nind(pDta);

    if(pDta->parentgroup.Valid())
    {
        GroupIndex::iterator it = m_GroupIndex.find(pDta->parentgroup);
        if(it != m_GroupIndex.end())
		{
			GroupNameIndex::iterator nit = it->second.find(nind);
			while(nit != it->second.end() && nit->second != pDta->id.id)++nit;
			if(nit != it->second.end())
				it->second.erase(nit);
		}
		//if(it->second.empty()){m_GroupIndex.erase(it);}
    }

    if(pDta->parentgroup2.Valid())
    {
		GroupIndex::iterator it = m_GroupIndex.find(pDta->parentgroup2);
		if(it != m_GroupIndex.end())
		{
			GroupNameIndex::iterator nit = it->second.find(nind);
				while(nit != it->second.end() && nit->second != pDta->id.id)++nit;
			if(nit != it->second.end())it->second.erase(nit);
		}
		//if(it->second.empty()){m_GroupIndex.erase(it);}
    }

    if(pDta->parentgroup3.Valid())
    {
		GroupIndex::iterator it = m_GroupIndex.find(pDta->parentgroup3);
		if(it != m_GroupIndex.end())
		{
			GroupNameIndex::iterator nit = it->second.find(nind);
				while(nit != it->second.end() && nit->second != pDta->id.id)++nit;
			if(nit != it->second.end())it->second.erase(nit);
		}
		//if(it->second.empty()){m_GroupIndex.erase(it);}
    }
    Inherited::UnIndexValue(pDta);
}


bool CFleetsStorage::NeedReindex(value_type const* old,value_type const* newd)
{
	if(old->parentgroup  != newd->parentgroup  ||
		old->parentgroup2 != newd->parentgroup2 ||
		old->parentgroup3 != newd->parentgroup3   ) return true;
	if(old->sortindex  != newd->sortindex)return true;
	if(wcscmp(old->caption,newd->caption)!=0)return true;

	return   Inherited::NeedReindex(old,newd);
}

bool CFleetsStorage::IndexValue( const value_type* pDta )
{
	m_GroupStart = m_GroupDummie.end();
	m_GroupEnd  = m_GroupDummie.end();

    if(pDta->group == DCMAP_FGRP_RootGroup)
    {
        if(m_RootGroup.Valid())return false;
        m_RootGroup = pDta->id;
    }

    if(!Inherited::IndexValue(pDta))return false;

	NameIndex nind(pDta);

    if(pDta->parentgroup.Valid())
	{
		GroupNameIndex &gind = m_GroupIndex[pDta->parentgroup];
		gind.insert(GroupNameIndex::value_type(nind,pDta->id.id));
	}

	if(pDta->parentgroup2.Valid())
	{
		GroupNameIndex &gind = m_GroupIndex[pDta->parentgroup2];
		gind.insert(GroupNameIndex::value_type(nind,pDta->id.id));
	}

	if(pDta->parentgroup3.Valid())
	{
		GroupNameIndex &gind = m_GroupIndex[pDta->parentgroup3];
		gind.insert(GroupNameIndex::value_type(nind,pDta->id.id));
	}

    return true;
}

bool DCMAPAPI CFleetsStorage::SelectGroupNames(dcmapDBREF const& ref,int sind,dcmapDCSTR name)
{
	if(!DataLoad())return false;

	GroupIndex::iterator it = m_GroupIndex.find(ref);
	if(it == m_GroupIndex.end())
	{
		m_GroupStart = m_GroupDummie.end();
		m_GroupEnd  = m_GroupDummie.end();
		return false;
	}
	NameIndex nind(sind,name);

	m_GroupStart = it->second.lower_bound(nind);
	m_GroupEnd = it->second.upper_bound(nind);

	if(m_GroupStart == m_GroupEnd)
	{
		m_GroupStart = m_GroupDummie.end();
		m_GroupEnd  = m_GroupDummie.end();
		return false;
	}
	m_pCurrentRecord = &m_lstData[m_GroupStart->second];
	return true;
}

int DCMAPAPI CFleetsStorage::GroupSize(dcmapDBREF const& ref)
{
	GroupIndex::iterator it = m_GroupIndex.find(ref);
	if(it != m_GroupIndex.end())
	{
		return it->second.size();
	}
	return 0;
}

bool DCMAPAPI CFleetsStorage::SelectGroup( dcmapDBREF const& ref )
{
	if(!DataLoad())return false;
	GroupIndex::iterator it = m_GroupIndex.find(ref);
	if(it == m_GroupIndex.end())
	{
		m_GroupStart = m_GroupDummie.end();
		m_GroupEnd  = m_GroupDummie.end();
		return false;
	}
	m_GroupStart = it->second.begin();
	m_GroupEnd = it->second.end();

	if(m_GroupStart == m_GroupEnd)
	{
		m_GroupIndex.erase(it);
		m_GroupStart = m_GroupDummie.end();
		m_GroupEnd  = m_GroupDummie.end();
		return false;
	}
	m_pCurrentRecord = &m_lstData[m_GroupStart->second];
	return true;
}

bool DCMAPAPI CFleetsStorage::Next()
{
	if(m_GroupStart != m_GroupEnd)
	{
		m_GroupStart++;
		if(m_GroupStart == m_GroupEnd)
		{
			m_GroupStart = m_GroupDummie.end();
			m_GroupEnd  = m_GroupDummie.end();
			m_pCurrentRecord = 0;
			return false;
		}
		m_pCurrentRecord = &m_lstData[m_GroupStart->second];
		return true;
	}
	return Inherited::Next();
}

void CFleetsStorage::ClearIndex()
{
	Inherited::ClearIndex();
	m_GroupIndex.clear();

	m_GroupStart = m_GroupDummie.end();
	m_GroupEnd  = m_GroupDummie.end();
}

void* DCMAPAPI CFleetsStorage::CastTo( DCMapInterfaceID idInterface )
{
	DCMAP_MODULE_SUPPORTS(CFleetsStorage);
	return Inherited::CastTo(idInterface);
}

void DCMAPAPI CFleetsStorage::FillDefaults( value_type* pinf )
{
	Inherited::FillDefaults(pinf);

	pinf->sortindex = FGI_Regular;
	pinf->list_image = FICO_None;
	pinf->list_status_image = FICO_STATUS_None;

	pinf->parentgroup.Invalidate();
	pinf->parentgroup2.Invalidate();
	pinf->parentgroup3.Invalidate();
	pinf->parent.Invalidate();
	pinf->ruler.Invalidate();
	pinf->loader.Invalidate();
	pinf->icon.Invalidate();

	pinf->behavior = 0x313444;
}

dcmapWCSTR DCMAPAPI CFleetsStorage::GetDisplayName()
{
	if(!m_pCurrentRecord)return L"";
	return m_pCurrentRecord->name;
}

CProjectsStorage::CProjectsStorage():Inherited("players","owner")
{
	m_BR.RegisterWideString("name",&value_type::name,DCMDB_PROJECT_NAME);
	m_BR.RegisterWideString("description",&value_type::descriptin,DCMDB_PROJECT_DESCRIPTION);
	m_BR.RegisterInt("turn",&value_type::turn,DCMDB_PROJECT_TURN);

	m_BR.RegisterReference("chassis",&value_type::chassis,"components",DCMDB_PROJECT_CHASSIS);

	m_BR.RegisterInt("cost_main",&value_type::cost_main,DCMDB_PROJECT_COST_MAIN);
	m_BR.RegisterInt("cost_second",&value_type::cost_second,DCMDB_PROJECT_COST_SECOND);
	m_BR.RegisterInt("cost_money",&value_type::cost_money,DCMDB_PROJECT_COST_MONEY);
	m_BR.RegisterInt("cost_pepl",&value_type::cost_pepl,DCMDB_PROJECT_COST_PEPL);

	m_BR.RegisterInt("support_main",&value_type::support_main,DCMDB_PROJECT_SUPPORT_MAIN);
	m_BR.RegisterInt("support_second",&value_type::support_second,DCMDB_PROJECT_SUPPORT_SECOND);
	m_BR.RegisterInt("hit_points",&value_type::hit_points,DCMDB_PROJECT_HIT_POINTS);

	m_BR.RegisterInt("bonus_o",&value_type::bonus_o,DCMDB_PROJECT_BONUS_O);
	m_BR.RegisterInt("bonus_e",&value_type::bonus_e,DCMDB_PROJECT_BONUS_E);
	m_BR.RegisterInt("bonus_m",&value_type::bonus_m,DCMDB_PROJECT_BONUS_M);
	m_BR.RegisterInt("bonus_s",&value_type::bonus_s,DCMDB_PROJECT_BONUS_S);
	m_BR.RegisterInt("bonus_production",&value_type::bonus_production,DCMDB_PROJECT_BONUS_PRODUCTION);

	m_BR.RegisterInt("req_tehn_level",&value_type::req_tehn_level,DCMDB_PROJECT_REQ_TEHN_LEVEL);
	m_BR.RegisterInt("scan_strength",&value_type::scan_strength,DCMDB_PROJECT_SCAN_STRENGTH);
	m_BR.RegisterInt("detect_range",&value_type::detect_range,DCMDB_PROJECT_DETECT_RANGE);
	m_BR.RegisterInt("laser_damage",&value_type::laser_damage,DCMDB_PROJECT_LASER_DAMAGE);
	m_BR.RegisterInt("bomb_damage",&value_type::bomb_damage,DCMDB_PROJECT_BOMB_DAMAGE);
	m_BR.RegisterInt("laser_number",&value_type::laser_number,DCMDB_PROJECT_LASER_NUMBER);
	m_BR.RegisterInt("bomb_number",&value_type::bomb_number,DCMDB_PROJECT_BOMB_NUMBER);

	m_BR.RegisterInt("laser_dr",&value_type::laser_dr,DCMDB_PROJECT_LASER_DR);
	m_BR.RegisterInt("bomb_dr",&value_type::bomb_dr,DCMDB_PROJECT_BOMB_DR);
	m_BR.RegisterInt("laser_ar",&value_type::laser_ar,DCMDB_PROJECT_LASER_AR);
	m_BR.RegisterInt("bomb_ar",&value_type::bomb_ar,DCMDB_PROJECT_BOMB_AR);

	m_BR.RegisterInt("laser_target",&value_type::laser_target,DCMDB_PROJECT_LASER_TARGET);
	m_BR.RegisterInt("bomb_target",&value_type::bomb_target,DCMDB_PROJECT_BOMB_TARGET);
	m_BR.RegisterInt("carrier_capacity",&value_type::carrier_capacity,DCMDB_PROJECT_CARRIER_CAPACITY);
	m_BR.RegisterInt("transport_capacity",&value_type::transport_capacity,DCMDB_PROJECT_TRANSPORT_CAPACITY);

	m_BR.RegisterInt("fly_range",&value_type::fly_range,DCMDB_PROJECT_FLY_RANGE);
	m_BR.RegisterInt("fly_speed",&value_type::fly_speed,DCMDB_PROJECT_FLY_SPEED);
	m_BR.RegisterInt("maxcount",&value_type::maxcount,DCMDB_PROJECT_MAXCOUNT);
	m_BR.RegisterInt("weight",&value_type::weight,DCMDB_PROJECT_WEIGHT);
	m_BR.RegisterInt("building_class",&value_type::building_class,DCMDB_PROJECT_BUILDING_CLASS);

	m_BR.RegisterInt("build_speed",&value_type::build_speed,DCMDB_PROJECT_BUILD_SPEED);
	m_BR.RegisterInt("requires_pepl",&value_type::requires_pepl,DCMDB_PROJECT_REQUIRES_PEPL);

//	m_BR.RegisterInt("build_allow",&value_type::build_allow,DCMDB_PROJECT_BUILD_ALLOW);
//	m_BR.RegisterInt("build_req",&value_type::build_req,DCMDB_PROJECT_BUILD_REQ);
//	m_BR.RegisterInt("build_unique",&value_type::build_unique,DCMDB_PROJECT_BUILD_UNIQUE);

	m_BR.RegisterInt("carapace",&value_type::carapace,DCMDB_PROJECT_CARAPACE);
	m_BR.RegisterInt("color",&value_type::color,DCMDB_PROJECT_COLOR);

	m_BR.RegisterFlag("is_war",&value_type::flags,DCMAP_PRF_War,DCMDB_PROJECT_IS_WAR);
	m_BR.RegisterFlag("is_building",&value_type::flags,DCMAP_PRF_Building,DCMDB_PROJECT_IS_BUILDING);
	m_BR.RegisterFlag("is_ground_unit",&value_type::flags,DCMAP_PRF_GroundUnit,DCMDB_PROJECT_IS_GROUND_UNIT);
	m_BR.RegisterFlag("is_space_ship",&value_type::flags,DCMAP_PRF_SpaceShip,DCMDB_PROJECT_IS_SPACE_SHIP);
	m_BR.RegisterFlag("offensive",&value_type::flags,DCMAP_PRF_Offensive,DCMDB_PROJECT_OFFENSIVE);

	m_BR.RegisterInt("is_transportable",&value_type::transportable,DCMDB_PROJECT_IS_TRANSPORTABLE);

	m_BR.Alias("bonus_surface",DCMDB_PROJECT_BONUS_SURFACE);
};


CBaseBuildingsStorage::CBaseBuildingsStorage(dcmapCSTR parent):Inherited(parent)
{
	m_BR.RegisterInt("hp",&value_type::hp,DCMDB_BUILDING_HP);
	m_BR.RegisterReference("project",&value_type::project,"projects",DCMDB_BUILDING_PROJECT);
	m_BR.RegisterInt("turn",&value_type::turn,DCMDB_BUILDING_TURN);

	m_BR.RegisterInt("weight",&value_type::weight,DCMDB_BUILDING_WEIGHT);
	m_BR.RegisterInt("carapace",&value_type::carapace,DCMDB_BUILDING_CARAPACE);
	m_BR.RegisterInt("color",&value_type::color,DCMDB_BUILDING_COLOR);
	m_BR.RegisterInt("complete",&value_type::complete,DCMDB_BUILDING_COMPLETE);
	m_BR.RegisterInt("ready_in",&value_type::ready_in,DCMDB_BUILDING_READY_IN);
	m_BR.RegisterInt("age",&value_type::age,DCMDB_BUILDING_AGE);

	m_BR.RegisterFlag("planetary",&value_type::flags,DCMAP_BF_Planetary,DCMDB_BUILDING_PLANETARY);
	m_BR.RegisterFlag("outdated",&value_type::flags,DCMAP_BF_Outdated,DCMDB_BUILDING_OUTDATED);
	m_BR.RegisterFlag("queued",&value_type::flags,DCMAP_BF_Queued,DCMDB_BUILDING_QUEUED);
	m_BR.RegisterFlag("disbanded",&value_type::flags,DCMAP_BF_Disbanded,DCMDB_BUILDING_DISBANDED);
	m_BR.RegisterFlag("tagged",&value_type::flags,DCMAP_BF_Tagged,DCMDB_BUILDING_TAGGED);
};

CBuildingsStorage::CBuildingsStorage():CBaseBuildingsStorage("fleets")
{

};


/*
CHarrisonStorage::CHarrisonStorage():CBaseBuildingsStorage("planets")
{

};
DCMAP_REGISTER(CHarrisonStorage,"PDataStorage","garrison");
*/

/*
create_single_design_unit
drop_msgs
unsubscribe_player
archive_prototype
cancel_cargo_load
demolish_building
save_edited_design
change_fleet_name
cancel_jump
exploration_ministry_save
store_planet_fake_owner
stepbystepwizard_save
create_new_fleet
accept_nationality_offer
share_fleet_access
save_player_settings
add_building_to_que
drop_prototype
customise_container
store_user_vote
save_diplomacy_settings
save_ankete
all_ext_planet_perms
create_new_unit_design
store_planet_fake_stats
hidefleet
cancel_dip_relation
recall_fleet_permission
new_script_weight
drop_building_from_que
gimmeelephant
move_fleet
unshare_me
subscribe_me_to
create_prototype_design_unit
kill_player
move_unit_to_fleet
store_action
change_planet_name
change_behaviour
save_interface_settings
store_planet_mark
building_to_top
activate_free_subscription
change_dip_relation
showpost
set_bid
cancel_percent_change
create_channel
pack_parts
archive_design
enable_fleet_script
drop_design
subscribe_player
drop_fleet_script
remove_ext_permission
add_to_digest
store_messages_cleanup_setup
add_player_to_dip_by_name
rename_channel
create_fleet_from_choosen
alter_channel_permissions
cancel_action
change_planet_perms
unpack_container
renameacc
clear_queue
*/

/*
change_fleet_name,
cancel_jump,
create_new_fleet
add_building_to_que
hidefleet
drop_building_from_que
move_fleet
move_unit_to_fleet
change_planet_name
building_to_top
demolish_building
*/

/*

<x-dc-perform>
<act name="add_building_to_que" id="1">
<building_id>14</building_id>
<planetid>509:509</planetid>
</act>
<act name="add_building_to_que" id="2">
<building_id>66308</building_id>
<planetid>509:509</planetid>
</act>
</x-dc-perform>

В ответ получаете нечто типа такого:
<iframe>
<act id="1" result="error">
<error id="10019">
<txt>Недостаточно населения на планете</txt>
</error>
</act>
<act id="2" result="ok" return-id="8922103"/>
</iframe>
*/

CActionsStorage::CActionsStorage():Inherited("players","owner")
{
	m_BR.RegisterWideString("new_name",&value_type::new_name,DCMDB_ACTION_NEW_NAME);
	m_BR.RegisterWideString("old_name",&value_type::old_name,DCMDB_ACTION_OLD_NAME);

	m_BR.RegisterWideString("new_public_name",&value_type::new_public_name,DCMDB_ACTION_NEW_PUBLIC_NAME);
	m_BR.RegisterWideString("old_public_name",&value_type::old_public_name,DCMDB_ACTION_OLD_PUBLIC_NAME);

	m_BR.RegisterInt("type",&value_type::type,DCMDB_ACTION_TYPE);
	m_BR.RegisterInt("mode",&value_type::mode,DCMDB_ACTION_MODE);
	m_BR.RegisterInt("old_mode",&value_type::old_mode,DCMDB_ACTION_OLD_MODE);

	m_BR.RegisterInt("turn",&value_type::turn,DCMDB_ACTION_TURN);
	m_BR.RegisterInt("from_x",&value_type::from_x,DCMDB_ACTION_FROM_X);
	m_BR.RegisterInt("from_y",&value_type::from_y,DCMDB_ACTION_FROM_Y);
	m_BR.RegisterInt("x",&value_type::x,DCMDB_ACTION_X);
	m_BR.RegisterInt("y",&value_type::y,DCMDB_ACTION_Y);

	m_BR.RegisterInt("send_id",&value_type::send_id,DCMDB_ACTION_SEND_ID);
	m_BR.RegisterReference("fleet",&value_type::fleet,"fleets",DCMDB_ACTION_FLEET);
	m_BR.RegisterReference("source_fleet",&value_type::source_fleet,"fleets",DCMDB_ACTION_SOURCE_FLEET);
	m_BR.RegisterReference("building",&value_type::building,"buildings",DCMDB_ACTION_BUILDING);
	m_BR.RegisterReference("project",&value_type::project,"projects",DCMDB_ACTION_PROJECT);
	m_BR.RegisterReference("planet",&value_type::planet,"planets",DCMDB_ACTION_PLANET);
	m_BR.RegisterReference("actreq",&value_type::actreq,"actionsrequested",DCMDB_ACTION_ACTREQ);

	m_BR.RegisterFlag("sended",&value_type::Flags,DCMAP_ACTIF_Sended,DCMDB_ACTION_SENDED);
};

bool DCMAPAPI CActionsStorage::SelectSendID( int id )
{
	if(!Inherited::DataLoad())return false;
	if(Inherited::m_pCurrentRecord && Inherited::m_pCurrentRecord->id.id == id)return true;
	Inherited::m_pCurrentRecord = 0;
	if(m_lstData.is_free(id))return false;
	Inherited::m_pCurrentRecord = &m_lstData[id];
	return true;
}

CProjectActsStorage::CProjectActsStorage():Inherited("projects","project")
{
	m_BR.RegisterInt("actid",&value_type::actid,DCMDB_PROJECTACT_ACTID);

	m_BR.RegisterInt("cost_main",&value_type::cost_main,DCMDB_PROJECTACT_COST_MAIN);
	m_BR.RegisterInt("cost_second",&value_type::cost_second,DCMDB_PROJECTACT_COST_SECOND);
	m_BR.RegisterInt("cost_money",&value_type::cost_money,DCMDB_PROJECTACT_COST_MONEY);
	m_BR.RegisterInt("cost_pepl",&value_type::cost_pepl,DCMDB_PROJECTACT_COST_PEPL);
	m_BR.RegisterInt("maxcount",&value_type::maxcount,DCMDB_PROJECTACT_MAXCOUNT);

	m_BR.RegisterFlag("planet_empty",&value_type::flags,DCMAP_PAF_PlanetEmpty,DCMDB_PROJECTACT_PLANET_EMPTY);
	m_BR.RegisterFlag("planet_dip_enemy",&value_type::flags,DCMAP_PAF_PlanetDipEnemy,DCMDB_PROJECTACT_PLANET_DIP_ENEMY);

	m_BR.RegisterFlag("planet_dip_undefined",&value_type::flags,DCMAP_PAF_PlanetDipUndefined,DCMDB_PROJECTACT_PLANET_DIP_UNDEFINED);
	m_BR.RegisterFlag("planet_dip_neutrals",&value_type::flags,DCMAP_PAF_PlanetDipNeutrals,DCMDB_PROJECTACT_PLANET_DIP_NEUTRALS);
	m_BR.RegisterFlag("planet_dip_ally",&value_type::flags,DCMAP_PAF_PlanetDipAlly,DCMDB_PROJECTACT_PLANET_DIP_ALLY);
	m_BR.RegisterFlag("planet_dip_lord",&value_type::flags,DCMAP_PAF_PlanetDipLord,DCMDB_PROJECTACT_PLANET_DIP_LORD);
	m_BR.RegisterFlag("planet_dip_vassal",&value_type::flags,DCMAP_PAF_PlanetDipVassal,DCMDB_PROJECTACT_PLANET_DIP_VASSAL);

}

CActionsRequestedStorage::CActionsRequestedStorage():Inherited("buildings","unit")
{
	m_BR.RegisterInt("actid",&value_type::actid,DCMDB_ACTREQ_ACTID);
	m_BR.RegisterInt("turn",&value_type::turn,DCMDB_ACTREQ_TURN);
	m_BR.RegisterReference("action",&value_type::action,"projectacts",DCMDB_ACTREQ_ACTION);
	m_BR.RegisterReference("fleet",&value_type::fleet,"fleets",DCMDB_ACTREQ_FLEET);
	m_BR.RegisterReference("loader",&value_type::loader,"players",DCMDB_ACTREQ_LOADER);

	m_BR.RegisterFlag("canceled",&value_type::flags,DCMAP_ARF_Canceled,DCMDB_ACTREQ_CANCELED);
}

CUnitTagsStorage::CUnitTagsStorage():Inherited("buildings","unit")
{

	m_BR.RegisterReference("project",&value_type::project,"projects",DCMDB_UNITTAG_PROJECT);
	m_BR.RegisterReference("chassis",&value_type::chassis,"components",DCMDB_UNITTAG_CHASSIS);
	m_BR.RegisterReference("race",&value_type::race,"races",DCMDB_UNITTAG_RACE);
	m_BR.RegisterReference("load_fleet",&value_type::load_fleet,"fleets",DCMDB_UNITTAG_LOAD_FLEET);

	m_BR.RegisterInt("amount",&value_type::amount,DCMDB_UNITTAG_AMOUNT);
	m_BR.RegisterInt("itemid",&value_type::itemid,DCMDB_UNITTAG_ITEMID);
	m_BR.RegisterInt("type",&value_type::type,DCMDB_UNITTAG_TYPE);
	m_BR.RegisterInt("level",&value_type::level,DCMDB_UNITTAG_LEVEL);

	m_BR.RegisterFlag("unwrapped",&value_type::flags,DCMAP_UTF_Unwrapped,DCMDB_UNITTAG_UNWRAPPED);
	m_BR.RegisterFlag("loading",&value_type::flags,DCMAP_UTF_Loading,DCMDB_UNITTAG_LOADING);

}