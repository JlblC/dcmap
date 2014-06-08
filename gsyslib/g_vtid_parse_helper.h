#pragma once

#include "g_gsys.h"

#include "g_vtid_lib.h"
#include "g_vtid_var.h"

namespace gsys{namespace vtid{

	typedef std::list<gsys::vtid_ref> ref_obj_list;

	class lval_holder;
	class rval_holder;

	class lval_holder
	{
		friend class rval_holder;
		gsys::vtid_member_ref m_Value;
		bool m_bSetProp;
		gsys::vtid_typeinfo m_Type;
	public:
		bool init(istream & base_is,ref_obj_list const& vecRefObj);
		bool init(char const* czString,ref_obj_list const& vecRefObj);
		// write this ref value
		bool apply(gsys::vtid_ref const& ref);
		bool apply(const char* value);
		gsys::vtid_typeinfo type()const{return m_Type;}
	};

	class rval_holder
	{
		friend class lval_holder;
		gsys::vtid_refvar m_Value;
		string m_strValue;
		int m_iApplyMode;
	public:
		bool init(istream & base_is,ref_obj_list const& vecRefObj,gsys::vtid_typeinfo type);
		bool init(char const* czString,ref_obj_list const& vecRefObj,gsys::vtid_typeinfo type);
		bool write(gsys::vtid_ref const& ref);
		bool write(CLValueHolder& lVal);
		bool execute(gsys::vtid_typeinfo type,gsys::vtid_refvar & res);
		bool get_str(std::string &val);
	};

	void read_param(istream & is,string &param);

	bool ref_find(string const& strRef,ref_obj_list const& vecRefObj,gsys::vtid_member_ref& ref);
	bool parse_object_ref(istream & is,ref_obj_list const& vecRefObj,gsys::vtid_member_ref& ref);
	bool parse_object_ref(string const& strKeyword,istream & is,ref_obj_list const& vecRefObj,gsys::vtid_member_ref& ref);
}};