#pragma  once

#include <g_std_file.h>
#include <g_file_util.h>
#include <g_hash_map.h>
#include <g_assert.h>
#include <map>
#include "BinReflection.h"
#include "BlobStorage.h"
#include "time.h"
#include "utils.h"

#define DCMAP_REGISTER_STORAGEX(Module,Name,Params) DCMAP_REGISTEREX(Module,"PDataStorage",Name,Params)\
	DCMAP_REGALIAS2("PDataStorage",Name,IDCMapGetInterfaceID<Module*>())\
	DCMAP_REGALIAS3("PDataStorage",Name,IDCMapGetInterfaceID<Module::InheritedInterface*>())

#define DCMAP_REGISTER_STORAGE(Module,Name) DCMAP_REGISTER_STORAGEX(Module,Name,0)


extern "C" 
{
	#include "b64/cencode.h"
	#include "b64/cdecode.h"
}

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <sstream>

inline int ustring_casecompare(dcmapDCSTR s1,dcmapDCSTR s2)
{
	while(true)
	{
		wchar_t c1 = towlower((wchar_t)*s1);
		wchar_t c2 = towlower((wchar_t)*s2);

		if(c1 == c2)
		{
			if(c1==0)return 0;
		}
		else if(c1 > c2)return 1;
		else return -1;
		++s1;
		++s2;
	}
}

struct ustriless
{
	bool operator()(dcmap::dstring const& s1,dcmap::dstring const&  s2)const
	{
#if DCMAP_WCHAR_T_SIZE == 2 && 0
	return _wcsicmp(s1.c_str(),s2.c_str())<0;
#else
	return ustring_casecompare(s1.c_str(),s2.c_str())<0;
#endif			
	}
};

template <typename Key,typename Value>
class ustrimap: public std::map<Key,Value,ustriless >
{
};

template <class HldData,class Parent,class Target=Parent>
class CCustomDataStorage : public Parent
{
    typedef Parent Inherited;
public:
	typedef Parent InheritedInterface;
	typedef HldData value_type;
	virtual dcmapCSTR DCMAPAPI Name()=0;
	virtual bool DCMAPAPI SelectID(dcmapDBREF const& ref)=0;
protected:
	typedef CCustomDataStorage<HldData,Parent,Target> custom_storage;
protected:
	IDCMapCore* m_pCore;
	IDCMapDatabase* m_pDatabase;

	IDCMapDataStorage* m_pParentDataStorage;

	CBinReflection m_BR;
	CBLOBStorage m_CLOB;

	dcmapDBREF m_CurrentRef;

	value_type* m_pCurrentRecord;

	bool m_bInsertMode;
	bool m_bUpdateMode;

	dcmapQWORD m_baseModIndex;
	dcmapQWORD m_modIndex;
	value_type m_temp;

	dcmapWORD  m_DBVer;
	dcmapWORD  m_TableVer;

	static const int max_records = (1<<20);

	int  m_bBatchUpdate;
	bool m_bNeedUpdate;
	bool m_bEditIndex;

	bool m_bBlockDataUpdate;

	bool m_bLoaded;
	sysstring m_strFilename;
	int m_iLoadFlags;
protected:
	dcmapCSTR DCMAPAPI RecordName(){return "record";}

	virtual dcmapDWORD GenerateMagic()
	{
		dcmapDWORD base = ((dcmapDWORD)m_modIndex<<8);
		dcmapDWORD tm =  ~(unsigned)time(NULL)<<20;
		dcmapDWORD pre = base+tm+rand();
		if(pre == 0)pre += ((dcmapDWORD)rand())+1;
		return pre;
	}
	virtual dcmapWCSTR BlobToWStr(int field,dcmapDBREF data)
	{
		static dstring convString;
		convString = ToDStr(BlobToStr(field,data));
		return convString.c_str();
	}

	virtual bool WStrToBlob(int field,dcmapDBREF & data,dcmapWCSTR str)
	{
		return StrToBlob(field,data,StrD2C(str).c_str());
	}
	//////////////////////////////////////////////////////////////////////////
	virtual char const* BlobToStr(int field,dcmapDBREF data)
	{
		static gsys::raw_data_buffer<char> buffer;

		char* dta;
		int n = m_CLOB.GetValue(data,(dcmapBYTE**)&dta);
		buffer.reset(n*2);

		base64_encodestate state;
		base64_init_encodestate(&state);

		int len = base64_encode_block(dta, n, buffer.get(), &state);
		len += base64_encode_blockend(buffer.get()+len, &state);

		buffer.get()[len]=0;

		return buffer.get();
	}

	virtual bool StrToBlob(int field,dcmapDBREF & data,dcmapCSTR str)
	{
		static gsys::raw_data_buffer<char> buffer;

		int n = strlen(str);
		buffer.reset(n);

		base64_decodestate state;
		base64_init_decodestate(&state);
		int len = base64_decode_block(str, n, buffer.get(), &state);
		return SetBlob(field,(dcmapBYTE*)buffer.get(),len);
	}

	virtual void OnChange(){};
	virtual void OnDataUpdate(){};
	virtual bool FixIndexValue(value_type* pDta){return false;}
	void DataChanged()
	{
		OnChange();
		if(m_bBlockDataUpdate)return;

		m_modIndex++;
		if(!m_bBatchUpdate)
		{
			OnDataUpdate();
			m_pDatabase->OnDataUpdate();
		}
		else m_bNeedUpdate=true;
	}
	typedef std::vector<IDCMapDataStorage*> VecChilds;
	VecChilds m_vecChilds;

	dcmapWCSTR m_StorageCaption;
	dcmapWCSTR m_RecordCaption;
	dcmapWCSTR m_NewIndexText;

	bool DataLoad()
	{
		if(!m_bLoaded)
		{
			if(!DoLoadData())
			{
				return LoadEmpty();
			}
		}
		return true;
	}

public:
	CCustomDataStorage()
	{
		m_pParentDataStorage=0;
		m_pCore=0;
		m_pDatabase=0;
		m_BR.SetCBLOBStorage(&m_CLOB);

		m_DBVer=4;
		m_CurrentRef.Invalidate();
		m_TableVer=0;
		m_bEditIndex=false;
		m_baseModIndex=0xffffffffffffffff;
		m_modIndex=0xffffffffffffffff;
		m_bBlockDataUpdate = false;
		m_bLoaded = false;
		Clear();
	}

	virtual ~CCustomDataStorage()
	{
		Clear();
	}

	virtual int DCMAPAPI TableParams()
	{
		return 0;
	}

	bool DCMAPAPI InitStorage(IDCMapDatabase* pDatabase)
	{	
		m_pDatabase = pDatabase;
		return true;
	}
	void DCMAPAPI PostInit()
	{

	}


	virtual IDCMapDataStorage* DCMAPAPI GetStorage(){return this;}
	//dcmapCSTR  DCMAPAPI Name(){return "";}
	dcmapWCSTR DCMAPAPI StorageCaption(){return m_StorageCaption;}
	dcmapWCSTR DCMAPAPI RecordCaption(){return m_RecordCaption;}
	dcmapWCSTR DCMAPAPI NewIndexText(){return m_NewIndexText;}

	bool DCMAPAPI LoadDataFromFile(dcmapFCSTR szFileName, int Flags)
	{
		if(!gsys::FileExists(szFileName))return false;
		m_strFilename = szFileName;
		m_iLoadFlags = Flags;
		m_modIndex = 0xffffffffffffffff;

		if(Flags & DCMAP_DSLF_LAZY)return true; // load only if needed

		return DoLoadData();
	}

	virtual bool DoLoadData()=0;

	void DeleteDependants()
	{
		int n = m_BR.Size();
		for(int i=0;i<n;i++)
		{
			DCMapDataType type = m_BR.RecordType(i);
			if(type == DCMDT_BLOB || type == DCMDT_CLOB)
			{
				m_CLOB.Delete(*m_BR.GetReference(m_pCurrentRecord,i));
			}
		}

		BeginUpdate();
		// clear also child tables
		n = m_vecChilds.size();
		for(int i=0;i<n;i++)
		{
			IDCMapDataStorage* storage = m_vecChilds[i];
			storage->ClearByParent(m_pCurrentRecord->id);
		}	
		EndUpdate();
	}

	void DCMAPAPI Clear()
	{
		m_CLOB.Clear();
		m_pCurrentRecord = 0;

		m_bBatchUpdate=0;
		m_bNeedUpdate=0;
		m_bInsertMode=false;
		m_modIndex=0xffffffffffffffff;
		m_baseModIndex=0xffffffffffffffff;
		m_bUpdateMode=false;
	}

	bool DCMAPAPI LoadEmpty()
	{
		Clear();
		m_bLoaded = true;
		m_modIndex=2;
		m_baseModIndex=1;
		return true;
	}

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface)
	{
		DCMAP_MODULE_SUPPORTS(Parent);
		DCMAP_MODULE_SUPPORTS(Target);
		DCMAP_MODULE_SUPPORTS(IDCMapDataStorage);
		DCMAP_MODULE_SUPPORTS(IDCMapDataProvider);
		DCMAP_MODULE_SUPPORTS(IDCMapDataSource);
		DCMAP_MODULE_SUPPORTS(IDCMapModule);
		DCMAP_MODULE_SUPPORTS(IDCMapScriptable);
		DCMAP_MODULE_SUPPORTS(IDCMapObject);
		return 0;
	}
	void DCMAPAPI Destroy(){delete this;}

	//////////////////////////////////////////////////////////////////////////
	// meta information 
	int DCMAPAPI NumColumns(){return m_BR.Size();}
	int DCMAPAPI ColumnDataSize(int n){return m_BR.RecordSize(n);}
	DCMapDataType DCMAPAPI ColumnDataType(int n){return m_BR.RecordType(n);}

	bool DCMAPAPI ColumnNumeric(int n){return m_BR.RecordNumeric(n);	}
	bool DCMAPAPI ColumnNumericN(const char* n){return m_BR.RecordNumeric(m_BR.GetRecordID(n));}

	char const* DCMAPAPI ColumnName(int n){return m_BR.RecordName(n);}

	int DCMAPAPI ColumnParams(int n){return m_BR.RecordParam(n);}
	int DCMAPAPI ColumnParamsN(const char* n){return m_BR.RecordParam(m_BR.GetRecordID(n));}

	int DCMAPAPI ColumnDataSizeN(const char* n){return m_BR.RecordSize(m_BR.GetRecordID(n));}
	DCMapDataType DCMAPAPI ColumnDataTypeN(const char* n){return m_BR.RecordType(m_BR.GetRecordID(n));}
	char const* DCMAPAPI ColumnNameN(const char* n){return m_BR.RecordName(m_BR.GetRecordID(n));}

	char const* DCMAPAPI ColumnDataLinkage(int n){return m_BR.RecordLinkage(n);}
	char const* DCMAPAPI ColumnDataLinkageN(const char* n){return m_BR.RecordLinkage(m_BR.GetRecordID(n));}

	int DCMAPAPI ColumnID(const char* name){return m_BR.GetRecordID(name);};

	//////////////////////////////////////////////////////////////////////////
	// data access
	int DCMAPAPI GetInteger(int n){return m_BR.GetInteger(m_pCurrentRecord,n);}
	int DCMAPAPI GetIntegerN(char const* n){return m_BR.GetInteger(m_pCurrentRecord,m_BR.GetRecordID(n));}

	dcmapDWORD DCMAPAPI GetUnsigned(int n){return m_BR.GetUnsigned(m_pCurrentRecord,n);}
	dcmapDWORD DCMAPAPI GetUnsignedN(char const* n){return m_BR.GetUnsigned(m_pCurrentRecord,m_BR.GetRecordID(n));}

	dcmapREAL DCMAPAPI GetReal(int n){return m_BR.GetReal(m_pCurrentRecord,n);}
	dcmapREAL DCMAPAPI GetRealN(char const* n){return m_BR.GetReal(m_pCurrentRecord,m_BR.GetRecordID(n));}

	int DCMAPAPI GetBlob(int n,dcmapBYTE** pData)
	{
		if(m_BR.RecordType(n) == DCMDT_BLOB)
		{
			return m_CLOB.GetValue(*m_BR.GetReference(m_pCurrentRecord,n),pData);
		}
		return 0;
	}
	int DCMAPAPI GetBlobN(char const* n,dcmapBYTE** pData){return GetBlob(m_BR.GetRecordID(n),pData);}

	virtual char const* DCMAPAPI GetString(int n)
	{
		if(m_BR.RecordType(n) == DCMDT_BLOB)
		{
			return BlobToStr(n,*m_BR.GetReference(m_pCurrentRecord,n));
		}
		return m_BR.GetString(m_pCurrentRecord,n);
	}
	virtual char const* DCMAPAPI GetStringN(char const* n){return GetString(m_BR.GetRecordID(n));}

	virtual dcmapDCSTR DCMAPAPI GetWideString(int n)
	{
		if(m_BR.RecordType(n) == DCMDT_BLOB)
		{
			return BlobToWStr(n,*m_BR.GetReference(m_pCurrentRecord,n));
		}
		return m_BR.GetStringW(m_pCurrentRecord,n);
	}
	virtual dcmapDCSTR DCMAPAPI GetWideStringN(char const* n){return GetWideString(m_BR.GetRecordID(n));}

	virtual const dcmapDBREF* DCMAPAPI GetReference(int n){return m_BR.GetReference(m_pCurrentRecord,n);}
	virtual const dcmapDBREF* DCMAPAPI GetReferenceN(char const* n){return m_BR.GetReference(m_pCurrentRecord,m_BR.GetRecordID(n));}

	bool DCMAPAPI GetBoolean(int n){return m_BR.GetBoolean(m_pCurrentRecord,n);}
	bool DCMAPAPI GetBooleanN(char const* n){return m_BR.GetBoolean(m_pCurrentRecord,m_BR.GetRecordID(n));}

	//////////////////////////////////////////////////////////////////////////
	bool DCMAPAPI SetBlob(int n,dcmapBYTE const* value,int size)
	{
		if(m_BR.RecordType(n) == DCMDT_BLOB)
		{
			dcmapDBREF const& ref = *m_BR.GetReference(&m_temp,n);
			if(!value || !size)
				m_CLOB.Delete(ref);
			else if(!m_CLOB.UpdateValue(ref,value,size))
				m_BR.SetReference(&m_temp,n,*m_CLOB.InsertValue(value,size));
			return true;
		}
		return false;
	}
	virtual bool DCMAPAPI SetBlobN(char const* n,dcmapBYTE const* value,int size){return SetBlob(m_BR.GetRecordID(n),value,size);}

	bool DCMAPAPI SetString(int n,char const* value)
	{
		if(!value)return false;
		if(m_BR.RecordType(n) == DCMDT_BLOB)
		{
			dcmapDBREF ref = *m_BR.GetReference(&m_temp,n);
			return StrToBlob(n,ref,value);
		}
		return m_BR.SetString(&m_temp,n,value);
	}
	virtual bool DCMAPAPI SetStringN(char const* n,char const* value){return SetString(m_BR.GetRecordID(n),value);}

	bool DCMAPAPI SetWideString(int n,dcmapDCSTR value)
	{
		if(!value)return false;
		if(m_BR.RecordType(n) == DCMDT_BLOB)
		{
			dcmapDBREF ref = *m_BR.GetReference(&m_temp,n);
			return WStrToBlob(n,ref,value);
		}
		return m_BR.SetString(&m_temp,n,value);
	}
	virtual bool DCMAPAPI SetWideStringN(char const* n,dcmapDCSTR value){return SetWideString(m_BR.GetRecordID(n),value);}


	bool DCMAPAPI SetBoolean(int n,bool v){return m_BR.SetBoolean(&m_temp,n,v);}
	bool DCMAPAPI SetBooleanN(char const* n,bool v){return m_BR.SetBoolean(&m_temp,m_BR.GetRecordID(n),v);}

	bool DCMAPAPI SetInteger(int n,int v){return m_BR.SetInteger(&m_temp,n,v);}
	bool DCMAPAPI SetIntegerN(char const* n,int v){return m_BR.SetInteger(&m_temp,m_BR.GetRecordID(n),v);}

	bool DCMAPAPI SetUnsigned(int n,dcmapDWORD v){return m_BR.SetUnsigned(&m_temp,n,v);}
	bool DCMAPAPI SetUnsignedN(char const* n,dcmapDWORD v){return m_BR.SetUnsigned(&m_temp,m_BR.GetRecordID(n),v);}

	bool DCMAPAPI SetReal(int n,dcmapREAL v){return m_BR.SetReal(&m_temp,n,v);}
	bool DCMAPAPI SetRealN(char const* n,dcmapREAL v){return m_BR.SetReal(&m_temp,m_BR.GetRecordID(n),v);}

	bool DCMAPAPI SetReference(int n,dcmapDBREF const& ref){return m_BR.SetReference(&m_temp,n,ref);}
	bool DCMAPAPI SetReferenceN(char const* n,dcmapDBREF const& ref){return m_BR.SetReference(&m_temp,m_BR.GetRecordID(n),ref);}

	void DCMAPAPI SetData(const value_type* data)
	{
		_ASSERT(data);
		m_temp = *data;
	}

	int DCMAPAPI GetDataSize(){return sizeof(value_type);}
	dcmapQWORD DCMAPAPI GetStateIndex(){return m_modIndex;}

	dcmapDCSTR DCMAPAPI FetchWideName(dcmapDBREF const& pRef)
	{
		value_type* crec = m_pCurrentRecord;
		if(!SelectID(pRef))return dcmapDCSTR(L"");
		dcmapDCSTR res = GetWideName();
		m_pCurrentRecord = crec;
		return res;
	};

	dcmapCSTR DCMAPAPI FetchName(dcmapDBREF const& pRef)
	{
		value_type* crec = m_pCurrentRecord;
		if(!SelectID(pRef))return "";
		dcmapCSTR res = GetName();
		m_pCurrentRecord = crec;
		return res;
	};

	bool DCMAPAPI SelectName(const char* szName)
	{
		if(!DataLoad())return false;

		if(m_bUpdateMode || m_bInsertMode)return false;
		SetName(szName);
		return SelectIndex();
	}
	bool DCMAPAPI SelectWideName(dcmapDCSTR szName)
	{
		if(!DataLoad())return false;

		if(m_bUpdateMode || m_bInsertMode)return false;
		SetWideName(szName);
		return SelectIndex();
	}

	//dcmapDCSTR DCMAPAPI FetchWideName(dcmapDBREF const& pRef){return L"";};
	//dcmapCSTR DCMAPAPI FetchName(dcmapDBREF const& pRef){return "";};

	//bool DCMAPAPI SelectName(const char* szName){return false;}
	//bool DCMAPAPI SelectWideName(dcmapDCSTR szName){return false;}


	bool DCMAPAPI RetrieveByName(const char* szName,dcmapDBREF* pRef){return false;}
	bool DCMAPAPI RetrieveByWideName(dcmapDCSTR szName,dcmapDBREF* pRef=0){return false;}

	bool DCMAPAPI SelectByParentID(dcmapDBREF const& ref){return false;}

	const dcmapDBREF* DCMAPAPI GetParent()
	{
		static const dcmapDBREF ref;
		return &ref;
	}
	void DCMAPAPI SetParent(dcmapDBREF const& Ref){}
	void DCMAPAPI SetName(const char* szName){}
	void DCMAPAPI SetWideName(dcmapDCSTR szName){}

	bool DCMAPAPI EditIndex()
	{
		FillDefaults(&m_temp);
		m_bEditIndex=true;
		return true;
	}
	bool DCMAPAPI SelectIndex()
	{
		m_bEditIndex=false;
		return false;
	}

	void DCMAPAPI FillDefaults(value_type* pinf)
	{
		memset(pinf,0,sizeof(HldData));
	}

	bool DCMAPAPI Changed()
	{
		return m_modIndex != m_baseModIndex;
	}

	void DCMAPAPI ReserveForNew(int num){}
	bool DCMAPAPI OrderSwap(dcmapDBREF const& r1,dcmapDBREF const& r2){return false;}

	void DCMAPAPI BeginUpdate()
	{
		if(m_bBatchUpdate<=0)
		{
			m_pDatabase->LockDataUpdate();
			m_bBatchUpdate=1;
		}
		else m_bBatchUpdate++;
	}
	void DCMAPAPI EndUpdate()
	{
		m_bBatchUpdate--;
		if(m_bBatchUpdate<=0)
		{
			m_pDatabase->UnlockDataUpdate();
			if(m_bNeedUpdate)
			{
				OnDataUpdate();
				m_pDatabase->OnDataUpdate();
				m_bNeedUpdate=false;
			}
			m_bBatchUpdate=0;
		}
	}

	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		g_pLocalStrings->SelectSection("tables");
		m_StorageCaption = g_pLocalStrings->GetSectionStringW(((std::string)Name()+"_StorageCaption").c_str());
		m_RecordCaption = g_pLocalStrings->GetSectionStringW(((std::string)Name()+"_RecordCaption").c_str());
		m_NewIndexText = g_pLocalStrings->GetSectionStringW(((std::string)Name()+"_NewIndexText").c_str());

		m_pCore = core;
		m_pCore->OnDataUpdate();
		return true;
	};

	const char* DCMAPAPI GetName(){return "";};
	dcmapUCSTR DCMAPAPI GetDisplayName(){static ustring str; str = ToUStr(GetWideName());return str.c_str();}
	dcmapDCSTR DCMAPAPI GetWideName(){return dcmapDCSTR(L"");};
	void DCMAPAPI Insert()
	{
		DataLoad();
		
		m_bInsertMode=true;
		m_bUpdateMode=false;
		FillDefaults(&m_temp);
	}

	bool DCMAPAPI Update()
	{
		if(!m_pCurrentRecord) return false;
		if(!m_bInsertMode && !m_bUpdateMode)
		{
			m_bUpdateMode=true;
			m_temp = *m_pCurrentRecord;
		}
		return true;
	}

	const char* DCMAPAPI GetNotes(){return "";}
	dcmapDCSTR DCMAPAPI GetWideNotes(){return dcmapDCSTR(L"");}

	void DCMAPAPI SetNotes(const char* szName){}
	void DCMAPAPI SetWideNotes(dcmapDCSTR szName){}
	IDCMapDataStorage* DCMAPAPI ParentDataStorage(){return m_pParentDataStorage;}
	void DCMAPAPI Cansel()
	{
		m_bInsertMode=false;
		m_bUpdateMode=false;
	}

	const value_type* DCMAPAPI GetData()
	{
		return m_pCurrentRecord;
	}

	virtual void DCMAPAPI ClearByParent(dcmapDBREF const& pRef){}
	virtual void DCMAPAPI ClearUnparented(){};
	void DCMAPAPI AddChildTable(IDCMapDataStorage* pChild)
	{
		m_vecChilds.push_back(pChild);
	}
	virtual bool DCMAPAPI Duplicate(){return false;}
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static const dcmapFOURCC c_DBMagic = DCMAP_MAKEFOURCC('D','C','M','1');

enum EDCMDTF
{
	DCMDTF_EXTENDED_HEADER = (1<<0),
	DCMDTF_CUSTOM_HEADER = (1<<1),
};

template <class HldData,class Parent,class Target=Parent>
class CStandartDataStorage : public CCustomDataStorage<HldData,Parent,Target>
{
    typedef CCustomDataStorage<HldData,Parent,Target> Inherited;
    typedef CStandartDataStorage<HldData,Parent,Target> Myself;
protected:
	typedef typename Inherited::value_type value_type;
	typedef CStandartDataStorage<HldData,Parent,Target> standart_storage;

	typedef gsys::indexed_list<value_type,dcmapDWORD,dcmapDWORD,DCMapInvalidReferenceAge,1> DataList;
	DataList m_lstData;
	typename DataList::iterator m_DataIt;

	value_type& get(dcmapDWORD n){return m_lstData[n];}
	value_type const& get(dcmapDWORD n)const{return m_lstData[n];}

	dcmapSHORT m_CustomHeaderSize;

public:
	CStandartDataStorage():m_CustomHeaderSize(0)
	{
		Inherited::m_BR.RegisterReference("id",&HldData::id,"",DCMDB_BASE_ID);
	}
	virtual ~CStandartDataStorage()
	{
	}

	void DCMAPAPI ReserveForNew(int num)
	{
		m_lstData.reserve(m_lstData.size()+num);
	}

	int DCMAPAPI NumRecords(){if(!DataLoad())return 0;return m_lstData.size();}

	void DCMAPAPI Clear()
	{
		m_lstData.clear();
		m_DataIt = m_lstData.end();	
		Inherited::Clear();
	}

	bool TestRef(dcmapDBREF const& ref)
	{
        //if(!&ref)return false;
        GSYS_ASSERTREF(ref);
		if(!&ref) return false;
		if(ref.id >= m_lstData.capacity())return false;
		if(m_lstData[ref.id].id != ref)return false;
		return true;
	}

	// General Select 
	bool DCMAPAPI SelectID(dcmapDBREF const& ref)
	{
		if(!Inherited::DataLoad())return false;

		if(!TestRef(ref))return false;		
		
		if(Inherited::m_pCurrentRecord && Inherited::m_pCurrentRecord->id == ref)return true;
		//m_CurrentRef= ref;
		Inherited::m_pCurrentRecord = 0;
		Inherited::m_pCurrentRecord = &m_lstData[ref.id];
		return true;
	}

	bool DCMAPAPI SelectAll()
	{
		if(!Inherited::DataLoad())return false;

		Inherited::m_pCurrentRecord = 0;
		m_DataIt = m_lstData.begin();
		if(m_DataIt == m_lstData.end())return false;
		Inherited::m_pCurrentRecord = &(*m_DataIt);
		++m_DataIt;
		return true;
	}

	// Next selected record
	bool DCMAPAPI Next()
	{
		if(m_DataIt == m_lstData.end())
		{
			Inherited::m_pCurrentRecord = 0;
			return false;
		}
		Inherited::m_pCurrentRecord = &(*m_DataIt);
		++m_DataIt;
		return true;
	}

	virtual bool NeedReindex(value_type const* old,value_type const* newd)
	{
		return false;
	}

	bool DoIndexValue(value_type* data)
	{
		if(!IndexValue(data)) 
		{
			if(FixIndexValue(data))
			{
				return IndexValue(data);
			}
			else return false;
		}
		else return true;
	}
	virtual bool ProcessUpdatedData(value_type* data){return true;}
	virtual bool ProcessNewData(value_type* data){return true;}

	bool DCMAPAPI UpdateData(const value_type* data)
	{
		return UpdateData(data,0);
	}

	bool DCMAPAPI UpdateData(const value_type* data,dcmapDBREF* pRef)
	{
		if(!Inherited::m_pCurrentRecord)return false;
		Inherited::m_bUpdateMode=false;

		//dcmapDBREF notes = m_pCurrentRecord->notes;
		if(data->id.id == Inherited::m_pCurrentRecord->id.id && data->id.age == Inherited::m_pCurrentRecord->id.age)
		{   // reference in new data are valid still
			if(!ProcessUpdatedData(const_cast<value_type*>(data)))return false;
			if(NeedReindex(Inherited::m_pCurrentRecord,data))
			{
				UnIndexValue(Inherited::m_pCurrentRecord);
				if(!IndexValue(data))
				{
					IndexValue(Inherited::m_pCurrentRecord);
					return false;
				}
			}
			*Inherited::m_pCurrentRecord = *data;
		}
		else // reference in new data invalid
		{
			dcmapDBREF ref = Inherited::m_pCurrentRecord->id;
			value_type val = *data;
			if(!ProcessUpdatedData(&val))return false;
			val.id = ref;
			//val.notes = notes; // notes restore
			if(NeedReindex(Inherited::m_pCurrentRecord,&val))
			{
				UnIndexValue(Inherited::m_pCurrentRecord);
				if(!DoIndexValue(&val))
				{
					IndexValue(Inherited::m_pCurrentRecord);
					return false;
				}
			}
			*Inherited::m_pCurrentRecord = val;
		}
		if(pRef)*pRef = Inherited::m_pCurrentRecord->id;
		Inherited::DataChanged();
		return true;
	}

	bool DCMAPAPI InsertData(const value_type* data,dcmapDBREF* pRef)
	{
		if(!Inherited::DataLoad())return false;

		//_ASSERT(m_lstData.check_structure());

		m_lstData.push_back(*data);

		Inherited::m_pCurrentRecord = &m_lstData.back();

		Inherited::m_pCurrentRecord->id.id = m_lstData.back_id();

		Inherited::m_pCurrentRecord->id.age = Inherited::GenerateMagic();

		m_lstData.tag(m_lstData.back_id()) = Inherited::m_pCurrentRecord->id.age;

		if(!ProcessNewData(Inherited::m_pCurrentRecord))
		{
			m_lstData.pop_back();
			return false;
		}

		dcmapDBREF ref = Inherited::m_pCurrentRecord->id;
		if(!DoIndexValue(Inherited::m_pCurrentRecord))
		{
			m_lstData.pop_back();
			return false;
		}

		Inherited::DataChanged();
		SelectID(ref);

		if(pRef)*pRef = ref;

		return true;
	}

	bool DCMAPAPI Delete()
	{
		if(!Inherited::m_pCurrentRecord)return false;
		if(Inherited::m_pCurrentRecord->id.id == DCMapInvalidReferenceId) return false;

		Inherited::DeleteDependants();

		OnDelete(Inherited::m_pCurrentRecord);

		dcmapDWORD id = Inherited::m_pCurrentRecord->id.id;
		UnIndexValue(Inherited::m_pCurrentRecord);
		Inherited::m_pCurrentRecord->id.Invalidate();
		typename DataList::iterator it = m_lstData.erase(id);
		if(m_DataIt != m_lstData.end())m_DataIt = it;

		Inherited::DataChanged();

		return true;
	}

	bool DCMAPAPI Duplicate()
	{
		if(!Inherited::m_pCurrentRecord)return false;
		value_type *val = Inherited::m_pCurrentRecord;
		Inherited::Insert();
		SetData(val);
		// duplicate blob records
		for(int i=0;i<Inherited::m_BR.Size();i++)
		{
			if(Inherited::m_BR.RecordBLOB(i))
			{
				dcmapBYTE* pData=0;
				int size = Inherited::m_CLOB.GetValue(*Inherited::m_BR.GetReference(Inherited::m_pCurrentRecord,i),&pData);
				Inherited::m_BR.SetReference(&(this->m_temp),i,*Inherited::m_CLOB.InsertValue(pData,size));
			}
		}
		return true;
	}

#pragma pack(push)
#pragma pack(1)
	struct DataFileHeader
	{
		dcmapDWORD NumRecords;
		dcmapDWORD RecordSize;
		dcmapDWORD DbCapacity;
		dcmapQWORD modIndex;
		dcmapWORD  DBVer;
		dcmapWORD  TableVer;
		dcmapWORD  Flags;
		dcmapSHORT  CustomHeadersSize;
	};
	struct DataFileHeaderEx
	{
		dcmapSHORT cbSize;
		dcmapWORD  wReserved;
		dcmapDWORD dwReserved;
		dcmapTIME  saveTime;
	};
#pragma pack(pop)

	virtual bool OnDataConvert(void const* pData,int size,dcmapWORD ver,value_type & res)
	{
		return true;
	}
	virtual bool OnTestDataConvert(void const* pData,int size,dcmapWORD ver,value_type & res)
	{
		return 1;
	}
	virtual bool ProcessLoadData(value_type* pdata)
	{
		if(IndexValue(pdata))return true;

		if(FixIndexValue(pdata))
		{
			if(IndexValue(pdata))return true;
		}
		return false;
	}
	virtual void ProcessCustomHeader(dcmapSHORT cbSize,dcmapBYTE const* buf){};
	virtual bool GetCustomHeader(dcmapSHORT &cbSize,dcmapBYTE const* &buf){cbSize=0;buf=0;return false;};

	bool DoLoadData()
	{
		boost::scoped_ptr<gfile> pfile(OpenFileReadZ(Inherited::m_strFilename.c_str()));
		if(!pfile || !pfile->valid())return false;
		gfile& file = *pfile;

		dcmapFOURCC magic;
		file >> magic;
		if(magic != c_DBMagic)return false;

		DataFileHeader head;
		DataFileHeaderEx headex;
		file >> head;
		if(head.DbCapacity >= Inherited::max_records) return false;

		Inherited::m_modIndex = head.modIndex;
		Inherited::m_baseModIndex = head.modIndex;

		if(head.TableVer > Inherited::m_TableVer) return false; // incompatible
		if(head.DBVer > Inherited::m_DBVer) return false; // incompatible

		if(head.DBVer == 2 && head.Flags&DCMDTF_EXTENDED_HEADER)
		{
			file.skip(16); // skip early header ex struct
			memset(&headex,0,sizeof(headex));
		}

		if((head.Flags&DCMDTF_EXTENDED_HEADER) && head.CustomHeadersSize>0)
		{
			if(!file.load_sized_struct<DataFileHeaderEx,dcmapSHORT>(headex))return false;
			head.CustomHeadersSize -= headex.cbSize;
			if(head.CustomHeadersSize<0)return false;
		}

		if((head.Flags&DCMDTF_CUSTOM_HEADER) && head.CustomHeadersSize >0) // read custom header
		{
			dcmapBYTE* buf = new dcmapBYTE[m_CustomHeaderSize];
			//if(!file.read(buf,m_CustomHeaderSize))return false;
			
			dcmapSHORT cbSize;
			file.load_sized_buffer(buf,m_CustomHeaderSize,cbSize);
			ProcessCustomHeader(cbSize-sizeof(cbSize),buf);
			// process custom header
			delete[] buf;
			head.CustomHeadersSize -= cbSize;
			if(head.CustomHeadersSize<0)return false;
		}
		if(head.CustomHeadersSize > 0)
		{
			file.skip(head.CustomHeadersSize);
		}

		CBinReflection br;
		if(!br.ReadFromFile(&file,head.DBVer))return false;
		bool bPlainData = (br == Inherited::m_BR);
		if(head.TableVer != Inherited::m_TableVer) bPlainData = false;
		if(head.DBVer != Inherited::m_DBVer) bPlainData = false;

		ClearIndex();
		m_lstData.clear();
		m_lstData.reserve(head.DbCapacity);

		if(head.DBVer < 1) // old style blob at end of file
		{
			// read clob first
			unsigned data_pos = file.pos();
			file.seek(head.NumRecords*head.RecordSize,gsys::gfile::SeekCur);
			if(!Inherited::m_CLOB.LoadDataFromFile(&file,head.DBVer))
				return false;

			file.set_pos(data_pos);
		}
		else // new style - blob in front of data massive 
		{
			if(!Inherited::m_CLOB.LoadDataFromFile(&file,head.DBVer))
				return false;
		}

		Inherited::m_BR.SetCBLOBStorage(&(this->m_CLOB));
		br.SetCBLOBStorage(&(this->m_CLOB));
		// seek back and read main data
		if(head.RecordSize == sizeof(value_type) && bPlainData)
		{
			int j = 565;
			for(int i=0;i<head.NumRecords;i++)
			{
				value_type dta;
				file >> dta;
				if(dta.id.id >= head.DbCapacity) continue;
				if(dta.id.age == DCMapInvalidReferenceAge) continue;
				if(!m_lstData.is_free(dta.id.id))continue;

				if(!ProcessLoadData(&dta))continue;

				m_lstData.restore_element(dta.id.id,dta);
				m_lstData.tag(dta.id.id) = dta.id.age;
			}
		}
		else
		{
			int NumLoaded=0;
			dcmapBYTE* buf = new dcmapBYTE[head.RecordSize];
			for(int i=0;i<head.NumRecords;i++)
			{
				file.read(buf,head.RecordSize);
				value_type dta;
				FillDefaults(&dta);				
				
				int tk = OnTestDataConvert(buf,head.RecordSize,head.TableVer,dta);
				if(!tk)continue;
				if(tk==1)
				{
					if(!Inherited::m_BR.MakeCopyOf((dcmapBYTE*)&dta,buf,br))continue;
					if(!OnDataConvert(buf,head.RecordSize,head.TableVer,dta))continue;
				}
				if(!ProcessLoadData(&dta))continue;				
				m_lstData.restore_element(dta.id.id,dta);
				m_lstData.tag(dta.id.id) = dta.id.age;
				NumLoaded++;
			}
			delete[] buf;
			Inherited::m_modIndex++;
			if(!NumLoaded) LoadEmpty();
		}
		if(!CheckSanity())
		{
			m_lstData.clear();
			Inherited::m_CLOB.Clear();
			return false;
		}
		Inherited::m_bLoaded = true;
		return true;		
	}

	bool DCMAPAPI CheckSanity()
	{
		if(!m_lstData.check_structure())return false;
		return Inherited::m_CLOB.CheckSanity();
	}

	bool DCMAPAPI SaveDataToFile(dcmapFCSTR szFileName,int SaveFlags)
	{
		Inherited::ClearUnparented();
		if(!CheckSanity())return false;
		dcmapWORD  Flags = DCMDTF_EXTENDED_HEADER;

		dcmapSHORT ch_cbSize;
		const dcmapBYTE* ch_buf;
		if(GetCustomHeader(ch_cbSize,ch_buf))
		{
			Flags |= DCMDTF_CUSTOM_HEADER;
		}

		Inherited::m_strFilename = szFileName;

		boost::scoped_ptr<gsys::gfile> pfile(OpenFileCreateZ(szFileName,SaveFlags & DCMAP_DSLF_GZIPPED));
		if(!pfile || !pfile->valid())return false;
		gsys::gfile& file = *pfile;

		file << c_DBMagic;

		DataFileHeader head;
		memset(&head,0,sizeof(head));

		DataFileHeaderEx headex;
		memset(&headex,0,sizeof(headex));

		head.NumRecords = m_lstData.size();
		head.RecordSize = sizeof(value_type);
		head.DbCapacity = m_lstData.capacity();
		head.modIndex = Inherited::m_modIndex;
		head.DBVer = Inherited::m_DBVer;
		head.TableVer = Inherited::m_TableVer;
		head.Flags = Flags;

		head.CustomHeadersSize = sizeof(headex);
		if(Flags&DCMDTF_CUSTOM_HEADER)head.CustomHeadersSize += ch_cbSize+sizeof(ch_cbSize);

		headex.cbSize = sizeof(headex);
		headex.saveTime = dcmapGetTime();

		int maxwrite = m_lstData.size();
		int n=0;

		// save header
		file << head;
		file << headex;

		if(Flags&DCMDTF_CUSTOM_HEADER)
		{
			file << (dcmapSHORT)(ch_cbSize+sizeof(ch_cbSize));
			file.twrite(ch_buf,ch_cbSize);
		}

		// save table format
		Inherited::m_BR.WriteToFile(&file);

		// save blobe
		if(!Inherited::m_CLOB.SaveDataToFile(&file))return false;

		// save data
		_foreach(typename DataList,m_lstData,it) // Set new loaded index
		{
			if(n > maxwrite) // data error 
			{
				file.close();
				dcmapDeleteFile(szFileName);
				return false;
			}
			file << *it;
			n++;
		}
		file.close();
		Inherited::m_baseModIndex = Inherited::m_modIndex;

		return true;
	}

	const dcmapDBREF* DCMAPAPI GetID()
	{
		return &Inherited::m_pCurrentRecord->id;
	}

	bool DCMAPAPI Post(dcmapDBREF* pRef=0)
	{
		bool r=false;
		if(Inherited::m_bInsertMode)
			r =  InsertData(&(this->m_temp),pRef);
		else if(Inherited::m_bUpdateMode)
		{
			r = UpdateData(&(this->m_temp),pRef);
		}

		Inherited::m_bInsertMode=false;
		Inherited::m_bUpdateMode=false;
		return r;
	}

	virtual bool IndexValue( const value_type* pDta){return true;}
	virtual void UnIndexValue(const value_type* pDta){}
	virtual void ClearIndex(){}
	virtual void OnDelete(value_type* pDta){}

	bool DCMAPAPI OrderSwap(dcmapDBREF const& r1,dcmapDBREF const& r2)
	{
		if(!TestRef(r1))return false;
		if(!TestRef(r2))return false;

		m_lstData.order_swap(r1.id,r2.id);

		Inherited::DataChanged();
		//m_modIndex++;
		return true;
	}
};



template <class HldData,class Parent,class Target=Parent>
class CBaseDataStorage : public CStandartDataStorage<HldData,Parent,Target>
{
    typedef CStandartDataStorage<HldData,Parent,Target> Inherited;
protected:
	typedef typename Inherited::value_type value_type;
	typedef CStandartDataStorage<HldData,Parent,Target> base_storage;
public:
	CBaseDataStorage()
	{
		Inherited::m_BR.RegisterWideString("notes",&HldData::notes);
	}
	virtual ~CBaseDataStorage()
	{
	}

	const char* DCMAPAPI GetNotes()
	{
		static std::string conv_str;
		conv_str = StrD2C(Inherited::m_CLOB.GetValueW(Inherited::m_pCurrentRecord->notes));
		return conv_str.c_str();
	}

	dcmapDCSTR DCMAPAPI GetWideNotes()
	{
		dcmapDCSTR note = Inherited::m_CLOB.GetValueW(Inherited::m_pCurrentRecord->notes);
		return note?note:(dcmapDCSTR)L"";
	}

	void DCMAPAPI SetNotes(dcmapCSTR szName)
	{
		SetWideNotes(ToDStr(szName).c_str());
	}

	void DCMAPAPI SetWideNotes(dcmapDCSTR szName)
	{
		if(!szName || !szName[0])
			Inherited::m_CLOB.Delete(Inherited::m_temp.notes);
		else if(!Inherited::m_CLOB.UpdateValue(Inherited::m_temp.notes,szName))
			Inherited::m_temp.notes = *Inherited::m_CLOB.InsertValue(szName);
	}

};


template <class T,class INTRF,class Target=INTRF>
class CBaseNamedDataStorage : public CBaseDataStorage<T,INTRF,Target>
{
    typedef CBaseDataStorage<T,INTRF,Target> Inherited;
public:
	CBaseNamedDataStorage()
	{
		Inherited::m_BR.RegisterWideString("name",&value_type::name);
		Inherited::m_BR.SetParam(DCMAP_COLUMN_KEY);
	}
protected:
	typedef typename Inherited::value_type value_type;
	typedef CBaseNamedDataStorage<T,INTRF,Target> base_named_storage;
	typedef ustrimap<dstring,typename Inherited::DataList::size_type> MapIndex;
	MapIndex m_Index;
	bool DCMAPAPI SelectName(dcmapCSTR szName)
	{
		return SelectWideName(ToDStr(szName).c_str());
	}

	bool DCMAPAPI SelectWideName(dcmapDCSTR szName)
	{
		Inherited::m_pCurrentRecord=0;
		typename MapIndex::iterator it = m_Index.find(szName);
		if(it == m_Index.end())return false;
		Inherited::m_pCurrentRecord = &Inherited::m_lstData[it->second];
		return true;
	}	

	bool DCMAPAPI SelectIndex()
	{
		if(!Inherited::m_bEditIndex)return false;
		Inherited::m_bEditIndex = false;
		return SelectWideName(Inherited::m_temp.name);
	}

	bool NeedReindex(value_type const* old,value_type const* newd)
	{
		if(wcscmp(old->name,newd->name)!=0)return true;
		return Inherited::NeedReindex(old,newd);
	}

	bool IndexValue(const value_type* pDta)
	{
		if(!pDta->name[0])return false;
		typename MapIndex::iterator it = m_Index.find(pDta->name);
		if(it != m_Index.end())return false;
		m_Index[pDta->name] = pDta->id.id;
		return true;
	}

	void UnIndexValue(const value_type* pDta)
	{
		if(!pDta->name[0])return;
		m_Index.erase(pDta->name);
	}

	void ClearIndex()
	{
		m_Index.clear();
	}

	const char* DCMAPAPI GetName()
	{
		return Inherited::GetString(DCMDB_BASE_NAME);
		//return m_pCurrentRecord->name;
	};

	dcmapDCSTR DCMAPAPI GetWideName()
	{
		return Inherited::m_pCurrentRecord->name;
	};

	void DCMAPAPI SetName(const char* szName)
	{
		SetWideName(ToDStr(szName).c_str());
		//strncpy(m_temp.name,szName,value_type::name_len);
	}

	void DCMAPAPI SetWideName(dcmapDCSTR szName)
	{
		dcsncpy(Inherited::m_temp.name,szName,value_type::name_len);
		Inherited::m_temp.name[value_type::name_len-1]=0;
	}

	bool DCMAPAPI RetrieveByWideName(dcmapDCSTR szName,dcmapDBREF* pRef=0)
	{
		if(SelectWideName(szName))
		{
			if(pRef)*pRef = *Inherited::GetID();
		}
		else
		{
			value_type val;
			FillDefaults(&val);
			dcsncpy(val.name,szName,value_type::name_len-1);
			InsertData(&val,pRef);
		}
		return true;		
	}
	bool DCMAPAPI RetrieveByName(const char* szName,dcmapDBREF* pRef=0)
	{
		return RetrieveByWideName(ToDStr(szName).c_str(),pRef);
	}

};

template <class T,class INTRF,class Target=INTRF>
class CBaseGameIndexedDataStorage : public CBaseDataStorage<T,INTRF,Target>
{
    typedef CBaseDataStorage<T,INTRF,Target> Inherited;
	typedef CBaseDataStorage<T,INTRF,Target> my_parent;
public:
	CBaseGameIndexedDataStorage()
	{
		Inherited::m_BR.RegisterInt("game_id",&value_type::game_id);
		Inherited::m_BR.SetParam(DCMAP_COLUMN_KEY);
	}
protected:
	typedef dcmapDWORD id_type;
	typedef typename Inherited::value_type value_type;

	static const id_type invalid_id = 0; 

	typedef CBaseGameIndexedDataStorage<T,INTRF,Target> custom_gind_storage;
	typedef gsys::map<unsigned,typename Inherited::DataList::size_type> MapIndex;
	MapIndex m_Index;

	bool DCMAPAPI SelectGameID(id_type id)
	{
		if(!Inherited::DataLoad())return false;

		Inherited::m_pCurrentRecord=0;
		typename MapIndex::iterator it = m_Index.find(id);
		if(it == m_Index.end())return false;
		Inherited::m_pCurrentRecord = &Inherited::m_lstData[it->second];
		return true;
	}	

	bool DCMAPAPI RetrieveByGameID(id_type id,dcmapDBREF* pRef)
	{
		if(SelectGameID(id))
		{
			if(pRef)*pRef = GetData()->id;
			return true;
		}
		else
		{
			value_type val;
			FillDefaults(&val);
			val.game_id = id;
			return InsertData(&val,pRef);
		}
	}

	bool NeedReindex(value_type const* old,value_type const* newd)
	{
		if(old->game_id != newd->game_id)return true;
		return Inherited::NeedReindex(old,newd);
	}

	bool IndexValue(const value_type* pDta)
	{
		if(pDta->game_id)
		{
			typename MapIndex::iterator it = m_Index.find(pDta->game_id);
			if(it != m_Index.end())return false;
			m_Index[pDta->game_id] = pDta->id.id;
		}
		return Inherited::IndexValue(pDta);
	}

	void UnIndexValue(const value_type* pDta)
	{
		if(pDta->game_id)
		{
			m_Index.erase(pDta->game_id);
			Inherited::UnIndexValue(pDta);
		}
	}

	void ClearIndex()
	{
		m_Index.clear();
		Inherited::ClearIndex();
	}
	bool DCMAPAPI SelectIndex()
	{
		if(!Inherited::m_bEditIndex)return false;
		Inherited::m_bEditIndex = false;
		return SelectGameID(Inherited::m_temp.game_id);
	}
	dcmapCSTR DCMAPAPI GetName()
	{
		if(!Inherited::m_pCurrentRecord)return dcmapCSTR("");
		static string str;
		str = ToStr(Inherited::m_pCurrentRecord->game_id);
		return str.c_str();
	}	
	dcmapDCSTR DCMAPAPI GetWideName()
	{
		if(!Inherited::m_pCurrentRecord)return dcmapDCSTR(L"");
		static dstring str;
		str = ToDStr(Inherited::m_pCurrentRecord->game_id);
		return str.c_str();
	}

	bool DCMAPAPI SelectName(dcmapCSTR szName)
	{
		id_type id = invalid_id;
		gsys::lex_convert<id_type>(id,szName);
		if(id == invalid_id)return false;
		return SelectGameID(id);
	}

	bool DCMAPAPI SelectWideName(dcmapDCSTR szName)
	{
		id_type id = invalid_id;
		gsys::lex_convert<id_type>(id,szName);
		if(id == invalid_id)return false;
		return SelectGameID(id);
	}

	bool DCMAPAPI RetrieveByName(dcmapCSTR szName,dcmapDBREF* pRef)
	{
		id_type id = invalid_id;
		gsys::lex_convert<id_type>(id,szName);
		if(id == invalid_id)return false;
		return RetrieveByGameID(id,pRef);
	}

	bool DCMAPAPI RetrieveByWideName(dcmapDCSTR szName,dcmapDBREF* pRef)
	{
		id_type id = invalid_id;
		if(id == invalid_id)return false;
		return RetrieveByGameID(id,pRef);
	}

	void DCMAPAPI SetName(dcmapCSTR szName)
	{
		gsys::lex_convert<id_type>(Inherited::m_temp.game_id,szName);
	}

	void DCMAPAPI SetWideName(dcmapDCSTR szName)
	{
		gsys::lex_convert<id_type>(Inherited::m_temp.game_id,szName);
	}

};

template <class T,class INTRF,class Target=INTRF>
class CBaseNamedGameIndexedDataStorage : public CBaseNamedDataStorage<T,INTRF,Target>
{
    typedef CBaseNamedDataStorage<T,INTRF,Target> Inherited;

	typedef CBaseNamedDataStorage<T,INTRF,Target> my_parent;
public:
	CBaseNamedGameIndexedDataStorage()
	{
		//m_BR.RegisterInt("game_id",&value_type::game_id);
		//m_BR.SetParam(DCMAP_COLUMN_KEY);
	}
protected:
	typedef dcmapDWORD id_type;
	typedef typename Inherited::value_type value_type;

	typedef CBaseGameIndexedDataStorage<T,INTRF,Target> custom_gind_storage;
	typedef gsys::map<unsigned,typename Inherited::DataList::size_type> MapIndex;
	MapIndex m_Index;

	bool DCMAPAPI SelectGameID(id_type id)
	{
		if(!Inherited::DataLoad())return false;

		Inherited::m_pCurrentRecord=0;
		typename MapIndex::iterator it = m_Index.find(id);
		if(it == m_Index.end())return false;
		Inherited::m_pCurrentRecord = &Inherited::m_lstData[it->second];
		return true;
	}	

	bool NeedReindex(value_type const* old,value_type const* newd)
	{
		if(old->game_id != newd->game_id)return true;
		return Inherited::NeedReindex(old,newd);
	}

	bool IndexValue(const value_type* pDta)
	{
		if(pDta->game_id > 0)
		{
			if(!pDta->name[0])return false;
			typename MapIndex::iterator it = m_Index.find(pDta->game_id);
			if(it != m_Index.end())return false;
			m_Index[pDta->game_id] = pDta->id.id;
		}
		return my_parent::IndexValue(pDta);
	}

	void UnIndexValue(const value_type* pDta)
	{
		if(pDta->game_id > 0)
		{
			m_Index.erase(pDta->game_id);
		}
		my_parent::UnIndexValue(pDta);
	}

	void ClearIndex()
	{
		m_Index.clear();
		my_parent::ClearIndex();
	}
	/*
	bool DCMAPAPI SelectIndex()
	{
		if(!m_bEditIndex)return false;
		m_bEditIndex = false;
		return SelectGameID(m_temp.game_id);
	}*/
};

template <class T,class INTRF,class Target=INTRF>
class CBaseChildTableDataStorage : public CStandartDataStorage<T,INTRF,Target>
{
    typedef CStandartDataStorage<T,INTRF,Target> Inherited;

	typedef CStandartDataStorage<T,INTRF,Target> parent;
	std::string m_strParentTable;
public:
	CBaseChildTableDataStorage(dcmapCSTR ParentName,dcmapCSTR ParentFieldName="parent")
	{
		m_strParentTable = ParentName;
		Inherited::m_BR.RegisterReference(ParentFieldName,&value_type::parent,ParentName,DCMDB_BASE_PARENT);
		Inherited::m_BR.Alias("parent");
		m_ParentStart = m_Index.end();
		m_bOrderChanged = false;
	}
protected:

	typedef typename Inherited::value_type value_type;
	typedef std::multimap<dcmapDBREF,typename Inherited::DataList::size_type> MapIndex;
	MapIndex m_Index;

	typename MapIndex::iterator m_ParentStart;
	typename MapIndex::iterator m_ParentEnd;

	bool m_bOrderChanged;

	void OnDataUpdate()
	{
		if(m_bOrderChanged)
		{
			ClearIndex();
			_foreach(typename Inherited::DataList,Inherited::m_lstData,it) // Set new loaded index
			{
				IndexValue(&(*it));
			}
		}
		m_bOrderChanged = false;
	}
	bool NeedReindex(value_type const* old,value_type const* newd)
	{
		if(old->parent != newd->parent)return true;
		return parent::NeedReindex(old,newd);
	}
	bool IndexValue(const value_type* pDta)
	{
		if(!parent::IndexValue(pDta))return false;
		if(!pDta->parent.Valid())return false;

		m_Index.insert(typename MapIndex::value_type((pDta->parent),pDta->id.id));
		m_ParentStart = m_Index.end();
		return true;
	}

	void UnIndexValue(const value_type* pDta)
	{
		parent::UnIndexValue(pDta);
		typename MapIndex::iterator it = m_Index.find(pDta->parent);
		while(it != m_Index.end() && it->second != pDta->id.id)++it;
		if(it != m_Index.end())m_Index.erase(it);
		m_ParentStart = m_Index.end();
	}

	void ClearIndex()
	{
		parent::ClearIndex();
		m_Index.clear();
		m_ParentStart = m_Index.end();
	}

	void DCMAPAPI PostInit()
	{
		Inherited::m_pParentDataStorage = (Inherited::m_pDatabase)->GetDataStorage(m_strParentTable.c_str());
		Inherited::m_pParentDataStorage->AddChildTable(this);
		Inherited::PostInit();
	}

public:
	void DCMAPAPI ClearUnparented()
	{
		if(!Inherited::m_pParentDataStorage)return;
		static gsys::raw_data_buffer<dcmapDBREF,512,512> buf;
		buf.reset(0);
		buf.reserve(Inherited::NumRecords());
		if(Inherited::SelectAll())
		do
		{
			buf.push_back(*Inherited::GetID());
		}			
		while(Next());
		int num = buf.size();

		Inherited::BeginUpdate();
		for(int i=0;i<num;i++)
		{
			if(!Inherited::SelectID(buf[i]))continue;
			dcmapDBREF const* pRef = GetParent();
			if(!Inherited::m_pParentDataStorage->SelectID(*pRef))Inherited::Delete();
		}
		Inherited::EndUpdate();
	}
	void DCMAPAPI ClearByParent(dcmapDBREF const& pRef)
	{
		if(!Inherited::DataLoad())return;

		if(!Inherited::m_pParentDataStorage)return;
		static gsys::raw_data_buffer<dcmapDBREF,512,512> buf;
		buf.reset(0);
		buf.reserve(Inherited::NumRecords());
		if(SelectByParentID(pRef))
		do
		{
			buf.push_back(*Inherited::GetID());
		}			
		while(Next());
		int num = buf.size();

		Inherited::BeginUpdate();
		for(int i=0;i<num;i++)
		{
			if(!Inherited::SelectID(buf[i]))continue;
			if(*GetParent() == pRef)Inherited::Delete();
		}
		Inherited::EndUpdate();
	}

	bool DCMAPAPI OrderSwap(dcmapDBREF const& r1,dcmapDBREF const& r2)
	{
		bool pre = m_bOrderChanged;
		m_bOrderChanged = true;
		if(!parent::OrderSwap(r1,r2))
		{
			m_bOrderChanged = pre;
			return false;
		}
		return true;
	}

	const dcmapDBREF* DCMAPAPI GetParent()
	{
		return &Inherited::m_pCurrentRecord->parent;
	}
	void DCMAPAPI SetParent(dcmapDBREF const& Ref)
	{
		Inherited::m_temp.parent = Ref;
	}

	bool DCMAPAPI SelectByParentID(dcmapDBREF const& ref)
	{
		if(!Inherited::DataLoad())return false;

		m_ParentStart = m_Index.lower_bound(ref);
		m_ParentEnd = m_Index.upper_bound(ref);
		if(m_ParentStart == m_ParentEnd)
		{
			m_ParentStart = m_Index.end();
			return false;
		}
		Inherited::m_pCurrentRecord = &Inherited::m_lstData[m_ParentStart->second];
		return true;
	}

	// Next selected record
	bool DCMAPAPI Next()
	{
		if(m_ParentStart == m_Index.end()) return parent::Next();

		m_ParentStart++;
		if(m_ParentStart == m_ParentEnd)
		{
			m_ParentStart = m_Index.end();
			Inherited::m_pCurrentRecord=0;
			return false;
		}
		Inherited::m_pCurrentRecord = &Inherited::m_lstData[m_ParentStart->second];
		return true;
	}
};

template <class T,class INTRF,class Target=INTRF>
class CLocalyNamedChildTableDataStorage : public CBaseChildTableDataStorage<T,INTRF,Target>
{
    typedef CBaseChildTableDataStorage<T,INTRF,Target> Inherited;
	typedef CBaseChildTableDataStorage<T,INTRF,Target> parent;
protected:
	typedef typename Inherited::value_type value_type;
public:
	CLocalyNamedChildTableDataStorage(char* ParentName,char* ParentFieldName="parent"):
	  parent(ParentName,ParentFieldName)
	{
		Inherited::m_BR.SetParam(DCMAP_COLUMN_KEY);
		Inherited::m_BR.RegisterWideString("name",&value_type::name,DCMDB_LNCT_NAME);
		Inherited::m_BR.SetParam(DCMAP_COLUMN_KEY);
	}

	const char* DCMAPAPI GetName()
	{
		static std::string conv;
		conv = WideToString(GetWideName());
		return conv.c_str();
	};

	dcmapDCSTR DCMAPAPI GetWideName()
	{
		return Inherited::m_pCurrentRecord->name;
	};

	void DCMAPAPI SetName(const char* szName)
	{
		 SetWideName(ToWide(szName).c_str());
	}

	void DCMAPAPI SetWideName(dcmapDCSTR szName)
	{
		 wcsncpy(Inherited::m_temp.name,szName,value_type::name_len);
		 Inherited::m_temp.name[value_type::name_len-1]=0;
	}

protected:
	struct LNCIndex
	{
		dcmapDBREF ref;
		dstring name;
		friend bool operator < (LNCIndex const& l1,LNCIndex const& l2)
		{
			if(l1.ref.id < l2.ref.id)return true;
			if(l1.ref.id > l2.ref.id)return false;
			return ustring_casecompare(l1.name.c_str(),l2.name.c_str())<0;
		}
		LNCIndex(){};
		LNCIndex(dcmapDBREF const& _ref,dstring const& _name)
		{
			ref = _ref;
			name = _name;
		}
	};
	typedef std::map<LNCIndex,int> MapLNIndex;
	MapLNIndex m_LNIndex;
protected:
	bool TestNameMod(const value_type* pDta,dstring const& mod)
	{
		if(mod.empty())return false;
		LNCIndex ind(pDta->parent,mod);
		typename MapLNIndex::iterator it = m_LNIndex.find(ind);
		return it == m_LNIndex.end();
	}
	bool FixIndexValue(value_type* pDta)
	{
		if(TestNameMod(pDta,pDta->name))return true;
		for(int i=0;i<999;i++)
		{
			dstring name = pDta->name+(L"("+gsys::to_wstr(i)+L")");
			if(TestNameMod(pDta,name))
			{
				wcsncpy(pDta->name,name.c_str(),value_type::name_len);
				return true;
			}
		}
		return false;
	}
	bool NeedReindex(value_type const* old,value_type const* newd)
	{
		if(ustring_casecompare(old->name,newd->name)!=0)return true;
		return parent::NeedReindex(old,newd);
	}
	bool IndexValue(const value_type* pDta)
	{
		if(!pDta->name[0])return false;
		LNCIndex ind(pDta->parent,pDta->name);
		typename MapLNIndex::iterator it = m_LNIndex.find(ind);
		if(it == m_LNIndex.end())
		{
			if(!parent::IndexValue(pDta))return false;
			m_LNIndex[ind] = pDta->id.id;
			return true;
		}
		return false;
	}
	void UnIndexValue(const value_type* pDta)
	{
		parent::UnIndexValue(pDta);
		LNCIndex ind(pDta->parent,pDta->name);
		typename MapLNIndex::iterator it = m_LNIndex.find(ind);
		if(it != m_LNIndex.end())
		{
			m_LNIndex.erase(it);
		}
	}
	void ClearIndex()
	{
		parent::ClearIndex();
		m_LNIndex.clear();
	}

	bool DCMAPAPI SelectIndex()
	{
		if(!Inherited::DataLoad())return false;

		if(parent::SelectIndex())return true;

		LNCIndex ind(Inherited::m_temp.parent,Inherited::m_temp.name);
		typename MapLNIndex::iterator it = m_LNIndex.find(ind);
		if(it != m_LNIndex.end())
		{
			Inherited::m_pCurrentRecord = &Inherited::m_lstData[it->second];
			return true;
		}	
		return false;
	}
};

template <class T,class INTRF,class Target=INTRF>
class CIndexedChildTableDataStorage : public CBaseChildTableDataStorage<T,INTRF,Target>
{
    typedef CBaseChildTableDataStorage<T,INTRF,Target> Inherited;
	typedef CBaseChildTableDataStorage<T,INTRF,Target> my_parent;
public:
	CIndexedChildTableDataStorage(dcmapCSTR ParentName,dcmapCSTR ParentFieldName="parent"): my_parent(ParentName,ParentFieldName)
	{
		Inherited::m_BR.RegisterInt("game_id",&value_type::game_id);
		Inherited::m_BR.SetParam(DCMAP_COLUMN_KEY);
	}
protected:
	typedef CBaseGameIndexedDataStorage<T,INTRF,Target> parent;
	typedef typename Inherited::value_type value_type;

	typedef dcmapDWORD id_type;
	typedef CBaseGameIndexedDataStorage<T,INTRF,Target> custom_gind_storage;
	typedef gsys::map<unsigned,typename Inherited::DataList::size_type> MapGameIndex;
	MapGameIndex m_GameIndex;

protected:
	bool DCMAPAPI SelectGameID(id_type id)
	{
		if(!Inherited::DataLoad())return false;

		Inherited::m_pCurrentRecord=0;
		typename MapGameIndex::iterator it = m_GameIndex.find(id);
		if(it == m_GameIndex.end())return false;
		Inherited::m_pCurrentRecord = &Inherited::m_lstData[it->second];
		return true;
	}	

	bool NeedReindex(value_type const* old,value_type const* newd)
	{
		if(old->game_id != newd->game_id)return true;
		return Inherited::NeedReindex(old,newd);
	}

	bool IndexValue(const value_type* pDta)
	{
        if(pDta->game_id)
        {
		    typename MapGameIndex::iterator it = m_GameIndex.find(pDta->game_id);
		    if(it != m_GameIndex.end())return false;
		    if(!Inherited::IndexValue(pDta))return false;
		    m_GameIndex[pDta->game_id] = pDta->id.id;
            return true;
        }
        return Inherited::IndexValue(pDta);
	}

	void UnIndexValue(const value_type* pDta)
	{
		if(pDta->game_id != 0)m_GameIndex.erase(pDta->game_id);
		my_parent::UnIndexValue(pDta);
	}

	void ClearIndex()
	{
		m_GameIndex.clear();
		my_parent::ClearIndex();
	}
	bool DCMAPAPI SelectIndex()
	{
		if(!Inherited::DataLoad())return false;
		if(!Inherited::m_bEditIndex)return false;
		Inherited::m_bEditIndex = false;
		return SelectGameID(Inherited::m_temp.game_id);
	}

	dcmapCSTR DCMAPAPI GetName()
	{
		static std::string name;
		name = gsys::to_str(Inherited::m_pCurrentRecord->game_id);
		return name.c_str();
	}

	dcmapDCSTR DCMAPAPI GetWideName()
	{
		static dstring name;
		name = gsys::to_wstr(Inherited::m_pCurrentRecord->game_id);
		return name.c_str();
	}
};

template <class T,class INTRF,class Target=INTRF>
class CLocalyIndexedChildTableDataStorage : public CBaseChildTableDataStorage<T,INTRF,Target>
{
    typedef CBaseChildTableDataStorage<T,INTRF,Target> Inherited;
	typedef CBaseChildTableDataStorage<T,INTRF,Target> my_parent;
public:
	CLocalyIndexedChildTableDataStorage(char* ParentName,char* ParentFieldName="parent"):
	  Inherited(ParentName,ParentFieldName)
	  {
		  Inherited::m_BR.SetParam(DCMAP_COLUMN_KEY);
		  Inherited::m_BR.RegisterInt("game_id",&value_type::game_id,DCMDB_BASE_GAMEID);
		  Inherited::m_BR.SetParam(DCMAP_COLUMN_KEY);
	  }
protected:
	typedef typename Inherited::value_type value_type;

	struct LNCIndex
	{
		dcmapDBREF ref;
		dcmapDWORD id;
		friend bool operator < (LNCIndex const& l1,LNCIndex const& l2)
		{
			if(l1.id < l2.id)return true;
			if(l1.id > l2.id)return false;
			return l1.ref.id < l2.ref.id;
		}
		LNCIndex(){};
		LNCIndex(dcmapDBREF const& _ref,dcmapDWORD _id)
		{
			ref = _ref;
			id  = _id;
		}
	};
	typedef std::map<LNCIndex,int> MapLNIndex;
	MapLNIndex m_LNIndex;
protected:

	bool NeedReindex(value_type const* old,value_type const* newd)
	{
		if(old->game_id != newd->game_id)return true;
		return Inherited::NeedReindex(old,newd);
	}
	bool IndexValue(const value_type* pDta)
	{
		// allow 0 indexes for temporary objects
		if(pDta->game_id==0)
		{
			return my_parent::IndexValue(pDta);
		}

		LNCIndex ind(pDta->parent,pDta->game_id);
		typename MapLNIndex::iterator it = m_LNIndex.find(ind);
		if(it == m_LNIndex.end())
		{
			if(!my_parent::IndexValue(pDta))return false;
			m_LNIndex[ind] = pDta->id.id;
			return true;
		}
		return false;
	}
	void UnIndexValue(const value_type* pDta)
	{
		my_parent::UnIndexValue(pDta);
		if(pDta->game_id==0)return;

		LNCIndex ind(pDta->parent,pDta->game_id);
		typename MapLNIndex::iterator it = m_LNIndex.find(ind);
		if(it != m_LNIndex.end())
		{
			m_LNIndex.erase(it);
		}
	}
	void ClearIndex()
	{
		my_parent::ClearIndex();
		m_LNIndex.clear();
	}

	dcmapCSTR DCMAPAPI GetName()
	{
		static std::string name;
		name = gsys::to_str(Inherited::m_pCurrentRecord->game_id);
		return name.c_str();
	}

	dcmapDCSTR DCMAPAPI GetWideName()
	{
		static dstring name;
		name = gsys::to_wstr(Inherited::m_pCurrentRecord->game_id);
		return name.c_str();
	}

	bool DCMAPAPI SelectIndex()
	{
		if(!Inherited::DataLoad())return false;

		if(Inherited::SelectIndex())return true;

		LNCIndex ind(Inherited::m_temp.parent,Inherited::m_temp.game_id);
		typename MapLNIndex::iterator it = m_LNIndex.find(ind);
		if(it != m_LNIndex.end())
		{
			Inherited::m_pCurrentRecord = &Inherited::m_lstData[it->second];
			return true;
		}	
		return false;
	}
	virtual bool DCMAPAPI SelectGameID(dcmapDBREF const& ParentRef,dcmapDWORD id)
	{
		if(!Inherited::DataLoad())return false;

		LNCIndex ind(ParentRef,id);
		typename MapLNIndex::iterator it = m_LNIndex.find(ind);
		if(it != m_LNIndex.end())
		{
			Inherited::m_pCurrentRecord = &Inherited::m_lstData[it->second];
			return true;
		}	
		return false;		
	}
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <class T,class INTRF,class Target=INTRF>
class CBaseConnectionTableDataStorage : public CStandartDataStorage<T,INTRF,Target>
{
	typedef CStandartDataStorage<T,INTRF,Target> Inherited;
	typedef CStandartDataStorage<T,INTRF,Target> parent;
protected:
	std::string m_strParent1Table;
	std::string m_strParent2Table;
public:
	CBaseConnectionTableDataStorage()
	{
		m_ParentStart = m_Parent1Index.end();
		m_ParentEnd    = m_Parent1Index.end();

		m_bOrderChanged = false;
	}
protected:
	typedef typename Inherited::value_type value_type;

	typedef std::pair<dcmapDBREF,dcmapDBREF> IndexType;

	typedef std::multimap<dcmapDBREF,typename Inherited::DataList::size_type> MapParentIndex;
	typedef std::map<IndexType,typename Inherited::DataList::size_type> MapIndex;

	MapParentIndex m_Parent1Index;
	MapParentIndex m_Parent2Index;
	MapIndex m_Index;

	typename MapParentIndex::iterator m_ParentStart;
	typename MapParentIndex::iterator m_ParentEnd;

	bool m_bOrderChanged;

	void OnDataUpdate()
	{
		if(m_bOrderChanged)
		{
			ClearIndex();
			_foreach(typename Inherited::DataList,Inherited::m_lstData,it) // Set new loaded index
			{
				IndexValue(&(*it));
			}
		}
		m_bOrderChanged = false;
	}

	bool NeedReindex(value_type const* old,value_type const* newd)
	{
		if(old->c1 != newd->c1 || old->c2 != newd->c2)return true;
		return Inherited::NeedReindex(old,newd);
	}

	bool IndexValue(const value_type* pDta)
	{
		if(!pDta->c1.Valid())return false;
		if(!pDta->c2.Valid())return false;

		if(m_Index.find(IndexType(pDta->c1,pDta->c2)) != m_Index.end())return false;
		if(!parent::IndexValue(pDta))return false;
		m_Index[IndexType(pDta->c1,pDta->c2)] = pDta->id.id;

		m_Parent1Index.insert(typename MapParentIndex::value_type((pDta->c1),pDta->id.id));
		m_Parent2Index.insert(typename MapParentIndex::value_type((pDta->c2),pDta->id.id));

		m_ParentStart = m_Parent1Index.end();
		m_ParentEnd    = m_Parent1Index.end();
		return true;
	}

	void UnIndexValue(const value_type* pDta)
	{
		parent::UnIndexValue(pDta);
		typename MapParentIndex::iterator it;

		it = m_Parent1Index.find(pDta->c1);
		while(it != m_Parent1Index.end() && it->second != pDta->id.id)++it;
		if(it != m_Parent1Index.end())m_Parent1Index.erase(it);

		it = m_Parent2Index.find(pDta->c2);
		while(it != m_Parent2Index.end() && it->second != pDta->id.id)++it;
		if(it != m_Parent2Index.end())m_Parent2Index.erase(it);

		m_Index.erase(IndexType(pDta->c1,pDta->c2));

		m_ParentStart = m_Parent1Index.end();
		m_ParentEnd    = m_Parent1Index.end();
	}

	void ClearIndex()
	{
		parent::ClearIndex();
		m_Index.clear();
		m_Parent1Index.clear();
		m_Parent1Index.clear();

		m_ParentStart = m_Parent1Index.end();
		m_ParentEnd    = m_Parent1Index.end();
	}

	void DCMAPAPI PostInit()
	{
		Inherited::m_pParentDataStorage = (Inherited::m_pDatabase)->GetDataStorage(m_strParent2Table.c_str());
		if(Inherited::m_pParentDataStorage)Inherited::m_pParentDataStorage->AddChildTable(this);

		Inherited::m_pParentDataStorage = (Inherited::m_pDatabase)->GetDataStorage(m_strParent1Table.c_str());
		if(Inherited::m_pParentDataStorage)Inherited::m_pParentDataStorage->AddChildTable(this);

		Inherited::PostInit();
	}

public:
	/*	void DCMAPAPI ClearUnparented()
	{
	if(!Inherited::m_pParentDataStorage)return;
	static gsys::raw_data_buffer<dcmapDBREF,512,512> buf;
	buf.reset(0);
	buf.reserve(Inherited::NumRecords());
	if(Inherited::SelectAll())
	do
	{
	buf.push_back(*Inherited::GetID());
	}			
	while(Next());
	int num = buf.size();

	Inherited::BeginUpdate();
	for(int i=0;i<num;i++)
	{
	if(!Inherited::SelectID(buf[i]))continue;
	dcmapDBREF const* pRef = GetParent();
	if(!Inherited::m_pParentDataStorage->SelectID(*pRef))Inherited::Delete();
	}
	Inherited::EndUpdate();
	}
	*/
	void DCMAPAPI ClearByParent(dcmapDBREF const& pRef)
	{
		if(!Inherited::DataLoad())return;

		if(!Inherited::m_pParentDataStorage)return;
		static gsys::raw_data_buffer<dcmapDBREF,512,512> buf;
		buf.reset(0);
		buf.reserve(Inherited::NumRecords());
		if(SelectByParentID(pRef))
			do
			{
				buf.push_back(*Inherited::GetID());
			}			
			while(Next());
			int num = buf.size();

			Inherited::BeginUpdate();
			for(int i=0;i<num;i++)
			{
				if(!Inherited::SelectID(buf[i]))continue;
				if(*GetParent() == pRef)Inherited::Delete();
			}
			Inherited::EndUpdate();
	}

	bool DCMAPAPI OrderSwap(dcmapDBREF const& r1,dcmapDBREF const& r2)
	{
		bool pre = m_bOrderChanged;
		m_bOrderChanged = true;
		if(!parent::OrderSwap(r1,r2))
		{
			m_bOrderChanged = pre;
			return false;
		}
		return true;
	}

	bool DCMAPAPI SelectConnect(dcmapDBREF const& c1,dcmapDBREF const& c2)
	{
		if(!Inherited::DataLoad())return false;

		MapIndex::iterator it = m_Index.find(IndexType(c1,c2));
		if(it ==  m_Index.end())return false;
		Inherited::m_pCurrentRecord = &Inherited::m_lstData[it->second];
		return true;
	}

	bool DCMAPAPI SelectByParentID(dcmapDBREF const& ref)
	{
		if(!Inherited::DataLoad())return false;

		bool sel=true;

		m_ParentStart = m_Parent1Index.lower_bound(ref);
		m_ParentEnd = m_Parent1Index.upper_bound(ref);
		if(m_ParentStart == m_ParentEnd)
		{
			m_ParentStart = m_Parent2Index.lower_bound(ref);
			m_ParentEnd = m_Parent2Index.upper_bound(ref);
			if(m_ParentStart == m_ParentEnd)return false;
		}
		Inherited::m_pCurrentRecord = &Inherited::m_lstData[m_ParentStart->second];
		return true;
	}

	// Next selected record
	bool DCMAPAPI Next()
	{
		if(m_ParentStart == m_ParentEnd) return parent::Next();

		m_ParentStart++;
		if(m_ParentStart == m_ParentEnd)
		{
			m_ParentStart = m_Parent1Index.end();
			m_ParentEnd    = m_Parent1Index.end();

			Inherited::m_pCurrentRecord=0;
			return false;
		}
		Inherited::m_pCurrentRecord = &Inherited::m_lstData[m_ParentStart->second];
		return true;
	}
};
