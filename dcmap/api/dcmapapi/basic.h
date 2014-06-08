
namespace dcmap{}

#define DCMAP_MAKEFOURCC(ch0, ch1, ch2, ch3)               \
	((dcmapDWORD)(dcmapBYTE)(ch0) | ((dcmapDWORD)(dcmapBYTE)(ch1) << 8) |      \
	((dcmapDWORD)(dcmapBYTE)(ch2) << 16) | ((dcmapDWORD)(dcmapBYTE)(ch3) << 24 ))


#ifdef DCMAP_IMPL_HELPERS
#define DCMAP_IMPL_HELPER    {}
#define DCMAP_IMPL_HELPER_RET(x) {return (x);}
//#define DCMAP_IMPL_HELPER_RET    {}
#else
#define DCMAP_IMPL_HELPER_RET(x) =0;
//#define DCMAP_IMPL_HELPER    =0;
#endif

// Interface specialization template
typedef char const* DCMapInterfaceID;
template <class T> DCMapInterfaceID IDCMapGetInterfaceID(){return "";}
#define DCMAP_SPEC_INTERFACE(T) template <> inline const char* IDCMapGetInterfaceID<T*>(){return #T;}
#define DCMAP_INTERFACE_CMP(Id,T) (!_stricmp(idInterface,IDCMapGetInterfaceID<T*>()))
#define DCMAP_MODULE_SUPPORTS(T) {if(DCMAP_INTERFACE_CMP(idInterface,T))return static_cast<T*>(this);}

#define DCMAP_CP_UTF8 65001
#define DCMAP_CP_1251 1251
