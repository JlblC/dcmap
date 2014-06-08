
#pragma once
#include "g_gsys.h"
#include "g_fdir.h"
#include "g_mem_file.h"

#include "g_hash_map.h"
#include "g_zip_archive.h"

namespace gsys
{
	class zip_fdir;
	namespace detail
	{
		class zipped_file : public gfile
		{
			friend class ::gsys::zip_fdir;

			zipped_file(){}

			void Detach()
			{
				int cp = pos();
				int cs = size();

				m_pZip->close_file();
				m_pZip->open_file();

				m_memfile.init(cs);
				m_pZip->read(m_memfile.buffer(),cs);
				m_memfile.set_pos(cp);


				m_pZip->close_file();
				m_pZip = 0;
			}
			bool Attach(zip_archive* pZip)
			{
				if(pZip->open_file())
				{
					m_pZip = pZip;
					return true;
				}
				return false;
			}

			void close()
			{
				if(m_pZip)
				{
					m_pZip->close_file();
					m_pZip = 0;
				}
				else
				{
					m_memfile.init(0);
				}
			}

		protected:
			zip_archive* m_pZip;

			mem_file m_memfile;


			size_type seek_forward(int ref)
			{
				char buf[64];
				int readed = 0;
				int r;
				while(ref != readed)
				{
					if((ref-readed) > 64) r = 64;
					else r = ref-readed;

					r = m_pZip->read(buf,r);
					if(r>0)
						readed+= r;
					else return pos(); // error or eof
				}
				return pos();
			}

		public:

			~zipped_file()
			{
				close();
			}

			size_type read(void* pBuffer, size_type iSize, size_type iCount = 1)
			{
				if(m_pZip)
				{
					GSYS_ASSERTPTR(m_pZip);
					uint r = m_pZip->read(pBuffer,iSize*iCount);
					if(r < iSize*iCount)
					{
						size_type dif = iSize*iCount - r;
						iCount -= (dif / iSize);
					}
					return iCount;
				}
				
				return m_memfile.read(pBuffer,iSize,iCount);
			}
			size_type write(void const* pBuffer, size_type iSize, size_type iCount = 1)
			{
				return 0;
			}
			size_type seek(long lOffset, FileSeek iOrigin)
			{
				if(m_pZip)
				{
					size_type cp = pos();
					size_type cs = size();

					int ref=0;
					switch(iOrigin)
					{
					case SeekSet:
						ref = lOffset-cp;
						break;
					case SeekCur:
						ref = lOffset;
						break;
					case SeekEnd:
						ref = (cs-lOffset)-cp;
						break;
					}
					if(cp+ref > cs) ref = cs-cp;
					//if(cp+ref < 0) ref = -1*cp; // true always

					if(ref >0) // seek forward
					{
						if(ref > cs*(2.0/3.0) || cs-ref)
						{
							Detach();
							return seek(lOffset,iOrigin);
						}
						return seek_forward(ref);
					}
					else if(ref < 0) // seek backward
					{
						// test what better - reopen file or switch to memory buffer
						if( cs >= 255 && cp <= cs/2)
						{
							m_pZip->close_file();
							m_pZip->open_file();
							seek_forward(cp+ref);
						}
						else
						{
							Detach();
							return seek(lOffset,iOrigin);
						}
					}
				}
				return m_memfile.seek(lOffset,iOrigin);
			}
			size_type size()
			{
				if(m_pZip)
				{
					return m_pZip->file_size();
				}
				return m_memfile.size();
			}
			size_type pos()
			{
				if(m_pZip)
				{
					GSYS_ASSERTPTR(m_pZip);
					return m_pZip->pos();
				}
				return m_memfile.pos();
			}
			void flush()
			{
			}

			bool valid()const
			{
				if(m_pZip)
				{
					return true;
				}
				return m_memfile.valid();
			}
			bool eof()const
			{
				if(m_pZip)
				{
					return m_pZip->file_oef();
				}
				return m_memfile.eof();
			}

		};

	}

	class zip_fdir : public gfdir
	{
	protected:
		typedef strimap<string_type,zip_archive::file_pos> FileDir;

		zip_archive m_zip;

		detail::zipped_file* m_pCurrentFile;

		FileDir m_mapFiles;

	private:

		void ReadZipDir()
		{
			if(m_zip.goto_first_file())
			{
				do
				{
					m_mapFiles[m_zip.file_name()] = m_zip.get_file_pos();
				}while(m_zip.goto_next_file());
			}
		}

	public:
		zip_fdir():m_pCurrentFile(0){}
		~zip_fdir(){close();}

		explicit zip_fdir(char_type const* dir):m_pCurrentFile(0)
			{open(dir);};
		explicit zip_fdir(gfile* file):m_pCurrentFile(0)
			{open(file);};
		explicit zip_fdir(string_type const& dir):m_pCurrentFile(0)
			{open(dir.c_str());};

		bool open(string_type  const& dir){return open(dir.c_str());};

		bool open(char_type const* file)
		{
			close();
			if(m_zip.open(file))
			{
				ReadZipDir();
				return true;
			}
			return false;
		}

		bool open(gfile* file)
		{
			close();
			if(m_zip.open(file))
			{
				ReadZipDir();
				return true;
			}
			return false;
		}

		void close()
		{
			m_mapFiles.clear();

			if(m_pCurrentFile && m_zip.opened())
			{
				m_pCurrentFile->Detach();
			}
			m_pCurrentFile = 0;

			m_zip.close();
		}
	public:
		bool exists(char_type const* name)
		{
			return m_mapFiles.find(name) != m_mapFiles.end();
		}
		bool valid()
		{
			return m_zip.valid();
		}

   		gfile* get_file(char_type const* name,gfile::FileMode mode = gfile::ModeRead)
		{
			if(mode != gfile::ModeRead) return gsys_null;

			if(m_pCurrentFile)
			{
				if(m_zip.opened())
				{
					m_pCurrentFile->Detach();
				}
				m_pCurrentFile = 0;
			}

			FileDir::iterator i = m_mapFiles.find(name);
			if(i != m_mapFiles.end())
			{
				if(m_zip.select_file(i->second))
				{
					detail::zipped_file* file = new detail::zipped_file;
					if(file->Attach(&m_zip))
					{
						m_pCurrentFile = file;
						return file;
					}
				}
			}
			return gsys_null;
		}
	};
}


