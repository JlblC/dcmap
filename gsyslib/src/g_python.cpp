
#include "common.h"

#ifdef GSYS_ENABLE_PYTHON // disable python

#include "../g_python.h"
#include "../g_vector3.h"
#include "../g_matrix3.h"
#include "../g_vector4.h"
#include "../g_matrix4.h"
#include "../g_quaternion.h"
#include "../g_pack_file.h"

namespace gsys{ namespace py {

using namespace boost::python;

static void  ExportType_pack_file()
{
	class_<pack_file_builder>("pack_file_builder")
		.def(init<>())
		.def("add_file",&pack_file_builder::add_file)
		.def("build",&pack_file_builder::build)
		;
}

static void  ExportType_quaternion()
{
	class_<quaternion>("quaternion")
		.def(init<>())
		.def(init<float,float,float,float>())
		.def_readwrite("x", &quaternion::x)
		.def_readwrite("y", &quaternion::y)
		.def_readwrite("z", &quaternion::z)
		.def_readwrite("w", &quaternion::w)
//		.def(str(self))
		;

}

static void  ExportType_vector4()
{
	class_<vector4>("vector4")
		.def(init<>())
		.def(init<float,float,float,float>())
//		.def(str(self))
		;
}
static void  ExportType_matrix4()
{
	typedef matrix4::value_type vtype;
	// declare overloads
	matrix4& (matrix4::*translation_1)(vtype,vtype,vtype) = &matrix4::translation;
	matrix4& (matrix4::*translation_2)(vector3 const&) = &matrix4::translation;

	matrix4& (matrix4::*rotation_1)(vtype,vtype,vtype) = &matrix4::rotation;
	matrix4& (matrix4::*rotation_2)(vector3 const&) = &matrix4::rotation;
	matrix4& (matrix4::*rotation_3)(quaternion const&) = &matrix4::rotation;

	class_<matrix4>("matrix4")
		.def(init<>())
//		.def(str(self))
		.def("identity",&matrix4::identity,return_internal_reference<>())

		.def("translation",translation_1,return_internal_reference<>())
		.def("translation",translation_2,return_internal_reference<>())

		.def("rotation",rotation_1,return_internal_reference<>())
		.def("rotation",rotation_2,return_internal_reference<>())
		.def("rotation",rotation_3,return_internal_reference<>())

		.def("rotation_x",&matrix4::rotation_x,return_internal_reference<>())
		.def("rotation_y",&matrix4::rotation_y,return_internal_reference<>())
		.def("rotation_z",&matrix4::rotation_z,return_internal_reference<>())
		;
}                   


static void  ExportType_vector3()
{
	class_<vector3>("vector3")
///////////////////////////////////////////////////////////////// Contstruction
		.def(init<>())
		.def(init<float,float,float>())
///////////////////////////////////////////////////////////////// Data Access
		.def_readwrite("x", &vector3::x)
		.def_readwrite("y", &vector3::y)
		.def_readwrite("z", &vector3::z)
///////////////////////////////////////////////////////////////// Operations
		.def("norm",&vector3::norm,return_internal_reference<>())
		.def("cross",&vector3::cross)
		.def("dot",&vector3::dot)
		.def("lensquared",&vector3::lensquared)
		.def("len",&vector3::len)

///////////////////////////////////////////////////////////////// Operators
		.def(self - self) 
		.def(self + self) 
		.def(self * self) // cross product
		.def(self % self) // dot product
		.def(self * float())         
		.def(float() * self)         
		.def(self / float())   
		.def(self == self) 
//		.def(str(self))
       ;
}

static void ExportType_dwcolor()
{
	class_<dwcolor>("dwcolor")
///////////////////////////////////////////////////////////////// Contstruction
		.def(init<int,int,int>())
		.def(init<int,int,int,int>())
///////////////////////////////////////////////////////////////// Data Access
		.def_readwrite("r", &dwcolor::r)
		.def_readwrite("g", &dwcolor::g)
		.def_readwrite("b", &dwcolor::b)
		.def_readwrite("a", &dwcolor::a)
///////////////////////////////////////////////////////////////// Operators
       ;
	class_<fcolor>("fcolor")
///////////////////////////////////////////////////////////////// Contstruction
		.def(init<float,float,float>())
		.def(init<float,float,float,float>())
		.def(init<dwcolor const&>())
///////////////////////////////////////////////////////////////// Data Access
		.def_readwrite("r", &fcolor::r)
		.def_readwrite("g", &fcolor::g)
		.def_readwrite("b", &fcolor::b)
		.def_readwrite("a", &fcolor::a)
///////////////////////////////////////////////////////////////// Operators
       ;
}
static void init_module_gsys()
{
	py_export_gsys();
}

void py_export_gsys()
{
	ExportType_vector3();
	ExportType_vector4();
	ExportType_quaternion();
	ExportType_matrix4();
	ExportType_dwcolor();
	ExportType_pack_file();
}

void Initialize()
{
	Py_Initialize();
	// add classes from gsys into python scripts
    boost::python::detail::init_module("gsys",&init_module_gsys);
}

}}

#endif // GSYS_ENABLE_PYTHON

