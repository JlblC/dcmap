#pragma once
#include "g_gsys.h"
#include "g_zlib.h"
#include "g_alg.h"
#include "src/unzip/unzip.h"

namespace gsys
{
	namespace detail
	{
		void fill_gfile_filefunc(zlib_filefunc_def* pzlib_filefunc_def,gfile* pFile);
	};

	class zip_archive
	{
	public:
		typedef wchar_t char_type;
		typedef std::basic_string<char_type> string_type;
	protected:
		unzFile m_zip;

		bool m_bFileOpened;

		unz_file_info m_FileInfo;
		string_type m_strFileName;

		void GetFileInfo()
		{
			char buf[GSYS_MAX_PATH];
			unzGetCurrentFileInfo(m_zip,&m_FileInfo,buf,GSYS_MAX_PATH,0,0,0,0);
			m_strFileName = to_wstr(buf);
		};

	public:
		class file_pos
		{
			friend class zip_archive;
			unz_file_pos f_pos;

			file_pos(int i)
			{
				f_pos.pos_in_zip_directory = i;
				f_pos.num_of_file = i;
			}

			file_pos(unz_file_pos &pos ):f_pos(pos){}
			operator unz_file_pos& (){return f_pos;}
		public:
			file_pos(){}
		};

		zip_archive():m_zip(0),m_bFileOpened(false){}

		// using gfile (gfile must exists) 
		bool open(gfile* pFile)
		{
			zlib_filefunc_def fdef;
			detail::fill_gfile_filefunc(&fdef,pFile);
			m_zip = unzOpen2("",&fdef);
			if(!m_zip)return false;
            return true;
		}

		bool open(char_type const* file)
		{
			m_zip = unzOpen(detail::WideToStr(file));
			if(!m_zip)return false;
            return true;
		}
		void close()
		{
			if(m_bFileOpened)close_file();
			if(m_zip)unzClose(m_zip);
			m_zip = 0;
		}

		bool goto_first_file()
		{
			if(m_zip && !m_bFileOpened)
			{
				if(unzGoToFirstFile(m_zip) == UNZ_OK)
				{
					GetFileInfo();
					return true;
				}

			}
			return false;
		}

		bool goto_next_file()
		{
			if(m_zip && !m_bFileOpened)
			{
				if(unzGoToNextFile(m_zip) == UNZ_OK)
				{
					GetFileInfo();
					return true;
				}

			}
			return false;
		}

		bool locate_file(char_type const* name)
		{
			if(m_zip && !m_bFileOpened)
			{
				if(unzLocateFile(m_zip,detail::WideToStr(name),false) == UNZ_OK)
				{
					GetFileInfo();
					return true;
				}

			}
			return false;
		}

		file_pos get_file_pos()
		{
			file_pos pos;
			if(m_zip)
			{
				if(unzGetFilePos(m_zip,(unz_file_pos*)&pos) == UNZ_OK)
				{
					return pos;
				}
			}
			return file_pos(0);
		}

		bool select_file(file_pos const& pos)
		{
			if(m_zip && !m_bFileOpened)
			{
				if(unzGoToFilePos(m_zip,(unz_file_pos*)&pos) == UNZ_OK)
				{
					GetFileInfo();
					return true;
				}
			}
			return false;
		}

		string_type const& file_name(){return m_strFileName;}
		int file_size()const{return m_FileInfo.uncompressed_size;}
		int file_size_comressed()const{return m_FileInfo.compressed_size;}
        
		bool open_file(const char_type* password)
		{
			if(m_zip && !m_bFileOpened)
			{
				if(unzOpenCurrentFilePassword(m_zip,detail::WideToStr(password)) == UNZ_OK)
				{
					m_bFileOpened = true;
					return true;
				}
			}
			return false;
		}

		bool open_file()
		{
			if(m_zip && !m_bFileOpened)
			{
				if(unzOpenCurrentFile(m_zip) == UNZ_OK)
				{
					m_bFileOpened = true;
					return true;
				}
			}
			return false;
		}

		bool close_file()
		{
			if(m_zip && m_bFileOpened)
			{
				if(unzCloseCurrentFile(m_zip) == UNZ_OK)
				{
					m_bFileOpened = false;
					return true;
				}
			}
			return false;
		}

		ulong read(void* pBuffer,int iNum)
		{
			if(m_zip && m_bFileOpened) return unzReadCurrentFile(m_zip,pBuffer,iNum);
			return 0;
		}

		ulong pos()
		{
			if(m_zip && m_bFileOpened) return unztell(m_zip);
			return 0;
		}

		unzFile handle(){return m_zip;}

		bool valid()const{return m_zip != 0;}

		bool opened()const{return m_bFileOpened;}

		bool file_oef()const
		{
			if(m_zip && m_bFileOpened) return unzeof(m_zip) == 1;
			return true;
		}

	};
}
