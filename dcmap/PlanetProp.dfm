inherited PlanetParamFrame: TPlanetParamFrame
  Width = 308
  Height = 343
  ExplicitWidth = 308
  ExplicitHeight = 343
  object sSpeedButton1: TsSpeedButton [0]
    Left = 257
    Top = 18
    Width = 33
    Height = 22
    Caption = '...'
    OnClick = sSpeedButton1Click
    SkinData.SkinSection = 'SPEEDBUTTON'
  end
  object PlanetColor2: TsColorSelect [1]
    Left = 112
    Top = 177
    Width = 81
    Height = 22
    BiDiMode = bdLeftToRight
    Caption = #1062#1074#1077#1090' 2  '
    Glyph.Data = {
      06030000424D060300000000000036000000280000000F0000000F0000000100
      180000000000D002000000000000000000000000000000000000FF00FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF00FF00000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF00FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF00FF000000}
    Layout = blGlyphRight
    ParentBiDiMode = False
    SkinData.SkinSection = 'SPEEDBUTTON'
    OnChange = DataChange
    ColorValue = clBlack
  end
  object PlanetColor1: TsColorSelect [2]
    Left = 8
    Top = 177
    Width = 81
    Height = 22
    BiDiMode = bdLeftToRight
    Caption = #1062#1074#1077#1090' 1  '
    Glyph.Data = {
      06030000424D060300000000000036000000280000000F0000000F0000000100
      180000000000D002000000000000000000000000000000000000FF00FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF00FF00000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF00FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF00FF000000}
    Layout = blGlyphRight
    ParentBiDiMode = False
    SkinData.SkinSection = 'SPEEDBUTTON'
    OnChange = DataChange
    ColorValue = clBlack
  end
  object KeepNameBtn: TsSpeedButton [3]
    Left = 251
    Top = 59
    Width = 23
    Height = 21
    AllowAllUp = True
    GroupIndex = 1
    OnClick = DataChange
    SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
    ShowCaption = False
  end
  object SynchronizeBtn: TsSpeedButton [4]
    Left = 276
    Top = 59
    Width = 23
    Height = 21
    AllowAllUp = True
    GroupIndex = 1
    OnClick = DataChange
    SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
    ShowCaption = False
  end
  object NameEdit: TsEdit [5]
    Left = 8
    Top = 59
    Width = 238
    Height = 21
    TabOrder = 10
    Text = 'NameEdit'
    OnChange = NameEditChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'NameEdit'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object EditTurn: TsSpinEdit [6]
    Left = 216
    Top = 147
    Width = 81
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    MaxLength = 5
    ParentFont = False
    TabOrder = 9
    Text = '0'
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1061#1086#1076
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    MaxValue = 65535
    MinValue = 0
    Value = 0
  end
  object IconCombo2: TsComboBoxEx [7]
    Left = 159
    Top = 218
    Width = 138
    Height = 22
    BoundLabel.Active = True
    BoundLabel.Caption = #1044#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1099#1081' '#1079#1085#1072#1095#1086#1082
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    OnChange = DataChange
    Images = InterfaceModule.IconsImageList
    ItemsEx = <>
    SkinData.SkinSection = 'COMBOBOX'
  end
  object IconCombo1: TsComboBoxEx [8]
    Left = 8
    Top = 218
    Width = 138
    Height = 22
    BoundLabel.Active = True
    BoundLabel.Caption = #1054#1089#1085#1086#1074#1085#1086#1081' '#1079#1085#1072#1095#1086#1082
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnChange = DataChange
    Images = InterfaceModule.IconsImageList
    ItemsEx = <>
    SkinData.SkinSection = 'COMBOBOX'
  end
  object OwnerComboBox: TsComboBox [9]
    Left = 8
    Top = 19
    Width = 238
    Height = 21
    Alignment = taLeftJustify
    BoundLabel.Active = True
    BoundLabel.Caption = #1061#1086#1079#1103#1080#1085' '#1087#1083#1072#1085#1077#1090#1099':'
    BoundLabel.Indent = 2
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'COMBOBOX'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 0
    ItemIndex = -1
    ParentFont = False
    Sorted = True
    TabOrder = 8
    OnChange = DataChange
  end
  object EditO: TsSpinEdit [10]
    Left = 8
    Top = 107
    Width = 81
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    MaxLength = 3
    ParentFont = False
    TabOrder = 0
    Text = '0'
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1054#1088#1075#1072#1085#1080#1082#1072
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    MaxValue = 100
    MinValue = 0
    Value = 0
  end
  object EditM: TsSpinEdit [11]
    Left = 216
    Top = 107
    Width = 81
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    MaxLength = 3
    ParentFont = False
    TabOrder = 2
    Text = '0'
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1052#1080#1085#1077#1088#1072#1083#1099
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    MaxValue = 100
    MinValue = 0
    Value = 0
  end
  object EditE: TsSpinEdit [12]
    Left = 112
    Top = 107
    Width = 81
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    MaxLength = 3
    ParentFont = False
    TabOrder = 1
    Text = '0'
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'Energy'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    MaxValue = 100
    MinValue = 0
    Value = 0
  end
  object EditT: TsSpinEdit [13]
    Left = 8
    Top = 147
    Width = 81
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    MaxLength = 3
    ParentFont = False
    TabOrder = 3
    Text = '0'
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    MaxValue = 100
    MinValue = 0
    Value = 0
  end
  object EditS: TsSpinEdit [14]
    Left = 112
    Top = 147
    Width = 81
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    MaxLength = 3
    ParentFont = False
    TabOrder = 4
    Text = '0'
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1056#1072#1079#1084#1077#1088
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    MaxValue = 100
    MinValue = 0
    Value = 0
  end
  object CommentMemo: TsMemo [15]
    Left = 3
    Top = 258
    Width = 294
    Height = 71
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    OnChange = DataChange
    BoundLabel.Active = True
    BoundLabel.Caption = #1055#1088#1080#1084#1077#1095#1072#1085#1080#1103
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'EDIT'
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 392
  end
end
