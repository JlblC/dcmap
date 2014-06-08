//---------------------------------------------------------------------------

#ifndef UICustomFrameH
#define UICustomFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BaseModule.h"
#include "sFrameAdapter.hpp"

class TPluginFrame;

struct CUICustomFrame  : public IDCMapUICustomFrame
{
	TPluginFrame* pFrame;
	CUICustomFrame(TPluginFrame* _pFrame):pFrame(_pFrame){}
};

//---------------------------------------------------------------------------
class TPluginFrame : public TBaseModuleFrame
{
__published:	// IDE-managed Components
private:	// User declarations
protected:
	CUICustomFrame m_UIFrame;
public:		// User declarations
	__fastcall TPluginFrame(TComponent* Owner);
	virtual void* DCMAPAPI GetInterface(DCMapInterfaceID idInterface);
};
//---------------------------------------------------------------------------
extern PACKAGE TPluginFrame *PluginFrame;
//---------------------------------------------------------------------------
#endif
