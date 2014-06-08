#ifndef __INCLUDED_dcmapapi_basictypes_h__
#define __INCLUDED_dcmapapi_basictypes_h__
#pragma once

//////////////////  base fixed size numeric
typedef int              dcmapINT;
typedef unsigned int     dcmapUINT;

typedef char				 dcmapINT8;
typedef unsigned char		 dcmapUINT8;
typedef short				 dcmapINT16;
typedef unsigned short       dcmapUINT16;
typedef long				  dcmapINT32;
typedef unsigned int         dcmapUINT32;
typedef DCMAP_INT64		     dcmapINT64;
typedef unsigned DCMAP_INT64 dcmapUINT64;
///////////////////
typedef unsigned char    dcmapBYTE;
typedef unsigned short   dcmapWORD;
typedef unsigned long    dcmapDWORD;
typedef dcmapUINT64      dcmapQWORD;
///////////////////
typedef dcmapINT64 dcmapLLONG;
typedef dcmapINT64 dcmapLONGLONG;


typedef unsigned long DCMAP_W64   dcmapDWORD_PTR;
typedef int           DCMAP_W64   dcmapINT_PTR;
typedef unsigned int  DCMAP_W64   dcmapUINT_PTR;
typedef long          DCMAP_W64   dcmapLONG_PTR;
typedef unsigned long DCMAP_W64   dcmapULONG_PTR;

typedef int  dcmapBOOL;


typedef long             dcmapLONG;
typedef unsigned long    dcmapULONG;
typedef short            dcmapSHORT;

typedef dcmapDWORD       dcmapLPARAM; 
typedef dcmapDWORD       dcmapWPARAM;
typedef int              dcmapLRESULT; 

typedef dcmapQWORD       dcmapTIME;

typedef dcmapINT         dcmapMSG;

typedef double           dcmapDOUBLE;
typedef float            dcmapFLOAT;
typedef double           dcmapREAL;

typedef dcmapDOUBLE      dcmapREAL;

typedef dcmapDWORD       dcmapCOLORREF;
typedef dcmapDWORD    dcmapFOURCC;
typedef dcmapCOLORREF    dcmapCOLOR;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct dcmapCOORD
{
	union
	{
		struct  
		{
			dcmapWORD x;
			dcmapWORD y;
		};
		dcmapDWORD data;
	};
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define DCMapInvalidReferenceId  0xffffffff
#define DCMapInvalidReferenceAge 0
#define DCMapMaxReferenceId (1 << 30)

// data reference type
struct dcmapDBREF
{
	union
	{
		struct  
		{
			dcmapDWORD id;
			dcmapDWORD age;
		};
		dcmapQWORD data;
	};
	dcmapDBREF(dcmapINT64 dta):data(dta){}
	dcmapDBREF(dcmapQWORD dta):data(dta){}
	dcmapDBREF(dcmapDWORD _id,dcmapDWORD _age):id(_id),age(_age){}
	dcmapDBREF():id(DCMapInvalidReferenceId),age(DCMapInvalidReferenceAge){}

	void Invalidate()
	{
		id = DCMapInvalidReferenceId;
		age = DCMapInvalidReferenceAge;
	}

	bool ValidID()const
	{
		return id < DCMapMaxReferenceId;
	}
	bool ValidAge()const
	{
		return age != DCMapInvalidReferenceAge;
	}

	bool Valid()const
	{
		return id < DCMapMaxReferenceId && age != DCMapInvalidReferenceAge;
	}

	friend bool operator < (dcmapDBREF const& r1,dcmapDBREF const& r2) 
	{
		return r1.data < r2.data;
	}
};
typedef dcmapDBREF DCMapDataReference;
typedef dcmapDBREF DataReference;


inline bool operator == (dcmapDBREF const& r1,dcmapDBREF const& r2)
{return r1.id == r2.id && r1.age == r2.age;}
inline bool operator != (dcmapDBREF const& r1,dcmapDBREF const& r2){return !(r1 == r2);}


#ifdef DCMAP_WINAPI

typedef RECT dcmapRECT; 
typedef POINT dcmapPOINT; 
typedef HWND dcmapHWND;
typedef HDC dcmapHDC;
typedef HINSTANCE dcmapHINSTANCE;
typedef HBITMAP dcmapHBITMAP;
typedef HPEN dcmapHPEN;
typedef HBRUSH dcmapHBRUSH;
typedef HFONT dcmapHFONT;
typedef HRGN dcmapHRGN;

#else

typedef void* dcmapHRGN;
typedef void* dcmapHWND;
typedef void* dcmapHDC;
typedef void* dcmapHBITMAP;
typedef void* dcmapHPEN;
typedef void* dcmapHBRUSH;
typedef void* dcmapHFONT;
typedef void* dcmapHINSTANCE;

struct dcmapRECT
{
	dcmapLONG left;
	dcmapLONG top;
	dcmapLONG right;
	dcmapLONG bottom;
};

struct dcmapPOINT
{
	dcmapLONG x;
	dcmapLONG y;
};

#endif

#define dcmapMAKEWORD(a, b)      ((dcmapWORD)(((dcmapBYTE)(((dcmapDWORD_PTR)(a)) & 0xff)) | ((dcmapWORD)((dcmapBYTE)(((dcmapDWORD_PTR)(b)) & 0xff))) << 8))
#define dcmapMAKELONG(a, b)      ((dcmapLONG)(((dcmapWORD)(((dcmapDWORD_PTR)(a)) & 0xffff)) | ((dcmapDWORD)((dcmapWORD)(((dcmapDWORD_PTR)(b)) & 0xffff))) << 16))

#define dcmapLOWORD(l)           ((dcmapWORD)(((dcmapDWORD_PTR)(l)) & 0xffff))
#define dcmapHIWORD(l)           ((dcmapWORD)((((dcmapDWORD_PTR)(l)) >> 16) & 0xffff))
#define dcmapLOBYTE(w)           ((dcmapBYTE)(((dcmapDWORD_PTR)(w)) & 0xff))
#define dcmapHIBYTE(w)           ((dcmapBYTE)((((dcmapDWORD_PTR)(w)) >> 8) & 0xff))

#define dcmapRGB(r,g,b) ((dcmapCOLORREF)(((dcmapBYTE)(r)|((dcmapWORD)((dcmapBYTE)(g))<<8))|(((dcmapDWORD)(dcmapBYTE)(b))<<16)))
#define dcmapGetRValue(rgb)      (dcmapLOBYTE(rgb))
#define dcmapGetGValue(rgb)      (dcmapLOBYTE(((dcmapWORD)(rgb)) >> 8))
#define dcmapGetBValue(rgb)      (dcmapLOBYTE((rgb)>>16))


#endif // __INCLUDED_dcmapapi_basictypes_h__

