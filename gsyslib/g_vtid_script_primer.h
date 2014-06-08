#pragma once

#include "g_gsys.h"

#include "g_text_intended_parser.h"
#include "g_file_istream.h"

#include "g_vtid_lib.h"
#include "g_vtid_var.h"

namespace gsys{namespace vtid{
	class script_primer
	{
		script_primer(void);
		~script_primer(void);

		bool prime(gsys::gfile* file);
		bool stop();
		void read(gsys::vtid_ref const &cs);

		bool record(string &line,string &name, string &type,
					string &param,string &value,bool &block);

		void EnterBlock();
	};
}};





