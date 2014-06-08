#include "common.h"

#include "../g_vtid_const.h"
#include "../g_vtid_lib.h"
#include "../g_vtid_util.h"
#include <string>

namespace gsys{

vtid_valmap::vtid_valmap(void)
{
	m_iLastValue=-1;
	m_iMaxValue=0;
}

vtid_valmap::~vtid_valmap(void)
{
}

vtid_result vtid_valmap::OnSetValue(gsys::vtid_acess_data const *dta, gsys::vtid_ref* ref)
{
	if(dta->acc_mode != vtid_access_init)
	{
		return vtid_err_acess_denied;
	}
	StrMapType::iterator it = m_strMap.find(dta->name);
	if(it != m_strMap.end())
	{
		return vtid_err_already_exists;
	}
	value_type val;
	if(dta->value && gsys::lex_convert<value_type>(val,dta->value))
	{
		m_iLastValue = val;
		m_strMap[dta->name] = val;
	}
	else
	{
		m_strMap[dta->name] = ++m_iLastValue;
	}
	m_valMap[m_iLastValue] = dta->name;

	if(m_iLastValue > m_iMaxValue) m_iMaxValue = m_iLastValue;

	return vtid_proceeded;
}

vtid_valmap::value_type vtid_valmap::operator[](std::string const& name)const
{
	StrMapType::const_iterator it = m_strMap.find(name);
	if(it != m_strMap.end())
	{
		return it->second;
	}
	return -1;
}

bool vtid_valmap::find(std::string const& name,value_type& dta)const
{
	StrMapType::const_iterator it = m_strMap.find(name);
	if(it != m_strMap.end())
	{
		dta = it->second;
		return true;
	}
	return false;
}

char const* vtid_valmap::operator[](value_type i)const
{
	ValueMapType::const_iterator it = m_valMap.find(i);
	if(it != m_valMap.end())
	{
		return it->second.c_str();
	}
	return "";
}

vtid_const::vtid_const(void)
{
}

vtid_const::~vtid_const(void)
{
	_foreach(ValMapType,m_valMap,it)delete it->second;
}

gsys::vtid_result vtid_const::OnDefConst(gsys::vtid_acess_data const *dta, gsys::vtid_ref* ref)
{
	if(dta->acc_mode != vtid_access_init)
	{
		return vtid_err_acess_denied;
	}
	ValMapType::iterator it = m_valMap.find(dta->name);
	if(it == m_valMap.end())
	{
		if(dta->acc_mode != vtid_access_init)return vtid_err_not_found;
		m_valMap[dta->name] = new vtid_valmap;
		*ref = vtid_make_ref(*m_valMap[dta->name]);
		return vtid_ok;
	}
	*ref = vtid_make_ref(*it->second);
	return vtid_ok;
}

vtid_const::value_type vtid_const::operator[](std::string const& name)const
{
	value_type val = -1;
	find(name,val);
	return val;
}

bool vtid_const::find(std::string const& name,value_type& dta)const
{
	string::size_type pos = name.find_first_of("_");
	if(pos == string::npos)return false;
	string scope = name.substr(0,pos);
	string val = name.substr(pos+1);

	ValMapType::const_iterator it = m_valMap.find(scope);
	if(it == m_valMap.end())return false;

	return it->second->find(val,dta);
}


}