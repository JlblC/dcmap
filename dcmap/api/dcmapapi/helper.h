#ifndef dcmapapi_helper_h__
#define dcmapapi_helper_h__

#include <vector>
#include <memory.h>

#define LSTR(x) (g_pLocalStrings->GetString(#x))
#define LSTRW(x) (g_pLocalStrings->GetStringW(#x))
#define LSTRSECTION(x) (g_pLocalStrings->SelectSection(#x))
#define SLSTR(x) (g_pLocalStrings->GetSectionString(#x))
#define SLSTRW(x) (g_pLocalStrings->GetSectionStringW(#x))
#define SSLSTR(x,s) (g_pLocalStrings->GetSectionString(#x,#s))
#define SSLSTRW(x,s) (g_pLocalStrings->GetSectionStringW(#x,#s))
    // Some helper classes

	class DCMapScopeBlockUpdates
	{
		IDCMapCore* m_core;
	public:
		explicit DCMapScopeBlockUpdates(IDCMapCore* core) : m_core(core)
		{
			m_core->LockDataUpdate();
		}
		~DCMapScopeBlockUpdates()
		{
			m_core->UnlockDataUpdate();
		}
	};

	// Module autoregistation
	class CDCMapAutoRegisterHelperManager;

	extern IDCMapSys* g_pDCMapSys; 
	extern IDCMapLocalStrings* g_pLocalStrings;
	extern CDCMapAutoRegisterHelperManager* g_pDCMapAutoRegisterHelperManager;

	class CDCMapAutoRegisterHelperManager
	{
		struct Treg
		{
			char const* RegClass;
			char const* RegName;
			char const* RegAlias;
			char const* RegParams;
			dcmapCreateCallback pCreateCallback;
			void* pUserData;
		};
		std::vector<Treg> m_vecRegs;
	public:
		static void RegisterPlugin(dcmapCSTR RegClass,dcmapCSTR RegName,
			dcmapCreateCallback pCreateCallback,void* pUserData,dcmapCSTR RegParams=0)
		{
			if(!g_pDCMapAutoRegisterHelperManager) 
				g_pDCMapAutoRegisterHelperManager = new CDCMapAutoRegisterHelperManager;
			Treg reg;
			memset(&reg,0,sizeof(reg));
			reg.RegClass = RegClass;
			reg.RegName = RegName;
			reg.pCreateCallback = pCreateCallback;
			reg.pUserData = pUserData;
			reg.RegParams = RegParams;
			g_pDCMapAutoRegisterHelperManager->m_vecRegs.push_back(reg);
		}

		static void RegisterAlias(dcmapCSTR RegClass,dcmapCSTR RegName, dcmapCSTR Alias)
		{
			if(!g_pDCMapAutoRegisterHelperManager) g_pDCMapAutoRegisterHelperManager = new CDCMapAutoRegisterHelperManager;
			Treg reg;
			memset(&reg,0,sizeof(reg));
			reg.RegClass = RegClass;
			reg.RegName = RegName;
			reg.RegAlias = Alias;
			g_pDCMapAutoRegisterHelperManager->m_vecRegs.push_back(reg);
		}

		void RegisterModules(IDCMapSys* pSys)
		{
			for(unsigned i= 0;i< m_vecRegs.size();i++)
			{
				if(m_vecRegs[i].pCreateCallback)
				{
					g_pDCMapSys->RegisterPlugin(m_vecRegs[i].RegClass,
						m_vecRegs[i].RegName,
						m_vecRegs[i].pCreateCallback,
						m_vecRegs[i].pUserData,
						m_vecRegs[i].RegParams
						);
				}
				if(m_vecRegs[i].RegAlias)
				{
					g_pDCMapSys->RegisterAlias(m_vecRegs[i].RegClass,m_vecRegs[i].RegAlias,m_vecRegs[i].RegName);
				}
			}
		}
	};
	
	template<class Module>
	struct DCMapAutoRegisterHelper
	{
		static IDCMapObject* DCMAPAPI CreateCallback(DCMapObjectCreationParams const* pUserData)
		{
			Module* pModule = new Module();
			if(!pModule->OnCreate(pUserData))
			{
				delete pModule;
				return 0;
			}
			return pModule;
		}
		DCMapAutoRegisterHelper(dcmapCSTR RegClass,dcmapCSTR RegName,dcmapCSTR RegParams=0)
		{
			CDCMapAutoRegisterHelperManager::RegisterPlugin(RegClass,RegName,CreateCallback,0,RegParams);					
		}
	};

	struct DCMapAutoRegisterAliasHelper
	{
		DCMapAutoRegisterAliasHelper(dcmapCSTR RegClass,dcmapCSTR RegName,dcmapCSTR Alias)
		{
			CDCMapAutoRegisterHelperManager::RegisterAlias(RegClass,RegName,Alias);					
		}
	};

#define DCMAP_DETAIL_CONCAT_(x,y) x##y
#define DCMAP_DETAIL_CONCAT(x,y) DCMAP_DETAIL_CONCAT_(x,y)


#define DCMAP_REGALIAS(Class,Name,Alias) static DCMapAutoRegisterAliasHelper DCMAP_DETAIL_CONCAT(dcmap_register_alias,__LINE__)((Class),(Name),(Alias));
#define DCMAP_REGALIAS2(Class,Name,Alias) static DCMapAutoRegisterAliasHelper DCMAP_DETAIL_CONCAT(dcmap_register_alias2,__LINE__)((Class),(Name),(Alias));
#define DCMAP_REGALIAS3(Class,Name,Alias) static DCMapAutoRegisterAliasHelper DCMAP_DETAIL_CONCAT(dcmap_register_alias3,__LINE__)((Class),(Name),(Alias));

#define DCMAP_REGISTER(Module,Class,Name) static DCMapAutoRegisterHelper<Module> DCMAP_DETAIL_CONCAT(dcmap_register_##Module,__LINE__)(Class,Name);
#define DCMAP_REGISTEREX(Module,Class,Name,Params) static DCMapAutoRegisterHelper<Module> DCMAP_DETAIL_CONCAT(dcmap_register_##Module,__LINE__)(Class,Name,Params);

#define DCMAP_IMPLEMENT_PLUGIN \
	IDCMapSys* g_pDCMapSys=0;\
	IDCMapLocalStrings* g_pLocalStrings=0;\
	CDCMapAutoRegisterHelperManager* g_pDCMapAutoRegisterHelperManager=0;\
	extern "C"  __declspec(dllexport) dcmapINT  DCMAPDLLAPI  \
	InitPluginProc(IDCMapSys* pSys,IDCMapLocalStrings* pLocalStrings,dcmapDWORD iApiVer){\
		if(iApiVer != DCMAP_API_VERSION)return -1;\
		g_pDCMapSys=pSys;\
		g_pLocalStrings=pLocalStrings;\
		if(g_pDCMapAutoRegisterHelperManager){\
			g_pDCMapAutoRegisterHelperManager->RegisterModules(g_pDCMapSys);\
			delete g_pDCMapAutoRegisterHelperManager;g_pDCMapAutoRegisterHelperManager=0;\
		}\
			return 1;\
	}\


class IDCMapVarValueBasePtr
{
protected:
	IDCMapVarValue* ptr;

	void _new(){ptr = g_pDCMapSys->CreateVarValue();};
	void _delete(){if(ptr)ptr->Destroy();};

public:
	IDCMapVarValueBasePtr():ptr(0){}

	IDCMapVarValueBasePtr(IDCMapVarValue* _ptr){ptr = _ptr;}
	IDCMapVarValueBasePtr& operator = (IDCMapVarValue* _ptr){ptr = _ptr;return *this;}

	operator IDCMapVarValue*(){return ptr;}
	IDCMapVarValue* operator -> (){return ptr;}

	template<class T>	void Add(T const& v){ptr->AddObject(v);}
	template<class T>	void Add(dcmapWCSTR n,T const& v){ptr->SetNamedObject(n,v);}
	template<class T>	void Add(int n,T const& v){ptr->SetObject(n,v);}

	template<class T>	T Get(dcmapWCSTR n){return ptr->GetNamedObject(n)->PtrCast<T>();}
	template<class T>	T Get(int n){return ptr->GetObject(n)->PtrCast<T>();}

	template<class T>
	IDCMapVarValueBasePtr& operator << (T const& v){Add(v);return *this;}
};

class IDCMapVarValuePtr : public IDCMapVarValueBasePtr
{
public:
	IDCMapVarValuePtr(){}

    IDCMapVarValuePtr(IDCMapVarValue* _ptr){ptr = _ptr;}
	IDCMapVarValuePtr& operator = (IDCMapVarValue* _ptr){ptr = _ptr;return *this;}

	operator IDCMapVarValue*(){return ptr;}
	IDCMapVarValue* operator -> (){return ptr;}

	template<class T>
	IDCMapVarValuePtr& operator << (T const& v){Add(v);return *this;}
public:

	template<class IndexT>
	class IndexedRef
	{
		friend class IDCMapVarValuePtr;
		mutable IndexT index;
		IDCMapVarValueBasePtr ptr;
		IndexedRef(IndexT n,IDCMapVarValueBasePtr p):index(n),ptr(p){}
	public:
		template<class T>	void operator = (T const& v){ptr.Add<T>(index,v);}
		template<class T>	void operator << (T const& v){ptr.Add<T>(index,v);}
		template<class T>	void operator >> (T & v){T = ptr.Get<T>(index);}

		template<class T>	operator T(){return ptr.Get<T>(index);}

		dcmapWCSTR c_str(){return ptr.Get<dcmapWCSTR>(index);}

#ifdef DCMAP_VCL
		UnicodeString Str(){return ptr.Get<dcmapWCSTR>(index);}
#endif
#ifdef DCMAP_STD_STRING
		std::wstring str(){return ptr.Get<dcmapWCSTR>(index);}
#endif
	};
public:
	IndexedRef<int> operator[](int n){return IndexedRef<int>(n,*(IDCMapVarValueBasePtr*)this);}

	IndexedRef<dcmapWCSTR> operator[](dcmapWCSTR n){return IndexedRef<dcmapWCSTR>(n,*(IDCMapVarValueBasePtr*)this);}
#ifdef DCMAP_STD_STRING
	IndexedRef<dcmapWCSTR> operator[](std::wstring const& n){return IndexedRef<dcmapWCSTR>(n.c_str(),*(IDCMapVarValueBasePtr*)this);}
#endif
#ifdef DCMAP_VCL
	IndexedRef<dcmapWCSTR> operator[](UnicodeString const& n){return IndexedRef<dcmapWCSTR>(n.w_str(),*(IDCMapVarValueBasePtr*)this);}
#endif

};

#define DCMAP_DETAIL_IMPL_NUMBER(TYPE) \
template<> inline TYPE IDCMapVarValueBasePtr::Get<TYPE>(dcmapWCSTR n){return (TYPE)ptr->GetNamedNumber(n);} \
template<> inline TYPE IDCMapVarValueBasePtr::Get<TYPE>(int n){return (TYPE)ptr->GetNumber(n);} \
template<> inline void IDCMapVarValueBasePtr::Add<TYPE>(TYPE  const& v){ptr->AddNumber(v);}\
template<> inline void IDCMapVarValueBasePtr::Add<TYPE>(dcmapWCSTR n,TYPE  const& v){ptr->SetNamedNumber(n,v);}\
template<> inline void IDCMapVarValueBasePtr::Add<TYPE>(int n,TYPE  const& v){ptr->SetNumber(n,v);}

DCMAP_DETAIL_IMPL_NUMBER(int);
DCMAP_DETAIL_IMPL_NUMBER(unsigned int);

DCMAP_DETAIL_IMPL_NUMBER(char);
DCMAP_DETAIL_IMPL_NUMBER(unsigned char);
DCMAP_DETAIL_IMPL_NUMBER(short);
DCMAP_DETAIL_IMPL_NUMBER(unsigned short);
DCMAP_DETAIL_IMPL_NUMBER(long);
DCMAP_DETAIL_IMPL_NUMBER(unsigned long);

DCMAP_DETAIL_IMPL_NUMBER(dcmapINT64);
DCMAP_DETAIL_IMPL_NUMBER(dcmapUINT64);

DCMAP_DETAIL_IMPL_NUMBER(double);
DCMAP_DETAIL_IMPL_NUMBER(float);
DCMAP_DETAIL_IMPL_NUMBER(bool);

template<> inline IDCMapVarValue* IDCMapVarValueBasePtr::Get<IDCMapVarValue*>(dcmapWCSTR n){return ptr->GetNamedVar(n);}
template<> inline IDCMapVarValue* IDCMapVarValueBasePtr::Get<IDCMapVarValue*>(int n){return ptr->GetVar(n);}
template<> inline void IDCMapVarValueBasePtr::Add<IDCMapVarValue*>(IDCMapVarValue*  const& v){ptr->AddVar(v);}
template<> inline void IDCMapVarValueBasePtr::Add<IDCMapVarValue*>(dcmapWCSTR n,IDCMapVarValue*  const& v){ptr->SetNamedVar(n,v);}
template<> inline void IDCMapVarValueBasePtr::Add<IDCMapVarValue*>(int n,IDCMapVarValue*  const& v){ptr->SetVar(n,v);}

template<> inline dcmapWCSTR IDCMapVarValueBasePtr::Get<dcmapWCSTR>(dcmapWCSTR n){return ptr->GetNamedString(n);}
template<> inline dcmapWCSTR IDCMapVarValueBasePtr::Get<dcmapWCSTR>(int n){return ptr->GetString(n);}
template<> inline void IDCMapVarValueBasePtr::Add<dcmapWCSTR>(dcmapWCSTR  const& v){ptr->AddString(v);}
template<> inline void IDCMapVarValueBasePtr::Add<dcmapWCSTR>(dcmapWCSTR n,dcmapWCSTR  const& v){ptr->SetNamedString(n,v);}
template<> inline void IDCMapVarValueBasePtr::Add<dcmapWCSTR>(int n,dcmapWCSTR  const& v){ptr->SetString(n,v);}

template<> inline dcmapDBREF IDCMapVarValueBasePtr::Get<dcmapDBREF>(dcmapWCSTR n){return *ptr->GetNamedDBRef(n);}
template<> inline dcmapDBREF IDCMapVarValueBasePtr::Get<dcmapDBREF>(int n){return *ptr->GetDBRef(n);}
template<> inline void IDCMapVarValueBasePtr::Add<dcmapDBREF>(dcmapDBREF  const& v){ptr->AddDBRef(&v);}
template<> inline void IDCMapVarValueBasePtr::Add<dcmapDBREF>(dcmapWCSTR n,dcmapDBREF  const& v){ptr->SetNamedDBRef(n,&v);}
template<> inline void IDCMapVarValueBasePtr::Add<dcmapDBREF>(int n,dcmapDBREF  const& v){ptr->SetDBRef(n,&v);}

template<> inline dcmapDBREF const* IDCMapVarValueBasePtr::Get<dcmapDBREF const*>(dcmapWCSTR n){return ptr->GetNamedDBRef(n);}
template<> inline dcmapDBREF const* IDCMapVarValueBasePtr::Get<dcmapDBREF const*>(int n){return ptr->GetDBRef(n);}
template<> inline void IDCMapVarValueBasePtr::Add<dcmapDBREF const*>(dcmapDBREF const*  const& v){ptr->AddDBRef(v);}
template<> inline void IDCMapVarValueBasePtr::Add<dcmapDBREF const*>(dcmapWCSTR n,dcmapDBREF const*  const& v){ptr->SetNamedDBRef(n,v);}
template<> inline void IDCMapVarValueBasePtr::Add<dcmapDBREF const*>(int n,dcmapDBREF const*  const& v){ptr->SetDBRef(n,v);}

#ifdef DCMAP_STD_STRING
template<> inline std::wstring IDCMapVarValueBasePtr::Get<std::wstring>(dcmapWCSTR n){return ptr->GetNamedString(n);}
template<> inline std::wstring IDCMapVarValueBasePtr::Get<std::wstring>(int n){return ptr->GetString(n);}
template<> inline void IDCMapVarValueBasePtr::Add<std::wstring>(std::wstring  const& v){ptr->AddString(v.c_str());}
template<> inline void IDCMapVarValueBasePtr::Add<std::wstring>(dcmapWCSTR n,std::wstring  const& v){ptr->SetNamedString(n,v.c_str());}
template<> inline void IDCMapVarValueBasePtr::Add<std::wstring>(int n,std::wstring  const& v){ptr->SetString(n,v.c_str());}
#endif

#ifdef DCMAP_VCL
template<> inline UnicodeString IDCMapVarValueBasePtr::Get<UnicodeString>(dcmapWCSTR n){return ptr->GetNamedString(n);}
template<> inline UnicodeString IDCMapVarValueBasePtr::Get<UnicodeString>(int n){return ptr->GetString(n);}
template<> inline void IDCMapVarValueBasePtr::Add<UnicodeString>(UnicodeString  const& v){ptr->AddString(v.w_str());}
template<> inline void IDCMapVarValueBasePtr::Add<UnicodeString>(dcmapWCSTR n,UnicodeString  const& v){ptr->SetNamedString(n,v.w_str());}
template<> inline void IDCMapVarValueBasePtr::Add<UnicodeString>(int n,UnicodeString  const& v){ptr->SetString(n,v.w_str());}
#endif


class IDCMapVarValueScopedPtr : public IDCMapVarValuePtr
{
public:
	IDCMapVarValueScopedPtr(){}
	IDCMapVarValueScopedPtr(IDCMapVarValue* _ptr){ptr = _ptr;}
	IDCMapVarValueScopedPtr& operator = (IDCMapVarValue* _ptr){if(ptr)ptr->Destroy();ptr = _ptr;return *this;}
	~IDCMapVarValueScopedPtr(){if(ptr)ptr->Destroy();}
	void clear(){if(ptr)ptr->Destroy();ptr=0;}
	operator IDCMapVarValue*(){return ptr;}
	IDCMapVarValue* operator -> (){return ptr;}
};


//////////////////////////////////////////////////////////////////////////
class DCMapVarValue : public IDCMapVarValuePtr
{
public:
	DCMapVarValue(){_new();}
	~DCMapVarValue(){_delete();}
	operator IDCMapVarValue*(){return ptr;}
	IDCMapVarValue* operator -> (){return ptr;}

	template<class T>
	DCMapVarValue& operator << (T const& v){Add(v);return *this;}

	template <class V1>
		explicit DCMapVarValue(V1 const& v1){_new();Add(v1);}

	template <class V1,class V2>
		explicit DCMapVarValue(V1 const& v1,V2 const& v2){_new();Add<V1>(v1);Add<V2>(v2);}

	//NamedRef operator[](dcmapWCSTR n){return NamedRef(n,*(IDCMapVarValuePtr*)this);}
};

#endif // dcmapapi_helper_h__