#pragma once
#include "Parser.h"

enum EParserOperators
{
	OP_NONE=0,
	OP_ASSIGN,
	OP_PLUS,
	OP_MINUS,
	OP_INC,
	OP_DEC,
	NUM_OPERATORS
};

CBaseOperator* GetParserOperator(EParserOperators id);
CBaseOperator* GetAssignOperator(wiparsestream &is);