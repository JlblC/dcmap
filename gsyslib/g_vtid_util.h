#pragma once
#ifndef G_VTID_UTIL_H
#define G_VTID_UTIL_H

#include <boost/shared_ptr.hpp>

#include "g_vtid_var.h"

// helper definitions for vtid
#define GSYS_VTID_USE_STD_VARS(Vars) \
	GSYS_VTID_CLASS_ACCESS_PROC(Vars.OnDeclVar,float,"var")\
	GSYS_VTID_CLASS_ACCESS_PROC(Vars.OnDeclVar,float,"float")\
	GSYS_VTID_CLASS_ACCESS_PROC(Vars.OnDeclVar,float,"real")\
	GSYS_VTID_CLASS_ACCESS_PROC(Vars.OnDeclVar,bool,"bool")\
	GSYS_VTID_CLASS_ACCESS_PROC(Vars.OnDeclVar,gsys::gstring,"string")\

#define GSYS_VTID_USE_VAR(Vars,Type,Name) GSYS_VTID_CLASS_ACCESS_PROC(Vars.OnDeclVar,Type,Name)

namespace gsys
{
	class vtid_var_helper
	{
		typedef boost::shared_ptr<vtid_var>  VariablePtr;
		typedef strimap<gstring,vtid_var*> MapVars;
		MapVars m_mapVars;
	public:
		~vtid_var_helper()
		{
			_foreach(MapVars,m_mapVars,it)delete it->second;
		}
		vtid_var_helper(){};
		vtid_var_helper(vtid_var_helper const& var)
		{
			_const_foreach(MapVars,var.m_mapVars,it)
			{
				vtid_var* ptvar = new vtid_var;
				m_mapVars[it->first] = ptvar;
				ptvar->set_value(*(it->second));
			}
		};
		vtid_var_helper& operator=(vtid_var_helper const& var)
		{
			_foreach(MapVars,m_mapVars,it)delete it->second;
			m_mapVars.clear();
			_const_foreach(MapVars,var.m_mapVars,it)
			{
				vtid_var* ptvar = new vtid_var(it->second->typeinfo());
				m_mapVars[it->first] = ptvar;
				ptvar->set_value(*(it->second));
			}
			return *this;
		};
		template<typename T>
		vtid_result OnTypedDeclVar(gsys::vtid_acess_data const *dta, gsys::vtid_ref* ref)
		{
			if(dta->acc_mode != vtid_access_init) return vtid_err_acess_denied;
			// test param (name)
			if(!dta->param || dta->param[0]==0) return vtid_err_invalid_param;

			MapVars::iterator it = m_mapVars.find(dta->param);
			if(it != m_mapVars.end()) return vtid_err_already_exists;

			vtid_var* ptvar = new vtid_var;
			ptvar->create<T>();
			m_mapVars[dta->param] = ptvar;
			*ref = *ptvar;
			return vtid_ok;
		}

		vtid_result OnDeclVar(gsys::vtid_acess_data const *dta, gsys::vtid_ref* ref)
		{
			if(dta->acc_mode != vtid_access_init) return vtid_err_acess_denied;
			// test param (name)
			if(!dta->param || dta->param[0]==0) return vtid_err_invalid_param;
			
            // test type
			//if(dta->type_name)
	
			if(!dta->type)return vtid_err_invalid_type;
			if(!dta->type->creatable())return vtid_err_invalid_type;
            
			MapVars::iterator it = m_mapVars.find(dta->param);
			if(it != m_mapVars.end()) return vtid_err_already_exists;

			vtid_var* ptvar = new vtid_var(dta->type);
			m_mapVars[dta->param] = ptvar;
			*ref = *ptvar;
			return vtid_ok;
		}

		vtid_result OnGetRef(gsys::vtid_acess_data const *dta, gsys::vtid_ref* ref)
		{
			/*if(dta->acc_mode == vtid_access_init)
			{
				return vtid_err_not_found;
			}
			else*/
			{
				MapVars::iterator it = m_mapVars.find(dta->name);
				if(it == m_mapVars.end()) return vtid_err_not_found;
				
				*ref = *(it->second);
				return vtid_ok;
			}
		}

		template <class T>
		bool AddVariable(gstring const& name,T const& V)
		{
			MapVars::iterator it = m_mapVars.find(name);
			if(it != m_mapVars.end()) return false;
			vtid_var* ptvar = new vtid_var(gsys::vtid_ref().init(V));
			m_mapVars[name] = ptvar;
			return true;
		}
		template <class T>
		bool AddVariable(gstring const& name)
		{
			MapVars::iterator it = m_mapVars.find(name);
			if(it != m_mapVars.end()) return false;
			vtid_var* ptvar = new vtid_var(gsys::get_vtid_lib()->get<T>());
			m_mapVars[name] = ptvar;
			return true;
		}
	};

	class vtid_multi_ref_object
	{
		GSYS_VTID_FRIEND(vtid_multi_ref_object)

		vector<vtid_ref> m_vecRef;

		vtid_result OnGetRef(gsys::vtid_acess_data const *dta, gsys::vtid_ref* ref)
		{
			_foreach(vector<vtid_ref>,m_vecRef,i)
			{
				vtid_ref & r=*i;

				vtid_member_ref mref;
				vtid_result result =  r.get_decl_ref(dta,&mref);
				if(result == vtid_err_not_found) continue;

				if(result > 0)
				{
					if(mref.is_func()) return vtid_err_internal_error;
					*ref = mref.ref();
				}
				return result;
			}
			return gsys::vtid_err_not_found;
		}
	public:
		vtid_ref ref()const{return vtid_ref().init(*this);}

		vtid_multi_ref_object & operator << (vtid_ref const& ref)
		{
			m_vecRef.push_back(ref);
			return *this;
		}
	};
}

#endif