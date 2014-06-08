inherited WebFFViewFrame: TWebFFViewFrame
  Width = 617
  Height = 369
  ExplicitWidth = 617
  ExplicitHeight = 369
  object Browser: TGeckoBrowser [0]
    Left = 0
    Top = 29
    Width = 617
    Height = 340
    Align = alClient
    TabOrder = 0
    TabStop = False
    ExplicitLeft = 40
    ExplicitTop = 88
    ExplicitWidth = 505
    ExplicitHeight = 249
  end
  object sToolBar1: TsToolBar [1]
    Left = 0
    Top = 0
    Width = 617
    Height = 29
    Caption = 'sToolBar1'
    TabOrder = 1
    SkinData.SkinSection = 'TOOLBAR'
    ExplicitLeft = 208
    ExplicitTop = 48
    ExplicitWidth = 150
    object ToolButton1: TToolButton
      Left = 0
      Top = 0
      Caption = 'ToolButton1'
      ImageIndex = 0
      OnClick = ToolButton1Click
    end
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 32
    Top = 16
  end
end
