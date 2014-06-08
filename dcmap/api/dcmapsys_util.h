
#ifndef DCMAPSYS_UTIL_H
#define DCMAPSYS_UTIL_H

#include "dcmapsys.h"

extern "C" // Dll export functions
{
	DCMAPSYSEXPORT dcmapTIME DCMAPLIBAPI dcmapGetTime();

	DCMAPSYSEXPORT dcmapCSTR DCMAPLIBAPI  dcmapWideToStr(dcmapWCSTR str,int CP=0);
	DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapStrToWide(dcmapCSTR str,int CP=0);

	DCMAPSYSEXPORT dcmapCSTR DCMAPLIBAPI  dcmapURLEncode(dcmapCSTR cstr);
	DCMAPSYSEXPORT dcmapCSTR DCMAPLIBAPI  dcmapURLEncodeW(dcmapDCSTR cstr,int CP=0);
	DCMAPSYSEXPORT dcmapCSTR DCMAPLIBAPI  dcmapURLDecode(dcmapCSTR cstr);
	DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapURLDecodeW(dcmapWCSTR cstr);

	DCMAPSYSEXPORT bool DCMAPLIBAPI dcmapStrToCoords(dcmapCSTR str,dcmapPOINT* pt);

	DCMAPSYSEXPORT int DCMAPLIBAPI dcmapStrToCoordArray(dcmapWCSTR str,dcmapPOINT** pt,bool bRegion=false);
	DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapCoordArrayToStr(dcmapPOINT* pt,int n);

	DCMAPSYSEXPORT int DCMAPLIBAPI dcmapWideToTranslit(int CodePage,dcmapDCSTR src,int srclen,dcmapCHAR* buf,int blen);

	DCMAPSYSEXPORT bool DCMAPLIBAPI dcmapSaveText(dcmapWCSTR str,dcmapFCSTR Filename,int cp=0);
	DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapLoadText(dcmapFCSTR Filename,int cp=0);

	DCMAPSYSEXPORT IDCMapVarValue* DCMAPLIBAPI dcmapCreateVarValue();

	DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapProcessFormatedString(dcmapWCSTR Format,IDCMapVarValue* Values,int Flags=0);
	DCMAPSYSEXPORT dcmapWCSTR DCMAPLIBAPI dcmapProcessFormatedStringDB(dcmapWCSTR Format,IDCMapDataSource* pDataStorage,dcmapDBREF const* pDBRef,IDCMapVarValue* Values=0,int Flags=0);

};

#endif

