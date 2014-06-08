#pragma once
#ifndef G_BASE_MATRIX_H
#define G_BASE_MATRIX_H

#include "g_gsys.h"
#include "g_matrix_operations.h"

namespace gsys
{
	template <typename T,int w,int h> 
	struct base_matrix
	{
		typedef T value_type;
		typedef uint size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS

	};


}

#endif