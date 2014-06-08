#pragma once
#ifndef G_UTILITY_H
#define G_UTILITY_H


#include "g_gsys.h"

namespace gsys
{
	//  Private copy constructor and copy assignment ensure classes derived from
	//  class noncopyable cannot be copied.
	class noncopyable
	{
	protected:
		noncopyable() {}
		~noncopyable() {}
	private:  // emphasize the following members are private
		noncopyable( const noncopyable& );
		const noncopyable& operator=( const noncopyable& );
	};
	
	// version of dynamic cast
	template<class Target, class Source>
	Target dyn_cast(Source s)
	{
		return dynamic_cast<Target>(s);	
	}

	template<typename T,typename S> T&	reinterpret_ref_cast(S& ref){return *(reinterpret_cast<T*>(&ref));} 
	template<typename T,typename S> T const&	reinterpret_const_ref_cast(S const& ref){return *(reinterpret_cast<T const*>(&ref));} 

	//inline dword reinterpret_float2dword(float f){reinterpret_ref_cast<gsys_dword>(f);}
}

// for simplifying pointer typedefs
#define GSYSDETAIL_TYPEDEF_VALUE_POINTERS \
	typedef value_type& reference;\
	typedef value_type const& const_reference;\
	typedef value_type* pointer;\
	typedef value_type const* const_pointer;\

// define iterators and other for static array containers
#define GSYSDETAIL_IMPLEMENT_ARRAY_CONTAINER \
	typedef pointer iterator;\
	typedef const_pointer const_iterator;\
	iterator begin(){return &operator[](0);}\
	iterator end(){return begin()+size();}\
	const_iterator begin()const{return &operator[](0);}\
	const_iterator end()const{return begin()+size();}\
	reference front(){return operator[](0);}\
	reference back(){return operator[](size()-1);}\
	const_reference front()const{return operator[](0);}\
	const_reference back()const{return operator[](size()-1);}\
	pointer get(){return begin();}\
	const_pointer get()const{return begin();}\


#define GSYSDETAIL_IMPLEMENT_STATIC_ARRAY_CONTAINER(values,sz) \
	typedef pointer iterator;\
	typedef const_pointer const_iterator;\
	size_type size()const{return sz;}\
	reference operator [](size_type i){GSYS_CFN;GSYS_ASSERT(i<size());return values[i];}\
	const_reference operator [](size_type i)const{GSYS_CFN;GSYS_ASSERT(i<size());return values[i];}\
	iterator begin(){return &operator[](0);}\
	iterator end(){return begin()+size();}\
	const_iterator begin()const{return &operator[](0);}\
	const_iterator end()const{return begin()+size();}\
	reference front(){return operator[](0);}\
	reference back(){return operator[](size()-1);}\
	const_reference front()const{return operator[](0);}\
	const_reference back()const{return operator[](size()-1);}\
	pointer get(){return begin();}\
	const_pointer get()const{return begin();}\


// defines standart foreach stasment to represent stl compilant iterators
#define GSYS_DETAIL_FOREACH(it,x,i) for(it i=(x).begin(),__fe_end=(x).end();i!=__fe_end;++i)
#define GSYS_DETAIL_FOREACH_NOINC(it,x,i) for(it i=(x).begin();i!=(x).end();)

#define _foreach(t,x,i) GSYS_DETAIL_FOREACH(t::iterator,x,i)
#define _const_foreach(t,x,i) GSYS_DETAIL_FOREACH(t::const_iterator,x,i)

#define _foreach_erasable(t,x,i) \
		for(t::iterator i=(x).begin(); i!=(x).end();)\
		{ bool ___foreach_erasable_erased_##i = false; {

#define _erase_in_foreach(x,i) {___foreach_erasable_erased_##i = true;}

#define _end_foreach_erasable(x,i) } if(___foreach_erasable_erased_##i) i = (x).erase(i);else ++i; }

#define _foreach_noinc(t,x,i) GSYS_DETAIL_FOREACH_NOINC(t::iterator,x,i)
#define _const_foreach_noinc(t,x,i) GSYS_DETAIL_FOREACH_NOINC(t::const_iterator,x,i)

#define _foreach_array(x,i) for(int i=0;i<x.size();i++)

#endif
