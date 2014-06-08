#include "StdAfx.h"
#include "Parser.h"

const CVarValue CVarValue::nil_value;

void SubDef::Acquire(CObjectParser* mod)
{
    context = mod;
    if(mod)mod->AddRef();
}
void SubDef::Disacquire()
{
    if(context && context->Release() <= 0)
    {
        delete context;
    }
    context = 0;
}

CVarValue ObjectRef::get()
{
    return CVarValue();
}

CVarValue ObjectRef::get(CVarValue const& v)
{
    if(pModule)
    {
        return pModule->GetLocalVariable(v.as_string());
    }
    return CVarValue();
}

bool ObjectRef::set(CVarValue const& v)
{
    return false;
}

bool ObjectRef::retrieve(CVarValue const& v,CVarValue ** result)
{
    if(pModule)
    {
        return pModule->GetLocalVariableLValue(v.as_string(),result);
    }
    return false;
}

void ObjectRef::add_ref()
{
    if(pModule)
    {
        pModule->AddRef();
    }
}

IDCMapModule* ObjectRef::object_module()
{
    if(pModule)
    {
        return pModule->GetModule();
    }
    return 0;
}


void ObjectRef::free()
{
    if(pModule && pModule->Release() <= 0)
    {
        delete pModule;
        pModule = 0;
    }
}

void ObjectRef::clone(const ObjectRef& obj)
{
    *this = obj;
    if(pModule)
    {
        pModule->AddRef();
    }
}

DCMAPSYSEXPORT IDCMapVarValue* DCMAPLIBAPI dcmapCreateVarValue()
{
    return new CDCMapVarValue();
}

CVarValue ServiceDef::subobj( CVarValue const& v,wstring &str )
{
	return subobj(v);
}

bool ServiceDef::have_subobj( CVarValue const& v,wstring &str )
{
	int vtp = pmod->ScriptExec(DCMEXEC_QUERY,v.to_string().c_str(),0,0);
	return (vtp & DCMEXEC_FAIL) == 0;
}

CVarValue ServiceDef::subobj( CVarValue const& v)
{
	if(type == 0) // bare object ref
	{
		int vtp = pmod->ScriptExec(DCMEXEC_QUERY,v.to_string().c_str(),0,0);
		if(vtp & DCMEXEC_FAIL)
		{
			
		}
		else if(vtp & DCMEXEC_GET)
		{
			CDCMapVarValue vr;
			pmod->ScriptExec(DCMEXEC_GET,v.to_string().c_str(),0,&vr);
			return vr;
		}
		else
		{
			return CVarValue(pmod,vtp,v.to_string());
		}
	}
	return CVarValue();
}

CVarValue ServiceDef::retrieve_member( CVarValue const& v,wstring &str )
{
	if(type == 0) // bare object ref
	{
		int vtp = pmod->ScriptExec(DCMEXEC_QUERY,v.to_string().c_str(),0,0);
		return CVarValue(pmod,vtp,v.to_string());
	}
	return CVarValue();
}


bool ServiceDef::Assign(wstring const& str,CVarValue const& v)
{
	CDCMapVarValue vp;
	CDCMapVarValue vr(v);

	//if(type & DCMEXEC_SET_MEMBER)
	pmod->ScriptExec(DCMEXEC_SET,str.c_str(),&vp,&vr);	

	return true;
}

void ServiceDef::Execute(wstring const& str, CVarValue const& param,CVarValue & res )
{
	CDCMapVarValue vp(param);
	CDCMapVarValue vr;

	if(type)
	{
		pmod->ScriptExec(type,str.c_str(),&vp,&vr);
	}
	else
	{
		pmod->ScriptExec(1,0,&vp,&vr);
	}
	res = vr;
}