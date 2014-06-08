#pragma once
#ifndef G_TRANSFORM2D_H
#define G_TRANSFORM2D_H

#include "g_gsys.h"
#include "g_math.h"

#include "g_vector3.h"
#include "g_quaternion.h"
//#include "g_transform_quat.h"
#include "g_transform3_util.h"

// Special transform state to keep & process from & to pos/orient in 2d space
namespace gsys
{

#define  GSYS_RAD2IANG(f) (iang_t(int((f)/_pi*0x7fff)))
#define  GSYS_DEG2IANG(f) (iang_t(int((f)/180.0f*0x7fff)))

	class iang_t : public boost::additive1<iang_t>
	{
		typedef gsys_short value_type; 
		value_type _a;
	public:
		iang_t(){}
		operator gsys_short(){return _a;}

		iang_t& operator += (iang_t b){_a += b._a;return *this;}
		iang_t& operator -= (iang_t b){_a -= b._a;return *this;}

		friend iang_t operator * (iang_t a,float f){return (value_type)(a._a*f);}

		float as_rad()const{return float(_a)/0x7fff*_pi;}

		gsys_short& as_short(){return _a;}
		gsys_short  as_short()const{return _a;}

		vector2& xform( vector2 const&v, vector2 &res)const
		{
			float c = cosf(-as_rad());
			float s = sinf(-as_rad());
			res.x = (c)*v.x + (s)*v.y;
			res.y = (-s)*v.x + (c)*v.y;
			return res;
		}
		vector2 xform(vector2 const& v)const{vector2 lv;return xform(v,lv);}

		vector2 &make_dir(vector2 &vt)const
		{
			vt.x = cosf(as_rad());
			vt.y = sinf(as_rad());
			return vt;
		}
		vector2 make_dir()const{vector2 vt;return make_dir(vt);}

#pragma region initialization

		iang_t(gsys_short a):_a(a){}
		iang_t& identity()
		{
			_a = 0;
			return *this;
		}

		iang_t& from_unitdir(vector2 const&dir)
		{
			float x=dir.x;
			if(gsys::abs(x)>1)x=gsys::sign(x);
			float ang = acosf(x);
			_a = GSYS_RAD2IANG(ang);
			if(dir.y<0)_a = -_a;
			return *this;
		}

		iang_t& from_dir(vector2 dir)
		{
			dir.norm();
			from_unitdir(dir);
			return *this;
		}

		iang_t& from_unitdirx(vector2 const&dir)
		{
			float x=dir.x;
			if(gsys::abs(x)>1)x=gsys::sign(x);
			float ang = acosf(x);
			_a = GSYS_RAD2IANG(ang);
			if(dir.y<0)_a = -_a;
			_a -= GSYS_DEG2IANG(90);
			return *this;
		}

		iang_t& from_dirx(vector2 dir)
		{
			dir.norm();
			from_unitdirx(dir);
			return *this;
		}

#pragma endregion initialization
		GSYSDETAIL_OSTREAM_STD_BASIC_OBJ(iang_t,v._a);
		GSYSDETAIL_ISTREAM_STD_BASIC_OBJ(iang_t,ISN(v._a));
		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(iang_t);
	};

	// TODO: Optimize using direct float transform
	inline float iang2f(iang_t a){return float(a)/0x7fff;}
	inline iang_t f2iang(float f){return iang_t(int(f*0x7fff));}

	inline iang_t deg2iang(float f){return iang_t(int(f/180.0f*0x7fff));}
	inline iang_t rad2iang(float f){return iang_t(int(f/_pi*0x7fff));}

	inline float iang2rad(iang_t a){return float(a)/0x7fff*_pi;}
	inline float iang2deg(iang_t a){return float(a)/0x7fff*180;}

	class transform2_iang
	{
	public:
		vector2    pos;
		iang_t     iang;
	protected:

	public:
		transform2_iang(){}
		transform2_iang(vector2 const& p,iang_t a):pos(p),iang(a){}
		transform2_iang& identity()
		{
			pos.set(0,0);
			iang=0;
			return *this;
		}

		transform2_iang& add_rotation(iang_t a)
		{
			iang += a;
			return *this;
		}

		vector2& xform( vector2 const&v, vector2 &res)const
		{
			iang.xform(v,res);
			res += pos;
			return res;
		}
		vector2 xform(vector2 const& v)const{vector2 lv;return xform(v,lv);}

		GSYSDETAIL_OSTREAM_STD_BASIC_OBJ(transform2_iang,DO<<v.pos<<D<<v.iang<<DC);
		GSYSDETAIL_ISTREAM_STD_BASIC_OBJ(transform2_iang,DO>>v.pos>>D>>v.iang>>DC);
		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(transform2_iang);
	};
	// transform vector3 
	inline transform2_iang operator * (transform2_iang const& t1,transform2_iang const& t2)
	{
		transform2_iang q;
		q.iang = t1.iang + t2.iang;
		q.pos = t1.pos + t1.iang.xform(t2.pos);
		return q;
	}

	class transform22d
	{
	public:
		vector2    pos1;
		vector2    pos2;

        iang_t  iang1;
        iang_t  iang2;

		transform22d& set1(transform2_iang const& tr)
		{
			pos1 = tr.pos;
			iang1 = tr.iang;
			return *this;
		}	

		transform22d& set2(transform2_iang const& tr)
		{
			pos2 = tr.pos;
			iang2 = tr.iang;
			return *this;
		}		
	protected:

	public:
		static void RecalulateTables();
	public:
		vector3& dest3(){return *reinterpret_cast<vector3*>(&pos2);}
		gsys_float* floats(){return reinterpret_cast<gsys_float*>(&pos1);}

		vector2& diff(vector2& vt)const{vt=pos2-pos1;return vt;}
		vector2  diff()const{return pos2-pos1;}
		float diff_lensq()const{return (pos2-pos1).lensquared();}

		iang_t dir_iang()const
		{
			// TODO: Optimize using precalculated acos table
			vector2 dir(pos2-pos1);
			dir.norm();
			float ang = acosf(dir.x);
			iang_t a = rad2iang(ang);
			if(dir.y<0)a = -a;
			return a;
		}



		GSYSDETAIL_OSTREAM_STD_BASIC_OBJ(transform22d,DO<<v.pos1<<D<<v.iang1<<v.pos2<<D<<v.iang2<<DC);
		GSYSDETAIL_ISTREAM_STD_BASIC_OBJ(transform22d,DO>>v.pos1>>D>>v.iang1>>v.pos2>>D>>v.iang2>>DC);
		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(transform22d);
	};
}

#define DEGANGLE_TO_WANGLE(f) ((gsys::gsys_word)((f/180.0f)*0xffff))


typedef gsys::transform2_iang posor;



#endif