#pragma once
/*====================================================================
             GSYS Library Configuration
======================================================================*/

// enamble fast sqrt calculations
//#define GSYS_ENABLE_QUICK_SQRT

// enable sin cos & tan - table lookup
//#define GSYS_FASTMATH_ENABLE

// enable some image formats support requires dll
 
//#define GSYS_ENABLE_TIFF  // requires libtiff.dll
#define GSYS_ENABLE_JPEG  // requires libjpeg.dll
#define GSYS_ENABLE_PNG     // requires libpng.dll
#define GSYS_ENABLE_BMP  
#define GSYS_ENABLE_TGA  

// Let static link libjpeg.dll 

//#define GSYS_JPEG_STATIC_LINK 

// enable python support 

//#define GSYS_ENABLE_PYTHON  // requres python22.dll

// enables memory manager
#define GSYS_ENABLE_MEMORY_MANAGER  
#define GSYS_CUSTOM_MEMORY_MANAGER  

#define GSYS_ENABLE_CUSTOM_ASSERT

// enable assert in all configurations
//#define GSYS_FORCE_ASSERT

#define GSYS_ENABLE_ZLIB
#define GSYS_ENABLE_BZLIB
#define GSYS_ZLIB_STATIC_LINK

//#define GSYS_ENABLE_FORCE_INLINE

//#define GSYS_ENABLE_OGG_VORBIS
//#define GSYS_OGG_STATIC_LINK
//#define GSYS_VORBIS_STATIC_LINK

