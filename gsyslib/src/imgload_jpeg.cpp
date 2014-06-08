#include "common.h"

#include <setjmp.h>

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>

#include "../g_imgload.h"
#include "../g_file_util.h"
#include "../g_file.h"

#ifdef GSYS_ENABLE_JPEG
#	include "./libs/jpeglib.h"
#	ifdef GSYS_JPEG_STATIC_LINK
#		ifdef GSYS_DEBUG
#			pragma comment(lib ,"libjpegstat_d.lib")
#		else
#			pragma comment(lib ,"libjpegstat.lib")
#		endif
#	else
#		ifdef GSYS_DETAIL_LIB_OMF
#			pragma comment(lib ,"libjpeg_omf.lib")
#		else 
#			pragma comment(lib ,"libjpeg.lib")	
#		endif
#	endif


using namespace gsys;
using namespace boost;

namespace gsys
{
	static const int c_JPEGInputBufferSize = 1024;

	// ��������� ���������� ����������� ��� ����� ����������
	struct cDataManagerJPEGError
	{
		jpeg_error_mgr  pub;      // �������� �� ����������� ��������
		jmp_buf    setjmp_buffer;  // jump-������, ������������� ������
	};

	// ���� ������� ��������� ������
	void JPEGErrorExit(j_common_ptr cinfo)
	{
		// �������� �������� �� ���� ���������
		cDataManagerJPEGError* myerr = (cDataManagerJPEGError*)cinfo->err;
		// ������� ��������� �� ������ (�������� ����� ������)
		(*cinfo->err->output_message)(cinfo);
		// ������ ������ �� ������� ������ � ������ ������
		longjmp(myerr->setjmp_buffer, 1);
	}


	struct cDataManagerJPEGSource
	{
		jpeg_source_mgr  pub;    // �������� �� ����������� ��������
		gfile      *file;  // �������� ����
		JOCTET    *buffer;  // ����� ������
		bool      sof;    // ������� ����, ��� ���� ������ ��� �������
	};

	// ����������, ����� ���������� bytes_in_buffer ��������� 0 � ���������
	// ������������� � ����� ������ ����������. ���������� TRUE, ���� �����
	// ������������ �������, � FALSE ���� ��������� ������ �����/������.
	boolean JPEGFillInputBuffer(j_decompress_ptr cinfo)
	{
		cDataManagerJPEGSource* src = (cDataManagerJPEGSource*)cinfo->src;
		// ������ ������� �� c_JPEGInputBufferSize ����
		size_t nbytes = src->file->read(src->buffer, sizeof(JOCTET), c_JPEGInputBufferSize);
		// ���� �� ������ �� ������� :(
		if (nbytes <= 0) 
		{
			if ( src->sof )  return(FALSE); // ����, ��� ���� ������ ���� - ������ "��������" :)
			// ���� ��� ������ �� �����, �� ��������� � ����� ���� � ����� �����
			src->buffer[0] = (JOCTET) 0xFF;
			src->buffer[1] = (JOCTET) JPEG_EOI;
			nbytes = 2;
		}
		// �������� ���� � �����, � ������ ����� ������
		src->pub.next_input_byte = src->buffer;
		src->pub.bytes_in_buffer = nbytes;
		src->sof = false;  // ���� �� ������, �������� :)))
		// ���������� �������� ���������� ��������
		return(TRUE);
	}
    // ����������, ����� ���� ���������� ��������� ���� � �����
	void JPEGSkipInputData(j_decompress_ptr cinfo, long num_bytes)
	{
		cDataManagerJPEGSource* src = (cDataManagerJPEGSource*)cinfo->src;
		src->file->skip(num_bytes);
	}

	// ����������, ����� ���������� ���������������� �������� ������ ����� ������ ��������������
	void JPEGInitSource(j_decompress_ptr cinfo)
	{
		cDataManagerJPEGSource* src = (cDataManagerJPEGSource*)cinfo->src;
		src->sof = true;
	}

	// ����������  ��� ������� ������ �� ���������� ������ ����������.
	void JPEGTermSource(j_decompress_ptr cinfo)
	{
		cDataManagerJPEGSource* src = (cDataManagerJPEGSource*)cinfo->src;
		src->sof = true;
	}

	void JPEGStdioSrc(j_decompress_ptr cinfo, gfile* file)
	{
		cDataManagerJPEGSource* src = 0;
		// �������, �������� �� ������ ��� JPEG-������������ ��������?
		// �������� ��������, ����� ���������� ������������� ��������� � ���������
		// �� ���������� ���������
		if (cinfo->src == 0) 
		{
			// ������� ������ ��� ��� ��������, � ��������� �� ���� ��������� ���������� ���������
			// ����������. ��� ��� � ��������� �������� ������ ���������� JPEG �� ����������� ��
			// ������������ ��� ����. JPOOL_PERMANENT - �������� ��� ��� ������ ���������� �� ���
			// ����� ������ � �����������
			cinfo->src = (struct jpeg_source_mgr *) (*cinfo->mem->alloc_small) 
				((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(cDataManagerJPEGSource));
			src = (cDataManagerJPEGSource*) cinfo->src;
			// ������� ������ ��� ������� ������, ����������� �� �����
			src->buffer = (JOCTET*) (*cinfo->mem->alloc_small) 
				((j_common_ptr) cinfo, JPOOL_PERMANENT, c_JPEGInputBufferSize * sizeof(JOCTET));
			memset(src->buffer, 0, c_JPEGInputBufferSize * sizeof(JOCTET));
		}
		// ��� ��������� - ������ �������
		src = (cDataManagerJPEGSource*)cinfo->src;
		// ����������� ����������� ������� �� ���� �������
		src->pub.init_source = JPEGInitSource;
		src->pub.fill_input_buffer = JPEGFillInputBuffer;
		src->pub.skip_input_data = JPEGSkipInputData;
		src->pub.resync_to_restart = jpeg_resync_to_restart; // use default method
		src->pub.term_source = JPEGTermSource;
		// ������ ��������� ���� ����� ���������
		src->file = file;
		// ����������� ��������� �� ������
		src->pub.bytes_in_buffer = 0;  // forces fill_input_buffer on first read
		src->pub.next_input_byte = 0;  // until buffer loaded
		// !!! �������� !!! � ���������� ������, ������-�� ���� infile ������������ ���������
		// ���������� � 0 �� jpeg_start_decompress ������������ ��������... ������� ��������� ��� � 1 :)
//		src->pub.infile = (FILE*)1;    // ��������� �� ���� � ����������� ���������, ��. ����
//		src->pub.buffer = 0;      // ��� ����� ������������ ��������� ��������, �� �� �� ����������
	}


	GRESULT gsys::LoadJPEG(gsys::gfile *file, gsys::bitmap& clarr,int Options,int* pOptionsQuery)
	{
		jpeg_decompress_struct cinfo;
		cDataManagerJPEGError jerr;

		GRESULT grResult = GERR_INVALID_DATA;

		// ����������� ��������� ������ JPEG ��������������.
		cinfo.err = jpeg_std_error(&jerr.pub);    // ������������� ��������� �������� ��������� ������
		jerr.pub.error_exit = JPEGErrorExit;    // ����������� ��������� ������� ��� ��������� ������
		// ������������� �������� ����������� setjmp �� JPEGErrorExit
		if( setjmp(jerr.setjmp_buffer) )
		{
			// �����-�� ��������� ������. ������� ��������� � ������� ����
			jpeg_destroy_decompress(&cinfo);
			return grResult;
		}

		// �������������� ���������������� ������
		jpeg_create_decompress(&cinfo);
		// ��������� �������� ������
		JPEGStdioSrc(&cinfo, file);

		jpeg_save_markers(&cinfo, JPEG_COM, 0xFFFF);
		for(int m = 0; m < 16; m++) {
			jpeg_save_markers(&cinfo, JPEG_APP0 + m, 0xFFFF);
		}

		// ������ ��������� JPEG �����
		jpeg_read_header(&cinfo, TRUE);
		grResult = G_FAIL;

		// �������� ������������
		jpeg_start_decompress(&cinfo);


		// ��������� ������ ������ � ������
		int row_stride = cinfo.output_width * cinfo.output_components;

		
		//cinfo.o

		// �������� ������ ��� ��������
		scoped_array<gsys_byte> pixels(new gsys_byte[cinfo.output_height * row_stride]);

		// �������� ������ ��� ����� ������. �� ����� �������, ��� ��� ������ �����, ������ �� ������� 
		// �� ����� ������ :) JPOOL_IMAGE, ��������, ��� ������ �������� ���� ������ ��� ��������
		JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)( (j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1 );
		memset(*buffer, 0, sizeof(JSAMPLE) * row_stride);

		// ��������� ������
		// �.�. ����� ���������� ���������� cinfo.output_scanline, �� �� ����� ����������� ���� ����
		int y = 0;
		while ( cinfo.output_scanline < cinfo.output_height )
		{
			// ������ ������ � �����
			jpeg_read_scanlines(&cinfo, buffer, 1);
			// � ��������� ��� ������ �� ������ ����� � ������� ��������
			// ����, ��� ����� ����������� ��������! :(
			gsys_byte* temp = &pixels[ (cinfo.output_height - y - 1) * row_stride];
			memcpy(temp, buffer[0], row_stride);
			y++;
		}

		// ��������� ������������ �����
		jpeg_finish_decompress(&cinfo);
		//������� ������ ������������
		jpeg_destroy_decompress(&cinfo);

		// transform data to standard 
		if(cinfo.output_components == 3) // RGB
		{
			clarr.init(cinfo.output_width,cinfo.output_height);

			gsys_byte* pix = &pixels[0];

			_foreach(bitmap,clarr,i)
			{
				i->r = pix[0];
				i->g = pix[1];
				i->b = pix[2];
				i->a = 255;

				pix+= 3; // 3 components
			}
		}
		//GTransformImage(clarr,ImageVerticalMirror,Options);
		GTransformImage(clarr,0,Options,pOptionsQuery);
		return G_OK;
	}
}

#endif // GSYS_ENABLE_JPEG


