inherited ApperancePrefsFrame: TApperancePrefsFrame
  Width = 376
  Height = 311
  ExplicitWidth = 376
  ExplicitHeight = 311
  object HueLabel: TsLabel [0]
    Left = 193
    Top = 203
    Width = 82
    Height = 13
    Caption = #1062#1074#1077#1090#1086#1074#1086#1081' '#1089#1076#1074#1080#1075
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object SatLabel: TsLabel [1]
    Left = 9
    Top = 203
    Width = 70
    Height = 13
    Caption = #1053#1072#1089#1099#1097#1077#1085#1086#1089#1090#1100
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object SkinEnableCheck: TsCheckBox [2]
    Left = 9
    Top = 23
    Width = 138
    Height = 20
    Caption = #1048#1089#1087#1086#1083#1100#1079#1086#1074#1072#1090#1100' "'#1064#1082#1091#1088#1099'"'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = SkinEnableCheckClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object SkinList: TsListBox [3]
    Left = 9
    Top = 50
    Width = 345
    Height = 147
    AutoCompleteDelay = 500
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
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = SkinListClick
  end
  object HueBar: TsTrackBar [4]
    Left = 184
    Top = 222
    Width = 170
    Height = 45
    LineSize = 10
    Max = 360
    PageSize = 20
    Frequency = 20
    TabOrder = 2
    OnChange = HueBarChange
    SkinData.SkinSection = 'TRACKBAR'
  end
  object SatBar: TsTrackBar [5]
    Left = 1
    Top = 219
    Width = 175
    Height = 45
    LineSize = 20
    Max = 100
    Min = -100
    PageSize = 50
    Frequency = 25
    TabOrder = 3
    OnChange = SatBarChange
    SkinData.SkinSection = 'TRACKBAR'
  end
  object AlwaysShowTabsCheck: TsCheckBox [6]
    Left = 9
    Top = 265
    Width = 321
    Height = 20
    Caption = #1042#1089#1077#1075#1076#1072' '#1087#1086#1082#1072#1079#1099#1074#1072#1090#1100' '#1074#1082#1083#1072#1076#1082#1080
    AutoSize = False
    TabOrder = 4
    OnClick = OnChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object AnimationCheck: TsCheckBox [7]
    Left = 207
    Top = 265
    Width = 96
    Height = 20
    Caption = 'AnimationCheck'
    TabOrder = 5
    OnClick = OnChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object PreviewCheck: TsCheckBox [8]
    Left = 9
    Top = -3
    Width = 87
    Height = 20
    Caption = 'PreviewCheck'
    TabOrder = 6
    OnClick = PreviewCheckClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object DownloadSkinsBtn: TsBitBtn [9]
    Left = 184
    Top = 3
    Width = 170
    Height = 42
    Caption = 'DownloadSkinsBtn'
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 7
    OnClick = DownloadSkinsBtnClick
    Alignment = taLeftJustify
    Blend = 50
    SkinData.SkinSection = 'WEBBUTTON'
    Grayed = True
    Reflected = True
  end
  object AeroBordersCheck: TsCheckBox [10]
    Left = 9
    Top = 288
    Width = 109
    Height = 20
    Caption = 'AeroBordersCheck'
    TabOrder = 8
    Visible = False
    OnClick = OnChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 337
    Top = 276
  end
end
