#pragma once
#ifndef G_VTID_MACRO_H
#define G_VTID_MACRO_H

#include "boost/preprocessor/cat.hpp"
#include "g_alg.h"


namespace gsys{ namespace detail{
	template<typename T>class __gsys_detail_vtid_areg{};
}}
//#define GSYS_VTID_FRIEND friend class ::gsys::detail::__gsys_detail_vtid_areg;

#define GSYS_VTID_FRIEND(x)\
	friend class ::gsys::detail::__gsys_detail_vtid_areg<x>;\
	friend class ::gsys::detail::vtid_op<x>;

namespace gsys{namespace detail{ 

	template<class T> 
	struct vtid_type_holder
	{
		vtid_type_holder():type(0){}
		static vtid_typeinfo type;
	};

	template<typename T>
	class vtid_hsb
	{
	protected:
		typedef T lType;
	};

	template<typename lType>
	class vtid_op
	{
	public:

		static void* GSYS_CALLBACK createble(void* pt)
		{
			if(pt){delete reinterpret_cast<lType*>(pt);return 0;}
			return reinterpret_cast<void*>(new lType);
		}

		static void* GSYS_CALLBACK destroyable(void* pt)
		{
			if(pt)delete reinterpret_cast<lType*>(pt);
			return 0;
		}

		static void* GSYS_CALLBACK createble1(void* pt)
		{
			if(pt){delete reinterpret_cast<lType*>(pt);return 0;}
			return reinterpret_cast<void*>(new lType);
		}

		static void GSYS_CALLBACK copyable(void* ptr,void* src)
		{
			(*reinterpret_cast<lType*>(ptr)) = (*reinterpret_cast<lType*>(src));
		}

		static bool GSYS_CALLBACK apply_object(void* ptr,void* src,vtid_t const* type)
		{
			return false;
			//(*reinterpret_cast<lType*>(ptr)) = (*reinterpret_cast<lType*>(src));
		}

		static bool GSYS_CALLBACK from_str(void* ptr,gstring const &str)
		{
			try{
				(*reinterpret_cast<lType*>(ptr)) = ::gsys::lex_cast<lType>(str);
			}
			catch(...){return false;}
			return true;
		} 

		static bool GSYS_CALLBACK from_wstr(void* ptr,gstring const &str)
		{
			try{
				(*reinterpret_cast<lType*>(ptr)) = ::gsys::lex_cast<lType>(wstring(detail::StrToWide(str.c_str(),CP_UTF8)));
			}
			catch(...){return false;}
			return true;
		} 

		static bool GSYS_CALLBACK numconst_from_str(void* ptr,gstring const &str)
		{
			if(char_classify::alpha(str[0]))
			{
				gsys::vtid_valmap::value_type val;
				if(!get_vtid_lib()->GetConst(str.c_str(),val))return false;
				(*reinterpret_cast<lType*>(ptr)) = (lType)(val);
			}
			else
			{
				try{
					(*reinterpret_cast<lType*>(ptr)) = ::gsys::lex_cast<lType>(str);
				}
				catch(...){return false;}
			}
			return true;
		} 

		static bool GSYS_CALLBACK intnumconst_from_str(void* ptr,gstring const &str)
		{
			if(char_classify::alpha(str[0]))
			{
				gsys::vtid_valmap::value_type val;
				if(!get_vtid_lib()->GetConst(str.c_str(),val))return false;
				(*reinterpret_cast<lType*>(ptr)) = (lType)int(val);
			}
			else
			{
				try{
					(*reinterpret_cast<lType*>(ptr)) =  (lType)(gsys::lex_cast<int>(str));
				}
				catch(...){return false;}
			}
			return true;
		}

		static bool GSYS_CALLBACK to_str(void* ptr,gstring &str)
		{
			try{
				str = ::gsys::lex_cast<std::string>(*reinterpret_cast<lType*>(ptr));
			}
			catch(...){return false;}
			return true;
		} 

		static bool GSYS_CALLBACK to_wstr(void* ptr,gstring &str)
		{
			try{
				str = detail::WideToStr(::gsys::lex_cast<std::wstring>(*reinterpret_cast<lType*>(ptr)).c_str(),CP_UTF8);
			}
			catch(...){return false;}
			return true;
		} 

		static int GSYS_CALLBACK comparable(void* ptr,void* src)
		{
			lType & l1 = (*reinterpret_cast<lType*>(ptr)); 
			lType & l2 = (*reinterpret_cast<lType*>(src));
			return (l1 < l2)?-1:((l1 == l2)?0:1);  	
		} 

		static int GSYS_CALLBACK equal_comparable(void* ptr,void* src)
		{
			lType & l1 = (*reinterpret_cast<lType*>(ptr)); 
			lType & l2 = (*reinterpret_cast<lType*>(src));
			return (l1 == l2)?0:2;  	
		} 

		static void GSYS_CALLBACK serial_save(void* ptr,gsys::binostream &os)
		{
			os << *reinterpret_cast<lType*>(ptr);
		}

		static void GSYS_CALLBACK serial_init(void* ptr,gsys::binistream &is)
		{
			is >> *reinterpret_cast<lType*>(ptr);
		}

		static void GSYS_CALLBACK raw_serial_save(void* ptr,gsys::binostream &os)
		{
			os.put_rawdata(*reinterpret_cast<lType*>(ptr));
		}

		static void GSYS_CALLBACK raw_serial_init(void* ptr,gsys::binistream &is)
		{
			is.get_rawdata(*reinterpret_cast<lType*>(ptr));
		}
	};

}}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//               Base operation support
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define GSYS_VTID_CREATEBLE {vtidSource.ClassFactory = gsys::detail::vtid_op<lType>::createble;}
#define GSYS_VTID_DESTROYABLE {vtidSource.ClassFactory = gsys::detail::vtid_op<lType>::destroyable;}

#define GSYS_VTID_COPYABLE {vtidSource.BaseOp.h_copy_obj = gsys::detail::vtid_op<lType>::copyable;}

#define GSYS_VTID_HANLE_FROM_STR {vtidSource.BaseOp.h_from_str = gsys::detail::vtid_op<lType>::from_str;}
#define GSYS_VTID_HANLE_TO_STR {vtidSource.BaseOp.h_to_str = gsys::detail::vtid_op<lType>::to_str;}

#define GSYS_VTID_HANLE_FROM_WSTR {vtidSource.BaseOp.h_from_str = gsys::detail::vtid_op<lType>::from_wstr;}
#define GSYS_VTID_HANLE_TO_WSTR {vtidSource.BaseOp.h_to_str = gsys::detail::vtid_op<lType>::to_wstr;}

#define GSYS_VTID_HANLE_CONST_FROM_STR {vtidSource.BaseOp.h_from_str = gsys::detail::vtid_op<lType>::numconst_from_str;}

#define GSYS_VTID_COMPARABLE {vtidSource.BaseOp.h_compare_obj = gsys::detail::vtid_op<lType>::comparable;}
#define GSYS_VTID_EQUAL_COMPARABLE {vtidSource.BaseOp.h_compare_obj = gsys::detail::vtid_op<lType>::equal_comparable;}

#define GSYS_VTID_HANLE_SERIAL_SAVE {vtidSource.BaseOp.h_serial_save = gsys::detail::vtid_op<lType>::serial_save;}
#define GSYS_VTID_HANLE_SERIAL_INIT {vtidSource.BaseOp.h_serial_init = gsys::detail::vtid_op<lType>::serial_init;}

#define GSYS_VTID_HANLE_RAW_SERIAL_SAVE {vtidSource.BaseOp.h_serial_save = gsys::detail::vtid_op<lType>::raw_serial_save;}
#define GSYS_VTID_HANLE_RAW_SERIAL_INIT {vtidSource.BaseOp.h_serial_init = gsys::detail::vtid_op<lType>::raw_serial_init;}

#define GSYS_VTID_SERIALIZEBLE  GSYS_VTID_HANLE_SERIAL_SAVE  GSYS_VTID_HANLE_SERIAL_INIT
#define GSYS_VTID_RAW_SERIALIZEBLE  GSYS_VTID_HANLE_RAW_SERIAL_SAVE  GSYS_VTID_HANLE_RAW_SERIAL_INIT

#define GSYS_VTID_WIDE_STRINGIZEBLE  GSYS_VTID_HANLE_TO_WSTR  GSYS_VTID_HANLE_FROM_WSTR

#define GSYS_VTID_STRINGIZEBLE  GSYS_VTID_HANLE_TO_STR  GSYS_VTID_HANLE_FROM_STR
#define GSYS_VTID_NUM_STRINGIZEBLE  GSYS_VTID_HANLE_TO_STR  GSYS_VTID_HANLE_CONST_FROM_STR

#define GSYS_DETAIL_TYPENAME(x) (::gsys::vtid_get_typename<x>())

#define GSYS_DETAIL_TYPENAME_VAR(x) (::gsys::vtid_get_typename_var(x))

#define GSYS_DETAIL_REDABLE_TYPENAME(x) typeid(x).name()

#define GSYS_DETAIL_VTID_HANLE_TYPE(x) { \
	struct handler_struct{ \
	static std::type_info const& GSYS_CALLBACK handler(void) \
	{ return typeid(x);}}; \
	vtidSource.TypeProc = handler_struct::handler; }


#define GSYS_DETAIL_VTID_BEGIN_DECLS(x) { \
			word wAccessMask = 0;\
			::gsys::gstring desc;\
			lType const * ptClass=reinterpret_cast<lType const*>(0x0f000000); \
			::gsys::detail::vtid_source vtidSource; \
			vtidSource.czTypeName = GSYS_DETAIL_TYPENAME(lType); \
			GSYS_DETAIL_VTID_HANLE_TYPE(x)\
			GSYS_VTID_PUBLIC


// Access mod
#define GSYS_VTID_PUBLIC\
			{\
				wAccessMask  = 0;\
				wAccessMask |= ::gsys::vtid_access_init;\
				wAccessMask |= ::gsys::vtid_access_read;\
				wAccessMask |= ::gsys::vtid_access_write;\
				wAccessMask |= ::gsys::vtid_access_modify;\
				wAccessMask |= ::gsys::vtid_access_serialize;\
			}

#define GSYS_VTID_READONLY\
				wAccessMask &= ~::gsys::vtid_access_write;

#define GSYS_VTID_PRIVATE wAccessMask  = vtid_access_serialize;

#define GSYS_VTID_PROTECTED wAccessMask  = ::gsys::vtid_access_init|vtid_access_serialize;

#define GSYS_VTID_ALLOW_SERIALIZE(v)\
	{wAccessMask = (wAccessMask & ~::gsys::vtid_access_serialize)\
	|| (::gsys::vtid_access_serialize & v);}


#define GSYS_DETAIL_VTID_BEGIN_CL(x) \
	class __gsys_detail_vtid_areg\
	{typedef x lType;public: __gsys_detail_vtid_areg() \
		GSYS_DETAIL_VTID_BEGIN_DECLS(x)


#define GSYS_DETAIL_VTID_BEGIN_TEMPLATE(x)\
	vtid_typeinfo vtid_type_holder<x>::type;\
	template<>\
	class __gsys_detail_vtid_areg<x>\
		{typedef x lType;public:__gsys_detail_vtid_areg() \
		GSYS_DETAIL_VTID_BEGIN_DECLS(x)\
		vtidSource.pTypeHolder = &vtid_type_holder<x>::type;
		
#define GSYS_DETAIL_VTID_END {gsys::get_vtid_lib()->register_type(vtidSource);}}


#define GSYS_VTID_BEGIN(x) namespace gsys {namespace detail{ GSYS_DETAIL_VTID_BEGIN_TEMPLATE(x)

#ifdef GSYS_DETAIL_HAVE__COUNTER__


#define GSYS_VTID_END(x) GSYS_DETAIL_VTID_END};\
			static __gsys_detail_vtid_areg<x> \
			BOOST_PP_CAT(__gsys_detail_vtid_areg_placer,__COUNTER__ );}}\


/*#define GSYS_VTID_END(x) GSYS_DETAIL_VTID_END} \
		static BOOST_PP_CAT(__gsys_detail_vtid_areg_placer,__COUNTER__ );}}\
*/

#else

#define GSYS_VTID_END(x) GSYS_DETAIL_VTID_END};\
	static __gsys_detail_vtid_areg<x> \
	BOOST_PP_CAT(__gsys_detail_vtid_areg_placer,__LINE__ );}}\

#endif

#define GSYS_VTID_BEGIN_DYNAMIC(x) {GSYS_DETAIL_VTID_BEGIN_CL(x)

#define GSYS_VTID_END_DYNAMIC GSYS_DETAIL_VTID_END}\
	__gsys_detail_vtid_areg_placer_dyn;}

#define GSYS_VTID_ALIAS(x) {vtidSource.vtNameAliases.push_back(x);}
#define GSYS_VTID_TYPENAME(x) {vtidSource.vtNameAliases.push_back(vtidSource.czTypeName);vtidSource.czTypeName=x;}

#define GSYS_VTID_CREATEBLE0(T) { \
	struct handler_struct{ \
	static void* GSYS_CALLBACK handler(void* pt){ \
		if(pt){delete reinterpret_cast<T*>(pt);return 0;} \
		return reinterpret_cast<void*>(new T);}}; \
	vtidSource.ClassFactory = handler_struct::handler; }


#define GSYS_VTID_CREATEBLE1(T,x) { \
	struct handler_struct{ \
	static void* GSYS_CALLBACK handler(void* pt){ \
		if(pt){delete reinterpret_cast<T*>(pt);return 0;} \
		return reinterpret_cast<void*>(new T(x));}}; \
	vtidSource.ClassFactory = handler_struct::handler; }

#define GSYS_VTID_CREATEBLE2(T,x,y) { \
	struct handler_struct{ \
	static void* GSYS_CALLBACK handler(void* pt){ \
		if(pt){delete reinterpret_cast<T*>(pt);return 0;} \
		return reinterpret_cast<void*>(new T(x,y));} \
	}; vtidSource.ClassFactory = handler_struct::handler; }


#define GSYS_VTID_FROM_STR_FUNC(x) { \
	struct handler_struct{ \
	static bool GSYS_CALLBACK handler(void* ptr,gstring const &str) \
	{return reinterpret_cast<lType*>(ptr)->x(str);}}; \
	vtidSource.BaseOp.h_from_str = handler_struct::handler; }

#define GSYS_VTID_FROM_FILENAME_FUNC(x) { \
struct handler_struct{ \
	static bool GSYS_CALLBACK handler(void* ptr,wchar_t const* base,wchar_t const* str) \
	{return reinterpret_cast<lType*>(ptr)->x(base,str);}}; \
	vtidSource.BaseOp.h_from_filename = handler_struct::handler; }


#define GSYS_VTID_TO_STR_PCHAR_FUNC(x) { \
struct handler_struct{ \
	static bool GSYS_CALLBACK handler(void* ptr,gstring &str) \
	{ char const* pc = reinterpret_cast<lType*>(ptr)->x();\
	if(pc){str = pc;return true;}else return false;}}; \
	vtidSource.BaseOp.h_to_str = handler_struct::handler; }

#define GSYS_VTID_APPLY_FUNC(x) { \
	struct handler_struct{ \
	static bool GSYS_CALLBACK handler(void* ptr,void* src,vtid_t const* type) \
	{::gsys::vtid_ref ref(type,src);\
	return reinterpret_cast<lType*>(ptr)->x(ref);}};\
	vtidSource.BaseOp.h_apply_obj = handler_struct::handler; }


#define GSYS_VTID_USE_CLASS_ACCESS_PROC(x){\
	struct handler_struct : public vtid_hsb<lType>{\
	static ::gsys::vtid_result GSYS_CALLBACK handler(::gsys::vtid_acess_data const  *dta,::gsys::vtid_ref* ref)\
	{lType* tp =  (reinterpret_cast<lType*>(dta->ptr));\
		return tp->x(dta,ref); \
	}};\
	vtidSource.AccessProc = handler_struct::handler; }

#define GSYS_VTID_USE_CLASS_EXPORT_PROC(x){\
struct handler_struct{\
	static void GSYS_CALLBACK handler(void* classptr,::gsys::vtid_ref_info_receiver& pRes)\
	{lType* tp =  (reinterpret_cast<lType*>(classptr));\
	tp->x(pRes); \
	}};\
	vtidSource.ExportProc = handler_struct::handler; }


#define GSYS_DETAIL_VTID_DECL_BASE(t,n) \
	vtidSource.vtDataMembers.resize(vtidSource.vtDataMembers.size()+1); \
	::gsys::detail::vtid_decl_source& dcl_src = vtidSource.vtDataMembers.back(); \
	::gsys::detail::vtid_decl& dcl = dcl_src.decl;\
	dcl_src.czTypeName = t; \
	dcl_src.strName = n; \
	dcl.wAccessMask = wAccessMask;\
	::gsys::swap(dcl.strDescription,desc);

#define GSYS_DETAIL_VTID_SET_DECL_OFFSET(var)\
	dcl.uiOffset = (gtypes::uint)(reinterpret_cast<char const*>(&(ptClass->var))-reinterpret_cast<char const*>(ptClass));

#define GSYS_VTID_DECL(var,name)\
        { GSYS_DETAIL_VTID_DECL_BASE(GSYS_DETAIL_TYPENAME_VAR(ptClass->var),name)\
		GSYS_DETAIL_VTID_SET_DECL_OFFSET(var)}

#define GSYS_VTID_DECL_TYPED(var,type,name){ GSYS_DETAIL_VTID_DECL_BASE(GSYS_DETAIL_TYPENAME(type),name)\
	     GSYS_DETAIL_VTID_SET_DECL_OFFSET(var)}

#define GSYS_VTID_DECL_AUTONAME(x) GSYS_VTID_DECL(x,#x)
#define GSYS_VTID_DECLA(x) GSYS_VTID_DECL(x,#x)


#define GSYS_DETAIL_VTID_DECLFN(x,t,y){ GSYS_DETAIL_VTID_DECL_BASE(t,y)\
	struct handler_struct{\
	static ::gsys::vtid_result GSYS_CALLBACK handler(::gsys::vtid_acess_data const  *dta,::gsys::vtid_ref* ref)\
	{lType* tp =  (reinterpret_cast<lType*>(dta->ptr));\
		if(tp->x(dta->acc_mode,dta->name,dta->param,dta->type,ref))return ::gsys::vtid_ok;return ::gsys::vtid_err; \
	}};\
	dcl.h_access_var = handler_struct::handler;}

#define GSYS_VTID_DECL_VAR(x,type,n){ GSYS_DETAIL_VTID_DECL_BASE(GSYS_DETAIL_TYPENAME(type),n)\
struct handler_struct{\
	static ::gsys::vtid_result GSYS_CALLBACK handler(::gsys::vtid_acess_data const  *dta,::gsys::vtid_ref* ref)\
	{lType* tp =  (reinterpret_cast<lType*>(dta->ptr));\
		if(!tp->x.valid())tp->x.create(GSYS_DETAIL_TYPENAME(type));\
		if(ref->init(tp->x.as_ref()))return ::gsys::vtid_ok;return ::gsys::vtid_err; \
	}};\
	dcl.h_access_var = handler_struct::handler;}


#define GSYS_VTID_CLASS_ACCESS_PROC(x,t,y){ GSYS_DETAIL_VTID_DECL_BASE(GSYS_DETAIL_TYPENAME(t),y)\
	struct handler_struct{\
	static ::gsys::vtid_result GSYS_CALLBACK handler(::gsys::vtid_acess_data const  *dta,::gsys::vtid_ref* ref)\
	{lType* tp =  (reinterpret_cast<lType*>(dta->ptr));\
		return tp->x(dta,ref); \
	}};\
	dcl.h_access_var = handler_struct::handler;}


#define GSYS_VTID_DECL_ARRAY(var,name){ GSYS_DETAIL_VTID_DECL_BASE(GSYS_DETAIL_TYPENAME_VAR(*(ptClass->var)),name)\
struct handler_struct{\
	static ::gsys::vtid_result GSYS_CALLBACK handler(::gsys::vtid_acess_data const  *dta,::gsys::vtid_ref* ref){\
	lType* tp =  (reinterpret_cast<lType*>(dta->ptr));\
	int index=0; if(dta->param && dta->param[0]){if(!gsys::detail::vtid_op<int>::numconst_from_str(&index,dta->param))return gsys::vtid_err_invalid_param;}\
	if(sizeof(tp->var)/sizeof(*(tp->var)) <= index)return gsys::vtid_err_invalid_param;\
	ref->init(tp->var[index]);return gsys::vtid_ok; \
	}};\
	dcl.h_access_var = handler_struct::handler;}

#define GSYS_VTID_CLASS_INIT_PROC(p,n){ GSYS_DETAIL_VTID_DECL_BASE(GSYS_DETAIL_TYPENAME(void),n)\
	struct handler_struct{\
	static ::gsys::vtid_result GSYS_CALLBACK handler(::gsys::vtid_acess_data const  *dta,::gsys::vtid_ref* ref){\
	if(dta->acc_mode != gsys::vtid_access_init)return gsys::vtid_err_acess_denied;\
	lType* tp =  (reinterpret_cast<lType*>(dta->ptr));\
	if(tp->p(dta))return gsys::vtid_proceeded;\
	return gsys::vtid_err_unknown;\
	}};\
	dcl.h_access_var = handler_struct::handler;}


#define GSYS_VTID_AUTO_PTR(var,name){ GSYS_DETAIL_VTID_DECL_BASE(GSYS_DETAIL_TYPENAME(void),name)\
	struct handler_struct{\
	static ::gsys::vtid_result GSYS_CALLBACK handler(::gsys::vtid_acess_data const  *dta,::gsys::vtid_ref* ref)\
	{lType* tp =  (reinterpret_cast<lType*>(dta->ptr));\
	return tp->var.ref(dta,ref);\
	}\
	static bool GSYS_CALLBACK handler2(void* ptr,vtid_ref_info& rInf)\
	{lType* tp =  (reinterpret_cast<lType*>(ptr));\
	    return tp->var.serialize(rInf);\
	}};\
	dcl.h_access_var = handler_struct::handler;\
	dcl.h_serialize_proc = handler_struct::handler2;\
}

#define GSYS_VTID_DECL_PTR(var,name){ GSYS_DETAIL_VTID_DECL_BASE(GSYS_DETAIL_TYPENAME(void),name)\
	struct handler_struct{\
	static ::gsys::vtid_result GSYS_CALLBACK handler(::gsys::vtid_acess_data const  *dta,::gsys::vtid_ref* ref)\
	{lType* tp =  (reinterpret_cast<lType*>(dta->ptr));\
		if(!tp->var)return vtid_err_unknown;\
		ref->init(*tp->var);\
		return vtid_ok;\
	}};\
	dcl.h_access_var = handler_struct::handler;\
}

#define GSYS_VTID_DECL_PTR_CALL(var,fn,name){ GSYS_DETAIL_VTID_DECL_BASE(GSYS_DETAIL_TYPENAME(void),name)\
struct handler_struct{\
	static ::gsys::vtid_result GSYS_CALLBACK handler(::gsys::vtid_acess_data const  *dta,::gsys::vtid_ref* ref)\
	{lType* tp =  (reinterpret_cast<lType*>(dta->ptr));\
	if(!tp->var)return vtid_err_unknown;\
	ref->init(tp->var->fn());\
	return vtid_ok;\
	}};\
	dcl.h_access_var = handler_struct::handler;\
}

#define GSYS_VTID_ACCESS_PROC(x,t,n){ GSYS_DETAIL_VTID_DECL_BASE(GSYS_DETAIL_TYPENAME(t),n)\
	dcl.h_access_var = x;}

#define GSYS_VTID_CUSTOM_PROP(x,t,y) GSYS_DETAIL_VTID_DECLFN(x,t,y)

#define GSYS_VTID_CLASS_PROPERTY_SET(x,t,y) GSYS_VTID_MEMBER_PROC1(x,y,t)


// member functions export
#define GSYS_DETAIL_VTID_MEMBER_PROC_START(name,tr,n)\
	{ GSYS_DETAIL_VTID_DECL_BASE(GSYS_DETAIL_TYPENAME(tr),name)\
	struct handler_struct: ::gsys::detail::vtid_hsb<lType>{\
	static ::gsys::vtid_result GSYS_CALLBACK handler(void* classptr,int numparams,::gsys::vtid_ref const* params,::gsys::vtid_ref* result)\
	{lType* tp =  (reinterpret_cast<lType*>(classptr));\
	if(numparams != (n)) return ::gsys::vtid_err_invalid_function_parameters;
		
#define GSYS_DETAIL_VTID_MEMBER_PROC_END\
	return ::gsys::vtid_ok; \
	}};\
	dcl.h_class_proc = handler_struct::handler; 

#define GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t)\
	dcl_src.vecProcParamsNames.push_back(GSYS_DETAIL_TYPENAME(t));

#define GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t,p,n)\
	t* p = params[n].ptr_reinterpret_cast<t>();


#define GSYS_VTID_MEMBER_PROPERY_W(variable,set_proc,type,name)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,type,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(type,p0,0)\
	tp->set_proc(*p0);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(type)\
	GSYS_DETAIL_VTID_SET_DECL_OFFSET(variable)\
	dcl.wAccessMask &= ~::gsys::vtid_access_write;\
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


#define GSYS_VTID_EXTERN_PROC(proc,name)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,0)\
	proc(); \
	GSYS_DETAIL_VTID_MEMBER_PROC_END }

#define GSYS_VTID_EXTERN_PROC1(proc,name,t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	proc(*p0);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	}

#define GSYS_VTID_EXTERN_PROC2(proc,name,t0,t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	proc(*p0,*p1);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\

#define GSYS_VTID_EXTERN_PROC3(proc,name,t0,t1,t2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t2,p2,2)\
	proc(*p0,*p1,*p2);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t2)\

#define GSYS_VTID_EXTERN_PROC4(proc,name,t0,t1,t2,t3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,4)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t2,p2,2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t3,p3,3)\
	proc(*p0,*p1,*p2,*p3);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t3)\
	}

#define GSYS_VTID_EXTERN_PROC5(proc,name,t0,t1,t2,t3,t4)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,5)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t2,p2,2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t3,p3,3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t4,p4,4)\
	proc(*p0,*p1,*p2,*p3,*p4);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t4)\
	}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define GSYS_VTID_PROCESSOR_PROC(proc,name)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,0)\
	proc(*tp); \
	GSYS_DETAIL_VTID_MEMBER_PROC_END }

#define GSYS_VTID_PROCESSOR_PROC1(proc,name,t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	proc(*tp,*p0);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	}

#define GSYS_VTID_PROCESSOR_PROC2(proc,name,t0,t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	proc(*tp,*p0,*p1);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\
	}

#define GSYS_VTID_PROCESSOR_PROC3(proc,name,t0,t1,t2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t2,p2,2)\
	proc(*tp,*p0,*p1,*p2);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t2)\
	}

#define GSYS_VTID_PROCESSOR_PROC4(proc,name,t0,t1,t2,t3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,4)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t2,p2,2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t3,p3,3)\
	proc(*tp,*p0,*p1,*p2,*p3);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t3)\
	}

#define GSYS_VTID_PROCESSOR_PROC5(proc,name,t0,t1,t2,t3,t4)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,5)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t2,p2,2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t3,p3,3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t4,p4,4)\
	proc(*tp,*p0,*p1,*p2,*p3,*p4);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t4)\
	}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define GSYS_VTID_MEMBER_PROC(proc,name)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,0)\
	tp->proc(); \
	GSYS_DETAIL_VTID_MEMBER_PROC_END }

#define GSYS_VTID_MEMBER_PROC1(proc,name,t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	tp->proc(*p0);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	}

#define GSYS_VTID_MEMBER_PROC2(proc,name,t0,t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	tp->proc(*p0,*p1);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\
	}

#define GSYS_VTID_MEMBER_PROC3(proc,name,t0,t1,t2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t2,p2,2)\
	tp->proc(*p0,*p1,*p2);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t2)\
	}

#define GSYS_VTID_MEMBER_PROC4(proc,name,t0,t1,t2,t3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,4)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t2,p2,2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t3,p3,3)\
	tp->proc(*p0,*p1,*p2,*p3);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t3)\
	}

#define GSYS_VTID_MEMBER_PROC5(proc,name,t0,t1,t2,t3,t4)\
	GSYS_DETAIL_VTID_MEMBER_PROC_START(name,void,5)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t0,p0,0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t1,p1,1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t2,p2,2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t3,p3,3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_CONV(t4,p4,4)\
	tp->proc(*p0,*p1,*p2,*p3,*p4);\
	GSYS_DETAIL_VTID_MEMBER_PROC_END \
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t0)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t1)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t2)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t3)\
	GSYS_DETAIL_VTID_MEMBER_PROC_PARAM_DECL(t4)\
	}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define GSYS_VTID_PARENT(t){\
	vtidSource.czParentTypeName = (GSYS_DETAIL_TYPENAME(t));\
	vtidSource.iParentOffset = reinterpret_cast<gsys_byte const*>(static_cast<t const*>(ptClass))-reinterpret_cast<gsys_byte const*>(ptClass);\
	vtidSource.vtStaticCast.push_back(std::pair<const char*,gsys_pt_diff>(vtidSource.czParentTypeName,vtidSource.iParentOffset));\
	}

#define GSYS_VTID_STATIC_CAST(t){\
	const char* tp = (GSYS_DETAIL_TYPENAME(t));\
	::gsys::gsys_pt_diff pd = reinterpret_cast<gsys_byte const*>(static_cast<t const*>(ptClass))-reinterpret_cast<gsys_byte const*>(ptClass);\
	vtidSource.vtStaticCast.push_back(std::pair<const char*,gsys_pt_diff>(tp,pd));\
	}

//////////////////////////////////////////////////////////////////////////
//     Enum Support

// Start from str func
#define GSYS_DETAIL_VTID_ENUM_BEGIN { \
struct handler_struct: ::gsys::detail::vtid_hsb<lType>{ \
	static bool GSYS_CALLBACK handler(void* ptr,gstring const &str) \
	{lType& val = *reinterpret_cast<lType*>(ptr);\
	
#define GSYS_VTID_ENUM(value,name) if(_stricmp(str.c_str(),name)==0){val = value;return true;}

// End from str func
#define GSYS_DETAIL_VTID_ENUM_END \
    ;return gsys::detail::vtid_op<lType>::intnumconst_from_str(ptr,str);}}; \
	vtidSource.BaseOp.h_from_str = handler_struct::handler; }

#define GSYS_VTID_ENUM_BEGIN(x) GSYS_VTID_BEGIN(x){\
	GSYS_VTID_CREATEBLE;GSYS_VTID_COPYABLE;GSYS_VTID_COMPARABLE;GSYS_VTID_RAW_SERIALIZEBLE;\
	GSYS_DETAIL_VTID_ENUM_BEGIN
#define GSYS_VTID_ENUM_END(x) GSYS_DETAIL_VTID_ENUM_END}GSYS_VTID_END(x)

/// ENUM HELPERS

#define GSYS_VTID_AUTO_ENUM(x)	GSYS_VTID_ENUM(x,#x);
#define GSYS_VTID_PREFIX_ENUM(p,v) GSYS_VTID_ENUM(p##v,#v) GSYS_VTID_ENUM(p##v,#p#v)

///////////////////////////////////////////////////////////////////////////
// User info 
///////////////////////////////////////////////////////////////////////////

#define GSYS_VTID_DESCRIPTION(x) {vtidSource.UserInf.description = (x);}
#define GSYS_VTID_VALUE_INFO(x) {vtidSource.UserInf.values = (x);}
#define GSYS_VTID_DECL_DESC(x) {desc = (x);}

#endif
