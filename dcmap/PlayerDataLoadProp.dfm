inherited PlayerAdvPropFrame: TPlayerAdvPropFrame
  Width = 324
  Height = 388
  ExplicitWidth = 324
  ExplicitHeight = 388
  object Manage: TsCheckBox [0]
    Left = 11
    Top = 364
    Width = 58
    Height = 20
    Caption = 'Manage'
    TabOrder = 7
    OnClick = UpdateChecks
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object ImportDip: TsCheckBox [1]
    Left = 11
    Top = 283
    Width = 67
    Height = 20
    Caption = 'ImportDip'
    TabOrder = 8
    OnClick = UpdateChecks
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object UserIDEdit: TsSpinEdit [2]
    Left = 58
    Top = 3
    Width = 75
    Height = 21
    Color = 16118514
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Text = '0'
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'UserIDEdit'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    MaxValue = 0
    MinValue = 0
    Value = 0
  end
  object CorruptionCalcCheck: TsCheckBox [3]
    Left = 11
    Top = 29
    Width = 120
    Height = 20
    Caption = 'CorruptionCalcCheck'
    TabOrder = 0
    OnClick = CorruptionCalcCheckClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object ImportAlienFleets: TsCheckBox [4]
    Left = 11
    Top = 343
    Width = 104
    Height = 20
    Caption = 'ImportAlienFleets'
    TabOrder = 14
    OnClick = UpdateChecks
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object WebAccessCheck: TsCheckBox [5]
    Left = 11
    Top = 108
    Width = 104
    Height = 20
    Caption = 'WebAccessCheck'
    TabOrder = 3
    OnClick = UpdateChecks
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object ImportTrusted: TsCheckBox [6]
    Left = 11
    Top = 127
    Width = 89
    Height = 20
    Caption = 'ImportTrusted'
    TabOrder = 13
    OnClick = UpdateChecks
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object FleetIDEdit: TsSpinEdit [7]
    Left = 198
    Top = 538
    Width = 75
    Height = 21
    Color = 16118514
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    Text = '0'
    Visible = False
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'FleetIdedit'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    MaxValue = 0
    MinValue = 0
    Value = 0
  end
  object IncomeBox: TsGroupBox [8]
    Left = 3
    Top = 487
    Width = 270
    Height = 45
    Caption = 'IncomeBox'
    TabOrder = 6
    SkinData.SkinSection = 'GROUPBOX'
    object BasisImg: TImage
      Left = 8
      Top = 19
      Width = 18
      Height = 18
    end
    object BasisLabel: TsLabel
      Left = 28
      Top = 21
      Width = 30
      Height = 14
      Caption = 'Basis'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
    object SecondImg: TImage
      Left = 91
      Top = 19
      Width = 18
      Height = 18
    end
    object SecondLabel: TsLabel
      Left = 112
      Top = 20
      Width = 45
      Height = 14
      Caption = 'Second'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
    object CreditsImg: TImage
      Left = 186
      Top = 19
      Width = 18
      Height = 18
    end
    object CreditsLabel: TsLabel
      Left = 206
      Top = 20
      Width = 43
      Height = 14
      Caption = 'Credits'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = 5391682
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
  end
  object ImportUntrusted: TsGroupBox [9]
    Left = 3
    Top = 147
    Width = 302
    Height = 136
    Caption = 'ImportUntrusted'
    TabOrder = 12
    SkinData.SkinSection = 'GROUPBOX'
    object AddOccupied: TsCheckBox
      Left = 8
      Top = 15
      Width = 83
      Height = 20
      Caption = 'AddOccupied'
      TabOrder = 0
      OnClick = UpdateChecks
      SkinData.SkinSection = 'CHECKBOX'
      ImgChecked = 0
      ImgUnchecked = 0
    end
    object AddEmpty: TsCheckBox
      Left = 8
      Top = 33
      Width = 69
      Height = 20
      Caption = 'AddEmpty'
      TabOrder = 1
      OnClick = UpdateChecks
      SkinData.SkinSection = 'CHECKBOX'
      ImgChecked = 0
      ImgUnchecked = 0
    end
    object OverwriteEmpty: TsCheckBox
      Left = 8
      Top = 52
      Width = 98
      Height = 20
      Caption = 'OverwriteEmpty'
      TabOrder = 2
      OnClick = UpdateChecks
      SkinData.SkinSection = 'CHECKBOX'
      ImgChecked = 0
      ImgUnchecked = 0
    end
    object OverwriteOccupied: TsCheckBox
      Left = 8
      Top = 70
      Width = 112
      Height = 20
      Caption = 'OverwriteOccupied'
      TabOrder = 3
      OnClick = UpdateChecks
      SkinData.SkinSection = 'CHECKBOX'
      ImgChecked = 0
      ImgUnchecked = 0
    end
    object ClearOccupied: TsCheckBox
      Left = 8
      Top = 89
      Width = 89
      Height = 20
      Caption = 'ClearOccupied'
      TabOrder = 4
      OnClick = UpdateChecks
      SkinData.SkinSection = 'CHECKBOX'
      ImgChecked = 0
      ImgUnchecked = 0
    end
    object ImportRegionEdit: TsEdit
      Left = 112
      Top = 112
      Width = 182
      Height = 21
      TabOrder = 5
      OnChange = UpdateChecks
      SkinData.SkinSection = 'EDIT'
      BoundLabel.Active = True
      BoundLabel.Caption = 'Region'
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = RUSSIAN_CHARSET
      BoundLabel.Font.Color = clWindowText
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclLeft
      BoundLabel.MaxWidth = 0
      BoundLabel.UseSkinColor = True
    end
  end
  object ImportContents: TsCheckBox [10]
    Left = 11
    Top = 303
    Width = 96
    Height = 20
    Caption = 'ImportContents'
    TabOrder = 10
    OnClick = UpdateChecks
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object ImportFleets: TsCheckBox [11]
    Left = 11
    Top = 323
    Width = 81
    Height = 20
    Caption = 'ImportFleets'
    TabOrder = 11
    OnClick = UpdateChecks
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object PassEdit: TsEdit [12]
    Left = 58
    Top = 82
    Width = 165
    Height = 21
    Color = 16118514
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    PasswordChar = '*'
    TabOrder = 2
    OnChange = PassEditChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'PassEdit'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object NameEdit: TsEdit [13]
    Left = 58
    Top = 55
    Width = 165
    Height = 21
    Color = 16118514
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'NameEdit'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object PlanetImportModes: TsGroupBox [14]
    Left = 295
    Top = 487
    Width = 220
    Height = 95
    Caption = 'PlanetImportModes'
    TabOrder = 9
    SkinData.SkinSection = 'GROUPBOX'
    object UpdateMy: TsRadioButton
      Left = 9
      Top = 69
      Width = 69
      Height = 20
      Caption = 'UpdateMy'
      TabOrder = 0
      OnClick = UpdateChecks
      SkinData.SkinSection = 'RADIOBUTTON'
    end
    object ReplaceAll: TsRadioButton
      Left = 9
      Top = 14
      Width = 69
      Height = 20
      Caption = 'ReplaceAll'
      TabOrder = 1
      OnClick = UpdateChecks
      SkinData.SkinSection = 'RADIOBUTTON'
    end
    object UpdateAll: TsRadioButton
      Left = 9
      Top = 32
      Width = 66
      Height = 20
      Caption = 'UpdateAll'
      TabOrder = 2
      OnClick = UpdateChecks
      SkinData.SkinSection = 'RADIOBUTTON'
    end
    object UpdateActual: TsRadioButton
      Left = 9
      Top = 50
      Width = 85
      Height = 20
      Caption = 'UpdateActual'
      TabOrder = 3
      OnClick = UpdateChecks
      SkinData.SkinSection = 'RADIOBUTTON'
    end
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 280
    Top = 8
  end
end
