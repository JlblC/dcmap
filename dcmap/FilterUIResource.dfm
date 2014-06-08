inherited FilterUIResourceFrame: TFilterUIResourceFrame
  Width = 167
  Height = 46
  ExplicitWidth = 167
  ExplicitHeight = 46
  object NameCheck: TsCheckBox [0]
    Left = 3
    Top = 1
    Width = 62
    Height = 20
    Caption = #1056#1077#1089#1091#1088#1089
    Checked = True
    State = cbChecked
    TabOrder = 0
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
    WordWrap = False
  end
  object OpCombo1: TsComboBox [1]
    Left = 3
    Top = 20
    Width = 39
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
    SkinData.SkinSection = 'COMBOBOX'
    Style = csDropDownList
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 13
    ItemIndex = 1
    ParentFont = False
    TabOrder = 1
    Text = '>='
    Items.Strings = (
      '='
      '>='
      '<='
      '<'
      '>')
  end
  object ValueEdit1: TsDecimalSpinEdit [2]
    Left = 45
    Top = 20
    Width = 119
    Height = 22
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = '0,00'
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    Increment = 1.000000000000000000
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 160
    Top = 192
  end
end
