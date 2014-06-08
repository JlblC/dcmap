#include "common.h"

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>

#include "../g_imgload.h"
#include "../g_file_util.h"
#include "../g_file.h"

#define MEI_MAGIC MAKEFOURCC('M','E','I',0)

struct MEI_HEAD
{
	dword Magic;
	gsys_byte  nFormatVersion;
	gsys_byte  nComponounds;
	word  reserved1;
	dword reserved2;
};
struct MEI_COMPONOUND
{
	gsys_byte   nFormats;
	gsys_byte   tech_id;
	word   componound_uses;
	dword  reserved;
};

struct MEI_FORMAT
{
	dword format_id;
	word  material_uses;
	word  width;
	word  height;
	word  scanline_lenght;
	// bit per pixel
	gsys_byte bpp;
	// number of planes for this format, used for cubemaps or volume textures
	gsys_byte planes;
	// number of lods for this format
	gsys_byte lods;
	// set if performed squad pixel compression (mostly DXT formats) compression type identified by format_id  
	unsigned  squad_compressed:1;
	// identified alpha type of image; 0-none  1-premultyplied 2 - non premultyplied
	unsigned  alpha_type:2; 
	// sets if Image mirrored verticaly
	unsigned  vertical_mirror:1;
	// set if performed gsys_byte compression and type: 0 - none, 1 - rle, 2-zip
	unsigned  byte_compressed:4;
	// say's that format data if external to file
	unsigned  data_striped:1;
	dword     reserved;
};


GRESULT gsys::LoadMEI(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery)
{
	MEI_HEAD head;
	MEI_COMPONOUND componound;
	MEI_FORMAT format;
	*file >> head;
	if(head.Magic != MEI_MAGIC)return GERR_INVALID_DATA;
	if(head.nFormatVersion != 1)return GERR_INVALID_DATA;
	if(head.nComponounds != 1)return GERR_INVALID_DATA;
	*file >> componound;
	if(componound.nFormats != 1)return GERR_INVALID_DATA;
	*file >> format;
	if(format.planes != 1)return GERR_INVALID_DATA;
	if(format.lods != 1)return GERR_INVALID_DATA;
	if(format.bpp != 32)return GERR_INVALID_DATA;
	if(format.data_striped)return GERR_INVALID_DATA;
	if(format.squad_compressed)return GERR_INVALID_DATA;
	if(format.byte_compressed)return GERR_INVALID_DATA;

	clarr.init(format.width,format.height);
	file->tread(clarr.buffer(),clarr.size());

	int trns = 0;
	if(format.alpha_type == 1) trns |= ImagePremultipliedAlpha;
	else if(format.alpha_type == 2) trns |= ImageAlpha;
	if(format.vertical_mirror) trns |= ImageVerticalMirror;

	GTransformImage(clarr,trns,Options,pOptionsQuery);
	return G_OK;
}

GRESULT gsys::SaveMEI(gsys::gfile *file,bitmap const& clarr,ImageInfo const* inf)
{
	MEI_HEAD head;
	MEI_COMPONOUND componound;
	MEI_FORMAT format;

	memset(&head,0,sizeof(head));
	memset(&componound,0,sizeof(componound));
	memset(&format,0,sizeof(format));

	head.Magic = MEI_MAGIC;
	head.nFormatVersion = 1;
	head.nComponounds = 1;
	componound.nFormats = 1;
	format.planes = 1;
	format.lods = 1;
	format.bpp = 32;
	format.width = clarr.width();
	format.height = clarr.height();

	if(inf)
	{
		if( (inf->options&ImagePremultipliedAlpha) == ImagePremultipliedAlpha) format.alpha_type = 1;
		else if(inf->options & ImageAlpha) format.alpha_type = 2;

		if(inf->options & ImageVerticalMirror)format.vertical_mirror = 1;
	}

	*file << head;
	*file << componound;
	*file << format;

	file->twrite(clarr.buffer(),clarr.size());

	return G_OK;
}
