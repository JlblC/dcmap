#pragma once
#include "g_gsys.h"
#include "g_data_buffer.h"
#include "g_file.h"

namespace gsys
{
	enum {FILE_MODE_TEXT,FILE_MODE_BIN};

	namespace detail
	{
		class records_file : noncopyable
		{
		public:
			typedef gsys::gsys_ulong size_type;
		protected:
			gfile* m_pFile; // file to use
			bool m_bFileEnd;
			uint   m_wCurrentIntend;
			struct entry_header
			{
				word wDataSize; // data size
				gsys_byte bIntend; // intend value
				gsys_byte bNameLen; // size of value name
			}; // follows with name & data
		public:
			records_file():m_pFile(0),m_bFileEnd(true),m_wCurrentIntend(0){}
			void AssignFile(gfile* file=0)
			{GSYS_CFN;
				if(file) GSYS_ASSERT(*file);
				m_pFile = file;
				m_bFileEnd = false;
				m_wCurrentIntend = 0;
			}
		};
	}
	class records_file_writer : public detail::records_file
	{
	public:
		records_file_writer()
		{GSYS_CFN;
			
		}
		/////// Write Methods
		void write(char const* name, void const* pData,size_type szSize)
		{GSYS_CFN;/////////////////////////// check environment
			GSYS_ASSERTOBJ(m_pFile);
			GSYS_ASSERT(*m_pFile);
			GSYS_ASSERTSTR(name);
			////////////////////////////////////////////////////
			entry_header head;
			head.bNameLen = (gbyte)strlen(name);
			head.wDataSize = (word)szSize;
			head.bIntend = (gbyte)m_wCurrentIntend;
			m_pFile->twrite(&head); // write header
			m_pFile->twrite(name,head.bNameLen); // write name
			if(szSize)
			{
				GSYS_ASSERTPT(pData,szSize);
				m_pFile->write(pData,szSize); // write data
			}
		}
		void begin_block(){GSYS_CFN;m_wCurrentIntend++;}
		void end_block(){GSYS_CFN;m_wCurrentIntend--;}

		template<class T> 
		void write(char const* name, T const& Data){write(name,&Data,sizeof(Data));}
	};

	class records_file_reader : public detail::records_file
	{

	private:
		entry_header m_CurrentEntry;
		raw_data_buffer<gsys_byte> m_pData;
		raw_data_buffer<char> m_pName;
		bool m_bPreRead;
	public:
		records_file_reader():
			m_bPreRead(false)
			{}

		void enter_block(){GSYS_CFN;m_wCurrentIntend++;}
		void exit_block(){GSYS_CFN;m_wCurrentIntend--;}
		
		bool next_record() // Reads next record from file
		{GSYS_CFN;
			GSYS_ASSERTOBJ(m_pFile);
			GSYS_ASSERT(*m_pFile);
			if(m_bFileEnd)return false;
			///////////////////////////////////////
			if(!m_bPreRead)
			do // read entries scip unwanted intend
			{
				if(!m_pFile->tread(&m_CurrentEntry))
				{
					m_bFileEnd = true;
					return false;
				}

				// TODO: Replace this with buffer container
				m_pName.reset(m_CurrentEntry.bNameLen+1);
				m_pData.reset(m_CurrentEntry.wDataSize);

				m_pFile->read(m_pName.get(),m_CurrentEntry.bNameLen);
				m_pFile->read(m_pData.get(),m_CurrentEntry.wDataSize);

				// terminate string with 0
				m_pName[m_CurrentEntry.bNameLen]=0;
			}
			while(m_CurrentEntry.bIntend > m_wCurrentIntend); 
			if(m_CurrentEntry.bIntend < m_wCurrentIntend) 
			{
				m_bPreRead = true;
				return false;
			}
			m_bPreRead = false;
			return true;
		}
		char const* record_name()const{return m_pName.get();}
		size_type record_size()const{return m_pData.size();}
		void const* record_data()const{return m_pData.get();}
		int record_mode(){return FILE_MODE_BIN;}
	};

};



