#pragma once
#include "g_gsys.h"

namespace gsys
{
	namespace detail
	{
		typedef void* (GSYS_CALLBACK *vtid_class_factory_proc)(void* ptr);

		typedef  std::type_info const& (GSYS_CALLBACK *vtid_type_proc)(void); 

		// common operations 

		struct vtid_operations
		{
			typedef void (GSYS_CALLBACK *to_str)(void* ptr,std::string &str); 
			typedef void (GSYS_CALLBACK *from_str)(void* ptr,std::string const &str); 
			typedef void (GSYS_CALLBACK *copy_obj)(void* ptr,void* src); 

			to_str h_to_str;
			from_str h_from_str;
			copy_obj h_copy_obj;

			vtid_operations():h_to_str(0),h_from_str(0){}
		};


		struct vtid_decl_source
		{
			// Name of data member
			std::string strName;
			// autogenerated name of contained type
			std::string strTypeName;
			// offset of data member from begin of structure
			uint uiOffset;

			vtid_decl_source():uiOffset(0){}
		};

		struct vtid_source
		{
			// autogenerated type name
			char const * czTypeName;
			// aliases of type
			std::vector<std::string> vtNameAliases;
			// data members declarations
			std::vector<vtid_decl_source> vtDataMembers;

			// interface for creation of object
			vtid_class_factory_proc ClassFactory;
			vtid_type_proc TypeProc;

			// base manipulations wrapper
			vtid_operations BaseOp;
			
			vtid_source():ClassFactory(0),TypeProc(0){}
		};
	}
}