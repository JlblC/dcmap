//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "IconsListMan.h"
#include "Interface.h"
#include "Utils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

static CAutoReg<TIconsList> sReg("PCoreService","VCLIconsList");

void* DCMAPAPI TIconsList::CastTo(DCMapInterfaceID idInterface)
{
    DCMAP_MODULE_SUPPORTS(TIconsList);
    return 0;
}

TIconsList::TIconsList()
{
//    m_InconsStateIndex=0;
	m_pList = InterfaceModule->IconsImageList;
	m_bNeedUpdate = false;
}
TIconsList::~TIconsList()
{
	//delete m_pList;
}

bool  DCMAPAPI TIconsList::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	if(!m_pIcons.init(core)) return false;
	m_pIcons.reset_state();
	m_bNeedUpdate=true;

	return true;
}

dcmapLRESULT DCMAPAPI TIconsList::OnMessage(dcmapMSG Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender)
{
	 switch (Msg)
	 {
	 case DCMM_DATA_PROCESS:
		   m_bNeedUpdate=true;
		   break;
	 }
}
int TIconsList::IconIndex(dcmapDBREF* ref)
{
	if(ref->id < Refs.size() && *ref == Refs[ref->id])return ref->id;
	return -1;
}

int TIconsList::IconIndex(dcmapDBREF const& ref)
{
	if(!ref.Valid())return -1;
	if(ref.id < Refs.size() && ref == Refs[ref.id])return ref.id;
	return -1;
}

void TIconsList::Prepare()
{
   if(m_bNeedUpdate)UpdateIcons();
}

void TIconsList::UpdateIcons()
{
	m_bNeedUpdate = false;
	if(!m_pIcons.actualize_state())return;

	m_pList->Clear();

	Graphics::TBitmap* pDefault = new Graphics::TBitmap;
	pDefault->SetSize(m_pList->Width,m_pList->Height);

	TMemoryStream* pStream = new TMemoryStream;
	Graphics::TBitmap* pBitmap = new Graphics::TBitmap;

	Refs.clear();
	Refs.reserve(m_pIcons->NumRecords()+8);

	if(m_pIcons->SelectAll())
	do
	{
		int id = m_pIcons->GetID()->id;
		if(id >= Refs.size())
             Refs.resize(id+1);
        Refs[id] =  *m_pIcons->GetID();

	} while(m_pIcons->Next());

    int n = Refs.size();

	for(int i=0;i<n;i++)
    {
		if((m_pIcons->SelectID(Refs[i])))
		{
			BYTE* dta=0;
            int sz = m_pIcons->GetBlobN("imgdata",&dta);
            if(sz)
            {
                pStream->SetSize(sz);
                pStream->Position = 0;
                pStream->Write(dta,sz);
                pStream->Position = 0;
                pBitmap->LoadFromStream(pStream);
                //pBitmap->Transparent = true;
                //pBitmap->TransparentColor = pBitmap->Canvas->Pixels[0][0];
                
                pBitmap->SetSize(m_pList->Width,m_pList->Height);

                int id = m_pIcons->GetID()->id;
                m_pList->AddMasked(pBitmap,pBitmap->Canvas->Pixels[0][0]);
            }
            else m_pList->Add(pDefault,pDefault);
        }
        else m_pList->Add(pDefault,pDefault);
    }
    delete pStream;
    delete pBitmap;
    delete pDefault;

}

void TIconsList::FillItems(TsComboBoxEx* pCombo)
{
	if(m_bNeedUpdate)UpdateIcons();

    TsComboItem* Item =	pCombo->ItemsEx->Add();
    Item->Caption = L" Нет";
    Item->ImageIndex=-1;

    if(m_pIcons->SelectAll() && 1)
    do
    {
		int id = m_pIcons->GetID()->id;

        TsComboItem* Item =	pCombo->ItemsEx->Add();
		Item->Caption = m_pIcons->GetWideName();
		Item->ImageIndex=id;

    } while(m_pIcons->Next());

    pCombo->ItemIndex = 0;
}

int TIconsList::CreateMenuItems(TMenuItem* pParentItem,Classes::TNotifyEvent OnClick,int at)
{
	if(m_bNeedUpdate)UpdateIcons();

	int Count=0;

	if(m_pIcons->SelectAll())
    do
	{
		TMenuItem* pSubItem = new TMenuItem(pParentItem);
		pSubItem->Caption = m_pIcons->GetWideName();
		pSubItem->ImageIndex = m_pIcons->GetID()->id;

		pSubItem->OnClick = OnClick;
		pSubItem->Hint = DBRefToStr(*m_pIcons->GetID());
		pParentItem->Add(pSubItem);
		Count++;
	} while(m_pIcons->Next());
	WrapMenu(pParentItem);

	return Count;
}

