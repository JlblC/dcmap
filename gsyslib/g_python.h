#pragma once
#include "g_gsys.h"

#ifdef GSYS_ENABLE_PYTHON // disable python

#include "g_std_file.h"
#include "g_file_util.h"


#include "boost/python.hpp"

namespace gsys
{
	namespace py
	{	
		//using namespace std;
		using namespace boost::python;

		void py_export_gsys();

		void Initialize();
		void __inline  Finalize(){Py_Finalize();}


		class NameSpace : public dict
		{
		public:
			NameSpace() 
			{
			}
			NameSpace(handle<> h) : dict(h) {}
		};
		class GlobalNameSpace : public NameSpace
		{
		public:
			GlobalNameSpace(): 
			  NameSpace(handle<>(borrowed(PyModule_GetDict(PyImport_AddModule("__main__")))))
			{
				UseModule("__builtin__");
			}
			GlobalNameSpace(handle<> h) : NameSpace(h) {}

			void ImportModule(std::string mname)
			{
				handle<>((PyRun_String((char*)(std::string("import ")+mname).c_str(),
					Py_file_input,ptr(),ptr())));
			}
			void UseModule(std::string mname)
			{
				ImportModule(mname);
				handle<>(PyRun_String((char*)(std::string("from "+mname+" import *")).c_str(),
					Py_file_input,ptr(),ptr()));
			}
		};


		class ExcutionSpace
		{
			NameSpace m_dictGlobals;
			NameSpace m_dictLocals;
		public:
			ExcutionSpace(NameSpace const& glob) : 
				m_dictGlobals(glob)
			{
			}
			void Execute(char* cmd)
			{
				handle<>(PyRun_String(cmd,Py_file_input,m_dictLocals.ptr(), m_dictGlobals.ptr()));
			}
			void ExecuteFile(const char* fn)
			{
				std_file file(fn,std_file::ModeRead);
				if(file)
				{
					int sz = file.size();
					char* bf = new char[sz+1];
					file.read(bf,sz);
					bf[sz]=0;
					// remove /r from string
					char* pt=bf;
					while(*pt){if(*pt=='\r')*pt='\n';pt++;}
					handle<>(PyRun_String(bf,Py_file_input,m_dictLocals.ptr(), 
						m_dictGlobals.ptr()));
					delete[] bf;
				}
			}
		};


		class module
		{
			handle<> m_handle;
			module(handle<> h){m_handle = h;}

			friend module __inline AddModule(std::string name);
            
		public:
			module(){}
			dict Dict()
			{
				return dict(borrowed(PyModule_GetDict(m_handle.get())));
			}
			PyObject* get(){return m_handle.get();}
		};


		module __inline AddModule(std::string name)
			{return module(handle<>(borrowed(PyImport_AddModule(const_cast<char*>(name.c_str())))));}


		class script
		{
			NameSpace m_dictGlobals;
			NameSpace m_dictLocals;
			std::string m_strSource;
		public:
			NameSpace& locals(){return m_dictLocals;}

			void init_from_file(char const* filename, NameSpace const& glob)
			{
				std_file file(filename,std_file::ModeRead);
				read_text_file(file,m_strSource);
				m_dictGlobals = glob;
			}
			void init(char* source, NameSpace const& glob)
			{
				m_dictGlobals = glob;
				m_strSource = source;
			}

			void exec()
			{
				handle<>(PyRun_String((char*)m_strSource.c_str(),
					Py_file_input,m_dictLocals.ptr(),m_dictGlobals.ptr()));
			}
		};

	};
};

#endif // GSYS_ENABLE_PYTHON
