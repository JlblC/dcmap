#ifndef __INCLUDED_dcmapapi_strings_h__
#define __INCLUDED_dcmapapi_strings_h__
#pragma once
///////////////////// Chars and strings

typedef char                  dcmapCHAR;
typedef wchar_t               dcmapWCHAR;

#if DCMAP_WCHAR_T_SIZE == 2
typedef wchar_t     dcmapDCHAR;
#else
typedef dcmapINT16  dcmapDCHAR;
#endif

#define dcmapT(t)  t
#define dcmapWT(t) L##t
#define dcmapDT(t) ((dcmapDCSTR)L##t)

#ifdef DCMAP_UNICODE_SYS
typedef wchar_t          dcmapFCHAR;
typedef wchar_t          dcmapUCHAR;
#define dcmapFT(t) L##t
#define dcmapUT(t) L##t
#else
typedef char             dcmapUCHAR;
typedef char             dcmapFCHAR;
#define dcmapFT(t) t
#define dcmapUT(t) t
#endif

typedef   const dcmapDCHAR* dcmapLOCSTR;

typedef   dcmapCHAR*		dcmapSTR;   
typedef   const dcmapCHAR*	dcmapCSTR;

typedef   dcmapWCHAR*		dcmapWSTR;   // wide chars
typedef   const dcmapWCHAR* dcmapWCSTR;

typedef   dcmapDCHAR*		dcmapDSTR;   // database storage format
typedef   const dcmapDCHAR* dcmapDCSTR;

typedef   dcmapUCHAR*		dcmapUSTR;   // user interface format
typedef   const dcmapUCHAR* dcmapUCSTR;

typedef   dcmapFCHAR*		dcmapFSTR;   // filesystem format
typedef   const dcmapFCHAR* dcmapFCSTR;


#if defined(DCMAP_STD_STRING)

namespace dcmap
{
	typedef std::basic_string<dcmapFCHAR> sysstring;
	typedef std::wstring tstring;

	using std::string;
	using std::wstring;

	#if DCMAP_WCHAR_T_SIZE == 2
		typedef std::wstring  dstring;
	#else
		typedef std::basic_string<dcmapDCHAR>  dstring;
	#endif

	#ifdef DCMAP_UNICODE_SYS
		typedef std::wstring          fstring;
		typedef std::wstring          ustring;
	#else
		typedef std::string          fstring;
		typedef std::string          ustring;
	#endif
}

#endif

#endif // __INCLUDED_dcmapapi_strings_h__