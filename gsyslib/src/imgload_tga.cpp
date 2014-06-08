#include "common.h"

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>

#include "../g_imgload.h"
#include "../g_file_util.h"
#include "../g_file.h"

#ifdef GSYS_ENABLE_TGA

using namespace gsys;
using namespace boost;

namespace gsys
{
	static const int max_image_size = 1024*8;

	#define TARGA_NO_COLORMAP 0
	#define TARGA_COLORMAP 1

	#define TARGA_EMPTY_IMAGE 0
	#define TARGA_INDEXED_IMAGE 1
	#define TARGA_TRUECOLOR_IMAGE 2
	#define TARGA_BW_IMAGE 3
	#define TARGA_INDEXED_RLE_IMAGE 9
	#define TARGA_TRUECOLOR_RLE_IMAGE 10
	#define TARGA_BW_RLE_IMAGE 11


	//Targa header info
	#pragma pack (push)
	#pragma pack (1)	//dont pad the following struct

	typedef struct _TGAHeaderInfo
	{
		gsys_byte idlen;    //length of optional identification sequence
		gsys_byte cmtype;   //indicates whether a palette is present
		gsys_byte imtype;   //image data type (e.g., uncompressed RGB)
		gsys_word cmorg;    //first palette index, if present
		gsys_word cmcnt;    //number of palette entries, if present
		gsys_byte cmsize;   //number of bits per palette entry
		gsys_word imxorg;   //horiz pixel coordinate of lower left of image
		gsys_word imyorg;   //vert pixel coordinate of lower left of image
		gsys_word imwidth;  //image width in pixels
		gsys_word imheight; //image height in pixels
		gsys_byte imdepth;  //image color depth (bits per pixel)
		gsys_byte imdesc;   //image attribute flags
	}TGAHeaderInfo;

	typedef struct _TGA_pixel
	{
		gsys_byte blue;
		gsys_byte green;
		gsys_byte red;
		gsys_byte alpha;
	} pixel;

	#pragma pack (pop)

	
	inline void make_dwcl(gsys_byte* src,int bd,dwcolor& cl)
	{
		if(bd == 4) cl.set(src[2],src[1],src[0],src[3]);
		else if(bd == 3) cl.set(src[2],src[1],src[0],255);
		else if(bd == 2)
		{
			word& wrd = *((word*)src);
			cl.b = (wrd &(0x001F)) << 3;
			cl.g = (wrd &(0x03E0)) >> 2;
			cl.r = (wrd &(0x7C00)) >> 7;
			cl.a = (wrd &(0x8000))?255:0;
		}
		else if(bd == 1) cl.set(src[0],src[0],src[0],255);
	}

	inline void pack_dwcl(dwcolor const& cl,gsys_byte* dst,int bd)
	{
		if(bd == 4)
		{
			dst[2] = cl.r;
			dst[1] = cl.g;
			dst[0] = cl.b;
			dst[3] = cl.a;
		}
		else if(bd == 3)
		{
			dst[2] = cl.r;
			dst[1] = cl.g;
			dst[0] = cl.b;
		}
		else if(bd == 2)
		{
			word& wrd = *((word*)dst);
			wrd=0;
			wrd |= (word(cl.b) >> 3);
			wrd |= (word(cl.g) >> 3) << 5;
			wrd |= (word(cl.r) >> 3) << 10;
			wrd |= cl.a==0?0x8000:0;
		}
		else if(bd == 1) 
		{
			dst[0] = (cl.r+cl.g+cl.b)/3;
		}
	}

	GRESULT gsys::LoadTGA(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery)
	{
		TGAHeaderInfo TGAHeader;
		file->tread(&TGAHeader);
		
		if(!TGAHeader.imwidth || !TGAHeader.imheight) return GERR_INVALID_DATA;
		if(TGAHeader.imwidth > max_image_size || TGAHeader.imheight > max_image_size) return GERR_INVALID_DATA;
		if(TGAHeader.cmtype)  return GERR_INVALID_DATA;

		// only 8, 16, 24, 32 bits image support
		if ((TGAHeader.imtype != TARGA_TRUECOLOR_IMAGE) &&
			(TGAHeader.imtype != TARGA_BW_IMAGE) &&
			(TGAHeader.imtype != TARGA_TRUECOLOR_RLE_IMAGE) &&
			(TGAHeader.imtype != TARGA_BW_RLE_IMAGE)) return GERR_INVALID_DATA;

		if( TGAHeader.imdepth != 8 &&
			TGAHeader.imdepth != 16 &&
			TGAHeader.imdepth != 24 &&
			TGAHeader.imdepth != 32 ) return GERR_INVALID_DATA;
	
		file->seek(TGAHeader.idlen, gfile::SeekCur);

		//gsys::array2d<gsys::dwcolor> clarr(TGAHeader.imwidth,TGAHeader.imheight);
		clarr.init(TGAHeader.imwidth,TGAHeader.imheight);
		// allocate memporary buffer
		int bd = TGAHeader.imdepth/8;
		int Components = TGAHeader.imdepth/8;
		uint npixels = TGAHeader.imwidth*TGAHeader.imheight;
		int len = npixels*bd;

		gsys_byte Sample[4];
		bitmap::iterator it = clarr.begin();

		// RAW
		if ((TGAHeader.imtype == TARGA_TRUECOLOR_IMAGE) ||
			(TGAHeader.imtype == TARGA_BW_IMAGE))
		{
			scoped_array<gsys_byte> buf(new gsys_byte[len]);
			file->tread(buf.get(),len);

			gsys_byte* pt = &buf[0];
			while(it != clarr.end())
			{
				make_dwcl(pt,bd,*it);
                pt += bd;
				++it;
			}
		}
		// RLE
		else if ((TGAHeader.imtype == TARGA_TRUECOLOR_RLE_IMAGE) ||
			(TGAHeader.imtype == TARGA_BW_RLE_IMAGE))
		{
			gsys_byte PacketHeader, PacketLength;
			uint i=0;
			while(i < npixels)
			{
				file->tread(&PacketHeader);
				PacketLength = (PacketHeader & 0x7F) + 1;

				if ((PacketHeader & 0x80) != 0)// RL
				{
					file->read(Sample,bd);
					for (int j = 0; j < PacketLength; j++)
					{
						make_dwcl(Sample,bd,*it);
						++i;
						++it;
					}
				}
				else //RAW
				{
					for (int j = 0; j < PacketLength; j++)
					{
						file->read(Sample,bd);
						make_dwcl(Sample,bd,*it);
						++i;
						++it;
					}
				}
			}
		}
		else return GERR_INVALID_DATA;
		GTransformImage(clarr,ImageAlpha,Options,pOptionsQuery);
		return G_OK;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	GRESULT gsys::SaveTGA(gsys::gfile *file,bitmap const& clarr,ImageInfo const* inf)
	{
		int bd;
		TGAHeaderInfo TGAHeader;
		memset(&TGAHeader,0,sizeof(TGAHeader));

		bitmap temp;
		bitmap const* src = &clarr;
		if(inf)
		{
			if((inf->options & ImageVerticalMirror))
			{
				temp.make_copy(*src);
				temp.mirror_vertical();
				src = &temp;
			}
			bd = 4; 
			TGAHeader.imtype = TARGA_TRUECOLOR_IMAGE;
		}
		else
		{
			bd = 4; 
			TGAHeader.imtype = TARGA_TRUECOLOR_IMAGE;
		}

		TGAHeader.imwidth = (gsys_word)clarr.width();
		TGAHeader.imheight = (gsys_word)clarr.height();
		TGAHeader.idlen=0;
		
		TGAHeader.imdepth = bd*8;
			
		*file << TGAHeader;

		gsys_byte buf[4];

		if(TGAHeader.imtype == TARGA_TRUECOLOR_IMAGE)
		{
			//for(int i=TGAHeader.imheight;--i>=0;)
			for(int i=0;i<TGAHeader.imheight;i++)
			{
				dwcolor const* cl = src->row(i);
				for(int j=0;j<TGAHeader.imwidth;j++)
				{
					pack_dwcl(cl[j],buf,bd);
					file->write(buf,bd);
				}
			}
		}
		else return G_FAIL;	

		return G_OK;	
	}
}
#endif







