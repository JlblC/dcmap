// stdafx.cpp : source file that includes just the standard includes
// dcmapsys.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
#include "../dcmapsys.h"

int g_DCMapNumPlanets = DCMapNumPlanets;
bool g_bWine = false;
CDCMapAutoRegisterHelperManager* g_pDCMapAutoRegisterHelperManager=0;

dcmapDBREF g_NullDBREF;