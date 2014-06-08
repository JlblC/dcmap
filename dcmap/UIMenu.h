//---------------------------------------------------------------------------
#ifndef UIMenuH
#define UIMenuH
#include "UIBase.h"


class TDCMapMenuItem : public IDCMapUIMenuItem,public TDCMapBasicComponentItem
{
public:
	typedef std::map<TComponent*,TMenuItem*> MapItems;
	typedef std::list<TDCMapMenuItem*> ListItems;
private:
	MapItems Items;

	//TMenuItem* m_pMenuItem;
	AnsiString m_Path;
	String m_Caption;
	String ShortCut;
	bool Check;
	bool Checked;
	bool Enabled;
	bool Visible;

	ListItems* m_pLister;
	void SetupMenuItem(TMenuItem* pItem);
protected:
	void __fastcall DoClick(TObject *Sender);
public:
	void SetLister(ListItems* pLister)
	{
		if(m_pLister) m_pLister->erase(std::find(m_pLister->begin(),m_pLister->end(),this));
		m_pLister = pLister;
		if(m_pLister)m_pLister->push_back(this);
	}
	void InitItem(AnsiString Path,String Caption,IDCMapModule* pOwner,int iID);
	void AddMenuItem(TComponent* Target,TMenuItem* pMenuItem);
	void CreateMenuItem(TComponent* Target,TMenuItem* pMenuParent);
public:
	TDCMapMenuItem();
	bool DCMAPAPI Test();
	void DCMAPAPI Destroy(){SetLister(0);Unlink();delete this;}
	void InitItem(TMenuItem* pMenuItem,IDCMapModule* pOwner,int iID);
	bool CreateItem(TMenuItem* pMenuParent);
	void ClearItem();
	void InitItem(TDCMapUIContext* pContext,dcmapCSTR Path,dcmapWCSTR Caption,IDCMapModule* pOwner,int iID);
	int DCMAPAPI ScriptExec(int type,dcmapWCSTR command,IDCMapVarValue* pParams,IDCMapVarValue* pResult);

	void DCMAPAPI SetShortCut(dcmapWCSTR Str);
	void DCMAPAPI SetCaption(dcmapWCSTR Str);

	bool DCMAPAPI GetCheck(){return Check;}
	bool DCMAPAPI GetChecked(){return Checked;}
	bool DCMAPAPI GetEnabled(){return Enabled;}
	bool DCMAPAPI GetVisible(){return Visible;}
	dcmapWCSTR DCMAPAPI GetShortCut(){return ShortCut.w_str();}
	dcmapWCSTR DCMAPAPI GetCaption(){return m_Caption.w_str();}

	void DCMAPAPI SetCheck(bool v){
		if(Check == v)return;
		Check = v;
		if(!Check) SetChecked(false);
		_foreach(MapItems,Items,it)it->second->AutoCheck = Check;
	}
	void DCMAPAPI SetChecked(bool v){
		if(Checked == v)return;
		Checked = v;_foreach(MapItems,Items,it)it->second->Checked = Checked;
	}
	void DCMAPAPI SetEnabled(bool v){
		if(Enabled == v)return;
		Enabled = v;_foreach(MapItems,Items,it)it->second->Enabled = Enabled;
	}
	void DCMAPAPI SetVisible(bool v){
		if(Visible == v)return;
		Visible = v;_foreach(MapItems,Items,it)it->second->Visible = Visible;
	}

};

class TDCMapPopupMenu : public IDCMapUIMenu,public TDCMapBasicComponentItem
{
	TPopupMenu* m_pMenu;
    ~TDCMapPopupMenu()
    {
        if(m_pMenu)delete m_pMenu;
	}
	bool Updated;
public:
	TDCMapPopupMenu():m_pMenu(0),Updated(false){}
    void __fastcall DoClick(TObject *Sender);
	void DCMAPAPI Destroy(){Unlink();delete this;}
	void InitItem(TComponent* Owner,IDCMapModule* pOwner,int iID);
	void DCMAPAPI Popup();
    IDCMapUIMenuItem* DCMAPAPI AddItem(const char* MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id);
};


class TDCMapUIContext : public IDCMapUIContext
{
	typedef std::set<TDCMapBasicComponentItem*> ItemsList;
	ItemsList Items;
	TMenuItem* m_pMenu;
public:
	IDCMapVarValue* m_pContextValue;
	void  Link(TDCMapBasicComponentItem* pItem);
	void  Unlink(TDCMapBasicComponentItem* pItem);
	void  SetMenu(TMenuItem* pMenu);
public:
	TDCMapUIContext();
	~TDCMapUIContext();
	IDCMapUIMenuItem* DCMAPAPI AddMenuItem(const char* MenuPath,dcmapWCSTR MenuCaption,IDCMapModule* pOwner,int id);
	bool DCMAPAPI SetMenu(IDCMapUIMenu* pPopup);
	IDCMapVarValue* DCMAPAPI ContextValue();
	int DCMAPAPI   Setup();
	bool DCMAPAPI  TestEmpty();
	void DCMAPAPI  ClearItems();
};

//---------------------------------------------------------------------------
#endif
