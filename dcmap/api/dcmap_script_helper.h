#ifndef dcmapapi_script_exec_helper_h
#define dcmapapi_script_exec_helper_h
#pragma once

#include <string>
#include <map>
#include "dcmap.h"
#include "boost/mpl/vector/vector10.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/bool.hpp"

#ifndef DCMAP_CALL_MEMBER_FN
#	define DCMAP_CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))
#endif

static dcmapDBREF InvalidDBREF;

namespace dcmap_script_helper
{
	namespace mpl = boost::mpl;

	using mpl::true_;
	using mpl::false_;

	typedef mpl::long_<1> _1;
	typedef mpl::long_<2> _2;
	typedef mpl::long_<3> _3;
	typedef mpl::long_<4> _4;
	typedef mpl::long_<5> _5;
	typedef mpl::long_<6> _6;

	template<class T> struct is_void : public false_ {};
	template<>	struct is_void<void> : public true_{};

	template<class T> struct is_not_void : public true_ {};
	template<>	struct is_not_void<void> : public false_{};

	template <class T>
	T ValVarGet(IDCMapVarValue* val,int id)
	{
		return  IDCMapVarValuePtr(val).Get<T>(id);
	}	
	
	template <class T>
	class CValVarGet
	{
		T m_val;
	public:
		typedef T value_type;
		CValVarGet(IDCMapVarValue* val,int id):m_val(ValVarGet<T>(val,id)){}
		operator value_type(){return m_val;}
	};

	template <class T>
	void ValVarAdd(IDCMapVarValue* val,T const& v)
	{
		IDCMapVarValuePtr(val).Add<T>(v);
	}

	template <> inline
	void ValVarAdd<IDCMapVarValue*>(IDCMapVarValue* val,IDCMapVarValue* const& v)
	{
		val->AddVar(v);
		if(v)v->Destroy();
	}

	template <> inline
		void ValVarAdd<dcmapCSTR>(IDCMapVarValue* val,dcmapCSTR const& v)
	{
		val->AddString(g_pDCMapSys->StrToWide(v));
	}

	template <> inline
		void ValVarAdd<std::string>(IDCMapVarValue* val,std::string const& v)
	{
		val->AddString(g_pDCMapSys->StrToWide(v.c_str()));
	}

	template <> inline 
		dcmapDBREF const& ValVarGet<dcmapDBREF const&>(IDCMapVarValue* val,int id)
	{
		dcmapDBREF const* ref = val->GetDBRef(id);
		if(ref)return *ref;
		return InvalidDBREF;
	}

	template <>
	class CValVarGet<dcmapCSTR>
	{
		std::string m_val;
	public:
		typedef dcmapCSTR value_type;

		CValVarGet(IDCMapVarValue* val,int id):
		m_val(g_pDCMapSys->WideToStr(val->GetString(id))){}
		operator value_type(){return m_val.c_str();}
	};

	template <>
	class CValVarGet<std::string>
	{
		std::string m_val;
	public:
		typedef std::string value_type;

		CValVarGet(IDCMapVarValue* val,int id):
		m_val(g_pDCMapSys->WideToStr(val->GetString(id))){}
		operator value_type(){return m_val;}
	};


/*
	template <> inline 
		StrWrap<dcmapCHAR>  ValVarGet<dcmapCSTR>(IDCMapVarValue* val,int id)
	{
		return StrWrap<dcmapCHAR>(g_pDCMapSys->WideToStr(val->GetString(id)));
	}
	*/

	/*
	template <>
	void inline ValVarAdd<dcmapDBREF const*>(IDCMapVarValue* val,dcmapDBREF const* const& v)
	{
		val->AddDBRef(v);
	}



	template <> inline 
		dcmapWCSTR  ValVarGet<dcmapWCSTR>(IDCMapVarValue* val,int id)
	{
		return  val->GetString(id);
	}

	template <> inline 
	IDCMapScriptable*  ValVarGet<IDCMapScriptable*>(IDCMapVarValue* val,int id)
	{
		return  0;//val->GetString(id);
	}
	#ifdef DCMAP_VCL

	template <> inline 
		UnicodeString  ValVarGet<UnicodeString>(IDCMapVarValue* val,int id)
	{
		return  val->GetString(id);
	}		

	template <> inline
	void ValVarAdd<UnicodeString>(IDCMapVarValue* val,UnicodeString const& v)
	{
		val->AddString(v.w_str());
	}

	#endif
	*/

	#ifdef DCMAP_BORLANDC

	template <class R, class T> mpl::vector1<R> deduce_signature(R(__msfastcall T::*Fn)()){return mpl::vector1<R>();}
	template <class R, class T,class A1> mpl::vector2<R,A1> deduce_signature(R(__msfastcall T::*Fn)(A1)){return mpl::vector2<R,A1>();}
	template <class R, class T,class A1,class A2> mpl::vector3<R,A1,A2> deduce_signature(R(__msfastcall T::*Fn)(A1,A2)){return mpl::vector3<R,A1,A2>();}
	template <class R, class T,class A1,class A2,class A3> mpl::vector4<R,A1,A2,A3> deduce_signature(R(__msfastcall T::*Fn)(A1,A2,A3)){return mpl::vector4<R,A1,A2,A3>();}
	template <class R, class T,class A1,class A2,class A3,class A4> mpl::vector5<R,A1,A2,A3,A4> deduce_signature(R(__msfastcall T::*Fn)(A1,A2,A3,A4)){return mpl::vector5<R,A1,A2,A3,A4>();}
	template <class R, class T,class A1,class A2,class A3,class A4,class A5> mpl::vector6<R,A1,A2,A3,A4,A5> deduce_signature(R(__msfastcall T::*Fn)(A1,A2,A3,A4,A5)){return mpl::vector6<R,A1,A2,A3,A4,A5>();}

	#endif

	#ifdef DCMAP_MSVC

	template <class R, class T> mpl::vector1<R> deduce_signature(R(__thiscall T::*Fn)()){return mpl::vector1<R>();}
	template <class R, class T,class A1> mpl::vector2<R,A1> deduce_signature(R(__thiscall T::*Fn)(A1)){return mpl::vector2<R,A1>();}
	template <class R, class T,class A1,class A2> mpl::vector3<R,A1,A2> deduce_signature(R(__thiscall T::*Fn)(A1,A2)){return mpl::vector3<R,A1,A2>();}
	template <class R, class T,class A1,class A2,class A3> mpl::vector4<R,A1,A2,A3> deduce_signature(R(__thiscall T::*Fn)(A1,A2,A3)){return mpl::vector4<R,A1,A2,A3>();}
	template <class R, class T,class A1,class A2,class A3,class A4> mpl::vector5<R,A1,A2,A3,A4> deduce_signature(R(__thiscall T::*Fn)(A1,A2,A3,A4)){return mpl::vector5<R,A1,A2,A3,A4>();}
	template <class R, class T,class A1,class A2,class A3,class A4,class A5> mpl::vector6<R,A1,A2,A3,A4,A5> deduce_signature(R(__thiscall T::*Fn)(A1,A2,A3,A4,A5)){return mpl::vector6<R,A1,A2,A3,A4,A5>();}

	#else

 /*	template <class R, class T> mpl::vector1<R> deduce_signature(R(T::*Fn)()){return mpl::vector1<R>();}
	template <class R, class T,class A1> mpl::vector2<R,A1> deduce_signature(R(T::*Fn)(A1)){return mpl::vector2<R,A1>();}
	template <class R, class T,class A1,class A2> mpl::vector3<R,A1,A2> deduce_signature(R(T::*Fn)(A1,A2)){return mpl::vector3<R,A1,A2>();}
	template <class R, class T,class A1,class A2,class A3> mpl::vector4<R,A1,A2,A3> deduce_signature(R(T::*Fn)(A1,A2,A3)){return mpl::vector4<R,A1,A2,A3>();}
	template <class R, class T,class A1,class A2,class A3,class A4> mpl::vector5<R,A1,A2,A3,A4> deduce_signature(R(T::*Fn)(A1,A2,A3,A4)){return mpl::vector5<R,A1,A2,A3,A4>();}
	template <class R, class T,class A1,class A2,class A3,class A4,class A5> mpl::vector6<R,A1,A2,A3,A4,A5> deduce_signature(R(T::*Fn)(A1,A2,A3,A4,A5)){return mpl::vector6<R,A1,A2,A3,A4,A5>();}
*/
	#endif

	template <class R, class T> mpl::vector1<R> deduce_signature(R(_stdcall T::*Fn)()){return mpl::vector1<R>();}
	template <class R, class T,class A1> mpl::vector2<R,A1> deduce_signature(R(_stdcall T::*Fn)(A1)){return mpl::vector2<R,A1>();}
	template <class R, class T,class A1,class A2> mpl::vector3<R,A1,A2> deduce_signature(R(_stdcall T::*Fn)(A1,A2)){return mpl::vector3<R,A1,A2>();}
	template <class R, class T,class A1,class A2,class A3> mpl::vector4<R,A1,A2,A3> deduce_signature(R(_stdcall T::*Fn)(A1,A2,A3)){return mpl::vector4<R,A1,A2,A3>();}
	template <class R, class T,class A1,class A2,class A3,class A4> mpl::vector5<R,A1,A2,A3,A4> deduce_signature(R(_stdcall T::*Fn)(A1,A2,A3,A4)){return mpl::vector5<R,A1,A2,A3,A4>();}
	template <class R, class T,class A1,class A2,class A3,class A4,class A5> mpl::vector6<R,A1,A2,A3,A4,A5> deduce_signature(R(_stdcall T::*Fn)(A1,A2,A3,A4,A5)){return mpl::vector6<R,A1,A2,A3,A4,A5>();}

	template <class R, class T> mpl::vector1<R> deduce_signature(R(_cdecl T::*Fn)()){return mpl::vector1<R>();}
	template <class R, class T,class A1> mpl::vector2<R,A1> deduce_signature(R(_cdecl T::*Fn)(A1)){return mpl::vector2<R,A1>();}
	template <class R, class T,class A1,class A2> mpl::vector3<R,A1,A2> deduce_signature(R(_cdecl T::*Fn)(A1,A2)){return mpl::vector3<R,A1,A2>();}
	template <class R, class T,class A1,class A2,class A3> mpl::vector4<R,A1,A2,A3> deduce_signature(R(_cdecl T::*Fn)(A1,A2,A3)){return mpl::vector4<R,A1,A2,A3>();}
	template <class R, class T,class A1,class A2,class A3,class A4> mpl::vector5<R,A1,A2,A3,A4> deduce_signature(R(_cdecl T::*Fn)(A1,A2,A3,A4)){return mpl::vector5<R,A1,A2,A3,A4>();}
	template <class R, class T,class A1,class A2,class A3,class A4,class A5> mpl::vector6<R,A1,A2,A3,A4,A5> deduce_signature(R(_cdecl T::*Fn)(A1,A2,A3,A4,A5)){return mpl::vector6<R,A1,A2,A3,A4,A5>();}

	template <class R, class T> mpl::vector1<R> deduce_signature(R(_fastcall T::*Fn)()){return mpl::vector1<R>();}
	template <class R, class T,class A1> mpl::vector2<R,A1> deduce_signature(R(_fastcall T::*Fn)(A1)){return mpl::vector2<R,A1>();}
	template <class R, class T,class A1,class A2> mpl::vector3<R,A1,A2> deduce_signature(R(_fastcall T::*Fn)(A1,A2)){return mpl::vector3<R,A1,A2>();}
	template <class R, class T,class A1,class A2,class A3> mpl::vector4<R,A1,A2,A3> deduce_signature(R(_fastcall T::*Fn)(A1,A2,A3)){return mpl::vector4<R,A1,A2,A3>();}
	template <class R, class T,class A1,class A2,class A3,class A4> mpl::vector5<R,A1,A2,A3,A4> deduce_signature(R(_fastcall T::*Fn)(A1,A2,A3,A4)){return mpl::vector5<R,A1,A2,A3,A4>();}
	template <class R, class T,class A1,class A2,class A3,class A4,class A5> mpl::vector6<R,A1,A2,A3,A4,A5> deduce_signature(R(_fastcall T::*Fn)(A1,A2,A3,A4,A5)){return mpl::vector6<R,A1,A2,A3,A4,A5>();}


	template < class T >
	struct base_invoker
	{
		virtual int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)=0;
	};

	template <class Fn, class T >
	struct basic_invoker : public base_invoker<T>
	{
		Fn ptr;
		basic_invoker(Fn _ptr):ptr(_ptr){}
	};

	template <class N,class R,class T,class Fn,class Signature>
	struct func_invoker : public basic_invoker<Fn,T>{};

	template <class T,class Fn,class Signature>
	struct func_invoker<_1,true_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			ValVarAdd(pResult,DCMAP_CALL_MEMBER_FN(obj,ptr)());	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class Fn,class Signature>
	struct func_invoker<_1,false_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			DCMAP_CALL_MEMBER_FN(obj,ptr)();	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class Fn,class Signature>
	struct func_invoker<_2,true_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			ValVarAdd(pResult,DCMAP_CALL_MEMBER_FN(obj,ptr)(
				CValVarGet<typename Signature::item1>(pParams,0)
				));	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class Fn,class Signature>
	struct func_invoker<_2,false_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			DCMAP_CALL_MEMBER_FN(obj,ptr)(
				CValVarGet<typename Signature::item1>(pParams,0)
				);	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class Fn,class Signature>
	struct func_invoker<_3,false_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			DCMAP_CALL_MEMBER_FN(obj,ptr)(
				CValVarGet<typename Signature::item1>(pParams,0),
				CValVarGet<typename Signature::item2>(pParams,1)
				);	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class Fn,class Signature>
	struct func_invoker<_3,true_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			ValVarAdd(pResult, DCMAP_CALL_MEMBER_FN(obj,ptr)(
				CValVarGet<typename Signature::item1>(pParams,0),
				CValVarGet<typename Signature::item2>(pParams,1)
				));	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class Fn,class Signature>
	struct func_invoker<_4,false_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			DCMAP_CALL_MEMBER_FN(obj,ptr)(
				CValVarGet<typename Signature::item1>(pParams,0),
				CValVarGet<typename Signature::item2>(pParams,1),
				CValVarGet<typename Signature::item3>(pParams,2)
				);	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class Fn,class Signature>
	struct func_invoker<_4,true_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			ValVarAdd(pResult,DCMAP_CALL_MEMBER_FN(obj,ptr)(
				CValVarGet<typename Signature::item1>(pParams,0),
				CValVarGet<typename Signature::item2>(pParams,1),
				CValVarGet<typename Signature::item3>(pParams,2)
				));	
			return DCMEXEC_SUCCESS;
		}	
	};


	template <class T,class Fn,class Signature>
	struct func_invoker<_5,false_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			DCMAP_CALL_MEMBER_FN(obj,ptr)(
				CValVarGet<typename Signature::item1>(pParams,0),
				CValVarGet<typename Signature::item2>(pParams,1),
				CValVarGet<typename Signature::item3>(pParams,2),
				CValVarGet<typename Signature::item4>(pParams,3)
				);	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class Fn,class Signature>
	struct func_invoker<_5,true_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			ValVarAdd(pResult,DCMAP_CALL_MEMBER_FN(obj,ptr)(
				CValVarGet<typename Signature::item1>(pParams,0),
				CValVarGet<typename Signature::item2>(pParams,1),
				CValVarGet<typename Signature::item3>(pParams,2),
				CValVarGet<typename Signature::item4>(pParams,3)
				));	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class Fn,class Signature>
	struct func_invoker<_6,false_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			DCMAP_CALL_MEMBER_FN(obj,ptr)(
				CValVarGet<typename Signature::item1>(pParams,0),
				CValVarGet<typename Signature::item2>(pParams,1),
				CValVarGet<typename Signature::item3>(pParams,2),
				CValVarGet<typename Signature::item4>(pParams,3),
				CValVarGet<typename Signature::item5>(pParams,4)
				);	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class Fn,class Signature>
	struct func_invoker<_6,true_,T,Fn,Signature> : public basic_invoker<Fn,T>
	{
		func_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			ValVarAdd(pResult,DCMAP_CALL_MEMBER_FN(obj,ptr)(
				CValVarGet<typename Signature::item1>(pParams,0),
				CValVarGet<typename Signature::item2>(pParams,1),
				CValVarGet<typename Signature::item3>(pParams,2),
				CValVarGet<typename Signature::item4>(pParams,3),
				CValVarGet<typename Signature::item5>(pParams,4)
				));	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class N, class Fn,class T,class Signature>	
	struct process_invoker : public base_invoker<T>{};

	template <class T,class Fn,class Signature>
	struct process_invoker<mpl::long_<5>,T,Fn,Signature>  : public basic_invoker<Fn,T>
	{
		process_invoker(Fn fn):basic_invoker<Fn,T>(fn){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			DCMAP_CALL_MEMBER_FN(obj,ptr)(type,command,pParams,pResult);	
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class GetFn,class SetFn,class Signature>
	struct property_rw_invoker : public base_invoker<T>
	{
		SetFn set;
		GetFn get;

		property_rw_invoker(GetFn _get,SetFn _set):set(_set),get(_get){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			if(type == DCMEXEC_SET){ DCMAP_CALL_MEMBER_FN(obj,set)(ValVarGet<typename Signature::item1>(pResult,0));}
			else if(type == DCMEXEC_GET){ ValVarAdd(pResult,DCMAP_CALL_MEMBER_FN(obj,get)());}
			else return DCMEXEC_FAIL;
			return DCMEXEC_SUCCESS;
		}	
	};

	template <class T,class GetFn,class Signature>
	struct property_r_invoker : public base_invoker<T>
	{
		GetFn get;
		property_r_invoker(GetFn _get):get(_get){}
		int Invoke(T& obj,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
		{
			if(type == DCMEXEC_GET){ ValVarAdd(pResult,DCMAP_CALL_MEMBER_FN(obj,get)());}
			else return DCMEXEC_FAIL;
			return DCMEXEC_SUCCESS;
		}	
	};


	template<class Target>
	class DCMapScriptReflection
	{
		friend typename Target;
		typedef DCMapScriptReflection<Target> this_type;

		typedef int (Target::*SomeProc)();

		typedef int(Target::*_ProcessProc1)(int,dcmapWCSTR,IDCMapVarValue*,IDCMapVarValue*);
		typedef int(Target::*_ProcessProc2)(int,IDCMapVarValue*,IDCMapVarValue*);
		typedef int(Target::*_ProcessProc3)(IDCMapVarValue*,IDCMapVarValue*);

		typedef int (*WrapperProc)(Target& obj,SomeProc ptr,int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);
		struct Record
		{
			int type;
			base_invoker<Target> * ptr;

			Record(){}
			Record(base_invoker<Target>* _ptr,int _type):
			type(_type),ptr(_ptr){}
		};
		typedef std::map<std::wstring,Record> MapRecords;
		MapRecords m_Records;

		int ScriptExec(Target* obj, int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult )
		{
			//_ASSERT(obj);
			MapRecords::iterator it = m_Records.find(command);
			if(it == m_Records.end())return DCMEXEC_FAIL;
			if(type == DCMEXEC_QUERY) return it->second.type;
			else
			{
				return it->second.ptr->Invoke(*obj,type,command,pParams,pResult);
			}
		}
		void add(dcmapWCSTR name,base_invoker<Target>* ptr,int type)
		{
			m_Records[name] = Record(ptr,type);
		}

	public:
		~DCMapScriptReflection()
		{
			for(MapRecords::iterator it = m_Records.begin();it!=m_Records.end();++it)
			{
			   delete it->second.ptr;
            }
		}
	protected:

		typedef Target TargetType;

		template <class Fn,class Signature>
		void add_func_invoker(dcmapWCSTR name,Fn f,Signature)
		{
			add(name,new func_invoker<
				mpl::size<Signature>::type,
				is_not_void<Signature::item0>::type,
				Target,
				Fn,
				Signature
			>(f),DCMEXEC_CALL);
		}

		template <class Fn,class Signature>
		void add_proc_invoker(dcmapWCSTR name,Fn f,Signature)
		{
			add(name,new func_invoker<mpl::size<Signature>::type,false_,Target,Fn,Signature>(f),DCMEXEC_CALL);
		}

		template <class Fn>
		void func(dcmapWCSTR name,Fn f)
		{
			add_func_invoker(name,f,deduce_signature(f));
		}

		template <class Fn>
		void proc(dcmapWCSTR name,Fn f)
		{
			add_proc_invoker(name,f,deduce_signature(f));
		}

		template <class GetFn,class SetFn,class Signature>
		void add_property_invoker(dcmapWCSTR name,GetFn get,SetFn set,Signature)
		{
			add(name,new property_rw_invoker<Target,GetFn,SetFn,Signature>(get,set),DCMEXEC_SET | DCMEXEC_GET);
		}

		template <class GetFn,class Signature>
		void add_property_invoker(dcmapWCSTR name,GetFn get,Signature)
		{
			add(name,new property_r_invoker<Target,GetFn,Signature>(get),DCMEXEC_GET);
		}

		template <class GetFn,class SetFn>
		void property(dcmapWCSTR name,GetFn get,SetFn set)
		{
			add_property_invoker(name,get,set,deduce_signature(set));
		}

		template <class GetFn>
		void property(dcmapWCSTR name,GetFn get)
		{
			add_property_invoker(name,get,deduce_signature(get));
		}

		template <class Fn,class Signature>
		void add_process_invoker(dcmapWCSTR name,Fn f,Signature,int type)
		{
			add(name,new process_invoker<mpl::size<Signature>::type,Target,Fn,Signature>(f),type);
		}

		template <class Fn>
		void process(dcmapWCSTR name,Fn f,int type=DCMEXEC_CALL)
		{
			add_process_invoker(name,f,deduce_signature(f),type);
		}
	};
}

#define DCMAP_SCRIPT_REFLECTION(T) \
struct DCMapScriptReflInit_##T : public dcmap_script_helper::DCMapScriptReflection<T> \
{DCMapScriptReflInit_##T();}; \
	static DCMapScriptReflInit_##T s_##DCMapScriptReflInit_##T; \
	int DCMAPAPI T::ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult ) \
{return s_##DCMapScriptReflInit_##T.ScriptExec(this,type,command,pParams,pResult);} \
	DCMapScriptReflInit_##T :: DCMapScriptReflInit_##T()

#define DCMAP_SCRIPT_REFLECTION_INHERITED(T) \
struct DCMapScriptReflInit_##T : public dcmap_script_helper::DCMapScriptReflection<T> \
{DCMapScriptReflInit_##T();}; \
	static DCMapScriptReflInit_##T s_##DCMapScriptReflInit_##T; \
	int DCMAPAPI T::ScriptExec( int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult ) \
{int r = s_##DCMapScriptReflInit_##T.ScriptExec(this,type,command,pParams,pResult);\
	if(r == DCMEXEC_FAIL)return Inherited::ScriptExec(type,command,pParams,pResult);else return r;} \
	DCMapScriptReflInit_##T :: DCMapScriptReflInit_##T()



#define DCMAP_SCRIPT_FUNC(F) func(dcmapWT(#F),&TargetType::F);
#define DCMAP_SCRIPT_PROC(F) proc(dcmapWT(#F),&TargetType::F);

#define DCMAP_SCRIPT_PROPERTY_RW(F) property(dcmapWT(#F),&TargetType::Get##F,&TargetType::Set##F);
#define DCMAP_SCRIPT_PROPERTY_R(F) property(dcmapWT(#F),&TargetType::Get##F);

#define DCMAP_SCRIPT_PROPERTY(F) DCMAP_SCRIPT_PROPERTY_RW(F)


#endif // dcmapapi_script_exec_helper_h

