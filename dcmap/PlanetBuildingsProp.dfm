inherited PlanetBuildingsFrame: TPlanetBuildingsFrame
  Width = 310
  Height = 325
  ExplicitWidth = 310
  ExplicitHeight = 325
  object DesignCheck: TsCheckBox [0]
    Left = 106
    Top = 153
    Width = 52
    Height = 20
    Caption = 'Design'
    TabOrder = 11
    OnClick = DataChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object CompressorsEdit: TsSpinEdit [1]
    Left = 7
    Top = 204
    Width = 73
    Height = 21
    Color = clWhite
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
    BoundLabel.Caption = 'CompressorsEdit'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clBlack
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
  object ResearchCheck: TsCheckBox [2]
    Left = 106
    Top = 134
    Width = 65
    Height = 20
    Caption = 'Research'
    TabOrder = 10
    OnClick = DataChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object UnsafeCheck: TsCheckBox [3]
    Left = 152
    Top = 25
    Width = 83
    Height = 20
    Caption = 'UnsafeCheck'
    TabOrder = 17
    OnClick = DataChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object BarracksCheck: TsCheckBox [4]
    Left = 106
    Top = 56
    Width = 90
    Height = 20
    Caption = 'BarracksCheck'
    TabOrder = 6
    OnClick = DataChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object ComponentsCheck: TsCheckBox [5]
    Left = 106
    Top = 114
    Width = 103
    Height = 20
    Caption = 'ComponetsCheck'
    TabOrder = 8
    OnClick = DataChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object MiningRadio: TsRadioButton [6]
    Left = 3
    Top = 3
    Width = 77
    Height = 20
    Caption = 'MiningRadio'
    Checked = True
    TabOrder = 15
    TabStop = True
    OnClick = DataChange
    SkinData.SkinSection = 'RADIOBUTTON'
  end
  object FactoriesEdit: TsSpinEdit [7]
    Left = 7
    Top = 168
    Width = 73
    Height = 21
    Color = clWhite
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = '0'
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'FactoriesEdit'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clBlack
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
  object HWCheck: TsCheckBox [8]
    Left = 3
    Top = 25
    Width = 66
    Height = 20
    Caption = 'HWCheck'
    TabOrder = 16
    OnClick = HWCheckClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object MinesEdit: TsSpinEdit [9]
    Left = 7
    Top = 59
    Width = 73
    Height = 21
    Color = clWhite
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = '0'
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'MinesEdit'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clBlack
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
  object CorruptionEdit: TsEdit [10]
    Left = 7
    Top = 290
    Width = 73
    Height = 21
    Color = clWhite
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 18
    Text = 'CorruptionEdit'
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'Corruption'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clBlack
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
  end
  object PalaceCheck: TsCheckBox [11]
    Left = 106
    Top = 173
    Width = 51
    Height = 20
    Caption = 'Palace'
    TabOrder = 12
    OnClick = DataChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object IndustrialRadio: TsRadioButton [12]
    Left = 109
    Top = 3
    Width = 92
    Height = 20
    Caption = 'IndustrialRadio'
    TabOrder = 13
    OnClick = DataChange
    SkinData.SkinSection = 'RADIOBUTTON'
  end
  object ParamsGroupBox: TsGroupBox [13]
    Left = 106
    Top = 194
    Width = 196
    Height = 125
    Caption = 'ParamsGroupBox'
    TabOrder = 19
    SkinData.SkinSection = 'GROUPBOX'
    object BasisImg: TImage
      Left = 6
      Top = 19
      Width = 18
      Height = 18
    end
    object SecondImg: TImage
      Left = 66
      Top = 20
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
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
    object SecondLabel: TsLabel
      Left = 87
      Top = 21
      Width = 21
      Height = 14
      Caption = 'Sec'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
    object CreditsLabel: TsLabel
      Left = 146
      Top = 21
      Width = 43
      Height = 14
      Caption = 'Credits'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
    object CreditsImg: TImage
      Left = 124
      Top = 20
      Width = 18
      Height = 18
    end
    object MaxPopLabel: TsLabel
      Left = 61
      Top = 59
      Width = 63
      Height = 13
      Alignment = taRightJustify
      Caption = 'MaxPopLabel'
    end
    object PopMaxValue: TsLabel
      Left = 128
      Top = 58
      Width = 40
      Height = 15
      Caption = 'PopVal'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'Tempus Sans ITC'
      Font.Style = [fsBold]
    end
    object PopFreeLabel: TsLabel
      Left = 59
      Top = 73
      Width = 65
      Height = 13
      Alignment = taRightJustify
      Caption = 'PopFreeLabel'
    end
    object PopFreeValue: TsLabel
      Left = 128
      Top = 72
      Width = 40
      Height = 15
      Caption = 'PopVal'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'Tempus Sans ITC'
      Font.Style = [fsBold]
    end
    object ProductionValue: TsLabel
      Left = 128
      Top = 88
      Width = 40
      Height = 15
      Caption = 'PopVal'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'Tempus Sans ITC'
      Font.Style = [fsBold]
    end
    object ProductionLabel: TsLabel
      Left = 59
      Top = 89
      Width = 65
      Height = 13
      Alignment = taRightJustify
      Caption = 'PopFreeLabel'
    end
    object CorruptLabel: TsLabel
      Left = 62
      Top = 44
      Width = 62
      Height = 13
      Alignment = taRightJustify
      Caption = 'CorruptLabel'
    end
    object CorruptValue: TsLabel
      Left = 128
      Top = 43
      Width = 40
      Height = 15
      Caption = 'PopVal'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'Tempus Sans ITC'
      Font.Style = [fsBold]
    end
    object PopCurLabel: TsLabel
      Left = 64
      Top = 108
      Width = 60
      Height = 13
      Alignment = taRightJustify
      Caption = 'PopCurLabel'
    end
    object PopCurValue: TsLabel
      Left = 128
      Top = 107
      Width = 40
      Height = 15
      Caption = 'PopVal'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'Tempus Sans ITC'
      Font.Style = [fsBold]
    end
  end
  object BounceRadio: TsRadioButton [14]
    Left = 222
    Top = 3
    Width = 82
    Height = 20
    Caption = 'BounceRadio'
    TabOrder = 14
    OnClick = DataChange
    SkinData.SkinSection = 'RADIOBUTTON'
  end
  object BiofarmsEdit: TsSpinEdit [15]
    Left = 7
    Top = 95
    Width = 73
    Height = 21
    Color = clWhite
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = '0'
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'BiofarmsEdit'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clBlack
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
  object CustomsCheck: TsCheckBox [16]
    Left = 106
    Top = 95
    Width = 61
    Height = 20
    Caption = 'Customs'
    TabOrder = 9
    OnClick = DataChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object ShipyardCheck: TsCheckBox [17]
    Left = 106
    Top = 75
    Width = 91
    Height = 20
    Caption = 'ShipyardCheck'
    TabOrder = 7
    OnClick = DataChange
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object PowerPlantsEdit: TsSpinEdit [18]
    Left = 7
    Top = 131
    Width = 73
    Height = 21
    Color = clWhite
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = '0'
    OnChange = DataChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'PowerPlantsEdit'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clBlack
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
  object GovernorsEdit: TsSpinEdit [19]
    Left = 7
    Top = 248
    Width = 73
    Height = 21
    Color = clWhite
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    Text = '0'
    OnChange = HWCheckClick
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'GovernorsEdit'
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = clBlack
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
  inherited sFrameAdapter: TsFrameAdapter
    Left = 480
  end
end
