
#pragma once
#include "g_gsys.h"
#include "g_fdir.h"
#include "g_std_file.h"
#include "g_file_util.h"

#define BOOST_FILESYSTEM_VERSION 3
#define  BOOST_FILESYSTEM_NO_LIB
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/exception.hpp"

#include <boost/regex.hpp> 

namespace gsys
{
	class std_fdir : public gfdir
	{
		typedef boost::filesystem::path path_type;

		typedef boost::basic_regex<char_type, boost::regex_traits<char_type> > regex_type;

		path_type m_pathBase;
	public:
		std_fdir()
		{
			//m_pathBase. = "";
		};

		explicit std_fdir(string_type const& dir){open(dir.c_str());};

		string_type get_actual_path(char_type const* fn)
		{
			return (m_pathBase/fn).normalize().native();
		}

		bool open(string_type  const& dir){return open(dir.c_str());};
		bool open(char_type const* dir)
		{
			m_pathBase = L"";
			path_type p(dir);
			if(boost::filesystem::exists(p))
			{
				m_pathBase = p;
				return true;
			}
			return false;
		}
	
		std_fdir* create_fdir(char_type const* name)
		{
			if(!name || !name[0])return 0;
			path_type p(name);
			//p = p.relative_path();
			p = (m_pathBase/p);

			if(!boost::filesystem::exists(p))
			{
				boost::filesystem::create_directory(p);
			}

			string_type fn;
			try{fn = p.native();}
			catch(boost::filesystem::filesystem_error){return gsys_null;}
			std_fdir* f = new std_fdir();
			if(f->open(fn))return f;
			delete f;
			return gsys_null;
		}	

		gfile* create(char_type const* name)
		{
			if(!name || !name[0])return 0;
			path_type p(name);
			//p = p.relative_path();
			p = (m_pathBase/p);

			if(!boost::filesystem::exists(p))
			{
				path_type dir(ExtractFilePath(p.string()));
				if(!boost::filesystem::exists(dir))
					boost::filesystem::create_directory(dir);
			}
			gfile* file = new std_file(p.native(),gfile::ModeCreate);

			if(!*file)
			{
				delete file;
				return gsys_null;
			}
			return file;
		}

		void find_file_vt(string_type const& base,string_type const& pal,std::list<string_type> &vt)
		{return find_file_vt(base.c_str(),pal.c_str(),vt);}
		void find_file_vt(string_type const& pal,std::list<string_type> &vt)
		{return find_file_vt(0,pal.c_str(),vt);}
		void find_file_vt(char_type const* pal,std::list<string_type> &vt)
		{return find_file_vt(0,pal,vt);}

		void find_file_vt(char_type const* base,char_type const* pal,std::list<string_type> &vt)
		{
			std::list<string_type> dirlist;
			find_file_vt(base,pal,vt,dirlist);
		}

		void find_file_vt(char_type const* base,char_type const* pal,std::list<string_type> &vt,std::list<string_type> &dirlist)
		{
			vt.clear();
			try
			{
				regex_type e(pal);

				path_type p(m_pathBase);
				path_type pbase;
				if(base)
				{
					p/=base;
					pbase=base;
				}

				boost::filesystem::directory_iterator itr(p);
				boost::filesystem::directory_iterator end_itr;

				for(;itr != end_itr;++itr)
				{
					path_type fn = itr->path().filename();
					if(!boost::filesystem::is_directory(*itr))
					{
						// test fn to mask
						if(boost::regex_match(fn.generic_string<string_type>(),e)) 
						{
							// we found it 
							if(base)vt.push_back((pbase/fn).native());
							else vt.push_back(fn.generic_string<string_type>());
						}
					}
					else
					{
						if(base)dirlist.push_back((pbase/fn).native());
						else dirlist.push_back(fn.generic_string<string_type>());
					}
				}
			}
			catch(boost::bad_expression)
			{
				vt.clear();
			}
			catch(boost::filesystem::filesystem_error)
			{
				vt.clear();
			}
		}

	public:
		bool exists(char_type const* name)
		{
			return boost::filesystem::exists(m_pathBase/name);
		}
		bool valid()
		{
			return !m_pathBase.empty();
		}

   		gfile* get_file(char_type const* name,gfile::FileMode mode = gfile::ModeRead)
		{
			string_type fn;
			try
			{
				path_type p(name);
				fn = (m_pathBase/p).native();
			}
			catch(boost::filesystem::filesystem_error){return gsys_null;}

			std_file* f = new std_file(fn,mode);
			if(f->valid())return f;
			delete f;
			return gsys_null;
		}

		std_fdir* get_fdir(string_type const& name){return get_fdir(name.c_str());}
		std_fdir* get_fdir(char_type const* name)
		{
			string_type fn;
			try
			{
				path_type p(name);
				fn = (m_pathBase/p).native();
			}
			catch(boost::filesystem::filesystem_error){return gsys_null;}
			std_fdir* f = new std_fdir();
			if(f->open(fn))return f;
			delete f;
			return gsys_null;
		}
	};
}

