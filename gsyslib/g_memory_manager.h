#pragma once
#ifndef G_MEMORY_MANAGER_H
#define G_MEMORY_MANAGER_H

#ifdef GSYS_DETAIL_BORLAND
#	include <malloc.h>
namespace gsys
{
	inline void* galloc(gsys_size_t size){return malloc((size_t)size);}
	inline void  gfree(void* pt){free(pt);}
	inline void* grealloc(void* pt,size_t size){return realloc(pt,size);}
	template<typename T>
		T* galloc_raw_objects(gsys_size_t sz){return (T*)galloc(sz*sizeof(T));}

	struct memory_manager_interface{};
}
#else

#if defined(GSYS_ENABLE_MEMORY_MANAGER) && defined(GSYS_DEBUG) 
#	define CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
#else
#	include <stdlib.h>
#endif

#	include <malloc.h>

namespace gsys
{
	struct memory_manager_interface
	{
		virtual void* GSYS_API alloc(gsys_size_t size)=0;
		virtual void  GSYS_API free(void* pt)=0;
		virtual void* GSYS_API realloc(void* pt,size_t size)=0;
	};
	namespace detail
	{
		void* GSYS_API gmem_alloc(gsys_size_t size);
		void  GSYS_API gmem_free(void* pt);
		void* GSYS_API gmem_realloc(void* pt,size_t size);
	}

#ifdef GSYS_ENABLE_MEMORY_MANAGER
	memory_manager_interface* get_memory_manager();
	void set_memory_manager(memory_manager_interface* pman);

	inline void* galloc(gsys_size_t size){return detail::gmem_alloc((size_t)size);}
	inline void  gfree(void* pt){detail::gmem_free(pt);}
	inline void* grealloc(void* pt,size_t size){return detail::gmem_realloc(pt,size);}
#else
	inline void* galloc(gsys_size_t size){return malloc((size_t)size);}
	inline void  gfree(void* pt){free(pt);}
	inline void* grealloc(void* pt,size_t size){return realloc(pt,size);}
	inline memory_manager_interface* get_memory_manager(){return 0;};
	inline void set_memory_manager(memory_manager_interface* pman){};
#endif
	template<typename T>
		T* galloc_raw_objects(gsys_size_t sz){return (T*)galloc(sz*sizeof(T));}
}

#ifdef GSYS_CUSTOM_MEMORY_MANAGER
	void* operator new(size_t sz);
	void operator  delete(void* pt);
#else
	#ifdef GSYS_ENABLE_MEMORY_MANAGER
	inline void* operator new(size_t sz){return ::gsys::galloc((::gsys::gsys_size_t)sz);}
		inline void operator  delete(void* pt){return ::gsys::gfree(pt);}
	#endif
#endif


#endif //GSYS_DETAIL_BORLAND

#endif
