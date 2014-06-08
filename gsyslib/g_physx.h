#pragma once
#include "g_gsys.h"

#ifdef GSYS_WINDOWS
#pragma comment(lib,"PhysXLoader.lib")
#endif

#include "g_vector3.h"
#include "g_quaternion.h"
#include "g_matrix3x4.h"
#include "g_matrix3.h"

#include "NxPhysics.h"
#include "NxCooking.h"
#include "NxStream.h"


namespace gsys
{
	/*struct CTransform
	{
		gsys::matrix3t rot;
		vector3 pos;
	};
	*/

	namespace PhysX
	{
		#define GSYS_DETAIL_DEF_NXC(my,nx)\
			inline my& nxc(nx& v){return *reinterpret_cast<my*>(&v);}\
			inline my const& nxc(nx const& v){return *reinterpret_cast<my const*>(&v);}\
			inline nx& nxc(my& v){return *reinterpret_cast<nx*>(&v);}\
			inline nx const& nxc(my const& v){return *reinterpret_cast<nx const*>(&v);};

			GSYS_DETAIL_DEF_NXC(::gsys::vector3,NxVec3);
			GSYS_DETAIL_DEF_NXC(::gsys::quaternion,NxQuat);
			GSYS_DETAIL_DEF_NXC(::gsys::matrix3tx4,NxMat34);
			GSYS_DETAIL_DEF_NXC(::gsys::matrix3t,NxMat33);

		struct NxMemStream : public NxStream
		{
			gsys::mem_binostream os;
			mutable gsys::mem_binistream is;

			NxMemStream():is((void*)gsys_null,1){};
			NxMemStream(gsys_byte const* pdata,uint size):is(pdata,size){};

			virtual     NxU8            readByte()                              const{NxU8 v;is>>v;return v;}
			virtual     NxU16           readWord()                              const{NxU16 v;is>>v;return v;}
			virtual     NxU32           readDword()                             const{NxU32 v;is>>v;return v;}
			virtual     float           readFloat()                             const{float v;is>>v;return v;}
			virtual     double          readDouble()                            const{double v;is>>v;return v;}
			virtual     void            readBuffer(void* buffer, NxU32 size)    const
			{
				is.get_rawdata((gsys_byte*)buffer,size);
			}

			virtual     NxStream&       storeByte(NxU8 b){os << b;return *this;};
			virtual     NxStream&       storeWord(NxU16 w){os << w;return *this;};
			virtual     NxStream&       storeDword(NxU32 d){os << d;return *this;};
			virtual     NxStream&       storeFloat(NxReal f){os << f;return *this;};
			virtual     NxStream&       storeDouble(NxF64 f){os << f;return *this;};
			virtual     NxStream&       storeBuffer(const void* buffer, NxU32 size)
			{
				os.put_rawdata((gsys_byte const*)buffer,size);
				return *this;
			};
		};

		struct NxMemInStream : public NxStream
		{

		};	
	}
}



