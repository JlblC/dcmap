inherited ColorSchemeToolbarFrame: TColorSchemeToolbarFrame
  Width = 150
  Height = 26
  ParentShowHint = False
  ShowHint = True
  ExplicitWidth = 150
  ExplicitHeight = 26
  object ColorSchemeCombo: TsComboBox [0]
    Left = 2
    Top = 1
    Width = 145
    Height = 21
    Alignment = taLeftJustify
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'COMBOBOX'
    Style = csDropDownList
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 13
    ItemIndex = -1
    ParentFont = False
    TabOrder = 0
    OnChange = ColorSchemeComboChange
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 320
    Top = 136
  end
end
