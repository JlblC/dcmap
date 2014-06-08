#include "StdAfx.h"
#include "mod_html_ui.h"
#include "Utils.h"
#include <boost/regex.hpp>
#include <boost/scoped_ptr.hpp>
#include "modules.h"
using namespace gsys;

REG_MODULE(CHTMLUIModule,html_ui);

CHTMLUIModule::CHTMLUIModule()
{
}

CHTMLUIModule::~CHTMLUIModule()
{

}

bool DCMAPAPI CHTMLUIModule::Init(IDCMapCore* pCore,IDCMapWorkspace *pWorkspace)
{
    if(!Inherited::Init(pCore,pWorkspace))return false;

    return true;
}

void* DCMAPAPI CHTMLUIModule::CastTo(DCMapInterfaceID idInterface)
{
    if(DCMAP_INTERFACE_CMP(idInterface,IDCMapUIPanel) && html_frame)
        return html_frame->CastTo(idInterface);

    return Inherited::CastTo(idInterface);
}

bool CHTMLUIModule::InitObject()
{
    if(!Inherited::InitObject())return false;

    IDCMapUIItem* pItem = m_pCore->GetCoreUI()->CreateUIModule("HTMLFrame",this,0);
    if(!pItem)return false;
    html_frame = pItem->Cast<IDCMapUIHTMLFrame>();
    if(!html_frame)return false;
    return true;
}


bool CHTMLUIModule::ParseCustomKeyword(bool active,wstring const& kw,
                                       wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt)
{
    CVarValue params;
    if(kw == L"modaldlg")
    {
        cnt->ParseParams(active,is,params,1,2);

        if(active && html_frame)
        {
            if(params[0].to_bool())
                html_frame->LoadHTML(params[0].as_string().c_str(),params[1].as_string().c_str(),this);

            res = m_pCore->GetCoreUI()->ShowModal(html_frame,0,0);          
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }
    else if(kw == L"loadhtml")
    {
        cnt->ParseParams(active,is,params,1,2);

        if(active && html_frame)
        {
            if(params[0].to_bool())
                html_frame->LoadHTML(params[0].as_string().c_str(),params[1].as_string().c_str(),this);
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
    }
    else if(kw == L"reload")
    {
        if(active && html_frame)
        {
            html_frame->Reload(this);
        }
    }
    else if(kw == L"reformat")
    {
        if(active && html_frame)
        {
            html_frame->Reformat();
        }
    }
    else if(!expr && (kw == L"doc" || kw == L"document"))
    {
        is >> wsp_nl_ucmt_scip;
        int rpos = is.tellg();
        if(is.peek() != L'.')throw parse_exception(is,PERR_UNEXPECTED_SYMBOL,is.peek());
        is.ignore();
        wstring elem;
        wstring attrib;
        is >> wistream_keyword_ext(elem) >> wsp_nl_ucmt_scip;
        if(is.peek() == L'.')
        {
            is.ignore();
            is >> wistream_keyword_ext(attrib) >> wsp_nl_ucmt_scip;
        }
        if(is.peek() != L'=')
        {
            is.seekg(rpos);
        }
        else
        {
            CVarValue val;
            is.ignore();
            is >> wsp_nl_ucmt_scip;
            cnt->ParseExpression(active,is,val);
            if(active && html_frame)
            {
                bool r = html_frame->SetObjectAttribute(elem.c_str(),attrib.c_str(),val.as_string().c_str());
                if(!r) throw parse_exception(is,PERR_CUSTOM,L"invalid html object");
            }
            return true;
        }       
    }
    return Inherited::ParseCustomKeyword(active,kw,is,res,expr,cnt);
}


int DCMAPAPI CHTMLUIModule::OnMessage( int Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender )
{
    static CVarValue Result;
    switch(Msg)
    {
        case DCMM_UI_COMMAND_STR_FUNC:
        {
            dcmapDCSTR scr = (dcmapDCSTR)lParam;
            Result.set_nil();
            m_pCore->LockDataUpdate();
            SafeEval(scr,CVarValue(),Result);
            m_pCore->UnlockDataUpdate();
            return (int)Result.as_string().c_str(); 
        }
        case DCMM_UI_COMMAND_STR_PROC:
        {
            dcmapDCSTR scr = (dcmapDCSTR)lParam;
            CVarValue Res;
            m_pCore->LockDataUpdate();
            SafeEval(scr,CVarValue(),Res);
            m_pCore->UnlockDataUpdate();
            return 0; 
        }
    }
    if(html_frame && !pSender)
        html_frame->OnMessage(Msg,wParam,lParam,pSender);
    
    return Inherited::OnMessage(Msg,wParam,lParam,pSender);
}

void CHTMLUIModule::GetVariable( bool active,wstring const& kw,wiparsestream &is,CVarValue& res,CBasicBlockParser* cnt )
{
    if(kw == L"doc" || kw == L"document")
    {
        is >> wsp_nl_ucmt_scip;
        int rpos = is.tellg();
        if(is.peek() != L'.')throw parse_exception(is,PERR_UNEXPECTED_SYMBOL,is.peek());
        is.ignore();
        wstring elem;
        wstring attrib;
        is >> wistream_keyword_ext(elem) >> wsp_nl_ucmt_scip;
        if(is.peek() == L'.')
        {
            is.ignore();
            is >> wistream_keyword_ext(attrib) >> wsp_nl_ucmt_scip;
        }
        if(active && html_frame)
        {
            dcmapDCSTR r = html_frame->GetObjectAttribute(elem.c_str(),attrib.c_str());
            if(!r) throw parse_exception(is,PERR_CUSTOM,L"invalid html object");
            res.set_string(r);
        }
    }
    else
    {
        Inherited::GetVariable(active,kw,is,res,cnt);
    }
}