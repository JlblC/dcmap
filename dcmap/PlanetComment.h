//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef PlanetCommentH
#define PlanetCommentH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SizeableSideModule.h"
#include "sFrameAdapter.hpp"
#include "sMemo.hpp"
#include "BaseModule.h"
//---------------------------------------------------------------------------
class TPlanetCommentFrame : public TSizeableSideFrame
{
	typedef TSizeableSideFrame Inherited;
__published:	// IDE-managed Components
	TsMemo *CommentMemo;
	void __fastcall CommentMemoChange(TObject *Sender);
	void __fastcall CommentMemoExit(TObject *Sender);
private:	// User declarations
	DCMapDataReference m_CurrentRef;
public:		// User declarations
	__fastcall TPlanetCommentFrame(TComponent* Owner);
    IDCMapPlanetDataStorage* m_pPlanets;
    IDCMapPlanetManager* m_pPlanetMan;
    bool OnInit();
    int OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
    void UpdateData();

    bool m_bChanging;
    void SaveConfig(DCMapConfigWrapper cfg);

};
//---------------------------------------------------------------------------
extern PACKAGE TPlanetCommentFrame *PlanetCommentFrame;
//---------------------------------------------------------------------------
#endif
