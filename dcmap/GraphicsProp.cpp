//$$---- Form CPP ----
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GraphicsProp.h"
#include "Clipbrd.hpp"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseModule"
#pragma link "sFrameAdapter"
#pragma link "sAlphaListBox"
#pragma link "sColorSelect"
#pragma link "sGroupBox"
#pragma link "sSpeedButton"
#pragma link "sLabel"
#pragma link "sTrackBar"
#pragma link "sCheckBox"
#pragma link "sEdit"
#pragma link "sRadioButton"
#pragma link "sComboBox"
#pragma link "sMemo"
#pragma link "sSpinEdit"
#pragma resource "*.dfm"
TGraphicsEditFrame *GraphicsEditFrame;

static CFramePanelWrapper<TGraphicsEditFrame> sWrap("PListEditPropPage_graphics","BaseInfo");

//---------------------------------------------------------------------------
__fastcall TGraphicsEditFrame::TGraphicsEditFrame(TComponent* Owner)
	: TBaseModuleFrame(Owner)
{
    Caption = "Параметры";
}
//---------------------------------------------------------------------------
bool TGraphicsEditFrame::OnInit()
{
    IDCMapIconsDataStorage* m_pIcons;

	if(!m_pLayers.init(m_pCore,"layers"))return false;
	if(!m_pGraphics.init(m_pCore,"graphics"))return false;

	if(m_pLayers->SelectAll())
	do
	{
		TMenuItem* item = new TMenuItem(this);
		item->Caption = m_pLayers->GetWideName();
		item->Hint = m_pLayers->GetWideName();
		item->OnClick = TransferLayerClick;
		LayerMenu->Items->Add(item);
	}
	while(m_pLayers->Next());
	return true;
}

int TGraphicsEditFrame::OnMessage(int Msg,DWORD wParam,DWORD lParam,IDCMapObject* pSender)
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

void TGraphicsEditFrame::UpdateData()
{
    if(!m_pGraphics->SelectID(m_Ref)) return;

	DCMapGraphicsInfo const* pinf = m_pGraphics->GetData();


	NameEdit->OnChange = 0;
	NameEdit->Text = m_pGraphics->GetWideName();
	NameEdit->OnChange = NameEditChange;

	NotesEdit->Text = m_pGraphics[DCMDB_GRAPHICS_NOTES];

	TextEdit->Text = m_pGraphics[DCMDB_GRAPHICS_TEXT];

	LineColor->ColorValue = pinf->line_color&0x00ffffff;
    LineTrns->Position = 255-((pinf->line_color&0xff000000)>>24);

	FillColor->ColorValue = pinf->fill_color&0x00ffffff;
	FillTrns->Position = 255-((pinf->fill_color&0xff000000)>>24);

	TextColor->ColorValue = pinf->text_color&0x00ffffff;

	TextSizeEdit->Value = 	pinf->text_size;

	TextCheck->Checked = pinf->text_type;

    FillCheck->Checked = pinf->fill_mode != 0;

	VisibleCheck->Checked = pinf->visible;

	TextCoordsEdit->Text = (String)pinf->x+":"+pinf->y;

    LineCheck->Checked = true;
    ThinLineRadio->Checked = true;

    if(pinf->line_size >= 4)      	XXLLineRadio->Checked = true;
    else if(pinf->line_size >=  2)  WideLineRadio->Checked = true;
    else if(pinf->line_size >= 1)   ThinLineRadio->Checked = true;
    else                            LineCheck->Checked = false;

    if(pinf->type == DCMAP_GRAPHICS_POLYGON) 	   PolyRadio->Checked = true;
    else if(pinf->type == DCMAP_GRAPHICS_POLYLINE) LineRadio->Checked = true;
	else if(pinf->type == DCMAP_GRAPHICS_ZONES)    ZonesRadio->Checked = true;

	RadiusEdit->Value = pinf->radius;

    LineStyle->ItemIndex = pinf->line_type;

    CoordsList->Clear();
    POINT * pt;
    int n = m_pGraphics->GetBlobN("coords",(BYTE**)&pt)/sizeof(POINT);
    for(int i=0;i<n;i++)
    {
     	String Item = IntToStr((int)pt[i].x)+":"+IntToStr((int)pt[i].y);
        CoordsList->Items->Add(Item);
    }
    CoordsListClick(this);
    Changed=false;
}

int inline WrapCoord(int c){return (c+DCMapNumPlanets-1)%DCMapNumPlanets+1;}

String StrToCoord(String Str)
{
	TPoint pt= StrToPoint(Str);
	return IntToStr((int)pt.x)+":"+IntToStr((int)pt.y);
}

void TGraphicsEditFrame::OnApply()
{
    if(!Changed)return;

	if(!m_pGraphics->SelectID(m_Ref)) return;

	DCMapGraphicsInfo pinf;
	pinf = *m_pGraphics->GetData();

    pinf.line_color = LineColor->ColorValue&0x00ffffff;
    pinf.line_color |= (255-LineTrns->Position)<<24;

	pinf.fill_color = FillColor->ColorValue&0x00ffffff;
	pinf.fill_color |= (255-FillTrns->Position)<<24;

	const int text_trns = 70;

	pinf.text_color = TextColor->ColorValue&0x00ffffff;
	pinf.text_color |= (255-text_trns)<<24;

    if(FillCheck->Checked)
    	pinf.fill_mode = 1;
	else
    	pinf.fill_mode = 0;

	pinf.visible = VisibleCheck->Checked;       

	pinf.text_type = TextCheck->Checked;

	if(!LineCheck->Checked) pinf.line_size = 0;
	else if(XXLLineRadio->Checked)   pinf.line_size = 4;
    else if(WideLineRadio->Checked)  pinf.line_size = 2;
    else if(ThinLineRadio->Checked)  pinf.line_size = 1;

    if(PolyRadio->Checked)pinf.type = DCMAP_GRAPHICS_POLYGON;
	else if(LineRadio->Checked)pinf.type = DCMAP_GRAPHICS_POLYLINE;
	else if(ZonesRadio->Checked)pinf.type = DCMAP_GRAPHICS_ZONES;

	pinf.radius = RadiusEdit->Value;
 
	pinf.line_type = LineStyle->ItemIndex;

    int n = CoordsList->Count;
    TPoint * pt=0;
    if(n)
    {
        pt = new TPoint[n];
        for(int i=0;i<n;i++)
        {
            pt[i] = StrToPoint(CoordsList->Items->Strings[i]);
        }
        pinf.x = pt[0].x;
        pinf.y = pt[0].y;
	}

	pinf.text_size = TextSizeEdit->Value;

	try{pinf.pt = StrToPoint(TextCoordsEdit->Text);}catch(...){}

	m_pGraphics->Update();
	m_pGraphics->SetData(&pinf);

	m_pGraphics[DCMDB_GRAPHICS_NOTES] = NotesEdit->Text;
	
	m_pGraphics[DCMDB_GRAPHICS_TEXT] = TextEdit->Text;
	m_pGraphics->SetWideName(NameEdit->Text.w_str());
    m_pGraphics->SetBlobN("coords",(BYTE*)pt,sizeof(TPoint)*n);
    m_pGraphics->Post();

    if(n)delete[] pt;
	Changed=false;
}

bool TGraphicsEditFrame::OnDataTest()
{
/*
    if(!Changed)return true;
    if(m_pLayers->SelectName(NameEdit->Text.c_str()))
    {
        if(*m_pLayers->GetID() != m_Ref)
        {
            Dialogs::TMsgDlgButtons Buttons;Buttons << mbOK;
            sMessageDlg("Ошибка","Существует другой слой с таким именем!",mtError,Buttons,0);
            NameEdit->SetFocus();
            return false;
        }
    }
*/
    return true;
}

void __fastcall TGraphicsEditFrame::NameEditChange(TObject *Sender)
{
    SetNewKeyValue(NameEdit->Text);
    Changed=true;
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::OnChange(TObject *Sender)
{
	Changed=true;
	RadiusEdit->Enabled = ZonesRadio->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::CoordsListDblClick(TObject *Sender)
{
	// Edit selected value
    if(CoordsList->ItemIndex >= 0)
    {
        String Value = CoordsList->Items->Strings[CoordsList->ItemIndex];
        if(sInputQuery("Изменеие координат",
                "Введите список координат",Value))
        {
        	int pos = CoordsList->ItemIndex;
            //CoordsList->Items->Delete(pos);
            AddCoords(Value,pos,0);
            if(pos >= CoordsList->Count)pos--;
            CoordsList->ItemIndex = pos;
            //Changed=true;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::RemoveCoordBtnClick(TObject *Sender)
{
    if(CoordsList->ItemIndex >= 0)
    {
        int pos = CoordsList->ItemIndex;
        CoordsList->Items->Delete(pos);
        if(pos >= CoordsList->Count)pos--;
        CoordsList->ItemIndex = pos;
        Changed=true;
    }
    CoordsListClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::AddCoordBtnClick(TObject *Sender)
{
	String Value;
    if(sInputQuery("Добавление координат",
    		"Введите список координат",Value))
    {
    	AddCoords(Value,-1,1);
        CoordsList->ItemIndex= CoordsList->Count-1;
    }
    CoordsListClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::CopyBtnClick(TObject *Sender)
{
	//
    String Lst;
    int n = CoordsList->Count;
    for(int i=0;i<n;i++)
    {
		if(i)Lst+=L" – ";
        Lst += CoordsList->Items->Strings[i];
    }
    Clipboard()->SetTextBuf(Lst.w_str());
}
//---------------------------------------------------------------------------

void TGraphicsEditFrame::AddCoords(String Text,int Pos,int Mode)
{
    TStringList* pLst = new TStringList;

    try
    {
    	bool doit=true;
        int d[2];
        int dps=0;

        if(Mode < 2)
        	pLst->AddStrings(CoordsList->Items);

        if(Mode == 0 && Pos >= 0)
        {
            pLst->Delete(Pos);
        }

        String Delimeters = L":;,.-/|–\n";
        while(doit)
        {
            String Coord;
            int delim=-1;
            int sz = Text.Length();
            for(int i=1;i<sz;i++)
            {
                if(Text.IsDelimiter(Delimeters,i))
                {
                    delim=i;
                    break;
                }
            }
            if(delim <=0)
            {
                Coord = Text;
                doit = false;
            }
            else
            {
                Coord = Text.SubString(1,delim-1);
                Text = Text.SubString(delim+1,Text.Length());
            }
            Coord = Coord.Trim();
            if(Coord.Length())
            {
                d[dps] = StrToInt(Coord);
                dps++;
            }
            else continue;

            if(dps >= 2)
            {
                dps=0;
                String CoordPos = IntToStr(d[0])+":"+IntToStr(d[1]);
                if(Pos<0)
                    pLst->Add(CoordPos);
                else if(Mode <= 0)
                {
                    pLst->Insert(Pos,CoordPos);
                    Pos++;
                }
                else
                {
                    pLst->Insert(Pos+1,CoordPos);
                    Pos++;
                }
            }
        }
        if(dps !=0) throw(0);
    }
    catch(...)
    {
        sMessageDlg("Ошибка","Координаты не распознаны!",mtError,TMsgDlgButtons() << mbOK,0);
    	delete pLst;
        return;
    }
    if(Mode<0)Pos--;
    int index = Pos;
    if(index <0)index = CoordsList->ItemIndex;
    int top = CoordsList->TopIndex;
    CoordsList->Items->Clear();
    CoordsList->Items->AddStrings(pLst);
    if(index >= CoordsList->Count)index = CoordsList->Count-1;
    CoordsList->TopIndex = top;
    CoordsList->ItemIndex = index;
    delete pLst;
    Changed=true;
}

void __fastcall TGraphicsEditFrame::PasteBtnClick(TObject *Sender)
{
	if(!Clipboard()->HasFormat(CF_TEXT)) return;
    String Text = Clipboard()->AsText;
    //CoordsList->Clear();
    AddCoords(Text,-1,2);
    CoordsListClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::CoordsListKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if(Key == VK_DELETE)
    {
    	RemoveCoordBtnClick(Sender);
    }
    else if(Key == VK_SPACE)
    {
    	CoordsListDblClick(Sender);
    }
    /*else if(Key == VK_INSERT)
    {
    	// Edit selected value
        if(CoordsList->ItemIndex >= 0)
        {
            String Value;
            if(sInputQuery("Вставка координат",
                    "Введите список координат",Value))
            {
                int pos = CoordsList->ItemIndex+1;
                AddCoords(Value,pos);
                if(pos >= CoordsList->Count)pos--;
                CoordsList->ItemIndex = pos;
                Changed=true;
            }
            CoordsListDblClick(Sender);
        }
    }*/
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::InsertAfterClick(TObject *Sender)
{
	String Value;
    if(sInputQuery("Добавление координат",
    		"Введите список координат",Value))
    {
        int pos = CoordsList->ItemIndex;
    	AddCoords(Value,pos,1);
    }
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::InsertBeforeClick(TObject *Sender)
{
	String Value;
    if(sInputQuery("Добавление координат",
    		"Введите список координат",Value))
    {
        int pos = CoordsList->ItemIndex;
    	AddCoords(Value,pos,-1);
    }
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::CoordsListClick(TObject *Sender)
{
    bool e = CoordsList->ItemIndex >= 0;

    RemoveCoordBtn->Enabled = e;
    InsertAfter->Enabled = e;
    InsertBefore->Enabled = e;
    UpBtn->Enabled = e;
    DownBtn->Enabled = e;

    if(e)
    {
    	if(CoordsList->ItemIndex == 0)
    		UpBtn->Enabled = false;
        if(CoordsList->ItemIndex == CoordsList->Count-1)
    		DownBtn->Enabled = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::UpBtnClick(TObject *Sender)
{
	if(CoordsList->ItemIndex <= 0)return;

    String str1 =  CoordsList->Items->Strings[CoordsList->ItemIndex];
    String str2 =  CoordsList->Items->Strings[CoordsList->ItemIndex-1];
    CoordsList->Items->Strings[CoordsList->ItemIndex] = str2;
    CoordsList->Items->Strings[CoordsList->ItemIndex-1] = str1;

    CoordsList->ItemIndex--;

    Changed = true;

    CoordsListClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::DownBtnClick(TObject *Sender)
{
	if(CoordsList->ItemIndex < 0)return;
	if(CoordsList->ItemIndex >= CoordsList->Count-1)return;

    String str1 =  CoordsList->Items->Strings[CoordsList->ItemIndex];
    String str2 =  CoordsList->Items->Strings[CoordsList->ItemIndex+1];
    CoordsList->Items->Strings[CoordsList->ItemIndex] = str2;
    CoordsList->Items->Strings[CoordsList->ItemIndex+1] = str1;

    CoordsList->ItemIndex++;

    Changed = true;

    CoordsListClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::TransferLayerClick(TObject *Sender)
{
	TMenuItem* item = dynamic_cast<TMenuItem*>(Sender);
	if(!item)return;

	ApplyChanges();
	if(!m_pLayers->SelectWideName(UnicodeString(item->Hint).w_str()))return;
	if(!m_pGraphics->SelectID(m_Ref)) return;

	if(m_pGraphics[DCMDB_GRAPHICS_LAYER] == *m_pLayers->GetID())return;

	if(TransferCopyMode)
	{
		if(!m_pGraphics->Duplicate()) return;
		m_pGraphics[DCMDB_GRAPHICS_LAYER] = *m_pLayers->GetID();
		m_pGraphics->Post();
	}
	else  	// moving to layer
	{
		m_pGraphics->Update();
		m_pGraphics[DCMDB_GRAPHICS_LAYER] = *m_pLayers->GetID();
		m_pGraphics->Post();
		UpdateList();
    }
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::CopyLayerBtnMouseActivate(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y, int HitTest,
	  TMouseActivate &MouseActivate)
{
	TransferCopyMode=true;

}
//---------------------------------------------------------------------------

void __fastcall TGraphicsEditFrame::MoveLayerBtnMouseActivate(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y, int HitTest,
	  TMouseActivate &MouseActivate)
{
	TransferCopyMode=false;

}
//---------------------------------------------------------------------------

