#pragma once
#include "g_alg.h"
//#include <boost/operators.hpp>

namespace gsys
{
	template<class T>
	class descend_iterator
	{
	public:
		typename T::iterator typedef outer_iterator;
		typename T::value_type::iterator typedef inner_iterator;
	private:
		outer_iterator ii;
		outer_iterator ii_end;

		inner_iterator oi;
		inner_iterator oi_end;
	public:
		descend_iterator(T & val)
		{
			oi = val.begin();
			ii_end = val.end();
		}

	}
}
