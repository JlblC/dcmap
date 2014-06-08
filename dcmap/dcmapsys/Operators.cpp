#include "StdAfx.h"
#include "Parser.h"
#include "Operators.h"

#include <g_text.h>
using namespace gsys;

static CBaseOperator* s_Operators[NUM_OPERATORS] = {0}; 

CBaseOperator* GetParserOperator(EParserOperators id)
{
	return s_Operators[id];
}

struct OpRegister
{
	OpRegister(CBaseOperator* op,EParserOperators id)
	{
		s_Operators[id] = op;
	}
};

#define REGOP(name,id) static name; static OpRegister name##r(&name,id);

enum 
{
	OpPrioComma,   // ,
	OpPrioHash,	   // =>
	OpPrioRange,   // ..	
	OpPrioSelect,  // ?:
	OpPrioOr,      // or
	OpPrioAnd,     // and 
	OpPrioEqual,   // = <> is 
	OpPrioCompare, // > < >= <=   
	OpPrioConcat,  // ::

	OpPrioAdd,     // + -
	OpPrioMult,    // * /
	OpPrioPow,     // ^
	OpPrioCnt,     // cnt nct
	OpPrioRegexp,  // =~ 
	OpPrioUnary,   // ! etc
	OpPrioCall,    // ()
	OpPrioSubObj = OpPrioCall,  // .
	OpPrioArrElem = OpPrioSubObj,  // []
};

struct COperatorRange : public CBaseOperator
{
    COperatorRange()
    {
        m_prio=OpPrioRange;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
		int min = op1.to_real();
		int max = op2.to_real();
		res = min;
		if(min<max)
		{
			for(int i=min+1;i<=max;i++) res.append(CVarValue(i));
		}
		else
		{
			for(int i=min-1;i>=max;i--) res.append(CVarValue(i));
		}	
	};
	int priority(){return OpPrioRange;}
}static s_OpRange;

struct COperatorComma : public CBaseOperator
{
    COperatorComma()
    {
        m_prio=OpPrioComma;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
		res =  op1;
		res.append(op2);
	};
	int priority(){return OpPrioComma;}
}static s_OpComma;

struct COperatorHash : public CBaseOperator
{
    COperatorHash()
    {
        m_prio=OpPrioHash;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
		res.set_hash(op1,op2);
	};
	int priority(){return OpPrioHash;}
}static s_OpHash;

struct COperatorSelA : public CBaseOperator
{
    COperatorSelA()
    {
        m_prio=OpPrioSelect;
        m_and=true;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
		if(op1.to_bool())
		{
			res =  op2;
			prs->opresult = 1;
		}
		else 
		{
			res = op1;
			prs->opresult = -1;
		}
	};
	int priority(){return OpPrioSelect;}
	bool is_and(){return true;}
}static s_OpSelA;

struct COperatorSelNotA : public CBaseOperator{
    COperatorSelNotA()
    {
        m_prio=OpPrioSelect;
        m_or=true;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
		if(!op1.to_bool())
		{
			res =  op2;
			prs->opresult = 1;
		}
		else 
		{
			res = op1;
			prs->opresult = -1;
		}
	};
	int priority(){return OpPrioSelect;}
	bool is_or(){return true;}
}static s_OpSelNotA;

struct COperatorSelB : public CBaseOperator
{
    COperatorSelB()
    {
        m_prio=OpPrioSelect;
        m_or=true;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
		if(prs->opresult)
		{
			if(prs->opresult == 1)res = op1;
			else res = op2;
			prs->opresult=0;
		}
		else 
		{
			if(!op1.to_bool())res =  op2;
			else res = op1;
		}
	};
	int priority(){return OpPrioSelect;}
	bool is_or(){return true;}
}static s_OpSelB;


struct COperatorRegExp : public CBaseOperator
{
    COperatorRegExp()
    {
        m_prio=OpPrioRegexp;
        m_regexp=true;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op2;};
	int priority(){return OpPrioRegexp;}
	bool is_regexp(){return true;}
}static s_OpRegExp;

struct COperatorNot : public CBaseOperator
{
    COperatorNot()
    {
        m_prio=OpPrioUnary;
        m_unary=true;
    }
	void execute(CVarValue const& op1,CVarValue & res,CBasicBlockParser* prs)
	{res =  !op1.to_bool();};
	int priority(){return OpPrioUnary;}
	bool is_unary(){return true;}
}static s_OpNot;

struct COperatorOr : public CBaseOperator
{
    COperatorOr()
    {
        m_prio=OpPrioOr;
        m_or=true;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_bool() || op2.to_bool();};
	int priority(){return OpPrioOr;}
	bool is_or(){return true;}
}static s_OpOr;

struct COperatorXOr : public CBaseOperator
{
    COperatorXOr()
    {
        m_prio=OpPrioOr;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  (op1.to_bool() || op2.to_bool()) && !(op1.to_bool() && op2.to_bool());};
	int priority(){return OpPrioOr;}
}static s_OpXOr;

struct COperatorAnd : public CBaseOperator
{
    COperatorAnd()
    {
        m_prio=OpPrioAnd;
        m_and=true;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_bool() && op2.to_bool();};
	int priority(){return OpPrioAnd;}
	bool is_and(){return true;}
}static s_OpAnd;

struct COperatorEqual : public CBaseOperator
{
    COperatorEqual()
    {
        m_prio=OpPrioEqual;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
		if(op1.numeric() && op2.numeric()) res =  op1.as_real() == op2.as_real();
		else res = op1.to_string() == op2.to_string();
	};
	int priority(){return OpPrioEqual;}
}static s_OpEqual;

struct COperatorIs : public CBaseOperator
{
    COperatorIs()
    {
        m_prio=OpPrioEqual;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
        if(op1.type() == VT_OBJECT || op2.type() == VT_OBJECT)
        {
            res = false;
        }
        else res = (op1.type() == op2.type());
	};
	int priority(){return OpPrioEqual;}
}static s_OpIs;

struct COperatorNotEqual : public CBaseOperator
{
    COperatorNotEqual()
    {
        m_prio=OpPrioEqual;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
		if(op1.numeric() && op2.numeric()) res =  op1.as_real() != op2.as_real();
		else res = op1.to_string() != op2.to_string();
	};
	int priority(){return OpPrioEqual;}
}static s_OpNotEqual;


struct COperatorLess : public CBaseOperator
{
    COperatorLess()
    {
        m_prio=OpPrioCompare;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_real() < op2.to_real();};
	int priority(){return OpPrioCompare;}
}static s_OpLess;

struct COperatorGreater : public CBaseOperator
{
    COperatorGreater()
    {
        m_prio=OpPrioCompare;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_real() > op2.to_real();};
	int priority(){return OpPrioCompare;}
}static s_OpGreater;

struct COperatorLessEqual : public CBaseOperator
{
    COperatorLessEqual()
    {
        m_prio=OpPrioCompare;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_real() <= op2.to_real();};
	int priority(){return OpPrioCompare;}
}static s_OpLessEqual;

struct COperatorGreaterEqual : public CBaseOperator
{
    COperatorGreaterEqual()
    {
        m_prio=OpPrioCompare;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_real() >= op2.to_real();};
	int priority(){return OpPrioCompare;}
}static s_OpGreaterEqual;

struct COperatorConcat : public CBaseOperator
{
    COperatorConcat()
    {
        m_prio=OpPrioConcat;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_string() + op2.to_string();};
	int priority(){return OpPrioConcat;}
}static s_OpConcat;

struct COperatorCnt : public CBaseOperator
{
    COperatorCnt()
    {
        m_prio=OpPrioCnt;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_string().find((op2.to_string())) != wstring::npos;};
	int priority(){return OpPrioCnt;}
}static s_OpCnt;

struct COperatorNct : public CBaseOperator
{
    COperatorNct()
    {
        m_prio=OpPrioCnt;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_string().find((op2.to_string())) == wstring::npos;};
	int priority(){return OpPrioCnt;}
}static s_OpNct;


struct COperatorPlus : public CBaseOperator
{
    COperatorPlus()
    {
        m_prio=OpPrioAdd;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_real() + op2.to_real();};
	int priority(){return OpPrioAdd;}
}
REGOP(s_OpPlus,OP_PLUS);

struct COperatorInc : public COperatorPlus
{
	COperatorInc()
	{
		m_unary = true;
	}
	void execute(CVarValue const& op,CVarValue & res,CBasicBlockParser* prs)
	{res =  op.to_real()+1;};
}
REGOP(s_OpInc,OP_INC);

struct COperatorMinus : public CBaseOperator
{
    COperatorMinus()
    {
        m_prio=OpPrioAdd;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_real() - op2.to_real();};
	int priority(){return OpPrioAdd;}
}
REGOP(s_OpMinus,OP_MINUS);

struct COperatorDec : public COperatorMinus
{
	COperatorDec()
	{
		m_unary = true;
	}
	void execute(CVarValue const& op,CVarValue & res,CBasicBlockParser* prs)
	{res =  op.to_real()-1;};
}
REGOP(s_OpDec,OP_DEC);


struct COperatorMultiply : public CBaseOperator
{
    COperatorMultiply()
    {
        m_prio=OpPrioMult;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  op1.to_real() * op2.to_real();};
	int priority(){return OpPrioMult;}
}static s_OpMultiply;

struct COperatorDivide : public CBaseOperator
{
    COperatorDivide()
    {
        m_prio=OpPrioMult;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
		dcmapREAL o2 = op2.to_real();
		if(!o2)res.set_nil();
		else res =  op1.to_real() / o2; 
	};
	int priority(){return OpPrioMult;}
}static s_OpDivide;

struct COperatorPower : public CBaseOperator
{
    COperatorPower()
    {
        m_prio=OpPrioPow;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{res =  pow(op1.to_real(),op2.to_real());};
	int priority(){return OpPrioPow;}
}static s_OpPower;

struct COperatorCall : public CBaseOperator
{
    COperatorCall()
    {
        m_prio=OpPrioCall;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
		op1.call(op2,res);
	};
}static s_OpCall;

struct COperatorSubObj : public CBaseOperator
{
    COperatorSubObj()
    {
        m_prio=OpPrioSubObj;
		m_keyword=1;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
        res = op1.subobj(op2);
	};
}static s_OpSubObj;

struct COperatorArrElem : public CBaseOperator
{
    COperatorArrElem()
    {
        m_prio=OpPrioArrElem;
    }
	void execute(CVarValue const& op1,CVarValue const& op2,CVarValue & res,CBasicBlockParser* prs)
	{
		res = op1[op2];
	};
}static s_OpArrElem;


CBaseOperator* GetParserOperator(wiparsestream &is)
{
	wchar_t ps = is.peek();
	switch(ps)
	{
	//case L'q':
	//case L'w':
	case L'e':
	//case L'r':
	//case L't':
	//case L'y':
	//case L'u':
	case L'i':
	case L'o':
	case L'p':
	case L'a':
	//case L's':
	//case L'd':
	//case L'f':
	//case L'g':
	//case L'h':
	//case L'j':
	//case L'k':
	//case L'l':
	//case L'z':
	case L'x':
	//case L'c':
	//case L'v':
	//case L'b':
	case L'n':
	//case L'm':
	{
		int ppos = is.tellg();
		wstring op;
		is >> wistream_keyword(op);
		if(op == L"or")return &s_OpOr;
		else if(op == L"and") return &s_OpAnd;
		else if(op == L"eq") return &s_OpEqual;
		else if(op == L"ne") return &s_OpNotEqual;
		else if(op == L"is") return &s_OpIs;
		else if(op == L"cnt") return &s_OpCnt;
		else if(op == L"nct") return &s_OpNct;
		else if(op == L"not") return &s_OpNot;
		else if(op == L"pow") return &s_OpPower;
		else if(op == L"xor") return &s_OpXOr;
		else // not a operator
		{
			is.seekg(ppos,ios_base::beg);
			return 0;
		}
		is >> wsp_nl_ucmt_scip;
		break;
	}
	case L'(':{return &s_OpCall;}
	case L'[':{return &s_OpArrElem;}
	//case L'^':{is.ignore();return &s_OpPower;}
	case L',':{is.ignore();return &s_OpComma;}
	case L'+':{is.ignore();return &s_OpPlus;}
	case L'-':{is.ignore();return &s_OpMinus;}
	case L'*':{is.ignore();return &s_OpMultiply;}
	case L'/':{is.ignore();return &s_OpDivide;}
	case L':':
		  {
			  is.ignore();			
			  if(is.peek()==L':')
			  {
				  is.ignore();
				  return &s_OpConcat;
			  }
			  return &s_OpSelB;
		  }
	case L'?':
		  {
			  is.ignore();			
			  if(is.peek()==L':'){is.ignore();return &s_OpSelNotA; }
			  return &s_OpSelA;
		  }
	case L'.':
		{
			is.ignore();			
			if(is.peek()==L'.')
			{
				is.ignore();
				if(is.peek()==L'.')is.ignore();  
				return &s_OpRange;
			}
			return &s_OpSubObj;
		}
	case L'=':
		{
			is.ignore();
			if(is.peek()==L'~'){is.ignore();return &s_OpRegExp;}
			if(is.peek()==L'>'){is.ignore();return &s_OpHash;}
			if(is.peek()==L'=')is.ignore();
			return &s_OpEqual;
		}
	case L'|':
		{
			is.ignore();if(is.peek()==L'|')is.ignore();
			return &s_OpOr;
		}	
	case L'&':
		{
			is.ignore();if(is.peek()==L'&')is.ignore();
			return &s_OpAnd;
		}		
	case L'<':
		{
			is.ignore();
			if(is.peek()==L'='){is.ignore();return &s_OpLessEqual;}
			if(is.peek()==L'>'){is.ignore();return &s_OpNotEqual;}
			return &s_OpLess;
		}
	case L'>':
		{
			is.ignore();
			if(is.peek()==L'='){is.ignore();return &s_OpGreaterEqual;}
			if(is.peek()==L'<'){is.ignore();return &s_OpNotEqual;}
			return &s_OpGreater;
		}
	case L'!':
		{
			is.ignore();
			if(is.peek()==L'='){is.ignore();return &s_OpNotEqual;}
			else if(is.peek()==L'?'){is.ignore();return &s_OpSelNotA; }
			return &s_OpNot;
		}
	}
	return 0;
}


CBaseOperator* GetAssignOperator(wiparsestream &is)
{
	CBaseOperator* op = 0;
	wchar_t ps = is.peek();
	switch(ps)
	{
	case L'=':break;
	case L'+':
		{
			is.ignore();
			if(is.peek()==L'+')
			{
				is.ignore();
				op = &s_OpInc;
			}
			else op =  &s_OpPlus;
			break;
		}
	case L'-':
		{
			is.ignore();
			if(is.peek()==L'-')
			{
				is.ignore();
				op = &s_OpDec;
			}
			else op =  &s_OpMinus;
			break;
		}
	case L'*':{is.ignore();op =  &s_OpMultiply;break;}
	case L'/':{is.ignore();op =  &s_OpDivide;break;}
	case L':':
		{
			is.ignore();			
			if(is.peek()==L':')
			{
				is.ignore();
				op = &s_OpConcat;
			}
			break;
		}
	case L'|':
		{
			is.ignore();if(is.peek()==L'|')is.ignore();
			op =  &s_OpOr;
			break;
		}	
	case L'&':
		{
			is.ignore();if(is.peek()==L'&')is.ignore();
			op =  &s_OpAnd;
			break;
		}		
	}
	ps = is.peek();
	if(ps == L'=')
	{
		is.ignore();
		return op;
	}
	else
	{
		if(!op || !op->is_unary())
		{
			throw parse_exception(is,PERR_UNEXPECTED_SYMBOL,ps);
		}
	}
	return op;
}