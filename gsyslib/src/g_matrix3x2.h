
#pragma once

#include "g_gsys.h"
#include "g_alg.h"
#include "g_vector2.h"
#include "g_text.h"

#pragma warning(push)

// '=' : conversion from 'double' to 'gsys::matrix4::value_type', possible loss of data
#pragma warning(disable : 4244 ) 


namespace gsys
{
	struct matrix3x2
	{
/*====================================================================================*\
		types
\*====================================================================================*/
		typedef gsys_real value_type;
		typedef uint size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS
/*====================================================================================*\
		Data access
\*====================================================================================*/
		union
		{
			struct
			{
				value_type _11, _12, _13;
				value_type _21, _22, _23;
			};
			value_type m[3][2];
			value_type values[3*2];
		};
        reference operator()(size_type x,size_type y){return m[x][y];}
        reference operator[](size_type x){GSYS_ASSERT(x<3*2);return values[x];}
        const_reference operator()(size_type x,size_type y)const{return m[x][y];}
        const_reference operator[](size_type x)const{GSYS_ASSERT(x<3*2);return values[x];}
		size_type size()const{return 3*2;}
	};

	vector2 _inline operator * (vector2 const& vec, matrix3x2 const& mat)
	{
		matrix3x4::value_type x = mat._11*vec.x + mat._21*vec.y +  mat._31;
		matrix3x4::value_type y = mat._12*vec.x + mat._22*vec.y +  mat._32;
		return vector2(x,y);
	}

}



