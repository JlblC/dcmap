#pragma once
#include "utils.h"

enum EParseErros
{
    PERR_SUCCESS,
    PERR_INTERNAL, 
    PERR_CUSTOM,
    PERR_INVALID_VALUE,
    PERR_BAD_QUOTATION, 
    PERR_VARABLE_NAME_EXPECTED, 
    PERR_UNEXPECTED_SYMBOL, 
    PERR_VARABLE_NOT_FOUND, 
    PERR_INVALID_PARAMS, 
    PERR_INVALID_BREAK, 
    PERR_INVALID_CONTINUE, 
	PERR_INVALID_KEYWORD, 
    PERR_INSUFFICIENT_RIGHRS,

	PERR_INVALID_MODULE,
	PERR_INVALID_PROCEDURE,
	PERR_INVALID_OBJECT,

	PERR_INVALID_CONTEXT,

	NUM_PARSE_ERRORS
};

extern dcmapDCSTR ParseErrors[NUM_PARSE_ERRORS];

class parse_exception  : public std::exception
{
    void format_msgs(wiparsestream &is,EParseErros er,dcmapWCSTR erdta);
    wstring sinfo;
    wstring swhat;
    wstring errordta;
public:
    int err,line,col;
    wstring strline;
    wstring filename;

    virtual ~parse_exception() throw() {}

	parse_exception(wiparsestream &is,EParseErros er,dcmapWCSTR erdta=0)
	{
		format_msgs(is,er,erdta);
	}	
	
    parse_exception(wiparsestream &is,EParseErros er,wstring erdta)
    {
        format_msgs(is,er,erdta.c_str());
    }
    parse_exception(wiparsestream &is,EParseErros er,wchar_t erdta)
    {
        dcmapWCHAR t[] = {erdta,0};
        format_msgs(is,er,t);
    }
    dcmapWCSTR what(){return swhat.c_str();};
    dcmapWCSTR info(){return sinfo.c_str();};
};