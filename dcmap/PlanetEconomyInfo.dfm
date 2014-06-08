inherited PlanetEconomyInfoFrame: TPlanetEconomyInfoFrame
  Width = 199
  Height = 90
  ExplicitWidth = 199
  ExplicitHeight = 90
  object BasisImg: TImage [0]
    Left = 5
    Top = 3
    Width = 18
    Height = 19
    Visible = False
  end
  object SecondImg: TImage [1]
    Left = 5
    Top = 23
    Width = 18
    Height = 18
    Visible = False
  end
  object MoneyImg: TImage [2]
    Left = 5
    Top = 44
    Width = 18
    Height = 18
    Visible = False
  end
  object BasisLabel: TsLabel [3]
    Left = 28
    Top = 5
    Width = 33
    Height = 16
    Caption = 'Basis'
    ParentFont = False
    Visible = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
  end
  object SecondLabel: TsLabel [4]
    Left = 28
    Top = 24
    Width = 39
    Height = 16
    Caption = 'Secnd'
    ParentFont = False
    Visible = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
  end
  object MoneyLabel: TsLabel [5]
    Left = 28
    Top = 46
    Width = 43
    Height = 16
    Caption = 'Money'
    ParentFont = False
    Visible = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
  end
  object ProductionLabel: TsLabel [6]
    Left = 28
    Top = 69
    Width = 51
    Height = 16
    Caption = 'Product'
    ParentFont = False
    Visible = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
  end
  object ProductionImg: TImage [7]
    Left = 5
    Top = 67
    Width = 18
    Height = 18
    Visible = False
  end
  object Corruption: TsLabel [8]
    Left = 157
    Top = 69
    Width = 34
    Height = 13
    Anchors = [akTop, akRight]
    Caption = '100%'
    ParentFont = False
    Visible = False
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
  end
  object CorruptionLabel: TsLabel [9]
    Left = 98
    Top = 69
    Width = 55
    Height = 13
    Alignment = taRightJustify
    Anchors = [akTop, akRight]
    Caption = 'Corruption:'
    Visible = False
  end
  object PeopleGroupBox: TsGroupBox [10]
    Left = 83
    Top = 2
    Width = 112
    Height = 63
    Anchors = [akTop, akRight]
    Caption = 'PeopleGroupBox'
    TabOrder = 0
    Visible = False
    SkinData.SkinSection = 'GROUPBOX'
    object PeopleFreeLabel: TsLabel
      Left = 34
      Top = 28
      Width = 26
      Height = 13
      Alignment = taRightJustify
      Caption = 'Free:'
    end
    object GrowthLabel: TsLabel
      Left = 21
      Top = 44
      Width = 39
      Height = 13
      Alignment = taRightJustify
      Caption = 'Growth:'
    end
    object PeopleFree: TsLabel
      Left = 61
      Top = 28
      Width = 45
      Height = 13
      Caption = '999 999'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
    object PeopleGrowth: TsLabel
      Left = 61
      Top = 44
      Width = 44
      Height = 13
      Caption = '99.99%'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
    object PeopleTotal: TsLabel
      Left = 61
      Top = 14
      Width = 45
      Height = 13
      Caption = '999 999'
      ParentFont = False
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
    end
    object PeopleTotalLabel: TsLabel
      Left = 32
      Top = 14
      Width = 28
      Height = 13
      Alignment = taRightJustify
      Caption = 'Total:'
    end
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 128
    Top = 168
  end
end
