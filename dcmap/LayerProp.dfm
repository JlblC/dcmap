inherited LayerEditFrame: TLayerEditFrame
  Width = 253
  Height = 145
  ExplicitWidth = 253
  ExplicitHeight = 145
  object ModuleCombo: TsComboBox [0]
    Left = 16
    Top = 72
    Width = 145
    Height = 21
    Alignment = taLeftJustify
    BoundLabel.Active = True
    BoundLabel.Caption = #1052#1086#1076#1091#1083#1100
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
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
    ItemHeight = 0
    ItemIndex = -1
    ParentFont = False
    TabOrder = 1
    OnChange = OnChange
  end
  object NameEdit: TsEdit [1]
    Left = 16
    Top = 32
    Width = 225
    Height = 21
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = 'NameEdit'
    OnChange = NameEditChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1053#1072#1079#1074#1072#1085#1080#1077
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object HiddenCheck: TsCheckBox [2]
    Left = 16
    Top = 99
    Width = 87
    Height = 19
    Caption = 'HiddenCheck'
    TabOrder = 2
    OnClick = OnChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 192
    Top = 88
  end
end
