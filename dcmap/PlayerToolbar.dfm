inherited PlayerToolbarFrame: TPlayerToolbarFrame
  Width = 432
  Height = 23
  ExplicitWidth = 432
  ExplicitHeight = 23
  object BasisImg: TImage [0]
    Left = 123
    Top = 3
    Width = 10
    Height = 18
  end
  object SecondImg: TImage [1]
    Left = 215
    Top = 3
    Width = 10
    Height = 18
  end
  object BasisLabel: TsLabel [2]
    Left = 135
    Top = -1
    Width = 24
    Height = 13
    Caption = 'Basis'
    ParentFont = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object SecondLabel: TsLabel [3]
    Left = 230
    Top = -1
    Width = 35
    Height = 13
    Caption = 'Second'
    ParentFont = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object CreditsLabel: TsLabel [4]
    Left = 339
    Top = -1
    Width = 34
    Height = 13
    Caption = 'Credits'
    ParentFont = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object CreditsImg: TImage [5]
    Left = 328
    Top = 3
    Width = 10
    Height = 18
  end
  object BasisSpent: TsLabel [6]
    Left = 187
    Top = 10
    Width = 24
    Height = 13
    Alignment = taRightJustify
    Caption = 'Basis'
    ParentFont = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object BasisGain: TsLabel [7]
    Left = 135
    Top = 10
    Width = 24
    Height = 13
    Caption = 'Basis'
    ParentFont = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object SecondGain: TsLabel [8]
    Left = 230
    Top = 10
    Width = 35
    Height = 13
    Caption = 'Second'
    ParentFont = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object SecondSpent: TsLabel [9]
    Left = 289
    Top = 10
    Width = 35
    Height = 13
    Alignment = taRightJustify
    Caption = 'Second'
    ParentFont = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object CreditsGain: TsLabel [10]
    Left = 339
    Top = 10
    Width = 34
    Height = 13
    Caption = 'Credits'
    ParentFont = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object CreditsSpent: TsLabel [11]
    Left = 394
    Top = 10
    Width = 34
    Height = 13
    Alignment = taRightJustify
    Caption = 'Credits'
    ParentFont = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object PlayerCombo: TsComboBox [12]
    Left = 2
    Top = 2
    Width = 116
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
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 13
    ItemIndex = -1
    ParentFont = False
    TabOrder = 0
    OnChange = PlayerComboChange
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 80
    Top = 104
  end
end
