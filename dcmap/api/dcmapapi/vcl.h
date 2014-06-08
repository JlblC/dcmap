#ifndef dcmapapi_dcmap_vcl_h__
#define dcmapapi_dcmap_vcl_h__
#include "ui.h"
#include "helper.h"

/*
#define LSTR(x) (g_pLocalStrings->GetString(#x))
#define LSTRW(x) (g_pLocalStrings->GetStringW(#x))
#define LSTRSECTION(x) (g_pLocalStrings->SelectSection(#x))
#define SLSTR(x) (g_pLocalStrings->GetSectionString(#x))
#define SLSTRW(x) (g_pLocalStrings->GetSectionStringW(#x))
*/


typedef void __fastcall (__closure *TKeyValueChangeChangeEvent)(String Name,System::TObject* Sender);
typedef void __fastcall (__closure *TResizeFrameEvent)(int Width,int Height,System::TObject* Sender);

struct IDCMapVCLPanel : public IDCMapUIPanel
{
	virtual TFrame* DCMAPAPI CreateFrame()=0;
	virtual void    DCMAPAPI DestroyFrame(TFrame* frame)=0;
	virtual UnicodeString  DCMAPAPI GetCaption()=0;
  	virtual int     DCMAPAPI ImageIndex()=0;
	virtual void    DCMAPAPI DoOk(){};
	virtual void    DCMAPAPI DoApply(){};
	virtual void    DCMAPAPI DoCansel(){};
	virtual void    DCMAPAPI SetFocus(){};
	virtual void    DCMAPAPI OnShow(){};
	virtual bool    DCMAPAPI DoTestData(){return true;};
	virtual void    DCMAPAPI SetSkinSection(String Section){};
	virtual void    DCMAPAPI SetText(WideString Text){};

	virtual TMainMenu*    DCMAPAPI GetFrameMenu(){return 0;};

	TKeyValueChangeChangeEvent OnKeyValueChange;
	TResizeFrameEvent          OnResize;
	TNotifyEvent               OnChange;
	TNotifyEvent               OnApplyChanges;
	TNotifyEvent               OnUpdateList;
};
DCMAP_SPEC_INTERFACE(IDCMapVCLPanel);

/*
template<typename T>
struct AutoRegister{
	AutoRegister(){
		T::Register();
	}
};
*/

struct IDCMapVCLFrameUI : public IDCMapFrameUI
{
	virtual TForm* DCMAPAPI GetForm()=0;
};

#endif // dcmapapi_dcmap_vcl_h__