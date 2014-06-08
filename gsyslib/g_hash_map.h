#pragma once
#ifndef G_HASH_MAP
#define G_HASH_MAP
#include "g_gsys.h"
#include "g_text.h"

#include <map>
#include <set>

#ifdef GSYS_DETAIL_HAS_STDEXT_HASH_MAP
#	include <hash_map>
#	include <hash_set>
#endif

#if 0
	namespace stdext
	{
		using namespace std;

		template<class Key, class Traits = less<Key> >
		class hash_compare
		{
		};

		template <
		class Key,
		class Type,
		class Traits=hash_compare<Key, less<Key> >,
		class Allocator=allocator<pair <const Key, Type> >
		>
		class hash_map : public map<Key,Type,Allocator>
		{

		};
	}
#endif

namespace gsys
{
	template <typename T>
	struct striless
	{
		bool operator()(T const& s1,T const&  s2)const
		{
			return basic_char_classify<typename T::value_type>::compare_i(s1.c_str(),s2.c_str()) < 0;
		}
	};

	template <typename Key,typename Value>
	class strimap: public std::map<Key,Value,striless<Key> >
	{
	};

	template <typename Key,typename Value>
	class wstrimap: public std::map<Key,Value,striless<Key> >
	{
	};

	template <typename Key,typename Value>
	class strimultimap : public std::multimap<Key,Value,striless<Key> >
	{
	};

	template <typename Key>
	class striset: public std::set<Key,striless<Key> >
	{
	};
}



#endif
