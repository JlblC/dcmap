#pragma once
#include "g_gsys.h"

/* class for creatring base class with virtual functional object to desire from
 Ex:
 class MegaClass : public gsys::virt_func2<int,int,bool>
 {
	bool operator()(int a,int b)
	{
		return a==b;
	}
 };


*/
namespace gsys
{
	template<class R=void> struct virt_func
	{virtual R operator()(void)=0;};
	template<class T1,class R=void> struct virt_funcl
	{virtual R operator()(T1)=0;};
	template<class T1,class T2,class T3,class R=void> struct virt_func2
	{virtual R operator()(T1,T2)=0;};
	template<class T1,class T2,class T3,class R=void> struct virt_func3
	{virtual R operator()(T1,T2,T3)=0;};
	template<class T1,class T2,class T3,class T4,class R=void> struct virt_func4
	{virtual R operator()(T1,T2,T3,T4)=0;};
}