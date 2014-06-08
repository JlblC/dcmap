#pragma once

#include "g_gsys.h"

#include <stdlib.h>
#include <math.h>

//#define GLUT_DISABLE_ATEXIT_HACK

#ifdef GSYS_GLUT

#ifdef GSYS_WINDOWS
#	pragma comment(lib ,"glew32.lib")
#	pragma comment(lib ,"glut32.lib")
#endif

#include "./src/libs/glew.h"
#include "./src/libs/glut.h"
#else
#	include "GL/gl.h"
#	include "GL/glu.h"
#endif //GSYS_GLUT


#include "g_matrix4.h"
#include "g_color.h"
#include "g_fcolor.h"
#include "g_vector3.h"
#include "g_vector4.h"
#include "g_imgload.h"

#ifdef GSYS_GLUT

namespace gsys 
{
//
	// Simple wrapper to init glut without parametrs
	inline void glutInit()
	{
		int argc = 1;
		char *argv[] = {""};
		::glutInit(&argc,argv);
	}
}

#endif ///GSYS_GLUT


namespace gsys 
{

	// wrap Clear Color
	void inline glClearColor(gsys::fcolor const& cl)
	{
		::glClearColor(cl.r,cl.g,cl.b,cl.a);
	}

	// Simple wrapper to create ogl texture by gsys::bitmap
	void inline glTexImage2D(GLint level, GLint internalformat,gsys::bitmap const& l_Bmp)
	{
		::glTexImage2D(
				GL_TEXTURE_2D, 
				level,
				internalformat, 
				l_Bmp.height(), 
				l_Bmp.width(), 
				0, 
				GL_RGBA,
				GL_UNSIGNED_BYTE, 
				l_Bmp.buffer());
	}

	void inline glLoadMatrixf(matrix4 const& mat)
	{
		::glLoadMatrixf(&mat[0]);
	}

	void inline glVertex(vector3 const& v)
	{
		glVertex3d(v.x,v.y,v.z);
	}


}


