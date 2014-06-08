#include "common.h"
#include "g_platform.h"
#include "g_file_util.h"

void gsys::SetAppStartupDirAsCurrent()
{
    char buf[MAX_PATH];
    GetModuleFileName(0,buf,MAX_PATH);
	size_t sz = strlen(buf);
	for(int i=sz;--i >= 0;)
	{
		if(buf[i] != '\\')
			buf[i]=0;
		else break;
	}
    SetCurrentDirectory(buf);
}

std::string gsys::GetAppFilename()
{
	char buf[MAX_PATH];
	GetModuleFileNameA(0,buf,MAX_PATH);
	return (std::string)(buf);
}

std::wstring gsys::GetAppFilenameW()
{
	wchar_t buf[MAX_PATH];
	GetModuleFileNameW(0,buf,MAX_PATH);
	return (std::wstring)(buf);
}

std::string gsys::GetAppStartupDir()
{
	char buf[MAX_PATH];
	GetModuleFileNameA(0,buf,MAX_PATH);
	size_t sz = strlen(buf);
	for(int i=sz;--i >= 0;)
	{
		if(buf[i] != '\\')
			buf[i]=0;
		else break;
	}
	return (std::string)(buf);
}

std::wstring gsys::GetAppStartupDirW()
{
	wchar_t buf[MAX_PATH];
	GetModuleFileNameW(0,buf,MAX_PATH);
	size_t sz = wcslen(buf);
	for(int i=sz;--i >= 0;)
	{
		if(buf[i] != L'\\')
			buf[i]=0;
		else break;
	}
	return (std::wstring)(buf);
}

bool gsys::ListDirectory(const char* filename, vector<string> &vec)
{
  WIN32_FIND_DATAA FindFileData;
  HANDLE hFind;

  hFind = FindFirstFileA(filename, &FindFileData);

  if (hFind == INVALID_HANDLE_VALUE)
      return false;
  do
  {
        vec.push_back(FindFileData.cFileName);    
  }
  while(FindNextFileA(hFind,&FindFileData));

  FindClose(hFind);
  return true;
}

bool gsys::ListDirectory(const wchar_t* filename, std::vector<std::wstring> &vec)
{
	WIN32_FIND_DATAW FindFileData;
	HANDLE hFind;

	hFind = FindFirstFileW(filename, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	do
	{
		vec.push_back(FindFileData.cFileName);    
	}
	while(FindNextFileW(hFind,&FindFileData));

	FindClose(hFind);
	return true;}
