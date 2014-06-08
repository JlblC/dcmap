//---------------------------------------------------------------------------

#ifndef LocalizeH
#define LocalizeH

#include "dcmapsys.h"

void LocalizeContainer(TControl* Base,IDCMapLocalStrings* Strs=0);
void LocalizeComponent(TComponent* Cnt,IDCMapLocalStrings* Strs=0);
bool LocalizeMenuItem(TMenuItem* sControl,int Mode=0,IDCMapLocalStrings* Strs=0);


//---------------------------------------------------------------------------
#endif
