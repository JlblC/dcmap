//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RaceAdvProp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma resource "*.dfm"
TRaceAdvPropFrame *RaceAdvPropFrame;

static CFramePanelWrapper<TRaceAdvPropFrame> sWrap("PListEditPropPage_races","01AdvInfo");

//---------------------------------------------------------------------------
__fastcall TRaceAdvPropFrame::TRaceAdvPropFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------

bool TRaceAdvPropFrame::OnInit()
{
    DCMAP_GET_STORAGE(m_pCore,m_pRaces,IDCMapRacesDataStorage,races);
    return true;
}

int TRaceAdvPropFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
    case DCMM_SELECTION_DATA_CHANGED:
        m_Ref.id = wParam;
        m_Ref.age = lParam;
        UpdateData();
        break;
    }
    return 0;
}

void TRaceAdvPropFrame::UpdateData()
{
    if(!m_pRaces->SelectID(m_Ref)) return;

    DCMapRaceInfo const* inf = m_pRaces->GetData();

    ScienceEdit->Value = m_pRaces->GetReal(DCMDB_RACE_SCIENCE);
    SpeedEdit->Value = m_pRaces->GetReal(DCMDB_RACE_SPEED);
    PricesEdit->Value = m_pRaces->GetReal(DCMDB_RACE_PRICES);
    CivProdEdit->Value = m_pRaces->GetReal(DCMDB_RACE_CIVIL_PRODUCTION);
    MilProdEdit->Value = m_pRaces->GetReal(DCMDB_RACE_MILITARY_PRODUCTION);
    HitpointsEdit->Value = m_pRaces->GetReal(DCMDB_RACE_HITPOINTS);
    DamageEdit->Value = m_pRaces->GetReal(DCMDB_RACE_DAMAGE);
    DetectionEdit->Value = m_pRaces->GetReal(DCMDB_RACE_DETECTION);
    StealthEdit->Value = m_pRaces->GetReal(DCMDB_RACE_STEALTH);
    DefenceEdit->Value = m_pRaces->GetReal(DCMDB_RACE_DEFENCE);

    Changed=false;
}

void TRaceAdvPropFrame::OnApply()
{
    if(!Changed)return;

    if(!m_pRaces->SelectID(m_Ref)) return;
    DCMapRaceInfo inf = *m_pRaces->GetData();
    m_pRaces->Update();

    m_pRaces->SetReal(DCMDB_RACE_SCIENCE,ScienceEdit->Value);
    m_pRaces->SetReal(DCMDB_RACE_SPEED,SpeedEdit->Value);
    m_pRaces->SetReal(DCMDB_RACE_PRICES,PricesEdit->Value);
    m_pRaces->SetReal(DCMDB_RACE_CIVIL_PRODUCTION,CivProdEdit->Value);
    m_pRaces->SetReal(DCMDB_RACE_MILITARY_PRODUCTION,MilProdEdit->Value);
    m_pRaces->SetReal(DCMDB_RACE_HITPOINTS,HitpointsEdit->Value);
    m_pRaces->SetReal(DCMDB_RACE_DAMAGE,DamageEdit->Value);
    m_pRaces->SetReal(DCMDB_RACE_DETECTION,DetectionEdit->Value);
    m_pRaces->SetReal(DCMDB_RACE_STEALTH,StealthEdit->Value);
    m_pRaces->SetReal(DCMDB_RACE_DEFENCE,DefenceEdit->Value);

    m_pRaces->Post();

    Changed = false;
}


void __fastcall TRaceAdvPropFrame::DataChanged(TObject *Sender)
{
	Changed=true;	
}
//---------------------------------------------------------------------------

