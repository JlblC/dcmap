#include "stdafx.h"
#include "BlobStorage.h"
#include <time.h>
#include <stdlib.h>
#include "utils.h"

dcmapDWORD GenerateMagic(dcmapQWORD modIndex)
{
	dcmapDWORD base = (((dcmapDWORD)modIndex)&(~0xff));
	dcmapDWORD tm =  ~(unsigned)time(NULL)<<1;
	dcmapDWORD pre = base+tm+rand();
	if(pre == 0)pre += ((dcmapDWORD)rand())+1;
	return pre;
}


CBLOBStorage::CBLOBStorage(void)
{
	m_modIndex=0;
}

CBLOBStorage::~CBLOBStorage(void)
{
	Clear();
}

int CBLOBStorage::GetValue(dcmapDBREF const& ref,dcmapBYTE** pData)
{
	if(ref.id >= m_lstData.capacity()) return 0;
	if(ref.age == 0)return 0;
	if(m_lstData.tag(ref.id) != ref.age) return 0;
	*pData = m_lstData[ref.id].pData;
	int sz = m_lstData[ref.id].szData;
	_ASSERTPT(pData,sz);
	return sz;
}

dcmapCSTR CBLOBStorage::GetValue(dcmapDBREF const& ref)
{
	if(ref.id >= m_lstData.capacity()) return "";
	if(ref.age == 0)return "";
	if(m_lstData.tag(ref.id) != ref.age) return "";
	char* pData = (char*)m_lstData[ref.id].pData;
	_ASSERTPT(pData,m_lstData[ref.id].szData);
	return pData;
}
dcmapDCSTR CBLOBStorage::GetValueW(dcmapDBREF const& ref)
{
	if(ref.id >= m_lstData.capacity()) return dcmapDT("");
	if(ref.age == 0)return dcmapDT("");
	if(m_lstData.tag(ref.id) != ref.age) return dcmapDT("");
	dcmapDCHAR* pData = (dcmapDCHAR*)m_lstData[ref.id].pData;
	_ASSERTPT(pData,m_lstData[ref.id].szData);
	return pData;
}

bool CBLOBStorage::UpdateValue(dcmapDBREF const& ref,dcmapBYTE const* szData,int size)
{
	//_ASSERT(m_lstData.check_structure());
	if(!ref.ValidAge())return 0;
	if(ref.id >= m_lstData.capacity()) return 0;
	if(m_lstData.tag(ref.id) != ref.age) return 0;
	DataRecord& rec = m_lstData[ref.id];
	if(rec.szAllocated >= size)
	{
		memcpy(rec.pData,szData,size);
		rec.szData = size;
	}
	else
	{
		rec.szAllocated = (size/8+1)*8;
		rec.szData = size;
		delete[] rec.pData;
		rec.pData = new dcmapBYTE[rec.szAllocated];
		memcpy(rec.pData,szData,size);
	}
	DataChanged();
	return true;
}

bool CBLOBStorage::UpdateValue(dcmapDBREF const& ref,dcmapCSTR szData)
{
	return UpdateValue(ref,(dcmapBYTE const*)szData,strlen(szData)+1);
}
bool CBLOBStorage::UpdateValue(dcmapDBREF const& ref,dcmapDCSTR szData)
{
	return UpdateValue(ref,(dcmapBYTE const*)szData,(dcslen(szData)+1)*sizeof(dcmapDCHAR));
}

bool  CBLOBStorage::Delete(dcmapDBREF const& ref)
{
	if(!ref.ValidAge())return 0;
	if(ref.id >= m_lstData.capacity()) return 0;
	if(m_lstData.tag(ref.id) != ref.age) return 0;
	DataRecord& rec = m_lstData[ref.id];
	delete[] rec.pData;
	rec.pData=0;
	m_lstData.erase(ref.id);
	DataChanged();
	return true;
}

static dcmapDBREF inv_ref;

dcmapDBREF const* CBLOBStorage::InsertValue(dcmapBYTE const* szData,int sz)
{
	if(!szData || !sz)return &inv_ref;
	DataRecord rec;
	rec.szData = sz;
	rec.szAllocated = (sz/8+1)*8;
	rec.pData = new dcmapBYTE[rec.szAllocated];
	memcpy(rec.pData,szData,sz);
	m_lstData.push_back(rec);
	dcmapDWORD tag = GenerateMagic(m_modIndex);
	m_lstData.tag(m_lstData.back_id()) = tag;
	static dcmapDBREF rv;
	rv = dcmapDBREF(m_lstData.back_id(),tag);
	DataChanged();
	return &rv;
}

dcmapDBREF const* CBLOBStorage::InsertValue(char const* szData)
{
	return InsertValue((dcmapBYTE const*)szData,(strlen(szData)+1)*sizeof(dcmapCHAR));
}

dcmapDBREF const* CBLOBStorage::InsertValue(dcmapDCSTR szData)
{
	return InsertValue((dcmapBYTE const*)szData,(dcslen(szData)+1)*sizeof(dcmapDCHAR));
}

struct CLOBHeader
{
	//char magic[4];
	dcmapFOURCC magic;
	int count;
	dcmapQWORD modIndex;
	int reserved[4];
};

static const dcmapFOURCC c_BlobMagic = DCMAP_MAKEFOURCC('C','L','B','1');

bool CBLOBStorage::LoadDataFromFile(gsys::gfile *file,dcmapDWORD DBVER)
{
	Clear();

	CLOBHeader head;
	*file >> head;
	if(head.magic != c_BlobMagic)return false;

	m_modIndex = head.modIndex;
	m_lstData.reserve(head.count);
	for(int i=0;i<head.count;i++)
	{
		dcmapDBREF ref;
		*file >> ref;
		DataRecord rec;
		*file >> rec.szData;
		rec.szAllocated = rec.szData;
		rec.pData = new dcmapBYTE[rec.szAllocated];
		file->tread(rec.pData,rec.szData);
		m_lstData.restore_element(ref.id,rec);
		m_lstData.tag(ref.id) = ref.age;
	}
	_ASSERT(m_lstData.check_structure());
	return true;
}

bool CBLOBStorage::CheckSanity()
{
	return m_lstData.check_structure();
}

void CBLOBStorage::Clear()
{
	_ASSERT(m_lstData.check_structure());
	int num=0;
	int nm=m_lstData.size();
	_foreach(DataList,m_lstData,it)
	{
		if(num >= nm)break; // infinite loop protection
		if(it->pData)delete[] it->pData;
		num++;
	}
	m_lstData.clear();
}

bool CBLOBStorage::SaveDataToFile(gsys::gfile *file)
{
	_ASSERT(m_lstData.check_structure());
	CLOBHeader head;
	memset(&head,0,sizeof(head));
	head.magic = c_BlobMagic;

	head.modIndex = m_modIndex;

	head.count = m_lstData.size();
	*file << head;
	_foreach(DataList,m_lstData,it)
	{
		dcmapDBREF ref;
		ref.id = it;
		ref.age = m_lstData.tag(it);
		*file << ref;
		*file << it->szData;
		file->twrite(it->pData,it->szData);
	}
	return true;
}