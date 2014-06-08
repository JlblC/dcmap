#include "stdafx.h"
#include <g_text.h>
#include <strstream>
#include <string>

using namespace std;
using namespace gsys;

wstring EncodeCSVItem(wstring const& str,wchar_t delim)
{
	wstring out;
	int n = str.length();
	for(int i=0;i<n;i++)
	{
		wchar_t c = str[i];
		if(delim && c == delim)
		{
			if(delim == L' ')out += L"\\s";
			else out += wstring(L"\\")+delim;
		}
		else 
		switch(c)
		{
			case L'\\':
				out += L"\\\\";
				break;
			case L'\n':
				out += L"\\n";
				break;
			case L'\r':
				break;
			case L'\t':
				out += L"\\t";
				break;
			default:
				out += c;
		}
	}
	return out;
}


bool ReadCVSLineQueryDiv(std::wistream& is,std::vector<std::wstring>& items,
						 std::vector<wchar_t>& delims)
{
	//if(is.eof()) return false;

	items.clear();
	delims.clear();

	std::wstring val;
	while(!is.fail())
	{
		is >> wsp_scip >> gsys::wistream_keyword(val) >> wsp_scip; // read keyword field spec
		items.push_back(val);
		if(wchar_classify::newline(is.peek()))
		{
			is >> gsys::wsp_nl_scip;
			is >> wsp_scip;
			return true;
		}
		else if(!is.fail())
		{
			wchar_t chDivider = is.peek();
			if(wchar_classify::alpha(chDivider))chDivider = L' ';
			if(chDivider == L'\\')
			{
				is.ignore();
				chDivider = is.peek();
				if(chDivider == L't')chDivider = L'\t';
			}
			delims.push_back(chDivider);
			if(chDivider != L' ')is.ignore();
		}
	}
	return true;
}

std::wistream& wsp_single_nl_scip(std::wistream& is)
{
	wchar_t ps;
	if(is.fail())return is;
	int r=0;
	int n=0;
	while(!is.fail())
	{
		ps = is.peek();
		if(!wchar_classify::whitespace_newline(ps)) break;
		if(ps == L'\r')r++;
		if(ps == L'\n')n++;
		if(n>1 || r>1)break;
		is.ignore();
	}
	return is;	
}

wchar_t const* DecodeEscape(wchar_t ch)
{
	static wchar_t rt[] = {0,0};
	rt[0] = ch;
	switch(ch)
	{
	case L'r': return L"";
	case L'n': return L"\r\n";
	case L't': return L"\t";
	case L's': return L" ";
	default: return rt;
	}
}

class CSV_line_till_first_of
{
	wstring* m_pStr;
	wchar_t const* m_szNums;
public:
	CSV_line_till_first_of(wstring & str,wchar_t const* chrz)
		: m_pStr(&str),m_szNums(chrz){}

	friend inline std::wistream& operator>>(std::wistream& is,CSV_line_till_first_of & isk)
	{
		int cc = (int)wcslen(isk.m_szNums);
		wchar_t ps;
		if(is.fail())return is;
		isk.m_pStr->clear();

		while(!is.fail())
		{
			bool br=false;
			ps = is.peek();
			if(wchar_classify::newline(ps))break;

			if(ps == L'\\')
			{
				is.ignore();
				if(is.fail())break;
				ps = is.peek();
				if(ps == L'\n' || ps == L'\r')
				{
					is >>wsp_single_nl_scip;
				}
				else
				{
					(*isk.m_pStr) += DecodeEscape(ps);
					is.ignore();
				}
				continue;
			}

			for(int i=0;i<cc;i++)
			{
				if(ps == isk.m_szNums[i]){br=true;break;}
			}
			if(br)break;

			is.ignore();
			if(!is.fail()) (*isk.m_pStr) += ps;
		}

		int pos = (int)isk.m_pStr->find_last_not_of(L" \t");
		if ( pos != wstring::npos )
		{
			isk.m_pStr->erase(pos+1);
		}
		else 
		{
			isk.m_pStr->clear();
		}
		return is;	
	}
};


bool ReadCVSLine(std::wistream& is,std::vector<std::wstring>& items,std::vector<wchar_t> const& delims)
{
	if(is.eof()) return false;
	items.clear();
	std::wstring val;
	std::vector<wchar_t>::const_iterator delim = delims.begin();
	if(delim == delims.end())return false;

	while(!is.fail())
	{
		wchar_t div[] = {*delim,L'\r',L'\n',0};
		is >> wsp_scip >> CSV_line_till_first_of(val,div);
		//gsys::unquote(val);
		items.push_back(val);
		if(wchar_classify::newline(is.peek()))
		{
			is >> wsp_single_nl_scip;
			return true;
		}
		else
		{
			wchar_t ps = is.peek();
			if(*delim != L' ')is.ignore();
			if(delim+1 != delims.end())delim++;
		}
	}
	return true;
}

