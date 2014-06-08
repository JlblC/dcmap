//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "IconsProp.h"
#include "sDialogs.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sEdit"
#pragma link "sLabel"
#pragma link "sPanel"
#pragma link "sButton"
#pragma link "sDialogs"
#pragma link "sCheckBox"
#pragma resource "*.dfm"
TIconsPropFrame *IconsPropFrame;

static CFramePanelWrapper<TIconsPropFrame> sWrap("PListEditPropPage_icons","BaseInfo");

//---------------------------------------------------------------------------
__fastcall TIconsPropFrame::TIconsPropFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
}
//---------------------------------------------------------------------------
int TIconsPropFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
{
    switch (Msg)
    {
    case DCMM_SELECTION_DATA_CHANGED:
        m_Ref.id = wParam;
        m_Ref.age = lParam;
        UpdateData();
        break;
    }
    return 0;
}
bool TIconsPropFrame::OnInit()
{
    m_pIcons = m_pCore->GetDataSourceTyped<IDCMapIconsDataStorage>("icons");
    if(!m_pIcons)return false;

    return true;
}
void TIconsPropFrame::OnApply()
{
    if(!Changed)return;
    if(!m_pIcons->SelectID(m_Ref)) return;

    m_pIcons->Update();
	m_pIcons->SetWideName(NameEdit->Text.w_str());

     // Load bitmap into buffer
    TMemoryStream* pStream = new TMemoryStream;

    Image->Picture->Bitmap->SaveToStream(pStream);
    m_pIcons->SetBlobN("imgdata",(BYTE*)pStream->Memory,pStream->Size);

    pStream->Clear();
    BigImage->Picture->Bitmap->SaveToStream(pStream);
    m_pIcons->SetBlobN("bigimgdata",(BYTE*)pStream->Memory,pStream->Size);

    m_pIcons->SetIntegerN("transparent",Image->Transparent);

    m_pIcons->Post();
    delete pStream;
    Changed=false;
}
bool TIconsPropFrame::OnDataTest()
{
    if(!Changed)return true;
    if(m_pIcons->SelectWideName(NameEdit->Text.w_str()))
    {
        if(*m_pIcons->GetID() != m_Ref)
        {
            sMessageDlg("Ошибка","Существует другой значок с таким именем!",
            				mtError,TMsgDlgButtons()<<mbOK,0);
            NameEdit->SetFocus();
            return false;
        }
    }
    return true;
}

void TIconsPropFrame::UpdateData()
{
    if(!m_pIcons->SelectID(m_Ref)) return;

    NameEdit->OnChange = 0;
    NameEdit->Text = m_pIcons->GetWideName();
    NameEdit->OnChange = NameEditChange;

    BigImage->Picture->Bitmap->SetSize(0,0);
    Image->Picture->Bitmap->SetSize(0,0);

    TMemoryStream* pStream = new TMemoryStream;
    BYTE* dta=0;
    int sz = m_pIcons->GetBlobN("imgdata",&dta);
    if(sz)
    {
        pStream->Position = 0;
    	pStream->SetSize(sz);
        pStream->Write(dta,sz);
        pStream->Position = 0;
        Image->Picture->Bitmap->LoadFromStream(pStream);
    }

    sz = m_pIcons->GetBlobN("bigimgdata",&dta);
    if(sz)
    {
        pStream->Position = 0;
    	pStream->SetSize(sz);
        pStream->Write(dta,sz);
        pStream->Position = 0;
        BigImage->Picture->Bitmap->LoadFromStream(pStream);
    }
    delete pStream;

    TColor cl = Image->Picture->Bitmap->Canvas->Pixels[0][0];
    TColor cl2 = BigImage->Picture->Bitmap->Canvas->Pixels[0][0];

    Image->Picture->Bitmap->TransparentColor = cl;
    BigImage->Picture->Bitmap->TransparentColor = cl2;

    bool tr = m_pIcons->GetIntegerN("transparent");

    Image->Picture->Bitmap->Transparent  = tr;
    BigImage->Picture->Bitmap->Transparent  = tr;
    TrnsCheck->Checked = tr;

    Changed=false;
}

void __fastcall TIconsPropFrame::NameEditChange(TObject *Sender)
{
    SetNewKeyValue(NameEdit->Text);
    Changed=true;
}

//---------------------------------------------------------------------------
void __fastcall TIconsPropFrame::LoadImageButtonClick(TObject *Sender)
{
	if(OpenDlg->Execute())
    {
        Image->Picture->LoadFromFile(OpenDlg->FileName);
    	Changed=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TIconsPropFrame::TrnsCheckClick(TObject *Sender)
{
	Image->Transparent = TrnsCheck->Checked;
	BigImage->Transparent = TrnsCheck->Checked;
    Changed=true;
}
//---------------------------------------------------------------------------
void __fastcall TIconsPropFrame::LoadBigImageButtonClick(TObject *Sender)
{
	if(OpenDlg->Execute())
	{
		BigImage->Picture->LoadFromFile(OpenDlg->FileName);
		Changed=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TIconsPropFrame::ClearImageButtonClick(TObject *Sender)
{
    Image->Picture->Bitmap->SetSize(0,0);
    Changed=true;
}
//---------------------------------------------------------------------------

void __fastcall TIconsPropFrame::ClearBigImageButtonClick(TObject *Sender)
{
    BigImage->Picture->Bitmap->SetSize(0,0);
    Changed=true;
}
//---------------------------------------------------------------------------

void __fastcall TIconsPropFrame::SaveImageButtonClick(TObject *Sender)
{
	if(SaveDlg->Execute())
    {
		Image->Picture->SaveToFile(SaveDlg->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TIconsPropFrame::SaveBigImageButtonClick(TObject *Sender)
{
	if(SaveDlg->Execute())
	{
		BigImage->Picture->SaveToFile(SaveDlg->FileName);
	}
}
//---------------------------------------------------------------------------

