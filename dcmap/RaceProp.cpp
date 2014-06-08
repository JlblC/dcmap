//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RaceProp.h"
#include "dcmapsyslib.h"
#include "Interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma link "sCurrEdit"
#pragma link "sCurrencyEdit"
#pragma link "sCustomComboEdit"
#pragma link "sMaskEdit"
#pragma link "sComboBox"
#pragma link "sComboBoxes"
#pragma resource "*.dfm"
TRaceEditPropFrame *RaceEditPropFrame;

static CFramePanelWrapper<TRaceEditPropFrame> sWrap("PListEditPropPage_races","00BaseInfo");

//---------------------------------------------------------------------------
__fastcall TRaceEditPropFrame::TRaceEditPropFrame(TComponent* Owner)
    : TBaseModuleFrame(Owner)
{

}

bool TRaceEditPropFrame::OnInit()
{
    DCMAP_GET_STORAGE(m_pCore,m_pRaces,IDCMapRacesDataStorage,races);
    return true;
}

int TRaceEditPropFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
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

void TRaceEditPropFrame::OnShow()
{
}

void TRaceEditPropFrame::UpdateData()
{
    if(!m_pRaces->SelectID(m_Ref)) return;

    DCMapRaceInfo const* inf = m_pRaces->GetData();

    NameEdit->OnChange = 0;
    NameEdit->Text = inf->name;
    NameEdit->OnChange = NameEditChange;

    MinTEdit->Value = m_pRaces->GetReal(DCMDB_RACE_TMIN);
    MaxTEdit->Value = m_pRaces->GetReal(DCMDB_RACE_TMAX);

    NatureEdit->ItemIndex = inf->nature;
    BasisEdit->ItemIndex = inf->basis;
    SecondEdit->ItemIndex = inf->second;

    GrowthEdit->Value = m_pRaces->GetReal(DCMDB_RACE_GROWTH);
    MningEdit->Value = m_pRaces->GetReal(DCMDB_RACE_MINING);

    MinGrowthEdit->Value = m_pRaces->GetReal(DCMDB_RACE_MIN_GROWTH);

    //GrowthEdit->Value = float(inf->growth)/10;
    //MningEdit->Value = float(inf->mining)/10;

    Changed=false;
}

void TRaceEditPropFrame::OnApply()
{
    if(!Changed)return;

    if(!m_pRaces->SelectID(m_Ref)) return;
    DCMapRaceInfo inf = *m_pRaces->GetData();
    m_pRaces->Update();

    inf.nature = NatureEdit->ItemIndex;
    inf.basis = BasisEdit->ItemIndex;
    inf.second = SecondEdit->ItemIndex;
    m_pRaces->SetData(&inf);

	m_pRaces->SetWideName(NameEdit->Text.w_str());

    m_pRaces->SetReal(DCMDB_RACE_TMIN,MinTEdit->Value);
    m_pRaces->SetReal(DCMDB_RACE_TMAX,MaxTEdit->Value);

    m_pRaces->SetReal(DCMDB_RACE_GROWTH,GrowthEdit->Value);
    m_pRaces->SetReal(DCMDB_RACE_MINING,MningEdit->Value);

    m_pRaces->SetReal(DCMDB_RACE_MIN_GROWTH,MinGrowthEdit->Value);

    m_pRaces->Post();

    Changed = false;
}


//---------------------------------------------------------------------------
void __fastcall TRaceEditPropFrame::DataChanged(TObject *Sender)
{
    Changed = true;
}
//---------------------------------------------------------------------------
void SetIndexes(TsComboBoxEx* first,TsComboBoxEx* second,TsComboBoxEx* third)
{
    if(first->ItemIndex == second->ItemIndex)
    {
        for(int i=0;i<3;i++)
        {
            if(first->ItemIndex != i && third->ItemIndex !=i)
            {
                second->ItemIndex = i;
                break;
            }
        }
    }
    if(first->ItemIndex == third->ItemIndex)
    {
        for(int i=0;i<3;i++)
        {
            if(first->ItemIndex != i && second->ItemIndex !=i)
            {
                third->ItemIndex = i;
                break;
            }
        }
    }
}

void __fastcall TRaceEditPropFrame::NatureEditChange(TObject *Sender)
{
    Changed = true;
    SetIndexes(NatureEdit,BasisEdit,SecondEdit);
}
//---------------------------------------------------------------------------

void __fastcall TRaceEditPropFrame::BasisEditChange(TObject *Sender)
{
    Changed = true;
    SetIndexes(BasisEdit,NatureEdit,SecondEdit);
}
//---------------------------------------------------------------------------

void __fastcall TRaceEditPropFrame::SecondEditChange(TObject *Sender)
{
    Changed = true;
    SetIndexes(SecondEdit,BasisEdit,NatureEdit);
}
//---------------------------------------------------------------------------

void __fastcall TRaceEditPropFrame::NameEditChange(TObject *Sender)
{
    SetNewKeyValue(NameEdit->Text);
    Changed=true;
}
//---------------------------------------------------------------------------

bool TRaceEditPropFrame::OnDataTest()
{
    if(!Changed)return true;
    if(m_pRaces->SelectWideName(NameEdit->Text.w_str()))
    {
        if(*m_pRaces->GetID() != m_Ref)
        {
            Dialogs::TMsgDlgButtons Buttons;Buttons << mbOK;
			sMessageDlg(L"Ошибка",L"Существует другая раса с таким именем!",mtError,Buttons,0);
			return false;
        }
    }
    return true;
}


