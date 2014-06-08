#ifndef dcmapsys_vcl_h__
#define dcmapsys_vcl_h__


#if 0
#include "dcmapsys.h"

typedef void __fastcall (__closure *TKeyValueChangeChangeEvent)(String Name,System::TObject* Sender);

struct IDCMapVCLPanel : public IDCMapModule
{
    virtual TFrame* DCMAPAPI CreateFrame()=0;
    virtual void    DCMAPAPI DestroyFrame(TFrame* frame)=0;
	virtual String  DCMAPAPI GetCaption()=0;
	virtual void    DCMAPAPI DoOk(){};
	virtual void    DCMAPAPI DoApply(){};
	virtual void    DCMAPAPI DoCansel(){};
	virtual void    DCMAPAPI SetFocus(){};
	virtual void    DCMAPAPI OnShow(){};
	virtual bool    DCMAPAPI DoTestData(){return true;};
   	TKeyValueChangeChangeEvent OnKeyValueChange;
};
DCMAP_SPEC_INTERFACE(IDCMapVCLPanel);

template<typename T>
struct AutoRegister{
	AutoRegister(){
		T::Register();
	}
};
#endif


#endif // dcmapsys_vcl_h__
