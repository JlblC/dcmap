#include "StdAfx.h"
#include "Parser.h"
#include "Utils.h"
#include <boost/regex.hpp>
#include <boost/scoped_ptr.hpp>
#include "Keywords.h"

#include <g_text.h>
using namespace gsys;

void inline check_db_read(wiparsestream& is,CBasicBlockParser* cnt)
{
	if(!cnt->Permited(PRSR_DB_READ))
		throw(parse_exception(is,PERR_INSUFFICIENT_RIGHRS),L"database read");
}
void inline check_db_write(wiparsestream& is,CBasicBlockParser* cnt)
{
	if(!cnt->Permited(PRSR_DB_WRITE))
		throw(parse_exception(is,PERR_INSUFFICIENT_RIGHRS),L"database write");
}

struct CTableKW: CBaseKeyword{bool Parse(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt)
{
	wstring tn;
	is >> wistream_keyword_ext(tn)>> wsp_nl_ucmt_scip;
	if(active)
	{
		check_db_read(is,cnt);

		CDBBlockParser* db = cnt->DBBlockParser();
		if(!db)throw(parse_exception(is,PERR_INVALID_CONTEXT));

		res = db->SetTable(WideToString(tn).c_str());
	}
	if(!expr)cnt->ContinueExpression(active,is,res);
	return true;
}};
REG_KEYWORD(CTableKW,table);


CDBBlockParser::CDBBlockParser()
{
    m_pCore = GetDCMapCore();
    m_bReadOnly=false;
}

void CDBBlockParser::GetTableVariable(bool active,IDCMapDataStorage* pData, dcmapDBREF Ref, int id,wiparsestream &is,CVarValue& res)
{
    IDCMapDataStorage* refData=0;
    dcmapDBREF LinkRef;
    int LinkID;

    _ASSERT(id>=0);

    // get column data
    if(!pData->SelectID(Ref))
    {
        res.set_nil();
    }
    else if(pData->ColumnDataType(id) == DCMDT_Reference)
    {
        refData = m_pCore->GetDataSourceTyped<IDCMapDataStorage>(pData->ColumnDataLinkage(id));

		if(refData)
		{
			LinkRef = *pData->GetReference(id);

			if(is.peek() == L'.')
			{
				wstring field;
				is.ignore();
				is >> wistream_keyword_ext(field);
				LinkID = refData->ColumnID(WideToString(field).c_str());
				if(LinkID<0)throw parse_exception(is,PERR_CUSTOM,L"Bad field name");
				GetTableVariable(active,refData,LinkRef,LinkID,is,res);
			}
			else if(refData->SelectID(LinkRef))
			{
				res = refData->GetWideName();
			}
			else 
			{
				res = L"";
			}
		}
		else
		{
			res.set_dbref(Ref,pData);
		}
    }
    else if(pData->ColumnNumeric(id))
    {
        res = pData->GetReal(id);
    }
    else 
    {
        res = pData->GetWideString(id);
    }
}

void CDBBlockParser::GetVariable(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,CBasicBlockParser* cnt)
{
    int id=-1;
    if(m_pData && m_CurrentRef.Valid() && (id = m_pData->ColumnID(WideToString(kw).c_str())) >=0)
    {
        if(active)check_db_read(is,cnt);
        GetTableVariable(active,m_pData,m_CurrentRef,id,is,res);			
    }
    else 
    {
        Inherited::GetVariable(active,kw,is,res,cnt);
    }
}

void CDBBlockParser::SetVariable(bool active,wstring const& kw,CVarValue const& val,CVarValue & res)
{
    int id;
    if(active && m_pData && (id = m_pData->ColumnID(WideToString(kw).c_str()))>0)
    {
        m_vecValues[id].var = val;
        res = val;
    }
    else return CBlockParser::SetVariable(active,kw,val,res);
}

bool CDBBlockParser::GetVariableLValue(wstring const& kw,int scope,CVarValue ** result)
{
    int id;
    if(m_pData && (id = m_pData->ColumnID(WideToString(kw).c_str()))>0)
    {
        *result = &m_vecValues[id].var;
        return true;
    }
    return CBlockParser::GetVariableLValue(kw,scope,result);
}

bool CDBBlockParser::ParseCustomKeyword(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt)
{
    int id=-1;
    if(m_bReadOnly)
    {    
        if(m_pData && m_CurrentRef.Valid() && (id = m_pData->ColumnID(WideToString(kw).c_str())) >=0)
        {
            if(active)check_db_read(is,cnt);
            GetTableVariable(active,m_pData,m_CurrentRef,id,is,res);
            if(!expr) cnt->ContinueExpression(active,is,res);
			return true;
        }
        else
		{
			return Inherited::ParseCustomKeyword(active,kw,is,res,expr,cnt);
		}
	}
    else if(kw == L"editdatadlg" || kw == L"record_edit_ui")
    {
        if(active)
        {
            check_db_write(is,cnt);       
            if(m_CurrentRef.Valid());

            GetDCMapCore()->GetCoreUI()->OpenListTableEdit(m_TableName.c_str(),&m_CurrentRef,0);
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }
    else if(kw == L"iterate_childs")
    {
        is >> wsp_nl_ucmt_scip;

        string Table;

        if(wchar_classify::alpha_ext(is.peek()))
        {
            wstring tn;
            is >> wistream_keyword_ext(tn)>> wsp_nl_ucmt_scip;
            Table = WideToString(tn);
        }
		CVarValue parentval;
		cnt->ParseExpression(active,is,parentval);

		PushState();

		if(active && !Table.empty())res = SetTable(Table.c_str());

        IDCMapDataStorage* prnt = 0;
        if(!m_pData)active = false;
        else prnt = m_pData->ParentDataStorage();;

        // get parent id
        dcmapDBREF prid;
        if(!prnt)active = false;
        else
        {
			if(parentval.is_dbref())
			{
				if(prnt->SelectID(parentval.as_dbref()))
					prid = *prnt->GetID();
			}
			else if(prnt->SelectWideName(parentval.to_string().c_str()))
            {
                prid = *prnt->GetID();
            }
        }

        gsys::raw_data_buffer<dcmapDBREF,64,128> buf;
        buf.reset(0);
        if(active)
        {
            check_db_read(is,cnt);
            if(m_pData->SelectByParentID(prid))
                do
                {
                    buf.push_back(*m_pData->GetID());
                }			
                while(m_pData->Next());
        }
        int num = buf.size();

        CurcleBlockDef cdef;
        CVarValue r;
        cnt->BeginCircleBlock(is,cdef);
        for(int i=0;i<num;i++)
        {
            if(!SelectID(buf[i]))continue;
            CBasicBlockParser* block = cnt->PrepareCircleBlock(is,cdef);
            if(!cnt->ProcessCircleBlock(is,r,cdef,block))break;
        }
        cnt->EndCircleBlock(is,r,cdef);

        PopState();

        res=num;		
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }
    else if(kw == L"iterate_all")
    {
        is >> wsp_nl_ucmt_scip;

        PushState();

        string Table;

        if(wchar_classify::alpha_ext(is.peek()))
        {
            wstring tn;
            is >> wistream_keyword_ext(tn)>> wsp_nl_ucmt_scip;
            Table = WideToString(tn);
            if(active)res = SetTable(Table.c_str());
        }
        if(!m_pData)active = false;

        gsys::raw_data_buffer<dcmapDBREF,128,256> buf;
        buf.reset(0);
        if(active)
        {
            check_db_read(is,cnt);
            buf.reserve(m_pData->NumRecords());
            if(m_pData->SelectAll())
                do
                {
                    buf.push_back(*m_pData->GetID());
                }			
                while(m_pData->Next());
        }
        int num = buf.size();

        CVarValue r;
        CurcleBlockDef cdef;

        cnt->BeginCircleBlock(is,cdef);
        for(int i=0;i<num;i++)
        {
            if(!Table.empty())SetTable(Table.c_str());
            if(!SelectID(buf[i]))continue;
            CBasicBlockParser* block = cnt->PrepareCircleBlock(is,cdef);
            if(!cnt->ProcessCircleBlock(is,r,cdef,block))break;
        }
        cnt->EndCircleBlock(is,r,cdef);

        PopState();

        res=num;		
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }
    else if(kw == L"delete")
    {
        if(active)
        {
            check_db_write(is,cnt);
            res = Delete();
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }	
    else if(kw == L"select")
    {
        CVarValue parametrs;
        cnt->ParseParams(active,is,parametrs,0);
        if(active)
        {
            check_db_read(is,cnt);

            if(parametrs[0].type() == VT_DBREF)
            {
				SetTable(0,parametrs[0].dbstorage());
                res = SelectID(parametrs[0].as_dbref());
            }
            else res = Select();
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }
    else if(kw == L"clear")
    {
        if(active)
        {
            check_db_read(is,cnt);
            Clear();
        }
        res = active;
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }	

    else if(kw == L"post")
    {
        if(active)
        {
            check_db_write(is,cnt);
            res = Post();
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }

    else if(kw == L"cancel")
    {
        if(active)
        {
            check_db_read(is,cnt);
            res = Cansel();
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }	

    else if(kw == L"retrieve")
    {
        if(active)
        {
            check_db_write(is,cnt);
            res = Retrieve();
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }	
    else if(kw == L"insert")
    {
        if(active)
        {
            check_db_write(is,cnt);
            res = Insert();
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }
    else if(kw == L"update")
    {
        if(active)
        {
            check_db_write(is,cnt);
            res = Update();
        }
        if(!expr)cnt->ContinueExpression(active,is,res);
        return true;
    }
    else if(active && m_pData && (id = m_pData->ColumnID(WideToString(kw).c_str()))>0)
    {
        is >> wsp_nl_ucmt_scip;
        wchar_t ps = is.peek();
        if(ps==L'=')
        {
            is.ignore();
            is>>wsp_nl_ucmt_scip;
            cnt->ParseExpression(active,is,res);
            if(active)
            {
                check_db_read(is,cnt);
                m_vecValues[id].var = res;
            }
			return true;
        }
        else 
        {
            throw parse_exception(is,PERR_CUSTOM,L"invalid operator");
        }
    }
    else return Inherited::ParseCustomKeyword(active,kw,is,res,expr,cnt);

    return false;
}

void CDBBlockParser::ClearValues()
{
    int num = m_vecValues.size();
    for(int i=0;i<num;i++)
    {
        m_vecValues[i].var.set_nil();
    }
}

bool CDBBlockParser::SetTable(dcmapCSTR name,IDCMapDataStorage* pDataStorage)
{
    if(pDataStorage)
    {
        if(m_pData == pDataStorage)return true;
        if(!m_pData.init(pDataStorage))return false;
        m_TableName = m_pData->Name();
    }
    else 
    {
        if(m_pData && m_TableName == name)return true;
        m_TableName = name;
        if(!m_pData.init(m_pCore,name))return false;
    }

    int num = m_pData->NumColumns();

    m_vecValues.clear();
    m_vecValues.resize(num);

    for(int i=0;i<num;i++)
    {
        m_vecValues[i].var.set_nil();
        m_vecValues[i].bIndex = (m_pData->ColumnParams(i) & DCMAP_COLUMN_INDEX);
        m_vecValues[i].iType = m_pData->ColumnNumeric(i)?VT_NUM:VT_STR;
        if(m_pData->ColumnDataType(i) == DCMDT_Reference)
        {
            m_vecValues[i].m_pRef.init(m_pCore,m_pData->ColumnDataLinkage(i));
        }
    }
    return true;
}

bool CDBBlockParser::SelectID(dcmapDBREF const& pRef)
{
    if(!m_pData)return false;
    if(!m_pData->SelectID(pRef))
    {
        m_CurrentRef.Invalidate();
        return false;
    }
    m_CurrentRef = pRef;
    return true;
}

bool CDBBlockParser::Delete()
{
    if(!m_pData)return false;
    if(!m_CurrentRef.Valid())return false;
    if(!m_pData->SelectID(m_CurrentRef))return false;
    if(!m_pData->Delete())return false;
    m_CurrentRef.Invalidate();
    return true;
}

bool CDBBlockParser::Select()
{
    if(!m_pData)return false;

    m_pData->EditIndex();

    int num = m_vecValues.size();
    for(int i=0;i<num;i++)
    {
        ValueInfo &vi = m_vecValues[i];
        if(vi.bIndex && !vi.var.is_nil())
        {
            if(vi.var.numeric())m_pData->SetReal(i,vi.var.as_real());
            else m_pData->SetWideString(i,vi.var.as_string().c_str());
        }
    }
    if(!m_pData->SelectIndex())
    {
        m_CurrentRef.Invalidate();
        return false;
    }
    m_CurrentRef = *m_pData->GetID();
    return true;
}
bool CDBBlockParser::Retrieve()
{
    if(!m_pData)return false;
    if(Select())return true;
    m_pData->Insert();

    int num = m_vecValues.size();
    for(int i=0;i<num;i++)
    {
        ValueInfo &vi = m_vecValues[i];
        if(vi.bIndex && !vi.var.is_nil())
        {
            if(vi.var.numeric())m_pData->SetReal(i,vi.var.as_real());
            else m_pData->SetWideString(i,vi.var.as_string().c_str());
        }
    }
    if(!m_pData->Post(&m_CurrentRef))
    {
        m_CurrentRef.Invalidate();
        return false;
    }
    return true;
}

bool CDBBlockParser::Insert()
{
    if(!m_pData)return false;
    Select();
    return Post();
}

bool CDBBlockParser::Update()
{
    if(!m_pData)return false;
    if(!Select())return false;
    return Post();
}

bool CDBBlockParser::Post()
{
    if(!m_pData)return false;

    if(!m_CurrentRef.Valid())
    {
        Retrieve();
    }

    if(!m_pData->SelectID(m_CurrentRef))return false;
    if(!m_pData->Update())return false;

    int num = m_vecValues.size();
    for(int i=0;i<num;i++)
    {
        ValueInfo &vi = m_vecValues[i];
        if(!vi.var.is_nil())
        {
            if(vi.m_pRef)
            {
                dcmapDBREF ref;
                if(!vi.m_pRef->RetrieveByWideName(vi.var.as_string().c_str(),&ref)){m_pData->Cansel();return false;}
                m_pData->SetReference(i,ref);
            }
            else if(vi.var.numeric())m_pData->SetReal(i,vi.var.as_real());
            else m_pData->SetWideString(i,vi.var.as_string().c_str());
        }
    }
    if(!m_pData->Post(&m_CurrentRef))
    {
        m_CurrentRef.Invalidate();
        return false;
    }
    ClearValues();
    return true;
}

bool CDBBlockParser::Cansel()
{
    if(!m_pData)return false;
    m_pData->Cansel();
    ClearValues();
    return true;
}

void CDBBlockParser::Clear()
{
    if(m_pData)m_pData->Cansel();
    m_CurrentRef.Invalidate();
}

