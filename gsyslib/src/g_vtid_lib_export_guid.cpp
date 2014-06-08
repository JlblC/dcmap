#include "common.h"
#include "guiddef.h"
#include "boost/format.hpp"
#ifndef __BORLANDC__

#include "../g_vtid_lib.h"
#include "../g_vtid_util.h"
#include "../g_gid.h"

	std::istream& operator>>(std::istream& is,GUID & v)
	{
		GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
		is >> wsp_scip;
		if(is.peek() == '{')
		{
			dword v1;
			word v2,v3,v4;
			qword v5;
			wsp_delim D_BO('{');wsp_delim D_BC('}');wsp_delim D_D('-');
			is>>D_BO>>ISN_HEX(v1)>>D_D>>ISN_HEX(v2)>>D_D>>ISN_HEX(v3)>>D_D>>ISN_HEX(v4)>>D_D>>ISN_HEX(v5)>>D_BC;

			v.Data1 = v1;
			v.Data2=v2;
			v.Data3=v3;
			v5 += (qword(v4)<<48);
			for(int i=0;i<8;i++)
				v.Data4[i] = ((gsys_byte*)&v5)[7-i];
		}
		else
		{
			//is.setstate()
		}
		return is;
	}

	std::ostream& operator<<(std::ostream& os,GUID const& v)
	{
		GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
		
		dword v1 = v.Data1;
		word v2 = v.Data2;
		word v3 = v.Data3;
		qword v5;
		for(int i=0;i<8;i++)
			((gsys_byte*)&v5)[7-i] = v.Data4[i];
		word v4 = v5&(qword(0xffff)<<48) >> 48;

		string tstr = (boost::format("{%xd-%xd-%xd-%xd-%xd")%v1%v2%v3%v4%v5).str();

		return os << boost::format("{%xd-%xd-%xd-%xd-%xd")%v1%v2%v3%v4%v5;		
	}

namespace gsys 
{	
	binostream& operator<<(binostream& os,GUID const& v)
	{GSYS_ASSERTREF(os);GSYS_ASSERTREF(v);
		return os.put_rawdata(v);
	}

	binistream& operator>>(binistream& is,GUID & v)
	{GSYS_ASSERTREF(is);GSYS_ASSERTREF(v);
		return is.get_rawdata(v);
	}


	void register_guid()
	{
		GSYS_VTID_BEGIN_DYNAMIC(GUID)
		{
			GSYS_VTID_TYPENAME("GUID")
				GSYS_VTID_CREATEBLE;
			GSYS_VTID_COPYABLE;
			GSYS_VTID_STRINGIZEBLE;
			GSYS_VTID_SERIALIZEBLE;
		}
		GSYS_VTID_END_DYNAMIC 
	}
}

#endif

