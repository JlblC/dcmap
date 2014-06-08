#pragma once
#include "../dcmapsyslib.h"

bool dcgovReadPlanets(IDCMapPlanetDataStorage* planets, dcmapDBREF const& player);
bool dcgovUpdateCorruption(IDCMapPlanetDataStorage* planets);
bool dcgovClear();