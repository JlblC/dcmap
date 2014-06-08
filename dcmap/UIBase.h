//---------------------------------------------------------------------------
#ifndef UIBaseH
#define UIBaseH

#include "dcmapsyslib.h"
#include <map>
#include <set>
#include <list>
#include <algorithm>

#ifndef _foreach
#define GSYS_DETAIL_FOREACH(it,x,i) for(it i=(x).begin(),__fe_end=(x).end();i!=__fe_end;++i)
#define _foreach(t,x,i) GSYS_DETAIL_FOREACH(t::iterator,x,i)
#endif

class  TDCMapUIContext;
class TBaseFrameForm;

class TDCMapBasicComponentItem
{
friend class TCoreUIManager;
protected:
	IDCMapModule* m_pOwner;
	TDCMapUIContext* m_pContext;
	IDCMapLocalStrings* LocalStrings;
	int m_iID;
	void Unlink();
	void Link(IDCMapModule* pOwner);
	TDCMapBasicComponentItem();
	virtual ~TDCMapBasicComponentItem(){}
	virtual IDCMapVarValue* DCMAPAPI GetValue();

public:
	void InitItem(TDCMapUIContext* pContext,IDCMapModule* pOwner,int iID);
	int GetID(){return m_iID;}
	IDCMapModule* GetOwner(){return m_pOwner;}
	TDCMapUIContext* GetContext(){return m_pContext;}
};




//---------------------------------------------------------------------------
#endif
