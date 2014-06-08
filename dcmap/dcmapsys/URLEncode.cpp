
#include "stdafx.h"
#include "../dcmapsyslib.h"

#include <math.h>
#include <malloc.h>
#include <string>
#include <algorithm>
#include "utils.h"


class CURLEncode
{
private:
	static string csUnsafeString;
	string decToHex(char num, int radix);
	bool isUnsafe(char compareChar);
	string convert(char val);
public:
	CURLEncode() { };
	virtual ~CURLEncode() { };
	string URLEncode(string vData);
};

class CURLDecoder {
public:
	static string decode(string str);
private:
	static int convertToDec(const char* hex);
	static void getAsDec(char* hex);
};

DCMAPSYSEXPORT dcmapCSTR DCMAPLIBAPI dcmapURLEncodeW(dcmapWCSTR cstr,int CP)
{
	if(!CP)CP=CP_UTF8;
	static string str;
	CURLEncode enc;
	str = enc.URLEncode(WideToString(cstr,CP));
	return str.c_str();
}

DCMAPSYSEXPORT dcmapCSTR DCMAPLIBAPI dcmapURLEncode(dcmapCSTR cstr)
{
	if(memcmp(cstr,"\xef\xbb\xbf",3)==0)cstr+=3;
	static string str;
	CURLEncode enc;
	str = enc.URLEncode(cstr);
	return str.c_str();
}

DCMAPSYSEXPORT dcmapCSTR DCMAPLIBAPI dcmapURLDecode(dcmapCSTR cstr)
{
	static string str;
	CURLEncode enc;
	str = CURLDecoder::decode(cstr);
	return str.c_str();
}

DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapURLDecodeW(dcmapWCSTR cstr)
{
	static wstring str;
	CURLEncode enc;
	str = ToWStr(CURLDecoder::decode(ToStr(cstr)));
	return str.c_str();
}


#include <stdlib.h>
#include <crtdbg.h>
// HEX Values array
char hexVals[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
// UNSAFE String
string CURLEncode::csUnsafeString= "\"<>%\\^[]`+$,@:;/!#?=&";

// PURPOSE OF THIS FUNCTION IS TO CONVERT A GIVEN CHAR TO URL HEX FORM
string CURLEncode::convert(char val) 
{
	string csRet;
	csRet += "%";
	csRet += decToHex(val, 16);	
	return  csRet;
}

// THIS IS A HELPER FUNCTION.
// PURPOSE OF THIS FUNCTION IS TO GENERATE A HEX REPRESENTATION OF GIVEN CHARACTER
string CURLEncode::decToHex(char num, int radix)
{	
	int temp=0;	
	string csTmp;
	int num_char;
	num_char = (int) num;

	// ISO-8859-1 
	// IF THE IF LOOP IS COMMENTED, THE CODE WILL FAIL TO GENERATE A 
	// PROPER URL ENCODE FOR THE CHARACTERS WHOSE RANGE IN 127-255(DECIMAL)
	if (num_char < 0)
		num_char = 256 + num_char;

	while (num_char >= radix)
	{
		temp = num_char % radix;
		num_char = (int)floor((double)num_char / radix);
		csTmp = hexVals[temp];
	}

	csTmp += hexVals[num_char];

	if(csTmp.length() < 2)
	{
		csTmp += '0';
	}

	string strdecToHex(csTmp);
	// Reverse the String
	//strdecToHex.MakeReverse(); 
	std::reverse_copy(csTmp.begin(),csTmp.end(),strdecToHex.begin());

	return strdecToHex;
}

// PURPOSE OF THIS FUNCTION IS TO CHECK TO SEE IF A CHAR IS URL UNSAFE.
// TRUE = UNSAFE, FALSE = SAFE
bool CURLEncode::isUnsafe(char compareChar)
{
	bool bcharfound = false;
	char tmpsafeChar;
	int m_strLen = 0;

	m_strLen = csUnsafeString.length();
	for(int ichar_pos = 0; ichar_pos < m_strLen ;ichar_pos++)
	{
		tmpsafeChar = csUnsafeString[ichar_pos]; 
		if(tmpsafeChar == compareChar)
		{ 
			bcharfound = true;
			break;
		} 
	}
	int char_ascii_value = 0;
	//char_ascii_value = __toascii(compareChar);
	char_ascii_value = (int) compareChar;

	if(bcharfound == false &&  char_ascii_value > 32 && char_ascii_value < 123)
	{
		return false;
	}
	// found no unsafe chars, return false		
	else
	{
		return true;
	}

	return true;
}
// PURPOSE OF THIS FUNCTION IS TO CONVERT A STRING 
// TO URL ENCODE FORM.
string CURLEncode::URLEncode(string pcsEncode)
{	
	int ichar_pos;
	string csEncode;
	string csEncoded;	
	int m_length;
	int ascii_value;

	csEncode = pcsEncode;
	m_length = csEncode.length();

	for(ichar_pos = 0; ichar_pos < m_length; ichar_pos++)
	{
		char ch = csEncode[ichar_pos];
		if (ch < ' ') 
		{
			ch = ch;
		}		
		if(!isUnsafe(ch))
		{
			// Safe Character				
			csEncoded += ch;
		}
		else
		{
			// get Hex Value of the Character
			csEncoded += convert(ch);
		}
	}


	return csEncoded;

}

string CURLDecoder::decode(string str) {
	int len = str.length();
	char* buff = new char[len + 1];
	strcpy(buff,str.c_str());
	string ret = "";
	for(int i=0;i<len;i++) {
		if(buff[i] == '+') {
			ret = ret + " ";
		}else if(buff[i] == '%') {
			char tmp[4];
			char hex[4];			
			hex[0] = buff[++i];
			hex[1] = buff[++i];
			hex[2] = '\0';		
			//int hex_i = atoi(hex);
			sprintf(tmp,"%c",convertToDec(hex));
			ret = ret + tmp;
		}else {
			ret = ret + buff[i];
		}
	}
	delete[] buff;
	return ret;
}

int CURLDecoder::convertToDec(const char* hex) {
	char buff[12];
	sprintf(buff,"%s",hex);
	int ret = 0;
	int len = strlen(buff);
	for(int i=0;i<len;i++) {
		char tmp[4];
		tmp[0] = buff[i];
		tmp[1] = '\0';
		getAsDec(tmp);
		int tmp_i = atoi(tmp);
		int rs = 1;
		for(int j=i;j<(len-1);j++) {
			rs *= 16;
		}
		ret += (rs * tmp_i);
	}
	return ret;
}

void CURLDecoder::getAsDec(char* hex) {
	char tmp = tolower(hex[0]);
	if(tmp == 'a') {
		strcpy(hex,"10");
	}else if(tmp == 'b') {
		strcpy(hex,"11");
	}else if(tmp == 'c') {
		strcpy(hex,"12");
	}else if(tmp == 'd') {
		strcpy(hex,"13");
	}else if(tmp == 'e') {
		strcpy(hex,"14");
	}else if(tmp == 'f') {
		strcpy(hex,"15");
	}else if(tmp == 'g') {
		strcpy(hex,"16");
	}
}