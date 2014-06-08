//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BaseDataEdit.h"
#include "Interface.h"
#include "CoreUIManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma link "sBitBtn"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TBaseEditForm *BaseEditForm;
//---------------------------------------------------------------------------
__fastcall TBaseEditForm::TBaseEditForm(TComponent* Owner)
	: Inherited(Owner)
{
	m_iCurrentCopyMode=0;
	m_pIconsList=0;
	m_pContext=0;
        m_pDataSource=0;
}
__fastcall TBaseEditForm::~TBaseEditForm()
{
	if(m_pContext)m_pContext->ClearItems();
}
//---------------------------------------------------------------------------
void TBaseEditForm::SaveConfig()
{
    DCMapConfigWrapper cfg(GetDCMapCore()->GetConfig());
    cfg.section("Table_"+TableName);
    cfg["CurrentExportFormat"] << m_iCurrentCopyMode;
}

void TBaseEditForm::LoadConfig()
{
    DCMapConfigWrapper cfg(GetDCMapCore()->GetConfig());

    cfg.section("Table_"+TableName);
	int num=0;
	cfg["RegExConfNum"] >> num;
	DataPasteButton->Visible = num;

	cfg["CurrentExportFormat"] >> m_iCurrentCopyMode;
	num=0;
    cfg["ClipbrdExportNum"] >> num;
	DataCopyButton->Visible = num;
    for(int i=0;i<num;i++)
    {
    	TMenuItem* Item = new TMenuItem(this);
        UnicodeString val;
        cfg["ClipbrdExportName"+IntToStr(i)] >> val;
        Item->Caption = val;
        Item->Tag = i;
        Item->OnClick = CopyMenuClick;
        Item->GroupIndex=1;
        Item->RadioItem=true;
        if(i == m_iCurrentCopyMode)Item->Checked = true;
        CopyMenu->Items->Add(Item);
    }
}

void __fastcall TBaseEditForm::CopyMenuClick(TObject *Sender)
{
	((TMenuItem*)Sender)->Checked = true;
    m_iCurrentCopyMode = ((TMenuItem*)Sender)->Tag;
    DataCopyButtonClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TBaseEditForm::DataCopyButtonClick(TObject *Sender)
{
    IDCMapRecordExport* pExp = GetDCMapCore()->GetServiceTyped<IDCMapRecordExport>("RecordExport");

    DoApply();

    if(!m_pData->SelectID(m_SelRef))return;
    DCMapConfigWrapper cfg(GetDCMapCore()->GetConfig());
	cfg.section("Table_"+TableName);
    UnicodeString val;
    cfg["ClipbrdExportFormat"+IntToStr(m_iCurrentCopyMode)] >> val;
    pExp->ExportClipboard(TableName.c_str(),val.w_str(),&m_SelRef);
}
//---------------------------------------------------------------------------
void __fastcall TBaseEditForm::DataPasteButtonClick(TObject *Sender)
{
	IDCMapRecordImport* pImp = GetDCMapCore()->GetServiceTyped<IDCMapRecordImport>("RecordImport");
    if(!pImp)return;

	GetDCMapCore()->LockDataUpdate();
    DoApply();
    if(!pImp->ImportClipboard(TableName.c_str(),&m_SelRef))
    {

    }
    DoUpdateData();
    GetDCMapCore()->UnlockDataUpdate();
}

bool TBaseEditForm::Init(IDCMapDataSource* pData,DataReference const* Parent)
{
    if(!pData)return false;
    m_pDataSource = pData;
    if(!m_pData.init(m_pDataSource->GetStorage()))return false;


    if(Parent) m_ParentRef = *Parent;

    NewRecordName = m_pData->NewIndexText();
    Caption = m_pData->StorageCaption();

    LoadConfig();

    PropPageFrame->Init();
    PropPageFrame->PluginClassName = PluginClassName;

    PropPageFrame->OnCreate();
    PropPageFrame->SetOnKeyValueChange(KeyValueChange);

    Width += PropPageFrame->WidthUpdate;
	Height += PropPageFrame->HeightUpdate;

	m_pContext = g_CoreUIManager.GetContextEx(TableName+"_record");

    return true;
}

void __fastcall TBaseEditForm::KeyValueChange(String Val,TObject *Sender)
{

}
void __fastcall TBaseEditForm::ContextMenuPopup(TObject *Sender)
{
	m_pContext->SetMenu(ContextMenu->Items);
	IDCMapVarValue* pval = m_pContext->ContextValue();
	pval->Clear();
	pval->AddDBRef(&m_SelRef,m_pData);
	if(m_pContext->Setup())
	{
		InterfaceModule->HookMenu(ContextMenu);
    }
}

void TBaseEditForm::SelChanged()
{
	if(!m_SelRef.Valid())ContextButton->Visible = false;
	else
	{
		IDCMapVarValue* pval = m_pContext->ContextValue();
		pval->Clear();
		pval->AddDBRef(&m_SelRef,m_pData);
		ContextButton->Visible = !m_pContext->TestEmpty();
		if(ContextButton->Visible)ContextButton->Refresh();
    }
}
//---------------------------------------------------------------------------

void __fastcall TBaseEditForm::ContextButtonClick(TObject *Sender)
{
	TsSpeedButton* Button =  (TsSpeedButton*)Sender;
	if(Button->DropdownMenu)
	{
		TPoint at = Button->Parent->ClientToScreen(Point(Button->Left,Button->Top+Button->Height));
		Button->DropdownMenu->Popup(at.x,at.y);
	}
}
//---------------------------------------------------------------------------


