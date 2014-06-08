#include "StdAfx.h"
#include "Keywords.h"
#include "utils.h"

struct CSelectPathKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	dcmapDCSTR path = GetDCMapCore()->GetCoreUI()->SelectPath();
	if(path && path[0])return CVarValue(path);
	return CVarValue();
}};static KeywordAutoReg<CSelectPathKW> sCSelectPathKW(L"select_path");

struct COpenDlgKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	dcmapDCSTR file=0;
	if(param.type() == VT_HASH)
		file = GetDCMapCore()->GetCoreUI()->OpenDlg(param[L"path"].to_string().c_str(),param[L"filter"].to_string().c_str(),param[L"filter_index"].to_real());
	else
		file = GetDCMapCore()->GetCoreUI()->OpenDlg(param[0].to_string().c_str(),param[1].to_string().c_str(),param[2].to_real());
	if(file && file[0])return (CVarValue)file;
	return CVarValue();
}};static KeywordAutoReg<COpenDlgKW> sCOpenDlgKW(L"open_dialog");

struct CSaveDlgKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	dcmapDCSTR file=0;
	if(param.type() == VT_HASH)
		file = GetDCMapCore()->GetCoreUI()->SaveDlg(param[L"ext"].to_string().c_str(),param[L"path"].to_string().c_str(),param[L"filter"].to_string().c_str(),param[L"filter_index"].to_real());
	else
		file = GetDCMapCore()->GetCoreUI()->SaveDlg(param[0].to_string().c_str(),param[1].to_string().c_str(),param[2].to_string().c_str(),param[3].to_real());

	if(file && file[0])return (CVarValue)file;
	return CVarValue();
}};static KeywordAutoReg<CSaveDlgKW> sCSaveDlgKW(L"save_dialog");

int DoCCSVTableImport(dcmapCSTR table,dcmapDCSTR param,bool replace,dcmapDCSTR data);
struct CCSVImportKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	if(param.type() == VT_HASH)
		return (CVarValue)DoCCSVTableImport(
			WideToString(param[L"table"].to_string()).c_str(),
			param[L"param"].to_string().c_str(),
			param[L"replace"].to_bool(),
			param[L"data"].to_string().c_str());
	else
		return (CVarValue)DoCCSVTableImport(
			WideToString(param[0].to_string()).c_str(),
			param[1].to_string().c_str(),
			param[2].to_bool(),
			param[3].to_string().c_str());

}};static KeywordAutoReg<CCSVImportKW> sCCSVImportKW(L"csv_import");



struct CInputBoxKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	return (CVarValue)GetDCMapCore()->GetCoreUI()->InputBox(param[0].to_string().c_str(),param[1].to_string().c_str());
}};static KeywordAutoReg<CInputBoxKW> sCInputBoxKW(L"input_box");

struct CGoToMapCellKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
    IDCMapWorkspace* workspace = cnt->GetModule()->GetCurrentWorkspace();
	if(workspace)
	{
		IDCMapPlanetManager* pPlanetMan = workspace->GetServiceTyped<IDCMapPlanetManager>("PlanetManager");
		if(pPlanetMan)
		{
            bool centre = ((*m_pKW)[0] == L'c');
			if(param.size()==1)
			{
				dcmapPOINT pt;
				if(dcmapStrToCoords(WideToString(param.to_string()).c_str(),&pt))
				{
                    pPlanetMan->SetCurrentPlanet(0,pt.x,pt.y,centre?2:1);
				}
			}
			else
            {
                pPlanetMan->SetCurrentPlanet(0,param[0].to_real(),param[1].to_real());
			}
		}
	}
	return CVarValue();
}};
static KeywordAutoReg<CGoToMapCellKW> sCGoToMapCellKW(L"goto_cell");
static KeywordAutoReg<CGoToMapCellKW> sCGoToMapCellKW2(L"centre_cell");
static KeywordAutoReg<CGoToMapCellKW> sCGoToMapCellKW3(L"center_cell");

struct CBringOnTopKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){
	GetDCMapCore()->GetCoreUI()->BringOnTop();
	return CVarValue();
}};static KeywordAutoReg<CBringOnTopKW> sCBringOnTopKW(L"bring_to_top");

struct CCurrentCellKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
    IDCMapWorkspace* workspace = cnt->GetModule()->GetCurrentWorkspace();
    if(workspace)
    {
        IDCMapPlanetManager* pPlanetMan = workspace->GetServiceTyped<IDCMapPlanetManager>("PlanetManager");
        if(pPlanetMan)
        {
            int x,y;
            if(pPlanetMan->GetCurrentPlanetCoords(&x,&y))
            {
                CVarValue ret;
                ret.set_vector();
                ret.append_value(CVarValue(x));
                ret.append_value(CVarValue(y));
                return ret;
            }
        }
    }
    return CVarValue();
}};
static KeywordAutoReg<CCurrentCellKW> sCCurrentCellKW(L"current_cell");


struct CGetGraphicRegionsKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
    IDCMapWorkspace* workspace = cnt->GetModule()->GetCurrentWorkspace();
    if(!workspace)return CVarValue();

    IDCMapLayeredMapView* pView;
    if(workspace->SelectModuleByID("MapView"))
    {
        pView = workspace->Module()->Cast<IDCMapLayeredMapView>();
    }
    if(!pView)return CVarValue();

    float fx = param[0].to_real()-0.5;
    float fy = param[1].to_real()-0.5;

    //DataRefVector regions;
    int total=0;

    CDCMapVarValue Var;

    int n = pView->NumLayers();
    for(int i=0;i<n;i++)
    {
        if(!pView->LayerVisible(i))continue;
        IDCMapLayer* pLayer = pView->LayerInterface(i);
        if(!pLayer)continue;
        IDCMapGraphicLayer* layer = pLayer->Cast<IDCMapGraphicLayer>();
        if(!layer)continue;
        layer->TestPoint(fx,fy,&Var);
    }
    return Var;
}};
static KeywordAutoReg<CGetGraphicRegionsKW> sGetGraphicRegionsKW(L"get_graphics_at");

struct CShowMapLayerKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
    IDCMapWorkspace* workspace = cnt->GetModule()->GetCurrentWorkspace();
    if(!workspace)return CVarValue();

    IDCMapLayeredMapView* pView;
    if(workspace->SelectModuleByID("MapView"))
    {
        pView = workspace->Module()->Cast<IDCMapLayeredMapView>();
    }
    if(!pView)return CVarValue();

    wstring name = param[0].to_string();
    bool show = param[1].to_bool();

    //DataRefVector regions;
    int total=0;

    CDCMapVarValue Var;

    int n = pView->NumLayers();
    for(int i=0;i<n;i++)
    {
        if(name == pView->LayerName(i))
            pView->SetLayerVisiblity(i,show);

        if(name == pView->LayerCaption(i))
            pView->SetLayerVisiblity(i,show);
    }
    return Var;
}};
static KeywordAutoReg<CShowMapLayerKW> sShowMapLayerKW(L"show_map_layer");

struct CForceUpdateDataKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
    IDCMapCore* core = cnt->GetModule()->GetCore();
    if(!core)return CVarValue();
    core->ForceDataUpdate();
    return CVarValue();
}};
static KeywordAutoReg<CForceUpdateDataKW> sForceUpdateDataKW(L"update_data");

struct CUpdateMapKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
    IDCMapWorkspace* workspace = cnt->GetModule()->GetCurrentWorkspace();
    if(!workspace)return CVarValue();

    IDCMapLayeredMapView* pView;
    if(workspace->SelectModuleByID("MapView"))
    {
        pView = workspace->Module()->Cast<IDCMapLayeredMapView>();
    }
    if(!pView)return CVarValue();

    pView->UpdateMapRect(0);

    return CVarValue();
}};
static KeywordAutoReg<CUpdateMapKW> sUpdateMapKW(L"update_map");


struct CDBTableInfoDlgKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
	IDCMapDataStorage* stor = GetDCMapCore()->GetDataSourceTyped<IDCMapDataStorage>(ToStr(param[0].to_string()).c_str());
	if(!stor)return CVarValue();
	int r = stor->NumColumns();
	wstring out;
	for(int i=1;i<r;i++)
	{
		if(i>0)out += L"\n";
		wstring name = ToWide(stor->ColumnName(i));
		int cparam = stor->ColumnParams(i); 
		if(cparam & DCMAP_COLUMN_KEY)out += L"*";
		out += name;
		out += L" : ";
		DCMapDataType type = stor->ColumnDataType(i);
		switch(type)
		{
		case DCMDT_Number: out += L"Ineger";out+=L"["+ToWStr(stor->ColumnDataSize(i))+L"]";break;
		case DCMDT_Unsigned: out += L"Unsigned";out+=L"["+ToWStr(stor->ColumnDataSize(i))+L"]";break;
		case DCMDT_String: out += L"String";out+=L"["+ToWStr(stor->ColumnDataSize(i))+L"]";break;
		case DCMDT_CLOB: out += L"Memo";break;
		case DCMDT_BLOB: out += L"BLOB";break;
		case DCMDT_WideString: out += L"WideString";out+=L"["+ToWStr(stor->ColumnDataSize(i)/2)+L"]";break;
		case DCMDT_WideCLOB: out += L"WideMemo";break;
		case DCMDT_Real: out += L"Real";break;
		case DCMDT_Time: out += L"Time";break;
		case DCMDT_Flag: out += L"Boolean";break;
		case DCMDT_Reference: out += L"Reference";out+=L"["+ToWStr(stor->ColumnDataLinkage(i))+L"]";break;
		default: out += L"Unknown";break;
		}

	}
	return CVarValue(out);

}};static KeywordAutoReg<CDBTableInfoDlgKW> sCDBTableInfoDlgKW(L"dbtable_info");