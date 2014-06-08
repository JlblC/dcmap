inherited MapPrefsFrame: TMapPrefsFrame
  Width = 349
  Height = 257
  ExplicitWidth = 349
  ExplicitHeight = 257
  object sGroupBox1: TsGroupBox [0]
    Left = 11
    Top = 171
    Width = 254
    Height = 59
    Caption = 'sGroupBox1'
    TabOrder = 6
    SkinData.SkinSection = 'GROUPBOX'
    object ToolTipDelay: TsSpinEdit
      Left = 165
      Top = 21
      Width = 73
      Height = 22
      Color = 16118514
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Text = '800'
      OnChange = OnChanged
      SkinData.SkinSection = 'EDIT'
      BoundLabel.Active = True
      BoundLabel.Caption = #1047#1072#1076#1077#1088#1078#1082#1072' ('#1084#1080#1083#1083#1080#1089#1077#1082#1091#1085#1076#1099')'
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = DEFAULT_CHARSET
      BoundLabel.Font.Color = 5391682
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclLeft
      BoundLabel.MaxWidth = 0
      BoundLabel.UseSkinColor = True
      MaxValue = 10000
      MinValue = 0
      Value = 800
    end
  end
  object HighQuality: TsCheckBox [1]
    Left = 16
    Top = 44
    Width = 134
    Height = 19
    Caption = #1057#1075#1083#1072#1078#1080#1074#1072#1090#1100' '#1087#1083#1072#1085#1077#1090#1099
    TabOrder = 0
    OnClick = OnChanged
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object SelectEmptySpace: TsCheckBox [2]
    Left = 16
    Top = 18
    Width = 154
    Height = 19
    Caption = #1042#1099#1073#1080#1088#1072#1090#1100' '#1087#1091#1089#1090#1099#1077' '#1103#1095#1077#1081#1082#1080
    Checked = True
    State = cbChecked
    TabOrder = 1
    OnClick = OnChanged
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object SwapButtons: TsCheckBox [3]
    Left = 16
    Top = 69
    Width = 188
    Height = 19
    Caption = #1055#1086#1084#1077#1085#1103#1090#1100' '#1084#1077#1089#1090#1072#1084#1080' '#1082#1085#1086#1087#1082#1080' '#1084#1099#1096#1080
    TabOrder = 2
    OnClick = OnChanged
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object ClassicHLine: TsCheckBox [4]
    Left = 16
    Top = 94
    Width = 185
    Height = 19
    Caption = #1058#1086#1085#1082#1072#1103' '#1075#1086#1080#1079#1086#1085#1090#1072#1083#1100#1085#1072#1103' '#1083#1080#1085#1077#1081#1082#1072
    TabOrder = 3
    OnClick = OnChanged
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object RightClickSelect: TsCheckBox [5]
    Left = 16
    Top = 119
    Width = 200
    Height = 19
    Caption = #1042#1099#1073#1080#1088#1072#1090#1100' '#1087#1083#1072#1085#1077#1090#1091' '#1087#1088#1072#1074#1099#1084' '#1082#1083#1080#1082#1086#1084
    TabOrder = 4
    OnClick = OnChanged
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object EnableToolTip: TsCheckBox [6]
    Left = 16
    Top = 169
    Width = 219
    Height = 19
    Caption = #1055#1086#1082#1072#1079#1099#1074#1072#1090#1100' '#1074#1089#1087#1083#1099#1074#1072#1102#1097#1080#1077' '#1087#1086#1076#1089#1082#1072#1079#1082#1080
    Checked = True
    State = cbChecked
    TabOrder = 5
    OnClick = OnChanged
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object LeftMouseDrag: TsCheckBox [7]
    Left = 16
    Top = 144
    Width = 214
    Height = 19
    Caption = #1055#1077#1088#1077#1090#1072#1089#1082#1080#1074#1072#1090#1100' '#1082#1072#1088#1090#1091' '#1083#1077#1074#1086#1081' '#1082#1085#1086#1087#1082#1086#1081
    TabOrder = 7
    OnClick = OnChanged
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 296
    Top = 8
  end
end
