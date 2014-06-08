#pragma once

#include <g_hash_map.h>
#include "../dcmapsys.h"
#include <g_file.h>
#include "BlobStorage.h"

static const int BRRecordNameSize = 32;

class CBinReflection
{
private:
	CBLOBStorage* m_pCLOB;

	struct BRHeader
	{
		dcmapDWORD DataSize;
		dcmapDWORD RecordCount;
		dcmapDWORD Codepage;
		dcmapDWORD Reserved;
		friend bool operator == (BRHeader const& b1,BRHeader const& b2)
		{
			if(b1.DataSize != b2.DataSize)return false;
			if(b1.Codepage != b2.Codepage)return false;
			if(b1.RecordCount != b2.RecordCount)return false;
			return true;
		}	
	};
public:
	struct BRRecordData{
		dcmapWORD iOffset;
		dcmapWORD iSize;
		dcmapWORD iParams;
		dcmapBYTE iDataSet;
		dcmapBYTE iDataType;
		char czName[BRRecordNameSize];
	};

	struct BRRecord
	{
		union{
			struct{
				dcmapWORD iOffset;
				dcmapWORD iSize;
				dcmapWORD iParams;
				dcmapBYTE iDataSet;
				dcmapBYTE iDataType;
				char czName[BRRecordNameSize];
			};
			BRRecordData data;
		};

		std::string relation;
		volatile  IDCMapDataStorage* relation_ptr;
		int Params;

		BRRecord()
		{
			memset(&data,0,sizeof(data));
			Params=0;
			relation_ptr=0;
		}

		friend bool operator < (BRRecord const& b1,BRRecord const& b2)
		{
			return _strnicmp(b1.czName,b2.czName,BRRecordNameSize)<0;
		}	

		friend bool operator == (BRRecord const& b1,BRRecord const& b2)
		{
			if(_strnicmp(b1.czName,b2.czName,BRRecordNameSize))return false;
			if(b1.iOffset != b2.iOffset)return false;
			if(b1.iSize != b2.iSize)return false;
			if(b1.iParams != b2.iParams)return false;
			if(b1.iDataType != b2.iDataType)return false;
			if(b1.iDataSet != b2.iDataSet)return false;
			return true;
		}	
		friend bool operator != (BRRecord const& b1,BRRecord const& b2){return !(b1 == b2);}		
	};

private:
	BRHeader m_Head;
	typedef std::vector<BRRecord> VecRecords;
	VecRecords m_Records;
	typedef gsys::map<std::string,int> MapRecords;
	MapRecords m_mapRecords;
public:
	void SetCBLOBStorage(CBLOBStorage* pCLOB){m_pCLOB = pCLOB;};

	int Size(){return m_Records.size();}

	BRRecord const* GetRecord(int id)const
	{
		if(id <0 || id >= m_Records.size())return 0;
		return &m_Records[id];
	}
	int GetRecordID(char const*  name)const
	{
		MapRecords::const_iterator it = m_mapRecords.find(name);
		if(it == m_mapRecords.end())return -1;
		return it->second;
	}
	BRRecord const* GetRecord(char const*  name)const
	{
		return GetRecord(GetRecordID(name));
	}
	DCMapDataType RecordType(int id)const
	{
		if(id <0 || id >= m_Records.size())return DCMDT_Unknown;
		return (DCMapDataType)m_Records[id].iDataType;
	}
	bool RecordBLOB(int id)const
	{
		if(id <0 || id >= m_Records.size())return false;
		switch(m_Records[id].iDataType)
		{
		case DCMDT_BLOB:
		case DCMDT_CLOB:
		case DCMDT_WideCLOB:
			return true;
		}
		return false;
	}
	bool RecordNumeric(int id)const
	{
		if(id <0 || id >= m_Records.size())return false;
		switch(m_Records[id].iDataType)
		{
		case DCMDT_Integer:
		case DCMDT_Unsigned:
		case DCMDT_Real:
		case DCMDT_Fixed:
		case DCMDT_Flag:
			return true;
		}
		return false;
	}
	int RecordSize(int id)const
	{
		if(id <0 || id >= m_Records.size())return 0;
		return m_Records[id].iSize;
	}
	char const* RecordName(int id)const
	{
		if(id <0 || id >= m_Records.size())return 0;
		return m_Records[id].czName;
	}
	int RecordParam(int id)const
	{
		if(id <0 || id >= m_Records.size())return 0;
		return m_Records[id].Params;
	}
	int RecordDataSet(int id)const
	{
		if(id <0 || id >= m_Records.size())return 0;
		return m_Records[id].iDataSet;
	}

	const char* RecordLinkage(int id)const
	{
		if(id <0 || id >= m_Records.size())return "";
		return m_Records[id].relation.c_str();
	}
	IDCMapDataStorage* RecordLinkagePtr(int id)const;
private:
	template<class T>static DCMapDataType GetType(){return DCMDT_Unknown;}
	
	template<class T>static DCMapDataType GetStrType(){return GetType<T>()?GetType<T>():DCMDT_String;}
	template<class T>static DCMapDataType GetWStrType(){return GetType<T>()?GetType<T>():DCMDT_WideString;}

	//template<class T>static DCMapDataType GetStrType(){return DCMDT_String;}
	//template<class T>static DCMapDataType GetWStrType(){return DCMDT_WideString;}

	void UpdSize(int n)
	{
		if(n >= m_Records.size())
		{
			m_Records.resize(n+1);
			m_Head.RecordCount = m_Records.size();
		}
	}
public:
	template<class C,class V>
	void RegisterInt(char const* name,V C::*ref,int pos=-1)
	{
		m_Head.DataSize = sizeof(C);
		C* tst=0;
		V* rf = &(tst->*ref);
		dcmapWORD ofs = (dcmapWORD)reinterpret_cast<dcmapDWORD>(rf);

		BRRecord rec;
		memset(&rec,0,sizeof(rec));
		strncpy(rec.czName,name,24);
		rec.iOffset = ofs;
		rec.iSize = sizeof(V);
		rec.iDataType = GetType<V>();
		rec.iParams=0;

		if(pos < 0)pos =  m_Records.size();
		UpdSize(pos);
		m_Records[pos] = rec;
		m_mapRecords[rec.czName] = pos;	
	}

	void SetParam(int param,int pos=-1)
	{
		if(pos < 0)pos =  m_Records.size()-1;
		if(pos >= m_Records.size())m_Records.resize(pos+1);
		m_Records[pos].Params = param;
	}

	void Alias(char const* alias,int pos=-1)
	{
		if(pos < 0)pos =  m_Records.size()-1;
		if(pos >= m_Records.size())m_Records.resize(pos+1);
		m_mapRecords[alias] = pos;
	}

	template<class C,class V>
	void RegisterFixed(char const* name,V C::*ref,int mod,int pos=-1)
	{
		m_Head.DataSize = sizeof(C);
		C* tst=0;
		V* rf = &(tst->*ref);
		dcmapWORD ofs = (dcmapWORD)reinterpret_cast<dcmapDWORD>(rf);

		BRRecord rec;
		memset(&rec,0,sizeof(rec));
		strncpy(rec.czName,name,24);
		rec.iOffset = ofs;
		rec.iSize = sizeof(V);
		rec.iDataType = DCMDT_Fixed;
		rec.iParams=mod;

		if(pos < 0)pos =  m_Records.size();
		UpdSize(pos);
		m_Records[pos] = rec;
		m_mapRecords[rec.czName] = pos;	
	}

	template<class C,class V>
	void RegisterReference(dcmapCSTR const name,V C::*ref,dcmapCSTR linkage,int pos=-1)
	{
		m_Head.DataSize = sizeof(C);
		C* tst=0;
		V* rf = &(tst->*ref);
		dcmapWORD ofs = (dcmapWORD)reinterpret_cast<dcmapDWORD>(rf);

		BRRecord rec;
		memset(&rec,0,sizeof(rec));
		strncpy(rec.czName,name,24);
		rec.iOffset = ofs;
		rec.iSize = sizeof(V);
		rec.iDataType = DCMDT_Reference;
		rec.iParams=0;
		if(linkage)rec.relation = linkage;

		if(pos < 0)pos =  m_Records.size();
		UpdSize(pos);
		m_Records[pos] = rec;
		m_mapRecords[rec.czName] = pos;
	}

	template<class C,class V>
	void RegisterString(char const* name,V C::*ref,int pos=-1)
	{
		m_Head.DataSize = sizeof(C);
		C* tst=0;
		V* rf = &(tst->*ref);
		dcmapWORD ofs = (dcmapWORD)reinterpret_cast<dcmapDWORD>(rf);

		BRRecord rec;
		memset(&rec,0,sizeof(rec));
		strncpy(rec.czName,name,24);
		rec.iOffset = ofs;
		rec.iSize = sizeof(V);
		rec.iDataType = GetStrType<V>();
		rec.iParams=0;

		if(pos < 0)pos =  m_Records.size();
		UpdSize(pos);
		m_Records[pos] = rec;
		m_mapRecords[rec.czName] = pos;
	}

	template<class C,class V>
	void RegisterWideString(char const* name,V C::*ref,int pos=-1)
	{
		m_Head.DataSize = sizeof(C);
		C* tst=0;
		V* rf = &(tst->*ref);
		dcmapWORD ofs = (dcmapWORD)reinterpret_cast<dcmapDWORD>(rf);

		BRRecord rec;
		memset(&rec,0,sizeof(rec));
		strncpy(rec.czName,name,24);
		rec.iOffset = ofs;
		rec.iSize = sizeof(V);
		rec.iDataType = GetWStrType<V>();
		rec.iParams=0;

		if(pos < 0)pos =  m_Records.size();
		UpdSize(pos);
		m_Records[pos] = rec;
		m_mapRecords[rec.czName] = pos;
	}

	template<class C,class V>
	void RegisterFlag(char const* name,V C::*ref,int Flag,int pos=-1)
	{
		//ASSERT(Flag);
		m_Head.DataSize = sizeof(C);
		C* tst=0;
		V* rf = &(tst->*ref);
		dcmapWORD ofs = (dcmapWORD)reinterpret_cast<dcmapDWORD>(rf);
		BRRecord rec;
		memset(&rec,0,sizeof(rec));
		strncpy(rec.czName,name,24);
		rec.iOffset = ofs;
		rec.iSize = sizeof(V);
		rec.iDataType = DCMDT_Flag;

		for(rec.iParams=0;rec.iParams<32;rec.iParams++)
		{
			if(Flag >> rec.iParams == 1)
				break;
		}

		if(pos < 0)pos =  m_Records.size();
		UpdSize(pos);
		m_Records[pos] = rec;
		m_mapRecords[rec.czName] = pos;
	}
public:
	int GetInteger(void const* from,int n)const;
	unsigned GetUnsigned(void const* from,int n)const;
	dcmapCSTR GetString(void const* from,int n)const;
	dcmapDCSTR GetStringW(void const* from,int n)const;
	dcmapDBREF const* GetReference(void const* from,int n)const;
	bool GetBoolean(void const* from,int n)const;
	dcmapREAL GetReal(void const* from,int n)const;

	bool SetReference(void * from,int n,dcmapDBREF const& ref)const;

	bool SetBoolean(void* to,int n,bool v)const;
	bool SetString(void* to,int n,dcmapCSTR v)const;
	bool SetString(void* to,int n,dcmapDCSTR v)const;
	bool SetInteger(void* to,int n,int v)const;
	bool SetInteger64(void* to,int n,dcmapLLONG v)const;
	bool SetUnsigned(void* to,int n,unsigned v)const;
	bool SetUnsigned64(void* to,int n,dcmapQWORD v)const;
	bool SetReal(void* to,int n,dcmapREAL v)const;
	dcmapINT64 GetInt64(void const* from,int n)const;
	dcmapUINT64 GetUnsigned64(void const* from,int n)const;
public:
	~CBinReflection(void);
	CBinReflection(void);

	bool WriteToFile(gsys::gfile* file);
	bool ReadFromFile(gsys::gfile* file,int DBVer);

	bool MakeCopyOf(dcmapBYTE* pResult,dcmapBYTE const* pFrom,CBinReflection const& brFrom);

	bool operator == (CBinReflection const& br);
};

template<> inline DCMapDataType CBinReflection::GetType<int>(){return DCMDT_Integer;}
template<> inline DCMapDataType CBinReflection::GetType<long>(){return DCMDT_Integer;}
template<> inline DCMapDataType CBinReflection::GetType<short>(){return DCMDT_Integer;}
template<> inline DCMapDataType CBinReflection::GetType<char>(){return DCMDT_Integer;}
template<> inline DCMapDataType CBinReflection::GetType<dcmapINT64>(){return DCMDT_Integer;}
template<> inline DCMapDataType CBinReflection::GetType<unsigned int>(){return DCMDT_Unsigned;}
template<> inline DCMapDataType CBinReflection::GetType<unsigned long>(){return DCMDT_Unsigned;}
template<> inline DCMapDataType CBinReflection::GetType<unsigned short>(){return DCMDT_Unsigned;}
template<> inline DCMapDataType CBinReflection::GetType<unsigned char>(){return DCMDT_Unsigned;}
template<> inline DCMapDataType CBinReflection::GetType<dcmapUINT64>(){return DCMDT_Unsigned;}
template<> inline DCMapDataType CBinReflection::GetType<dcmapDBREF>(){return DCMDT_BLOB;}
template<> inline DCMapDataType CBinReflection::GetType<float>(){return DCMDT_Real;}
template<> inline DCMapDataType CBinReflection::GetType<double>(){return DCMDT_Real;}
template<> inline DCMapDataType CBinReflection::GetType<bool>(){return DCMDT_Unsigned;}
template<> inline DCMapDataType CBinReflection::GetType<dcmapSTR>(){return DCMDT_String;}
template<> inline DCMapDataType CBinReflection::GetType<dcmapWSTR>(){return DCMDT_WideString;}

template<> inline DCMapDataType CBinReflection::GetStrType<dcmapDBREF>(){return DCMDT_CLOB;}

template<> inline DCMapDataType CBinReflection::GetWStrType<dcmapDBREF>(){return DCMDT_WideCLOB;}

