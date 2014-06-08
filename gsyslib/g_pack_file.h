#pragma once

#include "g_gsys.h"
#include "g_file.h"
#include "g_mem_file.h"

namespace gsys
{
	namespace detail 
	{
		struct pack_file_header
		{
			dword dwFileId;
			dword dwNumEntries;
			dword dwDirectorySize;
			dword dwCompresedDirectorySize;
		};

		struct pack_file_entry
		{
			dword dwDataSize;
			dword dwCompressedDataSize;
			dword dwOffset;
			word wNameSize;
			gsys_byte bFlags;
			gsys_byte bCompress;
		};
	}

	class pack_file
	{
	public:
		typedef size_t size_type;
	protected:
		map<string,detail::pack_file_entry> m_mapDirectory;
		std_file m_File; // file for reading
	public:
		bool valid()const{return m_File.valid();}
		// init pack file with filename 
		bool init(string const& filename)
		{
			m_File.open(filename.c_str(),"rb");
			if(!m_File) return false;

			detail::pack_file_header Head; 

			m_File >> Head;

			// read file's directory
			for(uint i=0;i<Head.dwNumEntries;i++)
			{
				detail::pack_file_entry Entry;

				m_File >> Entry;

				raw_data_buffer<char,0,8> buf;
				buf.reset(Entry.wNameSize);
				m_File.tread(buf.get(),buf.size());

				string str;
				str.assign(buf.get(),buf.size());

				m_mapDirectory[str] = Entry;
			}			
			return true;
		}

		// gets file given by name from pack 
		gfile* get_file(string const& filename)
		{	
			map<string,detail::pack_file_entry>::iterator it = 
										m_mapDirectory.find(filename);

			if(it == m_mapDirectory.end())return 0;
			detail::pack_file_entry &Entry = it->second;
			m_File.set_pos(Entry.dwOffset);

			// get file 
			mem_file* MemFile = new mem_file();
			MemFile->init(Entry.dwDataSize);

			if(Entry.bCompress == 0)
				m_File.tread(MemFile->buffer(),Entry.dwDataSize);
			else
			{
				

			}
			return MemFile;
		}

	};

	class pack_file_builder
	{
	public:
		typedef size_t size_type;
	protected:
   		 list<string> m_lstFiles;
	public:
		void add_file(string const& file)	
		{
			m_lstFiles.push_back(file);
		}
		void build(string const& strFileName)
		{
			size_type szDirectorySize=0;

			list<size_type> lstSizes;
			// at first build directory
			_foreach(list<string>,m_lstFiles,it)
			{
				std_file File(it->c_str(),"rb");
				if(File)
				{
					lstSizes.push_back(File.size());

					// calculate directory size
					szDirectorySize += sizeof(detail::pack_file_entry); 
					szDirectorySize += it->length(); 
				}
				else
					it = m_lstFiles.erase(it);
			}
			size_type szOffset=0;


			std_file PackFile(strFileName.c_str(),"wb");

			detail::pack_file_header Head;
			Head.dwNumEntries = (uint)m_lstFiles.size();
			Head.dwDirectorySize = (uint)szDirectorySize;
			Head.dwCompresedDirectorySize = (uint)szDirectorySize;

			PackFile << Head; // write header

			szOffset = sizeof(Head) + szDirectorySize;

			list<string>::iterator i;
			list<size_type>::iterator j;

			i = m_lstFiles.begin();
			j = lstSizes.begin();
			// write directory
			while(i != m_lstFiles.end())
			{
				detail::pack_file_entry Entry;

				Entry.bCompress = 0;
				Entry.bFlags = 0;
				Entry.wNameSize = (word)i->length();
				Entry.dwOffset = (dword)szOffset;
				Entry.dwDataSize = (dword)*j;
				Entry.dwCompressedDataSize = (dword)*j;

				PackFile << Entry;
				PackFile.twrite(i->data(),(size_type)i->length());

				szOffset += *j;

				i++;
				j++;
			}
			// write files
			i = m_lstFiles.begin();
			j = lstSizes.begin();

			raw_data_buffer<gsys_byte> Buffer;

			while(i != m_lstFiles.end())
			{
				std_file File(i->c_str(),"rb");
				Buffer.reset((size_type)*j);
                File.tread(Buffer.get(),Buffer.size());
				PackFile.twrite(Buffer.get(),Buffer.size());
				i++;
				j++;
			}
   		}
	};
}
