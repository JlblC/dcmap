#include "StdAfx.h"
#include "platform.h"
#include "../dcmapsys.h"
#include "utils.h"
#include "g_file_util.h"
#include "g_std_file.h"
#include "g_gzip_file.h"
#include "LocalStrings.h"

struct trconventry
{
    word num;
    wchar_t dta[3];
};
static trconventry tr[256];

#define TR(a,r){int ind = (L##a)&0xff; tr[ind].num=wcslen(L##r);wcsncpy((wchar_t*)tr[ind].dta,L##r,tr[ind].num);}
struct TranslitConv
{
    TranslitConv()
    {
        // initial data
        for(int i=0;i<1;i++)
        {
            tr[i].num=1;
            tr[i].dta[0]=i;
        }

        TR('é',"j");
        TR('ö',"c");
        TR('ó',"u");
        TR('ê',"k");
        TR('å',"e");
        TR('í',"n");
        TR('ã',"g");
        TR('ø',"sh");
        TR('ù',"shh");
        TR('ç',"z");
        TR('õ',"h");
        TR('ú',"#");
        TR('ô',"f");
        TR('û',"y");
        TR('â',"v");
        TR('à',"a");
        TR('ï',"p");
        TR('ð',"r");
        TR('î',"o");
        TR('ë',"l");
        TR('ä',"d");
        TR('æ',"zh");
        TR('ý',"je");
        TR('ÿ',"ja");
        TR('÷',"ch");
        TR('ñ',"s");
        TR('ì',"m");
        TR('è',"i");
        TR('ò',"t");
        TR('ü',"'");
        TR('á',"b");
        TR('þ',"ju");

        TR('É',"J");
        TR('Ö',"C");
        TR('Ó',"U");
        TR('Ê',"K");
        TR('Å',"E");
        TR('Í',"N");
        TR('Ã',"G");
        TR('Ø',"Sh");
        TR('Ù',"Shh");
        TR('Ç',"Z");
        TR('Õ',"H");
        TR('Ú',"#");
        TR('Ô',"F");
        TR('Û',"Y");
        TR('Â',"V");
        TR('À',"A");
        TR('Ï',"P");
        TR('Ð',"R");
        TR('Î',"O");
        TR('Ë',"L");
        TR('Ä',"D");
        TR('Æ',"Zh");
        TR('Ý',"Je");
        TR('ß',"Ja");
        TR('×',"Ch");
        TR('Ñ',"S");
        TR('Ì',"M");
        TR('È',"I");
        TR('Ò',"T");
        TR('Ü',"'");
        TR('Á',"B");
        TR('Þ',"Ju");
   }
}        
static tr_init;

DCMAPSYSEXPORT int DCMAPLIBAPI 
dcmapWideToTranslit(int CodePage,dcmapDCSTR src,int srclen,dcmapCHAR* buf,int blen)
{
    if(!src)return 0;
    if(srclen<0)srclen = wcslen(src)+1;
    static gsys::raw_data_buffer<wchar_t> sbuffer;

    // calculate buf size
    if(!buf)
    {
        int bufsz=0;
        for(int i=0;i<srclen;i++)
        {
            if((src[i]&0xff00) == 0x0400) 
            {
                bufsz += tr[src[i]&0xff].num;
            }
            else bufsz++;
        }
        return bufsz;
    }
    sbuffer.reset();
    sbuffer.reserve(srclen+16);
    
    for(int i=0;i<srclen;i++)
    {
        if( (src[i]&0xff00) == 0x0400) 
        {
            int ind = src[i]&0xff;
            if(tr[ind].num == 1)
            {
                sbuffer.push_back(tr[ind].dta[0]);
            }
            else
            {
                wchar_t* ptr = &sbuffer.grow(tr[ind].num);
                memcpy(ptr,tr[ind].dta,tr[ind].num*sizeof(wchar_t));
            }
        }
        else sbuffer.push_back(src[i]);
    }
    return WideCharToMultiByte(CodePage,0,sbuffer.get(),sbuffer.size(),buf,blen,0,0);
}

DCMAPSYSEXPORT dcmapHFONT DCMAPLIBAPI 
dcmapCreateFontIndirectA(CONST LOGFONTA* lplf)
{
    if(g_SysStrings.ForceCharSet())
    {
        LOGFONTA lf;
        memcpy(&lf,lplf,sizeof(LOGFONTA));
        lf.lfCharSet = g_SysStrings.ForceCharSet();
        return CreateFontIndirectA(&lf);
    }
    else
    {
        return CreateFontIndirectA(lplf);
    }
}

DCMAPSYSEXPORT int DCMAPLIBAPI 
dcmapWideCharToMultiByte( 
						 dcmapINT     CodePage,
						 dcmapDWORD    dwFlags,
						 dcmapDCSTR  lpWideCharStr,
						 dcmapINT      cchWideChar,
						 dcmapSTR   lpMultiByteStr,
						 dcmapINT      cbMultiByte,
						 dcmapCSTR   lpDefaultChar,
						 dcmapBOOL*  lpUsedDefaultChar)

{
    if(CodePage<0)
    {
        CodePage = g_SysStrings.CopePage();  
		CodePage = CP_UTF8;  

        if(g_SysStrings.Translit())
        {
            return dcmapWideToTranslit(CodePage,lpWideCharStr,cchWideChar,
                                        lpMultiByteStr,cbMultiByte);
        }
    }    
    return WideCharToMultiByte(CodePage,dwFlags,lpWideCharStr,cchWideChar, 
        lpMultiByteStr,cbMultiByte,lpDefaultChar,(LPBOOL)lpUsedDefaultChar); 
}

DCMAPSYSEXPORT int DCMAPLIBAPI 
dcmapMultiByteToWideChar(dcmapINT  CodePage,
                     dcmapDWORD    dwFlags,
                     dcmapCSTR   lpMultiByteStr,
                     dcmapINT      cbMultiByte,
                     dcmapDSTR  lpWideCharStr,
                     dcmapINT      cchWideChar)
{
    if(CodePage<0)
    {
        CodePage = g_SysStrings.CopePage();  
		CodePage = CP_UTF8;  

    }    

    return MultiByteToWideChar(CodePage,dwFlags,lpMultiByteStr,cbMultiByte,
                        lpWideCharStr,cchWideChar);
}

string WideToString(dcmapDCSTR wstr,int CodePage)
{
    if(!wstr || !wstr[0])return string();
    int wlen = wcslen(wstr);
    int len = dcmapWideCharToMultiByte(CodePage,0,wstr,wlen,0,0,0,0);
    string str;
    str.resize(len,' ');
    dcmapWideCharToMultiByte(CodePage,0,wstr,wlen,(char*)str.data(),len,0,0);
    return str;
}

string WideToString(wstring wstr,int CodePage)
{
	int wlen = wstr.length();
	int len = dcmapWideCharToMultiByte(CodePage,0,wstr.c_str(),wlen,0,0,0,0);
	string str;
	str.resize(len,' ');
	dcmapWideCharToMultiByte(CodePage,0,wstr.c_str(),wlen,(char*)str.data(),len,0,0);
	return str;
}

wstring ToWide(dcmapCSTR str,int CodePage)
{
    if(!str || !str[0])return wstring();

    int len = strlen(str);
    int wlen = dcmapMultiByteToWideChar(CodePage,0,str,len,0,0);
    wstring wstr;
    wstr.resize(wlen,L' ');
    dcmapMultiByteToWideChar(CodePage,0,str,len,(dcmapDCHAR*)wstr.data(),wlen);
    return wstr;
}

wstring ToWide(string str,int CodePage)
{
	int len = str.length();
	int wlen = dcmapMultiByteToWideChar(
		CodePage,         // code page
		0,         // character-type options
		str.c_str(), // string to map
		len,       // number of bytes in string
		0,  // wide-character buffer
		0        // size of buffer
		);
	wstring wstr;
	wstr.resize(wlen,L' ');
	dcmapMultiByteToWideChar(
		CodePage,         // code page
		0,         // character-type options
		str.c_str(), // string to map
		len,       // number of bytes in string
		(dcmapDCHAR*)wstr.data(),  // wide-character buffer
		wlen        // size of buffer
		);
	return wstr;
}

static char ReadPoint(istream& is,dcmapPOINT &pt)
{
	string tmp;
	is >> ws >> gsys::istream_numb_integer(tmp) >> ws;
	if(!gsys::lex_convert(pt.x,tmp))return false;
	char div1 = is.peek();
	if(!gsys::char_classify::alpha_ext(div1) || div1 == '_')is.ignore();
	else return false;
	is >> ws >> gsys::istream_numb_integer(tmp) >> ws;
	if(!gsys::lex_convert(pt.y,tmp))return false;
	return div1;
}

static wchar_t ReadPoint(wistream& is,dcmapPOINT &pt)
{
	wstring tmp;
	is >> ws >> gsys::wistream_numb_integer(tmp) >> ws;
	if(!gsys::lex_convert(pt.x,tmp))return false;
	wchar_t div1 = is.peek();
	if(!gsys::wchar_classify::alpha_ext(div1) || div1 == L'_')is.ignore();
	else return false;
	is >> ws >> gsys::wistream_numb_integer(tmp) >> ws;
	if(!gsys::lex_convert(pt.y,tmp))return false;
	return div1;
}


bool RawStrToCoords(dcmapCSTR str,dcmapPOINT& pt)
{
	std::istringstream is(str);
	return ReadPoint(is,pt);
}

bool StrToCoords(dcmapCSTR str,dcmapPOINT& pt)
{
	if(!str)return false;
	if(str[0]=='N')str++;

	if(RawStrToCoords(str,pt))return true;
	dcmapCSTR s = strstr(str,"planetid="); // 9 chars
	if(s)
	{
		return RawStrToCoords(s+9,pt);
	}
	return false;
}
DCMAPSYSEXPORT bool DCMAPLIBAPI dcmapStrToCoords(dcmapCSTR str,dcmapPOINT* pt)
{
	if(!pt)return false;
	return StrToCoords(str,*pt);
}

inline dcmapPOINT dcmPoint(int x,int y){dcmapPOINT pt={x,y};return pt;}

DCMAPSYSEXPORT int DCMAPLIBAPI dcmapStrToCoordArray(dcmapWCSTR str,dcmapPOINT** pt,bool bRegion)
{
	static gsys::raw_data_buffer<dcmapPOINT> buf;
	buf.reset();

	std::wistringstream is(str);

	dcmapPOINT pt1,pt2;
	if(bRegion )
	{
		wchar_t div1 = ReadPoint(is,pt1);
		wchar_t div2 = is.get();
		wchar_t div3 = ReadPoint(is,pt2);
		wchar_t div4 = is.get();
		if(!div1 || !div3)return 0;

		if(is.fail())
		{
			if(div1 == L'-' && div3 == L'-') // 
			{
				buf.push_back(dcmPoint(min(pt1.x,pt1.y),min(pt2.x,pt2.y)));
				buf.push_back(dcmPoint(max(pt1.x,pt1.y),max(pt2.x,pt2.y)));
			}
			else
			{
				buf.push_back(dcmPoint(min(pt1.x,pt2.x),min(pt1.y,pt2.y)));
				buf.push_back(dcmPoint(max(pt1.x,pt2.x),max(pt1.y,pt2.y)));
			}
		}
		else
		{
			buf.push_back(pt1);
			buf.push_back(pt2);
		}
	}

	while(!is.fail())
	{
		char div1 = ReadPoint(is,pt1);
		if(div1) buf.push_back(pt1);
		char div2 = is.get();
	}

	*pt = buf.get(); 

	return buf.size();
}

DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapCoordArrayToStr(dcmapPOINT* pt,int n)
{
	static wstring BlobStr;
	if(!n || !pt)return L"";
	BlobStr.clear();
	for(int i=0;i<n;i++)
	{
		BlobStr += gsys::to_wstr(pt[i].x)+L":"+ gsys::to_wstr(pt[i].y);
		if(i<n-1)BlobStr += L"-";
	}
	return BlobStr.c_str();
}


void DestroyDir(dcmapFCSTR pDir)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	dcmapDWORD dwError;

	sysstring fdir = sysstring(pDir)+dcmapFT("\\*.*");

	hFind = FindFirstFile(fdir.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return;
	} 
	else 
	{
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			if(!wcscmp(FindFileData.cFileName,L".")|| !wcscmp(FindFileData.cFileName,L".."))
				continue;
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				DestroyDir(FindFileData.cFileName);
			else
				DeleteFile((sysstring(pDir)+L"\\"+FindFileData.cFileName).c_str());
		}
		dwError = GetLastError();
		FindClose(hFind);
	}
	RemoveDirectory(pDir);
}

bool ListDirectory(dcmapDCSTR filename, std::vector<std::wstring> &vec)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(filename, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	do
	{
		vec.push_back(FindFileData.cFileName);    
	}
	while(FindNextFile(hFind,&FindFileData));

	FindClose(hFind);
	return true;
}

sysstring ToSys(dcmapCSTR str,int CodePage)
{
	return ToWide(str,CodePage);
}

dstring ToUStr(std::string str,int CodePage)
{
	return ToWide(str,CodePage);
}


sysstring GetAppStartupDir()
{
	return gsys::GetAppStartupDirW();
}

bool dcmapHardLinkFile(dcmapDCSTR from,dcmapDCSTR to,bool FailIfExists)
{
	if(!CreateHardLink(to,from,0))
		return CopyFile(from,to,FailIfExists);
	return true;
}


bool dcmapCopyFile(dcmapDCSTR from,dcmapDCSTR to,bool FailIfExists)
{
	return CopyFile(from,to,FailIfExists);
}

bool dcmapDeleteFile(dcmapFCSTR pDir)
{
	return DeleteFile(pDir);
}

bool dcmapRename(dcmapFCSTR pFrom,dcmapFCSTR pTo)
{
	return MoveFile(pFrom,pTo);
}

bool dcmapFileExists(dcmapFCSTR pDir)
{
	return gsys::FileExists(pDir);
}

bool dcmapMakeDir(dcmapFCSTR pDir)
{
	return _wmkdir(pDir) == 0;
}

DCMAPSYSEXPORT dcmapTIME DCMAPLIBAPI dcmapGetTime()
{
	dcmapTIME tm;
	_time64((__time64_t*)&tm); 
	return tm;
}


DCMAPSYSEXPORT bool DCMAPLIBAPI dcmapSaveText(dcmapWCSTR str,dcmapFCSTR Filename,int CodePage)
{
	if(!CodePage)CodePage = CP_UTF8;
	gsys::std_file fl(Filename,gsys::gfile::ModeCreate);
	if(!fl.valid())return L"";
	dcmapCSTR res = gsys::wide_to_str(str,CodePage);
	if(CodePage == CP_UTF8)
	{
		fl << dcmapWORD(0xBBEF);
		fl << dcmapBYTE(0xBF);
	}
	int len = strlen(res);
	fl.write(res,len);
	return true;
}

gfile* OpenFileCreateZ(dcmapFCSTR Filename,bool z)
{
	if(z)	return new gsys::gzip_file(Filename,gsys::gfile::ModeWrite);
	else 	return new gsys::std_file(Filename,gsys::gfile::ModeWrite);
}


gfile* OpenFileReadZ(dcmapFCSTR Filename)
{
	gsys::gfile* pfile(new gsys::std_file(Filename,gsys::gfile::ModeRead));

	if(!pfile->valid())
	{
		delete pfile;
		return 0;
	}
	byte head[2];
	if(pfile->read(head,2) && head[0] == 0x1f && head[1] == 0x8b) // gzip file
	{
		gsys::gfile* zfile = new gsys::gzip_file(Filename,gsys::gfile::ModeRead);
		if(zfile->valid())
		{
			delete pfile;
			pfile = zfile;
		}
		else
		{
			delete zfile;
		}
	}
	else
	{
		pfile->set_pos(0);
	}
	return pfile;
}

DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapLoadText(dcmapFCSTR Filename,int CodePage)
{
	static gsys::gstring StrBuffer;
	static gsys::wgstring StrResult;

	boost::scoped_ptr<gfile> pfile(OpenFileReadZ(Filename));

	if(!pfile)return L"";

	gsys::gfile::size_type  len = pfile->size();
	dcmapSTR buf = StrBuffer.update_buffer(len);
	pfile->read(buf,len);
	pfile.reset();

	// check for head bytes
	if( *(dcmapWORD*)buf == 0xBBEF && (dcmapBYTE)buf[2] == 0xBF ) // Support UTF8 BOM  EF BB BF
	{
		buf+=3; CodePage = CP_UTF8;
		len-=3;
	}
	if(!CodePage)CodePage = CP_UTF8;
	
	int wlen = dcmapMultiByteToWideChar(CodePage,0,buf,len,0,0);
	dcmapWSTR rbuf = StrResult.update_buffer(wlen+1);
	dcmapMultiByteToWideChar(CodePage,0,buf,len,rbuf,wlen);
	rbuf[wlen]=0;
	return rbuf;
}

DCMAPSYSEXPORT dcmapCSTR DCMAPLIBAPI  dcmapWideToStr(dcmapWCSTR str,int CP)
{
	return gsys::wide_to_str(str,CP);
}
DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapStrToWide(dcmapCSTR str,int CP)
{
	return gsys::str_to_wide(str,CP);
}


/*
class RegExpRecordImporttt
{
	IDCMapDataStorage* m_pData;
	typedef vector<IDCMapDataStorage*> VecRelations;
	VecRelations TableRel;

	typedef vector<int> VecIndexes;
	VecIndexes keyind;
	typedef map<string,wstring> MapVars;
	MapVars m_vars;
	typedef pair<int,wstring> PInf;
	typedef vector<PInf> VecInfo;
	VecInfo m_vecInfo;
public:
	bool DCMAPAPI Init(IDCMapCore* pCore,IDCMapDataStorage* pData)
	{
		m_pData = pData;
		if(!m_pData)return false;

		// Init indexes
		int n = m_pData->NumColumns();

		TableRel.clear();
		TableRel.resize(n,0);

		keyind.clear();
		for(int i=0;i<n;i++)
		{
			if(m_pData->ColumnParams(i) & DCMAP_COLUMN_INDEX)
				keyind.push_back(i);
			if(m_pData->ColumnDataType(i) == DCMDT_Reference)
			{
				TableRel[i] = pCore->GetDataSourceTyped<IDCMapDataStorage>(m_pData->ColumnDataLinkage(i));
			}
		}
		return true;
	}
	bool Parse(dcmapDCSTR strData,dcmapDCSTR strRegExp,dcmapCSTR strFormat)
	{
		boost::wsmatch r;
		if(!boost::regex_match((wstring)strData,r,boost::wregex((wstring)strRegExp)))return false;
		m_vecInfo.clear();

		// parse format string & form input structure
		std::istrstream is(strFormat);
		// Read x
		while(!is.fail())
		{
			string key,val;
			is >> gsys::istream_keyword(key)>>ws;
			int id = m_pData->ColumnID(val.c_str());
			if(id<0)return false;

			if(is.peek() != '=')return false;
			is.ignore();
			if(is.peek() == '$') // some variable
			{
				is.ignore();
				bool re = gsys::char_is_numb(is.peek());
				is >> gsys::istream_numb_keyword(val)>>ws;
				int n=0;
				if(re && gsys::lex_convert(val,n))
				{
					if(r.size()>n)return false;
					m_vecInfo.push_back(PInf(id,r[n]));
				}
				else
				{
					MapVars::iterator it = m_vars.find(val);
					if(it == m_vars.end())return false;
					m_vecInfo.push_back(PInf(id,it->second));
				}
			}
			else if(is.peek() == '\'' || is.peek() == '\"')
			{
				is >> gsys::istream_quoted(val);
				m_vecInfo.push_back(PInf(id,ToWide(val)));
			}
			is >> ws;
			if(is.peek() != ',')break;
			is.ignore();
		}
		return true;		
	}
};
*/
