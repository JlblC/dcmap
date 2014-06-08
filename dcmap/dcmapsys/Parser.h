#pragma once
#include "../dcmapsyslib.h"
#include <g_text.h>
#include "varvalue.h"
#include <stack>
#include "parse_errors.h"

#include "parsestream.h"

enum EParserVariableScope
{
	PRSR_SCOPE_DEFAULT,
	PRSR_SCOPE_LOCAL,
	PRSR_SCOPE_GLOBAL
};


enum EParserRights
{
    PRSR_DB_READ        = 1 << 0,
    PRSR_DB_WRITE       = 1 << 1,
    PRSR_FILE_READ      = 1 << 2,
    PRSR_FILE_WRITE     = 1 << 3,
    PRSR_WEBACCESS      = 1 << 4,
    PRSR_SYSCLOSE       = 1 << 5,
    PRSR_VMAP_SETPOS    = 1 << 6,
    PRSR_CFG_READ       = 1 << 7,
    PRSR_CFG_WRITE      = 1 << 8,
    PRSR_CLBRD_READ     = 1 << 9,
    PRSR_CLBRD_WRITE    = 1 << 10,
    PRSR_OSEXEC         = 1 << 11,
    PRSR_INCLUDE        = 1 << 12,
    PRSR_UI_BASIC       = 1 << 13,
    PRSR_UI_ADVANCED    = 1 << 14,
    PRSR_DECLARE_OBJ    = 1 << 15,
    PRSR_MSG_HANDLE     = 1 << 16,
    PRSR_MODEXEC        = 1 << 17,
    PRSR_DEF_VAR        = 1 << 18,
    PRSR_DEF_SUB        = 1 << 20,
    PRSR_DEF_OBJ        = 1 << 21,
    PRSR_DEF_MOD        = 1 << 22,
	PRSR_DEF_GLOBAL_SUB = 1 << 22,
	PRSR_DEF_GLOBAL_VAR = 1 << 23,
    
    // Typical right set

    PRSR_ALL            = 0xffffffff,
    PRSR_NONE           = 0,
    PRSR_READ           = PRSR_DB_READ|PRSR_FILE_READ|PRSR_CLBRD_READ|PRSR_CFG_READ,
    PRSR_SAFE           = PRSR_READ|PRSR_CLBRD_WRITE|PRSR_VMAP_SETPOS|PRSR_UI_BASIC,

    PRSR_FILTER         = PRSR_READ,
};

enum EParseOptions
{
	PRSO_TREAT_UNKNOWN_KEYWORD_AS_STRING  = 1 << 0,


	PRSO_DEFAULT = PRSO_TREAT_UNKNOWN_KEYWORD_AS_STRING
};

enum EVarContext{VarContexObject,VarContexScalar,VarContexVector};

struct die_exception  : public std::exception
{
	std::wstring reason;
	die_exception(dcmapWCSTR ch):reason(ch){}
	virtual ~die_exception() throw() {}
};
struct break_exception  : public std::exception{};
struct continue_exception  : public std::exception{};
struct return_exception  : public std::exception
{
	CVarValue val;
	explicit return_exception(CVarValue const& v):val(v){}
	virtual ~return_exception() throw() {}
};


class CBasicBlockParser;
class CBlockParser;
class CDBBlockParser;
class CObjectParser;
class CModuleParser;
class CScriptModule;

class CBaseOperator
{
protected:
    int  m_prio;
    bool m_binary,m_or,m_and,m_regexp,m_unary,m_keyword;
public:
    CBaseOperator():
          m_prio(0),
          m_binary(1),
          m_or(0),
          m_and(0),
          m_regexp(0),
          m_keyword(0),
          m_unary(0)
    {}
	virtual void execute( CVarValue const& op,CVarValue & res,CBasicBlockParser* prs){};
	virtual void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs){};

	int priority(){return m_prio;}
	bool binary(){return m_binary;}
	bool is_or(){return m_or;}
	bool is_and(){return m_and;}
	bool is_regexp(){return m_regexp;}
	bool is_unary(){return m_unary;}
    bool is_keyword(){return m_keyword;}
};
CBaseOperator* GetParserOperator(wiparsestream &is);

bool ParseStaticKeywords(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt);

bool CmdExec(wstring const& CmdLine,wstring & Result);

typedef std::pair<int,int> CurcleBlockDef;

class CBasicBlockParser
{
	friend class CBlockParser;
	friend class CDBBlockParser;
protected:
	CBasicBlockParser* m_pParent;
	wchar_t m_chQoute;
    dcmapDWORD m_Rights;
	dcmapDWORD m_ParseOptions;
public:
    short opresult;
    short ifresult;
protected:
	void ParseStringLexGroup(bool active,wiparsestream &is,wstring& result,wchar_t qtch,bool groupping=true,bool decode=true);
	void ContinueExpressionOp(bool active,int stop_prio, CVarValue& v1,CBaseOperator* op,wiparsestream &is,
		CVarValue& res,CBaseOperator* &nextop,CBasicBlockParser* cnt);

	virtual bool ParseCustomKeyword(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt);
	void ParseRegExp(bool active,wiparsestream &is,CVarValue const& data,CVarValue& res);
public:
	CBasicBlockParser* GetRoot(){return m_pParent?m_pParent->GetRoot():this;}
    virtual CModuleParser* GetModule(){return m_pParent?m_pParent->GetModule():0;}

	void ParseKeyword(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr=false);

	void BeginCircleBlock(wiparsestream &is,CurcleBlockDef& def);
	CBasicBlockParser* PrepareCircleBlock(wiparsestream &is,CurcleBlockDef& def);
	bool ProcessCircleBlock(wiparsestream &is,CVarValue& res,CurcleBlockDef& def,CBasicBlockParser* proc=0);
	void EndCircleBlock(wiparsestream &is,CVarValue& res,CurcleBlockDef& def);

	virtual CVarValue&  GetDefaultValue();
	void ParseBlock(bool active,wiparsestream &is,CVarValue& result);
	void ParseBlock(bool active,wiparsestream &is,CVarValue& result,CVarValue const& locals);
	void ParseBasicBlock(bool active,wiparsestream &is,CVarValue& result);

	virtual void ContinueExpression(bool active,wiparsestream &is,CVarValue& res,int stop_prio=0);
	virtual void ParseExpression(bool active,wiparsestream &is,CVarValue& res,int stop_prio=0);
	virtual void ParseValue(bool active,wiparsestream &is,CVarValue& res,CBasicBlockParser* cnt);
	virtual void SetVariable(bool active,wstring const& kw,CVarValue const& val,CVarValue & res);
	virtual void SetLocalVariable(bool active,wstring const& kw,CVarValue const& val,CVarValue & res);
	virtual bool GetVariableLValue(wstring const& kw,int scope,CVarValue ** result);
	virtual void GetVariable(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,CBasicBlockParser* cnt);

	virtual void SetDefaultValue(CVarValue const& val){if(m_pParent)m_pParent->SetDefaultValue(val);}

public:
	CBasicBlockParser();
    virtual ~CBasicBlockParser(){Destruct();};

	virtual void Destruct(){};
	virtual bool Construct(){return true;};

	virtual int ParseString(bool active,wiparsestream &is,CVarValue& res,bool quoted,bool groupped=true,bool decode=true,bool keep_quote=false);
	virtual void Parse(bool active,wiparsestream &is,CVarValue& result,bool quoted=false);
    virtual CObjectParser* GetObjectParser(){return m_pParent?m_pParent->GetObjectParser():0;}

    void ParseParams(bool active,wiparsestream &is,CVarValue& params,int pmin,int pmax=0);

	static IDCMapProcessRecordReseiver* pRsrv;

    dcmapDWORD Rights() const { return m_Rights; }
    void Rights(dcmapDWORD val) { m_Rights = val; }

	bool CheckRights(dcmapDWORD val) {return m_Rights & val == val;}

	dcmapDWORD ParseOptions() const { return m_ParseOptions; }
	void ParseOptions(dcmapDWORD val) { m_ParseOptions = val; }

    CBasicBlockParser* Parent() const { return m_pParent; }
    void Parent(CBasicBlockParser* val) 
    {
        if(val)
		{
			m_ParseOptions=val->m_ParseOptions;
			m_Rights=val->m_Rights; 
		}
		m_pParent = val; 
    }
	bool Permited(EParserRights right){return (m_Rights & right) == right;}

	// parent parsers get
	virtual CBlockParser* BlockParser() { return m_pParent?m_pParent->BlockParser():0; }
	virtual CObjectParser* ObjectParser() { return m_pParent->ObjectParser(); }
	virtual CDBBlockParser* DBBlockParser() { return m_pParent?m_pParent->DBBlockParser():0; }
	virtual CModuleParser* ModuleParser() { return m_pParent?m_pParent->ModuleParser():0; }
	virtual CScriptModule* ScriptModule() { return m_pParent?m_pParent->ScriptModule():0; }
};

class CBlockParser : public CBasicBlockParser
{
    typedef CBasicBlockParser Inherited;
protected:
    CVarValue m_valDefault;
	typedef std::map<wstring,CVarValue> VarMap;
	typedef std::vector<CVarValue> VarVector;

	VarMap m_Locals;
	VarVector m_LocalUsings;

	virtual void SetVariable(bool active,wstring const& kw,CVarValue const& val,CVarValue & var);
	virtual void SetLocalVariable(bool active,wstring const& kw,CVarValue const& val,CVarValue & var);

	virtual void GetVariable(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,CBasicBlockParser* cnt);
	virtual CVarValue&  GetDefaultValue();
public:
	virtual void Destruct();
	virtual void SetDefaultValue(CVarValue const& val){m_valDefault = val;}
	void SetVariable(wstring const& name,CVarValue const& val){m_Locals[name]=val;}
	void UseVariable(CVarValue const& val);
	void SetDefaultValue(wstring const& str){m_valDefault = str;}
    bool GetVariableLValue(wstring const& kw,int scope,CVarValue ** result);
	CBlockParser* BlockParser(){ return this;}

    CVarValue GetLocalVariable(wstring const& kw)
    {
        VarMap::iterator it = m_Locals.find(kw);
        if(it != m_Locals.end())
        {
            return it->second;
        }
        return CVarValue();
    }

    bool GetLocalVariableLValue(wstring const& kw,CVarValue ** result)
    {
        *result = &m_Locals[kw];
        return true;
    }
};

class CObjectParser : public CBlockParser
{
    typedef CBlockParser Inherited;
protected:
    int m_iRefCount;
public:
    CObjectParser():m_iRefCount(1){}
	virtual ~CObjectParser();
    virtual CObjectParser* GetObjectParser(){return this;}
    int AddRef(){return ++m_iRefCount;}
    int Release(){return --m_iRefCount;}

    virtual bool InitObject(){return true;}

	virtual CObjectParser* ObjectParser() { return this; }

};


class CDBBlockParser : public CObjectParser
{
    typedef CObjectParser Inherited;
protected:
	IDCMapCore* m_pCore;
	IDCMapDataStoragePtr m_pData;
	dcmapDBREF m_CurrentRef;
	bool m_bReadOnly;

	struct ValueInfo
	{
		CVarValue var;
		bool bIndex;
		IDCMapDataStoragePtr m_pRef;
		int iType;
	};
	typedef std::vector<ValueInfo> VecCurrentValues;
	VecCurrentValues m_vecValues;
	string m_TableName;

	struct StateData
	{
		IDCMapDataStoragePtr pData;
		dcmapDBREF CurrentRef;
		VecCurrentValues values;
	};
	std::stack<StateData> states;
	void PushState()
	{
		StateData dta;
		dta.pData = m_pData;
		dta.CurrentRef = m_CurrentRef;
		dta.values = m_vecValues;
		states.push(dta);
	}
	void PopState()
	{
		StateData& dta = states.top();
		SetTable("",dta.pData);
		SelectID(dta.CurrentRef);
		m_vecValues = dta.values;
		states.pop();
	}	

	void ClearValues();
	void GetTableVariable(bool active,IDCMapDataStorage* pData, dcmapDBREF Ref, int id,wiparsestream &is,CVarValue& res);
	void SetVariable(bool active,wstring const& kw,CVarValue const& val,CVarValue & res);

protected:
	virtual bool ParseCustomKeyword(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt);
	virtual void GetVariable(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,CBasicBlockParser* cnt);
public:
	CDBBlockParser();

	virtual CDBBlockParser* DBBlockParser() { return this; }

	bool GetVariableLValue(wstring const& kw,int scope,CVarValue ** result);
	void SetVariable(wstring const& name,CVarValue const& val){CBlockParser::SetVariable(name,val);}
	void SetReadOnly(bool b){m_bReadOnly = b;}
	bool SetTable(dcmapCSTR name,IDCMapDataStorage* pDataStorage=0);
	bool Select();
	bool Delete();
	bool SelectID(dcmapDBREF const& m_CurrentRef);
	bool Retrieve();
	bool Post();
	bool Cansel();
	void Clear();
	bool Insert();
	bool Update();
	void Destruct();
};

class CModuleParser : public IDCMapScriptModule, public CDBBlockParser
{
	typedef CDBBlockParser Inherited;

protected:
	struct ActionHandler
	{
		CVarValue test_handle;
		CVarValue action_handle;
		ActionHandler(){}
		ActionHandler(CVarValue const& v):action_handle(v){}
		ActionHandler(CVarValue const& v,CVarValue const& v2):action_handle(v),test_handle(v2){}
	};

	typedef std::vector<ActionHandler> VecHandlers;
	VecHandlers m_vecHandlers;

	typedef std::map<int,ActionHandler> MsgHandlers;
	MsgHandlers m_handlers;

protected:
    CModuleParser* m_pRootModule;
    CModuleParser* m_pParentModule;
    IDCMapWorkspace* m_pWorkspace;
    bool ParseCustomKeyword(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt);
public:
	virtual bool DCMAPAPI Init(IDCMapCore* pCore,IDCMapWorkspace *pWorkspace);

	bool DCMAPAPI CallProc(dcmapWCSTR Proc,IDCMapVarValue* Params,IDCMapVarValue* Result);
public:
   CModuleParser();
    virtual ~CModuleParser(){};
    virtual CModuleParser* GetModule(){return this;}
    virtual bool InitObject();
    bool SafeExecute(CVarValue val,CVarValue param,CVarValue& res);
	bool DbSafeExecute(CVarValue val,CVarValue param,CVarValue& res);
	bool UIExecute(CVarValue val,CVarValue param,CVarValue& res);
    bool SafeEval(dcmapDCSTR srcipt,CVarValue param,CVarValue& res);
    bool Eval(dcmapDCSTR srcipt,CVarValue param,CVarValue& res);
    void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
    int DCMAPAPI OnMessage(int Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender);

    IDCMapWorkspace* GetCurrentWorkspace()
    {
        if(m_pWorkspace) return m_pWorkspace;
        return m_pCore->GetCurrentWorkspace();
    }
    IDCMapCore* GetCore(){return m_pCore;}

	virtual CModuleParser* ModuleParser() { return this; }

};

extern CModuleParser g_GlobalModule; 