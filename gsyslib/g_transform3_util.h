#pragma once
#ifndef G_TRANSFORM3_UNIL_H
#define G_TRANSFORM3_UNIL_H

#include "g_gsys.h"
#include "g_vector3.h"
#include "g_quaternion.h"

namespace gsys
{
	// Transform 2 quaterions with constant angle speed
	// return remainding angle between quaterions.
	inline float slerp_const_speed(quaternion const& q1,
						quaternion const& q2,
						gsys_real fTime,gsys_real fSpeed,
						quaternion & r)
	{
		vector3 d1 = vector3_UNIT_Y * q1;
		vector3 d2 = vector3_UNIT_Y * q2;

		gsys_real a = angle(d1,d2);

        if(a==0)
        {
            r = q2;
            return 0;
        }

		gsys_real fMod = fTime/a*fSpeed;
		if(fequ(a,0,1E-4f) || fMod >= 1.0) 
		{
			r = q2;
			a = 0;
		}
		else
		{
			r = slerp(q1,q2,fMod);
		}
		return a;
	}
};

#endif //G_TRANSFORM3_UNIL_H
