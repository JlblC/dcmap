//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GraphHelper.h"
#include "Interface.h"
#include "GraphicToolbar.h"
#include "UISystem.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sAlphaListBox"
#pragma link "sButton"
#pragma link "sCheckListBox"
#pragma link "sSkinProvider"
#pragma link "sColorSelect"
#pragma link "sComboBoxes"
#pragma link "sEdit"
#pragma link "sGroupBox"
#pragma link "sSpeedButton"
#pragma link "sSpinEdit"
#pragma link "sToolBar"
#pragma link "sPanel"
#pragma resource "*.dfm"
TGraphHelperForm *GraphHelperForm=0;
//---------------------------------------------------------------------------
__fastcall TGraphHelperForm::TGraphHelperForm(TComponent* Owner)
	: TBaseViewPopupForm(Owner)
{
	m_bUpdating=false;
}
//---------------------------------------------------------------------------

TGraphHelperForm* TGraphHelperForm::CreateInstance(IDCMapWorkspace* pWorkspace)
{
	TGraphHelperForm* form = new TGraphHelperForm(static_cast<IDCMapVCLFrameUI*>(pWorkspace->GetFrameUI())->GetForm());
	form->SetupInstance(pWorkspace->GetCore(),pWorkspace);
	return form;
}
void TGraphHelperForm::SetupSize()
{
	 int h = 0;

	 if(FillGroupBox->Visible) h += FillGroupBox->Height+2;
	 if(LinesGroupBox->Visible) h += LinesGroupBox->Height+2;
	 if(TextGroupBox->Visible) h += TextGroupBox->Height+2;

	 h += BottomPanel->Height;

	 ClientHeight=h;
}


void TGraphHelperForm::Setup(TGraphicToolbarFrame* pToolbar,IDCMapGraphicLayer* layer)
{
	 m_pToolbar = pToolbar;
	 m_pLayer = layer;
	 DCMapGraphicsInfo* pinf = m_pLayer->GetGraphicsInfo();

	 LinesGroupBox->Visible = true;
	 FillGroupBox->Visible = pinf->type != DCMAP_GRAPHICS_POLYLINE;
	 TextGroupBox->Visible = false;

	 SetupSize();

	 FillRadius->Visible = pinf->type == DCMAP_GRAPHICS_ZONES;
	 UpdateData(pinf);

	 PopupMode = pmExplicit;
	 PopupParent = Application->MainForm;
}

void TGraphHelperForm::UpdateData(DCMapGraphicsInfo* pinf)
{
	m_bUpdating = true;
	m_GInfo = *pinf;

	LineColor->ColorValue = pinf->line_color&0x00ffffff;
	LineTrns->Value = (100.0/255.0)*(255-((pinf->line_color&0xff000000)>>24));
	LineWidth->Value = pinf->line_size;
	LineStyle->ItemIndex = pinf->line_type;

	FillColor->ColorValue = pinf->fill_color&0x00ffffff;
	FillTrns->Value = (100.0/255.0)*(255-((pinf->fill_color&0xff000000)>>24));

	TextColor->ColorValue = pinf->text_color&0x00ffffff;
	TextTrns->Value = (100.0/255.0)*(255-((pinf->text_color&0xff000000)>>24));

	TextSize->Value = pinf->text_size;

	FillRadius->Value = pinf->radius;

	m_bUpdating = false;
}

void __fastcall TGraphHelperForm::ParamsChnage(TObject *Sender)
{
	if(m_bUpdating)return;

	m_GInfo.line_color = LineColor->ColorValue&0x00ffffff;
	m_GInfo.line_color |= (255-DWORD(LineTrns->Value*(255.0/100.0)))<<24;
	m_GInfo.line_size = LineWidth->Value;
	m_GInfo.line_type = LineStyle->ItemIndex;


	m_GInfo.fill_color = FillColor->ColorValue&0x00ffffff;
	m_GInfo.fill_color |= (255-DWORD((255.0/100.0)*FillTrns->Value))<<24;

	m_GInfo.text_color = TextColor->ColorValue&0x00ffffff;
	m_GInfo.text_color |= (255-DWORD((255.0/100.0)*TextTrns->Value))<<24;

	m_GInfo.text_size = TextSize->Value;

	m_GInfo.radius = FillRadius->Value;

	m_pLayer->UpdateGraphicsInfo(&m_GInfo);
}
//---------------------------------------------------------------------------

void __fastcall TGraphHelperForm::OkbtnClick(TObject *Sender)
{
	m_pLayer->EndDraw();
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TGraphHelperForm::CancelBtnClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TGraphHelperForm::TntFormClose(TObject *Sender,
	  TCloseAction &Action)
{
	m_pLayer->CancelDraw();
	if(m_pToolbar)
	{
		m_pToolbar->DoEndDraw();
	}
	Action = caFree;
	OnClose();
}
//---------------------------------------------------------------------------

void __fastcall TGraphHelperForm::UndoBtnClick(TObject *Sender)
{
	m_pLayer->Undo();
}
//---------------------------------------------------------------------------

void __fastcall TGraphHelperForm::TextBtnClick(TObject *Sender)
{
	// add text
	m_pLayer->PlaceText(TextBtn->Down);
}
//---------------------------------------------------------------------------
void TGraphHelperForm::TextPlaced(bool ok)
{
	TextBtn->Down = false;
	TextGroupBox->Visible = ok;
	SetupSize();

	UpdateData(m_pLayer->GetGraphicsInfo());
}


