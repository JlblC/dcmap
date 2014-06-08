#pragma once
#include "g_gsys.h"
#include "g_binstream.h"
#include "g_text.h"

namespace gsys
{

/////////////////////////////////////////////////////////////////////////////////////// 
	struct ID
	{
	private:
		union
		{
			gsys_dword m_dwA;
			gsys_dword m_dwData;
		};
	public:
		ID(){m_dwA=dword_MAX;}
		ID(gsys_dword a){m_dwA=a;}
		dword GetData()const {return m_dwData;}
		bool operator == (const ID &idClass)const{return m_dwData ==  idClass.m_dwData;}
		bool operator != (const ID &idClass)const{return m_dwData !=  idClass.m_dwData;}
        bool operator <  (const ID &id)const{return id.m_dwA < m_dwA;}
        bool operator >  (const ID &id)const{return id.m_dwA > m_dwA;}
        bool operator <=  (const ID &id)const{return id.m_dwA <= m_dwA;}
        bool operator >=  (const ID &id)const{return id.m_dwA >= m_dwA;}

		bool IsValid()const{return m_dwA != dword_MAX;}
		bool valid()const{return IsValid();}
		void MakeInvalid(){m_dwA=dword_MAX;}
		void MakeUndefined(){m_dwA=0;}
		bool IsDefined()const{return m_dwA != 0;}
		ID operator  ++() {return ++m_dwData;}
		ID operator  ++(int) {return m_dwData++;}
        operator gsys_dword()const{return m_dwA;}

		// Load & Save
		friend inline std::ostream& operator<<(std::ostream& os,ID const& v)
		{return os << v.m_dwA;}
		friend inline std::istream& operator>>(std::istream& is,ID & v)
		{return is >> wsp_scip >> v.m_dwA;}
		friend inline binostream& operator<<(binostream& os,ID const& v){return os.put_rawdata(v);}
		friend inline binistream& operator>>(binistream& is,ID & v){return is.get_rawdata(v);}
	};
    const ID InvalidID(dword_MAX);

//	GSYS_SET_TYPENAME(ID,"ID");

/////////////////////////////////////////////////////////////////////////////////////// 
	struct GID
	{
	private:
		union
		{
			struct 
			{
				dword m_dwA;
				dword m_dwB;
			};
			qword m_qwData;
		};
	public:
		GID(){MakeInvalid();}
		GID(dword a,dword b):m_dwA(a),m_dwB(b){}
		GID(dword a):m_dwA(a),m_dwB(dword_MAX){}
		GID(qword data):m_qwData(data){}
		GID(ID id):m_dwA(id.GetData()),m_dwB(dword_MAX){}

		GID(GID const& id):m_qwData(id.m_qwData){}
		GID & operator = (GID const& id){m_qwData = id.m_qwData;return *this;}

		bool operator == (const GID &idClass)const{return m_qwData ==  idClass.m_qwData;}
		bool operator != (const GID &idClass)const{return m_qwData !=  idClass.m_qwData;}
        bool operator > (const GID &idClass)const{return m_qwData >  idClass.m_qwData;}   
        bool operator < (const GID &idClass)const{return m_qwData <  idClass.m_qwData;}   
        bool operator >= (const GID &idClass)const{return m_qwData >=  idClass.m_qwData;}   
        bool operator <= (const GID &idClass)const{return m_qwData <=  idClass.m_qwData;}   


		bool IsValid()const{return m_dwA != dword_MAX || m_dwB != dword_MAX;}
		void MakeInvalid(){m_dwA=m_dwB=dword_MAX;}
		void MakeUndefined(){m_dwA=m_dwB=0;}
		qword GetData(){return m_qwData;}
		bool IsDefined()const{return m_dwA != 0 && m_dwB != 0;}

		static GID InvalidGID(){return GID(dword_MAX,dword_MAX);}
		static GID GeneralGID(){return GID(0x00000001ul,0x00000001ul);}

		GID operator  ++() {return GID(++m_qwData);}
		GID& operator  ++(int) {m_qwData++;return *this;}

		// Load & Save
		/*
		friend inline std::ostream& operator<<(std::ostream& os,GID const& v)
		{return os << "{" << v.m_dwA << "," << v.m_dwB << "}";}
		friend inline std::istream& operator>>(std::istream& is,GID & v)
		{return is >> wsp_delim('{') >> v.m_dwA >> wsp_delim(',') >> v.m_dwB >> wsp_delim('}');}
		friend inline binostream& operator<<(binostream& os,GID const& v){return os.put_rawdata(v);}
		friend inline binistream& operator>>(binistream& is,GID & v){return is.get_rawdata(v);}
		*/

		GSYSDETAIL_OSTREAM_BASIC_OBJ(GID,os << "{" << v.m_dwA << "," << v.m_dwB << "}")

		GSYSDETAIL_ISTREAM_STD_BASIC_OBJ(GID,DO >> v.m_dwA >> D >> v.m_dwB >> DC)

		GSYSDETAIL_FRIEND_BINSTREAM_RAWDATA(GID)

	};

	GSYS_SET_TYPENAME(ID,"GID");


    const GID InvalidGID(dword_MAX,dword_MAX);
    const GID GeneralGID(0x00000001ul,0x00000001ul);

   const GID gid_INVALID(dword_MAX,dword_MAX);
   const GID gid_GENERAL(0x00000001ul,0x00000001ul);
   const GID gid_MAX(0xfffffffeul,0xfffffffeul);
   const GID gid_BASE(0x000000fful,0x000000fful);

	// load & save

/////////////////////////////////////////////////////////////////////////////////////// 
}
