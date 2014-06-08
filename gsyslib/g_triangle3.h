#pragma once
#ifndef G_TRIANGLE3_H
#define G_TRIANGLE3_H

#include "g_gsys.h"
#include "g_vector3.h"
#include "g_plane.h"

namespace gsys
{
	struct triangle3
	{
		typedef vector3 value_type;
		typedef gsys_unsigned size_type;
		GSYSDETAIL_TYPEDEF_VALUE_POINTERS;
		GSYSDETAIL_IMPLEMENT_ARRAY_CONTAINER;

		value_type v0;
		value_type v1;
		value_type v2;

		triangle3(){}

		triangle3(vector3 const& _v0,vector3 const& _v1,vector3 const& _v2):
		v0(_v0),v1(_v1),v2(_v2){}

		reference operator [](int i){gsys_assert(i<3);return (&v0)[i];}
		const_reference operator [](int i)const{gsys_assert(i<3);return (&v0)[i];}
		size_type size()const{return 3;}


		void set(vector3 const& _v0,vector3 const& _v1,vector3 const& _v2){v0=_v0;v1=_v1;v2=_v2;}

		void get_normal(vector3& n)const{n = cross(v1 - v2,v1 - v0).norm();}
		vector3 get_normal()const{return cross(v1 - v2,v1 - v0).norm();}

		void normal(vector3& n)const{n = cross(v1 - v2,v1 - v0).norm();}
		vector3 normal()const{return get_normal();}

		void get_plane(plane& n)const{n.init_vectors(v0,v1,v2);}
		plane get_plane()const{plane p;return p.init_vectors(v0,v1,v2);}

	};
}

#endif //G_TRIANGLE3_H
