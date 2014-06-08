#pragma once
#include "g_text_intended_parser.h"

namespace gsys
{
	template <class Elem, class Tr = basic_char_classify<Elem> >
	class basic_istream_py_quoted :public detail::base_istream_reader<Elem,Tr>
	{
		string_type* m_pStr;
		bool ml;
	public:
		basic_istream_py_quoted(string_type & str,bool multyline=false) : m_pStr(&str),ml(multyline){}
		friend inline istream_type& operator>>(istream_type& is,basic_istream_py_quoted<Elem,Tr> & isk)
		{
			const char_type etsc = chars::etscape();
			char_type qs=0;
			char_type ps;
			if(is.fail())return is;
			isk.m_pStr->clear();

			char_type qi = is.peek();

			qs = chars::quote_pair(qi);
			if(!qs){is.setstate(std::ios::failbit);return is;}
			is.ignore();

			unsigned qcount=1;

			bool triple_quoted=false;

			if(qi == qs && !isk.ml)
			{
				ps = is.peek();
				if(ps == qi) 
				{
					is.ignore();
					ps = is.peek();
					if(ps == qi) // triple quote string
					{
						triple_quoted = true;
						is.ignore();
					}
					else return is; // empty string
				}
			}

			while(true)
			{
				if(is.eof()){is.setstate(std::ios::failbit);return is;}
				ps = is.peek();
				if(chars::newline(ps) && !isk.ml)
				{
					if(triple_quoted)
					{
						is.ignore();
						char_type nps = is.peek();
						if(chars::newline(nps) && nps != ps)is.ignore();
						(*isk.m_pStr) += etsc;
						(*isk.m_pStr) += 'n';
						continue;
					}
					is.setstate(std::ios::failbit);return is;
				}
				else if(ps == etsc)
				{
					is.ignore();
					char_type next = is.get();
					if(!chars::newline(next))
					{
						//(*isk.m_pStr) += chars::decode_cescaped_char(next);
						(*isk.m_pStr) += etsc;
						(*isk.m_pStr) += next;
						continue;
					}
					else
					{
						char_type nps = is.peek();
						if(chars::newline(nps) && nps != next)is.ignore();
						continue;
					}
				}
				else if(ps == qs)
				{
					qcount--;
					if(!qcount)
					{
						is.ignore();
						if(triple_quoted)
						{
							ps = is.peek();
							if(is.peek() != qs)
							{
								(*isk.m_pStr) += etsc;
								(*isk.m_pStr) += qs;
								qcount++;
								continue;
							}
							else
							{
								is.ignore();
								ps = is.peek();
								if(is.peek() != qs)
								{
									(*isk.m_pStr) += etsc;
									(*isk.m_pStr) += qs;
									(*isk.m_pStr) += etsc;
									(*isk.m_pStr) += qs;
									qcount++;
									continue;
								}
								else 
								{
									is.ignore();
									break;
								}
							}
						}
						else break;
					}
				}				
				else if(ps == qi)
				{
					qcount++;
				}
				(*isk.m_pStr) += is.get();
			}
			//is >> wsp_scip; // scip tailing spaces
			return is;	
		}
	};
	typedef basic_istream_py_quoted<char> istream_py_quoted;
	typedef basic_istream_py_quoted<wchar_t> wistream_py_quoted;

	std::istream& GSYS_INC_API text_intended_paser::nl_scip(std::istream& is)
	{
		char ps;
		if(is.fail())return is;
		while(!is.eof())
		{
			ps = is.peek();
			if(!char_is_nl(ps)) break;
			if(ps == '\n')m_iCurrentLine++;
			is.ignore();
		}
		return is;	
	}

		std::istream& GSYS_INC_API text_intended_paser::wsp_scip(std::istream& is)
		{
			while(1)
			{
				char pc = is.peek();
				if(char_is_wsp(pc))
				{
					is.ignore();
					continue;
				}
				if(pc == cCommentChar)
				{
					is >> istream_scip_till_nl;
				}
				break;
			}
			return is;
		}


	namespace nms_text_intended_paser
	{
		void GSYS_INC_API read_value(istream& is, string& value)
		{
			value.clear();
			wsp_scip(is);
			while(1)
			{
				string str;
				istream_line_till_first_of ln(str,":#'\"");
				is >> ln;
				value += str;

				char ps = is.peek();

				if(ps == '#')
				{
					is >> istream_scip_line;
					break;
				}
				else if(ps == ':')
				{
					break;
				}
				else if(char_classify::quote_pair(ps)) // quote
				{
					is >> istream_py_quoted(str);					
					value += ps + str + char_classify::quote_pair(ps);
				}
				else if(char_is_nl(ps))
				{
					if(!value.empty() && *(value.end()-1) == '\\') // ended with backslash so continue line 
					{
						nl_scip(is);
						value.erase(value.end()-1);
					}
					else break; // end of block
				}
				else
				{
					break;
				}
			}
		}
	}
	

	int GSYS_INC_API text_intended_paser::read_intend(istream& is)
	{
		int i=-1;

		while(1)
		{
			istream_wsp_count wsc(i,m_iTbl);
			is >> wsc;
			char pc = is.peek();
			if(is.eof()) return -1;
			if(!char_is_nl(pc))
			{
				if(pc == cCommentChar)
				{
					is >> istream_scip_line;
					m_iCurrentLine++;
				}
				else return i;
			}
			nl_scip(is);
		}
	}

	bool GSYS_INC_API text_intended_paser::GetRecord(string &name, string& data, bool &block)
	{
		string strTemp;

		istream& is = *m_pStream;

		string l_keyword,l_data;

		if(m_vtIntends.size() == 0)
		{
			// first time execute
			m_vtIntends.push_back(-1); // fill default block with neg intend

			m_bSingelineBlock=false; 
			m_bBlock = false;
			
			m_iIntend = read_intend(is);// pre read intend
		}

		if(m_bSingelineBlock && !m_bEnterBlock)
		{
			strTemp.clear();
			// read till :
			{
				nms_text_intended_paser::read_value(is,strTemp);
				char ps = is.peek();
				if(ps == ':')
				{
					is.ignore();
					wsp_scip(is);
					if(char_is_nl(is.peek())) 
					{
						m_bSingelineBlock = false;
						m_vtIntends.push_back(m_iIntend);
					}
				}
			}
			nl_scip(is);
			m_iIntend = read_intend(is);// pre read intend
		}

		if(!m_bSingelineBlock)
		{
			if(m_bBlock && !m_bEnterBlock)
			{
				// scip all blocks in existens
				while(1)
				{
					if(m_iIntend > m_vtIntends.back())
					{
						istream_line  il(strTemp);
						is >> il;
						nl_scip(is); // next line

						m_iIntend = read_intend(is);
					}
					else break;
				}
				if(m_vtIntends.size()>1)m_vtIntends.pop_back();
			}

			// test intend
			if(m_iIntend <= m_vtIntends.back())
			{
				// exit current level
				m_vtIntends.pop_back();
				return false;
			}
		}

		m_bBlock = false;
		m_bEnterBlock = false;
		m_bSingelineBlock = false;

		m_iKeywordLine = m_iCurrentLine;

		istream_keyword iskeyword(l_keyword);
		//istream_line_till_first_of ltfo(l_data,":");

		if(char_is_alpha(is.peek())) // read keyword
		{
			is >> iskeyword;
		}
		// parce current line
		nms_text_intended_paser::read_value(is,l_data);
		//wsp_scip(is) >> ltfo;

		if(is.peek() == ':') // that is block
		{
			m_bBlock = true;
			is.ignore();
			wsp_scip(is);
			if(!char_is_nl(is.peek())) // end of line
			{
				m_bSingelineBlock=true;
				// this is singleline block
			}
			else m_bSingelineBlock = false;
		}
		else 
		{
			m_bSingelineBlock = false;
		}

 		if(!m_bSingelineBlock)
		{
			nl_scip(is);
			// read next intend
			int nwsp = read_intend(is);

			if(nwsp <= m_iIntend)
			{
				m_bBlock = false;
			}
			else if(nwsp > m_iIntend)
			{
				// record new block if in clock mode
				if(m_bBlock)
				{
					m_vtIntends.push_back(m_iIntend);
				}
			}
			m_iIntend = nwsp;
		}
		else m_vtIntends.push_back(m_iIntend);


		name = l_keyword;
		data = l_data;
		block = m_bBlock;

		return true;
	}

}