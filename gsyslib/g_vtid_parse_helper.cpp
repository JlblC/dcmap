#include "common.h"
#include "../g_vtid_parse_helper.h"

bool gsys::vtid::rval_holder::execute(gsys::vtid_typeinfo type,gsys::vtid_refvar & res)
{
	if(m_iApplyMode == 1)
	{
		res.make_ref(m_Value);
	}
	else if(m_iApplyMode == 2)
	{
		string val;
		if(!res.create(type))return false;
		if(!m_Value.get_value(val))return false;
		if(!res.set_value(val))return false;		
	}
	else
	{
		if(!res.create(type))return false;
		if(!res.set_value(m_strValue))return false;
	}
	return true;
}

bool gsys::vtid::rval_holder::get_str(std::string &val)
{
	if(!m_Value.get_value(val))return false;	
	return true;
}

bool CRValueHolder::WriteTo(gsys::vtid_ref const& ref,resloc const& file)
{
	if(m_iApplyMode == 1)
	{
		ref.set_value(m_Value);
	}
	else if(m_iApplyMode == 2)
	{
		string val;
		if(!m_Value.get_value(val))return false;
		if(!ref.set_value(val))return false;		
	}
	else
	{
		ref.set_value(m_strValue);
	}
	return true;
}

bool CRValueHolder::WriteTo(gsys::vtid_ref const& ref)
{
	if(m_iApplyMode == 1)
	{
		ref.set_value(m_Value);
	}
	else if(m_iApplyMode == 2)
	{
		string val;
		if(!m_Value.get_value(val))return false;
		if(!ref.set_value(val))return false;		
	}
	else
	{
		ref.set_value(m_strValue);
	}
	return true;
}

bool CRValueHolder::Init(char const* czString,VTIDRefObjList const& vecRefObj,gsys::vtid_typeinfo type)
{
	std::istrstream is(czString,strlen(czString));
	return Init(is,vecRefObj,type);
}

bool CRValueHolder::Init(istream & is,VTIDRefObjList const& vecRefObj,gsys::vtid_typeinfo type)
{
	is >> ws;

	// read value
	m_iApplyMode=0;

	bool strict=false;

	char pc = is.peek();
	if(pc == '$') // acces by binary ref
	{
		m_iApplyMode = 1;
		is.ignore();
		pc = is.peek();
		if(pc == '$')
		{
			strict=true; // double $ - means strict conversion rules 
			is.ignore();
		}
		is >> ws;
	}
	else if(pc == '@') // access by string value  
	{
		m_iApplyMode = 2;
		is.ignore();
		is >> ws;
	}

	if(m_iApplyMode)
	{
		vtid_member_ref cref;
		if(!ParseObjectRef(is,vecRefObj,cref)) return false;
		if(cref.is_prop_w()) 
		{
			// all ok, we can read W property as normal variable
		}
		else if(cref.is_func())
		{
			// We currently can't get value from function (property)
			return false;
		}
		if(!cref.test_access(gsys::vtid_access_read))return false;
		if(strict && !cref.test_access(gsys::vtid_access_write))return false;

		m_Value = cref.ref();
		if(m_iApplyMode == 1)
		{
			if(strict && type != cref.ref().typeinfo())
			{
				return false;
			}
			else if(type->applyeble())
			{
				// try to apply unknown types
			}
			else if(type != cref.ref().typeinfo() || !type->copyable())
			{
				m_iApplyMode=2;
			}
		}

		if(m_iApplyMode == 2)
		{
			if(!(cref.ref().typeinfo()->can_save_to_string() &&
				type->can_init_frorm_string()))return false;
		}
	}
	else
	{
		string val;
		is >> istream_line(val);
		gsys::unquote(val);
		if(type->creatable() && type->copyable() && m_Value.create(type))
		{
			if(!m_Value.set_value(val))return false;
			m_iApplyMode=1;
		}
		else
		{
			m_strValue = val;
		}
	}
	return true;
}

bool CRValueHolder::WriteTo(CLValueHolder& lVal)
{
	if(lVal.m_bSetProp)
	{
		gsys::vtid_refvar ref;
		if(!Execute(lVal.m_Type,ref))return false;
		lVal.m_Value.call(ref);
	}
	else
	{
		if(!WriteTo(lVal.m_Value.ref()))return false;
	}
	return true;
}

bool CLValueHolder::Init(char const* czString,VTIDRefObjList const& vecRefObj)
{
	std::istrstream is(czString,strlen(czString));
	return Init(is,vecRefObj);
}

bool CLValueHolder::Init(istream & base_is,VTIDRefObjList const& vecRefObj)
{
	if(!ParseObjectRef(base_is,vecRefObj,m_Value)) return false;

	if(m_Value.is_prop_w())
	{
		m_Type = m_Value.ref().typeinfo();
		m_bSetProp=true;
	}
	else if(m_Value.is_func() || m_Value.is_prop_w())
	{
		m_bSetProp = true;
		// get first param
		if(m_Value.fn_num_formal_params() == 1 && m_Value.fn_return_type()->type() == typeid(void))
		{
			m_Type = m_Value.fn_param_type(0);
		}
		else return false; // we do not support this type of functions
	}
	else
	{
		m_bSetProp = false;
		if(!m_Value.test_access(gsys::vtid_access_write))return false;
		m_Type = m_Value.ref().typeinfo();
	}

	return true;
}

// write this ref value
bool CLValueHolder::ApplyValue(gsys::vtid_ref const& ref)
{
	if(m_bSetProp)
	{
		m_Value.call(ref);
	}
	else
	{
		return m_Value.ref().set_value(ref);
	}
	return true;
}

bool CLValueHolder::ApplyValue(const char* value)
{
	if(m_bSetProp)
	{
		gsys::vtid_var ref;
		if(!ref.create(m_Value.ref().vtid()))return false;
		if(!ref.set_value(value))return false;
		m_Value.call(ref);
	}
	else
	{
		return m_Value.ref().set_value(value);
	}
	return true;
}



void ReadParam(istream & is,string &param)
{
	int brnum[]={0,0,0};
	char quote_char=0;;

	is >> ws;
	char pc;
	while(!is.eof())
	{
		pc = is.peek();
		if(!quote_char)
		{
			if(pc == ',')
			{
				if((brnum[0]+brnum[1]+brnum[2]) == 0)
				{
					is.ignore();
					break;
				}
			}
			else if(is.eof())
			{
				break;
			}
			else switch(pc)
			{
			case '{':
				brnum[0]++;
				break;
			case '}':
				brnum[0]--;
				break;
			case '(':
				brnum[1]++;
				break;
			case ')':
				brnum[1]--;
				break;	
			case '[':
				brnum[2]++;
				break;
			case ']':
				brnum[2]--;
				break;		
			case '"':
			case '\'':
			case '`':
				quote_char = pc;
				break;
			}
			if(brnum[0]<0)break;
			if(brnum[1]<0)break;
			if(brnum[2]<0)break;
		}
		else
		{
			if(pc == quote_char)quote_char=0;
		}
		param += pc;
		is.ignore();
	}
	size_t n = param.find_last_not_of(" \t");
	param.erase(n+1);
	gsys::unquote(param);
}

bool RefFind(string const& strRef,VTIDRefObjList const& vecRefObj,vtid_member_ref& ref)
{

	vtid_member_ref cref;

	int sz = vecRefObj.size();

	_const_foreach(VTIDRefObjList, vecRefObj,i)
	{
		cref = i->get_prop(strRef,vtid_access_modify);
		if(cref){;ref = cref;return true;}
	}
	return false;
};
bool ParseObjectRef(string const& strKeyword,istream & is,VTIDRefObjList const& vecRefObj,vtid_member_ref& ref)
{
	vtid_member_ref cref;
	if(!RefFind(strKeyword,vecRefObj,cref))return false;

	string strkw;

	while(1)
	{
		char pc = is.peek();

		if(pc == '.' || pc == '/')
		{
			is.ignore();
			strkw.clear();

			is >>  gsys::istream_keyword(strkw) >> ws;

			if(cref.is_func())
			{
				//  we couldn't get member of function
				return false; 
			}

			cref = cref.ref().get_prop(strkw,vtid_access_modify);
			if(!cref)return false;
		}
		else break; 

		if(is.fail())return false;
	}
	is >> ws;
	ref = cref;
	return true;
}

bool ParseObjectRef(istream & is,VTIDRefObjList const& vecRefObj,vtid_member_ref& ref)
{
	string strKeyword;
	is >> ws >> gsys::istream_keyword(strKeyword) >> ws;
	return ParseObjectRef(strKeyword,is,vecRefObj,ref);
}


