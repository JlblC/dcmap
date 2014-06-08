inherited DataImportForm: TDataImportForm
  Left = 359
  Top = 184
  VertScrollBar.Visible = False
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = #1048#1084#1087#1086#1088#1090' '#1076#1072#1085#1085#1099#1093
  ClientHeight = 429
  ClientWidth = 578
  Color = 13750737
  Constraints.MinHeight = 370
  Constraints.MinWidth = 512
  OnCreate = FormCreate
  ExplicitWidth = 586
  ExplicitHeight = 461
  PixelsPerInch = 96
  TextHeight = 13
  object sSplitter1: TsSplitter [0]
    Left = 0
    Top = 177
    Width = 578
    Height = 6
    Cursor = crVSplit
    Align = alTop
    AutoSnap = False
    SkinData.SkinSection = 'SPLITTER'
    ExplicitTop = 154
    ExplicitWidth = 588
  end
  object sPanel1: TsPanel [1]
    Left = 0
    Top = 0
    Width = 578
    Height = 57
    Align = alTop
    BevelOuter = bvNone
    ParentBackground = False
    TabOrder = 0
    SkinData.SkinSection = 'CHECKBOX'
    DesignSize = (
      578
      57)
    object FileRadio: TsRadioButton
      Left = 8
      Top = 8
      Width = 71
      Height = 20
      Caption = #1048#1079' '#1092#1072#1081#1083#1072':'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = ReadButtonClick
      SkinData.SkinSection = 'RADIOBUTTON'
    end
    object FilenameEdit: TsFilenameEdit
      Left = 87
      Top = 32
      Width = 400
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      MaxLength = 255
      ParentFont = False
      TabOrder = 1
      OnChange = ReadButtonClick
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
      OnButtonClick = FilenameEditButtonClick
      GlyphMode.Blend = 0
      GlyphMode.Grayed = False
    end
    object ClipboardButton: TsRadioButton
      Left = 110
      Top = 7
      Width = 112
      Height = 20
      Caption = #1048#1079' '#1073#1091#1092#1077#1088#1072' '#1086#1073#1084#1077#1085#1072
      TabOrder = 2
      OnClick = ReadButtonClick
      SkinData.SkinSection = 'RADIOBUTTON'
    end
    object ReadButton: TsButton
      Left = 493
      Top = 32
      Width = 78
      Height = 21
      Anchors = [akTop, akRight]
      Caption = #1055#1077#1088#1077#1095#1080#1090#1072#1090#1100
      TabOrder = 3
      OnClick = ReadButtonClick
      SkinData.SkinSection = 'BUTTON'
    end
    object ModuleRadio: TsRadioButton
      Left = 268
      Top = 7
      Width = 72
      Height = 20
      Caption = #1048#1079' '#1084#1086#1076#1091#1083#1103
      Enabled = False
      TabOrder = 4
      SkinData.SkinSection = 'RADIOBUTTON'
    end
    object sComboBox1: TsComboBox
      Left = 359
      Top = 5
      Width = 121
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
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 13
      ItemIndex = -1
      ParentFont = False
      TabOrder = 5
    end
    object CodepageComboBox: TsComboBox
      Left = 8
      Top = 32
      Width = 73
      Height = 21
      Alignment = taLeftJustify
      BoundLabel.Caption = #1050#1086#1076#1080#1088#1086#1074#1082#1072
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
      ItemIndex = 0
      ParentFont = False
      TabOrder = 6
      Text = 'Win 1251'
      Items.Strings = (
        'Win 1251'
        'DOS 866'
        'UTF8')
    end
  end
  object sPanel2: TsPanel [2]
    Left = 0
    Top = 183
    Width = 578
    Height = 66
    Align = alTop
    BevelOuter = bvNone
    ParentBackground = False
    TabOrder = 1
    SkinData.SkinSection = 'CHECKBOX'
    DesignSize = (
      578
      66)
    object SaveParam: TsSpeedButton
      Left = 527
      Top = 43
      Width = 19
      Height = 19
      Hint = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1087#1072#1088#1072#1084#1077#1090#1088#1099
      Anchors = [akTop, akRight]
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        18000000000000030000120B0000120B00000000000000000000FF00FFFF00FF
        FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00
        FFFF00FFFF00FFFF00FFFF00FFFF00FF004B00004B00A18283A18283A18283A1
        8283A18283A18283A18283004B00004B00FF00FFFF00FFFF00FFFF00FF004B00
        008100008100DDD4D5004B00004B00DCE0E0D7DADECED5D7BDBABD004B000163
        01004B00FF00FFFF00FFFF00FF004B00008100008100E2D9D9004B00004B00D9
        D8DAD9DEE1D3D9DCC1BDC1004B00016301004B00FF00FFFF00FFFF00FF004B00
        008100008100E6DCDC004B00004B00D5D3D5D8DEE1D7DDE0C6C2C5004B000163
        01004B00FF00FFFF00FFFF00FF004B00008100008100EADEDEE7DDDDDDD4D5D7
        D3D5D5D7D9D7D8DACAC2C5004B00016301004B00FF00FFFF00FFFF00FF004B00
        0081000081000081000081000081000081000081000081000081000081000081
        00004B00FF00FFFF00FFFF00FF004B000081009DC29D9DC29D9DC29D9DC29D9D
        C29D9DC29D9DC29D9DC29D9DC29D008100004B00FF00FFFF00FFFF00FF004B00
        008100F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F70081
        00004B00FF00FFFF00FFFF00FF004B00008100F7F7F7F7F7F7F7F7F7F7F7F7F7
        F7F7F7F7F7F7F7F7F7F7F7F7F7F7008100004B00FF00FFFF00FFFF00FF004B00
        008100F7F7F7BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFF7F7F70081
        00004B00FF00FFFF00FFFF00FF004B00008100F7F7F7F7F7F7F7F7F7F7F7F7F7
        F7F7F7F7F7F7F7F7F7F7F7F7F7F7008100004B00FF00FFFF00FFFF00FF004B00
        008100F7F7F7BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFF7F7F70081
        00004B00FF00FFFF00FFFF00FF004B00008100F7F7F7F7F7F7F7F7F7F7F7F7F7
        F7F7F7F7F7F7F7F7F7F7F7F7F7F7008100004B00FF00FFFF00FFFF00FFFF00FF
        004B00F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7004B
        00FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
        00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF}
      ParentShowHint = False
      ShowHint = True
      OnClick = SaveParamClick
      SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
      ExplicitLeft = 535
    end
    object DeleteParam: TsSpeedButton
      Left = 552
      Top = 43
      Width = 19
      Height = 19
      Hint = #1059#1076#1072#1083#1080#1090#1100' '#1087#1072#1088#1072#1084#1077#1090#1088#1099
      Anchors = [akTop, akRight]
      Flat = True
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        18000000000000030000120B0000120B00000000000000000000FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFF0005B70005B7FFFFFF0005B70005B7FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0005B70005B7FFFFFFFFFFFF0005B7
        0005B70005B7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0005
        B70005B7FFFFFFFFFFFFFFFFFF0005B70005B60005B70005B7FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF0005B70005B7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        0006D70005BA0005B70005B7FFFFFFFFFFFFFFFFFFFFFFFF0005B70005B7FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0005B70005B70005B6FF
        FFFF0005B60005B70005B7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFF0005B60006C70006C70006CE0005B4FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0006C100
        05C10006DAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFF0005B60006D70006CE0006DA0006E9FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0006E50006DA0006D3FF
        FFFFFFFFFF0006E50006EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFF0006F80006DA0006EFFFFFFFFFFFFFFFFFFFFFFFFF0006F80006F6FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0006F60006F60006F8FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF0006F60006F6FFFFFFFFFFFFFFFFFFFFFFFF0006F6
        0006F60006F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FF0006F6FFFFFFFFFFFF0006F60006F60006F6FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0006F60006F6
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentShowHint = False
      ShowHint = True
      OnClick = DeleteParamClick
      SkinData.SkinSection = 'SPEEDBUTTON_SMALL'
      ExplicitLeft = 560
    end
    object ModuleList: TsComboBox
      Left = 8
      Top = 17
      Width = 129
      Height = 21
      Alignment = taLeftJustify
      BoundLabel.Active = True
      BoundLabel.Caption = #1052#1086#1076#1091#1083#1100
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = DEFAULT_CHARSET
      BoundLabel.Font.Color = 2171169
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclTopLeft
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
      OnChange = ModuleListChange
    end
    object ProcessButton: TsButton
      Left = 496
      Top = 6
      Width = 75
      Height = 31
      Anchors = [akTop, akRight]
      Caption = #1057#1095#1080#1090#1072#1090#1100
      TabOrder = 1
      OnClick = ProcessButtonClick
      SkinData.SkinSection = 'BUTTON'
    end
    object ParamEdit: TsComboBox
      Left = 72
      Top = 43
      Width = 449
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      Alignment = taLeftJustify
      BoundLabel.Active = True
      BoundLabel.Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = DEFAULT_CHARSET
      BoundLabel.Font.Color = 2171169
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclLeft
      BoundLabel.MaxWidth = 0
      BoundLabel.UseSkinColor = True
      SkinData.SkinSection = 'COMBOBOX'
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 13
      ItemIndex = -1
      ParentFont = False
      Sorted = True
      TabOrder = 2
    end
    object ReplaceRadio: TsRadioButton
      Left = 255
      Top = 20
      Width = 110
      Height = 20
      Caption = #1047#1072#1084#1077#1085#1103#1090#1100' '#1076#1072#1085#1085#1099#1077
      TabOrder = 3
      SkinData.SkinSection = 'RADIOBUTTON'
    end
    object AppentRadio: TsRadioButton
      Left = 255
      Top = 1
      Width = 118
      Height = 20
      Caption = #1044#1086#1087#1086#1083#1085#1103#1090#1100' '#1076#1072#1085#1085#1099#1077
      Checked = True
      TabOrder = 4
      TabStop = True
      SkinData.SkinSection = 'RADIOBUTTON'
    end
    object TableCombo: TsComboBox
      Left = 147
      Top = 16
      Width = 98
      Height = 21
      Alignment = taLeftJustify
      BoundLabel.Active = True
      BoundLabel.Caption = #1058#1072#1073#1083#1080#1094#1072
      BoundLabel.Indent = 0
      BoundLabel.Font.Charset = DEFAULT_CHARSET
      BoundLabel.Font.Color = 2171169
      BoundLabel.Font.Height = -11
      BoundLabel.Font.Name = 'Tahoma'
      BoundLabel.Font.Style = []
      BoundLabel.Layout = sclTopLeft
      BoundLabel.MaxWidth = 0
      BoundLabel.UseSkinColor = True
      SkinData.SkinSection = 'COMBOBOX'
      Style = csDropDownList
      Color = clWhite
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 13
      ItemIndex = -1
      ParentFont = False
      Sorted = True
      TabOrder = 5
      OnChange = TableComboChange
    end
  end
  object sPanel3: TsPanel [3]
    Left = 0
    Top = 388
    Width = 578
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    ParentBackground = False
    TabOrder = 2
    SkinData.SkinSection = 'CHECKBOX'
    DesignSize = (
      578
      41)
    object MessageLabel: TsLabel
      Left = 121
      Top = 6
      Width = 367
      Height = 27
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      ParentFont = False
      WordWrap = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 2171169
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ExplicitWidth = 401
    end
    object Progress: TsGauge
      Left = 124
      Top = 8
      Width = 360
      Height = 25
      Anchors = [akLeft, akRight, akBottom]
      Visible = False
      SkinData.SkinSection = 'GAUGE'
      ForeColor = clBlack
      Progress = 0
      Suffix = '%'
      ExplicitWidth = 368
    end
    object ImportButton: TsButton
      Left = 8
      Top = 8
      Width = 105
      Height = 25
      Caption = #1048#1084#1087#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100
      TabOrder = 0
      OnClick = ImportButtonClick
      SkinData.SkinSection = 'BUTTON'
    end
    object CloseButton: TsButton
      Left = 494
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = #1042#1099#1093#1086#1076
      ModalResult = 2
      TabOrder = 1
      SkinData.SkinSection = 'BUTTON'
    end
  end
  object Pages: TsPageControl [4]
    Left = 0
    Top = 249
    Width = 578
    Height = 139
    ActivePage = RecordsTab
    Align = alClient
    HotTrack = True
    MultiLine = True
    TabOrder = 3
    TabWidth = 80
    SkinData.SkinSection = 'PAGECONTROL'
    object RecordsTab: TsTabSheet
      Caption = #1047#1072#1087#1080#1089#1080
      SkinData.CustomColor = False
      SkinData.CustomFont = False
      object RecordList: TsCheckListBox
        Left = 0
        Top = 0
        Width = 570
        Height = 111
        AutoCompleteDelay = 500
        BorderStyle = bsSingle
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
        Sorted = True
        Align = alClient
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = RecordListClick
      end
    end
    object ErrorsTab: TsTabSheet
      Caption = #1054#1096#1080#1073#1082#1080
      SkinData.CustomColor = False
      SkinData.CustomFont = False
      object ErrorsList: TsListBox
        Left = 0
        Top = 0
        Width = 570
        Height = 111
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
        Align = alClient
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = ErrorsListClick
      end
    end
    object MessagesTab: TsTabSheet
      Caption = #1057#1086#1086#1073#1097#1077#1085#1080#1103
      SkinData.CustomColor = False
      SkinData.CustomFont = False
      object MessagesMemo: TsMemo
        Left = 0
        Top = 0
        Width = 570
        Height = 111
        Align = alClient
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
        WantReturns = False
        WordWrap = False
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
      end
    end
  end
  inline ScriptEditFrame: TScriptEditFrame [5]
    Left = 0
    Top = 57
    Width = 578
    Height = 120
    HorzScrollBar.Visible = False
    VertScrollBar.Visible = False
    Align = alTop
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 4
    TabStop = True
    ExplicitTop = 57
    ExplicitWidth = 578
    ExplicitHeight = 120
    inherited Edit: TSynEdit
      Width = 578
      Height = 98
      Font.Height = -12
      Font.Pitch = fpFixed
      ReadOnly = True
      RightEdge = 512
      WantReturns = False
      WantTabs = False
      ExplicitWidth = 578
      ExplicitHeight = 98
    end
    inherited sToolBar1: TsToolBar
      Width = 578
      Visible = False
      ExplicitWidth = 578
    end
  end
end
