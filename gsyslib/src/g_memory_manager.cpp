
#include "common.h"

#ifdef GSYS_ENABLE_MEMORY_MANAGER

#include "../g_std_file.h"
#include "../g_file_util.h"

#include "../boost/filesystem/path.hpp"
#include <list>

using namespace gsys;
using namespace std;

namespace gsys
{
	namespace detail
	{
		struct memory_manager : public gsys::memory_manager_interface
		{
			~memory_manager();

			static void* GSYS_API _alloc(gsys_size_t size);
			static void  GSYS_API _free(void* pt);
			static void* GSYS_API _realloc(void* pt,size_t size);

			void* GSYS_API alloc(gsys_size_t size){return _alloc(size);};
			void  GSYS_API free(void* pt){_free(pt);};
			void* GSYS_API realloc(void* pt,size_t size){return _realloc(pt,size);};
		} g_memory_manager;
		memory_manager_interface* g_pmem=0;
	}
}

memory_manager_interface*  gsys::get_memory_manager()
{
	if(detail::g_pmem) return detail::g_pmem; 
	return &g_memory_manager;
}

void gsys::set_memory_manager(memory_manager_interface* pman)
{
	detail::g_pmem = pman;
}

void* GSYS_API  gsys::detail::gmem_alloc(gsys_size_t size)
{
	if(detail::g_pmem)return detail::g_pmem->alloc(size);
	else return detail::g_memory_manager._alloc(size);
}

void  GSYS_API  gsys::detail::gmem_free(void* pt)
{
	if(detail::g_pmem)detail::g_pmem->free(pt);
	else detail::g_memory_manager._free(pt);
}

void* GSYS_API  gsys::detail::gmem_realloc(void* pt,size_t size)
{
	if(detail::g_pmem)return detail::g_pmem->realloc(pt,size);
	else return detail::g_memory_manager._realloc(pt,size);
}

struct alcn
{
	uint size;
	uint n_alloc;
	uint n_free;
	int n_total;
	int n_max;
	int max_alloc;
};

bool operator <(alcn const& a,alcn const& b)
{
	return a.n_alloc > b.n_alloc;
}

#define NUMB (1024*4+10)
static alcn nn[NUMB+1];

static int g_total=0;
static int g_total_size=0;
static int g_max_size=0;

static bool g_breported=false;

static int inited= 0;
static int use_nn = 1;

static void init_nn()
{
	for(int i=0;i<NUMB+1;i++)
	{
		nn[i].size = i*4;
		nn[i].n_alloc=0;
		nn[i].n_free=0;
		nn[i].n_total=0;
		nn[i].n_max=0;
		nn[i].max_alloc=0;
	}

    nn[NUMB].size = 0xffffffff;
	inited=1;
}

#ifdef GSYS_ENABLE_MEMORY_LOG
static void report_mem()
{

	struct cnm{cnm(){use_nn = 0;}~cnm(){use_nn = 1;}}l_cnm;

	boost::filesystem::path p(gsys::GetAppStartupDir(),boost::filesystem::native);

	list<alcn> lst;

	for(int i=0;i<NUMB+1;i++)
	{
		if(nn[i].n_alloc>0)lst.push_back(nn[i]);
	}
	lst.sort();

	if(!lst.empty())
	{
		std_file file(p.native_file_string()+"/mem.txt","wt");
		if(!file)return;

		_foreach(list<alcn>,lst,i)
		{
			file.printf("%-4d  allocs: %-4d  frees: %-4d  left: %-4d  max: %-4d %4d\n",
				i->size,i->n_alloc,i->n_free,i->n_total,i->n_max, i->n_max*i->size);
		}
		file.printf("-----------------------------------------------------------------------------\n");
		file.printf(" left: %d  %d  max: %d \n",g_total,g_total_size,g_max_size);
	}
	g_breported = true;
}
#endif // GSYS_ENABLE_MEMORY_LOG

gsys::detail::memory_manager::~memory_manager()
{
#ifdef GSYS_ENABLE_MEMORY_LOG
	try
	{
		report_mem();
	}
	catch (...)
	{
	}
#endif //GSYS_ENABLE_MEMORY_LOG
}

#if 0
static void dbg_fill_mem(void* pt,uint sz)
{
	memset(pt,1,sz);
}
static void dbg_fill_mem(void* pt){dbg_fill_mem(pt,(uint)_msize(pt));}
#else
#	define dbg_fill_mem(x,y)
#	define dbg_fill_mem(x)
#endif

void* GSYS_API  gsys::detail::memory_manager::_alloc(gsys_size_t size)
{
#ifdef GSYS_ENABLE_MEMORY_LOG
	if(!inited) init_nn();

	if(use_nn)
	{

		g_total++;
		g_total_size+=(int)size;
		if(g_total_size > g_max_size) g_max_size = g_total_size;

		uint n = uint(size)/4;
		if(n > NUMB) n = NUMB;
		nn[n].n_alloc++;
		nn[n].n_total++;

		if(nn[n].n_total > nn[n].n_max) 
			nn[n].n_max = nn[n].n_total;
	}
#endif
	void* res = ::malloc(size);
	dbg_fill_mem(res,size);
	return res;
 }
 void GSYS_API gsys::detail::memory_manager::_free(void* pt)
 {
	if(!pt) return;
	GSYS_ASSERTPTR(pt);

#ifndef GSYS_ENABLE_MEMORY_LOG
	dbg_fill_mem(pt);
	::free(pt);
#else
	uint size;
	try
	{
		size = (uint)_msize(pt);
		::free(pt);
	}
	catch(...)
	{
		GSYS_ASSERT(0);
		// Memory deallocation error
		return;
	}

	if(use_nn)
	{
		g_total--;
		g_total_size-=size;

		uint n = size/4;
		if(n > NUMB) n = NUMB;
		nn[n].n_free++;
		nn[n].n_total--;

		if(nn[n].n_total == 0 && g_breported) report_mem();
	}
#endif
 }
 void* GSYS_API gsys::detail::memory_manager::_realloc(void* pt,size_t size)
 {
	 // this is error here 
	 //GSYS_ASSERT(false);

	 return ::realloc(pt,size);
 }
#else // GSYS_ENABLE_MEMORY_MANAGER

void* GSYS_API gsys::galloc(gsys_size_t size){return malloc((size_t)size);}
void  GSYS_API gsys::gfree(void* pt){free(pt);}
void* GSYS_API gsys::grealloc(void* pt,size_t size){return realloc(pt,size);}

#endif // GSYS_ENABLE_MEMORY_MANAGER

