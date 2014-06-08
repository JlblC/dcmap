#include "StdAfx.h"
#include "platform.h"
#include <stdio.h> 
#include "utils.h"

#define BUFSIZE 4096

static HANDLE hChildStdinRd, hChildStdinWr, hChildStdinWrDup, 
	hChildStdoutRd, hChildStdoutWr, hChildStdoutRdDup, 
	hInputFile, hStdout; 

static bool Handles = false;

VOID ErrorExit (LPTSTR lpszMessage) 
{ 
	fprintf(stderr, "%s\n", lpszMessage); 
	ExitProcess(0); 
}


void CreateHandles()
{
	BOOL fSuccess; 

	SECURITY_ATTRIBUTES saAttr; 
	// Set the bInheritHandle flag so pipe handles are inherited. 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	// Get the handle to the current STDOUT. 
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 

	// Create a pipe for the child process's STDOUT. 
	if(!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, BUFSIZE)) 
		ErrorExit(L"Stdout pipe creation failed\n"); 

	// Create non inheritable read handle and close the inheritable read 
	// handle. 
	fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
		GetCurrentProcess(), &hChildStdoutRdDup , 0,
		FALSE,
		DUPLICATE_SAME_ACCESS);
	if( !fSuccess )
		ErrorExit(L"DuplicateHandle failed");
	CloseHandle(hChildStdoutRd);

	Handles = true;
}

static wstring* pOutputBuffer=0;

/*VOID CALLBACK ReadCompletionRoutine(dcmapDWORD dwErrorCode,
									  dcmapDWORD dwNumberOfBytesTransfered,
									  LPOVERLAPPED lpOverlapped
									  )
{
	if(pOutputBuffer)
	{
		chBuf[dwNumberOfBytesTransfered]=0;
		*pOutputBuffer += ToWide(chBuf,CP_OEMCP);
	}
}*/

bool CmdExec(wstring const& CmdLine,wstring & Result)
{
	if(!Handles)CreateHandles();

	// Now create the child process. 
	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFOW siStartInfo;
	BOOL bFuncRetn = FALSE; 

	// Set up members of the PROCESS_INFORMATION structure. 
	ZeroMemory( &piProcInfo, sizeof(piProcInfo) );

	// Set up members of the STARTUPINFO structure. 
	ZeroMemory( &siStartInfo, sizeof(siStartInfo) );
	siStartInfo.cb = sizeof(siStartInfo); 
	siStartInfo.hStdError = hChildStdoutWr;
	siStartInfo.hStdOutput = hChildStdoutWr;
	siStartInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 

	bFuncRetn = CreateProcessW(NULL, 
		(LPWSTR)CmdLine.c_str(),       // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		CREATE_NO_WINDOW,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	if (bFuncRetn == 0) 
	{
		Result.clear();
		return false;
		//ErrorExit("CreateProcess failed");
	}

	static char chBuf[BUFSIZE+1];
	while(true)
	{
		HANDLE handles[]={piProcInfo.hProcess,hChildStdoutRdDup};
		//dcmapDWORD wres = WaitForMultipleObjectsEx(2,handles,false,INFINITE,true);
		DWORD wres = WaitForSingleObject(piProcInfo.hProcess,100);
		DWORD dwRead=0;
		PeekNamedPipe(hChildStdoutRdDup,0,0,0,&dwRead,0);
		if(dwRead)
		{
			ReadFile(hChildStdoutRdDup, chBuf, BUFSIZE, &dwRead,0); 
			chBuf[dwRead]=0;
			Result += ToWide(chBuf,CP_OEMCP);	
		}
		//if(wres != WAIT_OBJECT_0)break;	
		if(wres != WAIT_TIMEOUT)break;	
	}
	CloseHandle(piProcInfo.hProcess);
	CloseHandle(piProcInfo.hThread);
	pOutputBuffer=0;
	return true;
}


BOOL CreateChildProcess() 
{ 
	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO siStartInfo;
	BOOL bFuncRetn = FALSE; 

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

	// Set up members of the STARTUPINFO structure. 

	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.hStdError = hChildStdoutWr;
	siStartInfo.hStdOutput = hChildStdoutWr;
	siStartInfo.hStdInput = hChildStdinRd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 

	bFuncRetn = CreateProcess(NULL, 
		L"child",       // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	if (bFuncRetn == 0) 
	{
		ErrorExit(L"CreateProcess failed");
		return FALSE;
	}
	else 
	{
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
		return bFuncRetn;
	}
}


