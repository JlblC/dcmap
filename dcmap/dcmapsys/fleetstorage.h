#pragma once
#include "../dcmapsys.h"
#include "BaseDataStorage.h"
#include <stack>

class CFleetsStorage : public CBaseGameIndexedDataStorage<DCMapFleetInfo,IDCMapFleetsDataStorage>
{
	typedef CBaseGameIndexedDataStorage<DCMapFleetInfo,IDCMapFleetsDataStorage> Inherited;

	struct NameIndex
	{
		NameIndex(){};
		explicit NameIndex(DCMapFleetInfo const* pInf)
		{
			sind = pInf->sortindex;
			name = pInf->caption;
		}
		NameIndex(int ind,dcmapDCSTR str)
		{
			sind=ind;
			name = str;
		}
		int sind;
		wstring name;
		friend bool operator < (NameIndex const& l,NameIndex const& r)
		{
			if(!l.sind || !r.sind) return l.name < r.name;
			if(l.sind == r.sind) return l.name < r.name;
			return l.sind < r.sind;
		}
	};    
	typedef std::multimap<NameIndex,DataList::size_type> GroupNameIndex;
	
	typedef std::map<dcmapDBREF,GroupNameIndex> GroupIndex;
	GroupIndex m_GroupIndex;
	GroupNameIndex m_GroupDummie;

	GroupNameIndex::iterator m_GroupStart;
	GroupNameIndex::iterator m_GroupEnd;

	typedef std::pair<GroupNameIndex::iterator,GroupNameIndex::iterator> SelState;
	std::stack<SelState> m_SelStack;

    bool IndexValue(const value_type* pDta);
    void UnIndexValue(const value_type* pDta);
	void OnDelete(value_type* pDta);
	void ClearIndex();
    dcmapDBREF m_RootGroup;

public:
	CFleetsStorage();
	dcmapCSTR DCMAPAPI Name(){return "fleets";}

	dcmapWCSTR DCMAPAPI GetDisplayName();

	dcmapDBREF* DCMAPAPI GetRootGroup(){ return &m_RootGroup; }
    void SetRootGroup(dcmapDBREF val) { m_RootGroup = val; }

	bool DCMAPAPI SelectGroup(dcmapDBREF const& ref);
	bool DCMAPAPI SelectGroupNames(dcmapDBREF const& ref,int sind,dcmapDCSTR name);

	int DCMAPAPI GroupSize(dcmapDBREF const& ref);

	bool DCMAPAPI Next();

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);

	void DCMAPAPI FillDefaults(value_type* pinf);

	void DCMAPAPI PushGroupSelectState()
	{
		m_SelStack.push(SelState(m_GroupStart,m_GroupEnd));
	}
	void DCMAPAPI PopGroupSelectState()
	{
		m_GroupStart = m_SelStack.top().first;
		m_GroupEnd = m_SelStack.top().second;
		m_SelStack.pop();
	}
	int DCMAPAPI TableParams(){return DCMAP_TABLE_INTERNAL | Inherited::TableParams();}
	bool NeedReindex(value_type const* old,value_type const* newd);
	bool OnTestDataConvert(void const* pData,int size,dcmapWORD ver,value_type & res);
};
DCMAP_SPEC_INTERFACE(CFleetsStorage);


class CProjectsStorage : public CLocalyIndexedChildTableDataStorage<DCMapProjectInfo,IDCMapProjectDataStorage>
{
	typedef CLocalyIndexedChildTableDataStorage<DCMapProjectInfo,IDCMapProjectDataStorage> Inherited;
public:
	CProjectsStorage();
	dcmapCSTR DCMAPAPI Name(){return "projects";}
	int DCMAPAPI TableParams(){return DCMAP_TABLE_INTERNAL | Inherited::TableParams();}
};
DCMAP_SPEC_INTERFACE(CProjectsStorage);

class CProjectActsStorage : public CBaseChildTableDataStorage<DCMapProjectActInfo,IDCMapProjectActsDataStorage>
{
	typedef CBaseChildTableDataStorage<DCMapProjectActInfo,IDCMapProjectActsDataStorage> Inherited;
public:
	CProjectActsStorage();
	dcmapCSTR DCMAPAPI Name(){return "projectacts";}
	int DCMAPAPI TableParams(){return DCMAP_TABLE_INTERNAL | Inherited::TableParams();}
};
DCMAP_SPEC_INTERFACE(CProjectActsStorage);

class CActionsRequestedStorage : public CIndexedChildTableDataStorage<DCMapActionsRequestedInfo,IDCMapActionsRequestedDataStorage>
{
	typedef CIndexedChildTableDataStorage<DCMapActionsRequestedInfo,IDCMapActionsRequestedDataStorage> Inherited;
public:
	CActionsRequestedStorage();
	dcmapCSTR DCMAPAPI Name(){return "actionsrequested";}
	int DCMAPAPI TableParams(){return DCMAP_TABLE_INTERNAL | Inherited::TableParams();}
};
DCMAP_SPEC_INTERFACE(CActionsRequestedStorage);


class CUnitTagsStorage : public CBaseChildTableDataStorage<IDCMapUnitTagInfo,IDCMapUnitTagsDataStorage>
{
	typedef CBaseChildTableDataStorage<IDCMapUnitTagInfo,IDCMapUnitTagsDataStorage> Inherited;
public:
	CUnitTagsStorage();
	dcmapCSTR DCMAPAPI Name(){return "utnittags";}
	int DCMAPAPI TableParams(){return DCMAP_TABLE_INTERNAL | Inherited::TableParams();}
};
DCMAP_SPEC_INTERFACE(CUnitTagsStorage);


class CBaseBuildingsStorage : public CIndexedChildTableDataStorage<DCMapBuildingInfo,IDCMapBuildingsDataStorage>
{
	typedef CIndexedChildTableDataStorage<DCMapBuildingInfo,IDCMapBuildingsDataStorage> Inherited;
public:
	CBaseBuildingsStorage(dcmapCSTR parent);
	int DCMAPAPI TableParams(){return DCMAP_TABLE_INTERNAL | Inherited::TableParams();}
};

class CBuildingsStorage : public CBaseBuildingsStorage
{
public:
	CBuildingsStorage();
	dcmapCSTR DCMAPAPI Name(){return "buildings";}
};
DCMAP_SPEC_INTERFACE(CBuildingsStorage);


class CActionsStorage : public CBaseChildTableDataStorage<DCMapActionInfo,IDCMapActionsDataStorage>
{
	typedef CBaseChildTableDataStorage<DCMapActionInfo,IDCMapActionsDataStorage> Inherited;
public:
	CActionsStorage();
	dcmapCSTR DCMAPAPI Name(){return "actions";}

	int DCMAPAPI TableParams(){return DCMAP_TABLE_INTERNAL | Inherited::TableParams();}
	bool DCMAPAPI SelectSendID(int id);
};
DCMAP_SPEC_INTERFACE(CActionsStorage);