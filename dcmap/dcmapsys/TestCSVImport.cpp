// dcmapsys.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../dcmapsyslib.h"
#include "dcmapsys.h"
#include "dcmapcore.h"

#include <g_text.h>
#include <g_std_file.h>
#include <g_file_istream.h>

using namespace std;
using namespace gsys;

static bool ReadCVSLine(std::istream& is,std::vector<std::string>& items,char chDivider)
{
	if(is.eof()) return false;

	items.clear();
	char div[] = {chDivider,'\r','\n',0};
	std::string val;
	while(!is.eof() && !is.fail())
	{
		is >> ws >> gsys::istream_line_till_first_of(val,div);
		gsys::unquote(val);
		items.push_back(val);
		if(gsys::char_is_nl(is.peek()))
		{
			is >> gsys::wsp_nl_scip;
			is >> ws;
			return true;
		}
		else
		{
			is.ignore();
		}
	}
	return true;
}

struct CTestCSVImportPlug : public IDCMapModule
{
	void DCMAPAPI Destroy(){delete this;}
	static IDCMapModule* DCMAPAPI CreateCallback(void* pUserData){return new CTestCSVImportPlug();}

	IDCMapCoreUI* m_pUI;
	IDCMapPlanetDataStorage* m_pPlanets;
	IDCMapPlayersDataStorage* m_pPlayers;

	bool  DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		DCMAP_GET_STORAGE(core,m_pPlanets,IDCMapPlanetDataStorage,planets);
		DCMAP_GET_STORAGE(core,m_pPlayers,IDCMapPlayersDataStorage,players);

		m_pUI = core->GetCoreUI();
		m_pUI->AddMenuItem(_T"data",_T"&Импорт CSV",this,0);
		return true;
	}

	void Execute()
	{
		if(!m_pUI->FileOpenDialog(".csv","CSV Файлы|*.csv"))return;

		std_file file(m_pUI->GetFilename(),std_file::ModeRead);
		if(!file.valid())return;
		igfilestream is(&file);

		vector<string> Items;

		int ind_x=0,ind_y=1,ind_o=2,ind_e=3,ind_m=4,ind_t=5,ind_s=6,ind_owner=7,ind_name=8;

		int ind_max = 8;

		m_pPlanets->BeginUpdate();
		m_pPlayers->BeginUpdate();

		int errors=0;

		while(ReadCVSLine(is,Items,';'))
		{
			if(Items.size() <= ind_max)continue;
			
			int x,y;
			if(!lex_convert(x,Items[ind_x])){errors++;continue;}
			if(!lex_convert(y,Items[ind_y])){errors++;continue;}

			DCMapPlanetInfo pinf;
			m_pPlanets->FillDefaults(&pinf);

			pinf.x = x;
			pinf.y = y;

			int iv=0;
			lex_convert(iv,Items[ind_o]);pinf.o = iv;iv=0;
			lex_convert(iv,Items[ind_e]);pinf.e = iv;iv=0;
			lex_convert(iv,Items[ind_m]);pinf.m = iv;iv=0;
			lex_convert(iv,Items[ind_t]);pinf.t = iv;iv=0;
			lex_convert(iv,Items[ind_s]);pinf.s = iv;iv=0;

			strncpy(pinf.caption,Items[ind_name].c_str(),DCMapPlanetCaptionLen);

			if(!Items[ind_owner].empty())
			{
				m_pPlayers->RetrieveByName(Items[ind_owner].c_str(),&pinf.owner);
			}

			m_pPlanets->RetrieveByCoords(x,y);
			if(!m_pPlanets->UpdateData(&pinf))
			{
				errors++;
			}
		}

		/*
		for(int x=1;x<=1000;x+=2)
		{
			for(int y=1;y<=1000;y+=2)
			{
				DCMapPlanetInfo pinf;
				m_pPlanets->FillDefaults(&pinf);
				m_pPlanets->RetrieveByCoords(x,y);

				pinf.x = x;
				pinf.y = y;

				pinf.o = float(rand())/RAND_MAX*100;
				pinf.e = float(rand())/RAND_MAX*100;
				pinf.m = float(rand())/RAND_MAX*100;
				pinf.t = float(rand())/RAND_MAX*100;
				pinf.s = float(rand())/RAND_MAX*100;

				if(!m_pPlanets->UpdateData(&pinf));
			}
		}*/

		m_pPlanets->EndUpdate();
		m_pPlayers->EndUpdate();

		m_pUI->MessageBox("Импорт успешно завершён","Test Import plugin",0);
	}

	int DCMAPAPI OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapModule* pSender)
	{
		switch(Msg)
		{
		case DCMM_UI_COMMAND:
			Execute();
			break;
		default:
			break;
		}
		return 0;
	};
};

void Register_CTestCSVImportPlug()
{
	g_pDCMapSys->RegisterPlugin("PCoreUtil","TestPlug1",CTestCSVImportPlug::CreateCallback,0);
}


