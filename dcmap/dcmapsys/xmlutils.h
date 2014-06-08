#pragma once
#include "XMLite.h"

enum EGameXMLErrors
{
	GAMEXML_ERROR=0,
	GAMEXML_OK=1,
	GAMEXML_TURN_IN_PROGRESS = 2,
	GAMEXML_NOT_LOGIN = 3,
};

int CheckGameXMLErrors(LPXNode root);