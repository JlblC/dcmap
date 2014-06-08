#pragma once
#ifndef G_FIXED_VECTOR_H
#define G_FIXED_VECTOR_H

#include "g_gsys.h"
#include "g_assert.h"

namespace gsys
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

	};

	template<typename Type>
	class object_creator : public object_creator_base<Type>
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
	class raw_object_creator : public object_creator_base<Type>
	{
	public:
		void static construct(pointer _Ptr, const value_type& _Val)
		{
			*_Ptr = _Val;
		}
		void static construct(pointer _Ptr){}
		void static destroy(pointer _Ptr){}
	};


	template<typename T,int size>
	{
		
	}
}

#endif //G_FIXED_VECTOR_H
