inherited FleetEditFrame: TFleetEditFrame
  Width = 345
  Height = 220
  ExplicitWidth = 345
  ExplicitHeight = 220
  object NameEdit: TsEdit [0]
    Left = 48
    Top = 22
    Width = 239
    Height = 21
    TabOrder = 0
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
  object ExtNameEdit: TsEdit [1]
    Left = 48
    Top = 67
    Width = 239
    Height = 21
    TabOrder = 1
    OnChange = ExtNameEditChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'ExtNameEdit'
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
  object BehavoirGroup: TsGroupBox [2]
    Left = 11
    Top = 104
    Width = 321
    Height = 105
    Caption = 'BehavoirGroup'
    TabOrder = 2
    CaptionLayout = clTopCenter
    SkinData.SkinSection = 'GROUPBOX'
    object UnknownBehCombo: TsComboBox
      Left = 89
      Top = 20
      Width = 81
      Height = 21
      Alignment = taLeftJustify
      BoundLabel.Active = True
      BoundLabel.Caption = #1053#1077#1080#1079#1074#1077#1089#1085#1099#1081
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = RUSSIAN_CHARSET
      BoundLabel.Font.Color = clWindowText
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclLeft
      BoundLabel.MaxWidth = 0
      BoundLabel.UseSkinColor = True
      SkinData.SkinSection = 'COMBOBOX'
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 0
      Text = #1042#1090#1086#1088#1078#1077#1085#1080#1077
      OnChange = BehComboChange
      Items.Strings = (
        #1042#1090#1086#1088#1078#1077#1085#1080#1077)
    end
    object NeutralsBehCombo: TsComboBox
      Left = 89
      Top = 47
      Width = 81
      Height = 21
      Alignment = taLeftJustify
      BoundLabel.Active = True
      BoundLabel.Caption = #1053#1077#1080#1079#1074#1077#1089#1085#1099#1081
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = RUSSIAN_CHARSET
      BoundLabel.Font.Color = clWindowText
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclLeft
      BoundLabel.MaxWidth = 0
      BoundLabel.UseSkinColor = True
      SkinData.SkinSection = 'COMBOBOX'
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 1
      Text = #1042#1090#1086#1088#1078#1077#1085#1080#1077
      OnChange = BehComboChange
      Items.Strings = (
        #1042#1090#1086#1088#1078#1077#1085#1080#1077)
    end
    object VassalBehCombo: TsComboBox
      Left = 89
      Top = 74
      Width = 81
      Height = 21
      Alignment = taLeftJustify
      BoundLabel.Active = True
      BoundLabel.Caption = #1053#1077#1080#1079#1074#1077#1089#1085#1099#1081
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = RUSSIAN_CHARSET
      BoundLabel.Font.Color = clWindowText
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclLeft
      BoundLabel.MaxWidth = 0
      BoundLabel.UseSkinColor = True
      SkinData.SkinSection = 'COMBOBOX'
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 2
      Text = #1042#1090#1086#1088#1078#1077#1085#1080#1077
      OnChange = BehComboChange
      Items.Strings = (
        #1042#1090#1086#1088#1078#1077#1085#1080#1077)
    end
    object EnemiesBehCombo: TsComboBox
      Left = 229
      Top = 20
      Width = 81
      Height = 21
      Alignment = taLeftJustify
      BoundLabel.Active = True
      BoundLabel.Caption = #1053#1077#1080#1079#1074#1077#1089#1085#1099#1081
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = RUSSIAN_CHARSET
      BoundLabel.Font.Color = clWindowText
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclLeft
      BoundLabel.MaxWidth = 0
      BoundLabel.UseSkinColor = True
      SkinData.SkinSection = 'COMBOBOX'
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 3
      Text = #1042#1090#1086#1088#1078#1077#1085#1080#1077
      OnChange = BehComboChange
      Items.Strings = (
        #1042#1090#1086#1088#1078#1077#1085#1080#1077)
    end
    object AllyBehCombo: TsComboBox
      Left = 229
      Top = 47
      Width = 81
      Height = 21
      Alignment = taLeftJustify
      BoundLabel.Active = True
      BoundLabel.Caption = #1057#1086#1102#1079#1085#1080#1082
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = RUSSIAN_CHARSET
      BoundLabel.Font.Color = clWindowText
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclLeft
      BoundLabel.MaxWidth = 0
      BoundLabel.UseSkinColor = True
      SkinData.SkinSection = 'COMBOBOX'
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 4
      Text = #1042#1090#1086#1088#1078#1077#1085#1080#1077
      OnChange = BehComboChange
      Items.Strings = (
        #1042#1090#1086#1088#1078#1077#1085#1080#1077)
    end
    object LordBehCombo: TsComboBox
      Left = 229
      Top = 74
      Width = 81
      Height = 21
      Alignment = taLeftJustify
      BoundLabel.Active = True
      BoundLabel.Caption = #1053#1077#1080#1079#1074#1077#1089#1085#1099#1081
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = RUSSIAN_CHARSET
      BoundLabel.Font.Color = clWindowText
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclLeft
      BoundLabel.MaxWidth = 0
      BoundLabel.UseSkinColor = True
      SkinData.SkinSection = 'COMBOBOX'
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 5
      Text = #1042#1090#1086#1088#1078#1077#1085#1080#1077
      OnChange = BehComboChange
      Items.Strings = (
        #1042#1090#1086#1088#1078#1077#1085#1080#1077)
    end
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 0
    Top = 0
  end
end
