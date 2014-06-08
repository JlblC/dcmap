#include "common.h"

#include "../g_vtid_lib.h"
#include "../g_vtid_util.h"
#include "../g_text.h"

#include <string>


namespace gsys
{
	vtid_lib*  g_LibVTID;

	namespace {
		
		struct vtid_lib_destruct_t
		{
			~vtid_lib_destruct_t()
			{
				if(g_LibVTID)
				{
					delete g_LibVTID;
					g_LibVTID = 0;
				}
			}
		} vtid_lib_destruct;
	};
};

static void register_vtid_util()
{
/*	GSYS_VTID_BEGIN_DYNAMIC(::gsys::vtid_multi_ref_object)
	{
		GSYS_VTID_USE_CLASS_ACCESS_PROC(OnGetRef);
	}
	GSYS_VTID_END_DYNAMIC
*/	
}

bool gsys::vtid_lib::_GetConst(char const* name,vtid_const::value_type& val)
{
	if(!name || !name[0])return false;
	if( char_classify::alpha(name[0]))
	{
		if(m_pParent)return m_pParent->GetConst(name,val);
		else return m_consts.find(name,val);
	}
	else
	{
		return lex_convert(val,name);
	}
}

bool gsys::vtid_lib::init_as_extention(gsys::vtid_lib* pParent)
{
	GSYS_ASSERTPTR(pParent);
	m_pParent = pParent;

	m_pParent->add_extention(this);

	m_vtSources.clear();

	return true;
}

bool gsys::vtid_lib::init_as_extention(gsys_handle hParent)
{
	return init_as_extention(reinterpret_cast<gsys::vtid_lib*>(hParent));
}

bool GSYS_API gsys::vtid_lib::add_extention(vtid_lib * pLib)
{
	register_source(*(pLib->get_source()));
	return true;
}

void gsys::vtid_lib::free()
{
	m_pParent=0;


	clear();
}

bool gsys::vtid_lib::init()
{
	register_base_types();
	register_vtid_util();
	register_source(m_vtSources);
	m_vtSources.clear();
	return true;
}

bool gsys::vtid_lib::register_source(VectorVTIDSource const& vtSources)
{
	// first pass - add all vtid_t's into library
	_const_foreach(VectorVTIDSource,vtSources,i) 
	{
		detail::vtid_source const& vtidTypeSource = *i;

		if(!vtidTypeSource.czTypeName)
		{
			//vtidTypeSource.bBad = true;
		}
		else if(m_mapVTID.find(vtidTypeSource.czTypeName) == m_mapVTID.end())
		{

			GSYS_ASSERTSTR(vtidTypeSource.czTypeName);
			GSYS_ASSERT_CALLBACK(vtidTypeSource.TypeProc);

			vtid_t* ptVTID = new vtid_t;
			m_mapVTID[vtidTypeSource.czTypeName] = ptVTID;

			if(vtidTypeSource.pTypeHolder)*vtidTypeSource.pTypeHolder = ptVTID;

			ptVTID->czTypeName = vtidTypeSource.czTypeName;
			ptVTID->TypeProc = vtidTypeSource.TypeProc;
			ptVTID->AccessProc = vtidTypeSource.AccessProc;
			ptVTID->ExportProc = vtidTypeSource.ExportProc;
			ptVTID->ClassFactory = vtidTypeSource.ClassFactory;

			ptVTID->BaseOp = vtidTypeSource.BaseOp;
			ptVTID->UserInf = vtidTypeSource.UserInf;

			ptVTID->m_iAliasCount = (int)vtidTypeSource.vtNameAliases.size();

			// add all aliases into library dict
			_const_foreach(gvector<gstring>,vtidTypeSource.vtNameAliases,j)
			{
				MapVTID::iterator it = m_mapVTID.find(*j);
				if(it == m_mapVTID.end())
				{
					m_mapVTID[*j] = ptVTID;
				}
				else
				{
					ptVTID->m_iAliasCount--;
					GSYS_DEBUG_OUTPUT(((string)
						"VTID ERROR: Invalid Alias <"+j->c_str()+"> of type <"+
						vtidTypeSource.czTypeName+"> duplicates alias of <"+
						it->second->czTypeName+ "> \n"						
						).c_str());
					break;
				}
			}
		}
		else
		{
			GSYS_DEBUG_OUTPUT(((string)
				"VTID ERROR: Type duplicated <"+vtidTypeSource.czTypeName+">\n"	
				).c_str());
			//vtidTypeSource.bBad = true;
		}
	}

	// second pass: init typed contained objects
	_const_foreach(VectorVTIDSource,vtSources,i)
	{
		vtid_t* ptVTID = (vtid_t*)get_by_name(i->czTypeName);
		if(!ptVTID)continue;

		// process parent declaration
		if(i->czParentTypeName)
		{
			ptVTID->iParentOffset = i->iParentOffset;
			ptVTID->ParentType = get_by_name(i->czParentTypeName);
		}
		else
		{
			ptVTID->iParentOffset = 0;
			ptVTID->ParentType = 0;
		}
		// process cast declarations
		_const_foreach(vtid_source::VecStaticCast,i->vtStaticCast,it)
		{
			vtid_typeinfo tp = get_by_name(it->first);
			ptVTID->m_mapCast[tp] = it->second;
		}

		if(i->czParentTypeName)
		{
			ptVTID->iParentOffset = i->iParentOffset;
			ptVTID->ParentType = get_by_name(i->czParentTypeName);
		}
		else
		{
			ptVTID->iParentOffset = 0;
			ptVTID->ParentType = 0;
		}

		// process all sub declarations
		_const_foreach(gvector<detail::vtid_decl_source>,i->vtDataMembers,j)
		{
			gsys::detail::vtid_decl_source const& src = *j;
			vtid_decl decl = src.decl;

			vtid_typeinfo pvt = get_by_name(src.czTypeName);
			if(!pvt)
			{
				GSYS_DEBUG_OUTPUT(((string)
					"VTID ERROR: Type used but not registered <"+src.czTypeName+">\n").c_str());
				continue;
			}
			GSYS_ASSERTOBJ(pvt);
			decl.vtidType = pvt;
			// process params of declaration
			_const_foreach(gvector<gsys::gstring>,j->vecProcParamsNames,k)
			{
				pvt = get_by_name(k->c_str());
				if(!pvt)
				{
					GSYS_DEBUG_OUTPUT(((string)
						"VTID ERROR: Type used but not registered <"+k->c_str()+">\n").c_str());
					continue;
				}
				GSYS_ASSERTOBJ(pvt);
				decl.vecProcParams.push_back(pvt);
			}
			if(!pvt) continue;

			// Test for legal of the declaration
			vtid_t::DeclMap::iterator it = ptVTID->m_mapDecl.find(j->strName);
			if(it != ptVTID->m_mapDecl.end())
			{
				if(!it->second.h_class_proc || !it->second.h_class_proc)
				{
					GSYS_DEBUG_OUTPUT(((string)
						"VTID ERROR: Type <"+i->czTypeName+"> declaration duplicated ("+j->strName.c_str()+") \n"
						).c_str());
					continue;
				}
				it->second.strName = src.strName;
				decl.strName = src.strName;
			}
			ptVTID->m_mapDecl.insert(vtid_t::DeclMap::value_type(src.strName,decl));
		}
	}
	return true;
}

