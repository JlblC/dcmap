inherited IconsPropFrame: TIconsPropFrame
  Width = 245
  Height = 275
  ExplicitWidth = 245
  ExplicitHeight = 275
  object IconLabel: TsLabel [0]
    Left = 16
    Top = 44
    Width = 36
    Height = 13
    Caption = #1047#1085#1072#1095#1086#1082
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 5059883
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object BigIconLabel: TsLabel [1]
    Left = 136
    Top = 43
    Width = 83
    Height = 13
    Caption = #1050#1088#1091#1087#1085#1099#1081' '#1079#1085#1072#1095#1086#1082
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 5059883
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object sPanel2: TsPanel [2]
    Left = 136
    Top = 62
    Width = 88
    Height = 73
    BevelInner = bvLowered
    BevelOuter = bvNone
    Color = clAppWorkSpace
    TabOrder = 4
    SkinData.SkinSection = 'PANEL_LOW'
    object BigImage: TImage
      Left = 8
      Top = 8
      Width = 73
      Height = 57
      Center = True
      Transparent = True
    end
  end
  object LoadBigImageButton: TsButton [3]
    Left = 136
    Top = 141
    Width = 88
    Height = 25
    Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100'...'
    TabOrder = 5
    OnClick = LoadBigImageButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object NameEdit: TsEdit [4]
    Left = 8
    Top = 17
    Width = 225
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = 'NameEdit'
    OnChange = NameEditChange
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = #1053#1072#1079#1074#1072#1085#1080#1077
    BoundLabel.Indent = 0
    BoundLabel.Font.Charset = DEFAULT_CHARSET
    BoundLabel.Font.Color = 5059883
    BoundLabel.Font.Height = -11
    BoundLabel.Font.Name = 'Tahoma'
    BoundLabel.Font.Style = []
    BoundLabel.Layout = sclTopLeft
    BoundLabel.MaxWidth = 0
    BoundLabel.UseSkinColor = True
    ExplicitWidth = 268
  end
  object sPanel1: TsPanel [5]
    Left = 16
    Top = 62
    Width = 88
    Height = 73
    BevelInner = bvLowered
    BevelOuter = bvNone
    Color = clAppWorkSpace
    TabOrder = 1
    SkinData.SkinSection = 'PANEL_LOW'
    object Image: TImage
      Left = 8
      Top = 8
      Width = 73
      Height = 57
      Center = True
      Transparent = True
    end
  end
  object LoadImageButton: TsButton [6]
    Left = 16
    Top = 141
    Width = 88
    Height = 25
    Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100'...'
    TabOrder = 2
    OnClick = LoadImageButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object TrnsCheck: TsCheckBox [7]
    Left = 16
    Top = 237
    Width = 164
    Height = 20
    Caption = #1048#1089#1087#1086#1083#1100#1079#1086#1074#1072#1090#1100' '#1087#1088#1086#1079#1088#1072#1095#1085#1086#1089#1090#1100
    TabOrder = 3
    OnClick = TrnsCheckClick
    SkinData.SkinSection = 'CHECKBOX'
    ImgChecked = 0
    ImgUnchecked = 0
  end
  object ClearBigImageButton: TsButton [8]
    Left = 136
    Top = 206
    Width = 88
    Height = 25
    Caption = #1054#1095#1080#1089#1090#1080#1090#1100
    TabOrder = 7
    OnClick = ClearBigImageButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object ClearImageButton: TsButton [9]
    Left = 16
    Top = 206
    Width = 88
    Height = 25
    Caption = #1054#1095#1080#1089#1090#1080#1090#1100
    TabOrder = 6
    OnClick = ClearImageButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object SaveImageButton: TsButton [10]
    Left = 16
    Top = 174
    Width = 88
    Height = 25
    Caption = 'SaveAs'
    TabOrder = 8
    OnClick = SaveImageButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  object SaveBigImageButton: TsButton [11]
    Left = 136
    Top = 174
    Width = 88
    Height = 25
    Caption = 'SaveAs'
    TabOrder = 9
    OnClick = SaveBigImageButtonClick
    SkinData.SkinSection = 'BUTTON'
  end
  inherited sFrameAdapter: TsFrameAdapter
    Left = 336
    Top = 208
  end
  object OpenDlg: TsOpenPictureDialog
    Filter = 'Bitmaps (*.bmp)|*.bmp|All Files (*.*)|*.*'
    Left = 216
    Top = 232
  end
  object SaveDlg: TsSavePictureDialog
    DefaultExt = '.bmp'
    Filter = 'Bitmaps (*.bmp)|*.bmp|All Files (*.*)|*.*'
    Left = 256
    Top = 232
  end
end
