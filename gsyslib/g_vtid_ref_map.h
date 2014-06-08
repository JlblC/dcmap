#pragma once
#ifndef G_VTID_REF_MAP_H
#define G_VTID_REF_MAP_H

#include "g_gsys.h"
#include "g_vtid_var.h"

namespace gsys
{
	class vtid_ref_map
	{
		GSYS_VTID_FRIEND(vtid_ref_map);
		typedef strimap<string,gsys::vtid_ref> MapType;
		MapType m_map;
	public:
		gsys::vtid_result OnGetVar(gsys::vtid_acess_data const *dta, gsys::vtid_ref* ref)
		{
			MapType::iterator it = m_map.find(dta->name);
			if(it == m_map.end())return gsys::vtid_err_not_found;
			*ref = it->second;
			return gsys::vtid_ok;
		}
	public:
		vtid_ref_map(void){}
		~vtid_ref_map(void){}

		void add(string const& name,gsys::vtid_ref const& ref)
		{
			m_map[name] = ref;
		}
		void remove(gsys::vtid_ref const& ref)
		{
			MapType::iterator it = m_map.begin();
			while(it != m_map.end())
			{
				if(it->second == ref) it = m_map.erase(it);
				else ++it;
			}
		}
		void remove(string const& name)
		{
			MapType::iterator it = m_map.find(name);
			if(it != m_map.end())m_map.erase(it);
		}
	};
}

#endif