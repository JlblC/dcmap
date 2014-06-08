#include "stdafx.h"
#include "BinReflection.h"
#include "utils.h"
#include "DCMapCore.h"

// simplify 
#define LexConvCall(fn,tp) {tp r;if(gsys::lex_convert(r,v))return fn(to,n,r);else return false;}


CBinReflection::CBinReflection(void)
{
	m_Head.DataSize=0;
	m_Head.Codepage=CP_UTF8;
	m_Head.RecordCount=0;
	m_Head.Reserved=0;
	m_pCLOB=0;
}

CBinReflection::~CBinReflection(void)
{
}

bool CBinReflection::WriteToFile(gsys::gfile* file)
{
	m_Head.RecordCount = m_Records.size();

	*file << m_Head;

	_foreach(VecRecords,m_Records,it)
	{
		*file << it->data;
	}
	return true;
}

bool CBinReflection::ReadFromFile(gsys::gfile* file,int DBVer)
{
	int sz;

	m_Records.clear();

	if(DBVer < 2)
	{
		*file >> m_Head.DataSize;
		*file >> m_Head.RecordCount;
		m_Head.Codepage = 1251;
	}
	else
	{
		*file >> m_Head;
	}

	if(m_Head.RecordCount > 1024)return false;
	
	m_Records.reserve(m_Head.RecordCount);

	for(int i=0;i<m_Head.RecordCount;i++)
	{
		BRRecord rec;
		if(DBVer < 4)
		{
			memset(&rec.data,0,sizeof(rec.data));

			struct BRRecordDataV3{
				dcmapWORD iOffset;
				dcmapWORD iSize;
				dcmapWORD iParams;
				dcmapDWORD iDataType;
				char czName[24];
			};

			BRRecordDataV3 recdata;
			*file >> recdata;

			rec.data.iDataType = recdata.iDataType;
			rec.data.iOffset = recdata.iOffset;
			rec.data.iSize = recdata.iSize;
			rec.data.iParams = recdata.iParams;

			strncpy(rec.data.czName,recdata.czName,24);
		}
		else
		{
			*file >> rec.data;
		}
		m_Records.push_back(rec);
		if(rec.czName[0])
			m_mapRecords[rec.czName] = m_Records.size()-1;
	}
	return true;
}

bool CBinReflection::operator == (CBinReflection const& br)
{
	m_Head.RecordCount = m_Records.size();
	if(!(br.m_Head == m_Head))return false;
	_foreach(VecRecords,m_Records,it1)
	{
		MapRecords::const_iterator it2 = br.m_mapRecords.find(it1->czName);
		if(it2 == br.m_mapRecords.end())return false;
		if(br.m_Records[it2->second] != *it1)return false;
	}
	return true;
}

bool CBinReflection::MakeCopyOf(dcmapBYTE* pResult,dcmapBYTE const* pFrom,CBinReflection const& brFrom)
{
	for(int i=0;i<brFrom.m_Records.size();i++)
	{
		MapRecords::const_iterator it2 = m_mapRecords.find(brFrom.m_Records[i].czName);
		if(it2 == m_mapRecords.end())continue;

		int from = i;
		int to = it2->second;

		BRRecord const& br_to = m_Records[to];
		BRRecord const& br_from = brFrom.m_Records[from];
		
		if(br_to.iDataType != DCMDT_Flag && br_to.iDataType == br_from.iDataType &&  br_to.iSize == br_from.iSize && br_to.iParams == br_from.iParams)
		{
			memcpy(pResult+br_to.iOffset,pFrom+br_from.iOffset,br_to.iSize); // copy raw data
		}
		else if(br_to.iDataType == DCMDT_String &&	br_from.iDataType == DCMDT_String) // copy string
		{
			strncpy((char*)(pResult+br_to.iOffset),
				(char const*)(pFrom+br_from.iOffset),br_to.iSize-1);
		}
		else if(br_to.iDataType == DCMDT_WideString &&	br_from.iDataType == DCMDT_WideString) // copy string
		{
			wcsncpy((dcmapDCHAR*)(pResult+br_to.iOffset),
				(dcmapDCHAR const*)(pFrom+br_from.iOffset),(br_to.iSize/sizeof(dcmapDCHAR))-1);
		}
		else if(br_to.iDataType == DCMDT_WideString &&	br_from.iDataType == DCMDT_String) // copy convert string
		{
			wstring str = ToWide((char const*)(pFrom+br_from.iOffset),brFrom.m_Head.Codepage);
			wcsncpy((dcmapDCHAR*)(pResult+br_to.iOffset),
				    str.c_str(),(br_to.iSize/sizeof(dcmapDCHAR))-1);
		}
		else if(br_to.iDataType == DCMDT_Flag && br_from.iDataType == DCMDT_Flag) // copy flag
		{
			SetBoolean(pResult,to,brFrom.GetBoolean(pFrom,from));
		}
		else if(br_from.iDataType == DCMDT_Unknown &&  br_to.iSize == br_from.iSize )
		{
			memcpy(pResult+br_to.iOffset,pFrom+br_from.iOffset,br_to.iSize); // copy raw data
		}
		else // need some data conversion
		{
			dstring wstr = brFrom.GetStringW(pFrom,from);
			if(SetString(pResult,to,wstr.c_str()))
			{

			}
			else
			{
				// do nothing - data cannot be converted
				//return false;
			}
		}

	}	
	return true;
}

template<typename T> T& conv(void const* from, dcmapWORD Offset)
{
	return *(T*)(((dcmapBYTE const*)from)+Offset);
}

template<typename T> T& conv(void* from, dcmapWORD Offset)
{
	return *(T*)(((dcmapBYTE const*)from)+Offset);
}

int CBinReflection::GetInteger(void const* from,int n)const
{
	if(!from)return 0;
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;

	if(rec->iDataType == DCMDT_Integer)
	{
		if(rec->iSize == 1)	        return conv<char>(from,rec->iOffset);
		else if(rec->iSize == 2)	return conv<short>(from,rec->iOffset);
		else if(rec->iSize == 4)	return conv<int>(from,rec->iOffset);
		else if(rec->iSize == 8)	return conv<dcmapINT64>(from,rec->iOffset);
	}
	else if(rec->iDataType == DCMDT_Unsigned) return GetUnsigned(from,n);
	return GetReal(from,n);
}

dcmapINT64 CBinReflection::GetInt64(void const* from,int n)const
{
	if(!from)return 0;
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;

	if(rec->iDataType == DCMDT_Integer)
	{
		if(rec->iSize == 1)	        return conv<char>(from,rec->iOffset);
		else if(rec->iSize == 2)	return conv<short>(from,rec->iOffset);
		else if(rec->iSize == 4)	return conv<int>(from,rec->iOffset);
		else if(rec->iSize == 8)	return conv<dcmapINT64>(from,rec->iOffset);
	}
	else if(rec->iDataType == DCMDT_Unsigned) return GetUnsigned64(from,n);
	return 	GetReal(from,n);
}

dcmapUINT64 CBinReflection::GetUnsigned64(void const* from,int n)const
{
	if(!from)return 0;
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;
	/*
	if(!rec || !(rec->iDataType == DCMDT_Integer || rec->iDataType == DCMDT_Unsigned))return 0;
	*/

	if(rec->iDataType == DCMDT_Unsigned)
	{
		if(rec->iSize == 1)	        return conv<unsigned char>(from,rec->iOffset);
		else if(rec->iSize == 2)	return conv<unsigned short>(from,rec->iOffset);
		else if(rec->iSize == 4)	return conv<unsigned int>(from,rec->iOffset);
		else if(rec->iSize == 8)	return conv<dcmapUINT64>(from,rec->iOffset);
	}
	else if(rec->iDataType == DCMDT_Integer) return GetInt64(from,n);
	return 	GetReal(from,n);
}

unsigned CBinReflection::GetUnsigned(void const* from,int n)const
{
	if(!from)return 0;
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;
/*
	if(!rec || !(rec->iDataType == DCMDT_Integer || rec->iDataType == DCMDT_Unsigned))return 0;
*/

	if(rec->iDataType == DCMDT_Unsigned)
	{
		if(rec->iSize == 1)	        return conv<unsigned char>(from,rec->iOffset);
		else if(rec->iSize == 2)	return conv<unsigned short>(from,rec->iOffset);
		else if(rec->iSize == 4)	return conv<unsigned int>(from,rec->iOffset);
		else if(rec->iSize == 8)	return conv<dcmapUINT64>(from,rec->iOffset);
	}
	else if(rec->iDataType == DCMDT_Integer) return GetInteger(from,n);
	return 	GetReal(from,n);
}

dcmapREAL CBinReflection::GetReal(void const* from,int n)const
{
	if(!from)
		return 0;
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;
	dcmapREAL val=0;

	if(rec->iDataType == DCMDT_Real)
	{
		if(rec->iSize == 4) return conv<float>(from,rec->iOffset);
		else if(rec->iSize == 8) return conv<double>(from,rec->iOffset);
		else return 0;
	}
	if(rec->iDataType == DCMDT_Fixed)
	{
		int fmod = dcmapLOWORD(rec->iParams);
		int val;
		if(rec->iSize == 1)	        val= conv<char>(from,rec->iOffset);
		else if(rec->iSize == 2)	val= conv<short>(from,rec->iOffset);
		else if(rec->iSize == 4)	val= conv<int>(from,rec->iOffset);
		else if(rec->iSize == 8)	val= conv<dcmapINT64>(from,rec->iOffset);
		return val << fmod;
	}
	else if(rec->iDataType == DCMDT_Flag) return GetBoolean(from,n);
	else if(rec->iDataType == DCMDT_Unsigned) return GetUnsigned64(from,n);
	else if(rec->iDataType == DCMDT_Time) return GetUnsigned64(from,n);
	else if(rec->iDataType == DCMDT_Integer) return GetInt64(from,n);
	else if(rec->iDataType == DCMDT_String || rec->iDataType == DCMDT_CLOB)
	{
		if(gsys::lex_convert(val,GetString(from,n)))return val;
		return 0;
	}
	else if(rec->iDataType == DCMDT_WideString || rec->iDataType == DCMDT_WideCLOB)
	{
		if(gsys::lex_convert(val,GetStringW(from,n)))return val;
		return 0;
	}
	return 0;
}

char const* CBinReflection::GetString(void const* from,int n)const
{
	if(!from)return "";
	BRRecord const* rec = GetRecord(n);
	if(!rec)return "";
	static string convString;	
	
	if(rec->iDataType == DCMDT_String)
	{
		return ((char*)from)+rec->iOffset;
	}
	else if(rec->iDataType == DCMDT_CLOB && m_pCLOB)
	{
		dcmapDBREF const& ref = *GetReference(from,n);
		return m_pCLOB->GetValue(ref);
	}
	else if(rec->iDataType == DCMDT_Integer)
	{
		dcmapINT64 val = GetInt64(from,n);
		convString = gsys::to_str(val);
	}
	else if(rec->iDataType == DCMDT_Unsigned || rec->iDataType == DCMDT_Time)
	{
		dcmapUINT64 val = GetUnsigned64(from,n);
		convString = gsys::to_str(val);
	}
	else if(rec->iDataType == DCMDT_WideString)
	{
		convString = StrD2C((dcmapDCHAR*)(((char*)from)+rec->iOffset));
	}	
	else if(rec->iDataType == DCMDT_WideCLOB && m_pCLOB)
	{
		dcmapDBREF const& ref = *GetReference(from,n);
		convString = StrD2C(m_pCLOB->GetValueW(ref));
	}	
	else if(rec->iDataType == DCMDT_Reference)
	{
		if(!RecordLinkagePtr(n))return false;
		convString = RecordLinkagePtr(n)->FetchName(*GetReference(from,n));
		return convString.c_str();
	}
	else
	{
		dcmapREAL val = GetReal(from,n);
		convString = gsys::to_str(val);
	}	
	return convString.c_str();
}

dcmapDCSTR CBinReflection::GetStringW(void const* from,int n)const
{
	static dstring convString;

	if(!from)return dcmapDT("");

	BRRecord const* rec = GetRecord(n);
	if(!rec)return dcmapDT("");
	if(rec->iDataType == DCMDT_WideString)
		return (dcmapDCHAR*)(((char*)from)+rec->iOffset);

	else if(rec->iDataType == DCMDT_WideCLOB && m_pCLOB)
	{
		dcmapDBREF const& ref = *GetReference(from,n);
		return m_pCLOB->GetValueW(ref);
	}
	else if(rec->iDataType == DCMDT_Reference)
	{
		if(!RecordLinkagePtr(n))return dcmapDT("");
		convString = RecordLinkagePtr(n)->FetchWideName(*GetReference(from,n));
		return convString.c_str();
	}
	convString = ToDStr(GetString(from,n));
	return convString.c_str();
}

dcmapDBREF const* CBinReflection::GetReference(void const * from,int n)const
{
	static const dcmapDBREF inv_ref;
	if(!from)return &inv_ref;
	BRRecord const* rec = GetRecord(n);
	if(!rec)return &inv_ref;
	
	if(rec->iDataType != DCMDT_Reference && rec->iDataType != DCMDT_CLOB && rec->iDataType != DCMDT_WideCLOB && rec->iDataType != DCMDT_BLOB)return &inv_ref;
	return &conv<dcmapDBREF>(from,rec->iOffset);
}

template<class D>
bool GetFlag(CBinReflection::BRRecord const* rec,void const* from)
{
	D r = *(D*)(((dcmapBYTE*)from)+rec->iOffset);
	r = r >> rec->iParams;
	return r & 0x01;
}

bool CBinReflection::GetBoolean(void const* from,int n)const
{
	if(!from)return false;
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;
	if(rec->iDataType == DCMDT_Flag)
	{
		switch(rec->iSize)
		{
		case 4:
			return GetFlag<dcmapDWORD>(rec,from);	
		case 2:
			return GetFlag<dcmapWORD>(rec,from);	
		case 1:
			return GetFlag<dcmapBYTE>(rec,from);	
		case 8:
			return GetFlag<dcmapQWORD>(rec,from);	
		}
		dcmapDWORD r = *(dcmapDWORD*)(((dcmapBYTE*)from)+rec->iOffset);
		r = r >> rec->iParams;
		return r & 0x01;
	}
	else return GetInteger(from,n);
}

template<class D>
void SetFlag(CBinReflection::BRRecord const* rec,void* to,bool v)
{
	D& r = *(D*)(((dcmapBYTE*)to)+rec->iOffset);
	D msk = 0x01 << rec->iParams;
	if(v)r |= msk;
	else r &= ~msk;
}

bool CBinReflection::SetBoolean(void* to,int n,bool v)const
{
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;
	if(rec->iDataType == DCMDT_Flag)
	{
		switch(rec->iSize)
		{
		case 4:
			SetFlag<dcmapDWORD>(rec,to,v);
			return true;			
		case 2:
			SetFlag<dcmapWORD>(rec,to,v);
			return true;			
		case 1:
			SetFlag<dcmapBYTE>(rec,to,v);
			return true;			
		case 8:
			SetFlag<dcmapQWORD>(rec,to,v);
			return true;	
		default:
			return false;
		}
	}
	else return SetInteger(to,n,v);
}

bool CBinReflection::SetString(void* to,int n,char const* v)const
{
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;

	if(rec->iDataType == DCMDT_CLOB && m_pCLOB)
	{
		dcmapDBREF const& ref = *GetReference(to,n);
		if(!v || !v[0])
			m_pCLOB->Delete(ref);
		else if(!m_pCLOB->UpdateValue(ref,v))
			SetReference(to,n,*m_pCLOB->InsertValue(v));
		return true;
	}
	else if(rec->iDataType == DCMDT_String)
	{
		char* r = ((char*)to)+rec->iOffset;
		strncpy(r,v,rec->iSize);
		r[rec->iSize-1]=0;
		return true;
	}
	else if(rec->iDataType == DCMDT_WideString || rec->iDataType == DCMDT_WideCLOB)
	{
		SetString(to,n,ToDStr(v).c_str());
	}
	else if(rec->iDataType == DCMDT_Reference)
	{
		if(!RecordLinkagePtr(n))return false;
		dcmapDBREF ref;
		if(!RecordLinkagePtr(n)->RetrieveByName(v,&ref))
		{
			ref = *GetReference(to,n);
			if(!RecordLinkagePtr(n)->SelectID(ref))
				RecordLinkagePtr(n)->Insert();

			RecordLinkagePtr(n)->SetName(v);
			RecordLinkagePtr(n)->Post(&ref);
		}
		return SetReference(to,n,ref);
	}
	else if(rec->iDataType == DCMDT_Unknown) return false;
	else 
	{
		dcmapREAL r;
		if(gsys::lex_convert(r,v))
		{
			return SetReal(to,n,r);
		}
	}
	return false;
}

bool CBinReflection::SetString(void* to,int n,dcmapDCSTR v)const
{
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;

	switch(rec->iDataType)
	{
	case DCMDT_WideCLOB:
		if(m_pCLOB)
		{
			dcmapDBREF const& ref = *GetReference(to,n);
			if(!v || !v[0])
				m_pCLOB->Delete(ref);
			else if(!m_pCLOB->UpdateValue(ref,v))
				SetReference(to,n,*m_pCLOB->InsertValue(v));
			return true;
		}
		else return false;
	case DCMDT_WideString:
		{
			dcmapDCHAR* r = (dcmapDCHAR*)(((char*)to)+rec->iOffset);
			int sz = rec->iSize/sizeof(dcmapDCHAR);
			dcsncpy(r,v,sz);
			r[sz-1]=0;
			return true;
		}
	case DCMDT_Reference:
		{
			if(!RecordLinkagePtr(n))return false;
			dcmapDBREF ref;
			if(!RecordLinkagePtr(n)->RetrieveByWideName(v,&ref))
			{
				ref = *GetReference(to,n);
				if(!RecordLinkagePtr(n)->SelectID(ref))
					RecordLinkagePtr(n)->Insert();
				else RecordLinkagePtr(n)->Update();

				RecordLinkagePtr(n)->SetWideName(v);
				RecordLinkagePtr(n)->Post(&ref);
			}
			return SetReference(to,n,ref);
		}	
	case DCMDT_Integer:  
		if(rec->iSize>4)LexConvCall(SetInteger64,dcmapINT64)
		else LexConvCall(SetInteger,dcmapINT);

	case DCMDT_Unsigned:  
		if(rec->iSize>4)LexConvCall(SetUnsigned64,dcmapUINT64)
		else LexConvCall(SetUnsigned,dcmapUINT);

	case DCMDT_Flag:  LexConvCall(SetBoolean,bool);
	case DCMDT_Real:  LexConvCall(SetReal,dcmapREAL);
	case DCMDT_Unknown:  return false;
	case DCMDT_String:  
	case DCMDT_CLOB:  
	default:
		return SetString(to,n,StrD2C(v).c_str());
	}
	return false;
}

bool CBinReflection::SetInteger(void* to,int n,int v)const
{
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;

	if(rec->iDataType == DCMDT_Integer)
	{
		if(rec->iSize == 1) conv<char>(to,rec->iOffset) = v;
		else if(rec->iSize == 2) conv<short>(to,rec->iOffset) = v;
		else if(rec->iSize == 4) conv<int>(to,rec->iOffset) = v;
		else if(rec->iSize == 8) conv<dcmapINT64>(to,rec->iOffset) = v;
		else  return false;
	}
	else if(rec->iDataType == DCMDT_Flag)return SetBoolean(to,n,v);
	else if(rec->iDataType == DCMDT_Real) return SetReal(to,n,v);
	else if(rec->iDataType == DCMDT_Unsigned) return SetUnsigned(to,n,v);
	else if(rec->iDataType == DCMDT_Unknown) return false;
	else return SetString(to,n,gsys::to_str(v).c_str());
	return false;
}

bool CBinReflection::SetInteger64(void* to,int n,dcmapLLONG v)const
{
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;

	if(rec->iDataType == DCMDT_Integer)
	{
		if(rec->iSize == 1) conv<char>(to,rec->iOffset) = v;
		else if(rec->iSize == 2) conv<short>(to,rec->iOffset) = v;
		else if(rec->iSize == 4) conv<int>(to,rec->iOffset) = v;
		else if(rec->iSize == 8) conv<dcmapINT64>(to,rec->iOffset) = v;
		else  return false;
	}
	else if(rec->iDataType == DCMDT_Flag)return SetBoolean(to,n,v);
	else if(rec->iDataType == DCMDT_Real) return SetReal(to,n,v);
	else if(rec->iDataType == DCMDT_Unsigned) return SetUnsigned64(to,n,v);
	else if(rec->iDataType == DCMDT_Unknown) return false;
	else return SetString(to,n,gsys::to_str(v).c_str());
	return false;
}

bool CBinReflection::SetUnsigned(void* to,int n,unsigned v)const
{
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;

	if(rec->iDataType == DCMDT_Unsigned)
	{
		if(rec->iSize == 1) conv<unsigned char>(to,rec->iOffset) = v;
		else if(rec->iSize == 2) conv<unsigned short>(to,rec->iOffset) = v;
		else if(rec->iSize == 4) conv<unsigned int>(to,rec->iOffset) = v;
		else if(rec->iSize == 8) conv<dcmapUINT64>(to,rec->iOffset) = v;
		else  return false;
	}
	else if(rec->iDataType == DCMDT_Flag)return SetBoolean(to,n,v);
	else if(rec->iDataType == DCMDT_Real) return SetReal(to,n,v);
	else if(rec->iDataType == DCMDT_Integer) return SetInteger(to,n,v);
	else if(rec->iDataType == DCMDT_Unknown) return false;
	else return SetString(to,n,gsys::to_str(v).c_str());

	return false;
}

bool CBinReflection::SetUnsigned64(void* to,int n,dcmapQWORD v)const
{
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;

	if(rec->iDataType == DCMDT_Unsigned)
	{
		if(rec->iSize == 1) conv<unsigned char>(to,rec->iOffset) = v;
		else if(rec->iSize == 2) conv<unsigned short>(to,rec->iOffset) = v;
		else if(rec->iSize == 4) conv<unsigned int>(to,rec->iOffset) = v;
		else if(rec->iSize == 8) conv<dcmapUINT64>(to,rec->iOffset) = v;
		else  return false;
	}
	else if(rec->iDataType == DCMDT_Flag)return SetBoolean(to,n,v);
	else if(rec->iDataType == DCMDT_Real) return SetReal(to,n,v);
	else if(rec->iDataType == DCMDT_Integer) return SetInteger(to,n,v);
	else if(rec->iDataType == DCMDT_Unknown) return false;
	else return SetString(to,n,gsys::to_str(v).c_str());

	return false;
}

bool CBinReflection::SetReference(void * to,int n,dcmapDBREF const& ref)const
{
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;
	if(rec->iDataType != DCMDT_Reference && rec->iDataType != DCMDT_CLOB && rec->iDataType != DCMDT_WideCLOB && rec->iDataType != DCMDT_BLOB)return false;
	conv<dcmapDBREF>(to,rec->iOffset) = ref;
	return true;
}

bool CBinReflection::SetReal(void* to,int n,dcmapREAL v)const
{
	BRRecord const* rec = GetRecord(n);
	if(!rec)return false;

	if(rec->iDataType == DCMDT_Real)
	{
		if(rec->iSize == 4) conv<float>(to,rec->iOffset) = v;
		else if(rec->iSize == 8) conv<double>(to,rec->iOffset) = v;
		else false;
	}
	else if(rec->iDataType == DCMDT_Fixed)
	{
		int fmod = dcmapLOWORD(rec->iParams);
		if(rec->iSize == 1)	        conv<char>(to,rec->iOffset)  = v*(1<<fmod);
		else if(rec->iSize == 2)	conv<short>(to,rec->iOffset) = v*(1<<fmod);
		else if(rec->iSize == 4)	conv<int>(to,rec->iOffset)   = v*(1<<fmod);
	}
	else if(rec->iDataType == DCMDT_Flag)return SetBoolean(to,n,v);
	else if(rec->iDataType == DCMDT_Integer) return SetInteger64(to,n,v);
	else if(rec->iDataType == DCMDT_Unsigned) return SetUnsigned64(to,n,v);
	else if(rec->iDataType == DCMDT_Unknown) return false;
	else return SetString(to,n,gsys::to_str(v).c_str());
	return false;
}

IDCMapDataStorage* CBinReflection::RecordLinkagePtr( int id ) const
{
	if(id <0 || id >= m_Records.size())return 0;
	if(m_Records[id].relation_ptr)return (IDCMapDataStorage*)m_Records[id].relation_ptr;
	if(m_Records[id].relation.empty())return 0;
	const_cast<VecRecords&>(m_Records)[id].relation_ptr = 
		g_dcmapCore.GetDataSourceTyped<IDCMapDataStorage>(m_Records[id].relation.c_str());
	return (IDCMapDataStorage*)m_Records[id].relation_ptr;
}