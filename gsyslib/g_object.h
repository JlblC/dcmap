#pragma once
#include "g_gsys.h"
#include <string>
#include <typeinfo>
#include <stdexcept>

// g_object - base class top level class
// allowed gsys::dynamic_downcast<>() for checked conversion 
// implemented by IMPLEMENT_DYNAMIC_DOWNCAST(parent) in class definition

// define base object for dynmic downcast
namespace gsys
{
	class bad_dynamic_downcast : public std::bad_cast
	{
	public:
		virtual ~bad_dynamic_downcast() throw()
		{
		}
	};

	namespace detail // actual underlying concrete exception type
	{
		template<typename Target, typename Source>
		class no_dynamic_downcast : public bad_dynamic_downcast
		{
		public:
			no_dynamic_downcast()
				: description(
					std::string() + "bad dynamic downcast: " +
					"source type could not be interpreted as target, Target=" +
					typeid(Target).name() + ", Source=" + typeid(Source).name())
			{
			}
			virtual ~no_dynamic_downcast() throw()
			{
			}
			virtual const char *what() const throw()
			{
				return description.c_str();
			}
		private:
			const std::string description; // static initialization fails on MSVC6
		};
	}

	class g_object
	{
	protected:
#ifdef _GSYS_G_OBJECT_DYNAMIC_DOWNCAST
		virtual void* _g_object_test_downcasting(const type_info& inf) 
		{
			if(inf == typeid(this)) return reinterpret_cast<void*>(this);
			else return 0;
		}
#endif
	public:
		g_object(){}
		virtual ~g_object(){}
#ifdef _GSYS_G_OBJECT_DYNAMIC_DOWNCAST
		template<class Target,class Source> 
		friend Target dynamic_downcast(Source s)
		{
			void* r = s->_g_object_test_downcasting(typeid(Target));
			if(!r)throw detail::no_dynamic_downcast<Target, Source>();
			return reinterpret_cast<Target>(r); 
		}
		template<class Target>
		bool HasType(){return bool(_g_object_test_downcasting(typeid(Target*)));}
#else
		template<class Target,class Source> 
		friend Target dynamic_downcast(Source s)
		{
			Target r = dyn_cast<Target>(s);
			if(!r)throw detail::no_dynamic_downcast<Target, Source>();
			return r; 
		}
		template<class Target>
		bool HasType(){return bool(gsys::dyn_cast<Target*>(this));}
#endif // _GSYS_G_OBJECT_DYNAMIC_DOWNCAST
	};

}

#ifdef _GSYS_G_OBJECT_DYNAMIC_DOWNCAST
	#define IMPLEMENT_DYNAMIC_DOWNCAST(parent) \
		virtual void* _g_object_test_downcasting(type_info const& inf) { \
			if(inf == typeid(this)) return reinterpret_cast<void*>(this); \
			else return parent::_g_object_test_downcasting(inf);}
#else
	#define IMPLEMENT_DYNAMIC_DOWNCAST(parent)
#endif


