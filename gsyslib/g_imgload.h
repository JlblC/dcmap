/*======================================================================================
                            Image Load Library
----------------------------------------------------------------------------------------
Provide basic funtionality for loading images of varios types
curretly supported: PNG,TIFF,TGA,BMP
( TGA & BMP are limited by 24 & 32 bit types )

example of use 

#include <g_imgload.h>
#include <iostream>

main(int argc, char *argv[])
{
	gsys::bitmap bmp;
	if(!gsys::GLoadImage(argv[0],bmp))
		std::cerr << "Error loading image \"" << filename << "\" !\n"; 
	else
	{
		// process image
	}
}
=======================================================================================*/

#pragma once
#include "g_gsys.h"
#include "g_file.h"
#include "g_array2d.h"
#include "g_color.h"
#include "g_gresult.h"

namespace gsys
{
	typedef array2d<dwcolor> bitmap;
	typedef array2d<dwcolor> bitmap32;

	enum ImageFileFormat
	{
		ImageJPEG,
		ImageTIFF,
		ImagePNG,
		ImageTGA,
		ImageBMP,
		ImagePSD,
		ImageMEI,
	};

	enum ImageOptions
	{
		ImageVerticalMirror=0x01,
		ImageAlpha=0x02,                            // Use Alpha
		ImagePremultipliedAlpha= 0x04 | ImageAlpha, // USe Premultiplied Alpha 
	};

	struct ImageInfo
	{
		int options;
		gsys_byte bpp;
		bool use_alpha;
		gsys_byte compression;
	};

#ifdef GSYS_ENABLE_TIFF
	GRESULT LoadTIFF(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery=0);
#endif
#ifdef GSYS_ENABLE_PNG
	GRESULT LoadPNG(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery=0);
#endif
#ifdef GSYS_ENABLE_JPEG
	GRESULT LoadJPEG(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery=0);
#endif
	GRESULT LoadTGA(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery=0);
	GRESULT LoadPSD(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery=0);
	GRESULT SaveTGA(gsys::gfile *file,bitmap const& clarr,ImageInfo const* inf=0);
	GRESULT LoadBMP(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery=0);

	GRESULT LoadMEI(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery=0);
	GRESULT SaveMEI(gsys::gfile *file,bitmap const& clarr,ImageInfo const* inf=0);

	GRESULT GLoadImage(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery=0);

	// wrapper
	GRESULT GLoadImage(wchar_t const* filename,bitmap & clarr,int Options,int* pOptionsQuery=0);
	GRESULT GLoadImage(gfile* file,bitmap &clarr,ImageFileFormat frmt,int Options,int* pOptionsQuery=0);
	GRESULT GLoadImage(gfile* file,wchar_t const* filename,gsys::bitmap & clarr,int Options,int* pOptionsQuery=0);
	GRESULT GLoadImage(gfile* file,char const* filename,gsys::bitmap & clarr,int Options,int* pOptionsQuery=0);

	inline GRESULT GLoadImage(std::wstring const& filename,bitmap & clarr,int Options,int* pOptionsQuery=0)
		{return gsys::GLoadImage(filename.c_str(),clarr,Options,pOptionsQuery);}

	inline GRESULT GLoadImage(gfile* file,std::string const& filename,bitmap & clarr,int Options,int* pOptionsQuery=0)
		{return gsys::GLoadImage(file,filename.c_str(),clarr,Options,pOptionsQuery);}
		
	void GTransformImage(bitmap & clarr,int current,int target,int* pOptionsQuery=0);
}