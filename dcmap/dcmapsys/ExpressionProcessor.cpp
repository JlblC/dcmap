#include "StdAfx.h"
#include "../dcmapsyslib.h"
#include <g_text.h>
#include <g_gvector.h>
#include <ios>

using namespace std;
using namespace gsys;

enum DCMapExpVarType
{
	DCMapExpVar_Void,DCMapExpVar_Null,DCMapExpVar_Number,DCMapExpVar_String,DCMapExpVar_TableField,DCMapExpVar_Identifier,DCMapExpVar_Operator,DCMapExpVar_TableRef
};

struct IDCMapExpession
{
	
};

struct IDCMapExpessionOperator
{
	
};

struct IDCMapExpessionObject
{
	
};

class CDCMapExpressionContext
{

};

enum OPType
{
	OP_ERROR,OP_SYMB,OP_NUMBER,OP_STRING,
	OP_BRACE_OPEN,OP_BRACE_CLOSE,
	OP_PLUS,OP_MINUS,
	OP_MULTIPLY,OP_DIVIDE,
	OP_FUNCTIONAL,
	OP_FUNCTION,
	OP_END
};

struct CDCMapExpession : public IDCMapExpession
{
	DCMapExpVarType type;
	double num;
	string str;
	IDCMapExpessionObject* obj;
	// operators reference
	IDCMapExpessionOperator* pOperator;
	OPType optype;
	vector<CDCMapExpession> Params;
};



int op_level[256];

OPType ReadOpType(istream &is)
{
	is >> gsys::wsp_nl_scip;
	char ch = is.peek();
	if(gsys::char_is_alpha(ch))return OP_SYMB;
	if(gsys::char_is_numb(ch))return OP_NUMBER;
	if(ch == '"' || ch == '\'')return OP_STRING;

	if(ch == '+'){is.ignore();return OP_PLUS;}
	if(ch == '*'){is.ignore();return OP_MULTIPLY;}

	return OP_ERROR;
}

OPType read_token(istream &is,CDCMapExpession& expr)
{
	expr.type = DCMapExpVar_Void;
	OPType type = ReadOpType(is);
	if(type == OP_STRING)
	{
		is >> gsys::istream_quoted(expr.str);
		expr.type = DCMapExpVar_String;
	}
	else if(type == OP_NUMBER)
	{
		is >> gsys::istream_numeric<double>(expr.num);
		expr.type = DCMapExpVar_Number;
	}
	else if(type == OP_SYMB)
	{
		is >> gsys::istream_keyword(expr.str);
		expr.type = DCMapExpVar_Identifier;
	}
	return type;
}

OPType parse_exp(istream &is,CDCMapExpession& expr,int lev);

OPType read_blockexp(istream &is,CDCMapExpession& expr)
{
	OPType type = read_token(is,expr);
	if(type == OP_BRACE_OPEN) // we read set of operators
	{
		type = parse_exp(is,expr,0); 
	}
	return type;
}

OPType parse_exp(istream &is,CDCMapExpession& expr,int lev)
{
	CDCMapExpession current;
	// read first variable
	OPType type = read_blockexp(is,current); 
	if(current.type != DCMapExpVar_Void)//we read value
	{
		CDCMapExpession currentop;
		type = read_token(is,currentop);
		if(current.type == DCMapExpVar_Void) // we read operator
		{
			OPType nextop = type;
			while(1)
			{
				if(op_level[nextop]<lev)
				{
					expr = current;
					return type;
				}
				else
				{
					CDCMapExpession opexp;

					opexp.optype = nextop;
					opexp.Params.push_back(current);
					nextop = parse_exp(is,current,op_level[type]);
					opexp.Params.push_back(current);
					current = opexp;
				}
			}
		}
	}
}

bool ParceExp(const char* Str)
{
	istringstream is(Str);
	CDCMapExpession exp;

	parse_exp(is,exp,0);

	return true;
}

void InitExprtTest()
{
	op_level[OP_PLUS] = 5;
	op_level[OP_MULTIPLY] = 6;
	ParceExp("1+2*3+4");
}

struct TestEx
{
	TestEx(){InitExprtTest();}
}static c_Te;

