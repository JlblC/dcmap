#include "StdAfx.h"
#include "Parser.h"

dcmapDCSTR ParseErrors[NUM_PARSE_ERRORS];
dcmapDCSTR ParseErrorsParam[NUM_PARSE_ERRORS];

struct s{s()
{
    for(int i=0;i<NUM_PARSE_ERRORS;i++)
    {
        ParseErrors[i]=0;
        ParseErrorsParam[i]=0;
    }

    ParseErrors[PERR_SUCCESS]               = L"success";
    ParseErrors[PERR_INVALID_VALUE]         = L"invalid value";
    ParseErrors[PERR_BAD_QUOTATION]         = L"invalid quotation";
    ParseErrors[PERR_VARABLE_NAME_EXPECTED] = L"variable name expected";
    ParseErrors[PERR_INVALID_BREAK]         = L"invalid break";
    ParseErrors[PERR_INVALID_CONTINUE]      = L"invalid continue";
    ParseErrors[PERR_INTERNAL]              = L"internal error";
    ParseErrors[PERR_INVALID_PARAMS]        = L"invalid params";
    ParseErrors[PERR_UNEXPECTED_SYMBOL]     = L"unexpected symbol";
    ParseErrors[PERR_INSUFFICIENT_RIGHRS]   = L"insufficient rights to";
	ParseErrors[PERR_INVALID_KEYWORD]        = L"invalid keyword";

    ParseErrorsParam[PERR_CUSTOM]                = L"%s";
    ParseErrorsParam[PERR_UNEXPECTED_SYMBOL]     = L"unexpected symbol: '%s'";
    ParseErrorsParam[PERR_INSUFFICIENT_RIGHRS]   = L"insufficient rights to %s";
	ParseErrorsParam[PERR_INVALID_KEYWORD]       = L"invalid keyword: '%s'";


	ParseErrors[PERR_INVALID_MODULE]        = L"invalid module";
	ParseErrors[PERR_INVALID_PROCEDURE]        = L"invalid procedure";
	ParseErrors[PERR_INVALID_OBJECT]        = L"invalid object";

	ParseErrors[PERR_INVALID_CONTEXT]        = L"invalid operation context";

}}static ss;


void parse_exception::format_msgs(wiparsestream &is,EParseErros er,dcmapDCSTR errdta)
{
    err = er;
    line=0;col=0;
    is.get_pos_info(&line,&col,&strline);
    filename = is.filename();
    sinfo = filename+L" line:"+gsys::to_wstr(line)+L" col:"+gsys::to_wstr(col)+L" ";
    if(ParseErrors[err] && ParseErrors[err][0])
    {
        if(errdta && errdta[0])
        {
            errordta = errdta;
            swhat = L"error: " + boost::str(wformat(ParseErrorsParam[err]) % errordta);
        }
        else 
        {
            swhat = L"error: " + (wstring)ParseErrors[err];
        }
    }
    else
    {
        swhat = L"unknown parse error";
    }
    sinfo += swhat;
}