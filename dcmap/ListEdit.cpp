//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ListEdit.h"
#include "BaseModule.h"
#include "Interface.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "sAdapter"
#pragma link "sSkinProvider"
#pragma link "sAlphaListBox"
#pragma link "sComboBox"
#pragma link "sEdit"
#pragma link "sGroupBox"
#pragma link "sPanel"
#pragma link "sSpeedButton"
#pragma link "sTabControl"
#pragma link "sButton"
#pragma link "sCheckBox"
#pragma link "sBitBtn"
#pragma link "sListView"
#pragma resource "*.dfm"
TListEditForm *ListEditForm;
//---------------------------------------------------------------------------
__fastcall TListEditForm::TListEditForm(TComponent* Owner)
    : TBaseEditForm(Owner)
{
     m_bInsertMode=false;
     m_bKeyChanged=false;
     m_bError=false;
     m_bUpdatesHolded = false;
     m_iCurrentCopyMode=0;
     m_iIconColumn=-1;
     Filter=0;
}
__fastcall TListEditForm::~TListEditForm()
{
    ClearList();
}
//---------------------------------------------------------------------------

bool TListEditForm::Init(IDCMapDataSource* pData,DataReference const* Parent)
{
	if(!TBaseEditForm::Init(pData,Parent))return false;

	PropPageFrame->SetOnUpdateList(UpdateList);
	PropPageFrame->SetOnApplyChanges(ApplyButtonClick);

	if(AnsiString(m_pData->Name()) == "icons") m_iIconColumn = 0;
	else m_iIconColumn = m_pData->ColumnID("icon");

	if(m_iIconColumn >= 0)
	{
		m_pIconsList = GetDCMapCore()->GetServiceTyped<TIconsList>("VCLIconsList");
		if(!m_pIconsList)return false;
	}

    return true;
}

void __fastcall TListEditForm::FormShow(TObject *Sender)
{
    PropPageFrame->OnShow();
    UpdateList();
	if(DataList->Items->Count)DataList->ItemIndex=0;
	//PlayerListBoxClick(PlayerListBox);
}

void TListEditForm::ClearList()
{
	 int cnt = DataList->Items->Count;
	 for(int i=0;i<cnt;i++)
	 {
		TDataRefObject* pObj = (TDataRefObject*)DataList->Items->Item[i]->Data;
		if(pObj)delete pObj;
	 }
	//DeleteStringsObjects(PlayerListBox->Items);
	//PlayerListBox->Clear();
	DataList->Clear();
}

void TListEditForm::AddItem(String MaskText,dcmapDBREF const& ref)
{
	if(Filter)
        {
            if(Filter->Process(m_pData,ref))return;
        }

        String Name = m_pData->GetWideName();
        if(m_bKeyChanged && ref == m_SelRef)
        {
                Name = SelPlayer;
        }
        else if(MaskText.IsEmpty() || Name.UpperCase().Pos(MaskText))
        {

        }
        else return;

        TListItem* item = DataList->Items->Add();
        item->Caption = Name;
        item->Data = new TDataRefObject(ref);

        if(m_iIconColumn < 0) item->ImageIndex = -1;
        else
        {
                dcmapDBREF ref = *m_pData->GetReference(m_iIconColumn);
                item->ImageIndex = m_pIconsList->IconIndex(&ref);
                if(item->ImageIndex >= 0)HaveIcons = true;
        }
}

void __fastcall TListEditForm::UpdateList(TObject *Sender)
{
	ClearList();
	bool s;

        HaveIcons=false;

	if(m_pIconsList)m_pIconsList->Prepare();

	String MaskText = MaskEdit->Text.UpperCase();

	DataList->Items->BeginUpdate();


    if(m_ParentRef.id == DCMapInvalidReferenceId)
    	s = m_pData->SelectAll();
    else
    	s = m_pData->SelectByParentID(m_ParentRef);
    if(s)
    do
    {
    	AddItem(MaskText,*m_pData->GetID());
    } while(m_pData->Next());

	// Adjust column
	TListItem* pBottom = DataList->Items->Item[DataList->Items->Count-1];
	if(pBottom)
	{
		if(pBottom->Top > DataList->Height-25)
		{
			 DataList->Columns->Items[0]->Width = DataList->Width - 22;
		}
		else
			DataList->Columns->Items[0]->Width = DataList->Width - 2;
	}
	if(SortCheck->Checked)DataList->AlphaSort();

	if(HaveIcons && m_pIconsList)
	{
	   DataList->SmallImages = m_pIconsList->ImageList();
	}
	else DataList->SmallImages = 0;

	DataList->Items->EndUpdate();

	if(!DataList->Items->Count && SelPlayer.IsEmpty())
	{
		PropPageFrame->Hide();
	}
}


//---------------------------------------------------------------------------
void __fastcall TListEditForm::ShowListSelection()
{
		if(DataList->Selected->Top > DataList->Height - 20 ||
			DataList->Selected->Top < 0)
		{
			 DataList->Scroll(0,DataList->Selected->Top);
		}
}
void __fastcall TListEditForm::DelayTimerTimer(TObject *Sender)
{
	DelayTimer->Enabled=false;
        UpdateList();
	//if(PlayerListBox->ItemIndex < 0)return;
	//PlayerListBox->ItemIndex =  PlayerListBox->Items->IndexOf(SelPlayer);
	DataList->Selected = DataList->FindCaption(0,SelPlayer,false,true,false);
	if(!DataList->Selected)
	{
		if(DataList->Items->Count)
		{
			DataList->ItemIndex = 0;
			PropPageFrame->Show();
			DeleteButton->Enabled = true;
		}
		else
		{
			PropPageFrame->Hide();
			DeleteButton->Enabled = false;
		}
	}
	else
	{
		PropPageFrame->Show();
		DeleteButton->Enabled = true;

		ShowListSelection();
	}
}
//---------------------------------------------------------------------------
void __fastcall TListEditForm::MaskEditChange(TObject *Sender)
{
	if(m_pData->NumRecords() > 100)
        {
		DelayTimer->Enabled=false;
		DelayTimer->Enabled=true;
        }
        else
        {
             DelayTimerTimer(Sender);
        }
}
//---------------------------------------------------------------------------

void __fastcall TListEditForm::_PlayerListBoxClick(TObject *Sender)
{
	TListItem* SelItem = DataList->Selected;
	if(!SelItem)return;

	String Sel = SelItem->Caption;

	//if(PlayerListBox->ItemIndex < 0)return;
	//String Sel = PlayerListBox->Items->Strings[PlayerListBox->ItemIndex];

	TDataRefObject* SelRef = (TDataRefObject*)SelItem->Data;
	if(SelRef->ref != m_SelRef || m_bKeyChanged)
	{
		if(m_pData->SelectID(SelRef->ref))
		{
			m_SelRef = *m_pData->GetID();

			if(!SelPlayer.IsEmpty())
			{
				if(!m_bInsertMode)ApplyButtonClick(Sender);
				if(m_bError)
				{
					DataList->Selected = DataList->FindCaption(0,SelPlayer,false,true,false);
					//DataList->ItemIndex = DataList->Items->IndexOf()
					//PlayerListBox->ItemIndex =  PlayerListBox->Items->IndexOf(SelPlayer);
					return;
				}
			}
			DoUpdateData();
			SelPlayer = Sel;
			PropPageFrame->Show();
			DeleteButton->Enabled = true;
			SelChanged();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TListEditForm::MaskEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if(Key == VK_DOWN && DataList->Items->Count)
    {
		DataList->SetFocus();
		DataList->ItemIndex=0;
        //PlayerListBoxClick(PlayerListBox);
    }
}
//---------------------------------------------------------------------------


void __fastcall TListEditForm::_PlayerListBoxKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if(Key == VK_UP && DataList->ItemIndex == 0)
    {
        MaskEdit->SetFocus();
    }
}
//---------------------------------------------------------------------------

void __fastcall TListEditForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    if(ModalResult == mrCancel)
        CanselButtonClick(Sender);
    else if(ModalResult == mrOk)
    {
        ApplyButtonClick(Sender);
        if(m_bError) Action = caNone;
    }
    if(m_bUpdatesHolded)
    {
        m_pData->EndUpdate();
        m_bUpdatesHolded = false;
    }
    SaveConfig();
}
//---------------------------------------------------------------------------
void TListEditForm::DoApply()
{
    if(m_bInsertMode || m_bKeyChanged)
    {
        if(!PropPageFrame->DoTestData())
        {
            m_bError = true;
            return;
        }
        m_bError = false;

        PropPageFrame->DoApply();
        m_bInsertMode=false;
		DataList->Enabled = true;
        MaskEdit->Enabled = true;
        m_bKeyChanged=false;
        DeleteButton->Enabled = true;
        AddButton->Enabled = true;
		UpdateList();

		DataList->Selected = DataList->FindCaption(0,SelPlayer,false,true,false);
		//PlayerListBox->ItemIndex =  PlayerListBox->Items->IndexOf(SelPlayer);

        if(DataList->ItemIndex < 0)
            PropPageFrame->Hide();
        //else PlayerListBoxClick(PlayerListBox);
        //m_SelRef->
    }
    else if(!SelPlayer.IsEmpty())
    {
        PropPageFrame->DoApply();
	}
	PropPageFrame->SelectRef(m_SelRef);
	SelChanged();
}

void __fastcall TListEditForm::ApplyButtonClick(TObject *Sender)
{
	DoApply();
}
//---------------------------------------------------------------------------

void TListEditForm::Select(DataReference const& Ref)
{
    if(m_pData->SelectID(Ref))
    {
        String Name = m_pData->GetWideName();
        MaskEdit->Text=Name;
		UpdateList();
		DataList->Selected = DataList->FindCaption(0,SelPlayer,false,true,false);
		//PlayerListBox->ItemIndex =  PlayerListBox->Items->IndexOf(Name);
        //PlayerListBoxClick(PlayerListBox);
	}
	SelChanged();
}

//---------------------------------------------------------------------------


void __fastcall TListEditForm::DeleteButtonClick(TObject *Sender)
{
    CanselButtonClick(Sender);
    if(m_pData->SelectID(m_SelRef))
    {
        m_SelRef.Invalidate();
        m_pData->Delete();
        int i = DataList->ItemIndex;

		TListItem* pSel = DataList->Selected;
		if(pSel)
		{
			delete (TDataRefObject*)pSel->Data;
			DataList->DeleteSelected();
		}

        //int Ind = PlayerListBox->Items->IndexOf(SelPlayer);
        //PlayerListBox->Items->Delete(PlayerListBox->Items->IndexOf(SelPlayer));
		//delete PlayerListBox->Items->Objects[i];
		//PlayerListBox->Items->Delete(i);

        SelPlayer="";
        if(!DataList->Items->Count)
        {
			PropPageFrame->Hide();
			DeleteButton->Enabled = false;
        }
        else
		{
		  if(i< DataList->Items->Count)
			DataList->ItemIndex = i;
		  else
			DataList->ItemIndex = DataList->Items->Count-1;
		  //PlayerListBoxClick(Sender);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TListEditForm::AddButtonClick(TObject *Sender)
{
	 ApplyButtonClick(Sender);

     SelPlayer = "*"+NewRecordName;

     m_pData->Insert();
     m_pData->SetWideName(SelPlayer.w_str());

    if(m_ParentRef.id != DCMapInvalidReferenceId)
     	m_pData->SetParent(m_ParentRef);

	 m_pData->Post(&m_SelRef);
     m_bInsertMode=true;
	 DataList->Enabled = false;
     MaskEdit->Enabled = false;

     m_bKeyChanged=true;

	 UpdateList();
	 DataList->Selected = DataList->FindCaption(0,SelPlayer,false,true,false);
	 //PlayerListBox->ItemIndex =  PlayerListBox->Items->IndexOf(SelPlayer);
	 //PlayerListBoxClick(PlayerListBox);

     DeleteButton->Enabled = false;
     AddButton->Enabled = false;

     PropPageFrame->SetControlFocus();
     // Change planet
     // m_bInsertMode = true; //
}
//---------------------------------------------------------------------------

void __fastcall TListEditForm::KeyValueChange(String Val,TObject *Sender)
{
    SelPlayer = Val;
	if(DataList->ItemIndex < 0)return;
	//PlayerListBox->Items->Strings[PlayerListBox->ItemIndex] = Val;
	DataList->Selected->Caption = Val;
	m_bKeyChanged=true;
    DataList->Enabled = false;
    MaskEdit->Enabled = false;
    AddButton->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TListEditForm::CanselButtonClick(TObject *Sender)
{
     PropPageFrame->DoCansel();
     if(m_bInsertMode)
     {
        if(m_pData->SelectID(m_SelRef))
        {
            m_pData->Delete();
        }
        m_bInsertMode = false;
        m_bKeyChanged = false;
        DataList->Enabled = true;
        MaskEdit->Enabled = true;
        UpdateList();
        PropPageFrame->Hide();
        DeleteButton->Enabled = false;
        SelPlayer="";
     }
     else if(m_bKeyChanged)
     {
        if(m_pData->SelectID(m_SelRef))
        {
            KeyValueChange(m_pData->GetWideName(),this);
            m_bKeyChanged = false;
        }
     }
     MaskEdit->Enabled = true;
     DataList->Enabled = true;
     AddButton->Enabled = true;

     m_bError = false;
     DoUpdateData();
}
//---------------------------------------------------------------------------

void __fastcall TListEditForm::SortCheckClick(TObject *Sender)
{
	//PlayerListBox->Sorted = SortCheck->Checked;

    UpBtn->Enabled = !SortCheck->Checked;
    DownBtn->Enabled = !SortCheck->Checked;

	if(SortCheck->Checked)DataList->AlphaSort();
	else
	{
		UpdateList();
		DataList->Selected = DataList->FindCaption(0,SelPlayer,false,true,false);
	}
	//PlayerListBox->ItemIndex =  PlayerListBox->Items->IndexOf(SelPlayer);
}
//---------------------------------------------------------------------------
void TListEditForm::SaveConfig()
{
	TBaseEditForm::SaveConfig();
    DCMapConfigWrapper cfg(GetDCMapCore()->GetConfig());
    cfg.section("Table_"+TableName);
    cfg["Sorted"] << SortCheck->Checked;
}

void TListEditForm::LoadConfig()
{
	TBaseEditForm::LoadConfig();
    
    DCMapConfigWrapper cfg(GetDCMapCore()->GetConfig());

    cfg.section("Table_"+TableName);
    bool b;
	if(cfg["Sorted"] >> b)
		SortCheck->Checked = b;

    //PlayerListBox->Sorted = SortCheck->Checked;
    UpBtn->Enabled = !SortCheck->Checked;
    DownBtn->Enabled = !SortCheck->Checked;
}

void __fastcall TListEditForm::UpBtnClick(TObject *Sender)
{
	if(SortCheck->Checked)return;
	if(!DataList->Selected) return;

    if(!m_bUpdatesHolded)
    {
        m_pData->BeginUpdate();
        m_bUpdatesHolded = true;
	}

	TListItem* pItem = DataList->Selected;
	if(pItem->Index==0)return;
	TListItem* ExItem = DataList->Items->Item[pItem->Index-1];

	
	//int i2 = PlayerListBox->ItemIndex-1;

	TDataRefObject* r1 = (TDataRefObject*)pItem->Data;
	TDataRefObject* r2 = (TDataRefObject*)ExItem->Data;

	if(m_pData->OrderSwap(r1->ref,r2->ref))
	{
		String   s1 = pItem->Caption;
		void* o1 = pItem->Data;

		pItem->Caption = ExItem->Caption;
		pItem->Data = ExItem->Data;

		ExItem->Caption = s1;
		ExItem->Data = o1;
	}
	DataList->Selected = ExItem;
	ShowListSelection();
	ExItem->Focused = true;
}
//---------------------------------------------------------------------------

void __fastcall TListEditForm::DownBtnClick(TObject *Sender)
{
	if(SortCheck->Checked)return;
	if(!DataList->Selected) return;

    if(!m_bUpdatesHolded)
    {
        m_pData->BeginUpdate();
        m_bUpdatesHolded = true;
	}

	TListItem* pItem = DataList->Selected;
	if(pItem->Index==DataList->Items->Count-1)return;
	TListItem* ExItem = DataList->Items->Item[pItem->Index+1];

	TDataRefObject* r1 = (TDataRefObject*)pItem->Data;
	TDataRefObject* r2 = (TDataRefObject*)ExItem->Data;
	if(m_pData->OrderSwap(r1->ref,r2->ref))
	{
		String   s1 = pItem->Caption;
		void* o1 = pItem->Data;

		pItem->Caption = ExItem->Caption;
		pItem->Data = ExItem->Data;

		ExItem->Caption = s1;
		ExItem->Data = o1;
	}
	DataList->Selected = ExItem;
	ShowListSelection();
	ExItem->Focused = true;
}
//---------------------------------------------------------------------------


void __fastcall TListEditForm::_PlayerListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
      TOwnerDrawState State)
{
 /*	PlayerListBox->Canvas->Font->Charset = RUSSIAN_CHARSET;
	HDC dc = PlayerListBox->Canvas->Handle;
    String str = PlayerListBox->Items->Strings[Index];
	TextOut(dc,Rect.left+2,Rect.top,str.c_str(),str.Length());   */
}
//---------------------------------------------------------------------------

void TListEditForm::DoUpdateData()
{
	 //PropPageFrame->SendMessage(DCMM_SELECTION_DATA_CHANGED,m_SelRef.id,m_SelRef.age);
	 PropPageFrame->SelectRef(m_SelRef);
}

void __fastcall TListEditForm::DataListSelectItem(TObject *Sender,
	  TListItem *Item, bool Selected)
{
	if(!Selected)return;

	String Sel = Item->Caption;

	//if(PlayerListBox->ItemIndex < 0)return;
	//String Sel = PlayerListBox->Items->Strings[PlayerListBox->ItemIndex];

	TDataRefObject* SelRef = (TDataRefObject*)Item->Data;
	if(SelRef->ref != m_SelRef || m_bKeyChanged)
	{
		if(m_pData->SelectID(SelRef->ref))
		{
			m_SelRef = *m_pData->GetID();

			if(!SelPlayer.IsEmpty())
			{
				if(!m_bInsertMode)ApplyButtonClick(Sender);
				if(m_bError)
				{
					DataList->Selected = DataList->FindCaption(0,SelPlayer,false,true,false);
					//DataList->ItemIndex = DataList->Items->IndexOf()
					//PlayerListBox->ItemIndex =  PlayerListBox->Items->IndexOf(SelPlayer);
					return;
				}
			}
			DoUpdateData();
			SelPlayer = Sel;
			PropPageFrame->Show();
			DeleteButton->Enabled = true;
		}
	}

}
//---------------------------------------------------------------------------


