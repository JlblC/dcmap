//---------------------------------------------------------------------------
#include "vcl.h"
#pragma hdrstop

#include "UIBase.h"
#include "CoreUIManager.h"
#include "UISystem.h"

TDCMapBasicComponentItem::TDCMapBasicComponentItem()
{
	LocalStrings = 0;
	m_iID = 0;
	m_pOwner = 0;
	m_pContext = 0;
}

void TDCMapBasicComponentItem::Unlink()
{
	g_CoreUIManager.UnregisterUIItem(this,m_pOwner);
	if(m_pContext)m_pContext->Unlink(this);
}
void TDCMapBasicComponentItem::Link(IDCMapModule* pOwner)
{
    m_pOwner = pOwner;
	LocalStrings = g_CoreUIManager.RegisterUIItem(this,m_pOwner);
	if(m_pContext)m_pContext->Link(this);
}

void TDCMapBasicComponentItem::InitItem(TDCMapUIContext* pContext,IDCMapModule* pOwner,int iID)
{
	m_iID=iID;
	m_pContext = pContext;
	Link(pOwner);
}

IDCMapVarValue* DCMAPAPI TDCMapBasicComponentItem::GetValue()
{
	if(m_pContext)return m_pContext->ContextValue();
	return 0;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
