inherited RaceEditPropFrame: TRaceEditPropFrame
  Width = 341
  Height = 212
  ExplicitWidth = 341
  ExplicitHeight = 212
  object MinGrowthEdit: TsDecimalSpinEdit [0]
    Left = 232
    Top = 180
    Width = 105
    Height = 21
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    OnChange = DataChanged
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1052#1080#1085#1080#1084#1072#1083#1100#1085#1099#1081' '#1087#1088#1080#1088#1086#1089#1090
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Increment = 1.000000000000000000
    MaxValue = 100.000000000000000000
  end
  object SecondEdit: TsComboBoxEx [1]
    Left = 232
    Top = 56
    Width = 105
    Height = 24
    BoundLabel.Active = True
    BoundLabel.Caption = #1042#1090#1086#1088#1080#1095#1085#1099#1081' '#1088#1077#1089#1091#1088#1089
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 18
    ParentFont = False
    TabOrder = 3
    OnChange = SecondEditChange
    Images = InterfaceModule.RsrsImageList
    ItemsEx = <
      item
        Caption = #1054#1088#1075#1072#1085#1080#1082#1072
        ImageIndex = 0
      end
      item
        Caption = #1069#1085#1077#1088#1075#1080#1103
        ImageIndex = 1
      end
      item
        Caption = #1052#1080#1085#1077#1088#1072#1083#1099
        ImageIndex = 2
      end>
    SkinData.SkinSection = 'COMBOBOX'
  end
  object BasisEdit: TsComboBoxEx [2]
    Left = 120
    Top = 56
    Width = 105
    Height = 24
    BoundLabel.Active = True
    BoundLabel.Caption = #1055#1077#1088#1074#1080#1095#1085#1099#1081' '#1088#1077#1089#1091#1088#1089
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 18
    ParentFont = False
    TabOrder = 2
    OnChange = BasisEditChange
    Images = InterfaceModule.RsrsImageList
    ItemsEx = <
      item
        Caption = #1054#1088#1075#1072#1085#1080#1082#1072
        ImageIndex = 0
      end
      item
        Caption = #1069#1085#1077#1088#1075#1080#1103
        ImageIndex = 1
      end
      item
        Caption = #1052#1080#1085#1077#1088#1072#1083#1099
        ImageIndex = 2
      end>
    SkinData.SkinSection = 'COMBOBOX'
  end
  object NatureEdit: TsComboBoxEx [3]
    Left = 8
    Top = 56
    Width = 105
    Height = 24
    BoundLabel.Active = True
    BoundLabel.Caption = #1055#1088#1080#1088#1086#1076#1072' '#1088#1072#1089#1099
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 18
    ParentFont = False
    TabOrder = 1
    OnChange = NatureEditChange
    Images = InterfaceModule.RsrsImageList
    ItemsEx = <
      item
        Caption = #1054#1088#1075#1072#1085#1080#1082#1072
        ImageIndex = 0
      end
      item
        Caption = #1069#1085#1077#1088#1075#1080#1103
        ImageIndex = 1
      end
      item
        Caption = #1052#1080#1085#1077#1088#1072#1083#1099
        ImageIndex = 2
      end>
    SkinData.SkinSection = 'COMBOBOX'
  end
  object MningEdit: TsDecimalSpinEdit [4]
    Left = 176
    Top = 152
    Width = 161
    Height = 21
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    Text = '0,0'
    OnChange = DataChanged
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1044#1086#1073#1099#1095#1072' '#1088#1077#1089#1091#1088#1089#1086#1074
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Increment = 1.000000000000000000
    DecimalPlaces = 1
  end
  object GrowthEdit: TsDecimalSpinEdit [5]
    Left = 8
    Top = 152
    Width = 153
    Height = 21
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    Text = '0,0'
    OnChange = DataChanged
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1041#1086#1085#1091'c '#1087#1088#1080#1088#1086#1089#1090#1072
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Increment = 1.000000000000000000
    DecimalPlaces = 1
  end
  object NameEdit: TsEdit [6]
    Left = 8
    Top = 16
    Width = 329
    Height = 21
    Color = clWhite
    Font.Charset = RUSSIAN_CHARSET
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
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object MinTEdit: TsDecimalSpinEdit [7]
    Left = 8
    Top = 104
    Width = 153
    Height = 21
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Text = '0,00'
    OnChange = DataChanged
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1052#1080#1085'. '#1090#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Increment = 1.000000000000000000
    MaxValue = 200.000000000000000000
    MinValue = -100.000000000000000000
  end
  object MaxTEdit: TsDecimalSpinEdit [8]
    Left = 176
    Top = 104
    Width = 161
    Height = 21
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    Text = '0,00'
    OnChange = DataChanged
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1052#1072#1082#1089'. '#1090#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Increment = 1.000000000000000000
    MaxValue = 200.000000000000000000
    MinValue = -100.000000000000000000
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 232
    Top = 65520
  end
end
