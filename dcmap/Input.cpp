//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Utils.h"
#include "sGraphUtils.hpp"

#include "Input.h"
#include "UISystem.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseFormModule"
#pragma link "sSkinProvider"
#pragma link "sBitBtn"
#pragma link "sLabel"
#pragma link "sComboBox"
#pragma link "sEdit"
#pragma resource "*.dfm"
TInputDialog *InputDialog = 0;
//---------------------------------------------------------------------------
__fastcall TInputDialog::TInputDialog(TComponent* Owner)
	: TBaseModuleForm(Owner)
{
	ButtonsWidth=0;
	m_iResult=0;
	InputCombo=0;
	InputEdit=0;
}
//---------------------------------------------------------------------------

void TInputDialog::AddButton(String Caption,int Value,int type)
{
	TsBitBtn* Button = new TsBitBtn(this);
	Button->OnClick = ButtonClick;

	Button->Caption = Caption;
	Button->Parent = this;

	Button->Tag = Value;

	ButtonsWidth += Button->Width;

	VecButtons.push_back(Button);
}

void TInputDialog::AlignButtons()
{
	int n = VecButtons.size();

	int BtnLeft = (ClientWidth-(ButtonsWidth + 10 * n-1))/2;
	for (int i = 0; i < n; i++)
	{
		VecButtons[i]->Top = (ClientHeight-ButtonsHeight)+15;

		VecButtons[i]->Left = BtnLeft;
		BtnLeft += VecButtons[i]->Width+10;
	}
}

void TInputDialog::AddButtons()
{
	String Buttons = SLSTRW(Buttons);
	if(Buttons.IsEmpty() && m_Params)
        	Buttons = m_Params->GetNamedString(L"buttons");

        if(Buttons.IsEmpty())  Buttons = LSTRW(DefaultInputButtons);

	TSysCharSet Delims,BtnDelims;
	Delims << ',';
	BtnDelims << ':';

	int n=1;
	while(true)
	{
	  String Button = ExtractWord(n,Buttons,Delims);
	  if(!Button.Length())break;

	  String Caption =  ExtractWord(1,Button,BtnDelims);
	  if(Caption.IsEmpty())continue;

	  int iVal = 0;

	  String Value =    ExtractWord(2,Button,BtnDelims);
	  if(!Value.IsEmpty()) iVal = StrToInt(Value);

	  AddButton(Caption,iVal,0);
	  n++;
	}
        if(!VecButtons.empty())
        {
            VecButtons.front()->Default = true;
            VecButtons.back()->Cancel = true;
        }
}

void TInputDialog::AddLabel()
{
	String Message = m_Params?dcmapProcessFormatedString(SLSTRW(Message),m_Params,0):SLSTRW(Message);

	if(Message.IsEmpty() && m_Params)
        	Message = m_Params->GetNamedString(L"message");

	TsLabel* MessageLabel = new TsLabel(this);
	MessageLabel->Caption = Message;
	MessageLabel->Margins->Top = 10;
	MessageLabel->Margins->Left = 10;
	MessageLabel->Margins->Right = 10;
	MessageLabel->Margins->Bottom = ButtonsHeight;
	MessageLabel->AlignWithMargins = true;
	MessageLabel->Align = alClient;
	MessageLabel->Alignment = taCenter;
	MessageLabel->Parent = this;

        WorkHeight = MessageLabel->Height + MessageLabel->Margins->Top;
}

void TInputDialog::AddCombo()
{
	String Message = m_Params?dcmapProcessFormatedString(SLSTRW(Message),m_Params,0):SLSTRW(Message);
	if(Message.IsEmpty() && m_Params)
			Message = m_Params->GetNamedString(L"message");

	InputCombo = new TsComboBox(this);
	WorkHeight = InputCombo->Height;

	InputCombo->Margins->Top = 10;
	InputCombo->Margins->Left = 10;
	InputCombo->Margins->Right = 10;
	InputCombo->Margins->Bottom = ButtonsHeight;
	InputCombo->AlignWithMargins = true;
	InputCombo->Align = alClient;
	InputCombo->Parent = this;

	if(!Message.IsEmpty())
	{
		InputCombo->BoundLabel->Caption = Message;
		InputCombo->BoundLabel->Layout = sclTopLeft;
		InputCombo->BoundLabel->Active = true;
		InputCombo->Margins->Top += 15;
	}

	WorkHeight +=  InputCombo->Margins->Top;

	if(m_Params)
	{
		InputCombo->Text = m_Params->GetNamedString(L"value");
		IDCMapVarValueScopedPtr Items = m_Params->GetNamedVar(L"items");
		if(Items)
		{
			int n = Items->Count();
			for(int i=0;i<n;i++)
			{
			   InputCombo->Items->Add(Items->GetString(i));
			}
		}
	}

	ActiveControl = InputCombo;
}

void TInputDialog::AddEdit()
{
	String Message = m_Params?dcmapProcessFormatedString(SLSTRW(Message),m_Params,0):SLSTRW(Message);
	if(Message.IsEmpty() && m_Params)
			Message = m_Params->GetNamedString(L"message");

	InputEdit = new TsEdit(this);
	WorkHeight = InputEdit->Height;

	InputEdit->Margins->Top = 10;
	InputEdit->Margins->Left = 10;
	InputEdit->Margins->Right = 10;
	InputEdit->Margins->Bottom = ButtonsHeight;
	InputEdit->AlignWithMargins = true;
	InputEdit->Align = alClient;
	InputEdit->Parent = this;

	if(!Message.IsEmpty())
	{
		InputEdit->BoundLabel->Caption = Message;
		InputEdit->BoundLabel->Layout = sclTopLeft;
		InputEdit->BoundLabel->Active = true;
		InputEdit->Margins->Top += 15;
	}

	WorkHeight +=  InputEdit->Margins->Top;

	if(m_Params)
	{
		InputEdit->Text = m_Params->GetNamedString(L"value");
	}
	ActiveControl = InputEdit;
}

int TInputDialog::Query(AnsiString Module,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
{
	TInputDialog* dlg = new TInputDialog(SysUI.ActiveFrameForm());
	int r = dlg->DoQuery(Module,pParams,pResult);
	delete dlg;
	return r;
}

int TInputDialog::DoQuery(AnsiString Module,IDCMapVarValue* pParams,IDCMapVarValue* pResult)
{
	m_Params = pParams;
        m_Result = pResult;

	g_pLocalStrings->SelectSection(("Input::"+Module).c_str());

	Caption = pParams?dcmapProcessFormatedString(SLSTRW(Caption),pParams,0):SLSTRW(Caption);

	if(Caption.IsEmpty() && m_Params)
        	Caption = m_Params->GetNamedString(L"caption");
	if(Caption.IsEmpty()) Caption = L"DCMap";

	InputStyle = SLSTRW(Style);
	if(InputStyle.IsEmpty() && m_Params)
		InputStyle = m_Params->GetNamedString(L"style");

	if(InputStyle.IsEmpty())InputStyle = L"Query";

	WorkHeight = 10;

	if(SameStr(InputStyle,L"InputCombo"))
	{
		AddCombo();
	}
	else if(SameStr(InputStyle,L"Input"))
	{
		AddEdit();
	}
	else
	{
		// process buttons
		AddLabel();
	}
	ClientHeight = WorkHeight + ButtonsHeight;

	AddButtons();
	AlignButtons();

	Application->ProcessMessages();

	int mr = ShowModal();

	m_Params = 0;
	return m_iResult;
}

void __fastcall TInputDialog::ButtonClick(TObject *Sender)
{
	TsBitBtn* Button = (TsBitBtn*)Sender;
	m_iResult = Button->Tag;

        if(m_Result)
        {
			if(InputCombo)
			{
			   m_Result->SetNamedString(L"value",InputCombo->Text.w_str());
			   m_Result->SetNamedNumber(L"index",InputCombo->ItemIndex);
			}
			if(InputEdit)
			{
			   m_Result->SetNamedString(L"value",InputEdit->Text.w_str());
			}
        }
        Close();
}
//---------------------------------------------------------------------------
void __fastcall TInputDialog::FormResize(TObject *Sender)
{
	  AlignButtons();
}
//---------------------------------------------------------------------------
