#include "common.h"
#include "../g_vtid_var.h"


bool gsys::detail::vtid_variable::set_value(const vtid_variable& var)const
{GSYS_CFN;
	GSYS_ASSERTOBJ(m_vtid);
	GSYS_ASSERTPTR(m_ptr);

	GSYS_ASSERTREF(var);

	if(var.m_vtid == m_vtid && m_vtid->BaseOp.h_copy_obj)
	{
		GSYS_ASSERTPTR(var.m_ptr);

		m_vtid->BaseOp.h_copy_obj(m_ptr,var.m_ptr);
		return true;
	}
	else if(m_vtid->BaseOp.h_apply_obj)
	{
		if(m_vtid->BaseOp.h_apply_obj(m_ptr,var.m_ptr,var.m_vtid))
			return true;
	}

	if(m_vtid->ParentType)
	{
		vtid_variable pvar(m_vtid->ParentType,((gsys_byte*)(m_ptr))+m_vtid->iParentOffset);
		return pvar.set_value(var);
	}
	return false;
}

void gsys::detail::vtid_variable::receive_ref_info(vtid_ref_info_receiver& pRes)
{
	_const_foreach(vtid_t::DeclMap,m_vtid->m_mapDecl,it)
	{
		vtid_ref_info rinfo;

		vtid_decl const& dcl = it->second;
		rinfo.strName = it->first;
		if(dcl.h_class_proc == 0 && dcl.h_access_var == 0)
		{ // simple coded property - subject to export
			rinfo.ref.m_ptr = ((gsys_byte*)m_ptr)+dcl.uiOffset;;   
			rinfo.ref.m_vtid = dcl.vtidType;
			rinfo.strDesc = dcl.strDescription;
            pRes(rinfo);
		}
		else if(dcl.h_class_proc != 0 && dcl.h_access_var == 0 && dcl.uiOffset != gsys::invalid_pt_diff)
		{
			rinfo.ref.m_ptr = ((gsys_byte*)m_ptr)+dcl.uiOffset;;   
			rinfo.ref.m_vtid = dcl.vtidType;
			rinfo.strDesc = dcl.strDescription;
			pRes(rinfo);
		}
		else if(dcl.h_serialize_proc)
		{
			if(dcl.h_serialize_proc(m_ptr,rinfo))
			{
				pRes(rinfo);
			}
		}
	}	
    if(m_vtid->ExportProc)
	{
		m_vtid->ExportProc(m_ptr,pRes);
	}
	
	if(m_vtid->ParentType)
	{
		vtid_variable var(m_vtid->ParentType,((gsys_byte*)(m_ptr))+m_vtid->iParentOffset);
		var.receive_ref_info(pRes);
	}
}

vtid_result gsys::detail::vtid_variable::get_decl_ref(
	vtid_access_mode mode, // in: access mode
	string const& name,  // in: acces name
	string const* param, // in: acces param
	string const* type,  // in: acces type mod (could differ from default)
	string const* value,  // in: acces type mod (could differ from default)
	vtid_member_ref* mref)const   // out: out valid reference
{
	vtid_acess_data dta;

	dta.acc_mode = mode;
	dta.name = name.c_str();
	if(param)dta.param = param->c_str();
	if(type && !type->empty())dta.type_name = type->c_str();
	if(value)dta.value = value->c_str();

	return get_decl_ref(&dta,mref);
}

vtid_result gsys::detail::vtid_variable::get_decl_ref(
	vtid_access_mode mode, // in: access mode
	gstring const& name,  // in: acces name
	gstring const* param, // in: acces param
	gstring const* type,  // in: acces type mod (could differ from default)
	gstring const* value,  // in: acces type mod (could differ from default)
	vtid_member_ref* mref)const   // out: out valid reference
{
	vtid_acess_data dta;

	dta.acc_mode = mode;
	dta.name = name.c_str();
	if(param)dta.param = param->c_str();
	if(type && !type->empty())dta.type_name = type->c_str();
	if(value)dta.value = value->c_str();

	return get_decl_ref(&dta,mref);
}

vtid_result gsys::detail::vtid_variable::get_decl_ref(
	vtid_access_mode mode, // in: access mode
	string const& name,  // in: acces name
	vtid_member_ref* mref)const   // out: out valid reference
{
	vtid_acess_data dta;
	dta.acc_mode = mode;
	dta.name = name.c_str();
	return get_decl_ref(&dta,mref);
}


vtid_result gsys::detail::vtid_variable::get_decl_ref(vtid_acess_data const *dta,
													  vtid_member_ref* mref)const
{
	GSYS_ASSERTOBJ(m_vtid);
	GSYS_ASSERTPTR(m_ptr);

    gsys::detail::vtid_access_var_proc acc_proc=0;
	vtid_typeinfo vt;

	vtid_t::DeclMap::const_iterator it = m_vtid->m_mapDecl.find(dta->name);
	if(it == m_vtid->m_mapDecl.end()) // if not defined try use general access proc
	{
		if(m_vtid->AccessProc)
		{
			acc_proc = m_vtid->AccessProc;
			vt = GSYS_DETAIL_VTID_LIB->get<void>();
		}
		else 
		{
			if(m_vtid->ParentType)
			{
				vtid_variable var(m_vtid->ParentType,((gsys_byte*)(m_ptr))+m_vtid->iParentOffset);
				return var.get_decl_ref(dta,mref);
			}
			else return vtid_err_not_found;
		}
	}
	else
	{
		vtid_decl const& dcl = it->second;

		if((dcl.wAccessMask & dta->acc_mode) == 0) return vtid_err_acess_denied;

		GSYS_ASSERTOBJ(dcl.vtidType);

		void* pt;
		
		// fill ref_member from class offset if any
		if(dcl.uiOffset != invalid_pt_diff) 
		{ 
			pt = ((gsys_byte*)m_ptr)+dcl.uiOffset;
			mref->m_refMember.m_ptr = pt;
			mref->m_refMember.m_vtid = dcl.vtidType;
		}
		else
		{
			mref->m_refMember = vtid_ref(); // no member ref
		}

		if(dcl.h_class_proc) // this is exported procedure or 'coded' property
		{
			mref->m_refObject.m_ptr = m_ptr;   // init parent
			mref->m_refObject.m_vtid = m_vtid; // init parent
			mref->m_pMemberDecl = &dcl;
			return vtid_ok;
		}
		else if(dcl.h_access_var == 0) // use simple acces by mem offset
		{ // use raw access		
			mref->m_refObject.m_ptr = m_ptr;   // init parent
			mref->m_refObject.m_vtid = m_vtid; // init parent
			mref->m_pMemberDecl = &dcl;
			return vtid_ok;
		}
		else // use specified access proc
		{
			acc_proc = dcl.h_access_var;
			vt = dcl.vtidType;
		}
	}

	vtid_acess_data accdata(*dta);
	accdata.ptr = m_ptr;

	if(dta->type_name)
	{
		GSYS_ASSERTOBJ(dta->type_name);
		accdata.type = GSYS_DETAIL_VTID_LIB->get_by_name(dta->type_name);
		if(!accdata.type)
		{
			accdata.type = GSYS_DETAIL_VTID_LIB->get<void>();
		}
	}
	else
	{
		accdata.type = vt;
	}

	GSYS_ASSERT_CODE(acc_proc);
	GSYS_ASSERTOBJ(accdata.type);

	mref->m_pMemberDecl = 0;
	mref->m_refObject.m_ptr = m_ptr;   // init parent
	mref->m_refObject.m_vtid = m_vtid; // init parent

	vtid_result r = acc_proc(&accdata,&(mref->m_refMember));

	if(r==vtid_err_not_found)
	{
		if(m_vtid->ParentType)
		{
			vtid_variable var(m_vtid->ParentType,((gsys_byte*)(m_ptr))+m_vtid->iParentOffset);
			r = var.get_decl_ref(dta,mref);
		}
	}
	return r;
}


vtid_ref gsys::vtid_member_ref::call(vtid_ref const*  vars,int num)
{
	vtid_ref r;
	GSYS_ASSERTOBJ(m_pMemberDecl);
	if(m_pMemberDecl->h_class_proc)
	{
		vtid_result res = m_pMemberDecl->h_class_proc(m_refObject.m_ptr,num,vars,&r);
		if(res == vtid_err_invalid_function_parameters)
		{ // try to find sutable function

		}
	}
	return r;
}
vtid_ref gsys::vtid_member_ref::call()
{
	vtid_ref r;
	GSYS_ASSERTOBJ(m_pMemberDecl);
	if(m_pMemberDecl->h_class_proc)
	{
		m_pMemberDecl->h_class_proc(m_refObject.m_ptr,0,0,&r);
	}
	return r;
}

vtid_member_ref gsys::vtid_member_ref::find_overloaded_fn(vtid_ref const*  vars,int num)
{
	if(!m_pMemberDecl)return *this;
	GSYS_ASSERTOBJ(m_pMemberDecl);
	GSYS_ASSERT(m_refObject.valid());

	int n = m_pMemberDecl->vecProcParams.size();
	if(!vars)
	{
		if(n == num) return *this;
		if(m_pMemberDecl->strName.empty())return *this;
		
		typedef vtid_t::DeclMap::const_iterator iter;
        pair<iter,iter> range = m_refObject.m_vtid->m_mapDecl.equal_range(m_pMemberDecl->strName);

		GSYS_ASSERT(range.first != m_refObject.m_vtid->m_mapDecl.end());

        for(iter i=range.first;i!=range.second;++i)
		{
			if(i->second.vecProcParams.size() == num)
			{
				vtid_member_ref ref(*this);
				ref.m_pMemberDecl = &i->second;
				return ref;
			}
		}
	}
	return *this;
}

