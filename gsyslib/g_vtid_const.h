#pragma once
#include "g_vtid.h"
#include "detail/g_vtid_macro.h"

namespace gsys
{
	class vtid_valmap
	{
	public:
		typedef double value_type;
	private:
		typedef strimap<string,value_type> StrMapType;
		typedef std::map<value_type,string> ValueMapType;

		StrMapType   m_strMap;
		ValueMapType m_valMap;
		int m_iLastValue;
		int m_iMaxValue;
		//ValueMapType m_valueMap;
	public:
		gsys::vtid_result OnSetValue(gsys::vtid_acess_data const *dta, gsys::vtid_ref* ref);

		vtid_valmap(void);
		~vtid_valmap(void);

		value_type operator[](std::string const& name)const;
		char const* operator[](value_type i)const;

		bool find(std::string const& name,value_type& dta)const;
		int maxvalue()const{return m_iMaxValue;}
	};

	class vtid_const
	{
		GSYS_VTID_FRIEND(vtid_const);
		typedef strimap<string,vtid_valmap*> ValMapType;

		ValMapType   m_valMap;
		//ValueMapType m_valueMap;
	public:
		typedef vtid_valmap::value_type value_type;

		gsys::vtid_result OnDefConst(gsys::vtid_acess_data const *dta, gsys::vtid_ref* ref);

		vtid_const(void);
		~vtid_const(void);
		value_type operator[](std::string const& name)const;

		bool find(std::string const& name,value_type& dta)const;
	};
}
