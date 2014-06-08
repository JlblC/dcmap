#pragma once
#ifndef G_LINE3_H
#define G_LINE3_H

#include "g_gsys.h"
#include "g_vector3.h"
//#include "g_matrix4.h"

namespace gsys
{
	struct ray3
	{
		vector3 origin;
		vector3 direction;  

	};

	struct segment3
	{
		vector3 origin;
		vector3 destination;  

		segment3(){}
		segment3(vector3 vtStart,vector3 vtEnd):
			origin(vtStart),
			destination(vtEnd)
		{}
	};

	struct line3
	{
		vector3 origin;
		vector3 direction;  
	};

	gsys_real inline distance(line3 const& line,vector3& vt)
	{
		gsys_real sq =  cross(vt-line.origin,line.direction).lensquared()/line.direction.lensquared();
		return sqrtf(sq);
	}

}

#endif // G_LINE3_H
