#pragma once
#ifndef G_VTID_LIB_H
#define G_VTID_LIB_H


#include "g_gsys.h"
#include "g_hash_map.h"


#include "g_vtid.h"

#define GSYS_DETAIL_VTID_LIB gsys::g_LibVTID

#include "detail/g_vtid_macro.h"
#include "g_vtid_const.h"

namespace gsys
{
	class vtid_lib;
	extern vtid_lib*  g_LibVTID;
	namespace detail
	{
		void register_strings();
	}
};

namespace gsys
{
	//vtid_lib* get_vtid_lib();

	class vtid_lib : noncopyable
	{
		vtid_lib* m_pParent;
		vtid_const m_consts;
		virtual bool _GetConst(char const* name,vtid_const::value_type& val);
	public:
		vtid_const& Constants()
		{
			if(m_pParent)return m_pParent->Constants();
			return m_consts;
		}
		template<class T>
		bool GetConst(char const* name,T& val)
		{
			vtid_const::value_type v;
			if(_GetConst(name,v))
			{
				val = (T)v;
				return true;
			}
			return false;
		}
		template<> bool GetConst(char const* name,vtid_const::value_type& val){return _GetConst(name,val);}

		void static register_foundamental();
		void static register_geometry();
		void static register_color();
	private:
		void static register_base_types();
		void static register_helpers();

		friend vtid_lib* get_vtid_lib();

//		static vtid_lib* m_Lib;

#ifdef GSYS_DETAIL_HAS_STDEXT_HASH_MAP
		typedef stdext::hash_map<std::string,vtid_t*> MapVTID;
#else
		typedef std::map<std::string,vtid_t*> MapVTID;
#endif
		typedef gvector<detail::vtid_source> VectorVTIDSource;

		MapVTID m_mapVTID;

		VectorVTIDSource m_vtSources;
	
		bool register_source(VectorVTIDSource const& vtSources);

		virtual bool GSYS_API add_extention(vtid_lib * pLib);

		virtual VectorVTIDSource* GSYS_API get_source(){return &m_vtSources;}

		virtual vtid_typeinfo GSYS_API get_vtid(char const * szType) const
		{
			MapVTID::const_iterator i = m_mapVTID.find(szType);
			if(i != m_mapVTID.end())
			{
				return i->second;
			}
			return 0;
		}


	public:
		vtid_lib()
		{
			// register base types first
			//register_base_types();
			m_pParent=0;
		}
		virtual ~vtid_lib()
		{
			clear();
		}

	public:
		gsys_handle handle(){return reinterpret_cast<gsys_handle>(this);}
		bool init();
		bool init_as_extention(gsys_handle hParent);
		bool init_as_extention(gsys::vtid_lib* pParent);
		void free();

		void clear()
		{
			_foreach(MapVTID,m_mapVTID,i)
			{
				if(i->second->m_iAliasCount == 0)
					delete i->second;
				else
					i->second->m_iAliasCount--;
			}
			m_mapVTID.clear();
		}

		bool register_type(detail::vtid_source const &vtidTypeSource)
		{
			m_vtSources.push_back(vtidTypeSource);
            return true;
		}

		vtid_typeinfo get_by_name(char const * szType) const
		{
			if(m_pParent) return m_pParent->get_vtid(szType);
			return get_vtid(szType);
		}

		template<typename T>
		vtid_typeinfo get()
		{
			return get_by_name(GSYS_DETAIL_TYPENAME(T));
		}
	public: // some helper functions
		template<class C> 
		void find_types_implementation(vtid_typeinfo type, C& res)
		{
			_foreach(MapVTID,m_mapVTID,it)
			{
				vtid_t* vt = it->second;
				if(vt->can_cast_to(type) && vt->creatable())
				{
					if(vt->m_iAliasCount > 0 && std::find(res.begin(),res.end(),vt) != res.end())
					{
						continue;
					}
					res.push_back(vt);
				}
			}
		}
	};
	inline vtid_lib* get_vtid_lib()
	{
		if(GSYS_DETAIL_VTID_LIB == 0)GSYS_DETAIL_VTID_LIB = new vtid_lib;
		return GSYS_DETAIL_VTID_LIB;
	}

	template<class T> 
	vtid_typeinfo vtid_get_typeinfo()
	{
		return GSYS_DETAIL_VTID_LIB->get<T>();
	}

	template<class T> 
	vtid_typeinfo vtid_get_typeinfo(T const& t)
	{
		return GSYS_DETAIL_VTID_LIB->get<T>();
	}

	template<class T> 
	vtid_typeinfo vtid_get_typeinfo_static()
	{
		return gsys::detail::vtid_type_holder<T>::type;
	}

	template<class T> 
	vtid_typeinfo vtid_get_typeinfo_static(T const& t)
	{
		return gsys::detail::vtid_type_holder<T>::type;
	}
}

#endif
