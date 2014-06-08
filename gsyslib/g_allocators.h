#ifndef __INCLUDED_g_allocators_h__
#define __INCLUDED_g_allocators_h__
#pragma once

#include "g_gsys.h"
#include "g_assert.h"

namespace gsys
{
	namespace detail
	{
		template<typename Type>
		class object_creator_base
		{
		public:
			typedef Type value_type;
			typedef value_type* pointer;
			typedef value_type const* const_pointer;
			typedef value_type& reference;
			typedef value_type const& const_reference;

			typedef gsys_size_t size_type;
			typedef gsys_pt_diff difference_type;

			// return address of mutable _Val
			pointer static address(reference _Val){return (&_Val);}
			// return address of nonmutable _Val
			const_pointer static address(const_reference _Val){return (&_Val);}
		};
	}

	template<typename Type>
	class object_creator : public detail::object_creator_base<Type>
	{
	public:
		void static construct(pointer _Ptr, const value_type& _Val)
		{
			new ((void*)_Ptr) Type(_Val);
		}

		void static construct(pointer _Ptr)
		{
			new ((void*)_Ptr) Type();
		}

		void static destroy(pointer _Ptr)
		{
			_Ptr->~Type()
		}
	};

	template<typename Type>
	class raw_object_creator : public detail::object_creator_base<Type>
	{
	public:
		void static construct(pointer _Ptr, const value_type& _Val)
		{
			*_Ptr = _Val;
		}
		void static construct(pointer _Ptr){}
		void static destroy(pointer _Ptr){}
	};
}

#endif // __INCLUDED_g_allocators_h__

