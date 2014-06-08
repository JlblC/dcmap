#pragma once

#include "g_gsys.h"
/*
#ifdef GSYS_DEBUG
#	ifdef GSYS_OGG_STATIC_LINK
#		pragma comment(lib ,"ogg_static_d.lib")
#	else
#		pragma comment(lib ,"ogg_d.lib")
#	endif
#	ifdef GSYS_VORBIS_STATIC_LINK
#		pragma comment(lib ,"vorbis_static_d.lib")
#		pragma comment(lib ,"vorbisfile_static_d.lib")
#	else
#		pragma comment(lib ,"vorbis_d.lib")
#		pragma comment(lib ,"vorbisfile_d.lib")
#	endif
#else
#	ifdef GSYS_OGG_STATIC_LINK
#		pragma comment(lib ,"ogg_static.lib")
#	else
#		pragma comment(lib ,"ogg.lib")
#	endif
#	ifdef GSYS_VORBIS_STATIC_LINK
#		pragma comment(lib ,"vorbis_static.lib")
#		pragma comment(lib ,"vorbisfile_static.lib")
#	else
#		pragma comment(lib ,"vorbis.lib")
#		pragma comment(lib ,"vorbisfile.lib")
#	endif
#endif
*/

#		pragma comment(lib ,"vorbis.lib")
#		pragma comment(lib ,"vorbisfile.lib")
#		pragma comment(lib ,"ogg.lib")

//#include <stdlib.h>
//#include <math.h>

#include "./src/libs/ogg/ogg.h"
#include "./src/libs/vorbis/codec.h"
#include "./src/libs/vorbis/vorbisfile.h"
#include "./src/libs/vorbis/vorbisenc.h"

#include "g_file.h"
#include "g_std_file.h"

namespace gsys
{
	int ov_open_gfile(gfile *f,OggVorbis_File *vf,char *initial,long ibytes);

	class vorbis_file
	{
		OggVorbis_File vf; 
		bool opened;
	public:
		typedef ulong size_type;
		vorbis_file():opened(false)
		{
		}
		~vorbis_file()
		{
			close();
		}
		bool open(gfile *f,char *initial=NULL,long ibytes=-1)
		{
			opened=false;
			if(ov_open_gfile(f,&vf,initial,ibytes) ==0)
			{
				opened = true;
			}
			return opened;
		}
		void close()
		{
			if(opened)ov_clear(&vf);
			opened=false;
		}	
		size_type read(char *buffer,int length,
			int bigendianp,int word,int sgned,int *bitstream)
		{
			if(!opened)return 0;
			return ov_read(&vf,buffer,length,bigendianp,word,sgned,bitstream);
		}

		long read(char *buffer, int length)
		{
			if(!opened)return 0;
			int current_section;
			return ov_read(&vf,buffer,length,0,2,1,&current_section);
		}

		size_type pcm_total(int i=-1)
		{
			if(!opened)return 0;
			return (size_type)ov_pcm_total(&vf,i);
		}
		gsys_real time_total(int i=-1)
		{
			if(!opened)return 0;
			return (gsys_real)ov_time_total(&vf,i);
		}
		vorbis_info* info(int link=-1)
		{
			if(!opened)return 0;
			return ov_info(&vf,link);
		}

		int channels(int link=-1)
		{
			if(!opened)return 0;
			vorbis_info* vi;
			vi = ov_info(&vf,link);
			return vi->channels;
		}

		long rate(int link=-1)
		{
			if(!opened)return 0;
			vorbis_info* vi;
			vi = ov_info(&vf,link);
			return vi->rate;
		}

		bool pcm_seek(long pos)
		{
			if(!opened)return 0;
			return ov_pcm_seek(&vf,pos)==0;
		}

		bool time_seek(float fPos)
		{
			if(!opened)return 0;
			return ov_time_seek(&vf,fPos) == 0;
		}

		bool time_seek_lap(float fPos)
		{
			if(!opened)return 0;
			return ov_time_seek_lap(&vf,fPos) == 0;
		}

		bool rewind()
		{
			if(!opened)return 0;
			return ov_pcm_seek(&vf,0)==0;
		}

	};
};














