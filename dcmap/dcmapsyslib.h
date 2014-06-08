
#ifndef DCMAPSYSLIB_H
#define DCMAPSYSLIB_H

#include "dcmapbase.h"

struct IDCMapSysEx;

extern "C" // Dll export functions
{
	DCMAPSYSEXPORT IDCMapSysEx* DCMAPLIBAPI GetDCMapSys();

	DCMAPSYSEXPORT IDCMapCoreEx* DCMAPLIBAPI GetDCMapCore();
	DCMAPSYSEXPORT IDCMapWorkspaceEx* DCMAPLIBAPI CreateDCMapWorkspace();
	DCMAPSYSEXPORT void DCMAPLIBAPI DestroyDCMapWorkspace(IDCMapWorkspaceEx* pWorkspace);

	struct tagLOGFONTA;
    DCMAPSYSEXPORT dcmapHFONT DCMAPLIBAPI dcmapCreateFontIndirectA(const tagLOGFONTA* lplf);

    DCMAPSYSEXPORT int DCMAPLIBAPI 
        dcmapWideCharToMultiByte( 
            dcmapINT     CodePage,
            dcmapDWORD    dwFlags,
            dcmapDCSTR  lpWideCharStr,
            dcmapINT      cchWideChar,
            dcmapSTR   lpMultiByteStr,
            dcmapINT      cbMultiByte,
            dcmapCSTR   lpDefaultChar,
            dcmapBOOL*  lpUsedDefaultChar);

    DCMAPSYSEXPORT int DCMAPLIBAPI 
        dcmapMultiByteToWideChar(
            dcmapINT  CodePage,
            dcmapDWORD  dwFlags,
            dcmapCSTR   lpMultiByteStr,
            dcmapINT    cbMultiByte,
            dcmapDSTR  lpWideCharStr,
            dcmapINT    cchWideChar);
};

#endif








