inherited PlayerEditPropFrame: TPlayerEditPropFrame
  Width = 335
  Height = 377
  ExplicitWidth = 335
  ExplicitHeight = 377
  object sSpeedButton1: TsSpeedButton [0]
    Left = 296
    Top = 56
    Width = 33
    Height = 22
    HelpType = htKeyword
    Caption = '...'
    OnClick = sSpeedButton1Click
    SkinData.SkinSection = 'SPEEDBUTTON'
  end
  object sLabel1: TsLabel [1]
    Left = 9
    Top = 244
    Width = 52
    Height = 13
    Caption = #1071#1074#1083#1103#1077#1090#1089#1103':'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 5391682
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object SURL: TsWebLabel [2]
    Left = 170
    Top = 3
    Width = 159
    Height = 13
    Alignment = taRightJustify
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = #1053#1072#1081#1090#1080' '#1074' '#1080#1075#1088#1077
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    HoverFont.Charset = DEFAULT_CHARSET
    HoverFont.Color = clBlue
    HoverFont.Height = -11
    HoverFont.Name = 'Tahoma'
    HoverFont.Style = []
  end
  object sSpeedButton2: TsSpeedButton [3]
    Left = 295
    Top = 212
    Width = 33
    Height = 21
    HelpType = htKeyword
    Caption = '...'
    OnClick = sSpeedButton2Click
    SkinData.SkinSection = 'SPEEDBUTTON'
  end
  object PlayerColor1: TsColorSelect [4]
    Left = 9
    Top = 280
    Width = 73
    Height = 21
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
  object PlayerColor2: TsColorSelect [5]
    Left = 96
    Top = 280
    Width = 74
    Height = 21
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
  object RelMultCheckBox: TsCheckBox [6]
    Left = 65
    Top = 236
    Width = 62
    Height = 20
    Caption = #1084#1091#1083#1100#1090#1086#1084
    TabOrder = 4
    OnClick = RelMultCheckBoxClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object OtherDipCheck: TsRadioButton [7]
    Left = 105
    Top = 179
    Width = 57
    Height = 20
    Caption = #1044#1088#1091#1075#1080#1077
    TabOrder = 7
    OnClick = OtherDipCheckClick
    SkinData.SkinSection = 'RADIOBUTTON'
  end
  object RaceComboBox: TsComboBox [8]
    Left = 8
    Top = 56
    Width = 281
    Height = 21
    Alignment = taLeftJustify
    BoundLabel.Active = True
    BoundLabel.Caption = #1056#1072#1089#1072
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'COMBOBOX'
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 0
    ItemIndex = -1
    ParentFont = False
    Sorted = True
    TabOrder = 0
    OnChange = DataChange
  end
  object NotesMemo: TsMemo [9]
    Left = 9
    Top = 317
    Width = 319
    Height = 52
    Color = 16118514
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Lines.Strings = (
      'NotesMemo')
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 9
    OnChange = DataChange
    Text = 'NotesMemo'
    BoundLabel.Active = True
    BoundLabel.Caption = #1055#1088#1080#1084#1077#1095#1072#1085#1080#1103
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = RUSSIAN_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'EDIT'
  end
  object OtherDip: TsComboBox [10]
    Left = 171
    Top = 178
    Width = 157
    Height = 21
    Alignment = taLeftJustify
    BoundLabel.Caption = #1042#1093#1086#1076#1080#1090' '#1074' '#1086#1088#1075#1072#1085#1080#1079#1072#1094#1080#1102
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
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 0
    ItemIndex = -1
    ParentFont = False
    Sorted = True
    TabOrder = 8
    OnChange = OtherDipChange
  end
  object DipRadio: TsRadioGroup [11]
    Left = 8
    Top = 80
    Width = 321
    Height = 92
    Caption = #1044#1080#1087#1083#1086#1084#1072#1090#1080#1095#1077#1089#1082#1080#1077' '#1086#1090#1085#1086#1096#1077#1085#1080#1103
    ParentBackground = False
    TabOrder = 1
    OnClick = DipRadioClick
    SkinData.SkinSection = 'GROUPBOX'
    Columns = 2
  end
  object NameEdit: TsEdit [12]
    Left = 9
    Top = 16
    Width = 321
    Height = 21
    Color = 16118514
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
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
  object IconCombo: TsComboBoxEx [13]
    Left = 185
    Top = 279
    Width = 143
    Height = 22
    BoundLabel.Active = True
    BoundLabel.Caption = #1047#1085#1072#1095#1086#1082
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
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
  object RelationComboBox: TsComboBox [14]
    Left = 185
    Top = 244
    Width = 145
    Height = 22
    Alignment = taLeftJustify
    BoundLabel.Active = True
    BoundLabel.Caption = #1080#1075#1088#1086#1082#1072
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'COMBOBOX'
    Style = csOwnerDrawFixed
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 16
    ItemIndex = -1
    ParentFont = False
    Sorted = True
    TabOrder = 3
    OnChange = DataChange
  end
  object VassalCheckBox: TsCheckBox [15]
    Left = 65
    Top = 254
    Width = 66
    Height = 20
    Caption = #1074#1072#1089#1089#1072#1083#1086#1084
    TabOrder = 5
    OnClick = RelMultCheckBoxClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object OrgComboBox: TsComboBox [16]
    Left = 129
    Top = 212
    Width = 157
    Height = 21
    Alignment = taLeftJustify
    BoundLabel.Active = True
    BoundLabel.Caption = #1042#1093#1086#1076#1080#1090' '#1074' '#1086#1088#1075#1072#1085#1080#1079#1072#1094#1080#1102
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5391682
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    SkinData.SkinSection = 'COMBOBOX'
    Color = 16118514
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemHeight = 0
    ItemIndex = -1
    ParentFont = False
    Sorted = True
    TabOrder = 2
    OnChange = DataChange
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 392
  end
end
