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

	// структура содержащая необходимую для этого информацию
	struct cDataManagerJPEGError
	{
		jpeg_error_mgr  pub;      // ссылочка на стандартный менеджер
		jmp_buf    setjmp_buffer;  // jump-буфера, идентификатор прыжка
	};

	// наша функция обработки ошибок
	void JPEGErrorExit(j_common_ptr cinfo)
	{
		// получаем ссылочку на нашу структуру
		cDataManagerJPEGError* myerr = (cDataManagerJPEGError*)cinfo->err;
		// выводим сообщение об ошибке (наверное можно убрать)
		(*cinfo->err->output_message)(cinfo);
		// делаем прыжок на очистку данных и ретурн ошибки
		longjmp(myerr->setjmp_buffer, 1);
	}


	struct cDataManagerJPEGSource
	{
		jpeg_source_mgr  pub;    // ссылочка на стандартный менеджер
		gfile      *file;  // открытый файл
		JOCTET    *buffer;  // буфер данных
		bool      sof;    // признак того, что файл только что открыли
	};

	// вызывается, когда переменная bytes_in_buffer достигает 0 и возникает
	// необходимость в новой порции информации. возвращает TRUE, если буфер
	// перезаполнен успешно, и FALSE если произошла ошибка ввода/вывода.
	boolean JPEGFillInputBuffer(j_decompress_ptr cinfo)
	{
		cDataManagerJPEGSource* src = (cDataManagerJPEGSource*)cinfo->src;
		// читаем кусками по c_JPEGInputBufferSize байт
		size_t nbytes = src->file->read(src->buffer, sizeof(JOCTET), c_JPEGInputBufferSize);
		// если мы ничего не считали :(
		if (nbytes <= 0) 
		{
			if ( src->sof )  return(FALSE); // блин, нам дали пустой файл - заорем "нехорошо" :)
			// если уже читали до этого, то вставляем в буфер инфу о конце файла
			src->buffer[0] = (JOCTET) 0xFF;
			src->buffer[1] = (JOCTET) JPEG_EOI;
			nbytes = 2;
		}
		// загоняем инфу в буфер, и размер скока прочли
		src->pub.next_input_byte = src->buffer;
		src->pub.bytes_in_buffer = nbytes;
		src->sof = false;  // файл не пустой, пронесло :)))
		// возвращаем успешное выполнение операции
		return(TRUE);
	}
    // вызывается, когда надо пропустить несколько байт в файле
	void JPEGSkipInputData(j_decompress_ptr cinfo, long num_bytes)
	{
		cDataManagerJPEGSource* src = (cDataManagerJPEGSource*)cinfo->src;
		src->file->skip(num_bytes);
	}

	// вызывается, когда необходимо инициализировать источник данных перед первым использованием
	void JPEGInitSource(j_decompress_ptr cinfo)
	{
		cDataManagerJPEGSource* src = (cDataManagerJPEGSource*)cinfo->src;
		src->sof = true;
	}

	// вызывается  для очистки данных по завершении чтения информации.
	void JPEGTermSource(j_decompress_ptr cinfo)
	{
		cDataManagerJPEGSource* src = (cDataManagerJPEGSource*)cinfo->src;
		src->sof = true;
	}

	void JPEGStdioSrc(j_decompress_ptr cinfo, gfile* file)
	{
		cDataManagerJPEGSource* src = 0;
		// смотрим, выделена ли память под JPEG-декомпрессор менеджер?
		// возможна ситуация, когда происходит одновременное обращение к источнику
		// от нескольких библиотек
		if (cinfo->src == 0) 
		{
			// выделим память под наш менеджер, и установим на него указатель глобальной структуры
			// библиотеки. так как я использую менеджер памяти библиотеки JPEG то позаботится об
			// освобождении она сама. JPOOL_PERMANENT - означает что эта память выделяется на все
			// время работы с библиотекой
			cinfo->src = (struct jpeg_source_mgr *) (*cinfo->mem->alloc_small) 
				((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(cDataManagerJPEGSource));
			src = (cDataManagerJPEGSource*) cinfo->src;
			// выделяю память для буффера данных, прочитанных из файла
			src->buffer = (JOCTET*) (*cinfo->mem->alloc_small) 
				((j_common_ptr) cinfo, JPOOL_PERMANENT, c_JPEGInputBufferSize * sizeof(JOCTET));
			memset(src->buffer, 0, c_JPEGInputBufferSize * sizeof(JOCTET));
		}
		// для краткости - сестры таланта
		src = (cDataManagerJPEGSource*)cinfo->src;
		// настраиваем обработчики событий на наши функции
		src->pub.init_source = JPEGInitSource;
		src->pub.fill_input_buffer = JPEGFillInputBuffer;
		src->pub.skip_input_data = JPEGSkipInputData;
		src->pub.resync_to_restart = jpeg_resync_to_restart; // use default method
		src->pub.term_source = JPEGTermSource;
		// теперь заполняем поля нашей структуры
		src->file = file;
		// настраиваем указатели на буфера
		src->pub.bytes_in_buffer = 0;  // forces fill_input_buffer on first read
		src->pub.next_input_byte = 0;  // until buffer loaded
		// !!! ВНИМАНИЕ !!! в библиотеке глючок, почему-то если infile стандартного менеджера
		// установлен в 0 то jpeg_start_decompress отказывается работать... поэтому установим его в 1 :)
//		src->pub.infile = (FILE*)1;    // указатель на файл в стандартном менеджере, см. выше
//		src->pub.buffer = 0;      // это буфер стандартного менеджера загрузки, мы им не пользуемся
	}


	GRESULT gsys::LoadJPEG(gsys::gfile *file, gsys::bitmap& clarr,int Options,int* pOptionsQuery)
	{
		jpeg_decompress_struct cinfo;
		cDataManagerJPEGError jerr;

		GRESULT grResult = GERR_INVALID_DATA;

		// Настраиваем обработку ошибок JPEG декомпрессором.
		cinfo.err = jpeg_std_error(&jerr.pub);    // устанавливаем дефолтный менеджер обработки ошибок
		jerr.pub.error_exit = JPEGErrorExit;    // присваиваем дефолтную функцию для обработки ошибки
		// Устанавливаем контекст возвращения setjmp на JPEGErrorExit
		if( setjmp(jerr.setjmp_buffer) )
		{
			// когда-то произошла ошибка. Очистим структуру и закроем файл
			jpeg_destroy_decompress(&cinfo);
			return grResult;
		}

		// инициализируем декомпрессионный объект
		jpeg_create_decompress(&cinfo);
		// указываем источник данных
		JPEGStdioSrc(&cinfo, file);

		jpeg_save_markers(&cinfo, JPEG_COM, 0xFFFF);
		for(int m = 0; m < 16; m++) {
			jpeg_save_markers(&cinfo, JPEG_APP0 + m, 0xFFFF);
		}

		// читаем параметры JPEG файла
		jpeg_read_header(&cinfo, TRUE);
		grResult = G_FAIL;

		// начинаем декомпрессию
		jpeg_start_decompress(&cinfo);


		// вычисляем размер строки в байтах
		int row_stride = cinfo.output_width * cinfo.output_components;

		
		//cinfo.o

		// выделяем память для картинки
		scoped_array<gsys_byte> pixels(new gsys_byte[cinfo.output_height * row_stride]);

		// Выделяем память для одной строки. Но нужно помнить, что это массив строк, просто он состоит 
		// из одной строки :) JPOOL_IMAGE, означает, что память выделена пока читаем эту картинку
		JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)( (j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1 );
		memset(*buffer, 0, sizeof(JSAMPLE) * row_stride);

		// считываем данные
		// Т.к. юзаем внутреннюю переменную cinfo.output_scanline, то не будем отслеживать сами себя
		int y = 0;
		while ( cinfo.output_scanline < cinfo.output_height )
		{
			// читаем строку в буфер
			jpeg_read_scanlines(&cinfo, buffer, 1);
			// и вставляем эту строку на нужное место в массиве картинки
			// блин, они опять перевернули картинку! :(
			gsys_byte* temp = &pixels[ (cinfo.output_height - y - 1) * row_stride];
			memcpy(temp, buffer[0], row_stride);
			y++;
		}

		// Завершаем декомпрессию файла
		jpeg_finish_decompress(&cinfo);
		//Очищаем объект декомпрессии
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


