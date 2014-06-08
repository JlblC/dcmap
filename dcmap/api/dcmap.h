#ifndef dcmapapi_dcmap_h__
#define dcmapapi_dcmap_h__
#pragma once

// Верcия API 
#define DCMAP_API_VERSION  0xa0103100u
#define DCMAP_API_MAJOR    144
#define DCMAP_API_MINOR    145

#include "dcmapapi/configure.h"

#include "dcmapapi/basic.h"
#include "dcmapapi/basictypes.h"
#include "dcmapapi/strings.h"
#include "dcmapapi/base.h"
#include "dcmapapi/config.h"

#include "dcmapapi/ui.h"

#include "dcmapapi/messages.h"

#include "dcmapapi/db.h"

#include "dcmapapi/services.h"

#include "dcmapapi/modules.h"

#ifdef INC_VCL
#	include "dcmapapi/vcl.h"
#else
#	include "dcmapapi/helper.h"
#endif 

#endif // dcmapapi_dcmap_h__




