inherited DipColorPrefFrame: TDipColorPrefFrame
  Width = 261
  Height = 175
  ExplicitWidth = 261
  ExplicitHeight = 175
  object sColor_EMPTY: TsColorSelect [0]
    Tag = 11
    Left = 12
    Top = 58
    Width = 193
    Height = 22
    Caption = #1042#1083#1072#1076#1077#1083#1077#1094' '#1087#1083#1072#1085#1077#1090#1099' '#1085#1077#1080#1079#1074#1077'c'#1090#1077#1085
    Layout = blGlyphRight
    Alignment = taRightJustify
    SkinData.SkinSection = 'SPEEDBUTTON'
    OnChange = DipColorChange
    ColorValue = clBlack
  end
  object sColor_UNDEFINED: TsColorSelect [1]
    Left = 12
    Top = 30
    Width = 193
    Height = 22
    Caption = #1044#1080#1087'. '#1086#1090#1085#1086#1096#1077#1085#1080#1103' '#1085#1077' '#1086#1087#1088#1077#1076#1077#1083#1077#1085#1099
    Layout = blGlyphRight
    Alignment = taRightJustify
    SkinData.SkinSection = 'SPEEDBUTTON'
    OnChange = DipColorChange
    ColorValue = clBlack
  end
  object OthersButton: TsButton [2]
    Left = 72
    Top = 86
    Width = 75
    Height = 25
    Caption = #1044#1088#1091#1075#1080#1077'...'
    TabOrder = 0
    OnClick = OthersButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 320
    Top = 0
  end
end
