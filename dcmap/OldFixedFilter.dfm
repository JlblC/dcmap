inherited OldFixedFilterFrame: TOldFixedFilterFrame
  Width = 170
  Height = 321
  ExplicitWidth = 170
  ExplicitHeight = 321
  object sLabel1: TsLabel [0]
    Left = 86
    Top = 96
    Width = 6
    Height = 13
    Caption = 'X'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object sLabel2: TsLabel [1]
    Left = 86
    Top = 124
    Width = 6
    Height = 13
    Caption = 'X'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object RectRegionCheck: TsCheckBox [2]
    Left = 6
    Top = 70
    Width = 65
    Height = 20
    Caption = #1057#1077#1082#1090#1086#1088
    TabOrder = 2
    OnClick = RectRegionCheckClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
    WordWrap = False
  end
  object LeftEdit: TsSpinEdit [3]
    Left = 34
    Top = 91
    Width = 46
    Height = 22
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = '0'
    OnChange = RectRegionCheckClick
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1054#1090
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    MaxValue = 0
    MinValue = 0
    Value = 0
  end
  object ComentCheck: TsCheckBox [4]
    Left = 3
    Top = 260
    Width = 152
    Height = 20
    Caption = #1050#1086#1084#1084#1077#1085#1090#1072#1088#1080#1081' '#1089#1086#1076#1077#1088#1078#1080#1090':'
    TabOrder = 15
    OnClick = ComentCheckClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
    WordWrap = False
  end
  object ComentEdit: TsEdit [5]
    Left = 16
    Top = 282
    Width = 146
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 16
    OnChange = ComentCheckClick
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
  end
  object ValueEdit2: TsDecimalSpinEdit [6]
    Left = 70
    Top = 232
    Width = 92
    Height = 22
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 14
    Text = '0,00'
    OnChange = ResourceCheck2Click
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
  object BottomEdit: TsSpinEdit [7]
    Left = 98
    Top = 119
    Width = 42
    Height = 22
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    Text = '0'
    OnChange = RectRegionCheckClick
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    MaxValue = 0
    MinValue = 0
    Value = 0
  end
  object ResourceCombo2: TsComboBox [8]
    Left = 70
    Top = 205
    Width = 92
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
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 13
    ItemIndex = 0
    ParentFont = False
    TabOrder = 13
    Text = #1054#1088#1075#1072#1085#1080#1082#1072
    OnChange = ResourceCheck2Click
    Items.Strings = (
      #1054#1088#1075#1072#1085#1080#1082#1072
      #1069#1085#1077#1088#1075#1080#1103
      #1052#1080#1085#1077#1088#1072#1083#1099
      #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072
      #1056#1072#1079#1084#1077#1088
      #1055#1088#1080#1088#1086#1089#1090
      #1044#1086#1093#1086#1076
      #1044#1086#1073#1099#1095#1072' '#1087#1077#1088#1074'.'
      #1044#1086#1093#1086#1076' '#1089' '#1087#1077#1088#1074'.'
      #1044#1086#1073#1099#1095#1072' '#1074#1090#1086#1088'.'
      #1044#1086#1093#1086#1076' '#1089' '#1074#1090#1086#1088'.')
  end
  object NoFilterCheckBox: TsCheckBox [9]
    Left = 6
    Top = 3
    Width = 94
    Height = 20
    Caption = #1053#1077#1090' '#1092#1080#1083#1100#1090#1088#1072
    Checked = True
    State = cbChecked
    TabOrder = 17
    OnClick = NoFilterCheckBoxClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
    WordWrap = False
  end
  object ResourceCheck2: TsCheckBox [10]
    Left = 3
    Top = 206
    Width = 62
    Height = 20
    Caption = #1056#1077#1089#1091#1088#1089
    TabOrder = 12
    OnClick = ResourceCheck2Click
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
    WordWrap = False
  end
  object ResourceCheck1: TsCheckBox [11]
    Left = 3
    Top = 150
    Width = 62
    Height = 20
    Caption = #1056#1077#1089#1091#1088#1089
    TabOrder = 8
    OnClick = ResourceCheck1Click
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
    WordWrap = False
  end
  object RightEdit: TsSpinEdit [12]
    Left = 34
    Top = 119
    Width = 46
    Height = 22
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    Text = '0'
    OnChange = RectRegionCheckClick
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1044#1086
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    MaxValue = 0
    MinValue = 0
    Value = 0
  end
  object ValueEdit1: TsDecimalSpinEdit [13]
    Left = 71
    Top = 176
    Width = 92
    Height = 22
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    Text = '0,00'
    OnChange = ResourceCheck1Click
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
  object ResourceCombo1: TsComboBox [14]
    Left = 71
    Top = 149
    Width = 92
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
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 13
    ItemIndex = 0
    ParentFont = False
    TabOrder = 7
    Text = #1054#1088#1075#1072#1085#1080#1082#1072
    OnChange = ResourceCheck1Click
    Items.Strings = (
      #1054#1088#1075#1072#1085#1080#1082#1072
      #1069#1085#1077#1088#1075#1080#1103
      #1052#1080#1085#1077#1088#1072#1083#1099
      #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072
      #1056#1072#1079#1084#1077#1088
      #1055#1088#1080#1088#1086#1089#1090
      #1044#1086#1093#1086#1076
      #1044#1086#1073#1099#1095#1072' '#1087#1077#1088#1074'.'
      #1044#1086#1093#1086#1076' '#1089' '#1087#1077#1088#1074'.'
      #1044#1086#1073#1099#1095#1072' '#1074#1090#1086#1088'.'
      #1044#1086#1093#1086#1076' '#1089' '#1074#1090#1086#1088'.')
  end
  object OpCombo1: TsComboBox [15]
    Left = 16
    Top = 176
    Width = 49
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
    ItemIndex = 0
    ParentFont = False
    TabOrder = 9
    Text = '='
    OnChange = ResourceCheck1Click
    Items.Strings = (
      '='
      '>='
      '<='
      '>'
      '<')
  end
  object TopEdit: TsSpinEdit [16]
    Left = 98
    Top = 91
    Width = 42
    Height = 22
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Text = '0'
    OnChange = RectRegionCheckClick
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = clWindowText
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    MaxValue = 0
    MinValue = 0
    Value = 0
  end
  object OwnerCheck: TsCheckBox [17]
    Left = 6
    Top = 27
    Width = 77
    Height = 20
    Caption = #1042#1083#1072#1076#1077#1083#1077#1094
    TabOrder = 0
    OnClick = UpdateFilter
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
    WordWrap = False
  end
  object OpCombo2: TsComboBox [18]
    Left = 16
    Top = 232
    Width = 49
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
    ItemIndex = 0
    ParentFont = False
    TabOrder = 11
    Text = '='
    OnChange = ResourceCheck2Click
    Items.Strings = (
      '='
      '>='
      '<='
      '>'
      '<')
  end
  object PlayerList: TsComboBox [19]
    Left = 16
    Top = 47
    Width = 148
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
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 13
    ItemIndex = -1
    ParentFont = False
    TabOrder = 1
    OnChange = UpdateFilter
  end
  inherited sFrameAdapter: TsFrameAdapter
    SkinData.SkinSection = 'BARPANEL'
    Left = 136
    Top = 360
  end
end
