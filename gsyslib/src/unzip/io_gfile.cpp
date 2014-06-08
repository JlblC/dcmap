#include "common.h"

#include "../../g_file.h"
#include "../../g_zip_fdir.h"

#include "unzip.h"

namespace gsys{ namespace detail
{

	voidpf ZCALLBACK static fopen_file_func(voidpf opaque,const char* filename,int mode)
	{
		gfile& file = *reinterpret_cast<gfile*>(opaque);
		GSYS_ASSERTREF(file);
		if(file)return (voidpf)1;
		return (voidpf)0;
	}

	uLong ZCALLBACK static fread_file_func(voidpf opaque,voidpf stream,void* buf,uLong size)
	{
		gfile& file = *reinterpret_cast<gfile*>(opaque);
		GSYS_ASSERTREF(file);

		return file.read(buf,1,size);
	}


	uLong ZCALLBACK static fwrite_file_func (voidpf opaque,voidpf stream,const void* buf,uLong size)
	{
		gfile& file = *reinterpret_cast<gfile*>(opaque);
		GSYS_ASSERTREF(file);

		return file.write(buf,size);
	}

	long ZCALLBACK static ftell_file_func (voidpf opaque,voidpf stream)
	{
		gfile& file = *reinterpret_cast<gfile*>(opaque);
		GSYS_ASSERTREF(file);
		return file.pos();
	}

	long ZCALLBACK static fseek_file_func (voidpf opaque,voidpf stream,uLong offset,int origin)
	{
		gfile& file = *reinterpret_cast<gfile*>(opaque);
		GSYS_ASSERTREF(file);

		gfile::FileSeek fseek_origin;
		switch (origin)
		{
		case ZLIB_FILEFUNC_SEEK_CUR :
			fseek_origin = gfile::SeekCur;
			break;
		case ZLIB_FILEFUNC_SEEK_END :
			fseek_origin = gfile::SeekEnd;
			break;
		case ZLIB_FILEFUNC_SEEK_SET :
			fseek_origin = gfile::SeekSet;
			break;
		default: return -1;
		}

		file.seek(offset,fseek_origin);

		return 0;
	}

	int ZCALLBACK static fclose_file_func (voidpf opaque,voidpf stream)
	{
		return 0;
	}

	int ZCALLBACK static ferror_file_func (voidpf opaque,voidpf stream)
	{
		return 0;
	}
}}

void gsys::detail::fill_gfile_filefunc(zlib_filefunc_def* pzlib_filefunc_def,gfile* pFile)
{
	pzlib_filefunc_def->zopen_file = gsys::detail::fopen_file_func;
	pzlib_filefunc_def->zread_file = gsys::detail::fread_file_func;
	pzlib_filefunc_def->zwrite_file = gsys::detail::fwrite_file_func;
	pzlib_filefunc_def->ztell_file = gsys::detail::ftell_file_func;
	pzlib_filefunc_def->zseek_file = gsys::detail::fseek_file_func;
	pzlib_filefunc_def->zclose_file = gsys::detail::fclose_file_func;
	pzlib_filefunc_def->zerror_file = gsys::detail::ferror_file_func;
	pzlib_filefunc_def->opaque = (voidpf)pFile;
}

