inherited PlanetInfoFrame: TPlanetInfoFrame
  Width = 186
  Height = 152
  Constraints.MinWidth = 165
  ExplicitWidth = 186
  ExplicitHeight = 152
  object NameL: TsLabel [0]
    Left = 5
    Top = 29
    Width = 52
    Height = 13
    Caption = #1053#1072#1079#1074#1072#1085#1080#1077':'
    ParentFont = False
    OnDblClick = NameLabelDblClick
    Font.Charset = ANSI_CHARSET
    Font.Color = 5391682
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object NameLabel: TsLabel [1]
    Left = 61
    Top = 29
    Width = 120
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    Caption = 'NameLabel'
    ParentFont = False
    OnDblClick = NameLabelDblClick
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = 5391682
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ExplicitWidth = 105
  end
  object OwnerL: TsLabel [2]
    Left = 5
    Top = 45
    Width = 53
    Height = 13
    Caption = #1042#1083#1072#1076#1077#1083#1077#1094':'
    ParentFont = False
    OnDblClick = OwnerLabelDblClick
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = 5391682
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object OwnerLabel: TsLabel [3]
    Left = 61
    Top = 45
    Width = 120
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    Caption = 'OwnerLabel'
    ParentFont = False
    OnDblClick = OwnerLabelDblClick
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = 5391682
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ExplicitWidth = 105
  end
  object CopyButton: TsSpeedButton [4]
    Left = 139
    Top = 4
    Width = 42
    Height = 22
    Anchors = [akTop, akRight]
    OnClick = CopyButtonClick
    ButtonStyle = tbsDropDown
    SkinData.SkinSection = 'SPEEDBUTTON'
    DropdownMenu = CopyMenu
  end
  object ExpandButton: TsSpeedButton [5]
    Left = 163
    Top = 61
    Width = 18
    Height = 18
    Anchors = [akTop, akRight]
    Flat = True
    OnClick = CollapseButtonClick
    SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
    ExplicitLeft = 157
  end
  object sLabel1: TsLabel [6]
    Left = 5
    Top = 64
    Width = 12
    Height = 13
    Caption = 'O:'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object OLabel2: TsLabel [7]
    Left = 17
    Top = 64
    Width = 8
    Height = 13
    Caption = 'O'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object ELabel2: TsLabel [8]
    Left = 44
    Top = 64
    Width = 6
    Height = 13
    Caption = 'E'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object sLabel9: TsLabel [9]
    Left = 33
    Top = 64
    Width = 10
    Height = 13
    Caption = 'E:'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object sLabel10: TsLabel [10]
    Left = 60
    Top = 64
    Width = 12
    Height = 13
    Caption = 'M:'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object MLabel2: TsLabel [11]
    Left = 73
    Top = 64
    Width = 8
    Height = 13
    Caption = 'M'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object sLabel12: TsLabel [12]
    Left = 90
    Top = 64
    Width = 10
    Height = 13
    Caption = 'T:'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object TLabel2: TsLabel [13]
    Left = 101
    Top = 64
    Width = 6
    Height = 13
    Caption = 'T'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object SLabel2: TsLabel [14]
    Left = 129
    Top = 64
    Width = 6
    Height = 13
    Caption = 'S'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object sLabel15: TsLabel [15]
    Left = 118
    Top = 64
    Width = 10
    Height = 13
    Caption = 'S:'
    ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
  end
  object CoordEdit: TsEdit [16]
    Left = 76
    Top = 5
    Width = 60
    Height = 21
    Anchors = [akTop, akRight]
    AutoSize = False
    Color = 16118514
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = 'CoordEdit'
    OnKeyDown = CoordEditKeyDown
    SkinData.SkinSection = 'EDIT'
    BoundLabel.Active = True
    BoundLabel.Caption = 'Coord'
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
  object ExPanel: TsPanel [17]
    AlignWithMargins = True
    Left = 3
    Top = 61
    Width = 180
    Height = 88
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    SkinData.SkinSection = 'CHECKBOX'
    DesignSize = (
      180
      88)
    object WebGoPaid: TsWebLabel
      Left = 151
      Top = 72
      Width = 24
      Height = 13
      Hint = #1054#1090#1082#1088#1099#1090#1100' '#1074' '#1080#1075#1088#1077' ('#1085#1072' '#1087#1083#1072#1090#1085#1086#1081' '#1087#1086#1076#1087#1080#1089#1082#1077')'
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      Caption = '$$$$'
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
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
    object WebGoStandart: TsWebLabel
      Left = 145
      Top = 55
      Width = 30
      Height = 13
      Hint = #1054#1090#1082#1088#1099#1090#1100' '#1074' '#1080#1075#1088#1077
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      Caption = 'WWW'
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
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
      ExplicitLeft = 124
    end
    object BoxShape: TShape
      Left = 4
      Top = 3
      Width = 133
      Height = 82
      Brush.Color = 15461355
    end
    object sLabel3: TsLabel
      Left = 102
      Top = 6
      Width = 12
      Height = 13
      Caption = 'O:'
      ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      UseSkinColor = False
    end
    object sLabel4: TsLabel
      Left = 102
      Top = 21
      Width = 10
      Height = 13
      Caption = 'E:'
      ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      UseSkinColor = False
    end
    object sLabel5: TsLabel
      Left = 102
      Top = 36
      Width = 12
      Height = 13
      Caption = 'M:'
      ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      UseSkinColor = False
    end
    object sLabel6: TsLabel
      Left = 102
      Top = 51
      Width = 10
      Height = 13
      Caption = 'T:'
      ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      UseSkinColor = False
    end
    object sLabel7: TsLabel
      Left = 102
      Top = 67
      Width = 10
      Height = 13
      Caption = 'S:'
      ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      UseSkinColor = False
    end
    object OLabel: TsLabel
      Left = 118
      Top = 6
      Width = 8
      Height = 13
      Caption = 'O'
      ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      UseSkinColor = False
    end
    object ELabel: TsLabel
      Left = 118
      Top = 21
      Width = 6
      Height = 13
      Caption = 'E'
      ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      UseSkinColor = False
    end
    object MLabel: TsLabel
      Left = 118
      Top = 36
      Width = 8
      Height = 13
      Caption = 'M'
      ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      UseSkinColor = False
    end
    object TLabel: TsLabel
      Left = 118
      Top = 51
      Width = 6
      Height = 13
      Caption = 'T'
      ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      UseSkinColor = False
    end
    object SLabel: TsLabel
      Left = 118
      Top = 67
      Width = 6
      Height = 13
      Caption = 'S'
      ParentFont = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      UseSkinColor = False
    end
    object OBox: TShape
      Left = 9
      Top = 10
      Width = 13
      Height = 75
      Brush.Color = clLime
    end
    object EBox: TShape
      Left = 25
      Top = 10
      Width = 13
      Height = 75
      Brush.Color = clAqua
    end
    object MBox: TShape
      Left = 41
      Top = 10
      Width = 13
      Height = 75
      Brush.Color = clRed
    end
    object TBox: TShape
      Left = 57
      Top = 10
      Width = 13
      Height = 75
      Brush.Color = clYellow
    end
    object SBox: TShape
      Left = 73
      Top = 10
      Width = 13
      Height = 75
      Brush.Color = clFuchsia
    end
    object CollapseButton: TsSpeedButton
      Left = 160
      Top = 3
      Width = 18
      Height = 18
      Anchors = [akTop, akRight]
      Flat = True
      OnClick = CollapseButtonClick
      SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
      ExplicitLeft = 154
    end
    object CorruptionBox: TShape
      Left = 91
      Top = 3
      Width = 9
      Height = 42
      Brush.Color = 4213723
    end
    object CorruptionOpBox: TShape
      Left = 91
      Top = 46
      Width = 9
      Height = 39
      Brush.Color = 11594677
    end
    object WebGoUser: TsWebLabel
      Left = 143
      Top = 38
      Width = 32
      Height = 13
      Hint = #1054#1090#1082#1088#1099#1090#1100' '#1074' '#1080#1075#1088#1077
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      Caption = 'Owner'
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
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
  end
  inherited sFrameAdapter: TsFrameAdapter
    SkinData.SkinSection = 'BARPANEL'
    Top = 251
  end
  object CopyMenu: TPopupMenu
    MenuAnimation = [maNone]
    OwnerDraw = True
    Left = 8
    Top = 5
  end
end
