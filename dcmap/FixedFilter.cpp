//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FixedFilter.h"
#include "Utils.h"
#include "Interface.h"
#include "FilterSelect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sComboBox"
#pragma link "sCheckBox"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma link "sLabel"
#pragma link "FixedFilterDB"
#pragma link "sPanel"
#pragma link "sRadioButton"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TFixedFilterFrame *FixedFilterFrame;

static CFramePanelWrapper<TFixedFilterFrame> sWrap("PFrameBar","SimpleFilter");
static CAutoReg<CMapFixedFilter> sReg("PPlanetFilter","FixedFilter");
static CAutoReg<CMapGeoFixedFilter> sGeoReg("PGeoFilter","FixedFilter");

struct SectorData : public BaseCachedData
{
	TRect rcRect;
	HRGN  hRgn;
	SectorData():hRgn(0),rcRect(0,0,0,0){}
	~SectorData()
	{
		if(hRgn) DeleteObject(hRgn);
	}
};

static bool __fastcall ProcessSector(FilterDef const& def,DCMapPlanetParamInfo const& geo)
{
	SectorData* pData;
	if(!def.pCachedData)
	{
		pData = new SectorData;

		def.pCachedData = pData;

		dcmapPOINT* pt;
		int sz = dcmapStrToCoordArray(def.StrValue.w_str(),&pt,true);

		if(sz==2)
		{
		  pData->rcRect = Rect(pt[0].x,pt[0].y,pt[1].x,pt[1].y);
		}
		else if(sz>2)
		{
			pData->hRgn = CreatePolygonRgn(pt,sz,WINDING);
		}
		/*
		// Parce String into rect
		String left,top,right,bottom;
		String Rem=def.StrValue;
		pRect = new TRect(0,0,0,0);
		def.pCachedData = pRect;
		int d1 = def.StrValue.Pos("-");
		int d2 = def.StrValue.Pos(":");
        if(!d1 || !d2)return false;
        int d;
        if(d1 < d2)
        {
        	int d = d1;
       	 	left = Rem.SubString(1,d-1);
            Rem = Rem.SubString(d+1,Rem.Length());
        	d   = Rem.Pos(":");if(!d)return false;
       	 	right = Rem.SubString(1,d-1);
            Rem = Rem.SubString(d+1,Rem.Length());
        	d   = Rem.Pos("-");if(!d)return false;
       	 	top = Rem.SubString(1,d-1);
            bottom = Rem.SubString(d+1,Rem.Length());
        }
        else
        {
        	int d = d2;
       	 	left = Rem.SubString(1,d-1);
            Rem = Rem.SubString(d+1,Rem.Length());
        	d   = Rem.Pos("-");if(!d)return false;
       	 	top = Rem.SubString(1,d-1);
            Rem = Rem.SubString(d+1,Rem.Length());
        	d   = Rem.Pos(":");if(!d)return false;
       	 	right = Rem.SubString(1,d-1);
            bottom = Rem.SubString(d+1,Rem.Length());
        }
        TRect rc;
        try
        {
        	rc.left = left.Trim().ToInt();
        	rc.top = top.Trim().ToInt();
        	rc.right = right.Trim().ToInt()+1;
        	rc.bottom = bottom.Trim().ToInt()+1;
        }
        catch(...){return false;}
		*pRect = rc;
		*/
    }
	else pData = (SectorData*)def.pCachedData;

	bool r;
	if(pData->hRgn) r = PtInRegion(pData->hRgn,geo.x,geo.y);
	else r = PtInRect(pData->rcRect,Point(geo.x,geo.y));

    if(def.Operation == OP_NOTCNT || def.Operation == OP_NOTEQUAL) r = !r;
    return r;
}

bool __fastcall CMapFixedFilter::ProcessOpen(FilterDef const& pFDef,DCMapPlanetParamInfo const& pinf )
{
	if(!m_pPlanetPlayer->SelectPlanetPlayer(pinf.id,*pFDef.RefValue))return false;
	return (m_pPlanetPlayer->GetData()->flags & DCMAP_PPF_Open);
}


static bool __fastcall ProcessGeoValues(FilterDef const& pFDef,DCMapPlanetParamInfo const& geo,DCMapPlanetValueInfo const& val)
{
	EResourceType rsr = pFDef.Resource;
    EOperation op = pFDef.Operation;
	float RefValue = pFDef.Value;
    String StrValue = pFDef.StrValue;

	float Value=0;
	switch(rsr)
    {
    case RT_X:
    	Value = geo.x;
        break;
    case RT_Y:
    	Value = geo.y;
        break;
    case RT_Organics:
    	Value = geo.o;
        break;
    case RT_Energy:
    	Value = geo.e;
        break;
    case RT_Minerals:
    	Value = geo.m;
        break;
    case RT_Size:
    	Value = geo.s;
        break;
    case RT_Temperature:
    	Value = geo.t;
        break;
    case RT_Growth:
    	Value = val.Growth;
        break;
    case RT_MiningBase:
    	Value = val.Primary;
        break;
    case RT_CreditsBase:
    	Value = val.PrimaryCredit;
        break;
    case RT_MiningSecond:
    	Value = val.Secondaty;
        break;
    case RT_CreditsSecond:
    	Value = val.SecondatyCredit;
        break;
    case RT_Credits:
    	Value = Max(val.PrimaryCredit,val.SecondatyCredit);
        break;
    }
    if(rsr == RT_Sector)
    {
    	 return ProcessSector(pFDef,geo);
    }

    switch(op)
    {
        case OP_EQUAL:  return  Value == RefValue;
        case OP_NOTEQUAL:  return  Value != RefValue;
        case OP_GREATER_EQUAL:  return  Value >= RefValue;
        case OP_LESS_EQUAL:  return  Value <= RefValue;
        case OP_GREATER: return  Value > RefValue;
        case OP_LESS:  return  Value < RefValue;
    }
    return true;
}

CMapGeoFixedFilter::CMapGeoFixedFilter()
{
    for(int i=0;i<NumDBFilters;i++)
    {
    	m_fdef[i].m_bResourceFilter=false;
    }
	Changed=false;
	OpMode=false;
}

CMapGeoFixedFilter::~CMapGeoFixedFilter()
{
    for(int i=0;i<NumDBFilters;i++)
    	m_fdef[i].Init(0);
}

void* DCMAPAPI CMapGeoFixedFilter::CastTo(char const* idInterface)
{
	DCMAP_MODULE_SUPPORTS(IDCMapGeoDataFilter);
	return 0;
}

bool DCMAPAPI CMapGeoFixedFilter::ProcessGeoData(DCMapPlanetParamInfo const* pGeo,DCMapPlanetValueInfo const* pValue)
{
	if(OpMode == FMODE_AND)
	{
		bool bGenRes = false;
		bool bPreResult = true;
		for(int i=0;i<NumDBFilters;i++)
		{
			if(m_fdef[i].m_bResourceFilter && m_fdef[i].table == OP_RESOURCE)
			{
				if(m_fdef[i].Mode == FMODE_AND)
				{
					if(!bPreResult)continue; // skip this filter step
					else bPreResult = ProcessGeoValues(m_fdef[i],*pGeo,*pValue);
				}
				else // start new group
				{
					bGenRes |= bPreResult;
					if(bGenRes)return true;
					bPreResult = ProcessGeoValues(m_fdef[i],*pGeo,*pValue);
				}
			}
		}
		bGenRes |= bPreResult;
		return bGenRes;
	}
	else if(OpMode == FMODE_OR)
	{
		bool bGenRes = true;
		bool bPreResult = true;
		for(int i=0;i<NumDBFilters;i++)
		{
			if(m_fdef[i].m_bResourceFilter && m_fdef[i].table == OP_RESOURCE)
			{
				if(m_fdef[i].Mode == FMODE_OR)
				{
					if(bPreResult)continue; // skip this filter step
					bPreResult |= ProcessGeoValues(m_fdef[i],*pGeo,*pValue);
				}
				else // start new group
				{
					bGenRes &= bPreResult;
					if(!bGenRes)return false;
					bPreResult = ProcessGeoValues(m_fdef[i],*pGeo,*pValue);
				}
			}
		}
		bGenRes &= bPreResult;
		return bGenRes;
	}
	else
	{
		bool bResult = true;
		for(int i=0;i<NumDBFilters;i++)
		{
			if(m_fdef[i].m_bResourceFilter && m_fdef[i].table == OP_RESOURCE)
			{
				if(m_fdef[i].Mode == FMODE_OR)
				{
					bResult |= ProcessGeoValues(m_fdef[i],*pGeo,*pValue);
				}
				else
				{
					bResult &= ProcessGeoValues(m_fdef[i],*pGeo,*pValue);
				}
			}
		}
		return bResult;
	}
}

bool DCMAPAPI CMapGeoFixedFilter::ActualizeData()
{
	bool update = Changed;
    Changed=false;
    for(int i=0;i<NumDBFilters;i++)
    {
        update |= m_fdef[i].m_bResourceFilter;
    }
    return update;
}

CMapFixedFilter::CMapFixedFilter()
{
    for(int i=0;i<NumDBFilters;i++)
    {
    	m_fdef[i].m_bResourceFilter=false;
    }
	Changed=false;
	OpMode=false;
}
CMapFixedFilter::~CMapFixedFilter()
{
    for(int i=0;i<NumDBFilters;i++)
    	m_fdef[i].Init(0);
}

bool  DCMAPAPI CMapFixedFilter::Init(IDCMapCore* core,IDCMapWorkspace* workspace)
{
	m_pWorkspace = workspace;
	DCMAP_GET_STORAGE(core,m_pPlayers,IDCMapPlayersDataStorage,players);
	DCMAP_GET_STORAGE(core,m_pPlanets,IDCMapPlanetDataStorage,planets);

    m_pGeoCalc = m_pWorkspace->GetServiceTyped<IDCMapGeoValueCalculator>("GeoValueCalculator");
	if(!m_pGeoCalc)return false;

    for(int i=0;i<NumDBFilters;i++)
    	m_fdef[i].Init(m_pWorkspace);

	return true;
}

void  DCMAPAPI CMapFixedFilter::LoadConfig(IDCMapConfig* pConfig)
{
	DCMapConfigWrapper cfg(pConfig);
}

void  DCMAPAPI CMapFixedFilter::SaveConfig(IDCMapConfig* pConfig)
{
	DCMapConfigWrapper cfg(pConfig);
}

int DCMAPAPI CMapFixedFilter::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	switch(Msg)
	{
	case DCMM_DATA_CHANGED:
		break;
	case DCMM_POST_INIT:
		break;
	case DCMM_PRE_DESTROY:
	    for(int i=0;i<NumDBFilters;i++)
		{
        	m_fdef[i].Init(m_pWorkspace);
		}
		break;
	}
	return 0;
};

void* DCMAPAPI CMapFixedFilter::CastTo(char const* szInterface)
{
	if(!stricmp(szInterface,"IDCMapPlanetDataFilter"))return (IDCMapPlanetDataFilter*)this;
	return 0;
}

static bool ProcessExpression(FilterDef &def,DCMapDataReference const& ref)
{
    if(!def.pProcessModule)
    {
        def.pProcessModule =
        	def.pWorkspace->CreateModuleTyped<IDCMapExpressionTableFilterProcessor>
            				("PModules","ExprTableFilterProc");
        if(!def.pProcessModule) return false;

        if(!((IDCMapExpressionTableFilterProcessor*)def.pProcessModule)->Setup("planets",
        		ToWide(def.StrValue).w_str()))return false;
    }
    return ((IDCMapExpressionTableFilterProcessor*)def.pProcessModule)->Process(&ref);
}

bool CMapFixedFilter::Process(FilterDef &def, DCMapPlanetInfo const* pinf,DCMapPlanetValueInfo& val)
{
	IDCMapDataStorage* m_pStorage=0;

	if(def.table == OP_RESOURCE)
    {
		if(def.Resource == RT_Open)
		{
			return ProcessOpen(def,*pinf);
		}
		if(def.Resource == RT_Expression)
		{
			return ProcessExpression(def,pinf->id);
		}
        return ProcessGeoValues(def,*pinf,val);
    }
	if(def.table == OP_PLANETS)
    {
		m_pStorage = m_pPlanets;
    }
	else if(def.table == OP_PLAYERS)
    {
        //if(m_pPlanets->SelectID(pinfo->planet.id))
        {
    		if(m_pPlayers->SelectID(m_pPlanets->GetData()->owner))
            {
    			m_pStorage = m_pPlayers;
            }
        }
    }

    if(def.RefValue)
    {
    	DCMapDataReference Value;
        if(m_pStorage)Value = *m_pStorage->GetReference(def.iField);
        if(def.RefValue->Valid())
        {
            switch(def.Operation)
            {
                case OP_EQUAL:  return  Value == *def.RefValue;
                case OP_NOTEQUAL:  return  Value != *def.RefValue;
            }
        }
        else
        {
            switch(def.Operation)
            {
                case OP_EQUAL:  return  !def.pStorage->SelectID(Value);
                case OP_NOTEQUAL:  return  def.pStorage->SelectID(Value);
            }
        }
    }
    else if(def.bStrValue)
    {
    	String Value;
    	if(m_pStorage) Value = m_pStorage->GetWideString(def.iField);
        Value = Value.LowerCase();
        switch(def.Operation)
        {
        case OP_EQUAL:  return  Value == def.StrValue;
        case OP_NOTEQUAL:  return  Value != def.StrValue;
        case OP_GREATER_EQUAL:  return Value  >=  def.StrValue;
        case OP_LESS_EQUAL:  return  Value <= def.StrValue;
        case OP_GREATER:  return  Value > def.StrValue;
        case OP_LESS:  return  Value < def.StrValue;
        case OP_CNT:  return  Value.Pos(def.StrValue)>0;
        case OP_NOTCNT:  return  Value.Pos(def.StrValue)<=0;
        }
    }
    else
    {
    	dcmapREAL Value = 0;
    	if(m_pStorage) Value = m_pStorage->GetReal(def.iField);
        switch(def.Operation)
        {
            case OP_EQUAL:  return  Value == def.Value;
            case OP_NOTEQUAL:  return  Value != def.Value;
            case OP_GREATER_EQUAL:  return  Value >= def.Value;
            case OP_LESS_EQUAL:  return  Value <= def.Value;
            case OP_GREATER:  return  Value > def.Value;
            case OP_LESS:  return  Value < def.Value;
        }
    }
    return true;
}

bool DCMAPAPI CMapFixedFilter::ProcessPlanet(DCMapPlanetDisplayInfo* pinfo,DCMapPlanetInfo const* pinf)
{
	DCMapPlanetValueInfo val;
	m_pGeoCalc->CalculateGeoIndirect(&pinf->geo,&val);

	if(OpMode == FMODE_AND)
	{
		bool bGenRes = false;
		bool bPreResult = true;
		for(int i=0;i<NumDBFilters;i++)
		{
			if(m_fdef[i].m_bResourceFilter)
			{
				if(m_fdef[i].Mode == FMODE_AND)
				{
					if(!bPreResult)continue; // skip this filter step
					else bPreResult = Process(m_fdef[i],pinf,val);
				}
				else // start new group
				{
					bGenRes |= bPreResult;
					if(bGenRes)return true;
					bPreResult = Process(m_fdef[i],pinf,val);
				}
			}
		}
		bGenRes |= bPreResult;
		return bGenRes;
	}
	else if(OpMode == FMODE_OR)
	{
		bool bGenRes = true;
		bool bPreResult = true;
		for(int i=0;i<NumDBFilters;i++)
		{
			if(m_fdef[i].m_bResourceFilter)
			{
				if(m_fdef[i].Mode == FMODE_OR)
				{
					if(bPreResult)continue; // skip this filter step
					bPreResult |= Process(m_fdef[i],pinf,val);
				}
				else // start new group
				{
					bGenRes &= bPreResult;
					if(!bGenRes)return false;
					bPreResult = Process(m_fdef[i],pinf,val);;
				}
			}
		}
		bGenRes &= bPreResult;
		return bGenRes;
	}
	else
	{
		bool bResult = true;
		for(int i=0;i<NumDBFilters;i++)
		{
			if(m_fdef[i].m_bResourceFilter)
			{
				if(m_fdef[i].Mode == FMODE_OR)
				{
					bResult |= Process(m_fdef[i],pinf,val);
				}
				else
				{
					bResult &= Process(m_fdef[i],pinf,val);;
				}
			}
		}
		return bResult;
	}
}

bool DCMAPAPI CMapFixedFilter::ActualizeData()
{
	bool update = Changed;

    Changed=false;

    for(int i=0;i<NumDBFilters;i++)
    {
        update |= m_fdef[i].m_bResourceFilter;
    }
    return update;
}
//---------------------------------------------------------------------------
__fastcall TFixedFilterFrame::TFixedFilterFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
	m_pFilter = 0;
	//Caption = "Фильтр";
    CurrentFrame=0;
    m_bFilterChanged=false;

    for(int i=0;i<NumDBFilters;i++)Frames[i]=0;

	m_iNumFilters=4;
	m_iOpMode = FMODE_AND;
}
//---------------------------------------------------------------------------
bool TFixedFilterFrame::OnInit()
{
    return true;
}

void TFixedFilterFrame::CreateFilterFrame(int n,int h)
{
	Frames[n] = new TFixedFilterDBFrame(this);
	InterfaceModule->LocalizeContainer(Frames[n]);

	Frames[n]->Top=h;
	Frames[n]->Align = alTop;

    Frames[n]->Name = "FilterFrame"+IntToStr(n);
    Frames[n]->Parent = this;

    Frames[n]->m_pCore = m_pCore;
    Frames[n]->m_pWorkspace = m_pWorkspace;

    Frames[n]->m_iFilterNo = n;
    Frames[n]->m_pParent = this;
    Frames[n]->ResourceSelBtn->DropdownMenu = FilterMenu;

    DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());
    Frames[n]->LoadConfig(cfg);
}

int TFixedFilterFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
	switch (Msg)
	{
	case DCMM_DATA_CHANGED:
	  UpdateData();
	  break;
	case DCMM_POST_INIT:
	  if(m_pWorkspace->SelectModuleByID("CMapFixedFilter"))
	  {
		  m_pFilter = (CMapFixedFilter*)m_pWorkspace->Module();
	  }
	  if(m_pWorkspace->SelectModuleByID("CMapGeoFixedFilter"))
	  {
		  m_pGeoFilter = (CMapGeoFixedFilter*)m_pWorkspace->Module();
	  }
	  UpdateFilter(this);
	  break;
	}
	return 0;
}
void TFixedFilterFrame::UpdateData()
{
	for(int i=0;i<m_iNumFilters;i++)
	{
		 Frames[i]->UpdateData();
	}
}

void TFixedFilterFrame::OnShow()
{
	UpdateData();
}
void __fastcall TFixedFilterFrame::UpdateFilter(TObject *Sender)
{
	if(!m_pFilter)return;
	for(int i=0;i<NumDBFilters;i++)
	{
		if(Frames[i])Frames[i]->SetupFilter();
	}
	if(m_pFilter)m_pFilter->OpMode = m_iOpMode;
	if(m_pGeoFilter)m_pGeoFilter->OpMode = m_iOpMode;
}
//---------------------------------------------------------------------------
void TFixedFilterFrame::LoadParams(DCMapConfigWrapper cfg)
{
	cfg["OpMode"] >> m_iOpMode;
	if(m_iOpMode == FMODE_AND) OpModeButton->ImageIndex = InterfaceModule->GetMenuImage(L"op-prio-and");
	else if(m_iOpMode == FMODE_OR) OpModeButton->ImageIndex = InterfaceModule->GetMenuImage(L"op-prio-or");
	else OpModeButton->ImageIndex = InterfaceModule->GetMenuImage(L"op-prio-order");

	if(m_pFilter)m_pFilter->OpMode = m_iOpMode;
	if(m_pGeoFilter)m_pGeoFilter->OpMode = m_iOpMode;
}

void TFixedFilterFrame::LoadConfig(DCMapConfigWrapper cfg)
{
	bool bParam;

	cfg.section("PlanetFilter");
	if(cfg["ShadeFiltered"] >> bParam) ShadeButton->Down = bParam;
	if(bParam) ShadeButtonClick(this);

	cfg.section("FixedFilter");
	cfg["NumFilters"] >> m_iNumFilters;

	LoadParams(cfg);

    NumFiltersEdit->Value = m_iNumFilters;
    int h = TopPanel->Height;
	for(int i=0;i<m_iNumFilters;i++)
    {
		CreateFilterFrame(i,h);
        h += Frames[i]->Height;
	}
	Height = h+4;
}

void TFixedFilterFrame::SaveConfig(DCMapConfigWrapper cfg)
{
	cfg.section("PlanetFilter");
	cfg["ShadeFiltered"] << ShadeButton->Down;

	cfg.section("FixedFilter");

	cfg["NumFilters"] << NumFiltersEdit->Value;
	cfg["OpMode"] << m_iOpMode;

	for(int i=0;i<NumDBFilters;i++)
    {
        if(Frames[i])Frames[i]->SaveConfig(cfg);
    }
}

void __fastcall TFixedFilterFrame::NoFilterCheckBoxClick(TObject *Sender)
{
	if(NoFilterCheckBox->Checked)
    {
    	for(int i=0;i<NumDBFilters;i++)
        {
        	if(Frames[i])Frames[i]->UseCheck->Checked = false;
        }
    }
    else
    {
    	bool use = false;
    	for(int i=0;i<NumDBFilters;i++)
        {
            if(Frames[i]) use |= Frames[i]->UseCheck->Checked;
        }
        if(!use)NoFilterCheckBox->Checked = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterFrame::FilterMenuClick(TObject *Sender)
{
	if(CurrentFrame)CurrentFrame->FieldMenuClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterFrame::NumFiltersEditChange(TObject *Sender)
{
	DCMapConfigWrapper cfg(m_pWorkspace->GetConfig());

	if(!m_pWorkspace)return;
	int h = Height;
    if(NumFiltersEdit->Value > m_iNumFilters)
    {
    	for(int i=m_iNumFilters;i<NumFiltersEdit->Value;i++)
        {
            CreateFilterFrame(i,h);
            h += Frames[i]->Height;
        }
    }
    else if(NumFiltersEdit->Value < m_iNumFilters)
	{
		for(int i=NumFiltersEdit->Value;i<m_iNumFilters;i++)
		{
			Frames[i]->UseCheck->Checked = false;
			Frames[i]->SaveConfig(cfg);
			h -= Frames[i]->Height;
			delete Frames[i];
			Frames[i]=0;
		}
	}
	m_iNumFilters = NumFiltersEdit->Value;
	Height = h;
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterFrame::ShadeButtonClick(TObject *Sender)
{
	m_pWorkspace->BroadcastMessage(DCMM_MAP_SET_FILTER_SHADE_MODE,ShadeButton->Down?1:0,0,m_pPanelInterface);

}
//---------------------------------------------------------------------------
void TFixedFilterFrame::FilterChanged()
{
    m_bFilterChanged = true;
    DelayTimer->Enabled=false;
    DelayTimer->Enabled=true;
}
void __fastcall TFixedFilterFrame::DelayTimerTimer(TObject *Sender)
{
    DelayTimer->Enabled=false;
    if(!m_bFilterChanged)return;
    m_bFilterChanged = false;
    m_pWorkspace->BroadcastMessage(DCMM_PLANET_FILTER_CHANGED,0,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterFrame::LoadButtonClick(TObject *Sender)
{
	FilterSelectForm = new TFilterSelectForm(Application);

        DCMapConfigWrapper cfg(m_pCore);
        cfg.section("FixedFilter");

        if(FilterSelectForm->Execute(cfg))
        {
            cfg.section(FilterSelectForm->PresetSection);

            int NumFilters=1;
			cfg["NumFilters"] >> NumFilters;

			LoadParams(cfg);

            if(NumFiltersEdit->Value < NumFilters)
            {
            	int h = Height;
                for(int i=NumFiltersEdit->Value;i<NumFilters;i++)
                {
                    CreateFilterFrame(i,h);
                    h +=Frames[i]->Height;
                }
                m_iNumFilters = NumFilters;
                NumFiltersEdit->Value = NumFilters;
                Height = h;
            }
            for(int i=0;i<NumDBFilters;i++)
            {
                if(Frames[i])Frames[i]->LoadConfig(cfg);
            }
        }
	delete FilterSelectForm;
}
//---------------------------------------------------------------------------

void __fastcall TFixedFilterFrame::SaveButtonClick(TObject *Sender)
{
	DCMapConfigWrapper cfg(m_pCore);
		cfg.section("FixedFilter");
        int NumPresets = 0;
        int PresetInd = 0;
        cfg["NumPresets"] >> NumPresets;
        cfg["PresetInd"] >> PresetInd;

        TStringList* list = new TStringList();

	DCMapVarValue Val;
	DCMapVarValue Res;

        DCMapVarValue Items;

        for(int i=0;i<NumPresets;i++)
        {
            String PresetName;
            cfg["PresetName"+IntToStr(i)] >> PresetName;
            Items->AddString(PresetName.w_str());
            list->Add(PresetName);
        }
        Val->SetNamedVar(L"items",Items);

        //Val[L"value"] = UnitsToUnpack.size();
	if(m_pCore->GetCoreUI()->InputDialog("FilterSaveSetupNameQuery",Val,Res))
		{
            String FilterName = Res->GetNamedString(L"value");
            int Index = Res->GetNamedNumber(L"index");
            Index = list->IndexOf(FilterName);

			String FilterSection;
			if(Index < 0)
			{
				FilterSection = "FixedFilter::Preset"+IntToStr(++PresetInd);
				cfg["PresetName"+IntToStr(NumPresets)] << FilterName;
				cfg["PresetSection"+IntToStr(NumPresets)] << FilterSection;

				cfg["NumPresets"] << ++NumPresets;
				cfg["PresetInd"] << PresetInd;
			}
			else
			{
				cfg["PresetSection"+IntToStr(Index)] >> FilterSection;
			}
            cfg.section(FilterSection);

			cfg["OpMode"] << m_iOpMode;
			cfg["NumFilters"] << NumFiltersEdit->Value;
            for(int i=0;i<NumDBFilters;i++)
            {
                if(Frames[i])Frames[i]->SaveConfig(cfg);
            }
        }

        delete list;
}
//---------------------------------------------------------------------------


void __fastcall TFixedFilterFrame::OpModeButtonClick(TObject *Sender)
{
	if(m_iOpMode == FMODE_AND)
	{
		m_iOpMode = FMODE_OR;
		OpModeButton->ImageIndex = InterfaceModule->GetMenuImage(L"op-prio-or");
	}
	else if(m_iOpMode == FMODE_OR)
	{
		m_iOpMode = FMODE_NONE;
		OpModeButton->ImageIndex = InterfaceModule->GetMenuImage(L"op-prio-order");
	}
	else
	{
		m_iOpMode = FMODE_AND;
		OpModeButton->ImageIndex = InterfaceModule->GetMenuImage(L"op-prio-and");
	}
	if(m_pFilter)m_pFilter->OpMode = m_iOpMode;
	if(m_pGeoFilter)m_pGeoFilter->OpMode = m_iOpMode;
	FilterChanged();
}
//---------------------------------------------------------------------------

