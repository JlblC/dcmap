#pragma once
#include "g_gsys.h"
#include "g_text.h"

#include "g_file.h"
#include "g_file_istream.h"

namespace gsys
{
	class text_intended_paser: public noncopyable
	{
	private:
		istream* m_pStream;
		
		// how many spaces in one tab
		int m_iTbl;

		int m_iCurrentLine;
		int m_iKeywordLine;

		bool m_bSingelineBlock;
		bool m_bBlock;
		bool m_bEnterBlock;
		int m_iIntend;


		vector<int> m_vtIntends;
	
		int read_intend(istream& is);
		std::istream& nl_scip(std::istream& is);
		std::istream& wsp_scip(std::istream& is);

		static const char cCommentChar = '#';

	public:
		text_intended_paser(){}
		explicit text_intended_paser(istream* pStream){init(pStream);}
		void init(istream* pStream)
		{
			m_pStream = pStream;
			m_vtIntends.clear();
			m_iCurrentLine = 1;
			m_iKeywordLine = 1;
			m_iTbl = 4;
		}

		// name - token name
		// data - token data
		// level:  "-1" end block "0" simple record "1" record with subblock
        bool GetRecord(string &name, string& data, bool &block);

		// enter to prosess subblock
		void EnterSubblock()
		{
			if(m_bBlock) m_bEnterBlock = true;
		}

		int cline()const
		{
			return m_iKeywordLine;
		}
	};
}

#ifdef GSYSDETAIL_FORCE_INLINE
#	include "src/inc/g_text_intended_parser.hpp"
#endif

