inherited MapLayersFrame: TMapLayersFrame
  Width = 160
  Height = 123
  ExplicitWidth = 160
  ExplicitHeight = 123
  object LayersListBox: TsCheckListBox [0]
    AlignWithMargins = True
    Left = 5
    Top = 5
    Width = 150
    Height = 111
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    AutoCompleteDelay = 500
    BorderStyle = bsSingle
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'EDIT'
    ItemHeight = 13
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = 16118514
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = LayersListBoxClick
    OnDblClick = LayersListBoxDblClick
    OnClickCheck = LayersListBoxClickCheck
  end
  inherited BottomSizeBar: TsDragBar
    Top = 118
    Width = 160
  end
  inherited sFrameAdapter: TsFrameAdapter
    SkinData.SkinSection = 'BARPANEL'
    Left = 120
    Top = 240
  end
end
