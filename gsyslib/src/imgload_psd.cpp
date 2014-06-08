#include "common.h"

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include "../g_imgload.h"
#include "../g_file_util.h"
#include "../g_file.h"
#include "MyPSD.h"

GRESULT gsys::LoadPSD(gsys::gfile *file,bitmap & clarr,int Options,int* pOptionsQuery)
{
	CPSD psd;
	int r = psd.Load(file,clarr);
	if(r<0)return GERR_INVALID_DATA;

	GTransformImage(clarr,ImageVerticalMirror|ImageAlpha,Options,pOptionsQuery);
	return G_OK;
}


