#include "stdafx.h"
#include "../dcmapbase.h"
#include "XMLite.h"
#include "utils.h"
#include "xmlutils.h"

int CheckGameXMLErrors(LPXNode root)
{
	if(!root) return GAMEXML_ERROR;

	LPXNode errors = root->GetChild(L"errors");
	if(errors)
	{
		int n=errors->GetChildCount();
		for(int i=0;i<n;i++)
		{
			LPXNode error = errors->GetChild(i);
			if(error && error->name == L"error")
			{
				int err = xml_cast<int>(error->GetAttrValue(L"id"));

				if(err == 10000)return GAMEXML_NOT_LOGIN; // not login
			}
		}
	}

	LPXNode pagedata =  root->GetChild(L"page-data");
	if(pagedata)
	{
		LPXNode not_logged = pagedata->GetChild(L"not-logged-in");
		if(not_logged)
		{
			dcmapWCSTR page = not_logged->GetAttrValue(L"page");
			if(page && wcscmp(page,L"turn-in-progress")==0)
			{
				return GAMEXML_TURN_IN_PROGRESS; // turn in progress
			}
		}
	}
	return  GAMEXML_OK;
};