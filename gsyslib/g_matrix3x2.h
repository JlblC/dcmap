
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
	struct matrix2x3
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
				value_type _11, _12;
				value_type _21, _22;
				value_type _31, _32;
			};
			value_type m[2][3];
			value_type values[3*2];
		};
        reference operator()(size_type x,size_type y){return m[x][y];}
        reference operator[](size_type x){GSYS_ASSERT(x<3*2);return values[x];}
        const_reference operator()(size_type x,size_type y)const{return m[x][y];}
        const_reference operator[](size_type x)const{GSYS_ASSERT(x<3*2);return values[x];}
		size_type size()const{return 3*2;}


/*====================================================================================*\
		Initialisations
\*====================================================================================*/

        matrix2x3& identity()
        {
            _11 = 1; _12 = 0;
            _21 = 0; _22 = 1; 
            _31 = 0; _32 = 0;
			return *this;
        }

		matrix2x3& rotation(value_type t)
		{
			_11 = cos(t); _12 = -sin(t); 
			_21 = sin(t); _22 = cos(t); 
			_31 = 0; _32 = 0;
			return *this;
		}
		matrix2x3& translation(vector2 const& v){return translation(v.x,v.y);}
        matrix2x3& translation(value_type x,value_type y)
        {
            _11 = 1; _12 = 0; 
            _21 = 0; _22 = 1; 
            _31 = x; _32 = y;
			return *this;
        }

		matrix2x3& scaling(vector2 const& v){return scaling(v.x,v.y);}
		matrix2x3& scaling(value_type x){return scaling(x,x);}
		matrix2x3& scaling(value_type x,value_type y)
        {
            _11 = x; _12 = 0;
            _21 = 0; _22 = y; 
            _31 = 0; _32 = 0; 
			return *this;
        }

/*====================================================================================*\
		Procuctions - bilds objects from matrix
\*====================================================================================*/

		matrix2x3& build_muliplication(matrix2x3 const& mat,matrix2x3 & res)const
		{
			 res._11 = _11*mat._11 + _12*mat._21;
			 res._12 = _11*mat._12 + _12*mat._22; 

			 res._21 = _21*mat._11 + _22*mat._21;
			 res._22 = _21*mat._12 + _22*mat._22;

			 res._31 = _31*mat._11 + _32*mat._21 + mat._31; 
			 res._32 = _31*mat._12 + _32*mat._22 + mat._32; 
			 return res;
		}

/*====================================================================================*\
		Operations
\*====================================================================================*/
		matrix2x3& multiply(matrix2x3 const& mat)
		{
			 matrix2x3 m;
			 build_muliplication(mat,m);
			 *this = m;
			 return *this;
		}
		// 
		matrix2x3 & operator *=(matrix2x3 const& mat)
		{
			 return multiply(mat);
		}

		matrix2x3& scale(value_type x,value_type y)
		{
			// TODO optimise
			matrix2x3 m;
			m.scaling(x,y);
			multiply(m);
			return *this;
		}

		matrix2x3& translate(vector2 const& v){return translate(v.x,v.y);}
		matrix2x3& translate(value_type x,value_type y)
		{
			_31 += x;
			_32 += y;
			return *this;
		}

		matrix2x3& rotate(value_type t)
		{
			matrix2x3 m;
			m.rotation(t);
			matrix2x3 r;
			build_muliplication(m,r);
			*this = r;
			return *this;
		}


	};
	GSYS_SET_TYPENAME(matrix2x3,"m23");

	vector2 inline transform(vector2 const& vec, matrix2x3 const& mat)
	{
		matrix2x3::value_type x = mat._11*vec.x + mat._21*vec.y +  mat._31;
		matrix2x3::value_type y = mat._12*vec.x + mat._22*vec.y +  mat._32;
		return vector2(x,y);
	}

	vector2 inline operator * (vector2 const& vec, matrix2x3 const& mat)
								{return transform(vec,mat);}



}



