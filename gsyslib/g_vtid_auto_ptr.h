#pragma once
#ifndef G_VTID_AUTO_PTR__H
#define G_VTID_AUTO_PTR__H

#include "g_gsys.h"
#include "g_vtid_var.h"

namespace gsys
{
	template <class T>
	class vtid_auto_ptr
	{
	public:
		typedef T* pointer; 
		typedef T& reference; 
	private:
		pointer m_ptr;
		vtid_typeinfo m_objtype;
	public:

		void reset(T* pRef=0,vtid_typeinfo otype=0)
		{
			if(m_ptr)
			{
				vtid_typeinfo ptrtype = get_vtid_lib()->get<T>();
				if(!ptrtype)return;
				void* ptr = m_objtype->cast_from(m_ptr,ptrtype);
				m_objtype->delete_object(ptr);
				m_objtype=0;
			}	
			m_ptr = pRef;
			if(m_ptr)
			{
				if(otype) m_objtype = otype;
				else 
				{
					m_objtype = gsys::vtid_get_typeinfo(*m_ptr);
				}
			}
		}

		vtid_auto_ptr():m_ptr(0),m_objtype(0){}
		~vtid_auto_ptr()
		{
			reset();
		}
		bool serialize(vtid_ref_info& rInf)
		{
			if(!m_ptr)
			{
				rInf.ref.set(m_ptr,m_objtype);
				return false;
			}
 			vtid_typeinfo ptrtype = get_vtid_lib()->get<T>();
			void* ptr = m_objtype->cast_from(m_ptr,ptrtype);
			rInf.ref.set(ptr,m_objtype);
			rInf.strType = m_objtype->type_name();
			return true;
		}
		vtid_typeinfo type(){return m_objtype;} 

		vtid_result ref(vtid_acess_data const  *dta,vtid_ref* ref)
		{
			if(dta->acc_mode == vtid_access_modify)
			{
				if(!m_ptr) return vtid_err_no_object;
				ref->set(m_ptr,m_objtype);
			}
			else
			{
				m_objtype = dta->type;
				if(!m_objtype) return vtid_err_invalid_type;
				if(!m_objtype->creatable())return vtid_err_invalid_type;

				void* ptr = m_objtype->create_new();
				if(!ptr)return vtid_err_internal_error;

				vtid_typeinfo ptrtype = get_vtid_lib()->get<T>();
				m_ptr = reinterpret_cast<pointer>(m_objtype->cast_to(ptr,ptrtype));

				if(!m_ptr) return vtid_err_type_mismatch;
				ref->set(ptr,dta->type);
			}
			return vtid_ok;
		}
		pointer operator -> (){return m_ptr;}
		pointer get(){return m_ptr;}
		reference operator * (){return *m_ptr;}
		operator bool(){return m_ptr != 0;}
	};
}

#endif