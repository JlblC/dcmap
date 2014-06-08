#pragma once

#include "../dcmapsys.h"
#include <g_std_file.h>
#include <g_hash_map.h>


class CBLOBStorage
{
	static const int MaxdataSize = 0xffffff;
	dcmapQWORD m_modIndex;
	struct DataRecord
	{
		dcmapBYTE* pData;
		dcmapDWORD szData;
		dcmapDWORD szAllocated;
	};
	typedef gsys::indexed_list<DataRecord,dcmapDWORD,dcmapDWORD,DCMapInvalidReferenceAge,1> DataList;
	DataList m_lstData;

	void DataChanged()
	{
		//_ASSERT(m_lstData.check_structure());
		m_modIndex++;
	}

public:
	CBLOBStorage(void);
	~CBLOBStorage(void);
public:
	dcmapCSTR GetValue(dcmapDBREF const& ref);
	dcmapDCSTR GetValueW(dcmapDBREF const& ref);
	int GetValue(dcmapDBREF const& ref,dcmapBYTE** pData);
	bool UpdateValue(dcmapDBREF const& ref,char const* szData);
	bool UpdateValue(dcmapDBREF const& ref,dcmapDCSTR szData);
	bool UpdateValue(dcmapDBREF const& ref,dcmapBYTE const* szData,int size);
	bool Delete(dcmapDBREF const& ref);
	dcmapDBREF const* InsertValue(char const* szData);
	dcmapDBREF const* InsertValue(dcmapDCSTR szData);
	dcmapDBREF const* InsertValue(dcmapBYTE const* szData,int sz);

	bool LoadDataFromFile(gsys::gfile *file,dcmapDWORD DBVER);
	bool SaveDataToFile(gsys::gfile *file);

	bool CheckSanity();
	void Clear();
};
