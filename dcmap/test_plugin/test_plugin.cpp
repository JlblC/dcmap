
#include "../dcmap.h"
DCMAP_IMPLEMENT_PLUGIN;

#define ID_MENU1 1
#define ID_MENU2 2
#define ID_FORM 3

class CTestPlugin : public IDCMapModule
{
	IDCMapCore *m_pCore;

	bool DCMAPAPI Init(IDCMapCore* core,IDCMapWorkspace* workspace)
	{
		m_pCore = core;
		core->GetCoreUI()->AddMenuItem("TestPlugin:1000",L"TestPlugin",this,0);
		core->GetCoreUI()->AddMenuItem("TestPlugin/MENU1",L"Hallo",this,ID_MENU1);
		core->GetCoreUI()->AddMenuItem("TestPlugin/MENU2",L"HTML",this,ID_MENU2);
		return true;
	};

	dcmapLRESULT DCMAPAPI OnMessage(int Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender)
	{
		switch(Msg)
		{
		case DCMM_UI_COMMAND:
			if(wParam == ID_MENU1)
			{
				m_pCore->GetCoreUI()->MsgBox(L"Hello world",L"test",0);
			}
			else if(wParam == ID_MENU2)
			{
				IDCMapUIHTMLFrame* html_frame = m_pCore->GetCoreUI()->CreateUIModule
					("HTMLFrame",this,ID_FORM)->Cast<IDCMapUIHTMLFrame>();
				if(html_frame)
				{
					html_frame->LoadHTML(L"<input name='do' type='submit' onClick='DoIt' value='DoIt'>",0,this);

					m_pCore->GetCoreUI()->ShowModal(html_frame,0,0);          
				}
			}	
			break;
		case DCMM_UI_COMMAND_STR_FUNC:
			if(wParam == ID_FORM)
			{
				dcmapWCSTR str = (dcmapWCSTR)lParam;
				if( wcscmp(str,L"DoIt") == 0)	
				{
					m_pCore->GetCoreUI()->MsgBox(L"DoIt",L"test",0);
				}
			}
			break;
		}
		return 0;
	};
};

DCMAP_REGISTER(CTestPlugin,"PUtility","TestPlugin")


