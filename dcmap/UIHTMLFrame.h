//$$---- Form HDR ----
//---------------------------------------------------------------------------

#ifndef UIHTMLFrameH
#define UIHTMLFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sFrameAdapter.hpp"
#include "BaseModule.h"
#include "dcmapsys.h"
#include "coreuimanager.h"
#include "sLabel.hpp"
#include "Htmlview.hpp"

class THTMLFrame;

struct CUIHTMLFrame  : public IDCMapUIHTMLFrame
{
	THTMLFrame* pFrame;
	CUIHTMLFrame(THTMLFrame* _pFrame):pFrame(_pFrame){}

	void* DCMAPAPI CastTo(DCMapInterfaceID idInterface);
	IDCMapModule* DCMAPAPI GetFinalModule();
	bool DCMAPAPI InitUI(IDCMapModule* pOwner,int id);
	bool DCMAPAPI LoadHTML(dcmapWCSTR pHTML,dcmapWCSTR pbaseURL,IDCMapModule* pProcessModule);
	void DCMAPAPI Reload(IDCMapModule* pProcessModule);
	bool DCMAPAPI SetObjectAttribute(dcmapWCSTR szObjID,dcmapWCSTR szAttribute,dcmapWCSTR szValue);
	bool DCMAPAPI SetObjectVisible(dcmapWCSTR szObjID,bool bVisible);
	dcmapWCSTR DCMAPAPI GetObjectAttribute(dcmapWCSTR szObjID,dcmapWCSTR szAttribute);
	bool DCMAPAPI GetObjectVisible(dcmapWCSTR szObjID);
	int DCMAPAPI OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
	void DCMAPAPI Reformat();
};

//---------------------------------------------------------------------------
class THTMLFrame : public TBaseModuleFrame
{
	friend class CUIHTMLFrame;
__published:	// IDE-managed Components
	THTMLViewer *Html;
	void __fastcall htmlMeta(TObject *Sender, const AnsiString HttpEq,
          const AnsiString Name, const AnsiString Content);
	void __fastcall htmlFormSubmit(TObject *Sender, const AnsiString Action,
          const AnsiString Target, const AnsiString EncType,
          const AnsiString Method, TStringList *Results);
	void __fastcall htmlObjectAction(TObject *Sender, TObject *Obj,
          const AnsiString OnClick);
	void __fastcall HtmlScript(TObject *Sender, const AnsiString Name,
          const AnsiString Language, const AnsiString Script);
	void __fastcall HtmlInclude(TObject *Sender, const AnsiString Command,
          TStrings *Params, AnsiString &IString);
	void __fastcall HtmlProcessing(TObject *Sender, bool ProcessingOn);
	void __fastcall HtmlHotSpotClick(TObject *Sender, const AnsiString SRC,
          bool &Handled);
protected:
	virtual void* DCMAPAPI GetInterface(DCMapInterfaceID idInterface);
	CUIHTMLFrame m_UIFrame;
	IDCMapModule* pProcessor;
	bool inCallback,inProcess;
	UnicodeString CurrentHtml;
	String MetaSection;
	String BaseURL;
	bool m_bInside;

private:	// User declarations
	void SetupColors();
	String Evalate(String Script);
	bool DoLoadHTML();
public:		// User declarations
	__fastcall THTMLFrame(TComponent* Owner);
	bool LoadHTML(dcmapWCSTR pHTML,dcmapWCSTR pBaseURL,IDCMapModule* pProcessModule);
	void Reload(IDCMapModule* pProcessModule);
	void OnShow();
	bool SetObjectAttribute(dcmapWCSTR szObjID,dcmapWCSTR szAttribute,dcmapWCSTR szValue);
	bool SetObjectVisible(dcmapWCSTR szObjID,bool bVisible);
	dcmapWCSTR GetObjectAttribute(dcmapWCSTR szObjID,dcmapWCSTR szAttribute);
	bool GetObjectVisible(dcmapWCSTR szObjID);
	void OnChnageSkin();
	int OnMessage(int MSG,DWORD wParam,DWORD lParam,IDCMapObject* pSender);
};
//---------------------------------------------------------------------------
extern PACKAGE THTMLFrame *HTMLFrame;
//---------------------------------------------------------------------------
#endif
