#include "common.h"

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>

#include "../g_hash_map.h"
#include "../g_alg.h"
#include "../g_imgload.h"
#include "../g_file_util.h"
#include "../g_std_file.h"

#ifdef GSYS_ENABLE_PNG
#	include "./libs/png.h"
#	ifdef GSYS_DETAIL_LIB_OMF
#		pragma comment(lib ,"libpng_omf.lib")
#	else
#		pragma comment(lib ,"libpng.lib")
#	endif
#endif

#ifdef GSYS_ENABLE_TIFF
#	include "./libs/tiffio.h"
#	ifdef GSYS_DETAIL_LIB_OMF
#		pragma comment(lib ,"libtiff_omf.lib")
#	else 
#		pragma comment(lib ,"libtiff.lib")	
#	endif
#endif


using namespace gsys;
using namespace boost;

namespace gsys{

static const int max_image_size = 1024*8;

#ifdef GSYS_ENABLE_TIFF

// set of dummy functions to implement TIFF loading from gfile object
/////////////////////////////////////////////////////////////////////////////
static tsize_t _tiff_read(thandle_t handle, tdata_t data, tsize_t size)
{
	gfile* file = reinterpret_cast<gfile*>(handle);
	ulong pos = file->pos();
	ulong rd =  file->read(data,1,size);
	file->set_pos(pos);
	return rd;
}
static toff_t	_tiff_seek(thandle_t handle, toff_t offset, int pos)
{
	gfile* file = reinterpret_cast<gfile*>(handle);
	return file->seek(offset,(gfile::FileSeek)pos);
}
static int	_tiff_close(thandle_t)
{
	return 1;
}
static toff_t  _tiff_size(thandle_t handle)
{
	gfile* file = reinterpret_cast<gfile*>(handle);
	return file->size();
}
static int	_tiff_map(thandle_t handle, tdata_t* data, toff_t* offset)
{
	gfile* file = reinterpret_cast<gfile*>(handle);
	ulong pos = file->pos();
	int sz = file->size();
	char* dta = (char*)galloc(sz);
	file->tread(dta,sz);
	file->set_pos(pos);
	*data = dta;
	*offset = sz;
	return 1;
}
static void	_tiff_unmap(thandle_t, tdata_t data, toff_t)
{
	gfree(data);
}
//////////////////////////////////////////////////////////////////////////

GRESULT gsys::LoadTIFF(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery)
{
	TIFF* tif = TIFFClientOpen("","r",reinterpret_cast<thandle_t>(file),
		_tiff_read,
		_tiff_read,
		_tiff_seek,
		_tiff_close,
		_tiff_size,
		_tiff_map,
		_tiff_unmap);
	
	if(!tif) return GERR_INVALID_DATA;

	uint32 w, h;
	size_t npixels;
	uint32* raster;
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
	npixels = w * h;
	raster = (uint32*) _TIFFmalloc((tsize_t)(npixels * sizeof (uint32)));
	if (raster != NULL)
	{
	    if (TIFFReadRGBAImage(tif, w, h, raster, 0))
		{
			//gsys::array2d<gsys::dwcolor> clarr(w,h);
			clarr.init(w,h);
			for(uint i=0;i<npixels;i++)
			{
				dwcolor cl;
				uint32 ucl = raster[i];
				clarr(i) = dwcolor( (gbyte)TIFFGetR(ucl),
									(gbyte)TIFFGetG(ucl),
									(gbyte)TIFFGetB(ucl),
									(gbyte)TIFFGetA(ucl));
			}
	    }
	    _TIFFfree(raster);
	}
	TIFFClose(tif);

	GTransformImage(clarr,ImagePremultipliedAlpha,Options,pOptionsQuery);
	return G_OK;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // GSYS_ENABLE_TIFF


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef GSYS_ENABLE_PNG

static void png_read_data(png_structp png_ptr,png_bytep data, png_size_t length)
{
	gsys::gfile* file = reinterpret_cast<gsys::gfile*>(png_get_io_ptr(png_ptr));
	if(!file)return;
	file->read(data,(gfile::size_type)length);	
}


GRESULT gsys::LoadPNG(gsys::gfile *file,gsys::array2d<gsys::dwcolor> & clarr,int Options,int* pOptionsQuery)
{
	png_byte header[8];
	
	file->read(header,8);
	if (png_sig_cmp(header, 0, sizeof(header)))return GERR_INVALID_DATA;

	//png_set_sig_bytes_read();
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, gsys_null,gsys_null, gsys_null);
	if (!png_ptr)return G_FAIL;

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr,gsys_null,gsys_null);
		return G_FAIL;
	}

	png_set_sig_bytes(png_ptr,sizeof(header));

	png_set_read_fn(png_ptr,reinterpret_cast<void*>(file), png_read_data);


	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16|
									PNG_TRANSFORM_PACKING|
									PNG_TRANSFORM_PACKSWAP|
									PNG_TRANSFORM_BGR, NULL);

	int width = png_get_image_width(png_ptr,info_ptr);
	int height = png_get_image_height(png_ptr,info_ptr);
	int channels = png_get_channels(png_ptr,info_ptr);
	int color_type = png_get_color_type(png_ptr,info_ptr);
	int bit_debth = png_get_bit_depth(png_ptr,info_ptr);
	int pixel_bytes = bit_debth*channels/8;

	png_colorp palette=gsys_null;
	int num_palette=0;
	png_bytep trans=0;
	int num_trans=0;
	png_color_16p trans_values=0;

	//info_ptr->ch
	clarr.init(width,height);
	
	png_get_PLTE(png_ptr, info_ptr, &palette,&num_palette);
	png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans,&trans_values);

	png_bytepp rows = png_get_rows(png_ptr, info_ptr);
	for(int i=0;i<height;i++)
	{
		png_bytep row = rows[height-1-i];
		for(int j=0;j<width;j++)
		//for(int j=width;--j>0;)
		{
			dwcolor  & cl = *reinterpret_cast<dwcolor*>(row);
			switch(color_type)
			{
			case PNG_COLOR_TYPE_GRAY:
				clarr(j,i).r = cl.r;
				clarr(j,i).g = cl.r;
				clarr(j,i).b = cl.r;
				clarr(j,i).a = 255;
				break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				clarr(j,i).r = cl.r;
				clarr(j,i).g = cl.r;
				clarr(j,i).b = cl.r;
				clarr(j,i).a = cl.g;
				break;
			case PNG_COLOR_TYPE_RGB:
				clarr(j,i).r = cl.r;
				clarr(j,i).g = cl.g;
				clarr(j,i).b = cl.b;
				clarr(j,i).a = 255;
				break;
			case PNG_COLOR_TYPE_RGB_ALPHA:
				clarr(j,i).r = cl.r;
				clarr(j,i).g = cl.g;
				clarr(j,i).b = cl.b;
				clarr(j,i).a = cl.a;
				break;
			case PNG_COLOR_TYPE_PALETTE:
				clarr(j,i).r = palette[cl.r].red;
				clarr(j,i).g = palette[cl.r].green;
				clarr(j,i).b = palette[cl.r].blue;
				clarr(j,i).a = 255;
				if(num_trans == num_palette)clarr(j,i).a = trans[cl.r];
				else clarr(j,i).a = 255;
				break;
			}
			row += pixel_bytes;
		}
	}
	png_destroy_info_struct(png_ptr,&info_ptr);
	png_destroy_read_struct(&png_ptr,gsys_null,gsys_null);

	GTransformImage(clarr,ImageAlpha,Options,pOptionsQuery);
	return G_OK;
}

#endif //GSYS_ENABLE_PNG

#pragma pack (push,1)

struct BMP_FILEHEADER { 
  word    bfType; 
  dword   bfSize; 
  word    bfReserved1; 
  word    bfReserved2; 
  dword   bfOffBits; 
}; 

struct BMP_INFOHEADER{
  dword  biSize; 
  LONG   biWidth; 
  LONG   biHeight; 
  WORD   biPlanes; 
  WORD   biBitCount; 
  dword  biCompression; 
  dword  biSizeImage; 
  LONG   biXPelsPerMeter; 
  LONG   biYPelsPerMeter; 
  dword  biClrUsed; 
  dword  biClrImportant; 
}; 

#pragma pack (pop)


GRESULT gsys::LoadBMP(gsys::gfile *file,gsys::array2d<gsys::dwcolor> & clarr,int Options,int* pOptionsQuery)
{
	BMP_FILEHEADER fh;
	BMP_INFOHEADER ih;

	*file >> fh;
	if(fh.bfType != 0x4d42)return GERR_INVALID_DATA;
	if(fh.bfOffBits < (sizeof(fh) + sizeof(ih)))
		return GERR_INVALID_DATA;

	*file >> ih;
	// test sanity of structure
	if(ih.biSize < sizeof(BMP_INFOHEADER))return GERR_INVALID_DATA;
	if(ih.biWidth > max_image_size)return GERR_INVALID_DATA;
	if(ih.biHeight > max_image_size)return GERR_INVALID_DATA;
	// test if format supported
	if(ih.biPlanes != 1)return GERR_INVALID_DATA;
	if(ih.biCompression != 0)return GERR_INVALID_DATA; // no compression

	clarr.init(ih.biWidth,ih.biHeight);
	file->seek(fh.bfOffBits - (sizeof(fh)+sizeof(ih)),gfile::SeekCur);

	if(ih.biBitCount == 32)
	{
		scoped_array<gsys_byte> ar(new gsys_byte[clarr.size()*4]);
		file->tread(ar.get(),clarr.size()*4);
		gsys_byte *spt = ar.get();
		dwcolor *dpt = clarr.buffer();
		for(bitmap::size_type i=0;i<clarr.size();i++,spt+=4,dpt++)
		{
			(*dpt).b = spt[0];
			(*dpt).g = spt[1];
			(*dpt).r = spt[2];
			(*dpt).a = 255;
		}
	}
	else if(ih.biBitCount == 24)
	{
		scoped_array<gsys_byte> ar(new gsys_byte[clarr.size()*3]);
		file->tread(ar.get(),clarr.size()*3);
		gsys_byte *spt = ar.get();
		dwcolor *dpt = clarr.buffer();
		for(bitmap::size_type i=0;i<clarr.size();i++,spt+=3,dpt++)
		{
			(*dpt).b = spt[0];
			(*dpt).g = spt[1];
			(*dpt).r = spt[2];
			(*dpt).a = 255;
		}
	}
	else return GERR_INVALID_DATA; 

	return G_OK;
}

GRESULT gsys::GLoadImage(gfile* file,bitmap &clarr,ImageFileFormat frmt,int Options,int* pOptionsQuery)
{
	GRESULT gr;
	
	switch(frmt)
	{

#ifdef GSYS_ENABLE_JPEG
	case ImageJPEG:
		gr = LoadJPEG(file,clarr,Options);
		break;
#endif

#ifdef GSYS_ENABLE_TIFF
	case ImageTIFF:
		gr = LoadTIFF(file,clarr,Options);
		break;
#endif

#ifdef GSYS_ENABLE_PNG
	case ImagePNG:
		gr = LoadPNG(file,clarr,Options);
		break;
#endif

	case ImageTGA:
		gr = LoadTGA(file,clarr,Options);
		break;
	case ImagePSD:
		gr = LoadPSD(file,clarr,Options);
		break;
	case ImageBMP:
		gr = LoadBMP(file,clarr,Options);
		break;
	case ImageMEI:
		gr = LoadMEI(file,clarr,Options);
		break;

	default:
		gr = GERR_INVALID_PARAMS;
	}
	return gr;
}


GRESULT gsys::GLoadImage(gfile* file,bitmap &clarr,int Options,int* pOptionsQuery)
{
	if(!file || !file->valid())return GERR_INVALID_CALL;

	int pos = file->pos();

	GRESULT gr  = GERR_INVALID_DATA;

	if(gr == GERR_INVALID_DATA)
	{
		file->seek(pos - file->pos(),SEEK_CUR);
		gr = LoadMEI(file,clarr,Options,pOptionsQuery); // call last becouse of difficat's to test it
	}

#ifdef GSYS_ENABLE_JPEG
	if(gr == GERR_INVALID_DATA)
	{
		file->seek(pos - file->pos(),gfile::SeekCur);
		gr = LoadJPEG(file,clarr,Options,pOptionsQuery);
	}
#endif

#ifdef GSYS_ENABLE_BMP
	if(gr == GERR_INVALID_DATA)
	{
		file->seek(pos - file->pos(),gfile::SeekCur);
		gr = LoadBMP(file,clarr,Options,pOptionsQuery);
	}
#endif

#ifdef GSYS_ENABLE_TIFF
	if(gr == GERR_INVALID_DATA)
	{
		file->seek(pos - file->pos(),gfile::SeekCur);
		gr = LoadTIFF(file,clarr,Options,pOptionsQuery);
	}
#endif

#ifdef GSYS_ENABLE_PNG
	if(gr == GERR_INVALID_DATA)
	{
		file->seek(pos - file->pos(),SEEK_CUR);
		gr = LoadPNG(file,clarr,Options,pOptionsQuery); 
	}
#endif
	if(gr == GERR_INVALID_DATA)
	{
		file->seek(pos - file->pos(),SEEK_CUR);
		gr = LoadPSD(file,clarr,Options,pOptionsQuery); 
	}	

	if(gr == GERR_INVALID_DATA)
	{
		file->seek(pos - file->pos(),SEEK_CUR);
		gr = LoadTGA(file,clarr,Options,pOptionsQuery); // call last becouse of difficat's to test it
	}
	if(gr == GERR_INVALID_DATA)
	{
		file->seek(pos - file->pos(),SEEK_CUR);
	}
	return gr;
}
gsys::GRESULT gsys::GLoadImage(wchar_t const* filename,gsys::bitmap & clarr,int Options,int* pOptionsQuery)
{
	std_file file(filename,gfile::ModeRead);
	if(!file)return G_FAIL;
	return GLoadImage(&file,filename,clarr,Options,pOptionsQuery);
}

gsys::GRESULT gsys::GLoadImage(gfile* file,char const* filename,gsys::bitmap & clarr,int Options,int* pOptionsQuery)
{
	return gsys::GLoadImage(file, gsys::to_wstr(filename).c_str(),clarr,Options,pOptionsQuery);
}

gsys::GRESULT gsys::GLoadImage(gfile* file,wchar_t const* filename,gsys::bitmap & clarr,int Options,int* pOptionsQuery)
{
	typedef gsys::GRESULT (*LoadImageFunc)(gfile* file,gsys::bitmap & clarr,int Options,int* pOptionsQuery);
	typedef gsys::strimap<wstring,LoadImageFunc> ImgLoaderMap;
	static ImgLoaderMap imgext;
	struct cs{
		cs()
		{
			imgext[L"tga"] = LoadTGA;
			imgext[L"targa"] = LoadTGA;

#ifdef GSYS_ENABLE_JPEG
			imgext[L"jpg"] = LoadJPEG;
			imgext[L"jpeg"] = LoadJPEG;
#endif

#ifdef GSYS_ENABLE_BMP
			imgext[L"bmp"] = LoadBMP;
			imgext[L"dib"] = LoadBMP;
			imgext[L"bitmap"] = LoadBMP;
#endif

#ifdef GSYS_ENABLE_TIFF
			imgext[L"tif"] = LoadTIFF;
			imgext[L"tiff"] = LoadTIFF;
#endif

#ifdef GSYS_ENABLE_PNG
			imgext[L"png"] = LoadPNG;
#endif
			imgext[L"psd"] = LoadPSD;
			imgext[L"mei"] = LoadMEI;
		}
	}static ssc;


	if(!filename || !filename[0]) return GLoadImage(file,clarr,Options,pOptionsQuery);

	if(!file || !file->valid())return GERR_INVALID_CALL;

	GRESULT gr = GERR_INVALID_DATA;

	wchar_t const* ext = LocateFileExt(filename);
	if(ext[0])ext++;

	int pos = file->pos();

	ImgLoaderMap::iterator it = imgext.find(ext);
	if(it != imgext.end())
	{
		gr = it->second(file,clarr,Options,pOptionsQuery);
	}
	else return GLoadImage(file,clarr,Options);

	if(gr == GERR_INVALID_DATA)
	{
		file->seek(pos - file->pos(),gfile::SeekCur);
		gr = GLoadImage(file,clarr,Options,pOptionsQuery);
	}
	if(!gr)return G_FAIL; 
	return G_OK;


/*
		//if(0){}
		if(ext == ".mei")gr = LoadMEI(file,clarr,Options,pOptionsQuery);
		else if(ext == ".tga" || ext == ".targa")gr = LoadTGA(file,clarr,Options,pOptionsQuery);
	#ifdef GSYS_ENABLE_JPEG
		else if(ext == ".jpg" || ext == ".jpeg")gr = LoadJPEG(file,clarr,Options,pOptionsQuery);
	#endif
	#ifdef GSYS_ENABLE_BMP
		else if(ext == ".bmp" || ext == ".dib" || ext == ".bitmap")gr = gsys::LoadBMP(file,clarr,Options,pOptionsQuery);
	#endif
	#ifdef GSYS_ENABLE_TIFF
		else if(ext == ".tif" || ext == ".tiff")gr = LoadTIFF(file,clarr,Options,pOptionsQuery);
	#endif
	#ifdef GSYS_ENABLE_PNG
		else if(ext == ".png")gr = LoadPNG(file,clarr,Options,pOptionsQuery);
	#endif
		else if(ext == ".psd")gr = LoadPSD(file,clarr,Options,pOptionsQuery);
		else return GLoadImage(file,clarr,Options);
	
		if(gr == GERR_INVALID_DATA)
		{
			file->seek(pos - file->pos(),gfile::SeekCur);
			gr = GLoadImage(file,clarr,Options,pOptionsQuery);
		}
		if(!gr)return G_FAIL; 
		return G_OK;
*/

}



void GTransformImage(bitmap & clarr,int current,int target,int* pOptionsQuery)
{
	if(pOptionsQuery && (*pOptionsQuery&ImageVerticalMirror))
	{
		*pOptionsQuery = (*pOptionsQuery & ~ImageVerticalMirror) | current&ImageVerticalMirror;
	}
	else if((current & ImageVerticalMirror) !=  (target & ImageVerticalMirror)) // mirror image
	{
		clarr.mirror_vertical();
	}

	if(pOptionsQuery && (*pOptionsQuery&ImagePremultipliedAlpha))
	{
		*pOptionsQuery = (*pOptionsQuery & ~ImagePremultipliedAlpha) | current&ImagePremultipliedAlpha;
	}
	else if((current & ImagePremultipliedAlpha) !=  (target & ImagePremultipliedAlpha)) // alpha manipulation
	{
		if((current & ImageAlpha) && (target & ImageAlpha))
		{
			if((current & ImagePremultipliedAlpha) == ImagePremultipliedAlpha)
			{
				_foreach(bitmap,clarr,it)
				{
					float a = float(it->a)/255.f;
					if(a>0)
					{
						it->r /= a;
						it->g /= a;
						it->b /= a;
					}
				}
			}
			else
			{
				_foreach(bitmap,clarr,it)
				{
					int a = it->a;
					it->r = (a*(it->r))/0xff;
					it->g = (a*(it->g))/0xff;
					it->b = (a*(it->b))/0xff;
				}			
			}
		}
		else 
		{
			_foreach(bitmap,clarr,it)
			{
				it->a = 255;
			}
		}
	}
}



}