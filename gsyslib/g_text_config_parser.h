#pragma once

#include "g_gsys.h"
#include "g_text.h"
#include "g_file.h"

#include "g_file_istream.h"

namespace gsys
{
	class text_config_parser
	{
		// read keyword from stream 
		class istream_config_keyword
		{
			string* m_pStr;

			static bool good_char(char ps)
			{
				if(char_is_alpha(ps))return true;
				if(char_is_numb(ps))return true;
				if(ps == ':')return true;
				if(ps == '.')return true;
				if(ps == '-')return true;

				return false;
			}

		public:
			istream_config_keyword(string & str) : m_pStr(&str){}
			friend inline std::istream& operator>>(std::istream& is,istream_config_keyword & isk)
			{
				char ps;
				if(is.fail())return is;
				isk.m_pStr->clear();
				ps = is.peek();
				if(!good_char(ps)){/*is.setstate( std::ios::failbit );*/return is;}
				(*isk.m_pStr) += ps;
				is.ignore();
				while(!is.fail())
				{
					ps = is.peek();
					if(!good_char(ps))break;
					(*isk.m_pStr) += ps;
					is.ignore();
				}
				//is >> wsp_scip; // skip tailing spaces
				return is;	
			}
		};


	private:
		igfilestream m_igfilestream;
		std::istream* is;
		bool m_bProcessed;

		string m_str;
		bool m_bBlock;

		bool m_bUTF8;

	public:

		text_config_parser()
		{
			is=0;
			m_bProcessed=true;
			m_bBlock=false;
			m_bUTF8=false;
		}

		// operations
		bool parce(gfile* pFile)
		{
			m_bProcessed = true;
			m_bBlock = false;
			is = &m_igfilestream;
			if(!m_igfilestream.init(pFile))return false;

			if(is->peek() == 0xEF) // Support UTF 8 marker EF BB BF
			{
				char crs[4]={0,0,0,0};
				is->get(crs,4);
				if(*(unsigned*)crs == 0x00BFBBEF)m_bUTF8 = true;
				else return false; 
			}
			return true;
		}
		bool parce(std::istream* pis)
		{
			is = pis;
			return true;
		}
		bool is_utf8()const{return m_bUTF8;}

		bool record(string & strItem,bool& section)
		{
			section = false;
			
			wsp_delim OB('[');
			wsp_delim CB(']');
			wsp_delim TLD('~');
			wsp_delim EQ('=');
			
			istream_config_keyword Item(strItem);
			
			if(m_bBlock)
			{
				string tmpstr;
				block(tmpstr);
			} 
			m_bBlock = false;
			// scip unknown line
			if(!m_bProcessed)	*is >>	istream_scip_line >> wsp_nl_scip;
			m_bProcessed = false;
			*is >> ws;
			if(is->peek() == '[') // block
			{			
				*is>> OB >> Item >> CB >> wsp_nl_scip;
				section = true;
				m_bProcessed = true;
				return !is->fail();
			}
			*is>> Item >> ws;
			
			char ps = is->peek();
			if(ps == '~') // block data
			{
				*is >> TLD >> EQ >> m_str;
				*is >> istream_scip_line;
				m_bBlock = true;
			}
			else if(ps == '=') // normal data
			{
				*is >> EQ;
			}
			return !is->fail();
		}
		template<typename T>
		bool read(T & rObject)
		{
			m_bProcessed = true;
			*is >> rObject >> istream_scip_line;
			m_bProcessed = !is->fail();
			return m_bProcessed;
		}
		bool line(string & strLine)
		{
			istream_line isLine(strLine);
			
			*is >> isLine >> istream_scip_line;
			int pos = strLine.find_last_not_of(" \t");
			strLine.erase(pos+1,strLine.length());
			m_bProcessed = !is->fail();
			gsys::unquote(strLine);
			return m_bProcessed;
		}
		bool keyword(string & strKeyword)
		{
			istream_numb_keyword isKeyword(strKeyword);
			
			*is >> isKeyword >> istream_scip_line;
			m_bProcessed = !is->fail();
			return m_bProcessed;
		}
		bool block(string & strBlock)
		{
			if(!m_bBlock) return line(strBlock);
			m_bBlock = false;
			strBlock.clear();
			string strTmp;

			if(!line(strTmp))return false;
			strBlock = strTmp;
			while(true)
			{
				if(!line(strTmp))return false;
				if(strTmp == m_str)return true; // end of block 
				strBlock+= "\n"+strTmp;
			}
		}
	};
}


