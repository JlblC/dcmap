#include "common.h"

#ifdef GSYS_ENABLE_OGG_VORBIS
#include "../g_ogg_vorbis.h"

static size_t read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	gfile* file = reinterpret_cast<gfile*>(datasource);
	return file->read(ptr,(gfile::size_type)size,(gfile::size_type)nmemb);
}
static int    seek_func(void *datasource, ogg_int64_t offset, int whence)
{
	gfile* file = reinterpret_cast<gfile*>(datasource);
	file->seek(offset,whence);
	return 0;
}
static int close_func(void *datasource)
{
	gfile* file = reinterpret_cast<gfile*>(datasource);
	return 0;
}
static long   tell_func(void *datasource)
{
	gfile* file = reinterpret_cast<gfile*>(datasource);
	return file->pos();
}

int gsys::ov_open_gfile(gfile *f,OggVorbis_File *vf,char *initial,long ibytes)
{
	GSYS_ASSERT(f);
	ov_callbacks clbk;
	clbk.close_func = close_func;
	clbk.read_func = read_func;
	clbk.seek_func = seek_func;
	clbk.tell_func = tell_func;
	return ov_open_callbacks(f,vf,initial,ibytes,clbk);
}

#endif