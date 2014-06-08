#pragma once
#include "parser.h"

class CHTMLUIModule : public CModuleParser
{
    typedef CModuleParser Inherited;
    bool ParseCustomKeyword(bool active,wstring const& kw,
        wiparsestream &is,CVarValue& res,bool expr,CBasicBlockParser* cnt);

    IDCMapUIHTMLFrame* html_frame;
public:
    CHTMLUIModule();
    ~CHTMLUIModule();
    bool InitObject();
    int DCMAPAPI OnMessage(int Msg,dcmapWPARAM wParam,dcmapLPARAM lParam,IDCMapObject* pSender);
    bool DCMAPAPI Init(IDCMapCore* pCore,IDCMapWorkspace *pWorkspace);
    void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);

    void GetVariable(bool active,wstring const& kw,wiparsestream &is,CVarValue& res,CBasicBlockParser* cnt);

};
CObjectParser* HTMLUIModuleCreator();