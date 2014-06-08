#pragma once
#include "g_file.h"
#include <vector>
#include <stdio.h>
#include <fcntl.h>

//#include <io.h>
//#include <share.h>

namespace gsys
{

	// read's contens of text file into streang
	inline bool read_text_file(gfile &file,string & dst)
	{
		if(file)
		{
			file.set_pos(0);
			int sz = file.size();
			char* bf = new char[sz+1];
			file.read(bf,sz);
			bf[sz]=0;
			// remove /r from string
			char* pt=bf;
			while(*pt)
			{
				if(*pt != '\r')
				{
					dst += *pt;
				}
				pt++;				
			}
			delete[] bf;
		}
		return true;
	}

	bool inline FileExists(const char* fn)
	{
		FILE* fl = fopen(fn,"r");
		if(!fl)return false;
		fclose(fl);
		return true;
	}

#ifdef GSYS_DETAIL_UNICODE_OS
	bool inline FileExists(const wchar_t* fn)
	{
		FILE* fl = _wfopen(fn,L"r");
		if(!fl)return false;
		fclose(fl);
		return true;
	}
#endif // GSYS_DETAIL_UNICODE_OS

    std::string inline AddPathSlash(std::string fn)
    {
        if(fn[fn.length()-1] != '\\')
            fn += "\\"; 
        return fn;    
    }

	template<class CharT>
	std::basic_string<CharT> inline ExtractFileName(std::basic_string<CharT> const& fn)
    {
		typedef basic_char_classify<CharT> _Tr;

		int st = (int)fn.length();
		int ps=-1;
		for(int i=st;--i>0;)
		{
			if(_Tr::path_divider(fn[i])){ps=i+1;break;}
		}
		if(ps<=0)return fn;
		if(ps == st)return std::basic_string<CharT>();
		return fn.substr(ps,st-ps);
    }

	template<class CharT>
    std::basic_string<CharT> inline ExtractFilePath(std::basic_string<CharT> const& fn)
    {
		typedef basic_char_classify<CharT> _Tr;
		int st = (int)fn.length();
		if(!st)return std::basic_string<CharT>();
		int ps=-1;
		int i=st;
		for(;--i>0;)
		{
			if(_Tr::path_divider(fn[i])){ps=i+1;break;}
		}
		if(ps<=0)return std::basic_string<CharT>();
		if(ps == st)return fn;
		return fn.substr(0,ps);
    }

	template<typename StringT>
    inline StringT ExtractFileEXT(StringT fn)
    {
		int st = (int)fn.length();
		int ps=-1;
		for(int i=st;--i>0;)
		{
			if(fn[i] == basic_char_classify<typename StringT::value_type>::dot()){ps=i;break;}
		}
		if(ps<=0)return StringT();
		return fn.substr(ps,st-ps);
    }

	template<typename CharT>
	CharT const* LocateFileExt(CharT const* begin,CharT const* end=0)
	{
		typedef basic_char_classify<CharT> _Tr ;
		if(!end)end = begin+_Tr::length(begin);
		for(CharT const* pt = end;--pt>begin;)
		{
			if(_Tr::dot()==*pt)return pt;
			else if(_Tr::path_divider(*pt))break;
		}
		return end;
	}

	template<typename CharT>
	CharT const* LocateFileExt2(CharT const* begin,CharT const* end=0)
	{
		 CharT const* res = LocateFileExt(begin,end);
		 if(res == end)return end;
		 return LocateFileExt(begin,res);
	}

	template<typename CharT>
	CharT const* LocateFileName(CharT const* begin,CharT const* end=0)
	{
		typedef basic_char_classify<CharT> _Tr;

		if(!end)end = begin+_Tr::length(begin);
		for(CharT const* pt = end;--pt>begin;)
		{
			if(_Tr::path_divider(*pt))return pt+1;
		}
		return begin;
	}

   template<typename StringT>
   StringT inline StripEXT(StringT const& fn)
   {
		int st = (int)fn.length();
		int ps=-1;
		for(int i=st;--i>0;)
		{
			if(fn[i] == basic_char_classify<typename StringT::value_type>::dot()){ps=i;break;}
		}
		if(ps<=0)return fn;
		return fn.substr(0,ps);
   }

	bool inline FileExists(const std::string &fn)
    {return FileExists(fn.c_str());};

	bool inline FileExists(const std::wstring &fn)
	{return FileExists(fn.c_str());};

    bool ListDirectory(const char* filename, std::vector<std::string> &vec);
    bool __inline ListDirectory(const std::string& filename, std::vector<std::string> &vec)
        {return ListDirectory(filename.c_str(),vec);}

	bool ListDirectory(const wchar_t* filename, std::vector<std::wstring> &vec);
	bool __inline ListDirectory(const std::wstring& filename, std::vector<std::wstring> &vec)
	{return ListDirectory(filename.c_str(),vec);}


    void SetAppStartupDirAsCurrent();
	std::string GetAppStartupDir();
	std::wstring GetAppStartupDirW();

	std::string GetAppFilename();
	std::wstring GetAppFilenameW();
}