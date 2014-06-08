//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OldFixedFilter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sComboBox"
#pragma link "sCheckBox"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma link "sLabel"
#pragma resource "*.dfm"

TOldFixedFilterFrame *OldFixedFilterFrame;

static CFramePanelWrapper<TOldFixedFilterFrame> sWrap("PFrameBar","OldSimpleFilter");
static CAutoReg<COldFixedFilter> sReg("PPlanetFilter","OldFixedFilter");

COldFixedFilter::COldFixedFilter()
{
	m_bPlayerFilter = false;
	m_refPlayer.Invalidate();
    m_bRectRegionFilter = false;
    m_bCommentFilter = false;
    m_fdef[0].m_bResourceFilter=false;
    m_fdef[1].m_bResourceFilter=false;
    m_fdef[2].m_bResourceFilter=false;

    Changed=false;
}

bool  DCMAPAPI COldFixedFilter::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	m_pWorkspace = workspace;
	DCMAP_GET_STORAGE(core,m_pPlayers,IDCMapPlayersDataStorage,players);
	DCMAP_GET_STORAGE(core,m_pPlanets,IDCMapPlanetDataStorage,planets);

    m_pGeoCalc = m_pWorkspace->GetServiceTyped<IDCMapGeoValueCalculator>("GeoValueCalculator");
	if(!m_pGeoCalc)return false;

	return true;
}

void  DCMAPAPI COldFixedFilter::LoadConfig(IDCMapConfig* pConfig)
{
	DCMapConfigWrapper cfg(pConfig);
}

void  DCMAPAPI COldFixedFilter::SaveConfig(IDCMapConfig* pConfig)
{
	DCMapConfigWrapper cfg(pConfig);
}

int DCMAPAPI COldFixedFilter::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	switch(Msg)
	{
	case DCMM_DATA_CHANGED:
		break;
	case DCMM_POST_INIT:
		break;
	}
	return 0;
};

void* DCMAPAPI COldFixedFilter::CastTo(char const* szInterface)
{
	if(!stricmp(szInterface,"IDCMapPlanetDataFilter"))return (IDCMapPlanetDataFilter*)this;
	return 0;
}

bool COldFixedFilter::Process(EResourceType rsr,EOperation op,float RefValue,DCMapPlanetDisplayInfo* pinfo)
{
	float gr,mb,cb,ms,cs;
    if(rsr >= RT_Growth)
    {
        m_pGeoCalc->CalculatePlanet(pinfo->planet.id,&gr,&mb,&cb,&ms,&cs);
    }

	float Value=0;
    switch(rsr)
    {
    case RT_Organics:
    	Value = pinfo->planet.o;
        break;
    case RT_Energy:
    	Value = pinfo->planet.e;
        break;
    case RT_Minerals:
    	Value = pinfo->planet.m;
        break;
    case RT_Size:
    	Value = pinfo->planet.s;
        break;
    case RT_Temperature:
    	Value = pinfo->planet.t;
        break;
    case RT_Growth:
    	Value = gr;
        break;
    case RT_MiningBase:
    	Value = mb;
        break;
    case RT_CreditsBase:
    	Value = cb;
        break;
    case RT_MiningSecond:
    	Value = ms;
        break;
    case RT_CreditsSecond:
    	Value = cs;
        break;
    case RT_Credits:
    	Value = Max(cb,cs);
        break;
    }
    switch(op)
    {
        case OP_EQUAL:  return  Value == RefValue;
        case OP_GREATER_EQUAL:  return  Value >= RefValue;
        case OP_LESS_EQUAL:  return  Value <= RefValue;
        case OP_GREATER:  return  Value > RefValue;
        case OP_LESS:  return  Value < RefValue;
    }
	return true;
}

bool DCMAPAPI COldFixedFilter::ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf)
{
    if(m_bRectRegionFilter)
    {
    	if(!PtInRect(m_Rect,Point(pinfo->planet.x,pinfo->planet.y)))
        	return false;
    }
    if(m_bPlayerFilter)
	{
		if(m_pPlayers->SelectID(pinf->owner))
		{
			if(pinf->owner != m_refPlayer) return false;
		}
		else if(m_refPlayer.id != DCMapInvalidReferenceId) return false;
	}
    if(m_fdef[0].m_bResourceFilter)
    {
    	if(!Process(m_fdef[0].Resource,m_fdef[0].Operation,m_fdef[0].Value,pinfo))return false;
    }
    if(m_fdef[1].m_bResourceFilter)
    {
    	if(!Process(m_fdef[1].Resource,m_fdef[1].Operation,m_fdef[1].Value,pinfo))return false;
    }
    if(m_fdef[2].m_bResourceFilter)
    {
    	if(!Process(m_fdef[2].Resource,m_fdef[2].Operation,m_fdef[2].Value,pinfo))return false;
    }

    if(m_bCommentFilter && !Comment.IsEmpty())
    {
    	if(m_pPlanets->SelectID(pinfo->planet.id))
        {
           dcmapWCSTR note = m_pPlanets->GetWideNotes();

           //return strstr(note,Comment.c_str());
        }
    }
	return true;
}


bool DCMAPAPI COldFixedFilter::ActualizeData()
{
	bool update = Changed;

    Changed=false;
    
    update |= m_bPlayerFilter;
    update |= m_fdef[0].m_bResourceFilter;
    update |= m_fdef[1].m_bResourceFilter;
    update |= m_fdef[2].m_bResourceFilter;

    return update;
}


//---------------------------------------------------------------------------
__fastcall TOldFixedFilterFrame::TOldFixedFilterFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
	m_pFilter = 0;
}
//---------------------------------------------------------------------------
bool TOldFixedFilterFrame::OnInit()
{
	m_pPlayers = m_pCore->GetDataSourceTyped<IDCMapPlayersDataStorage>("players");
    if(!m_pPlayers)return false;

	//UpdateData();
	return true;
}
int TOldFixedFilterFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	switch (Msg)
	{
	case DCMM_DATA_CHANGED:
	  UpdateData();
	  break;
	case DCMM_POST_INIT:
	  if(m_pWorkspace->SelectModuleByID("CMapOldFixedFilter"))
	  {
		  m_pFilter = (COldFixedFilter*)m_pWorkspace->Module();
		  UpdateFilter(this);
          RectRegionCheckClick(this);
          ResourceCheck1Click(this);
          ResourceCheck2Click(this);
	  }
	  break;
	}
	return 0;
}
void TOldFixedFilterFrame::UpdateData()
{
	PlayerList->Items->Clear();
    if(m_pPlayers->SelectAll())
    {
        do
        {
			PlayerList->Items->Add(m_pPlayers->GetWideName());
		}while(m_pPlayers->Next());
	}
	PlayerList->Sorted=true;
	//PlayerList->Sorted=false;
	PlayerList->Items->Insert(0," Нет");
	PlayerList->ItemIndex = PlayerList->Items->IndexOf(m_strOwner);
	if(PlayerList->ItemIndex < 0)PlayerList->ItemIndex = 0;
}
void TOldFixedFilterFrame::OnShow()
{
	UpdateData();
}
void __fastcall TOldFixedFilterFrame::UpdateFilter(TObject *Sender)
{
	if(!m_pFilter)return;

    bool PreFilter = m_pFilter->m_bPlayerFilter;
	m_pFilter->m_bPlayerFilter = OwnerCheck->Checked;
	if(OwnerCheck->Checked)
	{
    	NoFilterCheckBox->Checked=false;
		if(!PlayerList->Text.IsEmpty())m_strOwner = PlayerList->Text;
		if(m_pPlayers->SelectWideName(m_strOwner.c_str()))
		{
			m_pFilter->m_refPlayer = *m_pPlayers->GetID();
		}
		else m_pFilter->m_refPlayer.Invalidate();
	}
    else NoFilterCheckBoxClick(Sender);

    m_pFilter->Changed = true;
    if(PreFilter || m_pFilter->m_bPlayerFilter)
		m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0,0);
}
//---------------------------------------------------------------------------
void TOldFixedFilterFrame::LoadConfig(DCMapConfigWrapper cfg)
{
	cfg.section("OldSimpleFilter");
	bool bVal;
    int iVal;
	float fVal;
    String sVal;

    if(cfg["UseOwner"] >> bVal) OwnerCheck->Checked = bVal;
	cfg["Owner"] >> m_strOwner;
	if(cfg["UseRectRegion"] >> bVal) RectRegionCheck->Checked = bVal;
	if(cfg["RectRegionLeft"] >> iVal)LeftEdit->Value = iVal;
	if(cfg["RectRegionRight"] >> iVal)RightEdit->Value = iVal;
	if(cfg["RectRegionTop"] >> iVal)TopEdit->Value = iVal;
	if(cfg["RectRegionBottom"] >> iVal)BottomEdit->Value = iVal;

	if(cfg["UseResource1"] >> bVal) ResourceCheck1->Checked = bVal;
	if(cfg["Resource1"] >> iVal) ResourceCombo1->ItemIndex = iVal;
	if(cfg["Op1"] >> iVal) OpCombo1->ItemIndex = iVal;
	if(cfg["Value1"] >> fVal) ValueEdit1->Value = fVal;

    if(cfg["UseResource2"] >> bVal) ResourceCheck2->Checked = bVal;
	if(cfg["Resource2"] >> iVal) ResourceCombo2->ItemIndex = iVal;
	if(cfg["Op2"] >> iVal) OpCombo2->ItemIndex = iVal;
	if(cfg["Value2"] >> fVal) ValueEdit2->Value = fVal;

    if(cfg["UseComment"] >> bVal) ComentCheck->Checked = bVal;
	if(cfg["Comment"] >> sVal) ComentEdit->Text = sVal;
}

void TOldFixedFilterFrame::SaveConfig(DCMapConfigWrapper cfg)
{
	cfg.section("OldSimpleFilter");
	cfg["UseOwner"] << OwnerCheck->Checked;
	cfg["Owner"] << m_strOwner;
	cfg["UseRectRegion"] << RectRegionCheck->Checked;
    cfg["RectRegionLeft"] << LeftEdit->Value;
    cfg["RectRegionRight"] << RightEdit->Value;
    cfg["RectRegionTop"] << TopEdit->Value;
    cfg["RectRegionBottom"] << BottomEdit->Value;

	cfg["UseResource1"] << ResourceCheck1->Checked ;
	cfg["Resource1"] << ResourceCombo1->ItemIndex;
	cfg["Op1"] << OpCombo1->ItemIndex;
	cfg["Value1"] << ValueEdit1->Value;

    cfg["UseResource2"] << ResourceCheck2->Checked ;
	cfg["Resource2"] << ResourceCombo2->ItemIndex;
	cfg["Op2"] << OpCombo2->ItemIndex;
	cfg["Value2"] << ValueEdit2->Value;

    cfg["UseComment"] << ComentCheck->Checked;
	cfg["Comment"]<< ComentEdit->Text;
}
void __fastcall TOldFixedFilterFrame::RectRegionCheckClick(TObject *Sender)
{
	if(!m_pFilter)return;

    bool PreFilter = m_pFilter->m_bRectRegionFilter;
	m_pFilter->m_bRectRegionFilter = RectRegionCheck->Checked;
	if(RectRegionCheck->Checked)
	{
    	NoFilterCheckBox->Checked=false;
        m_pFilter->m_Rect.Left = LeftEdit->Value;
        m_pFilter->m_Rect.Top = TopEdit->Value;
        m_pFilter->m_Rect.Right = RightEdit->Value;
        m_pFilter->m_Rect.Bottom = BottomEdit->Value;
	}
    else NoFilterCheckBoxClick(Sender);

    m_pFilter->Changed = true;
    if(PreFilter || m_pFilter->m_bRectRegionFilter)
		m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0,0);

}
//---------------------------------------------------------------------------

void __fastcall TOldFixedFilterFrame::ResourceCheck1Click(TObject *Sender)
{
	if(!m_pFilter)return;
     FilterDef& def = m_pFilter->m_fdef[0];
    bool PreFilter = def.m_bResourceFilter;
	def.m_bResourceFilter = ResourceCheck1->Checked;
	if(ResourceCheck1->Checked)
	{
    	NoFilterCheckBox->Checked=false;
        def.Resource = ResourceCombo1->ItemIndex;
        def.Operation = OpCombo1->ItemIndex;
        def.Value = ValueEdit1->Value;
	}
    else NoFilterCheckBoxClick(Sender);

    m_pFilter->Changed = true;
    if(PreFilter || def.m_bResourceFilter)
		m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TOldFixedFilterFrame::ResourceCheck2Click(TObject *Sender)
{
	if(!m_pFilter)return;
     FilterDef& def = m_pFilter->m_fdef[1];
    bool PreFilter = def.m_bResourceFilter;
	def.m_bResourceFilter = ResourceCheck2->Checked;
	if(ResourceCheck2->Checked)
	{
    	NoFilterCheckBox->Checked=false;
        def.Resource = ResourceCombo2->ItemIndex;
        def.Operation = OpCombo2->ItemIndex;
        def.Value = ValueEdit2->Value;
	}
    else NoFilterCheckBoxClick(Sender);

	m_pFilter->Changed = true;
    if(PreFilter || def.m_bResourceFilter)
		m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0,0);

}
//---------------------------------------------------------------------------

void __fastcall TOldFixedFilterFrame::ComentCheckClick(TObject *Sender)
{
	if(!m_pFilter)return;

    bool PreFilter = m_pFilter->m_bCommentFilter;
	m_pFilter->m_bCommentFilter = ComentCheck->Checked;
	if(ComentCheck->Checked)
	{
    	NoFilterCheckBox->Checked=false;
        m_pFilter->Comment = ComentEdit->Text;
	}
    else NoFilterCheckBoxClick(Sender);

    m_pFilter->Changed = true;
    if(PreFilter || m_pFilter->m_bCommentFilter)
		m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TOldFixedFilterFrame::NoFilterCheckBoxClick(TObject *Sender)
{
	if(NoFilterCheckBox->Checked)
    {
        ComentCheck->Checked = false;
        ResourceCheck1->Checked = false;
        ResourceCheck2->Checked = false;
        RectRegionCheck->Checked = false;
        OwnerCheck->Checked = false;
    }
    else
    {
    	bool use = false;
        use |= ComentCheck->Checked;
        use |= ResourceCheck1->Checked;
        use |= ResourceCheck2->Checked;
        use |= RectRegionCheck->Checked;
        use |= OwnerCheck->Checked;

        if(!use)NoFilterCheckBox->Checked = true;
    }
}
//---------------------------------------------------------------------------

