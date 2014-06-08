#pragma once
#ifndef G_VTID_VAR_H
#define G_VTID_VAR_H

#include "g_gsys.h"

#include "g_vtid_lib.h"

namespace gsys
{
	template<typename T>
		T* vtid_new(vtid_typeinfo id)
	{
		GSYS_ASSERTOBJ(id);
		if(!id) return false;
		vtid_typeinfo tt = GSYS_DETAIL_VTID_LIB->get<T>();
		T* pt2;

		if(tt != id)
		{
			if(!id->can_cast_to(tt))return 0;
			void* pt = id->create_new();
			if(!pt) return 0;
			pt2 = (T*)id->cast_to(pt,tt);
			if(!pt) return 0;
			if(!pt2)
			{
				id->delete_object(pt);
				return 0;
			}		
		}
		else
		{
			pt2 = (T*)id->create_new();
		}
        return pt2;
	}

	template<typename T>
		T* vtid_new(char const * type_name)
	{
		vtid_typeinfo id = GSYS_DETAIL_VTID_LIB->get_by_name(type_name);
		if(!id) return 0;
        return vtid_new<T>(id);
	}

	template<typename T>
		T* vtid_new()
	{
		vtid_typeinfo id = GSYS_DETAIL_VTID_LIB->get<T>();
		if(!id) return 0;
		return vtid_new<T>(id);
	}


	namespace detail
	{

		class vtid_variable
		{
			friend class vtid_member_ref;
			friend class vtid_var;
			friend class vtid_ref;
			friend class vtid_refvar;

		protected:
			vtid_typeinfo m_vtid;
			void* m_ptr;

			vtid_variable(vtid_typeinfo l_vtid,void* l_ptr):m_vtid(l_vtid),m_ptr(l_ptr){}

		protected:
			// operations
			vtid_variable():m_ptr(0),m_vtid(0){};

		public:

			vtid_result get_decl_ref(vtid_acess_data const *dta,vtid_member_ref* mref)const;

			gsys::vtid_result get_decl_ref(
				::gsys::vtid_access_mode mode, // in: access mode
				 string const& name,  // in: acces name
				 string const* param, // in: acces param
				 string const* type,  // in: acces type mod (could differ from default)
				 string const* value,  // in: acces type mod (could differ from default)
				 vtid_member_ref* ref // out: out valid reference
				 )const;  

			gsys::vtid_result get_decl_ref(
				::gsys::vtid_access_mode mode, // in: access mode
				 gstring const& name,  // in: acces name
				 gstring const* param, // in: acces param
				 gstring const* type,  // in: acces type mod (could differ from default)
				 gstring const* value,  // in: acces type mod (could differ from default)
				 vtid_member_ref* ref // out: out valid reference
				 )const;  

			gsys::vtid_result get_decl_ref(
				::gsys::vtid_access_mode mode, // in: access mode
				string const& name,  // in: acces name
				vtid_member_ref* ref // out: out valid reference
				)const;  

			vtid_member_ref get_prop(string const& name,::gsys::vtid_access_mode mode)const;
			vtid_member_ref get_prop(string const& name,::gsys::vtid_access_mode mode,string const& type,string const& param)const;
	
			vtid_ref operator[](const char* name);
			vtid_ref operator[](string const& name);
			vtid_ref operator[](gstring const& name);

			//vtid_member_ref get_prop(string const& name,::gsys::vtid_access_mode mode)const;

			void receive_ref_info(vtid_ref_info_receiver& pRes);


			// return true if variable represents valid object
			bool valid()const{return m_ptr != 0 && m_vtid != 0;}
			operator bool()const{return valid();}

			vtid_typeinfo vtid_t()const{return m_vtid;} 
			vtid_typeinfo typeinfo()const{return m_vtid;} 
			void* ptr()const{return m_ptr;} 

			// get type of representing object
			std::type_info const& type(void)const
			{
				if(m_vtid)return m_vtid->type();
				else return typeid(void);
			}

			// Get typed value  slow typecheck
			template <typename T>
			bool get_tval(T& val)const
			{
				GSYS_ASSERTOBJ(m_vtid);
				GSYS_ASSERTPTR(m_ptr);
				T* pt = vtid_cast<T>();
				if(pt)
				{
					val = *pt;
					return true;
				}
				std::string str;
				if(get_value(str))
				{
					val = lex_cast<T>(str);
					return true;
				}		
				return false;
			}

			template <typename T>
				bool set_tval(T& val)const
			{
				return set_value(vtid_make_ref(val));
			}

			std::string str()const
			{
				gstring str;
				get_value(str);
				return str;
			}

			bool get_value(gstring& str)const
			{
				GSYS_ASSERTOBJ(m_vtid);
				GSYS_ASSERTPTR(m_ptr);

				if(m_vtid->BaseOp.h_to_str)
				{
					return m_vtid->BaseOp.h_to_str(m_ptr,str);
				}
				return false;
			}

			bool set_value(const gstring& str)const
			{
				GSYS_ASSERTOBJ(m_vtid);
				GSYS_ASSERTPTR(m_ptr);

				if(m_vtid->BaseOp.h_from_str)
				{
					return m_vtid->BaseOp.h_from_str(m_ptr,str);
				}
				return false;
			}
			bool set_value(const char* str)const{return set_value(string(str));}	

			bool set_filename(const wchar_t* base,const wchar_t* str)const
			{
				GSYS_ASSERTOBJ(m_vtid);
				GSYS_ASSERTPTR(m_ptr);

				if(m_vtid->BaseOp.h_from_filename)
				{
					return m_vtid->BaseOp.h_from_filename(m_ptr,base,str);
				}
				return false;
			}

			bool set_value(const vtid_variable& var)const;

			int compare(const vtid_variable& var)const
			{GSYS_CFN;
				GSYS_ASSERTOBJ(m_vtid);
				GSYS_ASSERTPTR(m_ptr);

				GSYS_ASSERTREF(var);
				if(var.m_vtid == m_vtid)
				{
					GSYS_ASSERTPTR(var.m_ptr);
					if(m_vtid->BaseOp.h_compare_obj)
					{
						return m_vtid->BaseOp.h_compare_obj(m_ptr,var.m_ptr);
					}
				}
				return 0;
			}

			bool serialize(gsys::binostream &os)const
			{
				if(m_vtid->BaseOp.h_serial_save)
				{
					m_vtid->BaseOp.h_serial_save(m_ptr,os);
					return true;
				}
				return false;
			}

			bool serialize(gsys::binistream &is)const
			{
				if(m_vtid->BaseOp.h_serial_init)
				{
					m_vtid->BaseOp.h_serial_init(m_ptr,is);
					return true;
				}
				return false;
			}

			bool delete_object()
			{
				bool r = m_vtid->delete_object(m_ptr);
				if(r)m_ptr=0;
				return r;
			}


			//////////////////////////////////////////////////////////////////////
			// typed data access
			template<typename T>
				T* vtid_cast()const
			{
				return reinterpret_cast<T*>(m_vtid->cast_to(m_ptr,vtid_get_typeinfo<T>()));
			}

			template<typename T>
            T* ptr_type_cast()const
			{
				if(typeid(T) == m_vtid->type())
				{
					return reinterpret_cast<T*>(m_ptr);
				}
				return 0;
			}

			template<typename T>
				T* ptr_reinterpret_cast()const
			{
				return reinterpret_cast<T*>(m_ptr);
				return 0;
			}
           
		};
	}

	class vtid_ref : public detail::vtid_variable
	{
		friend class detail::vtid_variable;
		friend class vtid_var;
		friend class vtid_refvar;
		friend class vtid_member_ref;
	protected:

	public:
		vtid_ref(vtid_typeinfo l_vtid,void* l_ptr): detail::vtid_variable(l_vtid,l_ptr){}
		vtid_ref(){}
		void set(void* pt,vtid_typeinfo tp){m_vtid=tp;m_ptr = pt;}
		template<typename T>
		vtid_ref& init(T const& ref)
		{
			vtid_typeinfo vt = GSYS_DETAIL_VTID_LIB->get<T>();
			if(vt)
			{
				m_vtid = vt;
				m_ptr = const_cast<void*>((void const*)&ref);
			}
			return *this;
		}

		template<typename T>
		vtid_ref& static_init(T const& ref)
		{
			vtid_typeinfo vt = gsys::detail::vtid_type_holder<T>::type;
			if(!vt)vt = GSYS_DETAIL_VTID_LIB->get<T>();
			if(vt)
			{
				m_vtid = vt;
				m_ptr = const_cast<void*>((void const*)&ref);
			}
			return *this;
		}

		template<>	vtid_ref& init<vtid_ref>(vtid_ref const& ref)
		{
			if(!ref.valid()) release();
			else
			{
				m_vtid = ref.m_vtid;
				m_ptr  = ref.m_ptr;
			}
			return *this;
		}

		void release()
		{
			m_ptr = 0;
			m_vtid = 0;
		}

		vtid_ref operator[](const char* name){return gsys::detail::vtid_variable::operator[](name);}
		vtid_ref operator[](string const& name){return gsys::detail::vtid_variable::operator[](name);}

		bool operator == (vtid_ref const& ref)const{return m_ptr == ref.m_ptr && m_vtid == ref.m_vtid;}
		bool operator < (vtid_ref const& ref)const{return m_ptr == ref.m_ptr?m_vtid < ref.m_vtid : m_ptr < ref.m_ptr;}
	};

	template<typename T>
	vtid_ref vtid_make_ref(T& ref)
	{
		vtid_ref vr;
		vr.init<T>(ref);
		return vr;
	}

	class vtid_member_ref
	{
		friend class detail::vtid_variable;
		friend class vtid_ref;
		friend class vtid_var;
		friend class vtid_refvar;
	protected:
		// used class object reference
		vtid_ref m_refObject;
		// used to identify member declaration if exists
		detail::vtid_decl const* m_pMemberDecl;
		// reference to member (if any)
		vtid_ref m_refMember;
	public:
		vtid_member_ref():m_pMemberDecl(0){}
		
		// determine if that decl represents member function
		bool is_func(){return !m_refMember.valid();}
		
		// determine if that decl represents W property
		bool is_prop_w(){return (m_pMemberDecl) && m_pMemberDecl->h_class_proc && m_refMember.valid();}

		bool test_access(vtid_access_mode mode)const
		{
			if(!m_pMemberDecl)return true;
			return (m_pMemberDecl->wAccessMask & mode)!=0;
		}

		vtid_ref const & ref(){return m_refMember;}

		vtid_ref get_ref(vtid_access_mode mode)
		{
			if(test_access(mode))return m_refMember;
			else return vtid_ref();
		}

		bool valid()const{return m_refMember.valid() || (m_pMemberDecl && m_refObject.valid());}

		operator bool()const{return valid();}

		vtid_member_ref find_overloaded_fn(vtid_ref const*  vars,int num);

		vtid_typeinfo fn_return_type()const
		{
			if(!m_pMemberDecl) return 0;
			GSYS_ASSERTOBJ(m_pMemberDecl);
			return m_pMemberDecl->vtidType;
		}

		int fn_num_formal_params()const
		{
			if(!m_pMemberDecl)return 0;
			GSYS_ASSERTOBJ(m_pMemberDecl);
			return (int)m_pMemberDecl->vecProcParams.size();
		}

		vtid_typeinfo fn_formal_param_type(int n)const
		{
			if(!m_pMemberDecl)return 0;
			GSYS_ASSERTOBJ(m_pMemberDecl);
			return m_pMemberDecl->vecProcParams[n];
		}

		vtid_typeinfo fn_param_type(uint n)const
		{
			if(!m_pMemberDecl)return 0;
			GSYS_ASSERTOBJ(m_pMemberDecl);
			if(n >= m_pMemberDecl->vecProcParams.size())return 0; 
			return m_pMemberDecl->vecProcParams[n];
		}

		vtid_ref call(detail::vtid_variable const&  var)
		{
			vtid_ref r;
			GSYS_ASSERTOBJ(m_pMemberDecl);
			if(m_pMemberDecl->h_class_proc)
			{
				m_pMemberDecl->h_class_proc(m_refObject.m_ptr,1,(vtid_ref*)&var,&r);
			}
			return r;
		}
		vtid_ref call(vtid_ref const*  vars,int num);
		vtid_ref call();

		const char* description()
		{
			GSYS_ASSERTOBJ(m_pMemberDecl);
			return m_pMemberDecl->strDescription.c_str();
		}	
	};

	struct vtid_ref_info
	{
		vtid_ref ref;
		gstring strName;
		gstring strType;
		gstring strParam;
		gstring strValue;
		gstring strDesc;
	};

	class vtid_var : public detail::vtid_variable
	{
	public:
		vtid_var(){}
		vtid_var(vtid_typeinfo id){create(id);}
		~vtid_var()
		{	
			release();
		}
		operator vtid_ref () {return *reinterpret_cast<vtid_ref*>(this);}
		vtid_ref as_ref(){return *reinterpret_cast<vtid_ref*>(this);}

		vtid_var(detail::vtid_variable const& ref){clone(ref);}
		vtid_var(vtid_var const& ref){clone(ref);}
		vtid_var(vtid_ref const& ref){clone(ref);}

		vtid_var& operator = (detail::vtid_variable const& ref){return clone(ref);}
		vtid_var& operator = (vtid_var const& ref){return clone(ref);}
		vtid_var& operator = (vtid_ref const& ref){return clone(ref);}

		vtid_var& clone(detail::vtid_variable const& ref)
		{
			if(m_vtid == ref.m_vtid)
			{
				if(m_vtid)set_value(ref);
			}
			else
			{
				release();
				m_vtid = ref.m_vtid;
				if(m_vtid && m_vtid->copyable())
				{
					create(ref.m_vtid);
					set_value(ref);
				}
			}
			return *this;
		}

		bool create(vtid_typeinfo id)
		{
			GSYS_ASSERTOBJ(id);
			if(!id) return false;
			GSYS_ASSERTOBJ(id);
			if(!id->ClassFactory) return false;
			GSYS_ASSERT_CODE(id->ClassFactory);
			void* pt = id->ClassFactory(0);
			if(!pt) return false;
			GSYS_ASSERTPTR(id->ClassFactory);
			
			// all sucessfun init it
            m_vtid = id;
			m_ptr = pt;
            return true;            
		}

		template<typename T>
		void setType()
		{
			vtid_typeinfo id = GSYS_DETAIL_VTID_LIB->get<T>();
			if(id != m_vtid)release();
			m_vtid = id;
		}

		bool create(type_info const& inf){return create(vtid_raw_type_name(inf));}

		template<typename T>
		bool create()
		{
			vtid_typeinfo id = GSYS_DETAIL_VTID_LIB->get<T>();
			if(!id) return false;
			return create(id);
		}

		bool create(char const* name)
		{
			vtid_typeinfo id = GSYS_DETAIL_VTID_LIB->get_by_name(name);
			if(!id) return false;
			return create(id);
		}

		void release()
		{
			if(m_ptr) // release object
			{
				GSYS_ASSERTOBJ(m_vtid);
				GSYS_ASSERT(m_vtid->ClassFactory);
				// destroy object
				m_vtid->ClassFactory(m_ptr);
				m_ptr = 0;
			}
			m_vtid = 0;
		}
	};

	class vtid_refvar : public detail::vtid_variable
	{
		bool m_bOwnHandle;
	public:
	public:
		vtid_refvar():m_bOwnHandle(false){}
		vtid_refvar(vtid_typeinfo id){create(id);}
		~vtid_refvar()
		{	
			release();
		}

		vtid_refvar(vtid_refvar const& ref)
		{
			if(ref.m_bOwnHandle)make_copy(ref);
			else make_ref(ref);
		}

		vtid_refvar& clone(vtid_refvar const& ref)
		{
			release();
			m_vtid = ref.m_vtid;
			if(m_vtid->copyable())
			{
				create(ref.m_vtid);
				set_value(ref);
			}
			return *this;
		}
		vtid_refvar& make_copy(vtid_refvar const& ref){return clone(ref);}

		vtid_refvar& make_ref(vtid_refvar const& ref)
		{
			release();
			m_ptr = ref.m_ptr;
			m_vtid = ref.m_vtid;
			return *this;
		}

		/*
		 *	vtid_ref convertions
		 */
		vtid_refvar(vtid_ref const& ref)
		{
			m_ptr = ref.m_ptr;
			m_vtid = ref.m_vtid;
			m_bOwnHandle=false;
		}
		vtid_refvar& operator = (vtid_ref const& ref)
		{
			m_ptr = ref.m_ptr;
			m_vtid = ref.m_vtid;
			m_bOwnHandle=false;
			return *this;
		}
		operator vtid_ref ()const 
		{
			return vtid_ref(m_vtid,m_ptr);
		}

		bool create(vtid_typeinfo id)
		{
			release();
			GSYS_ASSERTOBJ(id);
			m_ptr = id->create_new();
			if(m_ptr)
			{
				m_vtid = id;
				m_bOwnHandle=true;
				return true;            
			}
			return false;
		}

		//bool create(type_info const& inf){return create(vtid_raw_type_name(inf));}

		template<typename T>
			bool create()
		{
			vtid_typeinfo id = GSYS_DETAIL_VTID_LIB->get<T>();
			if(!id) return false;
			return create(id);
		}

		bool create(char const* name)
		{
			vtid_typeinfo id = GSYS_DETAIL_VTID_LIB->get_by_name(name);
			if(!id) return false;
			return create(id);
		}

		void release()
		{
			if(m_bOwnHandle)
			{
				if(m_ptr) // release object
				{
					GSYS_ASSERTOBJ(m_vtid);
					GSYS_ASSERT(m_vtid->ClassFactory);
					// destroy object
					m_vtid->ClassFactory(m_ptr);
				}
			}
			m_vtid = 0;
			m_bOwnHandle=false;
		}
	};
}

inline gsys::vtid_ref gsys::detail::vtid_variable::operator[](const char* name)
{
	vtid_member_ref ref;
	get_decl_ref(vtid_access_modify,gsys::gstring(name),0,0,0,&ref);
	return ref.ref();
}

inline gsys::vtid_ref gsys::detail::vtid_variable::operator[](std::string const& name)
{
	vtid_member_ref ref;
	get_decl_ref(vtid_access_modify,gsys::gstring(name),0,0,0,&ref);
	return ref.ref();
}

inline gsys::vtid_ref gsys::detail::vtid_variable::operator[](gsys::gstring const& name)
{
	vtid_member_ref ref;
	get_decl_ref(vtid_access_modify,name,0,0,0,&ref);
	return ref.ref();
}


inline gsys::vtid_member_ref gsys::detail::vtid_variable::get_prop(std::string const& name,
																   ::gsys::vtid_access_mode mode)const
{
	vtid_member_ref ref;
	get_decl_ref(mode,gsys::gstring(name),0,0,0,&ref);
	return ref;
}

inline gsys::vtid_member_ref gsys::detail::vtid_variable::get_prop(std::string const& name,
																   ::gsys::vtid_access_mode mode,
																   std::string const& type,
																   std::string const& param)const
{
	vtid_member_ref ref;
	get_decl_ref(mode,gsys::gstring(name),&param,&type,0,&ref);
	return ref;
}


#endif
